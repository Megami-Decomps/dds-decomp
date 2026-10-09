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

extern s32 func_003014F0(char *dst, const char *format, ...);

extern void effObjSetOpacityPassEnabled(u32 enabled);

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 effMiscRandMod(void *state, u32 modulus);

extern u64 btlStartTask(void *);

/* Battle camera cursor (0x130 bytes at D_0035F100): script cursor fields, then the camera path state
 * read by func_001EB368 (retail offsets noted). */
typedef struct BtlCameraCursor {
    u16 unk_00;
    s16 frame;              /* 0x02 */
    s16 mode;
    s16 index;
    union {
        u16 unk_08;
        s8 category;
    };
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
    u16 kind;               /* 0x10: path kind (jump table 0..8) */
    s8 phase;               /* 0x12 */
    s8 counter;             /* 0x13 */
    s8 busy;                /* 0x14: nonzero suspends the path step */
    u8 pad15[0xB];
    f32 pathEnd[4];         /* 0x20 */
    f32 pathStart[4];       /* 0x30 */
    f32 eyeFrom[4];         /* 0x40 */
    f32 eyeTo[4];           /* 0x50 */
    f32 focusFrom[4];       /* 0x60 */
    f32 focusTo[4];         /* 0x70 */
    f32 direction[4];       /* 0x80: captured camera direction */
    f32 distance;           /* 0x90 */
    u8 pad94[0xC];
    f32 fov;                /* 0xA0 */
    u8 padA4[0xC];
    f32 startFrame;         /* 0xB0 */
    u8 padB4[0xC];
    f32 duration;           /* 0xC0 */
    u8 padC4[0x2C];
    f32 blendRate;          /* 0xF0 */
    u8 padF4[0xC];
    f32 rangeStart;         /* 0x100 */
    u8 pad104[0xC];
    f32 rangeEnd;           /* 0x110 */
    u8 pad114[0x1C];
} BtlCameraCursor;

extern BtlCameraCursor D_0035F100;

/* Serialized camera instructions advance by 0x10 in 001E9DE0. */
typedef struct BtlCameraTimedInstruction {
    s32 kind;
    s16 parameterIndex;
    u8 pad06[2];
    f32 startFrame;
    f32 duration;
} BtlCameraTimedInstruction;

extern const BtlCameraTimedInstruction *D_0035D9F0[];

extern const BtlCameraTimedInstruction *D_0035DA08[];

extern void func_001E9DE0(BtlLinkedCommand *, BtlCamState *, const BtlCameraTimedInstruction *);

extern void func_001EB368(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void func_001E6260(BtlLinkedCommand *, BtlCamState *);

extern void func_001E6368(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceCommandCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlInitLinkedUnitActionCursor(BtlTask *);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

typedef struct SoundBankEntry {
    u32 unk_00;
    u32 resource;
    u32 unk_08;
} SoundBankEntry;

extern SoundBankEntry D_0035F748[];

extern void btlUnitGetMuzzlePosVU(void *);

/* SYSEFF metadata and runtime registrations share these indices. */
enum {
    BTL_SOUND_ENTRY_COUNT = 0x31,
    BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT = 0x2C
};

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

typedef struct SoundLink {
    BtlUnit *owner;
    BattleEffect *effectHandle;
    SoundResourceNode *effect;
    u16 variant;
    u16 unk_0E;
} SoundLink;

extern void btlUpdateUnitCommandEffect(SoundLink *);

typedef struct SoundResourceLink {
    BtlUnit *owner;
    BattleEffect *effectHandle;
    SoundResourceNode *effect;
    u32 variant;
    u8 refreshRequested;
    u8 pad11[3];
} SoundResourceLink;

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern char D_003A5158[]; /* "%sMIDI%04X.SMG" */

extern s32 sndFindPackedTrackLoadStatus(u32);

typedef struct SceneLightRestoreArgs { u32 value; } SceneLightRestoreArgs;

extern s64 func_001F0998(SceneLightRestoreArgs *);

extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);

extern s32 btlRestoreSceneTransformLighting(SceneLightRestoreArgs *);

extern s64 func_001F0B90(void);

extern s32 sndPlaySkillSeTask(u32 *);


extern u32 sndFinishEarringPlayback(void);

extern s32 sndTickFadeCounter();

extern s32 btlQueueTintTransitionWhenEnabled(u32 *);

extern void *btlCreateMoveOtherUnitsTask(u8 *, u32);

extern s32 mnuPollTitleStreamStateLocked(void);

extern SoundResourceNode *sndAllocResourceNode(void);

extern u32 kwlnDrawControlFlags;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 D_003BB694;

extern u32 btlTintTransitionHoldCount;

extern u8 D_0035F5D0[];

extern char D_003BB6B0[];

extern u32 dds3AdvanceWorldCounter(void);

extern struct EffWorldNode *evtSpawnActionObj9(s32);

extern s32 btlSetActorEffectParameter();

extern s32 mdlFlagTest(u32);

extern void func_001DEFE0(BtlLinkedCommand *, BtlCamState *, f32);

extern void func_001DF358(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlResetUnitEffectVector(BtlLinkedCommand *, BtlCamState *);

extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);

extern s32 btlGetRuntime(void);

extern s32 datActionAnimationRecords;

extern s32 btlArrangeFormationSlots(s32 arg0);

extern s8 effSharedRandomState[];

extern void sndResetTransition(void);

extern void func_001F4430(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

extern s8 effSharedRandomState[];

extern void sndSetStationedSeVolume(u32);

extern void btlResetTitleStreamOnBattleFlag(void);

extern void btlAdvanceWorldCounterAndSpawnActionObject(void);

extern void btlCreateRainEffect(u32, u32);

extern s32 btlRepositionPartyAroundBattleCenter(void);

extern void kwlnFadeStartIn(s32);

extern s32 btlAreWorkBuffersReady(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *entry);

struct EvtUnit;

extern s32 btlIsCurrentValueBelowQuarterThreshold(void *);

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

s32 btlGetLoggedIndexedCommandItem(s32 index);

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

extern s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *, BtlCamState *, s8, s8);

typedef struct BattleVoiceEntry {
    u8 volume;
    u8 pad01[3];
    char fileName[0xC];
} BattleVoiceEntry;

#define CURSOR (&D_0035F100)

extern void sndFreeResourceNode(SoundResourceNode *);

extern void sndFreeListNode(ActiveSoundNode *);

extern s32 btlRepositionPartyAroundBattleCenter(void);

extern SoundTask *sndCreateStationedSeTask(u32);

extern SoundTask *sndCreateStationedSeTask(u32);

extern s32 btlGetRuntime(void);

extern void *btlAllocTask(s32);

extern void *btlAllocTask(s32);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32, s32);

extern void btlMarkTaskReady(SoundResourceLink *);

extern void btlSetUnitPosition(BtlUnit *object, void *position);

extern void btlSetUnitRotation(BtlUnit *object, void *rotation);

extern s32 sdfLoadMapRecordPositionVector(SdfModel *, s32);

extern void mdlLoadPrimaryVectorVU(MdlCtx *);

extern void effMiscQuatMultiplyVU(void);

extern void btlUnitGetBodyPosVU(u8 *);

extern s32 btlAimHorizontalDirectionVU(void *, void *);

extern s32 btlAimHorizontalDirectionClampedVU(void *, void *, f32);

extern u8 D_0037E110[];

extern void effMiscQuaternionToMatrixVU(void);

extern void effMiscQuaternionToMatrixVU(void);

extern void func_00160D88(BattleEffect *);

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

extern void effMiscQuaternionToMatrixVU(void);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];

extern s32 btlGetRuntime(void);

extern f32 btlUnitGetTopY(BtlUnit *);

/* With a qualifying unit, the span query leaves its last muzzle point in VF10. */
extern f32 func_001F66D8(s32, f32 *, f32 *);

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001E4708(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001E4720(BtlCamState *, BtlCamState *, BtlCamState *);

extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);

extern void btlInitializeCameraCursorForActorMode(BtlLinkedCommand *, BtlCamState *);

extern void func_001E2FF8(BtlLinkedCommand *);

extern void btlSetupActionCameraPair(BtlLinkedCommand *);

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern void func_001E0100();

extern void func_001E5718(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

/* Test whether the operand is empty, subject to command-category and slot-kind exclusions. */ s32 btlActionEntryIsEmpty(s32 index, BtlOperandGroup *slot, BtlOperandEntry *entry);

s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state);

/* Adjust the pose direction using the supplied horizontal height plane. */ s32 btlAdjustCameraDirectionForPlane(BtlCamState *state, f32 height);

void btlAimLinkedUnitAtMuzzle(u8 *action);

/* Allocate a native header followed by capacity pointer entries. */ BtlIndexList *btlAllocateIndexList(s32 capacity);

/* The caller keeps the live count within the allocated capacity. */ void btlAppendIndexListEntry(BtlIndexList *list, void *entry);

void btlApplyCombinedActorFlags(u8 *resource);

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode);

u32 btlCanUseActorCategoryFlag2(BtlLinkedCommand *actor);

/* Check actor status, linked-group marks, and owner restrictions before use. */ s32 btlCanUseLinkedActor(BtlLinkedCommand *actor);

void btlClearAllUnitDefeatCandidatesTask(void);

void btlClearUnitDefeatCandidate(BtlUnit *object);

void btlCopyMotionTransform(BtlCamState *dst, BtlCamState *src);

void btlCopyUnitRotationQuaternion(BtlUnit *unit, void *dst);

/* Return the oldest matching handle, or zero; unstarted tasks may have handle 0. */ s32 btlFindTaskByHandle(s64 handle);

void btlFlagLinkedGroupDefeatCandidatesTask(s32 arg0);

void btlFlagUnitDefeatCandidate(BtlUnit *unit);

void btlFreeIndexList(BtlIndexList *list);

u32 btlGetActiveUnitId(void);

/* Return the number of live entries, not the allocated capacity. */ u32 btlGetIndexListCount(BtlIndexList *list);

/* The caller supplies an in-range index. */ void *btlGetIndexListEntry(BtlIndexList *list, s32 index);

/* Return the argument address recorded by allocation (zero for no arguments). */ void *btlGetTaskArguments(void *task);

s32 btlGetUnitModelFrameCount(BtlUnit *unit);

f32 btlGetUnitModelValue1C(BtlUnit *unit);

s32 btlHasActorCategoryFlag100(BtlLinkedCommand *action);

/* Find a type-two linked group whose associated unit is not disabled. */ s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor);

s32 btlHasFirstLinkedCategoryFlag1000(BtlLinkedCommand *node);

/* Find an unmarked linked kind-two slot whose unit is not disabled. */ s32 btlHasIdleLinkedSlotKindTwo(u8 *actor);

/* Return whether a live linked group has its byte at 0x10 marked. */ u32 btlHasMarkedEntry10(u8 *object);

void btlInterpolateVectorStep(f32 *src);

void btlReleaseUnitModelColorResource(BtlUnit *unit, s32 value, f32 scalar);

void btlSetActorEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode);

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode);

void btlUnitFaceTarget(BtlUnit *object, BtlUnit *target);

void btlUnitFaceTargetScaled(BtlUnit *object, BtlUnit *target, f32 scale);

void func_001D6300(u8 *object, void *position);

/* vu0 routine: preserve the actor's primary position and rotation quaternion while * evaluating the requested model record; return the sampled vector in vf10. */ s32 func_001D6428(BtlUnit *unit, s32 value);

u32 func_001DD2C0(BtlLinkedCommand *actor);

u8 func_001DD488(BtlLinkedCommand *action);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001DDF20);

void btlAdvanceTargetCursorUnlessHookHandles(BtlLinkedCommand *actor) {
    s32 (*callback)(BtlLinkedCommand *) = *(s32 (**)(BtlLinkedCommand *))(btlGetRuntime() + 0x634);
    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    if (actor->stepKind == 12) {
        btlAdvanceTargetCursorAnimation(actor, &actor->camera);
    }
}

extern void func_001E3E58(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

void btlUpdateActionPoseForLinkedTarget(BtlLinkedCommand *action) {
    BtlUnit *target;

    if (((BtlState *)btlGetRuntime())->unk_1FC & 1) {
        target = action->task->unit;
        if (target->status.flags & 0x400) {
            func_001DF358(action, &action->camera);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform(&action->frontCamera, &action->camera);
        func_001E3E58(action, &action->backCamera, action->task->unit, 0);
        action->motionParameter = 7.0f;
        action->flags = (action->flags | 0x1041) & 0xFFFBFFFF;
    } else {
        func_001E3E58(action, &action->camera, action->task->unit, 0);
    }
}

void func_001DE5F0(void) {
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001DE5F8);

void func_001DE958(void) {
}

extern void btlSetupCameraAimDualUnit(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);

void func_001DE960(BtlLinkedCommand *action) {
    BtlUnit *target;
    s32 kind;
    f32 pos[4];
    if (btlGetIndexListCount(action->targetList) != 1) {
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (action->task->unit->status.flags & 0x200) {
        func_001E3E58(action, &action->camera, target, 0);
        return;
    }
    if (btlHasActorCategoryFlag100(action) != 0) {
        return;
    }
    btlUnitGetBodyPosVU(target);
    VU0_STORE_VF(vf10, pos);
    if (pos[0] > 0.0f) {
        kind = 2;
    } else {
        kind = 3;
    }
    btlSetupCameraAimDualUnit(action, &action->frontCamera, kind, 45.0f, 0.25f);
    btlSetupCameraAimDualUnit(action, &action->backCamera, kind, 1.0f, 0.5f);
    action->motionParameter = 30.0f;
    action->flags |= 0x41;
}

void func_001DEA68(void) {
}

void btlStartLinkedActionPoseBlendIfEligible(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (action->task->unit->status.flags & 0x400) {
        if (work->cameraPoseBlendHook != 0) {
            s32 hasFlag200 = 0;
            s32 hasFlag400 = 0;
            u32 i;
            u32 count = btlGetIndexListCount(action->task->indexWork.indices);
            for (i = 0; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(action->task->indexWork.indices, i);
                if (entry->status.flags & 0x200) {
                    hasFlag200 = 1;
                }
                if (entry->status.flags & 0x400) {
                    hasFlag400 = 1;
                }
            }
            if (work->cameraPoseBlendHook(action, hasFlag200, hasFlag400) != 0) {
                action->flags |= 0x10000;
                return;
            }
        }
        btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
        action->motionParameter = 200.0f;
        action->flags |= 0x10041;
    } else {
        func_001E5800(action, action);
    }
}

void btlAdvanceUnblockedPlayerCursorAnimation(BtlLinkedCommand *action) {
    if ((action->flags & 0x10000) != 0) {
        return;
    }
    btlAdvancePlayerCursorAnimation(action, &action->camera);
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001DEBE0);

void func_001DEDB8(BtlLinkedCommand *action) {
    if ((action->flags & 0x10000) != 0) {
        return;
    }
    func_001E6260(action, &action->camera);
}

void func_001DEDE8(BtlLinkedCommand *action) {
    func_001E6368(action, &action->camera);
}

void btlAdvanceCommandCursorTask(BtlLinkedCommand *action) {
    btlAdvanceCommandCursor(action, &action->camera);
}

extern void func_001E4AC0(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void func_001DEE18(BtlLinkedCommand *action) {
    func_001E4AC0(action, &action->frontCamera, &action->backCamera);
}

void func_001DEE38(void) {
    func_001E4E50();
}

extern void func_001E5198(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlStartLinkedDefeatCandidateAction(BtlLinkedCommand *action) {
    BtlTask *task = action->task;
    btlAppendIndexListEntry(action->targetList, task->unit);
    func_001E5198(action, &action->frontCamera, &action->backCamera);
    btlClearAllUnitDefeatCandidates();
    task = action->task;
    btlFlagUnitDefeatCandidate(task->unit);
    action->motionParameter = 50.0f;
    action->flags |= 0x41;
}

void func_001DEEC0(void) {
}

extern void btlBuildGroupFramingCameraPose(BtlCamState *, BtlCamState *);

void btlUpdateLinkedActionEffectVectorByTarget(BtlLinkedCommand *action) {
    if ((action->task->unit->status.flags & 0x200) != 0) {
        btlBuildGroupFramingCameraPose(&action->camera, &action->camera);
        return;
    }
    if (action->actionKind != 0x10) {
        btlResetUnitEffectVector(action, &action->camera);
        return;
    }
}

void func_001DEF20(void) {
}

void btlInitializeCursorForLinkedAction(BtlLinkedCommand *action) {
    if (action->task != 0) {
        btlInitLinkedUnitActionCursor(action->task);
        return;
    }
}

void func_001DEF58(void) {
}

s32 func_001DEF60(s32 actor) {
    s32 context = btlGetRuntime();
    s32 (*callback)(s32) = *(void **)(context + 0x618);
    if (callback != 0) {
        return callback(actor);
    }
    return 0;
}

s32 func_001DEFA0(s32 actor) {
    s32 context = btlGetRuntime();
    s32 (*callback)(s32) = *(void **)(context + 0x620);
    if (callback != 0) {
        return callback(actor);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001DEFE0);

void func_001DF358(BtlLinkedCommand *command, BtlCamState *pose) {
    func_001DEFE0(command, pose, 27.5f);
}

extern void func_002DD688(f32 angle);

void btlPrepareUnitPoseWithTiltRotation(command, pose, out)
BtlLinkedCommand *command;
BtlCamState *pose;
BtlCamState *out;
{
    func_001DEFE0(command, pose, 20.0f);
    btlCopyMotionTransform(out, pose);
    if (pose->direction[0] > 0.0f) {
        func_002DD688(-(20.0f * 0.017453293f));
    } else {
        func_002DD688(20.0f * 0.017453293f);
    }
    VU0_STORE_VF(vf10, pose->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
}

extern void btlClearAllUnitDefeatCandidates(void);

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern f32 func_002FA148(f32);

typedef struct BattlePairCameraPreset {
    f32 fromQuaternion[4];
    f32 toQuaternion[4];
    f32 fromDistanceScale;
    f32 toDistanceScale;
    f32 fromHeightScale;
    f32 motionParameter;
} BattlePairCameraPreset;

void btlConfigureBattleCameraAction(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quaternion[4];
    BattlePairCameraPreset poses[4] = {
        {{0x1.be76c8p-4f, -0x1.be76c8p-1f, -0x1.26e978p-2f, 0x1.6b851ep-2f},
         {0x1.47ae14p-7f, -0x1.f0a3d6p-1f, 0x1.eb851ep-6f, -0x1.1eb85p-3f},
         2.5f, 1.75f, 0.5f, 25.0f},
        {{-0x1.47ae14p-4f, -0x1.d1eb84p-1f, 0x1.47ae14p-3f, 0x1.5c28f4p-2f},
         {-0x1.999998p-5f, -0x1.eb851ep-1f, -0x1.47ae14p-3f, -0x1.1eb85p-3f},
         2.5f, 1.75f, 0x1.333332p-2f, 25.0f},
        {{-0x1.70a3d6p-4f, -0x1.c28f5cp-1f, -0x1.333332p-3f, -0x1.b851eap-2f},
         {-0x1.47ae14p-6f, -0x1.f5c28ep-1f, 0x1.eb851ep-6f, 0x1.c28f5cp-4f},
         2.5f, 1.75f, 0.5f, 25.0f},
        {{0x1.47ae14p-5f, -0x1.dc28f4p-1f, 0x1.eb851ep-4f, -0x1.3d70a2p-2f},
         {0x1.47ae14p-7f, -0x1.f0a3d6p-1f, -0x1.47ae14p-3f, 0x1.eb851ep-5f},
         2.5f, 1.75f, 0x1.333332p-2f, 25.0f},
    };
    BtlUnit *unit = action->task->unit;
    s32 pose;
    f32 fov;
    f32 halfFov;
    f32 tangent;
    f32 distance;

    if (unit->status.flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        pose = effMiscRandMod(0, 4);
        fov = action->camera.fov;
        from->fov = fov;
        to->fov = fov;
        if (!btlIsCurrentValueBelowQuarterThreshold((void *)unit) || !(unit->status.flags & 0x200)) {
            if (func_001D6428(unit, 1) == 0) {
                btlUnitGetMuzzlePosVU(unit);
            }
            VU0_STORE_VF(vf10, from->position);
            VU0_STORE_VF(vf10, to->position);
        } else {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF(vf10, from->position);
            VU0_STORE_VF(vf10, to->position);
            from->position[1] -= unit->height * unit->scale * 0.25f;
            to->position[1] -= unit->height * unit->scale * 0.25f;
        }

        from->position[1] *= poses[pose].fromHeightScale;
        halfFov = fov * 0.5f;
        tangent = func_002FA148(halfFov);
        distance = unit->cameraRadius * unit->scale / tangent;
        from->distance = distance * poses[pose].fromDistanceScale;
        to->distance = distance * poses[pose].toDistanceScale;

        VU0_LOAD_VF(vf10, poses[pose].fromQuaternion);
        VU0_LOAD_VF(vf11, quaternion);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, from->direction);

        VU0_LOAD_VF(vf10, poses[pose].toQuaternion);
        VU0_LOAD_VF(vf11, quaternion);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);

        btlAdjustCameraDirectionForDefaultPlane(from);
        btlAdjustCameraDirectionForDefaultPlane(to);
        if (btlHasMarkedEntry10((u8 *)action)) {
            action->durationFrames = func_001D6050(unit, unit->unkEC);
            action->flags |= 0x11;
        } else {
            action->flags |= 0x41;
            action->motionParameter = poses[pose].motionParameter;
        }
    }
}

void btlPrepareRandomizedActionCameraPose(BtlLinkedCommand *action, BtlCamState *from,
                                           BtlCamState *to) {
    f32 quat[4];
    /* Quaternion rows, distance multiplier, camera parameter, and padding. */
    f32 poses[4][12] = {
        {0.0f, -0.94f, 0.02f, 0x1.333332p-2f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
        {0.06f, -0.94f, -0x1.70a3d6p-3f, 0.25f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
        {0.0f, -0.94f, 0.02f, -0x1.333332p-2f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
        {-0.06f, -0.94f, -0x1.70a3d6p-3f, -0.25f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
    };
    BtlUnit *unit = action->task->unit;
    u32 flags = unit->status.flags;
    s32 pose;
    f32 fov;
    f32 half;
    f32 span;
    f32 distance;

    if (flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(flags & 0x600);
        btlCopyUnitRotationQuaternion(unit, quat);
        pose = effMiscRandMod(0, 4);
        fov = action->camera.fov;
        from->fov = fov;
        to->fov = fov;
        span = func_001F66D8(flags & 0x600, 0, 0) * 1.25f;
        VU0_STORE_VF(vf10, from->position);
        if (func_001D6428(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU((u8 *)unit);
        }
        VU0_STORE_VF(vf10, to->position);
        VU0_LOAD_VF(vf11, from->position);
        VU0_LERP_VF10(0.5f);
        VU0_STORE_VF(vf10, from->position);
        half = fov * 0.5f;
        distance = span / func_002FA148(half);
        from->distance = distance;
        distance = unit->cameraRadius * unit->scale / func_002FA148(half);
        to->distance = distance * poses[pose][8];
        VU0_LOAD_VF(vf10, poses[pose]);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, from->direction);
        VU0_LOAD_VF(vf10, &poses[pose][4]);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);
        btlAdjustCameraDirectionForDefaultPlane(from);
        btlAdjustCameraDirectionForDefaultPlane(to);
        action->motionParameter = poses[pose][9];
        action->flags |= 0x41;
    }
}

void btlAimEffectPoseAtUnit(BtlLinkedCommand *actor) {
    BtlUnit *object = actor->task->unit;
    if ((object->status.flags & 2) != 0) {
        if (btlSetActorEffectParameter((u8 *)object, 1) == 0) {
            btlUnitGetMuzzlePosVU(object);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, actor->backCamera.position);
        btlAdjustCameraDirectionForDefaultPlane(&actor->backCamera);
    }
}

extern void func_001E1288(BtlLinkedCommand *, BtlCamState *, u8);

extern void func_001E16C0(BtlLinkedCommand *, BtlCamState *);

extern s32 func_001D6050(BtlUnit *, s32);

/* One aim pose: quaternion, distance multiplier, height scale and two words this aim leaves unused. */
typedef struct CameraAimPose {
    f32 quat[4];
    f32 distanceScale;
    f32 heightScale;
    f32 unk18;
    f32 unk1C;
} CameraAimPose;

/* Build the from pose, then aim the to pose at the unit's muzzle using the pose row
 * for the side of the from view the muzzle lies on. */
void func_001DFAE0(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from) {
    f32 muzzle[4];
    f32 quat[4];
    CameraAimPose poses[2] = {
        {{0x1.70a3d6p-3f, -0x1.999998p-2f, -0x1.70a3d6p-4f, 0.89f}, 3.0f, 0x1.999998p-1f, 30.0f, 0.0f},
        {{0x1.70a3d6p-3f, 0x1.999998p-2f, 0x1.70a3d6p-4f, 0.89f}, 3.0f, 0x1.999998p-1f, 30.0f, 0.0f},
    };
    BtlUnit *unit = action->task->unit;
    s32 pose;
    f32 distance;
    f32 fov;

    if (unit->status.flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
        if (btlHasSingleLinkedResource(action) == 0) {
            func_001E16C0(action, from);
        } else {
            func_001E1288(action, from, 1);
        }
        btlCopyUnitRotationQuaternion(unit, quat);
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF(vf10, muzzle);
        VU0_LOAD_VF(vf10, muzzle);
        VU0_LOAD_VF(vf11, from->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(distance);
        if (muzzle[0] < from->direction[0] * -distance + from->position[0]) {
            pose = 0;
        } else {
            pose = 1;
        }
        fov = action->camera.fov;
        to->fov = fov;
        if (func_001D6428(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF(vf10, to->position);
        to->position[1] *= poses[pose].heightScale;
        distance = unit->cameraRadius * unit->scale / func_002FA148(fov * 0.5f);
        to->distance = distance * poses[pose].distanceScale;
        VU0_LOAD_VF(vf10, poses[pose].quat);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);
        btlAdjustCameraDirectionForDefaultPlane(to);
        action->durationFrames = func_001D6050(unit, unit->unkEC);
        action->flags |= 0x815;
        action->motionProgress = 0;
    }
}

void btlRefreshActionPoseBlendSnapshot(BtlLinkedCommand *action) {
    BtlCamState *saved;
    if (!(action->flags & 1) && action->motionProgress == 0) {
        saved = &action->backCamera;
        btlCopyMotionTransform(&action->frontCamera, &action->camera);
        btlCopyMotionTransform(saved, &action->camera);
        action->backCamera.distance += 125.0f;
        action->flags = (action->flags & ~0x14) | 0x41;
        action->state = 0;
        action->motionProgress = 1;
        action->motionParameter = 40.0f;
        btlAdjustCameraDirectionForDefaultPlane(saved);
    }
}

void func_001DFE28(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from) {
    f32 targetPosition[4];
    f32 rotation[4];
    f32 bodyPosition[4];
    f32 center[4];
    BtlUnit *unit = action->task->unit;
    BtlUnit *target;
    u32 count;
    u32 i;
    u32 targetFlags;
    f32 halfFov;
    f32 minimumDistance;

    if (unit->status.flags & 2) {
        btlFlagAllUnitsDefeatCandidate();
        count = btlGetIndexListCount(action->targetList);
        if (btlHasSingleLinkedResource(action) == 0) {
            func_001DEFE0(action, from, 17.5f);
            btlCopyMotionTransform(to, from);
            btlInterpolateVectorStep(from->position);
            VU0_STORE_VF(vf10, targetPosition);
            if (func_001D6428(unit, 1) == 0) {
                btlUnitGetMuzzlePosVU(unit);
            }
            VU0_STORE_VF(vf10, to->position);
            VU0_LOAD_VF(vf11, targetPosition);
            VU0_SUB(vf10, vf10, vf11);
            VU0_SET_VF10_COMPONENT(x, 0.0f);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, to->direction);
            halfFov = to->fov * 0.5f;
            minimumDistance = unit->cameraRadius * unit->scale / func_002FA148(halfFov);
            to->distance *= 0.5f;
            if (to->distance < minimumDistance) {
                to->distance = minimumDistance;
            }
            from->distance += 150.0f;
            if (unit->status.flags & 0x80000) {
                btlUnitGetBodyPosVU(unit);
                VU0_STORE_VF_UNCLOBBERED(vf10, bodyPosition);
                targetFlags = 0;
                for (i = 0; i < count; i++) {
                    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, i);
                    targetFlags |= target->status.flags & 0x600;
                }
                func_001F66D8(targetFlags, 0, 0);
                VU0_STORE_VF_UNCLOBBERED(vf10, center);
                if (btlAimHorizontalDirectionVU(bodyPosition, center) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
                    btlSetUnitRotation(unit, rotation);
                }
            }
        } else {
            target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
            func_001E1288(action, from, 2);
            halfFov = to->fov * 0.5f * 1.3333333f;
            btlCopyMotionTransform(to, from);
            minimumDistance = target->reach * target->scale / func_002FA148(halfFov);
            to->distance *= 0.6f;
            if (to->distance < minimumDistance) {
                to->distance = minimumDistance;
            }
            from->distance += 25.0f;
        }
        btlAdjustCameraDirectionForDefaultPlane(to);
        btlAdjustCameraDirectionForDefaultPlane(from);
        action->motionParameter = 20.0f;
        action->flags |= 0x845;
        action->motionProgress = 0;
    }
}

extern f32 btlGetActorEffectScale(BtlTask *);

void func_001E0100(BtlLinkedCommand *action, BtlCamState *pose) {
    BtlUnit *user;
    BtlUnit *target;
    s32 frames;
    s32 idle;
    s32 eligible;

    if (action->motionProgress == 0 && (s32)btlGetIndexListCount(action->targetList) < 2) {
        user = action->task->unit;
        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
        frames = func_001D6050(user, user->unkEC);
        frames = (s32)((f32)frames / btlGetActorEffectScale(action->task));
        if (action->state == frames && (target->status.flags & 0x200)) {
            idle = btlHasIdleLinkedSlotKindTwo((u8 *)action);
            eligible = btlHasEligibleLinkedEntryTypeTwo((u8 *)action);
            if (idle == 0 && eligible == 0) {
                return;
            }
            btlCopyMotionTransform(&action->frontCamera, pose);
            btlCopyMotionTransform(&action->backCamera, pose);
            action->backCamera.distance += idle != 0 ? 500.0f : 300.0f;
            action->motionProgress = 1;
            action->motionParameter = 10.0f;
            action->flags = (action->flags & ~0x14) | 0x41;
            action->state = 0;
            btlAdjustCameraDirectionForDefaultPlane(&action->backCamera);
        }
    }
}

extern void btlClearAllUnitDefeatCandidates(void);

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlSetupCameraPoseAimUnit(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quat[4];
    BtlUnit *unit = action->task->unit;
    f32 fov;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
    btlCopyUnitRotationQuaternion(unit, quat);
    fov = action->camera.fov;
    from->fov = fov;
    if (func_001D6428(unit, 1) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_STORE_VF(vf10, &from->position);
    from->distance = unit->cameraRadius * unit->scale / func_002FA148(fov * 0.5f);
    VU0_LOAD_VF(vf10, quat);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_NEGATE_XYZ(vf10);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, &from->direction);
    btlCopyMotionTransform(to, from);
    to->distance += 550.0f;
    action->flags = (action->flags & ~0x14) | 0x41;
    action->motionParameter = 25.0f;
    btlAdjustCameraDirectionForDefaultPlane(from);
    btlAdjustCameraDirectionForDefaultPlane(to);
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E0398);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E0718);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E0B68);

extern u32 effMiscRand(void *state);

void btlSetupCameraAimDualUnit(BtlLinkedCommand *command, BtlCamState *pose, s32 modeBits,
f32 angle, f32 blend) {
    f32 point[4];
    f32 plane[4];
    f32 firstPosition[4];
    f32 lastPosition[4];
    u8 mode = (u8)modeBits;
    BtlState *runtime;
    BtlUnit *unit;
    BtlUnit *first;
    BtlUnit *last;
    u32 i = 0;
    u32 count;
    u32 groups = 0;
    f32 totalReach;
    f32 fov;
    f32 length;
    f32 clearance;
    f32 distance;
    f32 projected;

    runtime = (BtlState *)btlGetRuntime();
    count = btlGetIndexListCount(command->targetList);
    for (; i < count; i++) {
        groups |= ((BtlUnit *)btlGetIndexListEntry(command->targetList, i))->status.flags & 0x600;
    }
    btlClearAllUnitDefeatCandidates();
    first = NULL;
    btlFlagMatchingUnitsDefeatCandidate(groups & 0x600);
    totalReach = 0.0f;
    last = NULL;
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        s32 flags = unit->status.flags;
        if (!(flags & 1)) {
            continue;
        }
        if (flags & 0xC0) {
            continue;
        }
        if (!(flags & groups)) {
            continue;
        }
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF(vf10, point);
        totalReach += unit->reach * unit->scale;
        if (first == NULL) {
            first = unit;
            VU0_STORE_VF_UNCLOBBERED(vf10, firstPosition);
            last = unit;
            VU0_STORE_VF(vf10, lastPosition);
        } else {
            if (unit->status.flags & 0x200) {
                f32 x = point[0];
                if (x < firstPosition[0]) {
                    first = unit;
                    PCP_COPY_VECTOR_F32(firstPosition, point);
                }
                if (lastPosition[0] < x) {
                    last = unit;
                    PCP_COPY_VECTOR_F32(lastPosition, point);
                }
            } else {
                f32 x = point[0];
                if (firstPosition[0] < x) {
                    first = unit;
                    PCP_COPY_VECTOR_F32(firstPosition, point);
                }
                if (x < lastPosition[0]) {
                    last = unit;
                    PCP_COPY_VECTOR_F32(lastPosition, point);
                }
            }
        }
    }
    if (first == NULL || last == NULL) {
        func_001DF358(command, pose);
        return;
    }
    if (mode == 3 || (mode == 1 && (effMiscRand(effSharedRandomState) & 1))) {
        unit = first;
        first = last;
        last = unit;
        VU0_LOAD_VF(vf10, firstPosition);
        VU0_LOAD_VF(vf11, lastPosition);
        VU0_STORE_VF(vf11, firstPosition);
        VU0_STORE_VF(vf10, lastPosition);
    }
    fov = command->camera.fov;
    pose->fov = fov;
    projected = func_001F66D8(groups, NULL, NULL);
    VU0_LOAD_VF(vf11, firstPosition);
    VU0_LERP_VF10(blend);
    VU0_STORE_VF(vf10, pose->position);
    fov *= 2.0f / 3.0f;
    if (totalReach <= projected) {
        f32 firstReach = first->reach * first->scale;
        f32 otherReach = last->reach * last->scale;
        f32 firstHeight;
        f32 otherHeight;
        f32 height;
        projected = otherReach < firstReach ? firstReach : otherReach;
        firstHeight = first->height * first->scale;
        otherHeight = last->height * last->scale;
        height = otherHeight < firstHeight ? firstHeight : otherHeight;
        height *= 0.5f;
        clearance = height < projected ? projected : height;
    } else {
        clearance = projected + (totalReach - projected) * 0.2f;
        pose->position[1] -= first->height * first->scale * 0.15f;
    }
    firstPosition[2] = pose->position[2];
    PCP_COPY_VECTOR_F32(plane, firstPosition);
    if (groups & 0x200) {
        f32 radians = angle * 0.017453293f;
        plane[2] += clearance;
        projected = pose->position[0];
        projected -= firstPosition[0];
        projected = ffabsf(projected);
        projected *= func_002FA148(radians);
        plane[2] += projected;
    } else {
        f32 radians = angle * 0.017453293f;
        plane[2] -= clearance;
        projected = pose->position[0];
        projected -= firstPosition[0];
        projected = ffabsf(projected);
        projected *= func_002FA148(radians);
        plane[2] -= projected;
    }
    projected = btlProjectOnPlaneVU(pose->position, plane, firstPosition);
    VU0_STORE_VF(vf10, point);
    projected += clearance;
    VU0_LOAD_VF(vf10, pose->position);
    VU0_LOAD_VF(vf11, point);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, pose->direction);
    distance += projected / func_002FA148(fov);
    projected = btlProjectOnPlaneVU(plane, pose->position, lastPosition);
    VU0_STORE_VF(vf10, point);
    projected += clearance;
    {
        f32 secondDistance = projected / func_002FA148(fov);
        VU0_LOAD_VF(vf10, point);
        VU0_LOAD_VF(vf11, pose->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        secondDistance -= length;
        pose->distance = distance < secondDistance ? secondDistance : distance;
    }
    btlAdjustCameraDirectionForDefaultPlane(pose);
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E1288);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E16C0);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E1CF8);

void btlActionAimUserAtTargets(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out) {
    s128 vec[3];
    BtlUnit *unit = command->task->unit;
    u32 mask = 0;
    u32 i;
    u32 count;
    btlPrepareUnitPoseWithTiltRotation(command, pose, out);
    count = btlGetIndexListCount(command->targetList);
    for (i = 0; i < count; i++) {
        mask |= ((BtlUnit *)btlGetIndexListEntry(command->targetList, i))->status.flags & 0x600;
    }
    if (unit->status.flags & 0x80000) {
        func_001F66D8(mask, 0, 0);
        VU0_STORE_VF(vf10, &vec[0]);
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU(&vec[1], &vec[0]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation(unit, &vec[2]);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E20C0);

extern void func_001E20C0(BtlLinkedCommand *, BtlCamState *, s32, f32 *, f32, f32);

extern f32 D_003A4310[4];

/* Camera framing preset: from and to quaternions, their distance multipliers,
 * the camera parameter copied to the action, and padding. */
typedef struct CameraFramePose {
    f32 fromQuat[4];
    f32 toQuat[4];
    f32 fromDistance;
    f32 toDistance;
    f32 parameter;
    f32 pad2C;
} CameraFramePose;

extern CameraFramePose D_003A4320[4];

/* Frame the from and to poses around the action's targets from a random preset row
 * (rows 2-3 for animations with flag 0x200); targets in group 0x400 mirror the presets. */
void func_001E2578(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 direction[4];
    CameraFramePose poses[4];
    u16 animationFlags;
    f32 scale;
    f32 distance;
    s32 pose;
    u32 count;
    u32 i;
    u32 groups;

    memcpy(poses, D_003A4320, sizeof(poses));
    if ((u32)(action->actionCode - 1) < 0x1FF) {
        animationFlags = ((BtlActionAnimationRecord *)datActionAnimationRecords)[action->actionCode].flags;
    } else {
        animationFlags = 0;
    }
    scale = (animationFlags & 0x200) ? 320.0f : 300.0f;
    if (animationFlags & 0x200) {
        pose = effMiscRandMod(0, 2) + 2;
    } else {
        pose = effMiscRandMod(0, 2);
    }
    groups = 0;
    count = btlGetIndexListCount(action->targetList);
    for (i = 0; i < count; i++) {
        groups |= ((BtlUnit *)btlGetIndexListEntry(action->targetList, i))->status.flags & 0x600;
    }
    VU0_LOAD_VF(vf10, poses[pose].fromQuat);
    if (groups & 0x400) {
        VU0_LOAD_VF(vf11, D_003A4310);
        effMiscQuatMultiplyVU();
    }
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, direction);
    distance = scale * poses[pose].fromDistance;
    func_001E20C0(action, from, groups, direction, distance, 0.0f);
    VU0_LOAD_VF(vf10, poses[pose].toQuat);
    if (groups & 0x400) {
        VU0_LOAD_VF(vf11, D_003A4310);
        effMiscQuatMultiplyVU();
    }
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, direction);
    distance = scale * poses[pose].toDistance;
    if (animationFlags & 0x200) {
        func_001E20C0(action, to, groups, direction, distance, 0.0f);
    } else {
        func_001E20C0(action, to, groups, direction, distance, -0x1.333332p-3f);
    }
    action->flags |= 0x41;
    action->motionParameter = poses[pose].parameter;
}

void btlFlagUserAndTargetDefeat(BtlLinkedCommand *command, BtlLinkedCommand *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = command->task->unit;
    target = btlGetIndexListEntry(command->targetList, 0);
    if (!(user->status.flags & target->status.flags & 0x600)) {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagMatchingUnitsDefeatCandidate(target->status.flags & 0x600);
    } else {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagUnitDefeatCandidate(target);
    }
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF_UNCLOBBERED(vf10, userPos);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPos);
    btlUnitFaceTarget(target, user);
    if (userPos[0] < targetPos[0]) {
        command->flags |= 0x200;
    } else {
        command->flags &= ~0x200;
    }
}

extern f32 func_002F9F60(f32);

extern f32 func_002FA060(f32);

#define BTL_APPROACH_DIST_START 0.55f

#define BTL_APPROACH_DIST_END 0.6f

#define BTL_APPROACH_PITCH_START 0.6108652f /* 35 degrees */

#define BTL_APPROACH_PITCH_END 0.6108652f

/* vu0 routine: */
void btlBuildApproachCamera(BtlLinkedCommand *action, BtlCamState *out) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 dir[4];
    f32 extent;
    f32 length;
    f32 span;
    f32 ratio;
    f32 factor;
    f32 angle;
    f32 width;

    user = action->task->unit;
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    extent = user->reach * user->scale;
    span = func_001D6050(user, user->unkEC);
    span /= btlGetActorEffectScale(action->task);
    ratio = (f32)action->state / span;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    out->fov = action->camera.fov;
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF(vf10, userPos);
    userPos[1] -= user->height * user->scale * 0.25f;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPos);
    VU0_LOAD_VF(vf11, userPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    factor = ratio * (BTL_APPROACH_DIST_END - BTL_APPROACH_DIST_START);
    factor += BTL_APPROACH_DIST_START;
    length *= factor;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    VU0_SCALE_VF_MFC1(vf10, length);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out->position);
    angle = ratio * (BTL_APPROACH_PITCH_END - BTL_APPROACH_PITCH_START);
    angle += BTL_APPROACH_PITCH_START;
    width = length * func_002FA060(angle);
    length *= func_002F9F60(angle);
    length += (extent + width) / func_002FA148(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (action->flags & 0x200) {
        angle = -angle;
    }
    func_002DD688(angle);
    VU0_LOAD_VF(vf10, dir);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
    btlAdjustCameraDirectionForPlane(out, -10.0f);
}

void btlSetupActionCameraPair(BtlLinkedCommand *command) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 lookPos[4];

    user = command->task->unit;
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    if (!(user->status.flags & target->status.flags & 0x600)) {
        btlFlagAllUnitsDefeatCandidate();
    } else {
        func_001DF358(command, &command->camera);
        return;
    }
    func_001E1CF8(command, (f32 *)&command->frontCamera);
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF_UNCLOBBERED(vf10, userPos);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPos);
    if (userPos[0] < targetPos[0]) {
        command->flags |= 0x200;
    } else {
        command->flags &= ~0x200;
    }
    command->flags |= 0x41;
    command->motionProgress = 0;
    command->motionParameter = 15.0f;
    btlInterpolateVectorStep(command->frontCamera.position);
    VU0_STORE_VF_UNCLOBBERED(vf10, lookPos);
    btlUnitGetMuzzlePosVU(user);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, command->frontCamera.position);
    VU0_LERP_VF10(0.25f);
    VU0_STORE_VF(vf10, command->frontCamera.position);
    VU0_LOAD_VF(vf11, lookPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, command->frontCamera.direction);
    btlUnitFaceTarget(user, target);
}

/* vu0 routine: update the action camera's saved target pose. */
void btlUpdateActionTargetCameraPose(BtlLinkedCommand *action) {
    BtlUnit *user;
    BtlUnit *target;
    BtlCamState *out;
    f32 targetPos[4];
    f32 userPos[4];
    f32 dir[4];
    f32 length;
    f32 extent;
    f32 angle;
    s32 frames;
    s32 idle;
    s32 eligible;

    out = &action->backCamera;
    user = action->task->unit;
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (target->status.flags & user->status.flags & 0x600) {
        return;
    }
    if (action->motionProgress != 0) {
        return;
    }
    frames = func_001D6050(user, user->unkEC);
    frames = (s32)((f32)frames / btlGetActorEffectScale(action->task));
    if (action->state == frames && (target->status.flags & 0x200)) {
        idle = btlHasIdleLinkedSlotKindTwo((u8 *)action);
        eligible = btlHasEligibleLinkedEntryTypeTwo((u8 *)action);
        if (idle == 0 && eligible == 0) {
            return;
        }
        btlCopyMotionTransform(&action->frontCamera, &action->camera);
        btlCopyMotionTransform(out, &action->camera);
        action->backCamera.distance += idle != 0 ? 500.0f : 300.0f;
        action->flags = (action->flags & ~0x14) | 0x41;
        action->motionProgress = 1;
        action->motionParameter = 10.0f;
        action->state = 0;
        btlAdjustCameraDirectionForDefaultPlane(out);
    } else {
        extent = target->reach * target->scale * 2.25f;
        out->fov = action->camera.fov;
        btlUnitGetMuzzlePosVU(target);
        VU0_STORE_VF(vf10, targetPos);
        targetPos[1] -= target->height * target->scale * 0.2f;
        btlUnitGetMuzzlePosVU(user);
        VU0_STORE_VF_UNCLOBBERED(vf10, userPos);
        VU0_LOAD_VF(vf11, targetPos);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        length *= 0.6f;
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, dir);
        VU0_SCALE_VF_MFC1(vf10, length);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out->position);
        angle = 0.34906585f;
        extent += length * func_002FA060(angle);
        length *= func_002F9F60(angle);
        out->distance = length + extent / func_002FA148(out->fov * 1.3333333f * 0.5f);
        if (!(action->flags & 0x200)) {
            angle = -0.34906585f;
        }
        func_002DD688(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, out->direction);
        btlAdjustCameraDirectionForDefaultPlane(out);
    }
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E2FF8);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E3310);

extern BtlUnit *btlFindFarthestUnit(u32, f32 *);

extern BtlUnit *btlFindNearestUnit(u32, BtlUnit *);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern f32 func_002F9F60(f32);

/* Frame the camera for a two-actor command: place the eye so the actor, the farthest
   and the nearest actors fit the view, then store the resulting distance. */
void btlBuildShoulderActionCamera(BtlLinkedCommand *action, BtlCamState *pose, BtlUnit *unit, s32 mode) {
    f32 focus[4];
    f32 eye[4];
    f32 mirror[4];
    f32 farMuzzle[4];
    f32 fits[3];
    f32 view[4];
    f32 refReach;
    BtlUnit *ref;
    f32 offset;
    f32 halfFov;
    f32 nearFit;
    f32 unitExtent;
    f32 fit;
    f32 ramp;
    f32 shift;

    /* Distance at which `ref` fits the view along the mirror direction. */
    f32 func_001E37B0(f32 heightScale) {
        f32 tanHalf;
        f32 distance;

        refReach = ref->reach * ref->scale;
        btlUnitGetMuzzlePosVU(ref);
        VU0_SCALAR_OP_CLOBBER(focus[1] * heightScale, "vaddx.y vf10, vf0, vf2x");
        VU0_STORE_VF(vf10, mirror);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, view);
        VU0_CROSS_XYZ(vf10, vf10, vf11);
        VU0_CROSS_XYZ(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_MOVE_VF(vf12, vf10);
        VU0_LOAD_VF(vf10, focus);
        VU0_LOAD_VF(vf11, mirror);
        VU0_SUB(vf10, vf10, vf11);
        VU0_MOVE_VF(vf11, vf12);
        VU0_DOT_XYZ(offset, vf10, vf11);
        distance = ffabsf(offset);
        tanHalf = func_002FA148(halfFov);
        distance = (distance + refReach / func_002F9F60(halfFov)) / tanHalf;
        VU0_LOAD_VF(vf10, eye);
        VU0_SCALAR_OP_CLOBBER(focus[1] * heightScale, "vaddx.y vf10, vf0, vf2x");
        VU0_LOAD_VF(vf11, mirror);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, view);
        VU0_DOT_XYZ(offset, vf10, vf11);
        return distance + ffabsf(offset);
    }

    btlGetRuntime();
    btlFlagAllUnitsDefeatCandidate();
    if (unit->status.flags & 0x200) {
        f32 span[3];
        f32 diameter;

        pose->fov = action->camera.fov;
        unitExtent = unit->reach * unit->scale;
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF(vf10, focus);
        func_001F66D8(0x200, &span[0], NULL);
        focus[1] = -span[0];
        fit = func_001F66D8(0x400, &span[1], &span[2]);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[0] = 0.0f;
        ref = btlFindFarthestUnit(0x400, eye);
        if (ref != NULL) {
            btlUnitGetMuzzlePosVU(ref);
            VU0_STORE_VF(vf10, farMuzzle);
            fit = ffabsf(farMuzzle[0]) + ref->reach * ref->scale - eye[0];
        }
        if (span[1] < 620.0f) {
            if (!(span[1] - span[2] < 250.0f)) {
                eye[1] = -span[2] * 0.75f;
            } else if (span[2] > 100.0f) {
                eye[1] = -span[2] * 0.55f;
            } else {
                eye[1] = -span[2] * 0.45f;
            }
        } else {
            eye[1] = -span[1] * 0.6f;
        }
        offset = focus[0] - eye[0];
        halfFov = pose->fov * 1.3333333f * 0.5f;
        if (unitExtent < ffabsf(offset)) {
            ramp = (500.0f - fit) / 500.0f * 2.25f;
            if (ramp < 0.0f) {
                ramp = 0.0f;
            }
            if (fit <= 500.0f) {
                shift = 0.0f;
                if (mode == 0) {
                    if (ramp >= 0.0f) {
                        shift = 0.5f;
                    }
                } else if (ramp >= 0.0f) {
                    shift = -1.75f;
                }
            } else {
                shift = 0.0f;
                if (mode != 0) {
                    shift = -1.75f;
                }
            }
            if (offset < 0.0f) {
                eye[0] += fit * ramp;
                focus[0] -= unitExtent * shift;
            } else {
                eye[0] -= fit * ramp;
                focus[0] += unitExtent * shift;
            }
            VU0_LOAD_VF(vf10, eye);
            VU0_STORE_VF(vf10, pose->position);
            VU0_LOAD_VF(vf11, focus);
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, pose->direction);
            VU0_LOAD_VF(vf10, eye);
            VU0_SCALAR_OP_CLOBBER(focus[1], "vaddx.y vf10, vf0, vf2x");
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, view);
            VU0_LOAD_VF(vf10, eye);
            VU0_SCALAR_OP_CLOBBER(-focus[0], "vaddx.x vf10, vf0, vf2x");
            VU0_STORE_VF_UNCLOBBERED(vf10, mirror);
            ref = btlFindFarthestUnit(0x400, mirror);
            ref = ref != NULL ? ref : unit;
            nearFit = func_001E37B0(0.0f);
        } else {
            VU0_LOAD_VF(vf10, eye);
            VU0_STORE_VF(vf10, pose->position);
            VU0_LOAD_VF(vf11, focus);
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, pose->direction);
            VU0_LOAD_VF(vf10, eye);
            VU0_SCALAR_OP_CLOBBER(focus[1], "vaddx.y vf10, vf0, vf2x");
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, view);
            nearFit = fit * 1.2f / func_002FA148(halfFov);
        }
        ref = btlFindFarthestUnit(0x200, focus);
        if (ref != NULL) {
            fits[0] = func_001E37B0(1.0f);
        } else {
            fits[0] = 0.0f;
        }
        ref = btlFindNearestUnit(0x200, unit);
        if (ref != NULL) {
            fits[1] = func_001E37B0(1.0f);
        } else {
            fits[1] = 0.0f;
        }
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(offset);
        diameter = 2.0f * unitExtent;
        if (span[0] < diameter) {
            fits[2] = offset + diameter / func_002FA148(halfFov);
        } else {
            fits[2] = offset + span[0] / func_002FA148(halfFov);
        }
        offset = fits[0] > nearFit ? fits[0] : nearFit;
        if (offset < fits[1]) {
            offset = fits[1];
        }
        if (fits[2] > offset) {
            offset = fits[2];
        }
        pose->distance = offset;
        if (mode == 1) {
            pose->distance = offset + 100.0f;
        }
    }
}

extern void func_001E3310(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);
extern void btlBuildShoulderActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

void func_001E3E58(BtlLinkedCommand *action, BtlCamState *pose,
                   BtlUnit *target, s32 mode) {
    BtlState *runtime;
    BtlUnit *unit;
    s32 actorCount;
    struct {
        f32 position[4];
        f32 rotation[4];
        f32 firstActorPosition[4];
        f32 secondActorPosition[4];
        f32 center[4];
    } vectors __attribute__((aligned(16)));

    if ((target->status.flags & 0x200) == 0) {
        return;
    }

    runtime = (BtlState *)btlGetRuntime();
    if (runtime->cameraArrangementHook != NULL &&
        runtime->cameraArrangementHook(action, pose, mode) != 0) {
        return;
    }

    actorCount = 0;
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        s32 flags;

        if ((btlUnitStatusPair(unit) & 0x300) != 0x300) {
            flags = unit->status.flags;
        } else if (runtime->cameraPresetMode != 3) {
            flags = unit->status.flags;
        } else {
            if (unit->lookupId != 1) {
                func_001D6300((u8 *)unit, vectors.position);
                vectors.position[2] = unit->position[2] - 45.0f;
                btlSetUnitPosition((u8 *)unit, vectors.position);
            }
            flags = unit->status.flags;
        }
        {
            s32 excludedFlags = flags & 0xC0;

            if ((flags & 1) != 0) {
                if (excludedFlags == 0) {
                    if ((flags & 0x400) != 0) {
                        actorCount++;
                    }
                }
            }
        }
    }

    if (actorCount == 0) {
        return;
    }
    if (runtime->cameraActorHighWater < actorCount) {
        runtime->cameraActorHighWater = actorCount;
    }

    if (runtime->cameraActorConfiguration == 0x10003) {
        func_001E3310(action, pose, target, mode);
    } else {
        btlBuildShoulderActionCamera(action, pose, target, mode);
    }

    func_001F66D8(0x400, NULL, NULL);
    VU0_STORE_VF_UNCLOBBERED(vf10, vectors.center);
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        s32 flags = unit->status.flags;

        if ((flags & 0x200) != 0) {
            if ((flags & 0xE0) == 0) {
                if ((flags & 0x80000) != 0) {
                    btlUnitGetBodyPosVU((u8 *)unit);
                    VU0_STORE_VF_UNCLOBBERED(vf10, vectors.firstActorPosition);
                    if (btlAimHorizontalDirectionVU(vectors.firstActorPosition, vectors.center) != 0) {
                        VU0_STORE_VF_UNCLOBBERED(vf10, vectors.rotation);
                        btlSetUnitRotation((u8 *)unit, vectors.rotation);
                    }
                }
            }
        }
    }

    func_001F66D8(0x200, NULL, NULL);
    VU0_STORE_VF_UNCLOBBERED(vf10, vectors.center);
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        s32 flags = unit->status.flags;

        if ((flags & 0x400) != 0) {
            if ((flags & 0x80000) != 0) {
                btlUnitGetBodyPosVU((u8 *)unit);
                VU0_STORE_VF_UNCLOBBERED(vf10, vectors.secondActorPosition);
                btlAimHorizontalDirectionClampedVU(vectors.secondActorPosition, vectors.center,
                                                     0.34906585f);
                VU0_STORE_VF_UNCLOBBERED(vf10, vectors.rotation);
                btlSetUnitRotation((u8 *)unit, vectors.rotation);
            }
        }
    }

    if (target != NULL && (target->status.flags & 2) != 0 &&
        (runtime->cameraPresetMode == 1 ||
         (runtime->cameraPresetMode == 3 && target->lookupId == 1))) {
        s32 field = mdlGetNodeMotionIndex(target->ext->owner, 0);

        if (field == 0xD || field == 0x12) {
            btlRefreshUnitEffectMotionAndEntry((u8 *)target);
        }
    }

    btlAdjustCameraDirectionForDefaultPlane(pose);
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E4180);

/* vu0 routine: measure camera clearance from the actor's adjusted muzzle position. */
void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out) {
    BtlUnit *actor = ((BtlActorWork *)btlGetRuntime())->actorList;
    f32 span;
    f32 distance;

    for (; actor != 0; actor = actor->next) {
        s32 flags = actor->status.flags;
        if (!(flags & 1)) {
            continue;
        }
        if (flags & 0x200) {
            break;
        }
    }
    func_001DF358(command, out);
    btlCopyMotionTransform(pose, out);
    span = func_001F66D8(0x200, 0, 0) * 0.5f;
    pose->position[0] -= span;
    out->position[0] += span;
    out->distance *= 0.8f;
    btlUnitGetMuzzlePosVU(actor);
    VU0_SET_VF10_COMPONENT(y, -btlUnitGetTopY(actor));
    VU0_LOAD_VF(vf11, out);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    distance += (actor->cameraRadius * actor->scale * 2.0f) /
                func_002FA148(out->fov * 0.5f);
    if (out->distance < distance) {
        out->distance = distance;
    }
}

void func_001E4708(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out) {
    btlPrepareUnitPoseWithTiltRotation(command, pose, out);
}

/* vu0 routine: frame the leftmost marked unit in two camera poses. */
void func_001E4720(BtlCamState *source, BtlCamState *from,
                   BtlCamState *to) {
    f32 point[4];
    f32 center[4];
    f32 height;
    f32 fov;
    f32 minX;
    f32 length;
    BtlState *scene;
    BtlUnit *unit;
    BtlUnit *selected;
    s32 first;
    u32 flags;

    scene = (BtlState *)btlGetRuntime();
    fov = source->fov;
    from->fov = fov;
    to->fov = fov;
    func_001F66D8(0x400, &height, 0);
    VU0_STORE_VF(vf10, center);
    center[1] = -height;
    first = 1;
    selected = NULL;
    minX = 0.0f;
    for (unit = scene->units; unit != NULL; unit = unit->next) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                btlUnitGetMuzzlePosVU(unit);
                VU0_STORE_VF(vf10, point);
                if (first) {
                    selected = unit;
                    first = 0;
                    minX = point[0];
                } else if (point[0] < minX) {
                    minX = point[0];
                    selected = unit;
                }
            }
        }
    }
    btlUnitGetMuzzlePosVU(selected);
    VU0_STORE_VF(vf10, point);
    point[1] = -btlUnitGetTopY(selected);
    VU0_LOAD_VF(vf10, center);
    VU0_STORE_VF(vf10, from->position);
    VU0_LOAD_VF(vf11, point);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, from->direction);
    from->distance = length + selected->cameraRadius * selected->scale * 3.5f /
                              func_002FA148(fov * 0.5f);
    btlCopyMotionTransform(to, from);
    func_002DD688(-(45.0f * 0.017453293f));
    VU0_LOAD_VF(vf10, to->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);
    to->distance = length + selected->cameraRadius * selected->scale * 3.0f /
                            func_002FA148(fov * 0.5f);
    btlAdjustCameraDirectionForDefaultPlane(from);
    btlAdjustCameraDirectionForDefaultPlane(to);
}

/* vu0 routine: frame the two unit groups using their bounding extents. */
void btlBuildGroupFramingCameraPose(BtlCamState *source, BtlCamState *out) {
    f32 target[4];
    f32 height;
    f32 fov;
    f32 span;

    btlFlagAllUnitsDefeatCandidate();
    fov = source->fov;
    out->fov = fov;
    span = func_001F66D8(0x200, &height, 0);
    VU0_STORE_VF(vf10, out->position);
    out->position[1] = -height * 1.15f;
    func_001F66D8(0x400, &height, 0);
    VU0_STORE_VF(vf10, target);
    target[1] = -height * 0.75f;
    if (target[0] > 100.0f) {
        target[0] = 100.0f;
    } else if (target[0] < -100.0f) {
        target[0] = -100.0f;
    }
    VU0_LOAD_VF(vf10, target);
    VU0_LOAD_VF(vf11, out->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, out->direction);
    fov *= 1.3333333f;
    if (span < 250.0f) {
        span = 250.0f;
    }
    out->distance = span / func_002FA148(fov * 0.5f);
    btlAdjustCameraDirectionForDefaultPlane(out);
}

void btlResetUnitEffectVector(BtlLinkedCommand *command, BtlCamState *pose) {
    func_001DEFE0(command, pose, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E4AC0);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E4E50);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E5198);

void func_001E5460(BtlLinkedCommand *action, BtlCamState *camera) {
    btlFlagUserAndTargetDefeat(action, action);
}

/* vu0 routine: interpolate pull-back and pitch for actor/target framing. */
void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *action, BtlCamState *out) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 dir[4];
    f32 extent;
    f32 length;
    f32 span;
    f32 ratio;
    f32 factor;
    f32 angle;
    f32 width;
    f32 height;

    user = action->task->unit;
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    extent = user->reach * user->scale;
    span = func_001D6050(user, user->unkEC);
    span /= btlGetActorEffectScale(action->task);
    ratio = (f32)action->state / span;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    out->fov = action->camera.fov;
    height = btlUnitGetTopY(target);
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF(vf10, userPos);
    userPos[1] += user->height * user->scale * 0.5f;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPos);
    if (height < 600.0f) {
        targetPos[1] -= target->height * target->scale * 0.15f;
    } else {
        targetPos[1] -= target->height * target->scale * 0.25f;
    }
    if (targetPos[1] > -250.0f) {
        targetPos[1] = -250.0f;
    }
    VU0_LOAD_VF(vf10, targetPos);
    VU0_LOAD_VF(vf11, userPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    factor = ratio * (BTL_APPROACH_DIST_END - BTL_APPROACH_DIST_START);
    factor += BTL_APPROACH_DIST_START;
    length *= factor;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    VU0_SCALE_VF_MFC1(vf10, length);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, out->position);
    angle = ratio * (BTL_APPROACH_PITCH_END - BTL_APPROACH_PITCH_START);
    angle += BTL_APPROACH_PITCH_START;
    width = length * func_002FA060(angle);
    length *= func_002F9F60(angle);
    length += (extent + width) / func_002FA148(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (action->flags & 0x200) {
        angle = -angle;
    }
    func_002DD688(angle);
    VU0_LOAD_VF(vf10, dir);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
    btlAdjustCameraDirectionForDefaultPlane(out);
}

void func_001E5700(BtlLinkedCommand *action, BtlCamState *camera) {
    func_001E5460(action, camera);
}

void func_001E5718(BtlLinkedCommand *action, BtlCamState *out) {
    btlBuildHeightClampedApproachCamera(action, out);
}

extern void func_001E0718(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlChooseActionPoseBlendFromActorCount(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    BtlState *work = (BtlState *)btlGetRuntime();
    u32 count;
    u32 mask;
    BtlUnit *unit;

    mask = ((BtlUnit *)btlGetIndexListEntry(action->targetList, 0))->status.flags & 0x600;
    count = 0;
    for (unit = work->units; unit != 0; unit = unit->next) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & mask) {
                count++;
            }
        }
    }
    if (count >= 2) {
        func_001E0718(action, &action->frontCamera, &action->backCamera);
        return;
    }
    btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
    action->motionParameter = 200.0f;
    action->flags |= 0x41;
}

void func_001E57F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4000);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4180);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4190);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4310);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4320);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A43E0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A43F0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4400);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4408);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4468);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E5800);

extern const BtlCameraTimedInstruction *D_0035DA28[];

extern const BtlCameraTimedInstruction *D_0035DAD0[];

void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->task->unit->status.flags & 0x400)) {
        func_001E9DE0(action, state, D_0035DA28[CURSOR->unk_0A]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

void func_001E6180(BtlLinkedCommand *action) {
    u16 selection;

    CURSOR->frame = 0;
    if (!(action->task->unit->status.flags & 0x400)) {
        return;
    }

    memset(CURSOR, 0, 0x130);
    selection = action->task->indexWork.skillId;
    switch (selection) {
    case 0x1BB:
        CURSOR->unk_0C = 0x1B;
        CURSOR->unk_0E = 0;
        break;
    case 0x1BF:
        CURSOR->unk_0C = 0x16;
        CURSOR->unk_0E = 0;
        break;
    case 0x1B3:
        CURSOR->unk_0C = 0x11;
        CURSOR->unk_0E = 0;
        break;
    case 0x1B7:
        CURSOR->unk_0C = 0x22;
        CURSOR->unk_0E = 0;
        break;
    case 0x1C3:
        CURSOR->unk_0C = 0x23;
        CURSOR->unk_0E = 0;
        break;
    }
}

extern const BtlCameraTimedInstruction *D_0035DA40[];

void func_001E6260(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->task->unit->status.flags & 0x400)) {
        func_001E9DE0(action, state, D_0035DA40[CURSOR->unk_0C]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 :
            CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        switch (action->task->indexWork.skillId) {
        case 0x1B3:
        case 0x1B7:
        case 0x1BB:
        case 0x1BF:
        case 0x1C3:
            func_001E9DE0(action, state, D_0035DA40[CURSOR->unk_0C]);
            func_001EB368(action, state);
            btlConstrainCameraEndpointHeight(action, state, 0, 0);
            CURSOR->frame++;
            CURSOR->frame = CURSOR->frame <= 0 ? 0 :
                CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
            break;
        default:
            return;
        }
    }
}

extern u32 btlNextScaledRandom(u32);

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void func_001E6668(BtlLinkedCommand *, BtlCamState *, s32, s32);

extern s16 D_0035D810[];

extern s16 D_0035D7F0[];

typedef struct BtlCursorChoices {
    u16 values[3][3][8];
} BtlCursorChoices;

extern const BtlCursorChoices D_003A4668;

void func_001E6368(BtlLinkedCommand *action, BtlCamState *state) {
    BtlCursorChoices choices = D_003A4668;
    DatGameState *game;
    s16 markedCount = 0;
    s16 i;
    s16 random;

    memset(CURSOR, 0, 0x130);
    CURSOR->mode = 0;
    game = datGameState;
    for (i = 0; i < game->partyCount; i++) {
        if (game->party[i].flags & 2) {
            markedCount++;
        }
    }

    CURSOR->category = action->task->unit->lookupId;
    random = btlNextScaledRandom(8);
    CURSOR->index = choices.values[markedCount][CURSOR->category - 3][random];
    btlBuildPresetCameraPose(action, state, 0, D_0035D810[CURSOR->index]);
    func_001E6668(action, state, 0, D_0035D7F0[CURSOR->index]);
}

void btlAdvanceCommandCursor(BtlLinkedCommand *arg0, BtlCamState *arg1) {
    if (CURSOR->mode == 0) {
        func_001E9DE0(arg0, arg1, D_0035D9F0[CURSOR->index]);
    } else {
        func_001E9DE0(arg0, arg1, D_0035DA08[CURSOR->index]);
    }
    func_001EB368(arg0, arg1);
    CURSOR->frame++;
}

s32 btlFindLinkedActorById(s32 owner, s32 id) {
    s32 first = *(s32 *)(*(s32 *)(owner + 0xF4) + 0x18);
    s32 second;
    s32 third;

    if (*(u8 *)(first + 0x11C) == id) {
        return first;
    }
    second = *(s32 *)(owner + 0xF8);
    if (*(u8 *)(second + 0x11C) == id) {
        return second;
    }
    third = *(s32 *)(owner + 0xFC);
    if (third != 0 && *(u8 *)(third + 0x11C) == id) {
        return third;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E6668);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E6AC8);

void btlUnitGetPosVU(BtlUnit *unit, u8 mode) {
    u8 pos[16];
    switch (mode) {
    case 1:
        btlSetActorEffectParameterOrMuzzlePosition(unit, 1);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        break;
    case 0:
    default:
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        break;
    }
    VU0_LOAD_VF(vf10, pos);
}

/* Complete 0x60-byte camera-setup records. The seven bank boundaries and
 * their native 0x60 indexing establish the record and array extents. */
typedef struct BtlCameraSetupPointPolicy {
    u8 kind;
    u8 mode;
    u8 reserved02[2];
    f32 side;
    f32 distance;
    f32 height;
} BtlCameraSetupPointPolicy;

typedef struct BtlCameraSetupPolicy {
    f32 singleFov;
    f32 multipleFov;
    u8 reserved08[8];
    f32 boundsSeed[4];
    f32 boundsRadius;
    BtlCameraSetupPointPolicy focusPolicy;
    BtlCameraSetupPointPolicy eyePolicy;
    BtlCameraSetupPointPolicy terminalPolicy;
    u8 reserved54[12];
} BtlCameraSetupPolicy;

extern BtlCameraSetupPolicy D_0035A730[8];

extern BtlCameraSetupPolicy D_0035AEB0[31];

extern BtlCameraSetupPolicy D_0035AAF0[7];

extern BtlCameraSetupPolicy D_0035ADF0[1];

extern BtlCameraSetupPolicy D_0035AD90[1];

extern BtlCameraSetupPolicy D_0035AA30[2];

extern BtlCameraSetupPolicy D_0035AE50[1];

extern f32 func_001E6AC8(BtlUnit *, u8, f32, f32);

extern BtlUnit *btlFindActiveActorById(s32 id);

extern u32 btlCountUnitsByFlags(u32);

extern f32 func_001ED5C8(s32, f32 *, f32 *, const f32 *, const f32 *, const f32 *, f32);

extern f32 func_001EDB20(s32, BtlUnit *, BtlUnit *, s8, f32 *, f32 *, const f32 *, const f32 *);

extern f32 func_001EE160(s32, BtlUnit *, BtlUnit *, s8, f32 *, f32 *, const f32 *, const f32 *, s8);

extern f32 func_001EE658(s32, BtlUnit *, BtlUnit *, s8, s8, f32 *, f32 *, const f32 *, const f32 *);

extern f32 D_0035F590[4];

extern f32 D_0035F5A0[4];

extern f32 D_0035F5B0[4];

extern f32 D_003BB690;

/* Select a camera policy, construct its focus and eye, and initialize the
 * optional endpoint used by the camera cursor. */
INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4668);

void btlBuildPresetCameraPose(BtlLinkedCommand *action, BtlCamState *out, s16 bank, s16 index) {
    f32 focus[4];
    f32 eye[4];
    f32 destination[4];
    f32 direction[4];
    f32 quaternion[4];
    f32 actorPoint[4];
    f32 alternatePoint[4];
    f32 terminalPoint[4];
    f32 axis[4] = { 1.0f, 0.0f, 1.0f, 0.0f };
    f32 localScale[4];
    f32 worldAxis[4];
    f32 angles[4];
    BtlCameraSetupPolicy *p;
    BtlUnit *unit;
    BtlUnit *otherUnit;
    s32 mask;
    s32 focusKind;
    f32 framingDistance;
    f32 halfFov;
    f32 span;
    f32 measuredDistance;
    f32 focusY;
    f32 adjustedFocusY;
    f32 length;
    f32 value;
    s32 sourceId;

    memset(localScale, 0, sizeof(localScale));
    localScale[2] = 1.0f;
    memset(worldAxis, 0, sizeof(worldAxis));
    worldAxis[2] = 1.0f;
    memset(angles, 0, sizeof(angles));
    framingDistance = 0.0f;
    switch (bank) {
    case 0: p = &D_0035A730[index]; break;
    case 1: p = &D_0035AEB0[index]; break;
    case 2: p = &D_0035AAF0[index]; break;
    case 3: p = &D_0035ADF0[index]; break;
    case 4: p = &D_0035AD90[index]; break;
    case 5: p = &D_0035AA30[index]; break;
    case 6:
    default: p = &D_0035AE50[index]; break;
    }
    if ((s32)btlGetIndexListCount(action->targetList) >= 2) {
        halfFov = p->multipleFov * 0.017453293f * 640.0f / 480.0f * 0.5f;
    } else {
        halfFov = p->singleFov * 0.017453293f * 640.0f / 480.0f * 0.5f;
    }
    focusKind = p->focusPolicy.kind;
    switch (focusKind) {
    case 0:
    case 2:
    case 3:
    case 0x14:
        if (p->focusPolicy.kind == 0x14) {
            unit = (BtlUnit *)btlFindLinkedActorById((s32)action, 1);
        } else if (p->focusPolicy.kind == 0) {
            unit = action->task->unit;
        } else if (p->focusPolicy.kind == 2) {
            unit = action->linkedA;
        } else {
            unit = action->linkedB;
        }
        btlUnitGetPosVU(unit, p->focusPolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        span = func_001E6AC8(unit, p->focusPolicy.mode,
                            p->focusPolicy.distance, p->focusPolicy.height);
        framingDistance = span / func_002FA148(halfFov);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        adjustedFocusY = focus[1];
        adjustedFocusY += p->focusPolicy.height;
        focus[1] = adjustedFocusY;
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        break;

    case 0x16:
    case 0x18:
    case 0x28:
        switch (focusKind) {
        case 0x16:
            unit = btlFindActiveActorById(0);
            if (unit != NULL) {
                btlUnitGetPosVU(unit, p->focusPolicy.mode);
                VU0_STORE_VF_UNCLOBBERED(vf10, focus);
            } else {
                unit = btlFindActiveActorById(1);
                btlUnitGetPosVU(unit, p->focusPolicy.mode);
                VU0_STORE_VF_UNCLOBBERED(vf10, focus);
            }
            break;
        case 0x18:
            unit = btlFindActiveActorById(2);
            if (unit != NULL) {
                btlUnitGetPosVU(unit, p->focusPolicy.mode);
                VU0_STORE_VF_UNCLOBBERED(vf10, focus);
            } else {
                unit = btlFindActiveActorById(1);
                btlUnitGetPosVU(unit, p->focusPolicy.mode);
                VU0_STORE_VF_UNCLOBBERED(vf10, focus);
            }
            break;
        default:
            if (action->task->unit->status.flags & 0x1000) {
                unit = action->task->unit;
            } else {
                unit = action->linkedA;
            }
            btlUnitGetPosVU(unit, p->focusPolicy.mode);
            VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        }
        span = func_001E6AC8(unit, p->focusPolicy.mode,
                            p->focusPolicy.distance, p->focusPolicy.height);
        framingDistance = span / func_002FA148(halfFov);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        adjustedFocusY = focus[1];
        adjustedFocusY += p->focusPolicy.height;
        focus[1] = adjustedFocusY;
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.eyeFrom);
        worldAxis[0] = p->terminalPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (p->focusPolicy.kind != 0x28) {
            func_001F66D8(0x200, NULL, NULL);
            VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        } else {
            func_001EE658(0x200, action->task->unit, action->linkedA,
                          (s8)p->focusPolicy.mode, (s8)p->focusPolicy.mode,
                          NULL, NULL, NULL, NULL);
            VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        }
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->terminalPolicy.distance);
        VU0_LOAD_VF(vf11, terminalPoint);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        terminalPoint[1] += p->terminalPolicy.height;
        VU0_LOAD_VF(vf10, terminalPoint);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.eyeTo);
        break;

    case 5:
        /* Three fresh owner chains around callback-sensitive queries. */
        btlUnitGetPosVU(action->targetList->entries[0], p->focusPolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        span = func_001E6AC8(action->targetList->entries[0],
                            p->focusPolicy.mode, 0.0f, 0.0f);
        framingDistance = span / func_002FA148(halfFov);
        btlCopyUnitRotationQuaternion(action->targetList->entries[0], quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        adjustedFocusY = focus[1];
        adjustedFocusY += p->focusPolicy.height;
        focus[1] = adjustedFocusY;
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        break;

    case 4:
        worldAxis[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        worldAxis[0] = 0.0f;
        VU0_MOVE_VF(vf10, vf0);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        adjustedFocusY = focus[1];
        adjustedFocusY += p->focusPolicy.height;
        focus[1] = adjustedFocusY;
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        break;

    case 7:
    case 8:
        worldAxis[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (action->task->unit->status.flags & 0x200) {
            mask = p->focusPolicy.kind == 8 ? 0x400 : 0x200;
        } else {
            mask = p->focusPolicy.kind == 8 ? 0x200 : 0x400;
        }
        /* Native passes the KIND byte here, intentionally not the mode. */
        span = func_001EDB20(mask, NULL, NULL, (s8)p->focusPolicy.kind,
                            NULL, NULL, NULL, NULL);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        framingDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        adjustedFocusY = focus[1];
        adjustedFocusY += p->focusPolicy.height;
        focus[1] = adjustedFocusY;
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        break;

    case 0x0F: {
        s32 boundsMask;
        worldAxis[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        boundsMask = 0x600;
        func_001F66D8(0x400, NULL, NULL);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        span = func_001ED5C8(boundsMask, NULL, NULL, NULL, NULL,
                            terminalPoint, 600.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        framingDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        adjustedFocusY = focus[1];
        adjustedFocusY += p->focusPolicy.height;
        focus[1] = adjustedFocusY;
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        break;
    }

    case 0x1B:
    case 0x1D:
    case 0x23:
    case 0x25:
    case 0x26:
    case 0x27:
        VU0_LOAD_VF(vf10, out->position);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.eyeFrom);
        switch (focusKind) {
        case 0x25:
            unit = action->task->unit;
            break;
        case 0x1B:
            unit = action->linkedB;
            break;
        case 0x27:
            if (action->task->unit->status.flags & 0x1000) {
                unit = action->linkedA;
            } else {
                unit = action->task->unit;
            }
            break;
        case 0x23:
            unit = action->linkedA;
            break;
        case 0x26:
            if (action->task->unit->status.flags & 0x1000) {
                unit = action->task->unit;
            } else {
                unit = action->linkedA;
            }
            break;
        default:
            unit = btlGetIndexListEntry(action->targetList, 0);
            break;
        }
        btlUnitGetPosVU(unit, p->focusPolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        span = func_001E6AC8(unit, p->focusPolicy.mode,
                            p->focusPolicy.distance, p->focusPolicy.height);
        framingDistance = span / func_002FA148(halfFov);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        focusY = focus[1];
        if (p->focusPolicy.kind == 0x1D) {
            if (focusY <= -225.0f) {
                focusY = -225.0f;
                focus[1] = -225.0f;
                terminalPoint[1] = focusY;
            }
        }
        VU0_LOAD_VF(vf10, direction);
        switch (p->focusPolicy.kind) {
        case 0x1B:
        case 0x23:
        case 0x25:
        case 0x26:
        case 0x27:
            VU0_SCALE_VF(vf10, p->focusPolicy.distance);
            break;
        default:
            VU0_SCALE_VF(vf10, span * 0.5f);
            break;
        }
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        focusY += p->focusPolicy.height;
        focus[1] = focusY;
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.eyeTo);
        VU0_LOAD_VF(vf10, out->position);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        break;

    case 0x1A:
    case 0x2C:
        worldAxis[0] = p->focusPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, angles);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        terminalPoint[1] += p->focusPolicy.height;
        if (p->focusPolicy.kind == 0x2C) {
            func_001F66D8(0x400, NULL, NULL);
            VU0_STORE_VF_UNCLOBBERED(vf10, destination);
            terminalPoint[0] = destination[0];
        }
        span = func_001ED5C8(0x400, NULL, NULL, NULL, NULL,
                            terminalPoint, 600.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        framingDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        break;

    default:
        break;
    }

    /* 001E77A0: main eye phase */
    out->fov = 40.0f * 0.017453293f;
    switch (p->eyePolicy.kind) {
    case 0: case 2: case 3: /* 001E77D0 */
        if (p->eyePolicy.kind == 0) unit = action->task->unit;
        else if (p->eyePolicy.kind == 2) unit = action->linkedA;
        else unit = action->linkedB;
        btlUnitGetPosVU(unit, p->eyePolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        /* 001E8F08 */
        eye[1] += p->eyePolicy.height;
        break;

    case 1: case 0x12: case 0x13: case 0x15: /* 001E7888 */
        if (p->eyePolicy.kind == 0x15) unit = (BtlUnit *)btlFindLinkedActorById((s32)action, 1);
        else if (p->eyePolicy.kind == 1) unit = action->task->unit;
        else if (p->eyePolicy.kind == 0x12) unit = action->linkedA;
        else unit = action->linkedB;
        btlUnitGetPosVU(unit, p->eyePolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, framingDistance + p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(measuredDistance);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (measuredDistance < framingDistance + p->eyePolicy.distance)
            measuredDistance = framingDistance + p->eyePolicy.distance;
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        break;

    case 0x2F: case 0x32: case 0x33: /* 001E7A08 */
        unit = action->task->unit;
        btlUnitGetPosVU(unit, p->focusPolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        span = func_001E6AC8(unit, p->focusPolicy.mode, 1.0f, 1.0f);
        framingDistance = span / func_002FA148(halfFov);
        if (p->eyePolicy.kind == 0x32) {
            func_001EDB20(0x400, unit, 0, 0, 0, 0, 0, 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, destination);
            btlUnitGetPosVU(unit, 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
        } else { /* 001E7AB0; fresh task/unit read after lookup+position calls */
            btlUnitGetPosVU(btlGetIndexListEntry(action->targetList, 0), 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, destination);
            btlUnitGetPosVU(action->task->unit, 1);
            VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
        }
        if (destination[0] > actorPoint[0]) { /* 001E7AE8 */
            localScale[0] = p->focusPolicy.side;
            D_0035F100.counter = 0;
        } else {
            localScale[0] = -p->focusPolicy.side;
            D_0035F100.counter = 1;
        }
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_SCALE_VF(vf10, framingDistance + p->focusPolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->focusPolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.focusFrom);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathEnd);
        if (p->eyePolicy.kind == 0x2F) localScale[0] = p->eyePolicy.side;
        else if (destination[0] > actorPoint[0]) localScale[0] = p->eyePolicy.side;
        else localScale[0] = -p->eyePolicy.side;
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_SCALE_VF(vf10, framingDistance + p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        destination[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.focusTo);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathStart);
        break;

    case 0x2E: /* 001E7C80 */
        unit = btlGetIndexListEntry(action->targetList, 0);
        btlUnitGetPosVU(unit, p->focusPolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        btlUnitGetPosVU(action->task->unit, p->eyePolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        if (fabsf(focus[0] - eye[0]) < 100.0f) {
            if (eye[0] < focus[0]) worldAxis[0] = p->eyePolicy.side;
            else worldAxis[0] = -p->eyePolicy.side;
        }
        span = func_001EE658(0x600, unit, action->task->unit,
                            (s8)p->focusPolicy.mode, (s8)p->eyePolicy.mode,
                            0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        framingDistance = span / func_002FA148(halfFov);
        framingDistance += p->eyePolicy.distance;
        worldAxis[2] = -worldAxis[2];
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_SCALE_VF(vf10, framingDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        /* 001E7D8C: NOT eye[1] += height; source is saved actor point. */
        eye[1] = destination[1] + p->eyePolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.direction);
        /* 001E7DD8: the unchanged scale value is consumed again. */
        VU0_SCALE_VF(vf10, framingDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        /* Deferred reframing uses both signed mode selectors and this half-angle. */
        D_0035F100.eyeFrom[0] = (f32)(s8)p->focusPolicy.mode;
        D_0035F100.eyeTo[0] = (f32)(s8)p->eyePolicy.mode;
        D_0035F100.fov = halfFov;
        break;

    case 0x30: case 0x35: /* 001E7E28 */
        unit = btlGetIndexListEntry(action->targetList, 0);
        btlUnitGetPosVU(unit, p->focusPolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        if (p->eyePolicy.kind == 0x35) {
            if (action->task->unit->status.flags & 0x1000) {
                btlUnitGetPosVU(action->linkedA, p->eyePolicy.mode);
                VU0_STORE_VF_UNCLOBBERED(vf10, eye);
                VU0_STORE_VF_UNCLOBBERED(vf10, destination);
                otherUnit = action->linkedA; /* 001E7E94: reload after call */
            } else {
                btlUnitGetPosVU(action->task->unit, p->eyePolicy.mode);
                VU0_STORE_VF_UNCLOBBERED(vf10, eye);
                VU0_STORE_VF_UNCLOBBERED(vf10, destination);
                otherUnit = action->task->unit; /* 001E7ED8: fresh reload */
            }
        } else {
            btlUnitGetPosVU(action->task->unit, p->eyePolicy.mode);
            VU0_STORE_VF_UNCLOBBERED(vf10, eye);
            VU0_STORE_VF_UNCLOBBERED(vf10, destination);
            otherUnit = action->task->unit;
        }
        if (focus[0] > eye[0]) worldAxis[0] = p->eyePolicy.side;
        else worldAxis[0] = -p->eyePolicy.side;
        span = func_001EE658(0x600, unit, otherUnit,
                            (s8)p->focusPolicy.mode, (s8)p->eyePolicy.mode,
                            0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        framingDistance = span / func_002FA148(halfFov);
        worldAxis[2] = -worldAxis[2];
        btlCopyUnitRotationQuaternion(otherUnit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        VU0_NEGATE_XYZ(vf10);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        VU0_LOAD_VF(vf11, worldAxis);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.direction);
        if (unit->status.flags & 0x400) measuredDistance = p->eyePolicy.distance;
        else if (otherUnit->partyRecord.unitId == 4) measuredDistance = p->eyePolicy.distance + 50.0f;
        else measuredDistance = p->eyePolicy.distance + 25.0f;
        VU0_LOAD_VF(vf10, D_0035F100.direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, destination);
        VU0_ADD(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.direction);
        VU0_SCALE_VF(vf10, framingDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathStart);
        VU0_LOAD_VF(vf10, D_0035F100.direction);
        VU0_SCALE_VF(vf10, framingDistance - 150.0f);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathEnd);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        D_0035F100.distance = framingDistance - 150.0f;
        break;

    case 0x31: case 0x34: /* 001E80D8 */
        unit = btlGetIndexListEntry(action->targetList, 0);
        mask = 0x400;
        if (!(unit->status.flags & 0x400)) mask = 0x200;
        if (p->eyePolicy.kind == 0x34) {
            if (action->task->unit->status.flags & 0x1000) {
                unit = action->linkedA;
            } else {
                unit = action->task->unit;
            }
        } else unit = action->task->unit;
        func_001EDB20(mask, unit, 0, (s8)p->eyePolicy.mode, 0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        btlUnitGetPosVU(unit, p->eyePolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        if (eye[1] + p->eyePolicy.height <= focus[1])
            eye[1] = eye[1] + p->eyePolicy.height;
        if (focus[0] > eye[0]) worldAxis[0] = p->eyePolicy.side;
        else worldAxis[0] = -p->eyePolicy.side;
        worldAxis[2] = -worldAxis[2];
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        span = func_001EDB20(mask, unit, 0, (s8)p->eyePolicy.mode, 0, 0, focus, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, out->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, out->position);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        break;

    case 0x2A: /* 001E82B0 */ {
        s32 centerMask = 0x200;
        unit = btlGetIndexListEntry(action->targetList, 0);
        span = func_001EE160(0x200, unit, 0, (s8)p->eyePolicy.mode, 0, 0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        btlUnitGetPosVU(action->task->unit, p->eyePolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
        VU0_LOAD_VF(vf11, eye);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(measuredDistance);
        VU0_NORMALIZE_VF10();
        VU0_SCALE_VF(vf10, measuredDistance * 0.7f);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        func_001F66D8(centerMask, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
        if (actorPoint[1] - 165.0f <= focus[1]) {
            if (focus[1] <= -150.0f) destination[1] -= 165.0f;
            else destination[1] -= 100.0f;
        }
        VU0_LOAD_VF(vf10, destination);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        /* 001E847C: the second target-list lookup/frame call is real. */
        unit = btlGetIndexListEntry(action->targetList, 0);
        span = func_001EE160(0x200, unit, 0, (s8)p->eyePolicy.mode, 0, 0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        break;
    }

    case 0x21: case 0x22: /* 001E8558 */
        worldAxis[0] = p->eyePolicy.side;
        worldAxis[2] = -worldAxis[2];
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        span = func_001F66D8(0x200, 0, 0);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathEnd);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.focusFrom);
        D_0035F100.distance = measuredDistance;
        worldAxis[2] = 1.0f;
        worldAxis[0] = 0.0f;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance + measuredDistance);
        VU0_LOAD_VF(vf11, actorPoint);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
        value = p->eyePolicy.height;
        value += value * 0.5f;
        actorPoint[1] += value;
        VU0_LOAD_VF(vf10, actorPoint);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.focusTo);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathStart);
        break;

    case 0x17: case 0x19: case 0x29: /* 001E86A0 */
        if (p->eyePolicy.kind != 0x29) {
            worldAxis[0] = p->eyePolicy.side;
            if ((s32)btlCountUnitsByFlags(0x200) < 3) {
                if (p->eyePolicy.kind == 0x19) {
                    sourceId = action->task->unit->lookupId;
                    if (sourceId == 1 || action->linkedA->lookupId == 1) {
                        if (sourceId == 0 || action->linkedA->lookupId == 0)
                            worldAxis[0] *= 0.5f;
                    }
                } else {
                    sourceId = action->task->unit->lookupId;
                    if (sourceId == 1 || action->linkedA->lookupId == 1) {
                        if (sourceId == 2 || action->linkedA->lookupId == 2)
                            worldAxis[0] *= 0.5f;
                    }
                }
            }
            VU0_LOAD_VF(vf10, worldAxis);
            VU0_STORE_VF_UNCLOBBERED(vf10, direction);
            span = func_001F66D8(0x200, 0, 0);
            measuredDistance = span / func_002FA148(halfFov);
            VU0_STORE_VF_UNCLOBBERED(vf10, eye);
            if ((s32)btlCountUnitsByFlags(0x200) < 3) {
                if (action->task->unit->lookupId == 1 || action->linkedA->lookupId == 1)
                    measuredDistance *= 1.3f;
            }
        } else { /* 001E87C0 */
            if (action->task->unit->status.flags & 0x1000) {
                if (action->task->unit->lookupId < action->linkedA->lookupId)
                    worldAxis[0] = -p->eyePolicy.side;
                else worldAxis[0] = p->eyePolicy.side;
            } else {
                if (action->linkedA->lookupId < action->task->unit->lookupId)
                    worldAxis[0] = -p->eyePolicy.side;
                else worldAxis[0] = p->eyePolicy.side;
            }
            if (action->task->unit->lookupId == 1 || action->linkedA->lookupId == 1)
                worldAxis[0] *= 0.6f;
            VU0_LOAD_VF(vf10, worldAxis);
            VU0_STORE_VF_UNCLOBBERED(vf10, direction);
            span = func_001EE658(0x200, action->task->unit, action->linkedA,
                                (s8)p->eyePolicy.mode, (s8)p->eyePolicy.mode, 0, 0, 0, 0);
            measuredDistance = span / func_002FA148(halfFov);
            VU0_STORE_VF_UNCLOBBERED(vf10, eye);
            /* 001E8894: fresh task/unit/linkedA reads after both calls */
            if (action->task->unit->lookupId == 1 || action->linkedA->lookupId == 1)
                measuredDistance *= 1.3f;
        }
        /* 001E88C0 */
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathEnd);
        VU0_LOAD_VF(vf11, D_0035F100.eyeTo);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, D_0035F100.eyeTo);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathStart);
        D_0035F100.distance = measuredDistance;
        break;

    case 0x24: /* 001E8998 */
        VU0_LOAD_VF(vf10, out->direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, out->distance);
        VU0_LOAD_VF(vf11, out->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        D_0035F100.eyeTo[1] += p->eyePolicy.height;
        break;

    case 0x1C: /* 001E89E0 */
        VU0_LOAD_VF(vf10, out->direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, out->distance);
        VU0_LOAD_VF(vf11, out->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathEnd);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathStart);
        break;

    case 0x1E: /* 001E8A30 */
        VU0_LOAD_VF(vf10, out->direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, out->distance);
        VU0_LOAD_VF(vf11, out->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathEnd);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.focusFrom);
        VU0_LOAD_VF(vf10, D_0035F100.focusFrom);
        VU0_LOAD_VF(vf11, terminalPoint);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(measuredDistance);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        direction[0] = 0.0f;
        measuredDistance *= 1.5f;
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, D_0035F100.focusFrom);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.focusTo);
        if (framingDistance < out->distance) framingDistance = 100.0f;
        framingDistance *= 0.1f;
        VU0_LOAD_VF(vf10, out->direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, framingDistance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathStart);
        D_0035F100.distance = framingDistance;
        break;

    case 4: /* 001E8B68 */
        worldAxis[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        worldAxis[0] = 0.0f;
        VU0_MOVE_VF(vf10, vf0);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        break;

    case 7: case 8: /* 001E8BC0 */
        worldAxis[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (action->task->unit->status.flags & 0x200) {
            if (p->eyePolicy.kind == 8) mask = 0x400;
            else mask = 0x200;
        } else {
            if (p->eyePolicy.kind == 8) mask = 0x200;
            else mask = 0x400;
        }
        func_001F66D8(mask, 0, 0);
        func_002FA148(halfFov); /* scalar ignored, call exists, vf10 preserved */
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        out->fov = 40.0f * 0.017453293f; /* 001E9564 */
        break;

    case 0xB: case 0xC: /* 001E8C60 */
        if (btlNextScaledRandom(2) == 0) {
            btlUnitGetPosVU((BtlUnit *)btlFindLinkedActorById((s32)action, 0), 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
            btlUnitGetPosVU((BtlUnit *)btlFindLinkedActorById((s32)action, 1), 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, alternatePoint);
        } else {
            btlUnitGetPosVU((BtlUnit *)btlFindLinkedActorById((s32)action, 1), 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, actorPoint);
            btlUnitGetPosVU((BtlUnit *)btlFindLinkedActorById((s32)action, 2), 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, alternatePoint);
        }
        VU0_LOAD_VF(vf10, actorPoint);
        VU0_LOAD_VF(vf11, alternatePoint);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(measuredDistance);
        measuredDistance *= 0.5f;
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, alternatePoint);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, focus);
        VU0_LOAD_VF(vf11, eye);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        framingDistance += p->eyePolicy.distance;
        if (framingDistance <= 650.0f) framingDistance = 650.0f;
        VU0_LOAD_VF(vf10, direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, framingDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        break;

    case 9: case 0xA: /* 001E8E20 */ {
        s32 eyeKind;
        worldAxis[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        eyeKind = p->eyePolicy.kind;
        if (eyeKind == 9) {
            VU0_NEGATE_XYZ(vf10);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (action->task->unit->status.flags & 0x200) {
            if (eyeKind == 0xA) mask = 0x400;
            else mask = 0x200;
        } else {
            if (eyeKind == 0xA) mask = 0x200;
            else mask = 0x400;
        }
        unit = btlGetIndexListEntry(action->targetList, 0);
        span = func_001EDB20(mask, unit, 0, (s8)p->eyePolicy.mode, 0, 0, 0, 0);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        break;
    }

    case 0x2D: /* 001E8F20 */ {
        f32 actorExtent;
        unit = action->targetList->entries[0];
        btlUnitGetPosVU(unit, p->eyePolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        actorExtent = func_001E6AC8(unit, p->eyePolicy.mode, 1.0f, 1.0f);
        framingDistance = actorExtent / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, out->position);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.eyeFrom);
        if (actorExtent >= 200.0f) {
            destination[0] -= actorExtent * 0.5f;
            measuredDistance = 150.0f;
        } else if (actorExtent >= 50.0f) {
            destination[0] -= actorExtent * 0.75f;
            measuredDistance = 140.0f;
        } else {
            measuredDistance = (50.0f - actorExtent) * 0.1f;
            if (1.8f <= measuredDistance) measuredDistance = 1.8f;
            if (measuredDistance <= 1.25f) measuredDistance = 1.25f;
            destination[0] -= actorExtent * measuredDistance;
            measuredDistance = 125.0f;
        }
        VU0_LOAD_VF(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.eyeTo);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf11, focus);
        VU0_LOAD_VF(vf10, eye);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, (framingDistance + measuredDistance) + 270.0f);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathEnd);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, framingDistance + measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F100.pathStart);
        D_0035F100.distance = (framingDistance + measuredDistance) + 270.0f;
        VU0_LOAD_VF(vf10, out->position);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        break;
    }

    case 0x1F: /* 001E91B8 */
        unit = action->task->unit;
        if (action->task->indexWork.skillId == 0x1D6) localScale[2] = 0.0f;
        btlUnitGetPosVU(unit, p->eyePolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] = p->eyePolicy.height; /* absolute assignment, 001E9258 */
        otherUnit = btlGetIndexListEntry(action->targetList, 0);
        span = func_001EE658(0x600, action->task->unit, otherUnit,
                            (s8)p->eyePolicy.mode, (s8)p->eyePolicy.mode, 0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        measuredDistance = span / func_002FA148(halfFov);
        if (measuredDistance < 350.0f) measuredDistance = 350.0f;
        VU0_LOAD_VF(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        break;

    case 0x10: case 0x11: /* 001E9358 */
        worldAxis[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (action->task->unit->status.flags & 0x200) {
            if (p->eyePolicy.kind == 0x11) mask = 0x400;
            else mask = 0x200;
        } else {
            if (p->eyePolicy.kind == 0x10) mask = 0x200;
            else mask = 0x400;
        }
        span = func_001F66D8(mask, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (framingDistance < measuredDistance * 0.5f)
            framingDistance = measuredDistance; /* entire measuredDistance, not half */
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, framingDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        break;

    case 0xD: case 0xE: /* 001E94B0 */
        worldAxis[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (action->task->unit->status.flags & 0x200) {
            if (p->eyePolicy.kind == 0xE) mask = 0x400;
            else mask = 0x200;
        } else {
            if (p->eyePolicy.kind == 0xE) mask = 0x200;
            else mask = 0x400;
        }
        span = func_001F66D8(mask, 0, 0);
        measuredDistance = span / func_002FA148(halfFov);
        measuredDistance += p->eyePolicy.distance;
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        VU0_LOAD_VF(vf10, direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        out->fov = 40.0f * 0.017453293f;
        break;

    case 0x20: case 0x2B: /* 001E9580 */
        worldAxis[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, p->boundsSeed);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        if (p->eyePolicy.kind == 0x20) {
            func_001F66D8(0x400, 0, 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, destination);
            terminalPoint[0] = destination[0];
        }
        span = func_001ED5C8(0x400, 0, 0, focus, eye, terminalPoint, p->boundsRadius);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, focus);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        break;

    case 0xF: /* 001E96E0 */ {
        s32 boundsMask;
        worldAxis[0] = p->eyePolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        func_001F66D8(0x400, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        VU0_LOAD_VF(vf10, angles);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint); /* the first store is intentionally overwritten */
        boundsMask = 0x600;
        func_001ED5C8(boundsMask, 0, 0, 0, 0, terminalPoint, 600.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        func_002FA148(halfFov); /* native ignored scalar */
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        span = func_001ED5C8(boundsMask, 0, 0, focus, eye, terminalPoint, 600.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->eyePolicy.distance);
        VU0_LOAD_VF(vf11, eye);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        eye[1] += p->eyePolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        measuredDistance = span / func_002FA148(halfFov);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, measuredDistance);
        VU0_LOAD_VF(vf11, out->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        out->fov = 40.0f * 0.017453293f;
        break;
    }

    case 5: case 6: case 0x14: case 0x16: case 0x18: case 0x1A: case 0x1B:
    case 0x1D: case 0x23: case 0x25: case 0x26: case 0x27: case 0x28: case 0x2C:
    default: /* 001E98A8, or 001E98AC for unsigned kind >= 0x36 */
        break;
    }

    /* 001E98A8 / 001E98AC / 001E98B0: common finalizer, no zero-length guard. */
    VU0_LOAD_VF(vf10, focus);
    VU0_LOAD_VF(vf11, eye);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    out->distance = length;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, out->direction);
    worldAxis[1] = 0.0f;

    switch (p->terminalPolicy.kind) {
    case 0: case 2: case 3: /* 001E9930 */
        if (p->terminalPolicy.kind == 0) unit = action->task->unit;
        else if (p->terminalPolicy.kind == 2) unit = action->linkedA;
        else unit = action->linkedB;
        btlUnitGetPosVU(unit, p->terminalPolicy.mode);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, axis);
        localScale[0] = p->terminalPolicy.side;
        VU0_LOAD_VF(vf11, localScale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->terminalPolicy.distance);
        VU0_LOAD_VF(vf11, destination);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        destination[1] += p->terminalPolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, destination);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        D_003BB690 = length;
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5A0);
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5B0);
        VU0_LOAD_VF(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F590);
        break;

    case 4: /* 001E9A78 */
        worldAxis[0] = p->terminalPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        worldAxis[0] = p->terminalPolicy.side; /* repeated native write, 001E9A90 */
        VU0_MOVE_VF(vf10, vf0);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->terminalPolicy.distance);
        VU0_LOAD_VF(vf11, destination);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        destination[1] += p->terminalPolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, destination);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        D_003BB690 = length;
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5A0);
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5B0);
        VU0_LOAD_VF(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F590);
        break;

    case 7: case 8: /* 001E9B70 */
        worldAxis[0] = p->terminalPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        if (action->task->unit->status.flags & 0x200) {
            if (p->terminalPolicy.kind == 8) mask = 0x400;
            else mask = 0x200;
        } else {
            if (p->terminalPolicy.kind == 8) mask = 0x200;
            else mask = 0x400;
        }
        func_001F66D8(mask, 0, 0);
        func_002FA148(halfFov); /* ignored scalar, vf10 survives */
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->terminalPolicy.distance);
        VU0_LOAD_VF(vf11, destination);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        destination[1] += p->terminalPolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, destination);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        D_003BB690 = length;
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5A0);
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5B0);
        VU0_LOAD_VF(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F590);
        break;

    case 0xF: /* 001E9CA8 */
        worldAxis[0] = p->terminalPolicy.side;
        VU0_LOAD_VF(vf10, worldAxis);
        VU0_STORE_VF_UNCLOBBERED(vf10, direction);
        func_001F66D8(0x600, 0, 0);
        func_002FA148(halfFov); /* ignored scalar, vf10 survives */
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        VU0_LOAD_VF(vf10, direction);
        VU0_SCALE_VF(vf10, p->terminalPolicy.distance);
        VU0_LOAD_VF(vf11, destination);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        destination[1] += p->terminalPolicy.height;
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, destination);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        D_003BB690 = length;
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5A0);
        VU0_LOAD_VF(vf10, eye);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5B0);
        VU0_LOAD_VF(vf10, destination);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F590);
        break;

    case 1: case 5: case 6: case 9: case 0xA: case 0xB: case 0xC: case 0xD: case 0xE:
    default: /* 001E9DA0 or unsigned terminal kind >= 0x10 */
        break;
    }
    /* 001E9DA0..001E9DDC: restore saved registers, return void. */
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001E9DE0);

/* vu0 routine: constrain a camera pose endpoint to the enabled height planes. */
s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *action, BtlCamState *pose, s8 bypassUpper, s8 bypassLower) {
    union {
        u128 q;
        f32 f[4];
    } point, plane;
    f32 distance;
    f32 adjustedDistance;
    f32 planeDistance;
    f32 pointDistance;
    f32 scale;

    if (CURSOR->busy != 0) {
        return 0;
    }
    VU0_LOAD_VF(vf10, pose->direction);
    VU0_NEGATE_XYZ(vf10);
    distance = pose->distance;
    VU0_SCALAR_OP(distance, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, pose->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, &point);
    if (bypassUpper == 0 && -24.0f <= point.f[1]) {
        VU0_LOAD_VF(vf10, pose->position);
        VU0_STORE_VF(vf10, &plane);
        plane.f[1] = -24.0f;
        VU0_LOAD_VF(vf10, pose->position);
        VU0_LOAD_VF(vf11, &plane);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(planeDistance);
        VU0_NORMALIZE_VF10();
        scale = distance * distance - planeDistance * planeDistance;
        point.f[1] = pose->position[1];
        scale = fsqrtf(scale);
        VU0_LOAD_VF(vf10, &point);
        VU0_LOAD_VF(vf11, pose->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(pointDistance);
        VU0_NORMALIZE_VF10();
        VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, &plane);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, &point);
    }
    if (bypassLower == 0 && point.f[1] < -900.0f) {
        point.f[1] = -900.0f;
    }
    VU0_LOAD_VF(vf10, pose->position);
    VU0_LOAD_VF(vf11, &point);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(adjustedDistance);
    pose->distance = adjustedDistance;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, pose->direction);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001EB368);

/* Camera parameter banks are indexed with a native 0x80-byte stride. */
typedef struct BtlCameraParameterRecord {
    u8 flags00;
    u8 pad01[0xF];
    f32 value[4];
    u8 flags20;
    u8 pad21[0xF];
    f32 fadeInDuration;
    u8 pad34[0xC];
    f32 fadeOutDuration;
    u8 pad44[0x18];
    f32 curveScale5C;
    f32 curveScale60;
    u8 pad64[0xC];
    s8 stop;
    u8 pad71[0xF];
} BtlCameraParameterRecord;

typedef char BtlCameraTimedInstruction_size[(sizeof(BtlCameraTimedInstruction) == 0x10) ? 1 : -1];

typedef char BtlCameraParameterRecord_size[(sizeof(BtlCameraParameterRecord) == 0x80) ? 1 : -1];

extern void func_001EBE88(BtlLinkedCommand *, BtlCamState *,
                         const BtlCameraTimedInstruction *, const s32 *,
                         const BtlCameraParameterRecord *, f32 *);

extern void func_002DD608(f32);

extern void sdfConvertEulerAnglesToQuaternionVU(f32, f32, f32);

extern f32 D_0035F590[4];

extern f32 D_0035F5A0[4];

extern f32 D_0035F5B0[4];

extern f32 D_003BB690;

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001EBE88);

void btlApplyScriptCameraTrack(BtlLinkedCommand *command, BtlCamState *pose,
                  const BtlCameraTimedInstruction *instruction, s32 *currentIndex,
                  const BtlCameraParameterRecord *records) {
    f32 savedPosition[4];
    f32 focus[4];
    f32 direction[4];
    f32 value[4];
    f32 length;
    f32 blend;
    f32 distance;
    u32 flags;
    s32 handled = 0;

    do {
        if (instruction->startFrame <= (f32)CURSOR->frame) {
            if ((f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                value[0] = records[*currentIndex].value[0];
                value[1] = records[*currentIndex].value[1];
                value[2] = records[*currentIndex].value[2];
                value[3] = records[*currentIndex].value[3];
                func_001EBE88(command, pose, instruction, currentIndex, records, value);
                switch (instruction->kind) {
                case 1:
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, pose->position);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedPosition);
                    func_002DD608(value[0]);
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, savedPosition);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->position);
                    break;
                case 0:
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, pose->position);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedPosition);
                    if (command->flags & 0x200) value[1] = -value[1];
                    func_002DD688(value[1]);
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, savedPosition);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->position);
                    break;
                case 2:
                    if (command->flags & 0x200) value[1] = -value[1];
                    func_002DD688(value[1]);
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    break;
                case 4:
                    pose->fov += value[0];
                    break;
                case 3:
                    pose->distance += value[0];
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, pose->position);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, focus);
                    VU0_LOAD_VF(vf10, D_0035F5A0);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(D_003BB690, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, focus);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F590);
                    break;
                case 10:
                case 11:
                case 12:
                    VU0_LOAD_VF(vf10, CURSOR->pathStart);
                    VU0_LOAD_VF(vf11, CURSOR->pathEnd);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LENGTH_VF10(length);
                    blend = value[0];
                    command->motionParameter = blend;
                    if (instruction->startFrame == (f32)CURSOR->frame) {
                        btlScalarRangeSetStartClearEnd(&command->exponentialRange, blend);
                    }
                    blend = btlScalarRangeStepExponential(&command->exponentialRange);
                    if (0.9999990f <= blend) blend = 0.9999990f;
                    distance = length * blend;
                    if (instruction->kind == 12) distance = -distance;
                    pose->distance = CURSOR->distance + distance;
                    if (instruction->kind == 11) {
                        VU0_LOAD_VF(vf10, CURSOR->direction);
                        VU0_NEGATE_XYZ(vf10);
                        VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                        VU0_LOAD_VF(vf11, pose->position);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
                        VU0_LOAD_VF(vf10, pose->position);
                        VU0_LOAD_VF(vf11, focus);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(pose->distance);
                        VU0_NORMALIZE_VF10();
                        VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    } else {
                        VU0_LOAD_VF(vf10, pose->direction);
                        VU0_NEGATE_XYZ(vf10);
                        VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                        VU0_LOAD_VF(vf11, pose->position);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
                    }
                    VU0_LOAD_VF(vf10, D_0035F5A0);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(D_003BB690, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, focus);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F590);
                    break;
                case 13:
                    VU0_LOAD_VF(vf10, CURSOR->pathStart);
                    VU0_LOAD_VF(vf11, CURSOR->pathEnd);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LENGTH_VF10(length);
                    VU0_NORMALIZE_VF10();
                    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
                    blend = value[0];
                    command->motionParameter = blend;
                    if (instruction->startFrame == (f32)CURSOR->frame) {
                        btlScalarRangeSetStartClearEnd(&command->exponentialRange, blend);
                    }
                    blend = btlScalarRangeStepExponential(&command->exponentialRange);
                    if (blend >= 0.9999990f) blend = 0.9999990f;
                    distance = length * blend;
                    VU0_LOAD_VF(vf10, direction);
                    VU0_SCALAR_OP(distance, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, CURSOR->pathEnd);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, focus);
                    VU0_LOAD_VF(vf10, pose->position);
                    VU0_LOAD_VF(vf11, focus);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LENGTH_VF10(pose->distance);
                    VU0_NORMALIZE_VF10();
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    VU0_LOAD_VF(vf10, D_0035F5A0);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(D_003BB690, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, focus);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F590);
                    break;
                case 5:
                    if (command->flags & 0x200) value[0] = -value[0];
                    if (value[0] != 0.0f && 0.0f <= D_0035F5A0[2]) value[1] = -value[1];
                    sdfConvertEulerAnglesToQuaternionVU(value[1], value[0], value[2]);
                    effMiscQuaternionToMatrixVU();
                    VU0_LOAD_VF(vf10, D_0035F5A0);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5A0);
                    VU0_LOAD_VF(vf10, D_0035F5A0);
                    VU0_SCALAR_OP(D_003BB690, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, D_0035F590);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F5B0);
                    flags = records[*currentIndex].flags00;
                    if (flags & 4) {
                        VU0_LOAD_VF(vf10, pose->direction);
                        VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                        VU0_LOAD_VF(vf11, D_0035F5B0);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_STORE_VF_UNCLOBBERED(vf10, pose->position);
                    } else if (flags & 1) {
                        VU0_LOAD_VF(vf10, pose->position);
                        VU0_LOAD_VF(vf11, D_0035F5B0);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(pose->distance);
                        VU0_NORMALIZE_VF10();
                        VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    }
                    break;
                case 8:
                    btlSetActorEffectParameterOrMuzzlePosition(command->task->unit, 1);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedPosition);
                    savedPosition[1] -= 200.0f;
                    btlSetActorEffectParameterOrMuzzlePosition(command->targetList->entries[0], 1);
                    VU0_STORE_VF_UNCLOBBERED(vf10, focus);
                    VU0_LOAD_VF(vf10, focus);
                    VU0_LOAD_VF(vf11, savedPosition);
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LENGTH_VF10(blend);
                    blend = blend * 4.0f / 5.0f;
                    VU0_NORMALIZE_VF10();
                    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
                    blend += 300.0f;
                    VU0_LOAD_VF(vf10, focus);
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->position);
                    VU0_LOAD_VF(vf10, direction);
                    VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    pose->distance = blend;
                    break;
                default:
                    break;
                }
                handled = 1;
            } else if (records[*currentIndex].stop != 0) {
                handled = 1;
            } else {
                ++*currentIndex;
            }
        } else {
            handled = 1;
        }
    } while (handled != 1);
}

BtlUnit *btlFindActiveActorById(s32 id) {
    BtlUnit *node;

    for (node = *(BtlUnit **)(btlGetRuntime() + 0x228); node != 0; node = *(BtlUnit **)((u8 *)node + 0x344)) {
        u32 flags = (u32)node->status.flags;

        if (flags & 1) {
            if ((flags & 0xC0) == 0) {
                if (flags & 0x200) {
                    if (*(u8 *)((u8 *)node + 0x11C) == id) {
                        return node;
                    }
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001ED5C8);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001EDB20);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001EE160);

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001EE658);

u32 btlCountUnitsByFlags(u32 mask) {
    s32 context = btlGetRuntime();
    BtlUnit *actor = *(BtlUnit **)(context + 0x228);
    u32 count = 0;
    while (actor != 0) {
        u32 flags = (u32)actor->status.flags;
        if ((flags & 1) != 0 && (flags & mask) != 0 && (flags & 0x20) == 0) {
            count++;
        }
        actor = *(BtlUnit **)((u8 *)actor + 0x344);
    }
    return count;
}

void btlInitializeCameraCursorForActorMode(BtlLinkedCommand *action, BtlCamState *state) {
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    switch (action->task->unit->partyRecord.unitId) {
    case 1:
        btlBuildPresetCameraPose(action, state, 2, 2);
        CURSOR->unk_0C = 2;
        func_001E6668(action, state, 2, 1);
        CURSOR->unk_00 = 1;
        break;
    case 4:
        btlBuildPresetCameraPose(action, state, 2, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 2, 1);
        CURSOR->unk_00 = 1;
        break;
    case 3:
    case 5:
    case 6:
        btlBuildPresetCameraPose(action, state, 2, 2);
        CURSOR->unk_0C = 1;
        func_001E6668(action, state, 2, 1);
        CURSOR->unk_00 = 1;
        break;
    case 2:
        return;
    default:
        if (btlHasSingleLinkedResource(action) != 0) {
            func_001E1288(action, (BtlCamState *)action, 1);
        }
        break;
    }
}

extern const BtlCameraTimedInstruction *D_0035DAE0[];

void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *action, BtlCamState *state) {
    BtlCameraCursor *cursor = &D_0035F100;

    if (cursor->unk_00 == 1) {
        if (!(action->task->unit->status.flags & 0x400)) {
            func_001E9DE0(action, state, D_0035DAE0[cursor->unk_0C]);
            func_001EB368(action, state);
            btlConstrainCameraEndpointHeight(action, state, 0, 1);
            cursor->frame++;
            cursor->frame = cursor->frame <= 0 ? 0 :
                cursor->frame >= 0x7FFF ? 0x7FFE : cursor->frame;
        }
    }
}

void btlClearCommandCursorAndRunAction(BtlLinkedCommand *actor) {
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    btlFlagUserAndTargetDefeat(actor, actor);
}

extern const BtlCameraTimedInstruction *D_0035DAF0[];

extern const BtlCameraTimedInstruction *D_0035DAF8[];

void btlAdvanceCommandCursorOrAction(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->unk_00 == 1) {
        if (action->task->unit->status.flags & 0x400) {
            return;
        }
        func_001E9DE0(action, state, D_0035DAF0[CURSOR->unk_0C]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera(action, &action->camera);
    }
}

void btlInitCommandCursorForCategory(BtlLinkedCommand *action, BtlCamState *state) {
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    switch (action->task->unit->partyRecord.unitId) {
    case 1:
        btlBuildPresetCameraPose(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 2:
        return;
    case 3:
        btlBuildPresetCameraPose(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 4:
        btlBuildPresetCameraPose(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 5:
        btlBuildPresetCameraPose(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 6:
        btlFlagUserAndTargetDefeat(action, action);
        break;
    }
}

/* Advance the command cursor with the neighboring animation-entry table. */
void func_001EEED8(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->unk_00 == 1) {
        if (action->task->unit->status.flags & 0x400) {
            return;
        }
        func_001E9DE0(action, state, D_0035DAF8[CURSOR->unk_0C]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera(action, &action->camera);
    }
}

void btlInitCommandCursorForFirstActor(BtlLinkedCommand *arg0, BtlCamState *arg1) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *first = btlGetIndexListEntry(arg0->targetList, 0);
    memset(CURSOR, 0, 0x130);
    btlRefreshUnitEffectMotionAndEntry(first);
    if (btlHasFirstLinkedCategoryFlag1000(arg0) != 0) {
        btlBuildPresetCameraPose(arg0, arg1, 5, 1);
    } else {
        btlBuildPresetCameraPose(arg0, arg1, 5, 0);
    }
    if (work->battleMode == 0x10E) {
        CURSOR->unk_0C = 3;
    } else {
        CURSOR->unk_0C = 0;
    }
    func_001E6668(arg0, arg1, 0, 0x11);
}

void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->task->unit->status.flags & 0x400)) {
        func_001E9DE0(action, state, D_0035DAD0[CURSOR->unk_0C]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void func_001E6668(BtlLinkedCommand *, BtlCamState *, s32, s32);

void btlInitLinkedUnitActionCursor(BtlTask *arg0) {
    BtlLinkedCommand *command = &((BtlState *)btlGetRuntime())->cameraCommand;
    command->task = arg0;
    memset(CURSOR, 0, 0x130);
    CURSOR->unk_0A = 0;
    CURSOR->unk_0E = 0;
    btlRefreshUnitEffectMotionAndEntry(arg0->unit);
    btlBuildPresetCameraPose(command, &command->camera, 6, 0);
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(arg0->unit);
    CURSOR->unk_0C = 0;
    func_001E6668(command, &command->camera, 0, 0);
}

/* Initialize the target cursor and orient the linked unit toward its target. */
void btlInitTargetCursorAndFacing(BtlLinkedCommand *action, BtlCamState *state) {
    f32 position[4];
    f32 quaternion[4];
    f32 aimPosition[4];
    f32 rotation[4];
    f32 offset[4];
    BtlUnit *unit;
    BtlUnit *target;

    memset(offset, 0, sizeof(offset));
    offset[2] = 1.0f;
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    btlBuildPresetCameraPose(action, state, 2, 3);
    func_001E6668(action, state, 0, 0);
    btlConstrainCameraEndpointHeight(action, state, 0, 1);
    btlFlagMatchingUnitsDefeatCandidate(0x600);
    unit = action->task->unit;
    if (unit->status.flags & 0x80000) {
        btlUnitGetPosVU(unit, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlCopyUnitRotationQuaternion(unit, quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, offset);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_SCALAR_OP(1.0f, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, aimPosition);
        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
        if (target->status.flags & 0x400) {
            func_001F66D8(0x400, 0, 0);
        } else {
            func_001F66D8(0x200, 0, 0);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlAimHorizontalDirectionVU((s128 *)aimPosition, (s128 *)position);
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(unit, (s128 *)rotation);
    }
}

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void func_001E6668(BtlLinkedCommand *, BtlCamState *, s32, s32);

extern s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *, BtlCamState *, s8, s8);

void btlInitCursorAndApplyAction(BtlLinkedCommand *actor, BtlCamState *target) {
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    btlBuildPresetCameraPose(actor, target, 2, 6);
    func_001E6668(actor, target, 0, 0);
    btlConstrainCameraEndpointHeight(actor, target, 0, 1);
    D_0035F100.unk_0C = 2;
}

void btlAdvanceWorldCounterAndSpawnActionObject(void) {
    u32 counter;
    s32 context;
    struct EffWorldNode *object;

    context = btlGetRuntime();
    counter = dds3AdvanceWorldCounter();
    object = evtSpawnActionObj9(counter);
    *(u32 *)(context + 0x204) = (u32)object;
    D_003BB694 = 0;
}

s32 btlAreWorkBuffersReady(void) {
    s32 context = btlGetRuntime();
    if (*(u32 *)(context + 0x57C) != 0) {
        if (*(u32 *)(context + 0x580) != 0) {
            return 1;
        }
    }
    return 0;
}

void btlReleaseWorkBuffers(void) {
    s32 context = btlGetRuntime();
    u32 pointer = *(u32 *)(context + 0x580);
    if (pointer != 0) {
        sdfReleaseChipOrRetainedResource((void *)pointer);
        *(u32 *)(context + 0x580) = 0;
    }
    if (*(u32 *)(context + 0x57C) != 0) {
        sdfReleaseChipOrRetainedResource((void *)(*(u32 *)(context + 0x57C)));
        *(u32 *)(context + 0x57C) = 0;
    }
}

void btlWaitForPendingWorkAndReleaseBuffers(void) {
    s64 temp_v0;
    s32 temp_v1;

    evtDrainSecondaryWorldNodes();
    do {
        temp_v0 = sdfCheckPendingWorkWithInterrupts();
    } while (temp_v0 != 0);
    evtDestroySecondaryWorldNode();
    do {
        temp_v0 = sdfCheckPendingWorkWithInterrupts();
    } while (temp_v0 != 0);
    btlReleaseWorkBuffers();
    temp_v1 = btlGetRuntime();
    *(u32 *)(temp_v1 + 500) = *(u32 *)(temp_v1 + 500) & 0xfffffffd;
}

extern char D_003A4AD8[];

extern char D_003A4AF0[]; /* "btl:free field F2\n" */

extern char D_003A4B08[]; /* "btl:free field F1\n" */

void btlFreeFieldBlocks(void) {
    BtlState *context = (BtlState *)btlGetRuntime();
    btlWaitForPendingWorkAndReleaseBuffers();
    if (context->fieldTBResourceId != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)context->fieldTBResourceId);
        context->fieldTBResourceId = 0;
        btlBossDebugPrintf(D_003A4AD8);
    }
    if (context->fieldF2ResourceId != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)context->fieldF2ResourceId);
        context->fieldF2ResourceId = 0;
        btlBossDebugPrintf(D_003A4AF0);
    }
    if (context->fieldF1ResourceId != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)context->fieldF1ResourceId);
        context->fieldF1ResourceId = 0;
        btlBossDebugPrintf(D_003A4B08);
    }
    context = (BtlState *)btlGetRuntime();
    context->battleFlags &= ~2;
}

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

extern void fldApplyLightSetCurrent(void);

void btlInitializeSceneLightingAndTint(void) {
    u8 *context = (u8 *)btlGetRuntime();
    fldApplyLightSetCurrent();
    btlInitTintTransitionResource(0x80, 0);
    VU0_LOAD_VF(vf10, (u8 *)D_00324770[0] + 0x10);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x10);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x40);
    VU0_LOAD_VF(vf10, D_00324770[0]);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x20);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x50);
    VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x30);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x60);
    *(u32 *)(context + 0x698) = 0x807E5C5E;
}

typedef struct BtlActionTableEntry {
    u8 kind;
    u8 pad01[2];
    u8 resourceType;
    u8 pad04[4];
    f32 lightColorMode;
    f32 lightColor[3];
    s32 defaultValue;
    u16 flags;
    u8 pad1E[2];
} BtlActionTableEntry;

typedef struct BtlSceneLightParams {
    f32 position[3];
    f32 unk0C;
    f32 color[3];
    f32 unk1C;
    f32 secondaryColor[3];
    f32 unk2C;
} BtlSceneLightParams;

void btlBuildActionLightParameters(s32 index, BtlSceneLightParams *light) {
    u8 *work = (u8 *)btlGetRuntime();
    BtlActionTableEntry *action;
    f32 *vector;
    f32 *defaultColor;
    u32 color;

    if ((*(u32 *)(work + 0x1F4) & 0x30000000) == 0) {
        /* The record table is stored as an integer address. */
        action = (BtlActionTableEntry *)(index * sizeof(BtlActionTableEntry) +
                                        datActionAnimationRecords);
        if (action->lightColorMode != 0.0f) {
            light->color[0] = action->lightColor[0];
            light->color[1] = action->lightColor[1];
            light->color[2] = action->lightColor[2];
            light->secondaryColor[0] = action->lightColor[0];
            light->secondaryColor[1] = action->lightColor[1];
            light->secondaryColor[2] = action->lightColor[2];
        } else {
            vector = D_00324770[0];
            defaultColor = (f32 *)kwlnDefaultColorVector;
            light->color[0] = vector[0];
            light->color[1] = vector[1];
            light->color[2] = vector[2];
            light->secondaryColor[0] = defaultColor[0];
            light->secondaryColor[1] = defaultColor[1];
            light->secondaryColor[2] = defaultColor[2];
        }
    } else {
        color = *(u32 *)(work + 0x698);
        light->color[0] = (f32)(color & 0xFF) / 255.0f;
        light->color[1] = (f32)((color >> 8) & 0xFF) / 255.0f;
        light->color[2] = (f32)((color >> 16) & 0xFF) / 255.0f;
        light->secondaryColor[0] = light->color[0];
        light->secondaryColor[1] = light->color[1];
        light->secondaryColor[2] = light->color[2];
    }
    light->position[0] = D_00324770[0][4];
    light->position[1] = D_00324770[0][5];
    light->position[2] = D_00324770[0][6];
}

s32 btlGetActionDefaultOrOverride(s32 index) {
    s32 context = btlGetRuntime();
    if (*(u32 *)(context + 0x1F4) & 0x30000000) {
        return *(s32 *)(context + 0x6A0);
    }
    return *(s32 *)(datActionAnimationRecords + index * 32 + 0x18);
}

u32 btlCameraVectorHasNaN(void) {
    u8 *context = (u8 *)btlGetRuntime();
    if (*(f32 *)(context + 0x50) != *(f32 *)(context + 0x50) ||
        *(f32 *)(context + 0x54) != *(f32 *)(context + 0x54) ||
        *(f32 *)(context + 0x58) != *(f32 *)(context + 0x58) ||
        *(f32 *)(context + 0x60) != *(f32 *)(context + 0x60) ||
        *(f32 *)(context + 0x64) != *(f32 *)(context + 0x64) ||
        *(f32 *)(context + 0x68) != *(f32 *)(context + 0x68)) {
        return 1;
    }
    return 0;
}

typedef struct SoundCommand {
    u32 handle;
    u32 resource;
    u16 currentId;
    u16 nextId;
} SoundCommand;

extern SoundCommand D_0035F5C0;

void btlInitTintTransitionResource(u32 resource, u16 soundId) {
    u32 handle;
    D_0035F5C0.currentId = soundId;
    D_0035F5C0.nextId = soundId;
    handle = fldGetSkyDrawState();
    D_0035F5C0.resource = resource;
    D_0035F5C0.handle = handle;
}

void btlInitTintTransitionDefault(u16 soundId) {
    u32 handle;
    D_0035F5C0.currentId = soundId;
    D_0035F5C0.nextId = soundId;
    handle = fldGetSkyDrawState();
    D_0035F5C0.handle = handle;
    D_0035F5C0.resource = 0x80;
}

void btlStepTintTransition(void) {
    SoundCommand *cmd = &D_0035F5C0;
    u32 value;
    u32 start;
    if (cmd->currentId != 0) {
        cmd->currentId += 0xFFFF;
        start = cmd->resource;
        value = (f32)(s32)(cmd->handle - start) * ((f32)cmd->currentId / (f32)cmd->nextId);
        fldSetSkyDrawState(value + D_0035F5C0.resource);
    } else {
        fldSetSkyDrawState(cmd->resource);
    }
}

typedef struct SoundTransition {
    u32 currentResource;
    u8 unk_04[0x14];
    u32 previousResource;
    u32 queuedResource;
    u16 soundId;
    u16 queuedId;
} SoundTransition;

void btlQueueTintTransition(u32 resource, u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_0035F5D0;
        transition->soundId = 0;
        transition->currentResource = resource;
        transition->queuedResource = resource;
        return;
    }
    transition = (SoundTransition *)D_0035F5D0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = resource;
}

void btlQueueTintTransitionToZero(u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_0035F5D0;
        transition->soundId = 0;
        transition->currentResource = 0;
        transition->queuedResource = 0;
        return;
    }
    transition = (SoundTransition *)D_0035F5D0;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = 0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
}

extern s32 btlBlendColor(s32, s32, f32);

void btlStepBlendColor(void) {
    SoundTransition *transition = (SoundTransition *)D_0035F5D0;

    if (transition->soundId != 0) {
        transition->currentResource = btlBlendColor(transition->queuedResource, transition->previousResource, (f32)transition->soundId / (f32)transition->queuedId);
        transition->soundId--;
    } else {
        transition->currentResource = transition->queuedResource;
    }
    btlStepTintTransition();
}

void btlDrawTintIfVisible(void) {
    SoundTransition *transition = (SoundTransition *)D_0035F5D0;

    if ((transition->currentResource & 0xFF000000) != 0) {
        func_00187C08(transition);
    }
}

void sndResetTransition(void) {
    SoundTransition *transition = (SoundTransition *)D_0035F5D0;
    D_003BB694 = 0;
    btlTintTransitionHoldCount = 0;
    transition->soundId = 0;
    transition->currentResource = 0;
}

void btlClearTintAndEnableCamera(void) {
    btlQueueTintTransitionToZero(0);
    effObjSetOpacityPassEnabled(1);
}

extern f32 *D_00324770[];

void btlUpdateTintAndWorldLight(void) {
    u8 *context = (u8 *)btlGetRuntime();
    f32 *position = D_00324770[0];

    if (position[0] == 0.0f && position[1] == 0.0f &&
        position[2] == 0.0f) {
        effObjSetOpacityPassEnabled(0);
    } else {
        effObjSetOpacityPassEnabled(1);
    }
    if (*(u32 *)(context + 0x1F8) & 0x20) {
        effObjSetOpacityPassEnabled(0);
    }
    btlStepBlendColor();
}

void btlTickFieldSwayAndTint(void) {
    s32 temp_v0 = btlGetRuntime();

    if ((((*(u32 *)(temp_v0 + 500) & 0x20000) != 0) && ((*(u32 *)(temp_v0 + 0x1f8) & 0x20) == 0)) &&
          ((*(u32 *)(temp_v0 + 0x1fc) & 0x4000000) == 0)) {
        func_00132BD0();
        fldUpdateSwayOffset();
        func_00132010();
    }
    btlDrawTintIfVisible();
}

extern WorldTransformSetup D_003D74A0;

extern void dds3LoadWorldTransformSetup(EffWorldNode *, WorldTransformSetup *);

extern void evtBeginUnitValueColorTransition(EffWorldNode *, s32);

void func_001EFD58(f32 *position, f32 *scale, s32 value) {
    BtlState *work = (BtlState *)btlGetRuntime();
    s32 listener = work->listener;

    D_003D74A0.transform.position[0] = position[0];
    D_003D74A0.transform.position[1] = position[1];
    D_003D74A0.transform.position[2] = position[2];
    D_003D74A0.transform.scale[0] = scale[0];
    D_003D74A0.transform.scale[1] = scale[1];
    D_003D74A0.transform.scale[2] = scale[2];
    D_003D74A0.unk00 = 0;
    D_003D74A0.flags = 0;
    D_003D74A0.mode = 0;
    D_003D74A0.transform.rotation[0] = 0.0f;
    D_003D74A0.transform.rotation[1] = 0.0f;
    D_003D74A0.transform.rotation[2] = 0.0f;
    D_003D74A0.transform.rotation[3] = 0.0f;
    dds3LoadWorldTransformSetup((EffWorldNode *)listener, &D_003D74A0);
    evtBeginUnitValueColorTransition((EffWorldNode *)work->listener, value);
}

extern SdfFlagListParams D_0035F5F8;

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4AD8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4AF0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4B08);

void btlCreateRainEffect(u32 kind, u32 arg) {
    BtlState *work = (BtlState *)btlGetRuntime();

    switch (kind) {
    case 0xDF:
        switch (arg) {
        case 0:
            break;
        case 1:
        case 3:
        case 4:
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_0035F5F8);
            break;
        }
        break;
    case 0xE0:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_0035F5F8);
        }
        break;
    case 0xE1:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_0035F5F8);
        }
        break;
    }
    if (work->soundTransitionTask != 0) {
        btlBossDebugPrintf("btl:rain create[f%03X_%03X]
", kind, arg);
    }
}

void btlDispatchLinkedEffectWhenBattleGatesClear(void) {
    if (mdlFlagTest(0x32)) {
        return;
    }
    {
        BtlState *context = (BtlState *)btlGetRuntime();
        if (context->commandRestrictFlags & 0x20) {
            return;
        }
        if (context->soundTransitionTask != 0) {
            effUpdateAndDrawSelectionEntries(context->soundTransitionTask);
        }
    }
}

extern char D_003A4B40[]; /* "btl:rain exit\n" */

void btlStopRainSoundTransition(void) {
    BtlState *context = (BtlState *)btlGetRuntime();
    if (context->soundTransitionTask != 0) {
        btlBossDebugPrintf(D_003A4B40);
        effReleaseSelectionFlagList(context->soundTransitionTask);
        context->soundTransitionTask = 0;
    }
}

extern char D_003A4B98[]; /* "/fld/b/f%03d/f%03d_%03df.tmx" */

extern char D_003A4BB8[]; /* "btl:load 0[%s]\n" */

extern char D_003A4BC8[]; /* "/fld/b/f%03d/f%03d_%03ds.tmx" */

extern char D_003A4BE8[]; /* "btl:load 1[%s]\n" */

extern char D_003A4BF8[]; /* "btl:floor load end 0\n" */

extern char D_003A4C10[]; /* "btl:floor load end 1\n" */

typedef struct BtlFieldArchiveNode {
    struct BtlFieldArchiveNode *next;
    u32 unk04;
    s32 handle;
    void *data;
} BtlFieldArchiveNode;

typedef struct BtlFieldArchiveRequest {
    u8 pad00[0x60];
    BtlFieldArchiveNode *resources;
} BtlFieldArchiveRequest;

typedef struct BtlFieldLoadArgs {
    s32 stage;
    s32 variant;
    BtlFieldArchiveRequest *request;
    u8 pad0C[0xC];
    void *fieldF1;
    void *fieldF2;
    void *fieldTB;
    s32 frame;
} BtlFieldLoadArgs;

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 evtCreateWorldObjectFromResource(s32, s32, s32, s32,
                                            const SdfTextureOffsetListHeader *, s32);

extern char D_003A4B50[]; /* "%sf%03d_%03d.LB" */

extern char D_003A4B60[]; /* "btl:field load[%s]\n" */

extern char D_003A4B78[]; /* "btl:field load end[f%03d_%03d]\n" */

u32 btlPollFieldArchiveLoad(BtlFieldLoadArgs *args) {
    BtlState *blocks = (BtlState *)btlGetRuntime();
    char directory[0x80];
    char path[0x80];
    BtlFieldArchiveNode *node;
    u32 i;

    if (args->frame == 0) {
        btlFreeFieldBlocks();
        fldFormatAreaDirectory(directory, args->stage, 1);
        func_003014F0(path, D_003A4B50, directory, args->stage, args->variant);
        btlBossDebugPrintf(D_003A4B60, path);
        args->request = (BtlFieldArchiveRequest *)fileQueuePlainDispatchRequest(path);
        args->fieldF1 = NULL;
        args->fieldF2 = NULL;
        args->fieldTB = NULL;
    } else {
        if (args->request != NULL && fileRequestIsReady((struct FileRequest *)args->request)) {
            BtlFieldArchiveRequest *request = args->request;
            node = request->resources;
            i = 0;
            while (node != NULL) {
                switch (i) {
                case 0:
                    blocks->fieldTBResourceId = node->handle;
                    args->fieldTB = node->data;
                    break;
                case 1:
                    blocks->fieldF2ResourceId = node->handle;
                    args->fieldF2 = node->data;
                    break;
                case 2:
                    blocks->fieldF1ResourceId = node->handle;
                    args->fieldF1 = node->data;
                    break;
                }
                node = node->next;
                i++;
            }
            func_00288788(request);
            args->request = NULL;
        }
        if (args->fieldF1 != NULL && args->fieldF2 != NULL && args->fieldTB != NULL) {
            evtCreateWorldObjectFromResource(args->stage, args->variant,
                                             (s32)args->fieldF1, (s32)args->fieldF2,
                                             (const SdfTextureOffsetListHeader *)args->fieldTB, 0);
            btlInitializeSceneLightingAndTint();
            if (blocks->fieldTBResourceId != 0) {
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)blocks->fieldTBResourceId);
                blocks->fieldTBResourceId = 0;
                btlBossDebugPrintf(D_003A4AD8);
            }
            blocks->battleFlags |= 2;
            btlBossDebugPrintf(D_003A4B78, args->stage, args->variant);
            return 1;
        }
    }
    args->frame++;
    return 0;
}

s32 fldCreateSceneTileTask(u32 soundId, u32 variant) {
    u8 *task = btlAllocTask(40);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u16 *)(task + 0x20) = 1;
    *(void **)(task + 0x4C) = btlPollFieldArchiveLoad;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    arguments = btlGetTaskArguments(task);
    memset(arguments, 0, 40);
    arguments[0] = soundId;
    arguments[1] = variant;
    arguments[9] = 0;
    return (s32)task;
}

typedef struct BtlFloorLoadArgs {
    s32 frontHandle;
    s32 sideHandle;
    s32 stage;
    s32 variant;
    s32 state;
} BtlFloorLoadArgs;

typedef struct BtlFloorLoadRuntime {
    u8 pad_000[0x57C];
    void *primaryBuffer;
    void *secondaryBuffer;
} BtlFloorLoadRuntime;

s32 btlPollFloorLoadTask(BtlFloorLoadArgs *args) {
    s32 result = 1;
    BtlFloorLoadRuntime *work = (BtlFloorLoadRuntime *)btlGetRuntime();
    s32 stage = args->stage;
    s32 variant = args->variant;
    char path[0x70];

    if (args->state == 0) {
        func_003014F0(path, D_003A4B98, stage, stage, variant);
        result = 0;
        args->frontHandle = (s32)fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A4BB8, path);
        func_003014F0(path, D_003A4BC8, stage, stage, variant);
        args->sideHandle = (s32)fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A4BE8, path);
    } else {
        if (args->frontHandle != 0) {
            if (fileIsRequestReadyInCurrentMode((struct FileRequest *)args->frontHandle) != 0) {
                work->primaryBuffer =
                    (void *)sdfResourceRetainAddress((struct SdfMemBlock *)(fileGetResourceHandle((struct FileRequest *)args->frontHandle)));
                filePollEntryCleanup((struct FileRequest *)(u32)args->frontHandle);
                args->frontHandle = 0;
                btlBossDebugPrintf(D_003A4BF8);
            } else {
                result = 0;
            }
        }
        if (args->sideHandle != 0) {
            if (fileIsRequestReadyInCurrentMode((struct FileRequest *)args->sideHandle) != 0) {
                work->secondaryBuffer =
                    (void *)sdfResourceRetainAddress((struct SdfMemBlock *)(fileGetResourceHandle((struct FileRequest *)args->sideHandle)));
                filePollEntryCleanup((struct FileRequest *)(u32)args->sideHandle);
                args->sideHandle = 0;
                btlBossDebugPrintf(D_003A4C10);
            } else {
                result = 0;
            }
        }
    }
    args->state++;
    return result;
}

u8 *btlCreateFloorLoadTask(u32 soundId, u32 variant) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u16 *)(task + 0x20) = 2;
    *(void **)(task + 0x4C) = btlPollFloorLoadTask;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    arguments = btlGetTaskArguments(task);
    arguments[2] = soundId;
    arguments[3] = variant;
    arguments[0] = 0;
    arguments[1] = 0;
    arguments[4] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001F0430);

extern u32 func_001F0430(u32 *);

void *btlCreateEffectTaskWithSourceParams(u8 *source, u32 value) {
    u8 *task = btlAllocTask(0x34);
    u8 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 3;
    *(u16 *)(task + 0x24) |= BTL_TASK_FLAG_DEFERRED;
    *(void **)(task + 0x4C) = func_001F0430;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    *(u32 *)(arguments + 0x30) = value;
    if (source != 0) {
        memcpy(arguments, source, 0x30);
    } else {
        memcpy(arguments, (u8 *)btlGetRuntime() + 0x10, 0x30);
    }
    return task;
}

s32 btlRestoreSceneTransformLighting(SceneLightRestoreArgs *args) {
    BtlState *work;
    BtlUnit *unit;
    s32 listener;
    u32 firstColor;
    u32 secondColor;
    u32 packedStart[4];
    u32 packedEnd[4];

    if (D_003BB694 == 0) {
        return 1;
    }
    D_003BB694--;
    if (D_003BB694 == 0) {
        work = (BtlState *)btlGetRuntime();
        if (work->battleFlags & 0x20000000) {
            return 1;
        }
        if (D_00324770[0][0] == 0.0f && D_00324770[0][1] == 0.0f && D_00324770[0][2] == 0.0f) {
            VEC3_SPLAT(D_00324770[0], 0.0078125f);
        }
        listener = work->listener;
        D_003D74A0.transform.position[0] = work->baselineLightColor[0];
        D_003D74A0.transform.position[1] = work->baselineLightColor[1];
        D_003D74A0.transform.position[2] = work->baselineLightColor[2];
        D_003D74A0.transform.scale[0] = work->baselineAmbientColor[0];
        D_003D74A0.transform.scale[1] = work->baselineAmbientColor[1];
        D_003D74A0.transform.scale[2] = work->baselineAmbientColor[2];
        D_003D74A0.unk00 = 0;
        D_003D74A0.flags = 0;
        D_003D74A0.mode = 0;
        D_003D74A0.transform.rotation[0] = 0.0f;
        D_003D74A0.transform.rotation[1] = 0.0f;
        D_003D74A0.transform.rotation[2] = 0.0f;
        D_003D74A0.transform.rotation[3] = 0.0f;
        dds3LoadWorldTransformSetup((EffWorldNode *)listener, &D_003D74A0);
        evtBeginUnitValueColorTransition((EffWorldNode *)work->listener, args->value);
        PCP_COPY_VECTOR_F32(work->lightColor, work->baselineLightColor);
        PCP_COPY_VECTOR_F32(work->ambientColor, work->baselineAmbientColor);
        PCP_COPY_VECTOR_F32(work->lightDirection, work->baselineLightDirection);
        for (unit = work->units; unit != NULL; unit = unit->next) {
            if (!(unit->status.flags & 2)) {
                continue;
            }
            if (!(unit->status.stateFlags & 0x10)) {
                continue;
            }
            if (unit->ext != NULL) {
                evtSetUnitStatusFlags(unit->ext);
                VU0_LOAD_VF(vf10, unit->colorStart);
                EE_MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
                firstColor = packedStart[0];
                VU0_LOAD_VF(vf10, unit->colorEnd);
                EE_MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
                secondColor = packedEnd[0];
                evtInitializeUnitColorTransition(unit->ext, args->value, firstColor, secondColor);
                VU0_LOAD_VF(vf10, unit->lightDirection);
                evtSetUnitNormalizedDirection(unit->ext, args->value);
            }
        }
        D_00324770[0][4] = work->baselineLightDirection[0];
        D_00324770[0][5] = work->baselineLightDirection[1];
        D_00324770[0][6] = work->baselineLightDirection[2];
    }
    return 1;
}

BtlRuntimeTask *func_001F0920(u32 value) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SceneLightRestoreArgs *arguments;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 4;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->callback = btlRestoreSceneTransformLighting;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->value = value;
    return task;
}

s64 func_001F0998(SceneLightRestoreArgs *arguments) {
    D_003BB694 = 1;
    return btlRestoreSceneTransformLighting(arguments);
}

BtlRuntimeTask *btlCreateSoundUpdateTask(u32 value) {
    BtlRuntimeTask *task = func_001F0920(value);
    task->taskId = 7;
    task->callback = func_001F0998;
    return task;
}

s32 btlQueueTintTransitionWhenEnabled(u32 *arguments) {
    s32 context = btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 0x20000000) == 0) {
        btlQueueTintTransition(arguments[0], *(u16 *)(arguments + 1));
    }
    btlTintTransitionHoldCount++;
    return 1;
}

SoundTask *sndCreateAcquireTask(u32 soundId, u32 flags) {
    SoundTask *task = (SoundTask *)btlAllocTask(8);
    u32 *data;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 5;
    task->callback.acquireSound = btlQueueTintTransitionWhenEnabled;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    data = btlGetTaskArguments(task);
    data[0] = soundId;
    data[1] = flags;
    return task;
}

s32 sndTickFadeCounter(soundId)
    u16 *soundId;
{
    s32 context = btlGetRuntime();

    if (btlTintTransitionHoldCount == 0) {
        return 1;
    }
    btlTintTransitionHoldCount--;
    if (btlTintTransitionHoldCount != 0) {
        return 1;
    }
    if ((*(u32 *)(context + 0x1F4) & 0x20000000) != 0) {
        return 1;
    }
    btlQueueTintTransitionToZero(*soundId);
    return 1;
}

SoundTask *sndCreateReleaseTask(sound)
    u32 *sound;

{
    SoundTask *task = (SoundTask *)btlAllocTask(4);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 6;
    task->callback.releaseSound = sndTickFadeCounter;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    *(u32 *)btlGetTaskArguments(task) = (u32)sound;
    return task;
}

s64 func_001F0B90(void) {
    btlTintTransitionHoldCount = 1;
    return sndTickFadeCounter();
}

SoundTask *btlCreateSoundReleaseTask(void) {
    SoundTask *task = (SoundTask *)sndCreateReleaseTask();
    task->taskId = 8;
    task->callback.update = func_001F0B90;
    return task;
}

void btlExtendTaskFrameLimit(SoundResourceNode *effect, s32 frames) {
    if (effect->fadeCountdown < frames) {
        effect->fadeCountdown = frames;
    }
}

u32 sndGetResourceStatus(SoundResourceNode *sound) {
    u32 flags;
    if (!sound->referenceCount) {
        return 0;
    }
    flags = sound->flags;
    if (flags & 1) {
        return 0xFFFFFFF;
    }
    if (flags & 2) {
        return sound->fadeCountdown;
    }
    return 0;
}

s32 sndLookupResourceType(s32 sound, s32 index) {
    s32 (*lookup)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x644);
    if (lookup != 0) {
        s32 value = lookup(sound, index);
        if (value != -1) {
            return value;
        }
    }
    return *(u8 *)(datActionAnimationRecords + index * 0x20 + 3);
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001F0CA0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4B40);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4B50);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4B60);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4B78);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4B98);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4BB8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4BC8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4BE8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4BF8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4C10);

void sndCreateSystemEffect(SoundResourceNode *effect) {
    SoundMixer *handle;
    if (!(effect->flags & 8) || effect->resourceHandle || effect->referenceCount) {
        return;
    }
    handle = sndMixerClone(effect->sourceHandle);
    effect->resourceHandle = handle;
    btlBossDebugPrintf("btl:system effect create[%p]\n", handle);
}

void sndDeleteSystemEffect(SoundResourceNode *effect) {
    if ((effect->flags & 8) && effect->resourceHandle && !effect->referenceCount) {
        btlBossDebugPrintf("btl:system effect delete[%p]\n", effect->resourceHandle);
        sndReleaseAllVoices(effect->resourceHandle);
        effect->resourceHandle = 0;
    }
}

void sndAddEffectReferences(SoundEffectReferenceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *source;
    BtlUnit *target;

    args->effect = 0;
    sndCreateSystemEffect(args->source);
    effect = args->source;
    target = args->targetOwner.unit;
    source = args->sourceOwner.unit;
    ++effect->referenceCount;
    ++source->effectLink.referenceCount;
    ++target->effectLink.referenceCount;
}

extern void effBattleSetInputValue(BattleEffect *, s32);

s32 func_001F1110(SoundEffectReferenceArgs *args) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *owner;

    if ((battle->battleFlags & 0x40000) == 0) {
        return 1;
    }
    owner = args->option == 2 ? args->targetOwner.unit : args->sourceOwner.unit;
    if (args->source->flags & 2) {
        if (args->effect == NULL) {
            args->effect = func_00160958(args->source->resourceHandle, args->option, owner, 0);
            args->duration = sndSetEffectNodeParameter(args->source, args->option);
            if ((owner->effectLink.packed & 0xA) == 8) {
                args->duration = 35;
                effBattleSetInputValue(args->effect, 35);
                effBattleUpdateSelectedValue(args->effect, args->duration);
            }
            btlExtendTaskFrameLimit(args->source, args->duration);
            return 0;
        }
        if (effBattleGetCurrentFrame(args->effect) >= args->duration) {
            return 1;
        }
        effBTLFieldColorSetSelectors(args->sourceOwner.selectorKey, args->targetOwner.selectorKey,
                                    args->sourceSelector, args->targetSelector);
        switch (args->option) {
        case 0:
        case 2:
            if (owner->status.flags & 4) {
                args->effect->flags |= 8;
            } else {
                args->effect->flags &= ~8;
            }
            break;
        }
    }
    if (args->effect != NULL && (owner->status.flags & 2)) {
        func_00160D88(args->effect);
    }
    return 0;
}

void sndReleaseEffectReferences(SoundEffectReferenceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *source;
    BtlUnit *target;

    if (args->effect) {
        effReleaseBattleVoiceOwner(args->effect);
    }
    effect = args->source;
    target = args->targetOwner.unit;
    source = args->sourceOwner.unit;
    --effect->referenceCount;
    --source->effectLink.referenceCount;
    --target->effectLink.referenceCount;
    sndDeleteSystemEffect(effect);
}

BtlRuntimeTask *func_001F12E8(SoundResourceNode *effect, BtlUnit *source, BtlUnit *owner, u16 variant) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundEffectReferenceArgs));
    SoundEffectReferenceArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x2B;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = owner->identity;
    task->onStart = sndAddEffectReferences;
    task->callback = func_001F1110;
    task->onFinish = sndReleaseEffectReferences;
    arguments = btlGetTaskArguments(task);
    arguments->source = effect;
    arguments->sourceOwner.unit = source;
    arguments->sourceSelector = arguments->sourceOwner.selectorKey;
    arguments->targetSelector = arguments->sourceOwner.selectorKey;
    arguments->targetOwner.unit = owner;
    arguments->option = variant;
    arguments->effect = 0;
    arguments->duration = 0;
    return task;
}

BtlRuntimeTask *sndCreateEffectWithTargets(SoundResourceNode *effect, BtlUnit *sourceOwner,
                                         s32 source, s32 target, BtlUnit *owner, u16 variant) {
    BtlRuntimeTask *task = func_001F12E8(effect, sourceOwner, owner, variant);
    SoundEffectReferenceArgs *data = btlGetTaskArguments(task);
    data->sourceSelector = source;
    data->targetSelector = target;
    return task;
}

typedef struct ActorEffectTaskArgs {
    SoundResourceNode *source;
    BattleEffect *effect;
    ActorEffectOwner owner;
    u32 duration;
    s32 counter;
} ActorEffectTaskArgs;

void sndStartEffectTask(ActorEffectTaskArgs *args) {
    SoundResourceNode *source;
    BtlUnit *unit;

    args->effect = 0;
    sndCreateSystemEffect(args->source);
    source = args->source;
    unit = args->owner.unit;
    source->referenceCount++;
    unit->effectLink.referenceCount++;
}

s32 sndUpdateActorEffectTask(ActorEffectTaskArgs *args) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    ActorEffectOwner owner = args->owner;
    BtlUnit *unit = owner.unit;

    if ((battle->battleFlags & 0x40000) == 0) {
        return 1;
    }
    if (args->effect == 0) {
        args->effect = func_00160958(args->source->resourceHandle, 0, unit, 0);
        effBattleUpdateSelectedValue((u8 *)args->effect, args->duration);
    }
    if (effBattleGetCurrentFrame(args->effect) >= args->duration) {
        return 1;
    }
    if (args->counter < 6) {
        args->effect->color = ((u32)(args->counter * 128.0f / 6.0f) << 24) | 0x00808080;
    } else {
        args->effect->color = 0x80808080;
    }
    effBTLFieldColorSetSelectors(owner.selectorKey, owner.selectorKey, 0, 0);
    if (unit->status.flags & 4) {
        args->effect->flags |= 8;
    } else {
        args->effect->flags &= ~8;
    }
    if (unit->status.flags & 2) {
        func_00160D88((u8 *)args->effect);
    }
    args->counter++;
    return 0;
}

void sndFinishActorEffectTask(ActorEffectTaskArgs *args) {
    SoundResourceNode *source;
    BtlUnit *unit;

    if (args->effect != 0) {
        effReleaseBattleVoiceOwner(args->effect);
    }
    source = args->source;
    unit = args->owner.unit;
    source->referenceCount--;
    unit->effectLink.referenceCount--;
    sndDeleteSystemEffect(source);
}

BtlRuntimeTask *sndCreateActorEffectTask(SoundResourceNode *source, BtlUnit *owner, u32 duration) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(ActorEffectTaskArgs));
    ActorEffectTaskArgs *args;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x2C;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = owner->identity;
    task->onStart = sndStartEffectTask;
    task->callback = sndUpdateActorEffectTask;
    task->onFinish = sndFinishActorEffectTask;
    args = btlGetTaskArguments(task);
    args->source = source;
    args->owner.unit = owner;
    args->duration = duration;
    args->effect = 0;
    args->counter = 0;
    return task;
}

void sndIncrementEffectActiveCount(TimedUnitEffectArgs *args) {
    args->source->activeCount = args->source->activeCount + 1;
}

s32 sndWaitEffectFramesAndApplyUnitParameter(TimedUnitEffectArgs *args) {
    SoundResourceNode *effect = args->source;
    s32 frames;

    if ((effect->flags & 2) == 0) {
        return 0;
    }
    frames = sndGetEffectNodeParameter(effect, args->option);
    if (args->frame >= frames) {
        args->source->activeCount -= 1;
        if (args->channel >= 0) {
            btlApplyScaledUnitEffectParameter((u8 *)args->unit, args->channel, args->volume, 1.0f);
        }
        return 1;
    }
    args->frame++;
    return 0;
}

BtlRuntimeTask *sndCreateTimedUnitEffectTask(SoundResourceNode *effect, BtlUnit *actor, u16 frames,
                                          s32 channel, u32 volume) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(TimedUnitEffectArgs));
    TimedUnitEffectArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x2E;
    task->ownerId = actor->identity;
    task->onStart = sndIncrementEffectActiveCount;
    task->callback = sndWaitEffectFramesAndApplyUnitParameter;
    task->onFinish = 0;
    arguments = btlGetTaskArguments(task);
    arguments->source = effect;
    arguments->unit = actor;
    arguments->option = frames;
    arguments->channel = channel;
    arguments->volume = volume;
    arguments->frame = 0;
    return task;
}

void sndBeginEffectLoad(EffectLoadArgs *args) {
    SoundResourceNode *effect = args->effect;
    if (effect->flags & 2) {
        if (effect->resourceHandle != 0) {
            sndReleaseAllVoices(effect->resourceHandle);
            effect->resourceHandle = 0;
        }
        effect->flags &= ~2;
    }
    args->loadHandle = fileQueueDefaultCallbackRequest(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

extern char D_003A4C88[];

s32 sndPollEffectLoad(EffectLoadArgs *args) {
    SoundResourceNode *effect = args->effect;
    s32 resource;

    if (effect->flags & 2) {
        return 1;
    }
    if (!fileIsRequestReadyInCurrentMode((struct FileRequest *)args->loadHandle)) {
        return 0;
    }
    btlBossDebugPrintf(D_003A4C88, args->name);
    resource = fileGetResourceHandle((struct FileRequest *)args->loadHandle);
    effect->resourceHandle =
        sndMixerClone((void *)sdfResourceRetainAddress((struct SdfMemBlock *)(resource)));
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(resource));
    filePollEntryCleanup((struct FileRequest *)(u32)args->loadHandle);
    effect->flags = (effect->flags & ~1) | 2;
    return 0;
}

BtlRuntimeTask *sndCreateEffectLoadTask(SoundResourceNode *effect, const char *filename) {
    BtlRuntimeTask *task = btlAllocTask(strlen(filename) + sizeof(EffectLoadArgs));
    EffectLoadArgs *arguments;
    char *name;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x2F;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->onStart = sndBeginEffectLoad;
    task->callback = sndPollEffectLoad;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    arguments = btlGetTaskArguments(task);
    name = (char *)(arguments + 1);
    arguments->effect = effect;
    arguments->name = name;
    strcpy(name, filename);
    return task;
}

extern u32 btlWaitUnitListIdle(void);

u32 btlWaitUnitListIdle(void) {
    s32 context = btlGetRuntime();
    s32 unit;

    if (*(u32 *)(context + 0x1F4) & 0x40000000) {
        return 1;
    }
    for (unit = *(s32 *)(context + 0x228); unit != 0; unit = *(s32 *)(unit + 0x344)) {
    }
    return 1;
}

void *btlCreateWaitUnitListIdleTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x30;
    *(void **)(task + 0x4C) = btlWaitUnitListIdle;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u32 *)btlGetTaskArguments(task) = sound;
    return task;
}

u32 sndApplyToActiveActors(u32 *soundId) {
    BtlUnit *object = *(BtlUnit **)(btlGetRuntime() + 0x228);
    while (object != 0) {
        u32 flags = (u32)object->status.flags;
        if (flags & 1) {
            if (flags & 2) {
                if (*(u32 *)((u8 *)object + 0x320) != 0 &&
                    (flags & 0xE0) == 0) {
                    btlBlendUnitColor(object, *(u32 *)((u8 *)object + 0x54), *soundId);
                }
            }
        }
        object = *(BtlUnit **)((u8 *)object + 0x344);
    }
    return 1;
}

void *btlCreateApplyToActiveActorsTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x31;
    *(void **)(task + 0x4C) = sndApplyToActiveActors;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u32 *)btlGetTaskArguments(task) = sound;
    return task;
}

u32 btlCancelTimedFadeTask(void) {
    kwlnCancelConfiguredFadeFrames();
    return 1;
}

SoundTask *btlCreateFadeStateResetTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlCancelTimedFadeTask;
    task->taskId = 0x32;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

void sndAddSourceReferences(SoundEffectSourceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *source;
    args->effect = 0;
    sndCreateSystemEffect(args->source);
    effect = args->source;
    source = args->owner.unit;
    ++effect->referenceCount;
    ++source->effectLink.referenceCount;
}

s32 func_001F1CC8(SoundEffectSourceArgs *args) {
    BtlUnit *owner;
    u32 flags;

    btlGetRuntime();
    owner = args->owner.unit;

    if (args->effect == NULL) {
        args->effect = func_00160958(args->source->resourceHandle, 0, owner, 0);
        args->effect->flags |= 1;
    }
    if (args->frameCount < 12) {
        args->effect->color = ((u32)((f32)args->frameCount * 128.0f / 12.0f) << 24) | 0x808080;
    } else if (btlFindTaskByHandle(args->resource) == 0) {
        if (args->fadeOutFrame != 12) {
            args->effect->color = ((u32)((1.0f - (f32)args->fadeOutFrame / 12.0f) * 128.0f) << 24) | 0x808080;
            args->fadeOutFrame++;
        } else {
            return 1;
        }
    } else {
        args->effect->color = 0x80808080;
    }
    effBTLFieldColorSetSelectors((s32)owner, (u32)owner, 0, 0);
    flags = owner->status.flags;
    if (flags & 4) {
        args->effect->flags |= 8;
    } else {
        args->effect->flags &= ~8;
    }
    if (flags & 2) {
        func_00160D88(args->effect);
    }
    args->frameCount++;
    return 0;
}

extern s32 func_001F1CC8(SoundEffectSourceArgs *);

void sndFinishEffectSourceTask(SoundEffectSourceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *unit;

    if (args->effect != 0) {
        effReleaseBattleVoiceOwner(args->effect);
    }
    effect = args->source;
    unit = args->owner.unit;
    effect->referenceCount = effect->referenceCount - 1;
    unit->effectLink.referenceCount = unit->effectLink.referenceCount - 1;
    sndDeleteSystemEffect(effect);
}

BtlRuntimeTask *sndCreateEffectSourceTask(SoundResourceNode *effect, BtlUnit *owner, u64 resource) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundEffectSourceArgs));
    SoundEffectSourceArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x2D;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = owner->identity;
    task->onStart = sndAddSourceReferences;
    task->callback = func_001F1CC8;
    task->onFinish = sndFinishEffectSourceTask;
    arguments = btlGetTaskArguments(task);
    arguments->source = effect;
    arguments->owner.unit = owner;
    arguments->resource = resource;
    arguments->effect = 0;
    arguments->frameCount = 0;
    arguments->fadeOutFrame = 0;
    return task;
}

u32 btlTaskStartFadeIn(u32 *arg0) {
    kwlnFadeStartIn(*arg0);
    return 1;
}

void *btlCreateFadeInTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x35;
    *(void **)(task + 0x4C) = btlTaskStartFadeIn;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments(task) = sound;
    return task;
}

u32 btlTaskStartCustomFadeIn(u8 *arg0) {
    kwlnFadeInStart(*arg0, arg0[1], arg0[2], *(u32 *)(arg0 + 4));
    return 1;
}

SoundTask *sndCreateCustomTask(u32 soundId, u32 options) {
    SoundTask *task = (SoundTask *)btlAllocTask(8);
    u32 *data;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x36;
    task->callback.playCustomSound = btlTaskStartCustomFadeIn;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    data = btlGetTaskArguments(task);
    data[0] = soundId;
    data[1] = options;
    return task;
}

u32 btlTaskSetBattleFlag40000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0x40000;
    mdlClearListedObjectFlag();
    return 1;
}

SoundTask *sndCreateSetBattleFlagTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlTaskSetBattleFlag40000;
    task->taskId = 0x37;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

u32 btlTaskClearBattleFlag40000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffbffff;
    mdlSetListedObjectFlag();
    return 1;
}

SoundTask *sndCreateClearBattleFlagTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlTaskClearBattleFlag40000;
    task->taskId = 0x38;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern const char D_003BB6A0[];

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4C88);

const char D_003A4CA8[16] __attribute__((aligned(8))) = "%s%03X.BED";

const char D_003A4CB8[32] __attribute__((aligned(8))) = "/efftool/bed/BTL_TEST.BED";

s32 btlFormatActionEventFilename(s32 index, char *output) {
    BtlState *state = (BtlState *)btlGetRuntime();

    if ((state->battleFlags & 0x10000000) == 0) {
        u16 assetId = ((BtlActionAnimationRecord *)datActionAnimationRecords)[index].displayCode;
        if (assetId == 0) {
            return 0;
        }
        func_003014F0(output, D_003A4CA8, D_003BB6A0, assetId);
    } else {
        func_003014F0(output, D_003A4CB8);
    }
    return 1;
}

s32 sndSetEffectNodeParameter(SoundResourceNode *effect, u16 option) {
    return sndReadSelectedMixerBankValue(effect->resourceHandle, option);
}

s32 sndGetEffectNodeParameter(SoundResourceNode *effect, u16 option) {
    return func_00160858(effect->resourceHandle, option);
}

s32 sndIsResourceNodeReferencedOrActive(SoundResourceNode *effect) {
    if (effect->referenceCount != 0) {
        return 1;
    }
    return effect->activeCount != 0;
}

s32 sndHasActiveActor(void) {
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    while (actor != 0) {
        SoundResourceNode *sound = actor->resourceNode;
        if (sound != 0 && sndIsResourceNodeReferencedOrActive(sound) != 0) {
            return 1;
        }
        actor = actor->next;
    }
    return 0;
}

/* Allocate a cleared resource node and prepend it to the runtime's resource list. */
SoundResourceNode *sndAllocResourceNode(void) {
    SoundResourceNode *node = sdfAllocAndClearQuadwords(sizeof(SoundResourceNode));
    BtlActorWork *state;
    SoundResourceNode *first;
    node->referenceCount = 0;
    node->activeCount = 0;
    node->fadeCountdown = 0;
    node->resourceHandle = 0;
    state = (BtlActorWork *)btlGetRuntime();
    node->previous = 0;
    first = state->soundResourceHead;
    if (first) {
        first->previous = node;
        node->next = state->soundResourceHead;
    } else {
        node->next = 0;
    }
    state->soundResourceHead = node;
    return node;
}

SoundResourceNode *sndCreateResourceNode(SoundMixer *soundId) {
    SoundResourceNode *node = (SoundResourceNode *)sndAllocResourceNode();
    node->resourceHandle = sndMixerClone(soundId);
    node->flags |= 2;
    return node;
}

/* Release owned voices, unlink the resource node, and free its allocation. */
void sndFreeResourceNode(SoundResourceNode *node) {
    if (node->resourceHandle) {
        sndReleaseAllVoices(node->resourceHandle);
    }
    if (node->next) {
        node->next->previous = node->previous;
    }
    if (node->previous) {
        node->previous->next = node->next;
    } else {
        ((BtlActorWork *)btlGetRuntime())->soundResourceHead = node->next;
    }
    sdfReleaseChipBlock(node);
}

/* Advance resource countdowns and battle tint, then tick slot-volume fades. */
void btlUpdateFadeColor(void) {
    BtlActorWork *context = (BtlActorWork *)btlGetRuntime();
    SoundResourceNode *node;

    for (node = context->soundResourceHead; node != 0; node = node->next) {
        if (node->referenceCount == 0) {
            node->fadeCountdown = 0;
        } else if (node->fadeCountdown > 0) {
            node->fadeCountdown = node->fadeCountdown - 1;
        }
    }
    if ((u32)(btlGetActiveUnitId() - 9) < 2 || context->unk2A4 != 0 || context->unk208 == 8) {
        context->fadeEnabled = 0;
    } else {
        context->fadeEnabled = 1;
    }
    switch (context->fadeEnabled) {
    case 0: {
        u32 packedColor = context->fadeColor;

        /* Test the packed threshold before adding; do not clamp the high byte alone. */
        if (packedColor <= 0x8080807F) {
            context->fadeColor = packedColor + 0x10000000;
        } else {
            context->fadeColor = 0x80808080;
        }
        break;
    }
    case 1: {
        u32 packedColor = context->fadeColor;

        if (packedColor > 0x808080) {
            context->fadeColor = packedColor - 0x10000000;
        } else {
            context->fadeColor = 0x808080;
        }
        break;
    }
    }
    effTickSlotVolumeFade();
}

void btlSweepFloorModelLists(void) {
    effSweepFloorModelList();
}

void btlResetFieldColorAndSweepFlags(void) {
    effBTLFieldColorResetFlags();
    fileResetRenderFlags();
    kwlnDrawControlFlags = kwlnDrawControlFlags & 0xdfffffff;
}

void btlClearSoundAndModelResources(void) {
    sndClearResourceNodes();
    mdlMarkAndProcessObjectNodes();
    effBTLFieldColorResetFlags();
    fileResetRenderFlags();
}

/* Destroy all resource nodes; preserve each next link before freeing its owner. */
void sndClearResourceNodes(void) {
    SoundResourceNode *node = ((BtlActorWork *)btlGetRuntime())->soundResourceHead;
    while (node) {
        SoundResourceNode *next = node->next;
        sndFreeResourceNode(node);
        node = next;
    }
}

s32 btlButtonMaskToIndex(u32 mask) {
    switch (mask & 0x7FFF) {
    case 0:
        return 0;
    case 0x0001:
        return 0xE;
    case 0x0002:
        return 0xB;
    case 0x0004:
        return 0xA;
    case 0x0008:
        return 8;
    case 0x0010:
        return 6;
    case 0x0020:
        return 7;
    case 0x0040:
        return 9;
    case 0x0080:
        return 5;
    case 0x0100:
        return 0xD;
    case 0x0200:
        return 4;
    case 0x0400:
        return 3;
    case 0x0800:
        return 0xC;
    case 0x1000:
        return 2;
    case 0x2000:
        return 1;
    case 0x4000:
        return 0x10;
    default:
        return 0;
    }
}

SoundResourceLink *sndAllocResourceLink(BtlUnit *owner) {
    SoundResourceLink *node = sdfAllocAndClearQuadwords(sizeof(SoundResourceLink));
    node->owner = owner;
    node->effectHandle = 0;
    node->variant = 0;
    node->effect = 0;
    return node;
}

void sndFreeResourceLink(SoundResourceLink *node) {
    if (node->effectHandle) {
        effReleaseBattleVoiceOwner(node->effectHandle);
        --node->effect->referenceCount;
        sndDeleteSystemEffect(node->effect);
    }
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001F2818);

void btlMarkTaskReady(SoundResourceLink *link) {
    link->refreshRequested = 1;
}

SoundLink *sndAllocLink(BtlUnit *owner) {
    SoundLink *node = sdfAllocAndClearQuadwords(sizeof(SoundLink));
    node->owner = owner;
    node->effectHandle = 0;
    node->variant = 0;
    node->effect = 0;
    return node;
}

void sndFreeLink(SoundLink *node) {
    if (node->effectHandle) {
        effReleaseBattleVoiceOwner(node->effectHandle);
        node->effect->referenceCount--;
        sndDeleteSystemEffect(node->effect);
    }
    sdfReleaseChipBlock(node);
}

/* Blend the linked command effect through the SDK's packed-color vectors. */
void btlUpdateUnitCommandEffect(SoundLink *link) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    BtlUnit *actor = link->owner;
    u16 effectId;

    if (actor->selectedEntryIndex > 0) {
        effectId = datCommandRecords[actor->selectedEntryIndex].unk2E;
    } else {
        effectId = 0;
    }
    if (effectId != 0 && !(actor->partyRecord.status & 0x4000)) {
        if (link->effectHandle == 0) {
            link->effect = work->soundResourceSlots[BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT];
            sndCreateSystemEffect(link->effect);
            link->effectHandle = func_00160958(link->effect->resourceHandle, 2, actor, 0);
            link->effect->referenceCount++;
            link->effectHandle->flags = (link->effectHandle->flags | 1) & ~6;
        }
        link->variant = effectId;
    } else if (link->effectHandle != 0) {
        effReleaseBattleVoiceOwner(link->effectHandle);
        link->effect->referenceCount--;
        sndDeleteSystemEffect(link->effect);
        link->effectHandle = 0;
        link->effect = 0;
    }
    if (link->effectHandle != 0 && (actor->status.flags & 4) && (work->fadeColor & 0xFF000000)) {
        s32 actorColor[4];
        s32 fadeColor[4];
        s32 propertyColor[4];
        s32 blended[4];
        u32 property = btlGetSelectedUnitProperty(actor);
        u32 unit;
        u32 packed;

        actorColor[0] = (actor->overlayColor & 0xFF000000) | 0x808080;
        unit = 0x3C000000;
        EE_MMI_RGBA_UNPACK(actorColor, unit);
        VU0_MOVE_VF(vf11, vf10);
        fadeColor[0] = work->fadeColor;
        EE_MMI_RGBA_UNPACK(fadeColor, unit);
        VU0_MUL(vf10, vf10, vf11);
        VU0_MOVE_VF(vf11, vf10);
        propertyColor[0] = property;
        EE_MMI_RGBA_UNPACK(propertyColor, unit);
        VU0_MUL(vf10, vf10, vf11);
        EE_MMI_RGBA_PACK(packed);
        blended[0] = packed;
        link->effectHandle->color = blended[0];
        func_00160D88((u8 *)link->effectHandle);
    }
}

/* Clear the fade gate and report completion; the frame updater may overwrite it. */
u32 btlDisableBattleFade(void) {
    BtlActorWork *work;

    work = (BtlActorWork *)btlGetRuntime();
    work->fadeEnabled = 0;
    return 1;
}

SoundTask *sndCreateClearStateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlDisableBattleFade;
    task->taskId = 0x33;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

/* Set the fade gate and report completion; the frame updater may overwrite it. */
u32 btlEnableBattleFade(void) {
    BtlActorWork *work;

    work = (BtlActorWork *)btlGetRuntime();
    work->fadeEnabled = 1;
    return 1;
}

SoundTask *sndCreateSetStateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlEnableBattleFade;
    task->taskId = 0x34;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern char D_003A5008[];

extern char D_003A5020[];

/* Consume archive records only for enabled SYSEFF rows; clear unavailable entries. */
void sndLoadSysEffLb(void) {
    const char *path = D_003A5008;
    s32 archive = (s32)fileQueuePlainDispatchRequest(path);
    s32 node;
    u32 i;

    func_00288C50((struct FileRequest *)archive);
    btlBossDebugPrintf(D_003A5020, path);
    node = *(s32 *)(archive + 0x60);
    i = 0;
    while (node != 0) {
        if (D_0035F748[i].unk_00 != 0) {
            u32 unk08 = *(u32 *)(node + 8);
            u32 unk0C = *(u32 *)(node + 0xC);
            D_0035F748[i].unk_08 = unk08;
            D_0035F748[i].resource = unk0C;
            node = *(s32 *)node;
        } else {
            D_0035F748[i].resource = 0;
            D_0035F748[i].unk_08 = 0;
        }
        i++;
    }
    for (; i < BTL_SOUND_ENTRY_COUNT; i++) {
        D_0035F748[i].resource = 0;
        D_0035F748[i].unk_08 = 0;
    }
    func_00288788((void *)archive);
}

/* Register available SYSEFF handles; unavailable slots are left untouched. */
void btlRefreshSoundEntries(void) {
    u32 i;

    btlGetRuntime();
    for (i = 0; i < BTL_SOUND_ENTRY_COUNT; i++) {
        if (D_0035F748[i].resource != 0) {
            /* Bank entries retain their original data-address word representation. */
            btlCreateIndexedSoundResourceNode(i, (void *)D_0035F748[i].resource);
        }
    }
}

/* Free and clear slots selected by the archive table, without discarding metadata. */
void sndFreeBattleSoundEntries(void) {
    s32 context = btlGetRuntime();
    SoundResourceNode **node = ((BtlActorWork *)context)->soundResourceSlots;
    u32 i;

    for (i = 0; i < BTL_SOUND_ENTRY_COUNT; i++, node++) {
        if (D_0035F748[i].resource != 0) {
            sndFreeResourceNode(*node);
            *node = 0;
        }
    }
}

/* Register a borrowed archive source in its SYSEFF slot, without cloning it. */
void btlCreateIndexedSoundResourceNode(s32 slotIndex, void * handle) {
    u32 flags;
    s32 work;
    SoundResourceNode *node;

    work = btlGetRuntime();
    node = sndAllocResourceNode();
    flags = node->flags;
    node->sourceHandle = handle;
    ((BtlActorWork *)work)->soundResourceSlots[slotIndex] = node;
    node->flags = flags | 10;
}

typedef struct SoundHandleNode {
    struct FileQueue *queue;
    void *actor;
} SoundHandleNode;

struct FileQueue;

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(8);
    SoundBankEntry *entry = &D_0035F748[index];
    node->actor = actor;
    node->queue = fileCloneQueueEntries((struct FileQueue *)entry->resource);
    return node;
}

void btlUpdateJobPositionFromModel(s32 *args) {
    SoundHandleNode *node = (SoundHandleNode *)args;
    f32 pos[4];

    if (sdfLoadMapRecordPositionVector(((MdlCtx *)node->actor)->inner, 1) == 0) {
        mdlLoadPrimaryVectorVU((MdlCtx *)node->actor);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        pos[1] -= 150.0f;
    } else {
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
    }
    fileQueueSetPosition(node->queue, pos);
    fileQueueUpdate(node->queue);
}

void sndDestroyFileQueueWrapper(u32 arg0) {
    SoundHandleNode *node = (SoundHandleNode *)arg0;
    fileQueueDestroy(node->queue);
    sdfReleaseChipBlock(node);
}

void func_001F3230(void) {
}

void sndSetStationedSeVolume(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x58, 0x3f);
}

void sndSetStationedSeHighVolume(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

extern void func_0026A5F0(s32);

extern void mnuResetTitleStreamLocked(void);

extern s32 D_0035F998[];

extern u32 D_003BB6A8;

void sndStartBattleSceneStream(s32 adjustmentIndex, s32 sceneIndex) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    s32 selection = 0;

    if (adjustmentIndex != 0) {
        selection = D_003BAA3C[adjustmentIndex].streamSelection;
    }
    if (datBattleSceneRecords[sceneIndex].unk24 != 0) {
        selection = datBattleSceneRecords[sceneIndex].unk24;
    } else if (runtime->unk24A != 0) {
        selection = 2;
    }
    if (selection == 0) {
        selection = 1;
    }
    if (mnuPollTitleStreamStateLocked() != 0) {
        mnuResetTitleStreamLocked();
    }
    if (selection == 5) {
        func_0026A5F0(D_0035F998[D_003BB6A8 % 5]);
        D_003BB6A8++;
    } else {
        func_0026A5F0(selection - 1);
    }
}

u8 sndIsStreamStatusTwoOrThree(void) {
    s32 temp_v0;

    temp_v0 = mnuPollTitleStreamStateLocked();
    return temp_v0 - 2U < 2;
}

void btlResetTitleStreamOnBattleFlag(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    if ((*(u32 *)(temp_v0 + 500) & 0x10000) != 0) {
        mnuTitleStreamUpdateAndLogBgm();
        return;
    }
}

void btlAdvanceTitleState(void) {
    mnuAdvanceTitleStateUnderSemaphore();
}

void btlAdvanceTitleStateWithAudioCleanup(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    func_002E8E28();
    func_002E8E00();
}

void btlAdvanceTitleStateTask(void) {
    btlAdvanceTitleState();
}

void btlAdvanceTitleStateWithAudioCleanupTask(void) {
    btlAdvanceTitleStateWithAudioCleanup();
}

s32 sndLoadAndPlayStationedSe(u32 soundId) {
    s32 loaded = sndFindPackedTrackLoadStatus(soundId);
    if (loaded != 0) {
        sndSetStationedSeVolume(soundId);
        return 1;
    }
    return loaded;
}

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4CD8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4CF0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4D08);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4D20);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4D38);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4D50);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4D68);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4D80);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4D98);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4DB0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4DC8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4DE0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4DF8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4E10);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4E28);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4E40);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4E58);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4E70);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4E88);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4EA0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4EC0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4EE0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4F00);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4F18);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4F30);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4F48);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4F60);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4F78);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4F90);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4FA8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4FC0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4FD8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A4FF0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5008);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5020);

s32 sndPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (sndLoadAndPlayStationedSe(soundId)) {
        btlBossDebugPrintf("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

SoundTask *sndCreateStationedSeTask(u32 soundId) {
    SoundTask *task = (SoundTask *)btlAllocTask(4);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x55;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback.playSound = sndPlayStationedSe;
    *(u32 *)btlGetTaskArguments(task) = soundId;
    return task;
}

/* Return whether the independent active-node list contains a node with flag 8. */
s32 sndHasFlaggedActiveNode(void) {
    ActiveSoundNode *node = ((BtlActorWork *)btlGetRuntime())->soundList;
    while (node != 0) {
        if (node->flags & 8) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

s32 sndPlaySkillSeTask(u32 *arg0) {
    u8 *task = (u8 *)arg0;
    s32 context = btlGetRuntime();
    u32 value;

    if (*(u16 *)(task + 4) == 2 && *(u32 *)(context + 0x268) < 6) {
        btlBossDebugPrintf("btl:skill SE ignore[frame:%d]\n", *(u32 *)(context + 0x268));
        return 1;
    }
    *(u32 *)(context + 0x268) = 0;
    if (sndFindPackedTrackLoadStatus(*(u32 *)task) != 0) {
        switch (*(u16 *)(task + 4)) {
        case 1:
            value = *(u32 *)task;
            break;
        case 2:
            value = *(u32 *)task | 1;
            break;
        case 3:
            value = *(u32 *)task | 1;
            break;
        default:
            value = 0;
            break;
        }
        sndSetStationedSeVolume(value);
        btlBossDebugPrintf("btl:sound skillSE play[%X-%X]\n", value >> 16, value & 0xFFFF);
        return 1;
    }
    btlBossDebugPrintf("btl:sound load wait[%X]\n", *(u16 *)(task + 2));
    return 0;
}

SoundTask *sndCreateSkillSeTask(s32 skill, u16 variant) {
    SoundTask *task = (SoundTask *)btlAllocTask(8);
    u8 *data;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x52;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback.playSound = sndPlaySkillSeTask;
    data = btlGetTaskArguments(task);
    *(u32 *)data = *(u32 *)(skill + 8);
    *(u16 *)(data + 4) = variant;
    return task;
}

typedef struct SoundLoadNode {
    u32 flags;
    u8 state;
    u8 unk_05[3];
    u32 position;
} SoundLoadNode;

typedef struct SoundFileRequest {
    SoundLoadNode *node;
    void *handle;
    u32 resourceHandle;
    u32 blockIndex;
    const char *name;
} SoundFileRequest;

void sndStartFileLoad(SoundFileRequest *request) {
    SoundLoadNode *node = request->node;

    request->handle = fileQueueDefaultCallbackRequest(request->name);
    node->flags |= 1;
    node->position = (request->blockIndex + 0x200) << 16;
    node->state = 2;
    btlBossDebugPrintf("btl:sound file load start[%s]\n", request->name);
}

extern char D_003A50D8[];

extern char D_003A50F0[];

extern char D_003A5110[];

extern char D_003A5138[];

extern void func_002E9450(s32, s32);

u32 sndPollMotSeFileAndSpu(SoundFileRequest *request) {
    SoundLoadNode *node = request->node;
    if (sndHasActiveFileLoad()) {
        btlBossDebugPrintf(D_003A50D8);
        return 0;
    }
    if ((node->flags & 2) == 0) {
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)request->handle)) {
            s32 size;
            s32 data;
            btlBossDebugPrintf(D_003A50F0, request->name);
            request->resourceHandle = fileGetResourceHandle((struct FileRequest *)request->handle);
            size = (s32)fileGetResourceSize((struct FileRequest *)(u32)request->handle);
            data = sdfResourceRetainAddress((struct SdfMemBlock *)(request->resourceHandle));
            if (sndFindPackedTrackLoadStatus(node->position) == 0) {
                func_002E9450(data, size);
                node->flags |= 8;
                /* The packed position stores the sound block number in its upper halfword. */
                btlBossDebugPrintf(D_003A5110, (u16)(node->position >> 16), size);
            }
            node->flags = (node->flags & ~1) | 2;
        }
    } else if (sndFindPackedTrackLoadStatus(node->position) != 0) {
        btlBossDebugPrintf(D_003A5138, (u16)(node->position >> 16));
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(request->resourceHandle));
        filePollEntryCleanup((struct FileRequest *)(u32)request->handle);
        node->flags = (node->flags & ~8) | 0x10;
        return 1;
    }
    return 0;
}

u8 *sndCreateFileLoadTask(SoundLoadNode *node, u32 variant, const char *filename) {
    u8 *task = btlAllocTask(strlen(filename) + 20);
    SoundFileRequest *request;
    char *name;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x53;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(void **)(task + 0x48) = sndStartFileLoad;
    *(void **)(task + 0x4C) = sndPollMotSeFileAndSpu;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    request = btlGetTaskArguments(task);
    name = (char *)(request + 1);
    request->node = node;
    request->blockIndex = variant;
    request->name = name;
    strcpy(name, filename);
    return task;
}

s32 sndLoadDataFile(const SoundDataFileArgs *data) {
    char filename[0x70];
    if (sndIsCommandBusySigned()) {
        return 1;
    }
    sndFormatResourceNameFromUnitMode(data->unit, filename);
    sdfSoundSendNamedCommand(filename, 0x34);
    return 1;
}

BtlRuntimeTask *sndCreateDataFileLoadTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundDataFileArgs));
    SoundDataFileArgs *args;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = sndLoadDataFile;
    task->taskId = 0x56;
    task->ownerId = unit->identity;
    task->onStart = NULL;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    return task;
}

s32 sndIsCommandBusySigned(void) {
    return (s8)sdfSoundIsCommandBusy();
}

s32 sndHasResourceFlagsOneOrEight(ActiveSoundNode *node) {
    s32 temp_v0;

    temp_v0 = node->flags;
    if ((temp_v0 & 1) != 0) {
        return 1;
    }
    return (temp_v0 & 8) > 0;
}

void sndFormatResourceNameFromIndex(s32 index, char *output) {
    func_003014F0(output, D_003A5158, D_003BB6B0, (index + 0x200) & 0xffff);
}

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A50D8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A50F0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5110);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5138);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5158);

void sndFormatResourceNameFromUnitMode(const BtlUnit *unit, char *output) {
    func_003014F0(output, "MDD_%03X.ADB", unit->partyRecord.unitId);
}

s32 sndMapResourceType(s32 sound, s32 index) {
    s32 type = sndLookupResourceType(sound, index);
    if (type < 0x1A && type != 0) {
        if (type >= 0xB) {
            return type + *(s32 *)(btlGetRuntime() + 0x1E4) - 6;
        }
        return type + 0xFFFF;
    }
    return -1;
}

/* Allocate a cleared active-sound node and prepend it to its independent list. */
ActiveSoundNode *sndAllocListNode(void) {
    ActiveSoundNode *node = sdfAllocAndClearQuadwords(0x14);
    BtlActorWork *state = (BtlActorWork *)btlGetRuntime();
    ActiveSoundNode *first;

    node->previous = 0;
    first = state->soundList;
    if (first != 0) {
        first->previous = node;
        node->next = state->soundList;
    } else {
        node->next = 0;
    }
    state->soundList = node;
    return node;
}

/* Unlink and free an active-sound node without changing the resource-node list. */
void sndFreeListNode(ActiveSoundNode *node) {
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        ((BtlActorWork *)btlGetRuntime())->soundList = node->next;
    }
    sdfReleaseChipBlock(node);
}

/* Clear active-sound nodes, saving next before each allocation is released. */
void sndClearList(void) {
    ActiveSoundNode *node = ((BtlActorWork *)btlGetRuntime())->soundList;
    while (node != 0) {
        ActiveSoundNode *next = node->next;
        sndFreeListNode(node);
        node = next;
    }
}

typedef struct SoundSlotTableEntry {
    s16 resourceOffset;
    u16 fileId;
} SoundSlotTableEntry;

typedef struct SoundTaskArgs {
    BtlUnit *actor;
    s32 option;
    u32 slot;
    s32 waitFrames;
} SoundTaskArgs;

extern SoundSlotTableEntry *btlSelectSideIndexedActorParameterTable(s32, s32);

/* Return the category/id/slot's packed motion-SE key, or zero if unavailable. */
u32 sndBuildMotSeResourceKey(u32 *sound, u32 slot) {
    u32 id = ((SoundSlotOwner *)sound)->id;
    u32 category = ((SoundSlotOwner *)sound)->category;
    SoundSlotTableEntry *table = btlSelectSideIndexedActorParameterTable(category, id);
    s32 specialCategory = 1;
    s32 scaledId = id * 0x20;
    s32 offset;
    u32 resource = 0;

    table += slot;
    offset = table->resourceOffset;

    if (offset < 0) {
        return resource;
    }
    resource = (scaledId + offset + 0x500) << 16;
    if (category != specialCategory) {
        return resource;
    }
    resource = 0;
    if (slot >= 23) {
        return resource;
    }
    return (id * 0x10 + offset + 0x1000) << 16;
}

extern char D_003A5178[];

extern char D_003A5188[];

extern char D_003A5198[];

extern char D_003A51A8[];

/* Queue available motion-SE files into fileRequests, including slot 11's stream. */
void sndLoadMotSeFiles(u32 *sound) {
    char filename[0x70];
    u32 slot = 0;
    s32 offset = 0x10;
    u8 *handleTable = (u8 *)sound + 8;
    do {
        u32 id = sndBuildMotSeResourceKey(sound, slot);
        if (id != 0) {
            if (slot != 0xB) {
                func_003014F0(filename, D_003A5158, D_003BB6B0, id >> 16);
            } else if (sound[1] == 0) {
                func_003014F0(filename, D_003A5178, D_003A5188, sound[2]);
            } else {
                func_003014F0(filename, D_003A5198, D_003A5188, sound[2]);
            }
            *(u32 *)(handleTable + offset) = (u32)fileQueueDefaultCallbackRequest(filename);
            btlBossDebugPrintf(D_003A51A8, slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound[0] |= 1;
}

/* Find the shared category/id owner, returning null if absent. */
SoundSlotOwner *sndFindListNodeForChannel(s32 category, s32 id) {
    s32 context = btlGetRuntime();
    SoundSlotOwner *node = ((BtlActorWork *)context)->soundSlotOwners;
    while (node != 0) {
        if (node->category == category && node->id == id) {
            return node;
        }
        node = node->next;
    }
    return 0;
}

extern char D_003A51D0[];

/* Retain or register an owner; model flag 0xC0F suppresses initial file queuing. */
SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id) {
    SoundSlotOwner *node = sndFindListNodeForChannel(category, id);
    BtlActorWork *context;
    SoundSlotOwner *head;

    if (node != 0) {
        btlBossDebugPrintf(D_003A51D0, node);
        node->work.refCount++;
        return node;
    }
    node = sdfAllocAndClearQuadwords(0x108);
    node->category = category;
    node->id = id;
    node->work.refCount = 1;
    context = (BtlActorWork *)btlGetRuntime();
    node->prev = 0;
    head = context->soundSlotOwners;
    if (head != 0) {
        head->prev = node;
        node->next = context->soundSlotOwners;
    } else {
        node->next = 0;
    }
    context->soundSlotOwners = node;
    if (mdlFlagTest(0xC0F) == 0) {
        sndLoadMotSeFiles((u32 *)node);
    }
    return node;
}

/* The last reference cleans queued files and resource handles, then unlinks/frees. */
void sndReleaseSlotOwner(SoundSlotOwner *node) {
    u32 count = node->work.refCount - 1;
    node->work.refCount = count;
    if (count == 0) {
        u32 i = 0;
        u32 *resources = (u32 *)node->work.resourceHandles;
        u32 *requests = (u32 *)node->work.fileRequests;
        for (; i < 0x1D; i++, requests++, resources++) {
            if (*requests != 0) {
                filePollEntryCleanup((struct FileRequest *)(u32)*requests);
            }
            if (*resources != 0) {
                sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(*resources));
            }
        }
        if (node->next != 0) {
            node->next->prev = node->prev;
        }
        if (node->prev != 0) {
            node->prev->next = node->next;
        } else {
            ((BtlActorWork *)btlGetRuntime())->soundSlotOwners = node->next;
        }
        sdfReleaseChipBlock(node);
    }
}

/* Release once per owner; preserve the next link before a final release may free. */
void sndReleaseAllSlotOwners(void) {
    SoundSlotOwner *node = ((BtlActorWork *)btlGetRuntime())->soundSlotOwners;

    while (node != 0) {
        SoundSlotOwner *next = node->next;

        sndReleaseSlotOwner(node);
        node = next;
    }
}

void btlStartMoveOtherUnitsTask(void *owner, s32 soundId) {
    void *task = btlCreateMoveOtherUnitsTask(owner, soundId);
    btlStartTask(task);
}

/* Flag 8 denotes packed-track loading, distinct from queued motion-SE files. */
s32 sndHasActiveFileLoad(void) {
    SoundSlotOwner *node = ((BtlActorWork *)btlGetRuntime())->soundSlotOwners;
    while (node) {
        if (node->flags & 8) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

void btlQueueUnitSoundSlotFileLoad(SoundTaskArgs *args) {
    SoundSlotOwner *owner = args->actor->soundSlotOwner;
    SoundSlotTableEntry *table;
    SoundSlotTableEntry *entry;
    if (owner == 0) {
        return;
    }
    if (owner->flags & 1) {
        return;
    }
    if (!(owner->flags & 2)) {
        return;
    }
    if (owner->work.resourceHandles[args->slot] == 0) {
        return;
    }
    table = btlSelectSideIndexedActorParameterTable(owner->category, owner->id);
    entry = &table[args->slot];
    args->option = entry->fileId;
    if (args->slot != 0xB) {
        owner->work.pendingSoundId = sndBuildMotSeResourceKey(owner, args->slot);
        owner->work.pendingSlot = args->slot;
        owner->flags |= 4;
        owner->flags &= ~8;
        owner->flags &= ~0x10;
    }
}

extern s32 mnuGetSoundBufferStateLocked(void);

extern void mnuResetSoundBufferLocked(void);

extern void mnuReleaseSoundBufferLocked(void);

extern void mnuPrintTitleDebugBanner(void);

extern void func_0026ABA8(u32, u32, s32);

extern s32 func_003003F0(const char *, ...);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5178);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5188);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5198);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A51A8);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A51D0);

u32 sndPollMotionSePlayback(SoundTaskArgs *args) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    SoundSlotOwner *owner = args->actor->soundSlotOwner;
    SoundSlotWork *soundWork;
    u32 key;
    u32 data;
    u32 size;

    if (owner == 0) {
        return 1;
    }
    if (owner->flags & 1) {
        return 1;
    }
    if (!(owner->flags & 2)) {
        return 1;
    }
    soundWork = &owner->work;
    if (soundWork->resourceHandles[args->slot] == 0) {
        return 1;
    }
    if (args->slot != 0xB) {
        key = sndBuildMotSeResourceKey((u32 *)owner, args->slot);
        if (owner->flags & 0x10) {
            sndSetStationedSeHighVolume(key);
            btlBossDebugPrintf("btl:motSE play[%X-%X]\n", key >> 16, key & 0xFFFF);
            return 1;
        }
        key >>= 16;
        btlBossDebugPrintf("btl:motSE load wait[%X]\n", key);
    } else {
        if (work->unk264 >= 0xB) {
            if (mnuGetSoundBufferStateLocked() != 0) {
                mnuResetSoundBufferLocked();
                mnuReleaseSoundBufferLocked();
            }
            data = (void *)sdfMemoryGetBlockAddress((struct SdfMemBlock *)(u32)soundWork->resourceHandles[args->slot]);
            size = sdfMemoryGetBlockSize((struct SdfMemBlock *)(u32)soundWork->resourceHandles[args->slot]);
            func_0026ABA8(data, size, 2);
            func_003003F0("%%%%%%%%%%%%%%%% EARRING : %d\n", args->slot);
            mnuPrintTitleDebugBanner();
            work->unk264 = 0;
            btlBossDebugPrintf("btl:motSE play(ATRAC3)\n");
        } else {
            btlBossDebugPrintf("btl:motSE ignore(ATRAC3)[frame:%d]\n", work->unk264);
        }
        return 1;
    }
    if (args->waitFrames > 90) {
        owner->flags &= ~8;
        owner->flags |= 0x10;
        btlBossDebugPrintf("btl:motSE load time out[%X]\n", key);
        return 1;
    }
    args->waitFrames++;
    return 0;
}

void *btlCreateMoveOtherUnitsTask(u8 *owner, u32 soundId) {
    u8 *task = btlAllocTask(16);
    SoundTaskArgs *arguments;

    task[0x10] = BTL_TASK_CONDITION_NEVER;
    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x54;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlQueueUnitSoundSlotFileLoad;
    *(void **)(task + 0x4C) = sndPollMotionSePlayback;
    arguments = btlGetTaskArguments(task);
    arguments->actor = (BtlUnit *)owner;
    arguments->slot = soundId;
    arguments->option = 0;
    arguments->waitFrames = 0;
    return task;
}

void func_001F4430(void) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();

    work->unk264 = -1;
    work->unk268 = -1;
}

void sndLoadBattleBank(void) {
    if (sndFindPackedTrackLoadStatus(0x10000) == 0) {
        sndEnsureMidiBankResident(0x10000);
        btlBossDebugPrintf("btl:sound load BSE SMG\n");
    }
}

u8 sndIsBattleBankLoaded(void) {
    s64 temp_v0;

    temp_v0 = sndFindPackedTrackLoadStatus(0x10000);
    return temp_v0 != 0;
}

void func_001F44C0(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    SoundSlotOwner *owner;
    s32 tracksReady;

    for (owner = state->soundSlotOwners; owner != NULL; owner = owner->next) {
        if ((owner->flags & 2) == 0) {
            u32 slot;
            s32 requestsPending = 0;

            for (slot = 0; slot < 0x1D; slot++) {
                if (owner->work.fileRequests[slot] != 0) {
                    if (fileIsRequestReadyInCurrentMode(
                            (struct FileRequest *)(u32)owner->work.fileRequests[slot]) != 0) {
                        u32 resource = fileGetResourceHandle(
                            (struct FileRequest *)(u32)owner->work.fileRequests[slot]);

                        struct FileRequest *completedRequest =
                            (struct FileRequest *)(u32)owner->work.fileRequests[slot];

                        owner->work.resourceHandles[slot] = resource;
                        filePollEntryCleanup(completedRequest);
                        owner->work.fileRequests[slot] = 0;
                    } else {
                        requestsPending = 1;
                    }
                }
            }
            if (requestsPending == 0) {
                owner->flags = (owner->flags & ~1) | 2;
                btlBossDebugPrintf("btl:motSE file load all end[%p]\n", owner);
            }
        }
    }

    if ((s32)state->motionSeLoadFrame >= 0) {
        state->motionSeLoadFrame++;
    }
    if ((s32)state->skillSeLoadFrame >= 0) {
        state->skillSeLoadFrame++;
    }

    tracksReady = 1;
    for (owner = state->soundSlotOwners; owner != NULL; owner = owner->next) {
        if (owner->flags & 8) {
            u32 loaded = sndFindPackedTrackLoadStatus((u32)owner->work.pendingSoundId);

            if (loaded != 0) {
                u32 flags = owner->flags;

                if (flags & 8) {
                    tracksReady = 0;
                }
                owner->flags = (flags & ~8) | 0x10;
            } else {
                tracksReady = 0;
            }
        }
    }

    if (tracksReady != 0) {
        if (sndHasFlaggedActiveNode() != 0) {
            btlBossDebugPrintf("btl:motSE wait[skillSE]\n");
        } else {
            for (owner = state->soundSlotOwners; owner != NULL; owner = owner->next) {
                if (owner->flags & 4) {
                    SoundSlotWork *work = &owner->work;
                    struct SdfMemBlock *block =
                        (struct SdfMemBlock *)(u32)work->resourceHandles[owner->work.pendingSlot];
                    s32 size = sdfMemoryGetBlockSize(block);
                    u32 address = sdfMemoryGetBlockAddress(
                        (struct SdfMemBlock *)(u32)work->resourceHandles[owner->work.pendingSlot]);

                    func_002E9450((s32)address, size);
                    owner->flags = (owner->flags & ~4) | 8;
                    owner->flags &= ~0x10;
                    break;
                }
            }
        }
    }
}

/* Return whether a not-yet-file-ready owner still has an outstanding request. */
s32 sndHasOccupiedNodeSlots(void) {
    s32 context = btlGetRuntime();
    SoundSlotOwner *node = ((BtlActorWork *)context)->soundSlotOwners;
    while (node != 0) {
        if ((node->flags & 2) == 0) {
            u32 index = 0;
            u32 *requests = (u32 *)node->work.fileRequests;
            for (; index < 0x1D; index++) {
                if (*requests != 0) {
                    return 1;
                }
                requests++;
            }
        }
        node = node->next;
    }
    return 0;
}

u32 sndFinishEarringPlayback(void) {
    s32 status = mnuGetSoundBufferStateLocked();
    if (status == 0) {
        return 1;
    }
    if (status == 2) {
        func_003003F0("%%%%%%%%%%%%%%%% EARRING(2)\n");
        mnuPrintTitleDebugBanner();
    }
    return 0;
}

SoundTask *sndCreateEarringTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = sndFinishEarringPlayback;
    task->taskId = 0x57;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

typedef struct BattleVoiceLoad {
    u32 request;
    s32 state;
    s32 index;
} BattleVoiceLoad;

extern BattleVoiceEntry D_00377650[];

extern char D_003A5328[];

extern char D_003A5340[];

extern char D_003A5358[];

s32 sndPollAtrac3SELoadTask(BattleVoiceLoad *args) {
    char path[0x80];
    s32 resource;
    u32 data;
    u32 size;

    if (args->state == 0) {
        func_003014F0(path, D_003A5328, D_00377650[args->index].fileName);
        args->request = (u32)fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A5340, path);
    } else if (fileIsRequestReadyInCurrentMode((struct FileRequest *)args->request) != 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        resource = fileGetResourceHandle((struct FileRequest *)args->request);
        data = (u32)sdfResourceRetainAddress((struct SdfMemBlock *)(resource));
        size = (s32)fileGetResourceSize((struct FileRequest *)(u32)args->request);
        filePollEntryCleanup((struct FileRequest *)(u32)args->request);
        func_0026ABA8(data, size, D_00377650[args->index].volume);
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(resource));
        btlBossDebugPrintf(D_003A5358);
        return 1;
    }
    args->state++;
    return 0;
}

void *sndCreateAtracEffectLoadTask(u32 owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x20) = 0x58;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(void **)(task + 0x4C) = sndPollAtrac3SELoadTask;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    arguments = btlGetTaskArguments(task);
    arguments[0] = 0;
    arguments[1] = 0;
    arguments[2] = owner;
    return task;
}

typedef struct BtlDeadLoadArgs {
    BtlUnit *unit;
    u32 request;
    u32 resource;
} BtlDeadLoadArgs;

extern char D_003A5370[];

void sndStartDeadAtracLoad(BtlDeadLoadArgs *args) {
    u8 *work = (u8 *)btlGetRuntime();
    BtlUnit *unit;
    s32 id;
    char path[0x70];

    if (*(u16 *)(work + 0x260) == 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        unit = args->unit;
        if (unit->status.flags & 0x200) {
            id = unit->partyRecord.unitId;
            if (unit->status.flags & 0x1000) {
                id += 0x10;
            }
            func_003014F0(path, D_003A5178, D_003A5188, id);
        } else {
            func_003014F0(path, D_003A5198, D_003A5188, unit->partyRecord.unitId);
        }
        args->request = (u32)fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A5370, path);
    }
    ++*(u16 *)(work + 0x260);
}

extern char D_003A5390[];

extern char D_003A53B0[];

extern char D_003A53D0[];

u32 sndUpdateEarringDeadPlayback(u32 *args) {
    u32 resource;
    u32 data;
    u32 size;

    if (args[1] == 0) {
        return 1;
    }
    if (args[2] == 0) {
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)args[1]) != 0) {
            resource = fileGetResourceHandle((struct FileRequest *)args[1]);
            args[2] = resource;
            data = sdfResourceRetainAddress((struct SdfMemBlock *)(resource));
            size = (s32)fileGetResourceSize((struct FileRequest *)(u32)args[1]);
            filePollEntryCleanup((struct FileRequest *)(u32)args[1]);
            func_0026ABA8(data, size, 2);
            mnuPrintTitleDebugBanner();
            func_003003F0(D_003A5390);
            btlBossDebugPrintf(D_003A53B0);
        }
        return 0;
    }
    if (mnuGetSoundBufferStateLocked() == 0) {
        btlBossDebugPrintf(D_003A53D0);
        return 1;
    }
    return 0;
}

void sndFinishEarringPlaybackTask(u32 *sound) {
    u8 *state = (u8 *)btlGetRuntime();
    if (sound[2]) {
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(sound[2]));
    }
    --*(u16 *)(state + 0x260);
}

void *sndCreateEarringPlaybackTask(u8 *owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 0x59;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = sndStartDeadAtracLoad;
    *(void **)(task + 0x4C) = sndUpdateEarringDeadPlayback;
    *(void **)(task + 0x50) = sndFinishEarringPlaybackTask;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    arguments[2] = 0;
    return task;
}

u32 btlPlayStationedSe1C(void) {
    sndLoadAndPlayStationedSe(0x1c);
    return 1;
}

SoundTask *btlCreateStationedSe1CTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlPlayStationedSe1C;
    task->taskId = 0x5A;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

u32 btlAdvanceTitleStateAfterSound(void) {
    btlAdvanceTitleState();
    return 1;
}

SoundTask *btlCreateAdvanceTitleStateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback.process = btlAdvanceTitleStateAfterSound;
    task->taskId = 0x5B;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern f32 D_0035F9B0[4];

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
    for (unit = battle->units; unit != 0; unit = unit->next) {
        s32 flags = unit->status.flags;
        if (flags & 0x200) {
            actors[count++] = unit;
            if ((flags & 1) || battle->cameraPresetMode == 3) {
                markedCount++;
            }
        }
    }
    func_001F66D8(0x400, 0, 0);
    VU0_STORE_VF_UNCLOBBERED(vf10, target);
    for (i = count - 1; i >= 0; i--) {
        unit = actors[i];
        if (!(unit->status.flags & 0x80000)) {
            PCP_COPY_VECTOR(unit->rotation, D_0035F9B0);
        } else if (unit->status.flags & 0xE0) {
            PCP_COPY_VECTOR(unit->rotation, D_0035F9B0);
        } else {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            if (btlAimHorizontalDirectionVU(position, target) != 0) {
                VU0_STORE_VF_UNCLOBBERED(vf10, direction);
                btlSetUnitRotation(unit, direction);
            }
        }
    }
    if (battle->cameraPresetMode >= markedCount) {
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
    if (battle->cameraPresetMode < markedCount) {
        battle->cameraPresetMode = markedCount;
        return 1;
    }
    return 0;
}

extern f32 D_0035F9C0[4];

extern f32 D_0035F9D0[4];

extern f32 func_002F9CD0(f32);

s32 btlArrangeFormationSlots(s32 filter) {
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
    for (unit = battle->units; unit != 0; unit = unit->next) {
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
        PCP_COPY_VECTOR(rotation, D_0035F9C0);
    } else {
        radius = 1000.0f;
        maxSpacing = 100.0f;
        minSpacing = 50.0f;
        PCP_COPY_VECTOR(rotation, D_0035F9D0);
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
    spacingAngle = func_002F9CD0(spacing / radius);
    totalArcAngle = -spacingAngle;
    for (i = 0; i < count; i++) {
        unit = actors[i];
        distance = unit->unkBC * unit->scale;
        halfAngle = func_002F9CD0(distance / radius);
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
        halfAngle = func_002F9CD0(unit->unkBC * unit->scale / radius);
        angle += halfAngle;
        position[0] = sdfSinPoly(angle) * radius;
        position[1] = 0.0f;
        position[2] = (distance - sdfEvaluateCosineViaSinePhaseShift(angle) * radius) * direction;
        PCP_COPY_VECTOR(unit->position, position);
        btlSetUnitPosition(unit, position);
        PCP_COPY_VECTOR(unit->rotation, rotation);
        angle += halfAngle;
        btlSetUnitRotation(unit, rotation);
        angle += spacingAngle;
    }
    if (battle->postPlacementCallback != 0) {
        battle->postPlacementCallback();
    }
    return 1;
}

extern s32 btlExpandPartyFormationAroundCenter(f32 *);

s32 btlRepositionPartyAroundBattleCenter(void) {
    f32 vector[4];
    u8 *context = (u8 *)btlGetRuntime();
    PCP_COPY_VECTOR(vector, context);
    return btlExpandPartyFormationAroundCenter(vector);
}

s64 func_001F53F0(void) {
    return btlArrangeFormationSlots(0x400);
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

extern f32 sdfSinPoly(f32);

/* Place the three actor slots around the common battle center supplied in vf10. */
void btlPlaceTripleFormationAroundCenter(BtlTask *link, BtlUnit *first, BtlUnit *second) {
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
    radius = func_001F66D8(0x400, 0, 0) + 200.0f;
    VU0_STORE_VF_UNCLOBBERED(vf10, center);
    pos[0] = center[0];
    pos[1] = 0.0f;
    pos[2] = center[2] - radius;
    btlSetUnitPosition(slot[1], pos);
    if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[1], (s128 *)rotation);
    }
    angle = 30.0f * 0.017453293f;
    pos[0] = center[0] - sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition(slot[0], pos);
    if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[0], (s128 *)rotation);
    }
    pos[0] = center[0] + sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition(slot[2], pos);
    if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[2], (s128 *)rotation);
    }
}

extern void btlFlagAllUnitsDefeatCandidate(void);

extern void btlClearMatchingUnitDefeatCandidates(s32);

extern u8 D_0037E100[];

void btlPlaceTripleFormationAroundTarget(BtlTask *link, BtlUnit *first, BtlUnit *second) {
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
        VU0_LOAD_VF(vf10, (u8 *)target + 0x70);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition(slot[1], pos);
        btlUnitFaceTarget(slot[1], target);
        VU0_LOAD_VF(vf10, D_0037E100);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition(slot[0], pos);
        btlUnitFaceTarget(slot[0], target);
        VU0_LOAD_VF(vf10, D_0037E100);
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

/* vu0 routine: Place three indexed actors around the middle actor's facing and muzzle. */
void btlPlaceTripleFormationAroundMiddleActor(BtlTask *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 dir[4];
    f32 rotation[4];
    f32 radius;
    u32 i;

    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(link->unit->status.flags & 0x600);
    slot[0] = NULL;
    slot[1] = NULL;
    slot[2] = NULL;
    slot[link->unit->lookupId] = link->unit;
    slot[first->lookupId] = first;
    slot[second->lookupId] = second;
    VU0_LOAD_VF(vf10, &slot[1]->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
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
    for (i = 0; i < 3; i++) {
        radius = slot[i]->unkBC * slot[i]->scale;
        radius += 100.0f;
        if (i != 1) {
            if (i == 0) {
                func_002DD688(2.094395f);
            } else if (i == 2) {
                func_002DD688(-2.094395f);
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
        if (btlAimHorizontalDirectionVU(pos, center) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
            btlSetUnitRotation(slot[i], rotation);
        }
    }
}

void btlOrientFrontAndBackUnitsTowardTargets(BtlTask *link, BtlUnit *a, BtlUnit *b) {
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
        func_001F66D8(target->status.flags & 0x600, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU(&vec[0], &vec[1]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation(front, &vec[2]);
        }
    }
    btlUnitFaceTarget(back, front);
}

/* vu0 routine: measure the center actor's displacement from its sole target. */
void btlAlignTripleFormationWithTarget(BtlTask *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 position[4];
    f32 targetPosition[4];
    u8 *runtime;
    BtlUnit *target;
    f32 offsetX;
    f32 offsetZ;
    u32 i;

    runtime = (u8 *)btlGetRuntime();
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
        btlUnitGetMuzzlePosVU(slot[1]);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlUnitGetMuzzlePosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, targetPosition);
        VU0_LOAD_VF(vf11, position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_GET_VF10_X(offsetX);
        targetPosition[2] -= target->reach * target->scale;
        offsetZ = -(500.0f - (targetPosition[2] - position[2]));
        if (*(u32 *)(runtime + 0x1F8) & 0x200) {
            offsetZ = 0.0f;
        }
        for (i = 0; i < 3; i++) {
            func_001D6300((u8 *)slot[i], position);
            position[0] += offsetX;
            position[2] += offsetZ;
            btlSetUnitPosition(slot[i], position);
            btlUnitFaceTarget(slot[i], target);
        }
    }
}

void func_001F5D00(BtlTask *link, BtlUnit *first, BtlUnit *second) {
}

enum {
    BATTLE_FORMATION_ACTION_FIRST = 0x1AB,
    BATTLE_FORMATION_ACTION_COUNT = 0x55
};

typedef struct BattleFormationActionRow {
    u16 unknown00;
    u16 kind;
    s8 actorSelector[3];
    u8 unknown07;
} BattleFormationActionRow;

typedef char BattleFormationActionRowSizeCheck[(sizeof(BattleFormationActionRow) == 8) ? 1 : -1];

typedef struct BattleFormationActionTable {
    BattleFormationActionRow rows[BATTLE_FORMATION_ACTION_COUNT];
} BattleFormationActionTable;

typedef char BattleFormationActionTableSizeCheck[(sizeof(BattleFormationActionTable) == 0x2A8) ? 1 : -1];

INCLUDE_ASM(const s32, "game/code_001DDF20", func_001F5D08);

typedef struct BattleFormationActionArgs {
    BtlTask *link;
    BtlUnit *first;
    BtlUnit *second;
    u32 actionId;
    u32 unk10;
} BattleFormationActionArgs;

extern BattleFormationActionTable *D_003BAA64;

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5328);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5340);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5358);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5370);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5390);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A53B0);

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A53D0);

u32 func_001F5ED8(BattleFormationActionArgs *args) {
    BtlTask *link;
    BtlUnit *first;
    BattleFormationActionRow *row;
    BtlState *runtime;

    runtime = (BtlState *)btlGetRuntime();
    first = args->first;
    if (first == 0) {
        if (args->second == 0) {
            return 1;
        }
    }

    link = args->link;
    if (link->unit->status.flags & 0x400) {
        row = (BattleFormationActionRow *)((u8 *)D_003BAA64 +
              args->actionId * sizeof(BattleFormationActionRow) -
              BATTLE_FORMATION_ACTION_FIRST * sizeof(BattleFormationActionRow));
        if (row->kind == 7) {
            func_001F5D00(args->link, first, args->second);
        }
        return 1;
    }

    row = (BattleFormationActionRow *)((u8 *)D_003BAA64 +
          args->actionId * sizeof(BattleFormationActionRow) -
          BATTLE_FORMATION_ACTION_FIRST * sizeof(BattleFormationActionRow));
    switch (row->kind) {
    case 0:
        break;
    case 1:
        if (runtime->commandRestrictFlags & 0x200) {
            break;
        }
        btlPlaceTripleFormationAroundCenter(link, first, args->second);
        break;
    case 2:
        if (runtime->commandRestrictFlags & 0x200) {
            break;
        }
        btlPlaceTripleFormationAroundTarget(link, first, args->second);
        break;
    case 3:
        btlPlaceTripleFormationAroundMiddleActor(link, first, args->second);
        break;
    case 4:
        btlOrientFrontAndBackUnitsTowardTargets(link, first, args->second);
        break;
    case 5:
        break;
    case 6:
        btlAlignTripleFormationWithTarget(link, first, args->second);
        break;
    default:
        break;
    }
    return 1;
}

extern u32 func_001F5ED8(BattleFormationActionArgs *);

u8 *btlCreateSoundPlaybackTask(u8 *owner, u32 soundId, u32 variant, u32 channel, u32 flags) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;
    u8 *sound;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = func_001F5ED8;
    sound = *(u8 **)(owner + 0x18);
    *(u16 *)(task + 0x20) = 0x5F;
    *(u32 *)(task + 0x48) = 0;
    *(u64 *)(task + 0x40) = *(u64 *)(sound + 0x108);
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = soundId;
    arguments[2] = variant;
    arguments[3] = channel;
    arguments[4] = flags;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001DDF20", D_003A5410);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB670);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB678);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB680);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB690);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB694);

INCLUDE_SDATA(const s32, "game/code_001DDF20", btlTintTransitionHoldCount);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB6A0);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB6A8);

INCLUDE_SDATA(const s32, "game/code_001DDF20", D_003BB6B0);

