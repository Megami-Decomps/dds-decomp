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
#include "mdl_asset_request.h"
#include "file_request_api.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "dat_command.h"

/* SYSEFF metadata and runtime registrations share these indices. */
enum {
    BTL_SOUND_ENTRY_COUNT = 0x31,
    BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT = 0x2C
};

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern void effObjSetOpacityPassEnabled(u32 enabled);

extern void btlClearAllUnitDefeatCandidates(void);

extern void func_001F3C30(BtlLinkedCommand *action);

extern s64 mnuGetSoundBufferStateLocked(void);

extern void func_00336538(f32);

extern void func_003364B8(f32);

extern u8 effSharedRandomState[];

extern void func_001EC5F0(BtlLinkedCommand *);

extern void func_001EF030(void *, void *);

extern void func_001EC868(BtlLinkedCommand *, BtlCamState *, f32);

extern void func_001ECBF8();

extern void btlResetUnitEffectVector(BtlLinkedCommand *, BtlCamState *);

typedef struct BtlUnit BtlUnit;

extern void func_001F2758(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

/* Metadata records reached through the battle table pointers. */
typedef struct BtlActionTableEntry {
    u8 kind;               /* 0x00 */
    u8 pad01[2];
    u8 resourceType;       /* 0x03 */
    u8 pad04[4];
    f32 lightColorMode;
    f32 lightColor[3];
    s32 defaultValue;      /* 0x18 */
    u16 flags;             /* 0x1C */
    u8 pad1E[2];
} BtlActionTableEntry;

typedef struct SoundEntry {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} SoundEntry;

extern SoundEntry D_003BDE18[];

extern u8 D_003BD7D0[];

/* Two camera vectors are written at work+0x50 and work+0x60 by btlInitializeSceneLightingAndTint. */
typedef struct BtlCameraVectors {
    u8 pad00[0x50];
    f32 eye[3];
    u8 pad5C[4];
    f32 target[3];
} BtlCameraVectors;

extern struct BtlRuntimeTask *btlCreateHookedUnitSoundTask();

extern u32 D_00436AD4;

extern u32 dds3AdvanceWorldCounter(void);

extern struct EffWorldNode *evtSpawnActionObj9(s32);

extern s8 btlSetActorEffectParameter(BtlUnit *, s32);

extern s32 mdlFlagTest(u32);

extern s32 btlGetRuntime(void);

extern f32 func_001F5780(BtlUnit *, u8, f32, f32);

extern f32 func_001FDD20(f32 *, f32, f32, f32 *);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 kwlnDrawControlFlags;

extern s32 mnuPollTitleStreamStateLocked(void);

extern void mnuResetTitleStreamLocked(void);

extern void func_002A2200(s32);

extern s32 D_00435E0C;

extern s32 sndFindPackedTrackLoadStatus(u32);

typedef struct SceneLightRestoreArgs { u32 value; } SceneLightRestoreArgs;

extern s64 func_00201520(SceneLightRestoreArgs *);

extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);

extern s64 func_00201718(const BtlTintReleaseArgs *args);

extern s32 sndPlaySkillSeTask(u32 *);

extern SoundResourceNode *sndAllocResourceNode(void);


extern s32 datActionAnimationRecords;

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern void btlBossDebugPrintf(const char *format, ...);

extern f32 func_00208000(s32, f32 *, f32 *);

extern s32 func_001E3230(BtlUnit *, s32);

extern s32 func_0035C860(char *, const char *, ...);

extern char D_004192E8[]; /* "MDD_%03X.ADB" */

extern char D_004192D8[];

extern char D_00436AE8[];

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);

typedef struct SoundCommand {
    u32 handle;
    u32 resource;
    u16 currentId;
    u16 nextId;
} SoundCommand;

extern SoundCommand D_003BDC90;

extern u8 D_003BDCA0[];

typedef struct BtlTintTransition {
    u32 currentColor;
    u8 unk_04[0x14];
    u32 sourceColor;
    u32 targetColor;
    u16 framesRemaining;
    u16 durationFrames;
} BtlTintTransition;


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

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern void effMiscQuaternionToMatrixVU(void);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_003E9130[];

typedef struct BtlCameraTimedInstruction {
    s32 kind;
    s16 parameterIndex;
    u8 pad06[2];
    f32 startFrame;
    f32 duration;
} BtlCameraTimedInstruction;

extern const BtlCameraTimedInstruction *D_003BBF70[];

extern const BtlCameraTimedInstruction *D_003BBF88[];

extern const BtlCameraTimedInstruction *D_003BBFA8[];

extern const BtlCameraTimedInstruction *D_003BC090[];

extern const BtlCameraTimedInstruction *D_003BC0A0[];

extern const BtlCameraTimedInstruction *D_003BC0C0[];

extern const BtlCameraTimedInstruction *D_003BC0C8[];

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void btlBuildGroupFramingCameraPose(BtlCamState *, BtlCamState *);

extern void func_001F3E48(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern void func_001FA480(BtlLinkedCommand *, BtlCamState *, const BtlCameraTimedInstruction *);

extern void func_001FBAC0(BtlLinkedCommand *, BtlCamState *);

extern s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *, BtlCamState *, s8, s8);

typedef struct SoundCursor {
    u16 unk_00;
    s16 frame;
    s16 mode;
    s16 index;
    union {
        u16 unk_08;
        s8 category;
    };
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
    u8 pad10[4];
    s8 busy; /* 0x14: nonzero suspends camera endpoint updates. */
    u8 pad15[0xB];
    f32 pathEnd[4]; /* 0x20 */
    f32 pathStart[4]; /* 0x30 */
    u8 pad40[0x40];
    f32 direction[4]; /* 0x80 */
    f32 distance; /* 0x90 */
    u8 pad94[0x9C];
} SoundCursor;

#define CURSOR ((SoundCursor *)D_003BD7D0)

extern char D_00418C58[];

extern s32 sndHasActiveFileLoad(void);

extern void func_003422F8(s32, s32);

typedef struct BattleFieldBlocks {
    u8 unk_00[0x2B8];
    s32 fieldF1;
    s32 fieldF2;
    s32 fieldTB;
} BattleFieldBlocks;

extern f32 *D_0037F770[];

extern u8 kwlnDefaultColorVector[];

extern void fldApplyLightSetCurrent(void);

extern f32 *D_0037F770[];

struct FileQueue;

extern void btlUnitGetMuzzlePosVU(BtlUnit *);

extern s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *);

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern void func_001F5868(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void func_001F5320(BtlLinkedCommand *, BtlCamState *, s32, s32);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern s32 func_001FF5D8(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void func_001F4F10(BtlLinkedCommand *, BtlCamState *);

extern void func_001F5018(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceCommandCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern SdfFlagListParams D_003BDCC8;

extern void mnuReleaseSoundBufferLocked(void);

extern void func_002A27A8(s32, s32, u8);

extern s32 btlGetLoggedIndexedCommandItem(s32);

extern BtlRuntimeTask *btlFindTaskByHandle(u64);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id);

extern void btlSetUnitPosition(BtlUnit *unit, f32 *position);

extern void btlSetUnitRotation(BtlUnit *unit, s128 *rotation);

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern u32 effMiscRandMod(void *state, u32 modulus);

extern s32 sdfLoadMapRecordPositionVector(SdfModel *, s32);

extern void mdlLoadPrimaryVectorVU(MdlCtx *);

extern void effMiscQuatMultiplyVU(void);

extern s32 btlAimHorizontalDirectionVU(f32 *, f32 *);

extern void btlUnitGetBodyPosVU(BtlUnit *);

extern void btlSetUnitRotation(BtlUnit *, s128 *);

extern s32 btlAimHorizontalDirectionClampedVU(f32 *, f32 *, f32);

typedef struct {
    u8 unk00[0x110];
    u32 flags;
    u8 unk114[0x10];
    u16 objectId;
} BattleEntryHeader;

extern void effMiscQuaternionToMatrixVU(void);

extern void func_00168978(BattleEffect *);

extern void func_002034A8(struct SoundResourceLink *);

extern s32 btlGetSelectedUnitProperty(BtlUnit *);


extern void *memset(void *, s32, u32);

extern void sndFreeResourceNode(struct SoundResourceNode *);


extern void sndFreeListNode(struct ActiveSoundNode *);

extern f32 btlUnitGetTopY(BtlUnit *);

static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

extern s32 btlIsActorCategoryMarked(s32);

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001F20B0(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void func_001F20C8(BtlCamState *, BtlCamState *, BtlCamState *);

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);

extern void btlSetupActionCameraPair(BtlLinkedCommand *);

extern void btlAdvanceUnblockedPlayerCursorAnimation(BtlLinkedCommand *);

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern extern f32 btlGetActorStateScale(ActionStateLink *);

void func_001ED9A0();

extern void func_001F34E0(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

/* Test whether the operand is empty, subject to command-category and slot-kind exclusions. */ s32 btlActionEntryIsEmpty(s32 index, BtlOperandGroup *slot, BtlOperandEntry *entry);

/* Adjust the pose direction when projected camera height is above Y = -20. */ s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state);

/* Adjust the pose direction using the supplied horizontal height plane. */ s32 btlAdjustCameraDirectionForPlane(BtlCamState *state, f32 height);

void btlAimLinkedUnitAtMuzzle(BtlLinkedCommand *action);

/* Allocate a native header followed by capacity pointer entries. */ BtlIndexList *btlAllocateIndexList(s32 capacity);

/* The caller keeps the live count within the allocated capacity. */ void btlAppendIndexListEntry(BtlIndexList *list, void *entry);

void btlApplyCombinedActorFlags(u8 *fx);

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode);

s32 btlCanUseActorCategoryFlag2(s32 actor);

s32 btlCanUseActorCategoryFlag4(s32 actor);

s32 btlCanUseLinkedActor(s32 actor);

void btlClearUnitDefeatCandidate(BtlUnit *unit);

void btlCopyMotionTransform(BtlCamState *dst, BtlCamState *src);

void btlCopyUnitRotationQuaternion(BtlUnit *unit, void *dst);

void btlFlagLinkedGroupDefeatCandidatesTask(s32 action);

void btlFlagUnitDefeatCandidate(BtlUnit *unit);

void btlFreeIndexList(BtlIndexList *list);

u32 btlGetActiveUnitId(void);

/* Return the number of live entries, not the allocated capacity. */ u32 btlGetIndexListCount(BtlIndexList *list);

/* The caller supplies an in-range index. */ void *btlGetIndexListEntry(BtlIndexList *list, s32 index);

/* Return the argument address recorded by allocation (zero for no arguments). */ void *btlGetTaskArguments(void *task);

s32 btlGetUnitModelFrameCount(BtlUnit *unit);

f32 btlGetUnitModelValue1C(BtlUnit *unit);

void btlGetUnitWorldPos(BtlUnit *unit, f32 *dst);

s32 btlHasActorCategoryFlag100(BtlLinkedCommand *action);

/* Find a type-two linked group whose associated unit is not disabled. */ s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor);

s32 btlHasFirstLinkedCategoryFlag1000(BtlLinkedCommand *actor);

/* Find an unmarked linked kind-two slot whose unit is not disabled. */ s32 btlHasIdleLinkedSlotKindTwo(u8 *actor);

/* Return whether a live linked group has its byte at 0x10 marked. */ s32 btlHasMarkedEntry10(s32 actor);

void btlInitMotionTransformFromComponents(BtlCamState *object, f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 vw, f32 fovDegrees);

void btlInterpolateVectorStep(f32 *src);

s32 btlIsSpecialActorCategory(s32 actor);

s32 btlMapActorCategory(s32 actor);

void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

/* Forward the packed model-color word and its scalar to the inner resource list. */ void btlReleaseUnitModelColorResource(BtlUnit *unit, u32 value, f32 scalar);

void btlSetActorEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode);

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode);

/* Install a fresh handle/reset phase counters, invoke startup, then reread handle. */

s32 btlStepPoseBlend(BtlLinkedCommand *command);

void btlUnitFaceTarget(BtlUnit *unit, BtlUnit *target);

void btlUnitFaceTargetScaled(BtlUnit *unit, BtlUnit *target, f32 scale);

void func_001E3108(void *object, f32 *dst);

u8 func_001EA940(BtlLinkedCommand *action);

u32 func_001EAA00(BtlLinkedCommand *action);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EB5B0);

void btlAdvanceActorStageAndPose(BtlLinkedCommand *action) {
    BtlState *work;
    s32 category;
    u8 *out;
    if (func_001EAA00(action) != 0) {
        func_001EC5F0(action);
        return;
    }
    work = (BtlState *)btlGetRuntime();
    if (work->unk66C != 0 && work->unk66C((BtlUnit *)action) != 0) {
        return;
    }
    category = btlMapActorCategory((s32)action);
    if (category > 0 && category == action->stageCount++) {
        btlClearRuntimeFlag2000();
        if (work->unk658 != 0 && work->unk658((BtlUnit *)action) != 0) {
            return;
        }
        if (btlIsSpecialActorCategory((s32)action) != 0) {
            func_001EC868(action, &action->camera, 0.0f);
            func_003364B8(-(10.0f * 0.017453293f));
            VU0_STORE_VF_UNCLOBBERED(vf10, action->camera.direction);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF(vf10, action->camera.direction);
            action->camera.distance += 150.0f;
            out = (u8 *)&action->backCamera;
        } else {
            u8 *pose = (u8 *)&action->frontCamera;
            out = (u8 *)&action->backCamera;
            func_001EF030(action, pose);
            btlCopyMotionTransform((BtlCamState *)out, (BtlCamState *)pose);
            action->backCamera.distance += 100.0f;
            action->flags |= 0x41;
            action->state = 0;
            action->motionParameter = 30.0f;
        }
        btlAdjustCameraDirectionForDefaultPlane((BtlCamState *)out);
        return;
    }
    if (action->stepKind == 0xD) {
        btlAdvanceTargetCursorAnimation(action, &action->camera);
    }
}

extern void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

void btlUpdateActionPoseForLinkedTarget(BtlLinkedCommand *action) {
    BtlUnit *target;
    if (((BtlState *)btlGetRuntime())->unk220 & 1) {
        target = action->link->unit;
        if (target->status.flags & 0x400) {
            func_001ECBF8(action, &action->camera, target);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform(&action->frontCamera, &action->camera);
        btlApplyMarkedUnitActionCamera(action, &action->backCamera, action->link->unit, 0);
        action->motionParameter = 7.0f;
        action->flags = (action->flags | 0x1041) & 0xFFFBFFFF;
    } else {
        btlApplyMarkedUnitActionCamera(action, &action->camera, action->link->unit, 0);
    }
}

void func_001EBE28(void) {
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EBE30);

void func_001EC190(void) {
}

extern void btlFrameTargetExtremesCameraPose(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);

void func_001EC198(BtlLinkedCommand *action) {
    BtlUnit *target;
    s32 kind;
    f32 pos[4];
    if (btlGetIndexListCount(action->targetList) != 1) {
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (action->link->unit->status.flags & 0x200) {
        btlApplyMarkedUnitActionCamera(action, &action->camera, target, 0);
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
    btlFrameTargetExtremesCameraPose(action, &action->frontCamera, kind, 45.0f, 0.25f);
    btlFrameTargetExtremesCameraPose(action, &action->backCamera, kind, 1.0f, 0.5f);
    action->motionParameter = 30.0f;
    action->flags |= 0x41;
}

void func_001EC2A0(void) {
}

extern void btlInitSkillCommandCursor(BtlLinkedCommand *, BtlCamState *);

void btlStartLinkedActionPoseBlendIfEligible(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (action->link->unit->status.flags & 0x400) {
        if (work->cameraPoseBlendHook != 0) {
            s32 hasFlag200 = 0;
            s32 hasFlag400 = 0;
            u32 i;
            u32 count = btlGetIndexListCount(action->link->indexWork.indices);
            for (i = 0; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(action->link->indexWork.indices, i);
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
        btlInitSkillCommandCursor(action, &action->camera);
    }
}

void btlAdvanceUnblockedPlayerCursorAnimation(BtlLinkedCommand *unit) {
    if (!(unit->flags & 0x10000)) {
        btlAdvancePlayerCursorAnimation(unit, &unit->camera);
    }
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EC418);

void func_001EC5F0(BtlLinkedCommand *unit) {
    if (!(unit->flags & 0x10000)) {
        func_001F4F10(unit, &unit->camera);
    }
}

void func_001EC620(BtlLinkedCommand *action) {
    func_001F5018(action, &action->camera);
}

void btlAdvanceCommandCursorTask(BtlLinkedCommand *action) {
    btlAdvanceCommandCursor(action, &action->camera);
}

void func_001EC650(BtlLinkedCommand *action) {
    func_001F2758(action, &action->frontCamera, &action->backCamera);
}

void func_001EC670(void) {
    func_001F2AE8();
}

extern void func_001F2E30(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlAppendLinkedUnitToActorIndices(BtlLinkedCommand *action) {
    btlAppendIndexListEntry(action->targetList, action->link->unit);
    func_001F2E30(action, &action->frontCamera, &action->backCamera);
}

void func_001EC6C8(void) {
}

void btlUpdateLinkedActionEffectVectorByTarget(BtlLinkedCommand *action) {
    if ((action->link->unit->status.flags & 0x200) != 0) {
        btlBuildGroupFramingCameraPose(&action->camera, &action->camera);
        return;
    }
    if (action->actionKind != 0x10) {
        btlResetUnitEffectVector(action, &action->camera);
        return;
    }
}

void func_001EC728(void) {
}

void btlInitializeCursorForLinkedAction(s32 action) {
    if (((BtlLinkedCommand *)action)->link != 0) {
        btlInitLinkedUnitActionCursor(((BtlLinkedCommand *)action)->link);
        return;
    }
}

void func_001EC760(void) {
}

extern void func_001F35C8(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void func_001EC768(BtlLinkedCommand *action) {
    func_001F35C8(action, &action->frontCamera, &action->backCamera);
}

typedef struct {
    u8 unk00[0x674];
    s32 (*allowDefaultSound)(void *);
} SoundEventCallbacks;

extern void func_001F3888(void *, void *, void *);

void btlRunDefaultActionPoseUnlessHooked(void *actor) {
    SoundEventCallbacks *callbacks = (SoundEventCallbacks *)btlGetRuntime();
    if (callbacks->allowDefaultSound && callbacks->allowDefaultSound(actor)) {
        return;
    }
    func_001F3888(actor, &((BtlLinkedCommand *)actor)->frontCamera, &((BtlLinkedCommand *)actor)->backCamera);
}

s32 func_001EC7E8(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk650;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}

s32 func_001EC828(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk658;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EC868);

void func_001ECBF8(BtlLinkedCommand *command, BtlCamState *pose) {
    func_001EC868(command, pose, 27.5f);
}

void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out) {
    func_001EC868(command, pose, 20.0f);
    btlCopyMotionTransform(out, pose);
    if (pose->direction[0] > 0.0f) {
        func_00336538(-(20.0f * 0.017453293f));
    } else {
        func_00336538(20.0f * 0.017453293f);
    }
    VU0_STORE_VF(vf10, pose->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
}

extern f32 func_00353228(f32);

typedef struct BattlePairCameraPreset {
    f32 fromQuaternion[4];
    f32 toQuaternion[4];
    f32 fromDistanceScale;
    f32 toDistanceScale;
    f32 fromHeightScale;
    f32 motionParameter;
} BattlePairCameraPreset;

/* The two default rows form one copied camera-preset bank. */
typedef struct BattlePairCameraPresetSet {
    BattlePairCameraPreset poses[2];
} BattlePairCameraPresetSet;

extern const BattlePairCameraPresetSet D_004183D8;

void btlPrepareHeightScaledActionCameraPose(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quaternion[4];
    BattlePairCameraPreset poses[4] = {
        {{0.109f, -0.872f, -0.288f, 0.355f},
         {0.01f, -0.97f, 0.03f, -0.14f},
         2.2f, 1.5f, 0.8f, 25.0f},
        {{-0.08f, -0.91f, 0.16f, 0.34f},
         {-0.05f, -0.96f, -0.16f, -0.14f},
         2.2f, 1.5f, 0.7f, 25.0f},
        {{-0.09f, -0.88f, -0.15f, -0.43f},
         {-0.02f, -0.98f, 0.03f, 0.11f},
         2.2f, 1.5f, 0.8f, 25.0f},
        {{0.04f, -0.93f, 0.12f, -0.31f},
         {0.01f, -0.97f, -0.16f, 0.06f},
         2.2f, 1.5f, 0.7f, 25.0f},
    };
    BtlUnit *unit = action->link->unit;
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
        if (!btlIsCurrentValueBelowQuarterThreshold(unit) || !(unit->status.flags & 0x200)) {
            if (func_001E3230(unit, 1) == 0) {
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
        tangent = func_00353228(halfFov);
        distance = unit->unkC0 * unit->scale / tangent;
        from->distance = distance * poses[pose].fromDistanceScale;
        to->distance = distance * poses[pose].toDistanceScale;

        VU0_LOAD_VF(vf10, poses[pose].fromQuaternion);
        VU0_LOAD_VF(vf11, quaternion);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, from->direction);

        VU0_LOAD_VF(vf10, poses[pose].toQuaternion);
        VU0_LOAD_VF(vf11, quaternion);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);

        btlAdjustCameraDirectionForDefaultPlane(from);
        btlAdjustCameraDirectionForDefaultPlane(to);
        if (btlHasMarkedEntry10((s32)action)) {
            action->durationFrames = func_001E2E58(unit, unit->unkEC);
            action->flags |= 0x11;
        } else {
            action->flags |= 0x41;
            action->motionParameter = poses[pose].motionParameter;
        }
    }
}

void btlPrepareRandomizedActionCameraPose(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quat[4];
    /* Four camera presets: two quaternion rows at [0..3] and [4..7],
       target-distance multiplier at [8], camera parameter at [9], and
       two unused zero slots. Keep the rows contiguous for the VU loads. */
    f32 poses[4][12] = {
        {0.0f, -0.94f, 0.02f, 0.3f, 0.0f, -1.0f, 0.0f, 0.0f, 1.15f, 25.0f, 0.0f, 0.0f},
        {0.06f, -0.94f, -0.18f, 0.25f, 0.0f, -1.0f, 0.0f, 0.0f, 1.15f, 25.0f, 0.0f, 0.0f},
        {0.0f, -0.94f, 0.02f, -0.3f, 0.0f, -1.0f, 0.0f, 0.0f, 1.15f, 25.0f, 0.0f, 0.0f},
        {-0.06f, -0.94f, -0.18f, -0.25f, 0.0f, -1.0f, 0.0f, 0.0f, 1.15f, 25.0f, 0.0f, 0.0f},
    };
    BtlUnit *unit = action->link->unit;
    u32 flags = unit->status.flags;
    s32 pose;
    f32 fov;
    f32 half;
    f32 span;
    f32 dist;
    if (flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(flags & 0x600);
        btlCopyUnitRotationQuaternion(unit, quat);
        pose = effMiscRandMod(0, 4);
        fov = action->camera.fov;
        from->fov = fov;
        to->fov = fov;
        span = func_00208000(flags & 0x600, 0, 0) * 1.25f;
        VU0_STORE_VF(vf10, from->position);
        if (func_001E3230(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF(vf10, to->position);
        VU0_LOAD_VF(vf11, from->position);
        VU0_LERP_VF10(0.5f);
        VU0_STORE_VF(vf10, from->position);
        half = fov * 0.5f;
        dist = span / func_00353228(half);
        from->distance = dist;
        dist = unit->unkC0 * unit->scale / func_00353228(half);
        to->distance = dist * poses[pose][8];
        VU0_LOAD_VF(vf10, poses[pose]);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, from->direction);
        VU0_LOAD_VF(vf10, &poses[pose][4]);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);
        btlAdjustCameraDirectionForDefaultPlane(from);
        btlAdjustCameraDirectionForDefaultPlane(to);
        action->motionParameter = poses[pose][9];
        action->flags |= 0x41;
    }
}

void btlAimEffectPoseAtUnit(u8 *fx) {
    BtlUnit *unit = ((BtlLinkedCommand *)fx)->link->unit;
    if (unit->status.flags & 2) {
        if (btlSetActorEffectParameter(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, &((BtlLinkedCommand *)fx)->backCamera);
        btlAdjustCameraDirectionForDefaultPlane(&((BtlLinkedCommand *)fx)->backCamera);
    }
}

/* Mirrored camera aim presets: quaternion, distance and height scale. */
typedef struct BattleSideCameraPreset {
    f32 quat[4];
    f32 distanceScale;
    f32 heightScale;
    f32 unk18;
    f32 unk1C;
} BattleSideCameraPreset;

extern void func_001EEB78(BtlLinkedCommand *, BtlCamState *, u8);

extern s32 func_001E2E58(BtlUnit *, s32);

void func_001ED380(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from) {
    f32 muzzle[4];
    f32 quat[4];
    BattleSideCameraPreset poses[2] = {
        {{0x1.70a3d6p-3f, -0x1.999998p-2f, -0x1.70a3d6p-4f, 0.89f}, 3.0f, 0x1.999998p-1f, 30.0f, 0.0f},
        {{0x1.70a3d6p-3f, 0x1.999998p-2f, 0x1.70a3d6p-4f, 0.89f}, 3.0f, 0x1.999998p-1f, 30.0f, 0.0f},
    };
    BtlUnit *unit = action->link->unit;
    s32 pose;
    f32 distance;
    f32 fov;

    if (unit->status.flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
        if (btlHasSingleLinkedResource(action) == 0) {
            func_001EF030(action, from);
        } else {
            func_001EEB78(action, from, 1);
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
        if (func_001E3230(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF(vf10, to->position);
        to->position[1] *= poses[pose].heightScale;
        distance = unit->unkC0 * unit->scale / func_00353228(fov * 0.5f);
        to->distance = distance * poses[pose].distanceScale;
        VU0_LOAD_VF(vf10, poses[pose].quat);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);
        btlAdjustCameraDirectionForDefaultPlane(to);
        action->durationFrames = func_001E2E58(unit, unit->unkEC);
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

void func_001ED6C8(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from) {
    f32 targetPosition[4];
    f32 rotation[4];
    f32 bodyPosition[4];
    f32 center[4];
    BtlUnit *unit = action->link->unit;
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
            func_001EC868(action, from, 17.5f);
            btlCopyMotionTransform(to, from);
            btlInterpolateVectorStep(from->position);
            VU0_STORE_VF(vf10, targetPosition);
            if (func_001E3230(unit, 1) == 0) {
                btlUnitGetMuzzlePosVU(unit);
            }
            VU0_STORE_VF(vf10, to->position);
            VU0_LOAD_VF(vf11, targetPosition);
            VU0_SUB(vf10, vf10, vf11);
            VU0_SET_VF10_COMPONENT(x, 0.0f);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, to->direction);
            halfFov = to->fov * 0.5f;
            minimumDistance = unit->unkC0 * unit->scale / func_00353228(halfFov);
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
                func_00208000(targetFlags, 0, 0);
                VU0_STORE_VF_UNCLOBBERED(vf10, center);
                if (btlAimHorizontalDirectionVU(bodyPosition, center) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
                    btlSetUnitRotation(unit, (s128 *)rotation);
                }
            }
        } else {
            target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
            func_001EEB78(action, from, 2);
            halfFov = to->fov * 0.5f * 1.3333333f;
            btlCopyMotionTransform(to, from);
            minimumDistance = target->reach * target->scale / func_00353228(halfFov);
            to->distance *= 0.6f;
            if (to->distance < minimumDistance) {
                to->distance = minimumDistance;
            }
            from->distance += 75.0f;
        }
        btlAdjustCameraDirectionForDefaultPlane(to);
        btlAdjustCameraDirectionForDefaultPlane(from);
        action->motionParameter = 20.0f;
        action->flags |= 0x845;
        action->motionProgress = 0;
    }
}

void func_001ED9A0(BtlLinkedCommand *action, BtlCamState *pose) {
    BtlUnit *user;
    BtlUnit *target;
    s32 frames;
    s32 idle;
    s32 eligible;

    if (action->motionProgress == 0 && (s32)btlGetIndexListCount(action->targetList) < 2) {
        user = action->link->unit;
        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
        frames = func_001E2E58(user, user->unkEC);
        frames = (s32)((f32)frames / btlGetActorStateScale(action->link));
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

void btlSetupCameraPoseAimUnit(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quat[4];
    BtlUnit *unit = action->link->unit;
    f32 fov;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
    btlCopyUnitRotationQuaternion(unit, quat);
    fov = action->camera.fov;
    from->fov = fov;
    if (func_001E3230(unit, 1) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_STORE_VF(vf10, from->position);
    from->distance = unit->unkC0 * unit->scale / func_00353228(fov * 0.5f);
    VU0_LOAD_VF(vf10, quat);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9130);
    VU0_NEGATE_XYZ(vf10);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, from->direction);
    btlCopyMotionTransform(to, from);
    to->distance += 550.0f;
    action->flags = (action->flags & ~0x14) | 0x41;
    action->motionParameter = 25.0f;
    btlAdjustCameraDirectionForDefaultPlane(from);
    btlAdjustCameraDirectionForDefaultPlane(to);
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EDC38);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EDFB8);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EE458);

extern u32 effMiscRand(void *state);

void btlFrameTargetExtremesCameraPose(BtlLinkedCommand *command, BtlCamState *pose, s32 modeBits,
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
    for (unit = runtime->units; unit != NULL; unit = unit->nextActor) {
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
        func_001ECBF8(command, pose);
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
    projected = func_00208000(groups, NULL, NULL);
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
        projected *= func_00353228(radians);
        plane[2] += projected;
    } else {
        f32 radians = angle * 0.017453293f;
        plane[2] -= clearance;
        projected = pose->position[0];
        projected -= firstPosition[0];
        projected = ffabsf(projected);
        projected *= func_00353228(radians);
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
    distance += projected / func_00353228(fov);
    projected = btlProjectOnPlaneVU(plane, pose->position, lastPosition);
    VU0_STORE_VF(vf10, point);
    projected += clearance;
    {
        f32 secondDistance = projected / func_00353228(fov);
        VU0_LOAD_VF(vf10, point);
        VU0_LOAD_VF(vf11, pose->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        secondDistance -= length;
        pose->distance = distance < secondDistance ? secondDistance : distance;
    }
    btlAdjustCameraDirectionForDefaultPlane(pose);
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EEB78);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EF030);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EF668);

void btlActionAimUserAtTargets(BtlLinkedCommand *action, BtlCamState *pose, BtlCamState *out) {
    s128 vec[3];
    BtlUnit *unit = action->link->unit;
    u32 mask = 0;
    u32 i;
    u32 count;
    btlPrepareUnitPoseWithTiltRotation(action, pose, out);
    count = btlGetIndexListCount(action->targetList);
    for (i = 0; i < count; i++) {
        mask |= ((BtlUnit *)btlGetIndexListEntry(action->targetList, i))->status.flags & 0x600;
    }
    if (unit->status.flags & 0x80000) {
        func_00208000(mask, 0, 0);
        VU0_STORE_VF(vf10, &vec[0]);
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU((f32 *)&vec[1], (f32 *)&vec[0]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation(unit, &vec[2]);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001EFA30);

extern void func_001EFA30(BtlLinkedCommand *, BtlCamState *, s32, f32 *, f32, f32);

extern f32 D_00418240[4];

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

extern CameraFramePose D_00418250[4];

/* Frame the from and to poses around the action's targets from a random preset row
 * (rows 2-3 for animations with flag 0x200); targets in group 0x400 mirror the presets. */
void func_001EFEE8(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 direction[4];
    CameraFramePose poses[4];
    u16 animationFlags;
    f32 scale;
    f32 distance;
    s32 pose;
    u32 count;
    u32 i;
    u32 groups;

    memcpy(poses, D_00418250, sizeof(poses));
    if (action->actionCode - 1 < 0x21F) {
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
        VU0_LOAD_VF(vf11, D_00418240);
        effMiscQuatMultiplyVU();
    }
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9130);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, direction);
    distance = scale * poses[pose].fromDistance;
    func_001EFA30(action, from, groups, direction, distance, 0.0f);
    VU0_LOAD_VF(vf10, poses[pose].toQuat);
    if (groups & 0x400) {
        VU0_LOAD_VF(vf11, D_00418240);
        effMiscQuatMultiplyVU();
    }
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9130);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, direction);
    distance = scale * poses[pose].toDistance;
    if (animationFlags & 0x200) {
        func_001EFA30(action, to, groups, direction, distance, 0.0f);
    } else {
        func_001EFA30(action, to, groups, direction, distance, -0x1.333332p-3f);
    }
    action->flags |= 0x41;
    action->motionParameter = poses[pose].parameter;
}

void btlFlagUserAndTargetDefeat(BtlLinkedCommand *command, BtlLinkedCommand *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];

    user = command->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
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

extern f32 func_00353140(f32);

extern f32 func_00353040(f32);

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

    user = action->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    extent = user->reach * user->scale;
    span = func_001E2E58(user, user->unkEC);
    span /= btlGetActorStateScale(action->link);
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
    width = length * func_00353140(angle);
    length *= func_00353040(angle);
    length += (extent + width) / func_00353228(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (action->flags & 0x200) {
        angle = -angle;
    }
    func_00336538(angle);
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

    user = command->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    if (!(user->status.flags & target->status.flags & 0x600)) {
        btlFlagAllUnitsDefeatCandidate();
    } else {
        func_001ECBF8(command, &command->camera);
        return;
    }
    func_001EF668(command, command->frontCamera.position);
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
    user = action->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (target->status.flags & user->status.flags & 0x600) {
        return;
    }
    if (action->motionProgress != 0) {
        return;
    }
    frames = func_001E2E58(user, user->unkEC);
    frames = (s32)((f32)frames / btlGetActorStateScale(action->link));
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
        extent += length * func_00353140(angle);
        length *= func_00353040(angle);
        out->distance = length + extent / func_00353228(out->fov * 1.3333333f * 0.5f);
        if (!(action->flags & 0x200)) {
            angle = -0.34906585f;
        }
        func_00336538(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, out->direction);
        btlAdjustCameraDirectionForDefaultPlane(out);
    }
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F0968);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F0C80);

extern BtlUnit *btlFindFarthestUnit(u32, f32 *);

extern BtlUnit *btlFindNearestUnit(u32, BtlUnit *);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern f32 func_00353040(f32);

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
    f32 func_001F1120(f32 heightScale) {
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
        tanHalf = func_00353228(halfFov);
        distance = (distance + refReach / func_00353040(halfFov)) / tanHalf;
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
        func_00208000(0x200, &span[0], NULL);
        focus[1] = -span[0];
        fit = func_00208000(0x400, &span[1], &span[2]);
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
            nearFit = func_001F1120(0.0f);
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
            nearFit = fit * 1.2f / func_00353228(halfFov);
        }
        ref = btlFindFarthestUnit(0x200, focus);
        if (ref != NULL) {
            fits[0] = func_001F1120(1.0f);
        } else {
            fits[0] = 0.0f;
        }
        ref = btlFindNearestUnit(0x200, unit);
        if (ref != NULL) {
            fits[1] = func_001F1120(1.0f);
        } else {
            fits[1] = 0.0f;
        }
        VU0_LOAD_VF(vf10, eye);
        VU0_LOAD_VF(vf11, focus);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(offset);
        diameter = 2.0f * unitExtent;
        if (span[0] < diameter) {
            fits[2] = offset + diameter / func_00353228(halfFov);
        } else {
            fits[2] = offset + span[0] / func_00353228(halfFov);
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

extern void func_001F0C80(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

extern void btlBuildShoulderActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *action, BtlCamState *pose,
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
    for (unit = runtime->units; unit != NULL; unit = unit->nextActor) {
        s32 flags;

        if ((btlUnitStatusPair(unit) & 0x300) != 0x300) {
            flags = unit->status.flags;
        } else if (runtime->unk268 != 3) {
            flags = unit->status.flags;
        } else {
            if (unit->lookupId != 1) {
                func_001E3108(unit, vectors.position);
                vectors.position[2] = unit->position[2] - 45.0f;
                btlSetUnitPosition(unit, vectors.position);
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
        func_001F0C80(action, pose, target, mode);
    } else {
        btlBuildShoulderActionCamera(action, pose, target, mode);
    }

    func_00208000(0x400, NULL, NULL);
    VU0_STORE_VF_UNCLOBBERED(vf10, vectors.center);
    for (unit = runtime->units; unit != NULL; unit = unit->nextActor) {
        s32 flags = unit->status.flags;

        if ((flags & 0x200) != 0) {
            if ((flags & 0xE0) == 0) {
                if ((flags & 0x80000) != 0) {
                    btlUnitGetBodyPosVU(unit);
                    VU0_STORE_VF_UNCLOBBERED(vf10, vectors.firstActorPosition);
                    if (btlAimHorizontalDirectionVU(vectors.firstActorPosition, vectors.center) != 0) {
                        VU0_STORE_VF_UNCLOBBERED(vf10, vectors.rotation);
                        btlSetUnitRotation(unit, vectors.rotation);
                    }
                }
            }
        }
    }

    func_00208000(0x200, NULL, NULL);
    VU0_STORE_VF_UNCLOBBERED(vf10, vectors.center);
    for (unit = runtime->units; unit != NULL; unit = unit->nextActor) {
        s32 flags = unit->status.flags;

        if ((flags & 0x400) != 0) {
            if ((flags & 0x80000) != 0) {
                btlUnitGetBodyPosVU(unit);
                VU0_STORE_VF_UNCLOBBERED(vf10, vectors.secondActorPosition);
                btlAimHorizontalDirectionClampedVU(vectors.secondActorPosition, vectors.center,
                                                     0.34906585f);
                VU0_STORE_VF_UNCLOBBERED(vf10, vectors.rotation);
                btlSetUnitRotation(unit, vectors.rotation);
            }
        }
    }

    if (target != NULL && (target->status.flags & 2) != 0 &&
        (runtime->unk268 == 1 ||
         (runtime->unk268 == 3 && target->lookupId == 1))) {
        s32 field = mdlGetNodeMotionIndex(target->ext->owner, 0);

        if (field == 0xD || field == 0x12) {
            btlRefreshUnitEffectMotionAndEntry(target);
        }
    }

    pose->distance += action->cameraDistanceOffset;
    btlAdjustCameraDirectionForDefaultPlane(pose);
}

extern f32 func_00208750(BtlIndexList *, f32 *, f32 *);
extern f32 func_00353040(f32);
extern f32 func_00353228(f32);
extern void btlInterpolateVectorStep(f32 *);
extern s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *);
extern void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

/* Transfer the complete vector while keeping the SDK pointer addressing form. */
static inline void btlDefeatCameraStoreVector(f32 *dst) {
    __asm__ volatile (
        ".set noreorder\n\tsqc2 vf10, 0(%1)\n\t.set reorder"
        : "=m" (*(PcpVectorCopyF32 *)dst) : "r" (dst));
}

/* Derive a camera pose around the supplied actor set, honoring the runtime
 * override slot before building either the selected-target or group view. */
void func_001F1B00(BtlLinkedCommand *action, BtlCamState *pose,
                   BtlUnit *startActor, BtlIndexList *indices, s8 mode) {
    typedef s32 (*CameraArrangementCallback)(BtlLinkedCommand *, BtlCamState *, s32);
    struct {
        f32 center[4];
        f32 edge[4];
        f32 interpolated[4];
    } vectors __attribute__((aligned(16)));
    BtlState *runtime = (BtlState *)btlGetRuntime();
    CameraArrangementCallback callback = runtime->defeatCameraHook;
    u32 count;
    u32 i;
    u32 sideFlags;
    f32 height;
    f32 depth;
    f32 span;
    f32 length;
    f32 tangent;
    f32 fov;

    if (callback != NULL && callback(action, pose, 1) != 0) {
        return;
    }

    if (mode == 0) {
        btlApplyMarkedUnitActionCamera(action, pose, startActor, 1);
        pose->distance += action->cameraDistanceOffset;
        btlAdjustCameraDirectionForDefaultPlane(pose);
        i = 0;
        count = btlGetIndexListCount(indices);
        if ((u32)(s32)mode < count) {
            for (; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(indices, (s32)i);
                if ((entry->status.flags & 0x20) != 0) {
                    btlInterpolateVectorStep(pose->position);
                    btlDefeatCameraStoreVector(vectors.interpolated);
                    func_00208000(0x200, NULL, NULL);
                    btlDefeatCameraStoreVector(pose->position);
                    pose->position[1] = 0.0f;
                    pose->position[2] *= 0.5f;
                    __asm__ volatile (".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder"
                        : : "r" (pose->position), "m" (*(const PcpVectorCopyF32 *)(pose->position)));
                    __asm__ volatile (".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder"
                        : : "r" (vectors.interpolated), "m" (*(const PcpVectorCopyF32 *)(vectors.interpolated)));
                    VU0_SUB(vf10, vf10, vf11);
                    VU0_LENGTH_VF10(pose->distance);
                    VU0_NORMALIZE_VF10();
                    btlDefeatCameraStoreVector(pose->direction);
                    btlAdjustCameraDirectionForDefaultPlane(pose);
                    return;
                }
            }
        }
        return;
    }
    fov = action->camera.fov;
    pose->fov = fov;
    sideFlags = 0;
    i = 0;
    count = btlGetIndexListCount(indices);
    for (; i < count; i++) {
        BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(indices, (s32)i);
        sideFlags |= entry->status.flags & 0x600;
    }
    if (mode == 0) {
        span = func_00208000((s32)sideFlags, &height, &depth);
    } else {
        span = func_00208750(indices, &height, &depth);
    }
    btlDefeatCameraStoreVector(vectors.center);
    if (mode == 0 || runtime->unk268 == 3) {
        span += ffabsf(vectors.center[0]);
        vectors.center[0] = 0.0f;
        vectors.center[1] = -(height + depth) * 0.5f;
    } else {
        vectors.center[1] = -height * 0.75f;
        span *= 1.25f;
    }

    func_00208000(0x400, NULL, &depth);
    btlDefeatCameraStoreVector(vectors.edge);
    vectors.edge[0] = 0.0f;
    if (depth > 150.0f) {
        vectors.edge[1] = -depth * 0.45f;
    } else {
        vectors.edge[1] = -depth * 0.25f;
    }
    if (mode == 0) {
        if (action->camera.direction[0] > 0.0f) {
            func_00336538(27.5f * 0.017453293f);
        } else {
            func_00336538(-(27.5f * 0.017453293f));
        }
    }

    __asm__ volatile (".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder"
                        : : "r" (vectors.edge), "m" (*(const PcpVectorCopyF32 *)(vectors.edge)));
    __asm__ volatile (".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder"
                        : : "r" (vectors.center), "m" (*(const PcpVectorCopyF32 *)(vectors.center)));
    VU0_LERP_VF10(0.5f);
    btlDefeatCameraStoreVector(pose->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    VU0_NORMALIZE_VF10();
    if (mode == 0) {
        VU0_ROTATE_VEC(vf10, vf10);
    }
    btlDefeatCameraStoreVector(pose->direction);

    tangent = func_00353040(27.5f * 0.017453293f);
    length /= tangent;
    fov *= 1.3333333f;
    tangent = func_00353228(fov * 0.5f);
    length += span / tangent;
    pose->distance = length;
    length += action->cameraDistanceOffset;
    pose->distance = length;
    btlAdjustCameraDirectionForDefaultPlane(pose);
}

/* vu0 routine: measure camera clearance from the actor's adjusted muzzle position. */
void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out) {
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    f32 span;
    f32 distance;

    for (; actor != 0; actor = actor->nextActor) {
        s32 flags = actor->status.flags;
        if (!(flags & 1)) {
            continue;
        }
        if (flags & 0x200) {
            break;
        }
    }
    func_001ECBF8(command, out);
    btlCopyMotionTransform(pose, out);
    span = func_00208000(0x200, 0, 0) * 0.5f;
    pose->position[0] -= span;
    out->position[0] += span;
    out->distance *= 0.8f;
    btlUnitGetMuzzlePosVU(actor);
    VU0_SET_VF10_COMPONENT(y, -btlUnitGetTopY(actor));
    VU0_LOAD_VF(vf11, out);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    distance += (actor->unkC0 * actor->scale * 2.0f) /
                func_00353228(out->fov * 0.5f);
    if (out->distance < distance) {
        out->distance = distance;
    }
}

void func_001F20B0(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out) {
    btlPrepareUnitPoseWithTiltRotation(command, pose, out);
}

/* vu0 routine: frame the leftmost marked unit in two camera poses. */
void func_001F20C8(BtlCamState *source, BtlCamState *from, BtlCamState *to) {
    f32 point[4];
    f32 center[4];
    f32 height;
    f32 fov;
    f32 minX;
    f32 length;
    BtlState *work;
    BtlUnit *unit;
    BtlUnit *selected;
    s32 first;
    u32 flags;

    work = (BtlState *)btlGetRuntime();
    fov = source->fov;
    from->fov = fov;
    to->fov = fov;
    func_00208000(0x400, &height, 0);
    VU0_STORE_VF(vf10, center);
    center[1] = -height;
    first = 1;
    selected = NULL;
    minX = 0.0f;
    for (unit = work->units; unit != NULL; unit = unit->nextActor) {
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
    from->distance = length + selected->unkC0 * selected->scale * 3.5f /
                              func_00353228(fov * 0.5f);
    btlCopyMotionTransform(to, from);
    func_00336538(-(45.0f * 0.017453293f));
    VU0_LOAD_VF(vf10, to->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);
    to->distance = length + selected->unkC0 * selected->scale * 3.0f /
                            func_00353228(fov * 0.5f);
    btlAdjustCameraDirectionForDefaultPlane(from);
    btlAdjustCameraDirectionForDefaultPlane(to);
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F2308);

/* vu0 routine: frame the two unit groups using their bounding extents. */
void btlBuildGroupFramingCameraPose(BtlCamState *source, BtlCamState *out) {
    f32 target[4];
    f32 height;
    f32 fov;
    f32 span;

    btlFlagAllUnitsDefeatCandidate();
    fov = source->fov;
    out->fov = fov;
    span = func_00208000(0x200, &height, 0);
    VU0_STORE_VF(vf10, out->position);
    out->position[1] = -height * 1.15f;
    func_00208000(0x400, &height, 0);
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
    out->distance = span / func_00353228(fov * 0.5f);
    btlAdjustCameraDirectionForDefaultPlane(out);
}

void btlResetUnitEffectVector(BtlLinkedCommand *command, BtlCamState *pose) {
    func_001EC868(command, pose, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F2758);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F2AE8);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F2E30);

void func_001F3228(BtlLinkedCommand *action, BtlCamState *camera) {
    btlFlagUserAndTargetDefeat(action, action);
}

/* Interpolate pull-back and pitch while framing an actor approaching its target. */
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

    user = action->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    extent = user->reach * user->scale;
    span = func_001E2E58(user, user->unkEC);
    span /= btlGetActorStateScale(action->link);
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
    width = length * func_00353140(angle);
    length *= func_00353040(angle);
    length += (extent + width) / func_00353228(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (action->flags & 0x200) {
        angle = -angle;
    }
    func_00336538(angle);
    VU0_LOAD_VF(vf10, dir);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
    btlAdjustCameraDirectionForDefaultPlane(out);
}

void func_001F34C8(BtlLinkedCommand *action, BtlCamState *camera) {
    func_001F3228(action, camera);
}

void func_001F34E0(BtlLinkedCommand *action, BtlCamState *out) {
    btlBuildHeightClampedApproachCamera(action, out);
}

void btlChooseActionPoseBlendFromActorCount(BtlLinkedCommand *action,
                                          BtlCamState *unusedFront, BtlCamState *unusedBack) {
    BtlState *work = (BtlState *)btlGetRuntime();
    u32 count;
    u32 mask;
    BtlUnit *unit;
    mask = ((BtlUnit *)btlGetIndexListEntry(action->targetList, 0))->status.flags & 0x600;
    count = 0;
    for (unit = work->units; unit != 0; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & mask) {
                count++;
            }
        }
    }
    if (count >= 2) {
        func_001EDFB8(action, &action->frontCamera, &action->backCamera);
        return;
    }
    btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
    action->motionParameter = 200.0f;
    action->flags |= 0x41;
}

void func_001F35C0(void) {
}

/* vu0 routine: camera preset quaternions are composed with the unit rotation. */
void func_001F35C8(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quat[4];
    BattlePairCameraPresetSet presets = D_004183D8;
    BtlUnit *unit = action->link->unit;
    s32 pose;
    f32 fov;
    f32 dist;
    f32 motionParameter;

    if (unit->status.flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
        btlCopyUnitRotationQuaternion(unit, quat);
        pose = effMiscRandMod(0, 2);
        fov = action->camera.fov;
        from->fov = fov;
        to->fov = fov;
        if (func_001E3230(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF(vf10, from->position);
        VU0_STORE_VF(vf10, to->position);
        from->position[1] *= presets.poses[pose].fromHeightScale;
        dist = unit->unkC0 * unit->scale / func_00353228(fov * 0.5f);
        from->distance = dist * presets.poses[pose].fromDistanceScale;
        to->distance = dist * presets.poses[pose].toDistanceScale;
        VU0_LOAD_VF(vf10, presets.poses[pose].fromQuaternion);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, from->direction);
        VU0_LOAD_VF(vf10, presets.poses[pose].toQuaternion);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);
        btlAdjustCameraDirectionForDefaultPlane(from);
        btlAdjustCameraDirectionForDefaultPlane(to);
        motionParameter = presets.poses[pose].motionParameter;
        action->motionProgress = 0;
        action->flags |= 0x41;
        action->motionParameter = motionParameter;
    }
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F3888);

/* Nine actor camera rows, each containing two 0x74-byte mode records. */
typedef struct BtlActionCameraPose {
    f32 position[4];
    f32 direction[4];
} BtlActionCameraPose;

typedef struct BtlActionCameraMode {
    BtlActionCameraPose front; /* 0x00 */
    BtlActionCameraPose back; /* 0x20 */
    BtlActionCameraPose transition; /* 0x40 */
    s32 triggerFrame; /* 0x60 */
    f32 motionParameter; /* 0x64 */
    f32 blendDuration; /* 0x68 */
    f32 distanceRatio; /* 0x6C */
    f32 distanceOffset; /* 0x70 */
} BtlActionCameraMode;

typedef struct BtlActionCameraSettings {
    BtlActionCameraMode mode[2];
} BtlActionCameraSettings;

typedef char BtlActionCameraMode_size_check[
    (sizeof(BtlActionCameraMode) == 0x74) ? 1 : -1];

typedef char BtlActionCameraSettings_size_check[
    (sizeof(BtlActionCameraSettings) == 0xE8) ? 1 : -1];

extern BtlActionCameraSettings D_003B6E50[9];

/* Mark defeat candidates and initialize the selected actor camera mode. */
void func_001F3C30(BtlLinkedCommand *action) {
    BtlUnit *user = action->link->unit;
    u16 unitId = user->partyRecord.unitId;
    u32 count;
    u32 i;
    u32 targetFlags;
    u32 mode;

    btlFlagUserAndTargetDefeat(action, action);
    if (unitId >= 9)
        return;
    if (D_003B6E50[unitId].mode[0].motionParameter == 0.0f)
        return;

    mode = ((s32)action->flags >> 9) & 1;
    targetFlags = 0;
    count = btlGetIndexListCount(action->targetList);
    for (i = 0; i < count; i++) {
        BtlUnit *target = btlGetIndexListEntry(action->targetList, i);
        targetFlags |= target->status.flags & 0x600;
    }

    btlClearAllUnitDefeatCandidates();
    if (targetFlags == 0x200) {
        btlFlagUnitDefeatCandidate(user);
    } else {
        btlFlagMatchingUnitsDefeatCandidate(0x200);
    }

    btlInitMotionTransformFromComponents(&action->frontCamera,
                                         D_003B6E50[unitId].mode[mode].front.position[0],
                                         D_003B6E50[unitId].mode[mode].front.position[1],
                                         D_003B6E50[unitId].mode[mode].front.position[2],
                                         D_003B6E50[unitId].mode[mode].front.direction[0],
                                         D_003B6E50[unitId].mode[mode].front.direction[1],
                                         D_003B6E50[unitId].mode[mode].front.direction[2],
                                         D_003B6E50[unitId].mode[mode].front.direction[3], 40.0f);
    btlInitMotionTransformFromComponents(&action->backCamera,
                                         D_003B6E50[unitId].mode[mode].back.position[0],
                                         D_003B6E50[unitId].mode[mode].back.position[1],
                                         D_003B6E50[unitId].mode[mode].back.position[2],
                                         D_003B6E50[unitId].mode[mode].back.direction[0],
                                         D_003B6E50[unitId].mode[mode].back.direction[1],
                                         D_003B6E50[unitId].mode[mode].back.direction[2],
                                         D_003B6E50[unitId].mode[mode].back.direction[3], 40.0f);
    action->motionParameter = D_003B6E50[unitId].mode[mode].motionParameter;
    action->flags |= 0x80041;
    action->motionProgress = 1;
    action->cameraFrame = 0;
}

/* Advance the configured actor camera transition and blend its distance. */
void func_001F3E48(BtlLinkedCommand *action) {
    BtlUnit *user = action->link->unit;
    u16 unitId = user->partyRecord.unitId;
    u32 selectedMode;
    s32 motionFrame;
    s32 triggerFrame;
    u32 aggregateFlags;
    u32 count;
    u32 i;
    f32 savedDistance;
    f32 progress;
    f32 ratio;
    f32 base;

    if (D_003B6E50[unitId].mode[0].motionParameter == 0.0f) {
        btlBuildApproachCamera(action, &action->camera);
        return;
    }

    selectedMode = ((s32)action->flags >> 9) & 1;
    motionFrame = func_001E2E58(user, user->unkEC);
    triggerFrame = D_003B6E50[unitId].mode[selectedMode].triggerFrame;
    if (action->state == triggerFrame && action->motionProgress != 0) {
        btlCopyMotionTransform(&action->frontCamera, &action->backCamera);
        btlInitMotionTransformFromComponents(
            &action->backCamera,
            D_003B6E50[unitId].mode[selectedMode].transition.position[0],
            D_003B6E50[unitId].mode[selectedMode].transition.position[1],
            D_003B6E50[unitId].mode[selectedMode].transition.position[2],
            D_003B6E50[unitId].mode[selectedMode].transition.direction[0],
            D_003B6E50[unitId].mode[selectedMode].transition.direction[1],
            D_003B6E50[unitId].mode[selectedMode].transition.direction[2],
            D_003B6E50[unitId].mode[selectedMode].transition.direction[3], 40.0f);
        {
            f32 blendDuration = D_003B6E50[unitId].mode[selectedMode].blendDuration;
            action->state = 0;
            action->motionParameter = blendDuration;
        }
        action->motionProgress = 0;
    }

    if (action->cameraFrame >= motionFrame) {
        if (action->cameraFrame == motionFrame) {
            btlClearAllUnitDefeatCandidates();
            btlFlagUnitDefeatCandidate(user);
            count = btlGetIndexListCount(action->targetList);
            aggregateFlags = 0;
            for (i = 0; i < count; i++) {
                BtlUnit *target = (BtlUnit *)btlGetIndexListEntry(action->targetList, i);
                aggregateFlags |= target->status.flags & 0x600;
            }
            if (aggregateFlags == 0x200) {
                for (i = 0; i < count; i++) {
                    btlFlagUnitDefeatCandidate(
                        (BtlUnit *)btlGetIndexListEntry(action->targetList, i));
                }
            } else {
                btlFlagMatchingUnitsDefeatCandidate(aggregateFlags);
            }
            action->state = 0;
            action->motionParameter = 15.0f;
        }

        if (btlStepPoseBlend((u8 *)action) != 0) {
            action->progress = 1.0f;
        }
        btlBuildApproachCamera(action, &action->camera);

        if (selectedMode != 0) {
            func_00336538((1.0f - action->progress) * 0.27925268f);
        } else {
            func_00336538((1.0f - action->progress) * -0.27925268f);
        }
        VU0_LOAD_VF(vf10, action->camera.direction);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, action->camera.direction);

        savedDistance = action->camera.distance;
        progress = action->progress;
        ratio = D_003B6E50[unitId].mode[selectedMode].distanceRatio;
        action->camera.distance = savedDistance * ratio;
        base = D_003B6E50[unitId].mode[selectedMode].distanceOffset + savedDistance * (1.0f - ratio);
        action->camera.distance += base * progress;
        btlAdjustCameraDirectionForDefaultPlane(&action->camera);
        action->flags &= ~1;
        action->flags &= ~0x80000;
    } else {
        func_001E3108(user, action->translation);
        btlCopyUnitRotationQuaternion(user, action->rotation);
    }

    action->cameraFrame++;
}

/* The descriptor pointer is stored in retail non-small .data. */
extern const BtlCameraTimedInstruction *D_003BBFC4 __attribute__((section(".data")));

extern s16 D_00436AB0[];

extern s16 D_00436AB8[];

extern s16 D_00436AC0[];

extern u32 btlNextScaledRandom(u32);

void btlInitSkillCommandCursor(BtlLinkedCommand *command, BtlCamState *pose) {
    s16 groupChoices[2];
    s16 pathChoices[4];
    s16 sideChoices[2];
    s16 choice;
    s32 orientation;
    s32 initialFlags;
    s32 skillId;
    BtlUnit *entryUnit = command->link->unit;
    BtlUnit *unit;

    memcpy(groupChoices, D_00436AB0, sizeof(groupChoices));
    initialFlags = entryUnit->status.flags;
    memcpy(pathChoices, D_00436AB8, sizeof(pathChoices));
    memcpy(sideChoices, D_00436AC0, sizeof(sideChoices));
    if (initialFlags & 0x400) {
        btlActionAimUserAtTargets(command, &command->frontCamera, &command->backCamera);
        command->motionParameter = 20.0f;
        command->flags |= 0x41;
        return;
    }
    memset(CURSOR, 0, sizeof(*CURSOR));
    skillId = (u16)command->link->indexWork.skillId;
    switch (skillId) {
    case 0x1D4:
        CURSOR->unk_0A = 1;
        CURSOR->unk_0E = 0;
        if (btlCanUseLinkedActor((s32)command) == 0) {
            CURSOR->unk_0C = 0x1E;
        } else {
            CURSOR->unk_0C = 0x21;
        }
        func_001F5868(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x1D3:
        CURSOR->unk_0A = 1;
        CURSOR->unk_0C = 0x1A;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x1DC:
        CURSOR->unk_0A = 2;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        CURSOR->unk_0C = 0x17;
        func_001F5320(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x1BB:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0C = 0x1B;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1BF:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        CURSOR->unk_0C = 0x16;
        func_001F5320(command, pose, 0, 5);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x1D2:
        CURSOR->unk_0A = 2;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        CURSOR->unk_0C = pathChoices[(s16)btlNextScaledRandom(4)];
        func_001F5320(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x1D7:
        CURSOR->unk_0A = 2;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        if (btlCanUseLinkedActor((s32)command) == 0) {
            CURSOR->unk_0C = 0x1E;
        } else {
            CURSOR->unk_0C = 0xB;
        }
        func_001F5320(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x1CF:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0C = 0x1D;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1D1:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0C = 0x1C;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1DD:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0C = 0x15;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1DE:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0C = 8;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1E0:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0C = 2;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x20D:
    case 0x20E:
    case 0x20F:
    case 0x210:
    case 0x1E1:
    case 0x1E2:
    case 0x1E3:
    case 0x1E7:
    case 0x1E8:
    case 0x1E9:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        unit = command->link->unit;
        orientation = unit->lookupId > command->linkedA->lookupId ? !btlHasFlag(unit->status.flags, 0x1000) : btlHasFlag(unit->status.flags, 0x1000);
        if (orientation == 0) {
            CURSOR->unk_0C = 0x1F;
            choice = 0;
        } else {
            CURSOR->unk_0C = 0x20;
            choice = 1;
        }
        func_001F5868(command, pose, 1, sideChoices[choice]);
        func_001F5320(command, pose, 0, 5);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x1E4:
    case 0x1E5:
    case 0x1E6:
    case 0x1EA:
    case 0x1EB:
    case 0x1EC:
    case 0x1ED:
    case 0x1EE:
    case 0x1EF:
    case 0x1F0:
    case 0x1F1:
    case 0x1F2:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        unit = command->link->unit;
        orientation = unit->lookupId > command->linkedA->lookupId ? !btlHasFlag(unit->status.flags, 0x1000) : btlHasFlag(unit->status.flags, 0x1000);
        if (orientation == 0) {
            CURSOR->unk_0C = 0x13;
            choice = 0;
        } else {
            CURSOR->unk_0C = 0x14;
            choice = 1;
        }
        func_001F5868(command, pose, 1, sideChoices[choice]);
        func_001F5320(command, pose, 0, 5);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask((s32)command);
        break;
    case 0x5B:
    case 0x5C:
    case 0x5D:
    case 0x1B3:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[1]);
        if (skillId == 0x5C) {
            CURSOR->unk_0C = 0x2E;
        } else if (skillId == 0x5B) {
            CURSOR->unk_0C = 0x30;
        } else if (skillId == 0x5D) {
            CURSOR->unk_0C = 0x2F;
        } else {
            CURSOR->unk_0C = 0x11;
        }
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1B7:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[1]);
        CURSOR->unk_0C = 0x12;
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1C3:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[1]);
        CURSOR->unk_0C = 0xE;
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1D5:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        switch (command->link->unit->lookupId) {
        case 0: choice = 0; break;
        case 2: choice = 1; break;
        default: choice = (s16)btlNextScaledRandom(2); break;
        }
        CURSOR->unk_0C = choice + 0xC;
        func_001F5868(command, pose, 1, sideChoices[choice]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1D6:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        switch (command->link->unit->lookupId) {
        case 0: CURSOR->unk_0C = 9; break;
        case 2: CURSOR->unk_0C = 10; break;
        default: CURSOR->unk_0C = choice + 9; break;
        }
        func_001F5868(command, pose, 1, sideChoices[choice]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x1DA:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        switch (command->link->unit->lookupId) {
        case 0: CURSOR->unk_0C = 0xF; break;
        case 2: CURSOR->unk_0C = 0x10; break;
        default: CURSOR->unk_0C = choice + 0xF; break;
        }
        func_001F5868(command, pose, 1, sideChoices[choice]);
        func_001F5320(command, pose, 0, 5);
        break;
    case 0x200:
    case 0x204:
        CURSOR->unk_0A = 6;
        if (skillId == 0x200) {
            CURSOR->unk_0C = 0x24;
        } else {
            CURSOR->unk_0C = 0x27;
        }
        CURSOR->unk_0E = 0;
        func_001FA480(command, pose, D_003BBFC4);
        choice = (s16)btlNextScaledRandom(2);
        choice += 0x1F;
        func_001F5868(command, pose, 1, choice);
        func_001F5320(command, pose, 0, 0x12);
        break;
    case 0x202:
    case 0x206:
    case 0x208:
        CURSOR->unk_0A = 6;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        if (!btlCanUseLinkedActor((s32)command)) choice = 1;
        if (skillId == 0x202) {
            CURSOR->unk_0C = choice + 0x28;
        } else if (skillId == 0x208) {
            CURSOR->unk_0C = choice + 0x2A;
        } else {
            CURSOR->unk_0C = choice + 0x2C;
        }
        func_001FA480(command, pose, D_003BBFC4);
        choice += 0x1F;
        func_001F5868(command, pose, 1, choice);
        func_001F5320(command, pose, 0, 0x12);
        break;
    case 0x20C:
        CURSOR->unk_0A = 6;
        CURSOR->unk_0C = 0x25;
        CURSOR->unk_0E = 0;
        func_001FA480(command, pose, D_003BBFC4);
        choice = (s16)btlNextScaledRandom(2);
        choice += 0x1F;
        func_001F5868(command, pose, 1, choice);
        func_001F5320(command, pose, 0, 0x12);
        break;
    case 0x1D8:
    case 0x1DF:
    case 0x20A:
    case 0x20B:
    mode4_setup:
        CURSOR->unk_0A = 4;
        CURSOR->unk_0C = 5;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        choice += 8;
        func_001F5868(command, pose, 1, choice);
        func_001F5320(command, pose, 0, 0xC);
        break;
    case 0x1D9:
        goto mode4_setup;
    case 0x1D0:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0C = 0x26;
        CURSOR->unk_0E = 0;
        func_001F5868(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001F5320(command, pose, 0, 5);
        break;
    default:
        CURSOR->unk_0A = 3;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        CURSOR->unk_0C = 0x19 - choice;
        func_001F5868(command, pose, 1, sideChoices[choice]);
        func_001F5320(command, pose, 0, 5);
        break;
    }
}

void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->link->unit->status.flags & 0x400)) {
        func_001FA480(action, state, D_003BBFA8[CURSOR->unk_0A]);
        func_001FBAC0(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00417F30);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004180B0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004180C0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418240);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418250);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418310);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418320);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418330);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418338);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418398);

const BattlePairCameraPresetSet D_004183D8 = {
    {
        {{0x1.a9fbe6p-6f, -0x1.916872p-1f, -0x1.c28f5cp-5f, 0x1.34bc6ap-1f},
         {-0x1.47ae14p-7f, -0x1.f9db22p-1f, -0x1.4fdf3ap-5f, 0x1.89374ap-5f},
         0x1.4p+0f, 0x1.8p+1f, 0x1p+0f, 0x1.4p+4f},
        {{-0x1.2b020cp-4f, -0x1.9eb85p-1f, -0x1.4bc6a6p-4f, -0x1.1e353ep-1f},
         {-0x1.47ae14p-7f, -0x1.f9db22p-1f, -0x1.4fdf3ap-5f, 0x1.89374ap-5f},
         0x1.4p+0f, 0x1.8p+1f, 0x1p+0f, 0x1.4p+4f}
    }
};

void func_001F4E30(BtlLinkedCommand *action) {
    CURSOR->frame = 0;
    if (!(action->link->unit->status.flags & 0x400)) {
        return;
    }

    memset(CURSOR, 0, 0x130);
    switch ((u16)action->link->indexWork.skillId) {
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

extern const BtlCameraTimedInstruction *D_003BBFC8[];

void func_001F4F10(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->link->unit->status.flags & 0x400)) {
        func_001FA480(action, state, D_003BBFC8[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 :
            CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        switch ((u32)action->link->indexWork.skillId) {
        case 0x1B3:
        case 0x1B7:
        case 0x1BB:
        case 0x1BF:
        case 0x1C3:
            func_001FA480(action, state, D_003BBFC8[CURSOR->unk_0C]);
            func_001FBAC0(action, state);
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

extern s16 D_003BBD90[];

extern s16 D_003BBD70[];

typedef struct BtlCursorChoices {
    u16 values[3][3][8];
} BtlCursorChoices;

extern const BtlCursorChoices D_004184D8;

void func_001F5018(BtlLinkedCommand *action, BtlCamState *state) {
    BtlCursorChoices choices = D_004184D8;
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

    CURSOR->category = action->link->unit->lookupId;
    random = btlNextScaledRandom(8);
    CURSOR->index = choices.values[markedCount][CURSOR->category - 3][random];
    func_001F5868(action, state, 0, D_003BBD90[CURSOR->index]);
    func_001F5320(action, state, 0, D_003BBD70[CURSOR->index]);
}

void btlAdvanceCommandCursor(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->mode == 0) {
        func_001FA480(action, state, D_003BBF70[CURSOR->index]);
    } else {
        func_001FA480(action, state, D_003BBF88[CURSOR->index]);
    }
    func_001FBAC0(action, state);
    CURSOR->frame++;
}

BtlUnit *btlFindActorLinkByCategory(BtlLinkedCommand *command, s32 category) {
    BtlUnit *candidate = command->link->unit;

    if (candidate->lookupId == category) {
        return candidate;
    }
    candidate = command->linkedA;
    if (candidate != NULL && candidate->lookupId == category) {
        return candidate;
    }
    candidate = command->linkedB;
    if (candidate != NULL && candidate->lookupId == category) {
        return candidate;
    }
    return command->link->unit;
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F5320);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F5780);

void btlUnitGetPosVU(BtlUnit *unit, u8 mode) {
    s128 pos;
    switch (mode) {
    case 1:
        btlSetActorEffectParameterOrMuzzlePosition(unit, 1);
        VU0_STORE_VF(vf10, &pos);
        break;
    case 0:
    default:
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF(vf10, &pos);
        break;
    }
    VU0_LOAD_VF_MEMORY(vf10, &pos);
}

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004184D8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418568);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001F5868);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001FA480);

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

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001FBAC0);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001FC5E0);

/* Camera parameter banks have a native 0x80-byte stride. */
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

extern void func_001FC5E0(BtlLinkedCommand *, BtlCamState *,
                         const BtlCameraTimedInstruction *, const s32 *,
                         const BtlCameraParameterRecord *, f32 *);

extern void sdfConvertEulerAnglesToQuaternionVU(f32, f32, f32);

extern f32 D_003BDC60[4];

extern f32 D_003BDC70[4];

extern f32 D_003BDC80[4];

extern f32 D_00436AD0;

/* vu0 routine: retail camera vector operations use the SDK macro interface. */
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
                func_001FC5E0(command, pose, instruction, currentIndex, records, value);
                switch (instruction->kind) {
                case 1:
                    VU0_LOAD_VF(vf10, pose->direction);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, pose->position);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedPosition);
                    func_003364B8(value[0]);
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
                    func_00336538(value[1]);
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
                    func_00336538(value[1]);
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
                    VU0_LOAD_VF(vf10, D_003BDC70);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(D_00436AD0, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, focus);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_003BDC60);
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
                    VU0_LOAD_VF(vf10, D_003BDC70);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(D_00436AD0, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, focus);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_003BDC60);
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
                    VU0_LOAD_VF(vf10, D_003BDC70);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_SCALAR_OP(D_00436AD0, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, focus);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_003BDC60);
                    break;
                case 5:
                    if (command->flags & 0x200) value[0] = -value[0];
                    if (value[0] != 0.0f && 0.0f <= D_003BDC70[2]) value[1] = -value[1];
                    sdfConvertEulerAnglesToQuaternionVU(value[1], value[0], value[2]);
                    effMiscQuaternionToMatrixVU();
                    VU0_LOAD_VF(vf10, D_003BDC70);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_003BDC70);
                    VU0_LOAD_VF(vf10, D_003BDC70);
                    VU0_SCALAR_OP(D_00436AD0, "vmulx.xyzw vf10, vf10, vf2x");
                    VU0_LOAD_VF(vf11, D_003BDC60);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, D_003BDC80);
                    flags = records[*currentIndex].flags00;
                    if (flags & 4) {
                        VU0_LOAD_VF(vf10, pose->direction);
                        VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
                        VU0_LOAD_VF(vf11, D_003BDC80);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_STORE_VF_UNCLOBBERED(vf10, pose->position);
                    } else if (flags & 1) {
                        VU0_LOAD_VF(vf10, pose->position);
                        VU0_LOAD_VF(vf11, D_003BDC80);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_LENGTH_VF10(pose->distance);
                        VU0_NORMALIZE_VF10();
                        VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
                    }
                    break;
                case 8:
                    btlSetActorEffectParameterOrMuzzlePosition(command->link->unit, 1);
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
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (!(unit->status.flags & 0xC0)) {
                if (unit->status.flags & 0x200) {
                    if (unit->lookupId == id) {
                        return unit;
                    }
                }
            }
        }
    }
    return 0;
}

f32 func_001FDD20(f32 *first, f32 radius, f32 secondRadius, f32 *second) {
    f32 center[4];
    f32 larger[4];
    f32 smaller[4];
    f32 direction[4];
    f32 largerRadius;
    f32 smallerRadius;
    f32 distance;
    f32 result;

    if (first == NULL || second == NULL || radius == 0.0f || secondRadius == 0.0f) {
        return 0.0f;
    }
    if (secondRadius < radius) {
        VU0_LOAD_VF(vf10, first);
        VU0_STORE_VF_UNCLOBBERED(vf10, larger);
        VU0_LOAD_VF(vf10, second);
        VU0_STORE_VF_UNCLOBBERED(vf10, smaller);
        largerRadius = radius;
        smallerRadius = secondRadius;
    } else {
        VU0_LOAD_VF(vf10, first);
        VU0_STORE_VF_UNCLOBBERED(vf10, smaller);
        VU0_LOAD_VF(vf10, second);
        VU0_STORE_VF_UNCLOBBERED(vf10, larger);
        largerRadius = secondRadius;
        smallerRadius = radius;
    }
    VU0_LOAD_VF(vf10, smaller);
    VU0_LOAD_VF(vf11, larger);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    result = distance;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, direction);
    radius = largerRadius - smallerRadius;
    if (result < radius) {
        radius = radius * 0.5f + largerRadius;
    } else {
        radius = (distance + largerRadius + smallerRadius) * 0.5f;
    }
    VU0_LOAD_VF(vf10, direction);
    VU0_SCALAR_OP(radius - largerRadius, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_LOAD_VF(vf11, larger);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, center);
    VU0_LOAD_VF(vf10, center);
    result = radius;
    return result;
}

f32 btlGetUnitTargetDistance(BtlUnit *unit, u8 mode, f32 *target, f32 radius) {
    f32 saved[4];
    f32 pos[4];
    f32 result;
    if (unit == 0 || target == 0) {
        return 0.0f;
    }
    result = func_001F5780(unit, mode, 1.0f, 1.0f);
    btlUnitGetPosVU(unit, mode);
    VU0_STORE_VF_UNCLOBBERED(vf10, pos);
    result = func_001FDD20(pos, result, radius, target);
    VU0_STORE_VF(vf10, saved);
    VU0_LOAD_VF(vf10, saved);
    return result;
}

f32 func_001FDF18(BtlUnit *first, BtlUnit *second, u8 firstMode, u8 secondMode, f32 *target, f32 radius) {
    f32 pos[4];
    f32 saved[4];
    f32 result;

    if (first != 0) {
        if (second != 0) {
            result = func_001F5780(second, secondMode, 1.0f, 1.0f);
            btlUnitGetPosVU(second, secondMode);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
            result = btlGetUnitTargetDistance(first, firstMode, pos, result);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        } else {
            result = func_001F5780(first, firstMode, 1.0f, 1.0f);
            btlUnitGetPosVU(first, firstMode);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        }
    } else {
        result = func_001F5780(second, secondMode, 1.0f, 1.0f);
        btlUnitGetPosVU(second, secondMode);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
    }
    if (target != 0) {
        result = func_001FDD20(pos, result, radius, target);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
    }
    VU0_LOAD_VF(vf10, pos);
    VU0_STORE_VF_UNCLOBBERED(vf10, saved);
    VU0_LOAD_VF(vf10, saved);
    return result;
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001FE068);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001FE5C0);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001FEC00);

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_001FF0F8);

s32 btlCountUnitsByFlags(u32 mask) {
    BtlUnit *unit;
    s32 count = 0;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if ((unit->status.flags & 1) && (unit->status.flags & mask)) {
            if (!(unit->status.flags & 0x20)) {
                count++;
            }
        }
    }
    return count;
}

func_001FF5D8(BtlLinkedCommand *action, BtlCamState *state) {
    memset(D_003BD7D0, 0, sizeof(SoundCursor));
    switch (action->link->unit->partyRecord.unitId) {
    case 1:
        func_001FA480(action, state, D_003BC0A0[4]);
        func_001F5868(action, state, 2, 4);
        CURSOR->unk_0C = 2;
        func_001F5320(action, state, 2, 1);
        CURSOR->unk_00 = 1;
        break;
    case 4:
        func_001FA480(action, state, D_003BC0A0[4]);
        func_001F5868(action, state, 2, 0);
        CURSOR->unk_0C = 0;
        func_001F5320(action, state, 2, 1);
        CURSOR->unk_00 = 1;
        break;
    case 3:
    case 5:
    case 6:
        func_001FA480(action, state, D_003BC0A0[3]);
        func_001F5868(action, state, 2, 2);
        CURSOR->unk_0C = 1;
        func_001F5320(action, state, 2, 1);
        CURSOR->unk_00 = 1;
        break;
    case 7:
        func_001FA480(action, state, D_003BC0A0[3]);
        func_001F5868(action, state, 2, 7);
        CURSOR->unk_0C = 5;
        func_001F5320(action, state, 2, 2);
        CURSOR->unk_00 = 1;
        break;
    case 2:
        func_001FA480(action, state, D_003BC0A0[4]);
        func_001F5868(action, state, 2, 8);
        CURSOR->unk_0C = 2;
        func_001F5320(action, state, 2, 2);
        CURSOR->unk_00 = 1;
        break;
    case 8:
        func_001FA480(action, state, D_003BC0A0[4]);
        func_001F5868(action, state, 2, 9);
        CURSOR->unk_0C = 6;
        func_001F5320(action, state, 2, 2);
        CURSOR->unk_00 = 1;
        break;
    default:
        if (btlHasSingleLinkedResource(action)) {
            func_001EEB78(action, &action->camera, 1);
        }
        break;
    }
}

void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->unk_00 == 1) {
        if (!(action->link->unit->status.flags & 0x400)) {
            func_001FA480(action, state, D_003BC0A0[CURSOR->unk_0C]);
            func_001FBAC0(action, state);
            btlConstrainCameraEndpointHeight(action, state, 0, 1);
            CURSOR->frame++;
            CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
        }
    }
}

void btlClearCommandCursorAndRunAction(BtlLinkedCommand *action) {
    memset(D_003BD7D0, 0, 0x130);
    btlFlagUserAndTargetDefeat(action, action);
}

void btlAdvanceCommandCursorOrAction(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->unk_00 == 1) {
        if (action->link->unit->status.flags & 0x400) {
            return;
        }
        func_001FA480(action, state, D_003BC0C0[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera(action, &action->camera);
    }
}

void btlInitCommandCursorForCategory(BtlLinkedCommand *action, BtlCamState *state) {
    memset(D_003BD7D0, 0, 0x130);
    switch (action->link->unit->partyRecord.unitId) {
    case 1:
        func_001F5868(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001F5320(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 2:
        return;
    case 3:
        func_001F5868(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001F5320(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 4:
        func_001F5868(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001F5320(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 5:
        func_001F5868(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001F5320(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 6:
        btlFlagUserAndTargetDefeat(action, action);
        break;
    }
}

void func_001FFAD8(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->unk_00 == 1) {
        if (action->link->unit->status.flags & 0x400) {
            return;
        }
        func_001FA480(action, state, D_003BC0C8[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera(action, &action->camera);
    }
}

void btlInitCommandCursorForFirstActor(BtlLinkedCommand *action, BtlCamState *state) {
    BtlUnit *first;
    btlGetRuntime();
    first = btlGetIndexListEntry(action->targetList, 0);
    memset(CURSOR, 0, 0x130);
    btlRefreshUnitEffectMotionAndEntry(first);
    if (btlHasFirstLinkedCategoryFlag1000(action) != 0) {
        func_001F5868(action, state, 5, 1);
    } else {
        func_001F5868(action, state, 5, 0);
    }
    CURSOR->unk_0C = 0;
    func_001F5320(action, state, 0, 0x11);
}

void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->link->unit->status.flags & 0x400)) {
        func_001FA480(action, state, D_003BC090[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

extern void btlClearAllUnitDefeatCandidatesTask(void);

void btlInitLinkedUnitActionCursor(ActionStateLink *linkState) {
    BtlLinkedCommand *scene = &((BtlState *)btlGetRuntime())->cameraCommand;
    scene->link = linkState;
    memset(D_003BD7D0, 0, 0x130);
    CURSOR->unk_0A = 0;
    CURSOR->unk_0E = 0;
    btlRefreshUnitEffectMotionAndEntry(linkState->unit);
    func_001F5868(scene, &scene->camera, 6, 0);
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(linkState->unit);
    CURSOR->unk_0C = 0;
    func_001F5320(scene, &scene->camera, 0, 0);
}

/* vu0 routine: initialize the target cursor and orient flagged actors toward its center. */
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
    memset(D_003BD7D0, 0, 0x130);
    func_001F5868(action, state, 2, 3);
    func_001F5320(action, state, 0, 0);
    btlConstrainCameraEndpointHeight(action, state, 0, 1);
    btlFlagMatchingUnitsDefeatCandidate(0x600);
    unit = action->link->unit;
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
            func_00208000(0x400, 0, 0);
        } else {
            func_00208000(0x200, 0, 0);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlAimHorizontalDirectionVU((f32 *)aimPosition, (f32 *)position);
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(unit, (s128 *)rotation);
    }
}

s32 btlInitCursorAndApplyAction(BtlLinkedCommand *action, BtlCamState *state) {
    s32 result;
    memset(D_003BD7D0, 0, 0x130);
    func_001F5868(action, state, 2, 6);
    func_001F5320(action, state, 0, 0);
    result = btlConstrainCameraEndpointHeight(action, state, 0, 1);
    CURSOR->unk_0C = 2;
    return result;
}

void btlSpawnBattleWorldAction(void) {
    u32 worldCounter;
    s32 work;
    u32 action;

    work = btlGetRuntime();
    worldCounter = dds3AdvanceWorldCounter();
    action = (u32)evtSpawnActionObj9(worldCounter);
    ((BtlState *)work)->unk228 = action;
    D_00436AD4 = 0;
}

s32 btlAreWorkBuffersReady(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->primaryBuffer != 0) {
        if (work->secondaryBuffer != 0) {
            return 1;
        }
    }
    return 0;
}

void btlReleaseWorkBuffers(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->secondaryBuffer != 0) {
        sdfReleaseChipOrRetainedResource(work->secondaryBuffer);
        work->secondaryBuffer = 0;
    }
    if (work->primaryBuffer != 0) {
        sdfReleaseChipOrRetainedResource(work->primaryBuffer);
        work->primaryBuffer = 0;
    }
}

void btlWaitForPendingWorkAndReleaseBuffers(void) {
    s64 pending;
    s32 work;

    evtDrainSecondaryWorldNodes();
    do {
        pending = sdfCheckPendingWorkWithInterrupts();
    } while (pending != 0);
    evtDestroySecondaryWorldNode();
    do {
        pending = sdfCheckPendingWorkWithInterrupts();
    } while (pending != 0);
    btlReleaseWorkBuffers();
    work = btlGetRuntime();
    ((BtlState *)work)->battleFlags = ((BtlState *)work)->battleFlags & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418C58);

void btlFreeFieldBlocks(void) {
    BattleFieldBlocks *blocks = (BattleFieldBlocks *)btlGetRuntime();
    btlWaitForPendingWorkAndReleaseBuffers();
    if (blocks->fieldTB != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)blocks->fieldTB);
        blocks->fieldTB = 0;
        btlBossDebugPrintf(D_00418C58);
    }
    if (blocks->fieldF2 != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)blocks->fieldF2);
        blocks->fieldF2 = 0;
        btlBossDebugPrintf("btl:free field F2\n");
    }
    if (blocks->fieldF1 != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)blocks->fieldF1);
        blocks->fieldF1 = 0;
        btlBossDebugPrintf("btl:free field F1\n");
    }
    blocks = (BattleFieldBlocks *)btlGetRuntime();
    ((BtlState *)blocks)->battleFlags &= ~2;
}

void btlInitializeSceneLightingAndTint(void) {
    u8 *context = (u8 *)btlGetRuntime();
    fldApplyLightSetCurrent();
    btlInitTintTransitionResource(0x80, 0);
    VU0_LOAD_VF(vf10, (u8 *)D_0037F770[0] + 0x10);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x10);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x40);
    VU0_LOAD_VF(vf10, D_0037F770[0]);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x20);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x50);
    VU0_LOAD_VF(vf10, kwlnDefaultColorVector);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x30);
    VU0_STORE_VF_UNCLOBBERED(vf10, context + 0x60);
    ((BtlState *)context)->tint71C = 0x807E5C5E;
}

typedef struct BtlSceneLightParams {
    f32 position[3];
    f32 unk0C;
    f32 color[3];
    f32 unk1C;
    f32 secondaryColor[3];
    f32 unk2C;
} BtlSceneLightParams;

void btlBuildActionLightParameters(s32 index, BtlSceneLightParams *light) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlActionTableEntry *action;
    f32 *vector;
    f32 *defaultColor;
    u32 color;

    if ((work->battleFlags & 0x30000000) == 0) {
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
            vector = D_0037F770[0];
            defaultColor = (f32 *)kwlnDefaultColorVector;
            light->color[0] = vector[0];
            light->color[1] = vector[1];
            light->color[2] = vector[2];
            light->secondaryColor[0] = defaultColor[0];
            light->secondaryColor[1] = defaultColor[1];
            light->secondaryColor[2] = defaultColor[2];
        }
    } else {
        color = work->tint71C;
        light->color[0] = (f32)(color & 0xFF) / 255.0f;
        light->color[1] = (f32)((color >> 8) & 0xFF) / 255.0f;
        light->color[2] = (f32)((color >> 16) & 0xFF) / 255.0f;
        light->secondaryColor[0] = light->color[0];
        light->secondaryColor[1] = light->color[1];
        light->secondaryColor[2] = light->color[2];
    }
    light->position[0] = D_0037F770[0][4];
    light->position[1] = D_0037F770[0][5];
    light->position[2] = D_0037F770[0][6];
}

s32 btlGetActionDefaultOrOverride(s32 index) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->battleFlags & 0x30000000) {
        return work->unk724;
    }
    return ((BtlActionTableEntry *)datActionAnimationRecords)[index].defaultValue;
}

u32 btlCameraVectorHasNaN(void) {
    u8 *context = (u8 *)btlGetRuntime();
    if (((BtlCameraVectors *)context)->eye[0] != ((BtlCameraVectors *)context)->eye[0] ||
        ((BtlCameraVectors *)context)->eye[1] != ((BtlCameraVectors *)context)->eye[1] ||
        ((BtlCameraVectors *)context)->eye[2] != ((BtlCameraVectors *)context)->eye[2] ||
        ((BtlCameraVectors *)context)->target[0] != ((BtlCameraVectors *)context)->target[0] ||
        ((BtlCameraVectors *)context)->target[1] != ((BtlCameraVectors *)context)->target[1] ||
        ((BtlCameraVectors *)context)->target[2] != ((BtlCameraVectors *)context)->target[2]) {
        return 1;
    }
    return 0;
}

void btlInitTintTransitionResource(u32 resource, u16 soundId) {
    u32 handle;
    D_003BDC90.currentId = soundId;
    D_003BDC90.nextId = soundId;
    handle = fldGetSkyDrawState();
    D_003BDC90.resource = resource;
    D_003BDC90.handle = handle;
}

void btlInitTintTransitionDefault(u16 soundId) {
    u32 handle;
    D_003BDC90.currentId = soundId;
    D_003BDC90.nextId = soundId;
    handle = fldGetSkyDrawState();
    D_003BDC90.handle = handle;
    D_003BDC90.resource = 0x80;
}

void btlStepTintTransition(void) {
    SoundCommand *cmd = &D_003BDC90;
    u32 value;
    u32 start;
    if (cmd->currentId != 0) {
        cmd->currentId += 0xFFFF;
        start = cmd->resource;
        value = (f32)(s32)(cmd->handle - start) * ((f32)cmd->currentId / (f32)cmd->nextId);
        fldSetSkyDrawState(value + D_003BDC90.resource);
    } else {
        fldSetSkyDrawState(cmd->resource);
    }
}

void btlQueueTintTransition(u32 color, u16 frames) {
    BtlTintTransition *transition;
    if (frames == 0) {
        transition = (BtlTintTransition *)D_003BDCA0;
        transition->framesRemaining = 0;
        transition->currentColor = color;
        transition->targetColor = color;
        return;
    }
    transition = (BtlTintTransition *)D_003BDCA0;
    transition->framesRemaining = frames;
    transition->durationFrames = frames;
    transition->sourceColor = transition->currentColor;
    transition->targetColor = color;
}

void btlQueueTintTransitionToZero(u16 frames) {
    BtlTintTransition *transition;
    if (frames == 0) {
        transition = (BtlTintTransition *)D_003BDCA0;
        transition->framesRemaining = 0;
        transition->currentColor = 0;
        transition->targetColor = 0;
        return;
    }
    transition = (BtlTintTransition *)D_003BDCA0;
    transition->sourceColor = transition->currentColor;
    transition->targetColor = 0;
    transition->framesRemaining = frames;
    transition->durationFrames = frames;
}

extern u32 btlBlendColor(u32, u32, f32);

void btlStepBlendColor(void) {
    BtlTintTransition *transition = (BtlTintTransition *)D_003BDCA0;
    if (transition->framesRemaining != 0) {
        transition->currentColor = btlBlendColor(transition->targetColor, transition->sourceColor,
                                                    (f32)transition->framesRemaining / (f32)transition->durationFrames);
        transition->framesRemaining += 0xFFFF;
    } else {
        transition->currentColor = transition->targetColor;
    }
    btlStepTintTransition();
}

void btlDrawTintIfVisible(void) {
    BtlTintTransition *transition = (BtlTintTransition *)D_003BDCA0;
    if (transition->currentColor & 0xFF000000) {
        func_0018F840(transition);
    }
}

void sndResetTransition(void) {
    BtlTintTransition *transition = (BtlTintTransition *)D_003BDCA0;
    D_00436AD4 = 0;
    btlTintTransitionHoldCount = 0;
    transition->framesRemaining = 0;
    transition->currentColor = 0;
}

void btlClearTintAndEnableCamera(void) {
    btlQueueTintTransitionToZero(0);
    effObjSetOpacityPassEnabled(1);
}

void btlUpdateTintAndWorldLight(void) {
    u8 *context = (u8 *)btlGetRuntime();
    f32 *position = D_0037F770[0];

    if (position[0] == 0.0f && position[1] == 0.0f &&
        position[2] == 0.0f) {
        effObjSetOpacityPassEnabled(0);
    } else {
        effObjSetOpacityPassEnabled(1);
    }
    if (((BtlState *)context)->commandRestrictFlags & 0x20) {
        effObjSetOpacityPassEnabled(0);
    }
    btlStepBlendColor();
}

void btlTickFieldSwayAndTint(void) {
    s32 work;

    work = btlGetRuntime();
    if ((((((BtlState *)work)->battleFlags & 0x20000) != 0) && ((((BtlState *)work)->commandRestrictFlags & 0x20) == 0)) &&
          ((((BtlState *)work)->unk220 & 0x4000000) == 0)) {
        func_001355D8();
        fldUpdateSwayOffset();
        func_00134A18();
    }
    btlDrawTintIfVisible();
}

extern WorldTransformSetup D_00452F50;

extern void dds3LoadWorldTransformSetup(EffWorldNode *, WorldTransformSetup *);

extern void evtBeginUnitValueColorTransition(EffWorldNode *, s32);

void func_00200930(f32 *position, f32 *scale, s32 value) {
    BtlState *work = (BtlState *)btlGetRuntime();
    s32 listener = work->unk228;

    D_00452F50.transform.position[0] = position[0];
    D_00452F50.transform.position[1] = position[1];
    D_00452F50.transform.position[2] = position[2];
    D_00452F50.transform.scale[0] = scale[0];
    D_00452F50.transform.scale[1] = scale[1];
    D_00452F50.transform.scale[2] = scale[2];
    D_00452F50.unk00 = 0;
    D_00452F50.flags = 0;
    D_00452F50.mode = 0;
    D_00452F50.transform.rotation[0] = 0.0f;
    D_00452F50.transform.rotation[1] = 0.0f;
    D_00452F50.transform.rotation[2] = 0.0f;
    D_00452F50.transform.rotation[3] = 0.0f;
    dds3LoadWorldTransformSetup((EffWorldNode *)listener, &D_00452F50);
    evtBeginUnitValueColorTransition((EffWorldNode *)work->unk228, value);
}

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
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_003BDCC8);
            break;
        }
        break;
    case 0xE0:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_003BDCC8);
        }
        break;
    case 0xE1:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_003BDCC8);
        }
        break;
    }
    if (work->soundTransitionTask != 0) {
        btlBossDebugPrintf("btl:rain create[f%03X_%03X]\n", kind, arg);
    }
}

void btlReleaseRainSoundTransition(void) {
}

void btlStopRainSoundTransition(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->soundTransitionTask != 0) {
        btlBossDebugPrintf("btl:rain exit\n");
        effReleaseSelectionFlagList(work->soundTransitionTask);
        work->soundTransitionTask = 0;
    }
}

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

u32 btlPollFieldArchiveLoad(args)
    BtlFieldLoadArgs *args;
{
    BattleFieldBlocks *blocks = (BattleFieldBlocks *)btlGetRuntime();
    char directory[0x80];
    char path[0x80];
    BtlFieldArchiveNode *node;
    u32 i;

    if (args->frame == 0) {
        btlFreeFieldBlocks();
        fldFormatAreaDirectory(directory, args->stage, 1);
        func_0035C860(path, "%sf%03d_%03d.LB", directory, args->stage, args->variant);
        btlBossDebugPrintf("btl:field load[%s]\n", path);
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
                    blocks->fieldTB = node->handle;
                    args->fieldTB = node->data;
                    break;
                case 1:
                    blocks->fieldF2 = node->handle;
                    args->fieldF2 = node->data;
                    break;
                case 2:
                    blocks->fieldF1 = node->handle;
                    args->fieldF1 = node->data;
                    break;
                }
                node = node->next;
                i++;
            }
            func_002C7CE8(request);
            args->request = NULL;
        }
        if (args->fieldF1 != NULL && args->fieldF2 != NULL && args->fieldTB != NULL) {
            evtCreateWorldObjectFromResource(args->stage, args->variant,
                                             args->fieldF1, args->fieldF2,
                                             (const SdfTextureOffsetListHeader *)args->fieldTB, 0);
            btlInitializeSceneLightingAndTint();
            if (blocks->fieldTB != 0) {
                sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)blocks->fieldTB);
                blocks->fieldTB = 0;
                btlBossDebugPrintf(D_00418C58);
            }
            ((BtlState *)blocks)->battleFlags |= 2;
            btlBossDebugPrintf("btl:field load end[f%03d_%03d]\n", args->stage, args->variant);
            return 1;
        }
    }
    args->frame++;
    return 0;
}

extern u32 btlPollFieldArchiveLoad();

BtlRuntimeTask *fldCreateSceneTileTask(s32 value, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0x28);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 1;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->callback = btlPollFieldArchiveLoad;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    memset(args, 0, 0x28);
    args->value = value;
    args->option = option;
    args->unk_24 = 0;
    return task;
}

typedef struct BtlFloorLoadArgs {
    s32 frontHandle;
    s32 sideHandle;
    s32 stage;
    s32 variant;
    s32 state;
} BtlFloorLoadArgs;

s32 btlPollFloorLoadTask(BtlFloorLoadArgs *args) {
    s32 result = 1;
    BtlState *work = (BtlState *)btlGetRuntime();
    s32 stage = args->stage;
    s32 variant = args->variant;
    char path[0x70];
    if (args->state == 0) {
        func_0035C860(path, "/fld/b/f%03d/f%03d_%03df.tmx", stage, stage, variant);
        result = 0;
        args->frontHandle = (s32)fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf("btl:load 0[%s]\n", path);
        func_0035C860(path, "/fld/b/f%03d/f%03d_%03ds.tmx", stage, stage, variant);
        args->sideHandle = (s32)fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf("btl:load 1[%s]\n", path);
    } else {
        if (args->frontHandle != 0) {
            if (fileIsRequestReadyInCurrentMode((struct FileRequest *)args->frontHandle) != 0) {
                work->primaryBuffer = (void *)sdfResourceRetainAddress((struct SdfMemBlock *)(fileGetResourceHandle((struct FileRequest *)args->frontHandle)));
                filePollEntryCleanup((struct FileRequest *)(u32)args->frontHandle);
                args->frontHandle = 0;
                btlBossDebugPrintf("btl:floor load end 0\n");
            } else {
                result = 0;
            }
        }
        if (args->sideHandle != 0) {
            if (fileIsRequestReadyInCurrentMode((struct FileRequest *)args->sideHandle) != 0) {
                work->secondaryBuffer = (void *)sdfResourceRetainAddress((struct SdfMemBlock *)(fileGetResourceHandle((struct FileRequest *)args->sideHandle)));
                filePollEntryCleanup((struct FileRequest *)(u32)args->sideHandle);
                args->sideHandle = 0;
                btlBossDebugPrintf("btl:floor load end 1\n");
            } else {
                result = 0;
            }
        }
    }
    args->state++;
    return result;
}

BtlRuntimeTask *btlCreateFloorLoadTask(s32 first, s32 second) {
    BtlRuntimeTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 2;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->callback = btlPollFloorLoadTask;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    args->unk_08 = first;
    args->unk_0C = second;
    args->value = 0;
    args->option = 0;
    args->unk_10 = 0;
    return task;
}

typedef struct SceneLightTransitionArgs {
    f32 direction[4];
    f32 lightColor[4];
    f32 ambientColor[4];
    u32 value;
} SceneLightTransitionArgs;

extern u32 btlBlendColorVec(f32 *colorA, f32 *colorB, f32 blendFactor);

u32 func_00200FB8(SceneLightTransitionArgs *args) {
    f32 *first = args->lightColor;
    f32 *second = args->ambientColor;
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    f32 *lightColor;
    f32 *ambientColor;
    u32 firstColor;
    u32 secondColor;
    u32 transitionCount;

    func_00200930(first, second, args->value);
    lightColor = work->lightColor;
    PCP_COPY_VECTOR_F32(lightColor, first);
    ambientColor = work->ambientColor;
    PCP_COPY_VECTOR_F32(ambientColor, second);
    PCP_COPY_VECTOR_F32(work->lightDirection, args->direction);
    for (unit = work->units; unit != NULL; unit = unit->nextActor) {
        if (!(unit->status.flags & 2)) {
            continue;
        }
        if (!(unit->status.stateFlags & 0x10)) {
            continue;
        }
        if (unit->ext != NULL) {
            evtSetUnitStatusFlags(unit->ext);
            firstColor = btlBlendColorVec(lightColor, unit->colorStart, 0.3f);
            secondColor = btlBlendColorVec(ambientColor, unit->colorEnd, 0.3f);
            evtInitializeUnitColorTransition(unit->ext, args->value, firstColor, secondColor);
        }
    }
    transitionCount = D_00436AD4;
    D_0037F770[0][4] = args->direction[0];
    D_0037F770[0][5] = args->direction[1];
    D_0037F770[0][6] = args->direction[2];
    D_00436AD4 = transitionCount + 1;
    return 1;
}


BtlRuntimeTask *btlCreateEffectTaskWithSourceParams(u8 *source, u32 value) {
    BtlRuntimeTask *task = btlAllocTask(0x34);
    u8 *arguments;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 3;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->callback = func_00200FB8;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
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

    if (D_00436AD4 == 0) {
        return 1;
    }
    D_00436AD4--;
    if (D_00436AD4 == 0) {
        work = (BtlState *)btlGetRuntime();
        if (work->battleFlags & 0x20000000) {
            return 1;
        }
        if (D_0037F770[0][0] == 0.0f && D_0037F770[0][1] == 0.0f && D_0037F770[0][2] == 0.0f) {
            VEC3_SPLAT(D_0037F770[0], 0.0078125f);
        }
        listener = work->unk228;
        D_00452F50.transform.position[0] = work->baselineLightColor[0];
        D_00452F50.transform.position[1] = work->baselineLightColor[1];
        D_00452F50.transform.position[2] = work->baselineLightColor[2];
        D_00452F50.transform.scale[0] = work->baselineAmbientColor[0];
        D_00452F50.transform.scale[1] = work->baselineAmbientColor[1];
        D_00452F50.transform.scale[2] = work->baselineAmbientColor[2];
        D_00452F50.unk00 = 0;
        D_00452F50.flags = 0;
        D_00452F50.mode = 0;
        D_00452F50.transform.rotation[0] = 0.0f;
        D_00452F50.transform.rotation[1] = 0.0f;
        D_00452F50.transform.rotation[2] = 0.0f;
        D_00452F50.transform.rotation[3] = 0.0f;
        dds3LoadWorldTransformSetup((EffWorldNode *)listener, &D_00452F50);
        evtBeginUnitValueColorTransition((EffWorldNode *)work->unk228, args->value);
        PCP_COPY_VECTOR_F32(work->lightColor, work->baselineLightColor);
        PCP_COPY_VECTOR_F32(work->ambientColor, work->baselineAmbientColor);
        PCP_COPY_VECTOR_F32(work->lightDirection, work->baselineLightDirection);
        for (unit = work->units; unit != NULL; unit = unit->nextActor) {
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
        D_0037F770[0][4] = work->baselineLightDirection[0];
        D_0037F770[0][5] = work->baselineLightDirection[1];
        D_0037F770[0][6] = work->baselineLightDirection[2];
    }
    return 1;
}

BtlRuntimeTask *func_002014A8(value)
    u32 value;
{
    BtlRuntimeTask *task = btlAllocTask(4);
    SceneLightRestoreArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 4;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->callback = btlRestoreSceneTransformLighting;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

s64 func_00201520(SceneLightRestoreArgs *args) {
    D_00436AD4 = 1;
    return btlRestoreSceneTransformLighting(args);
}

BtlRuntimeTask *btlCreateSoundUpdateTask(u32 value) {
    BtlRuntimeTask *task = func_002014A8(value);
    task->taskId = 7;
    task->callback = func_00201520;
    return task;
}

s32 btlQueueTintTransitionWhenEnabled(const BtlTintAcquireArgs *args) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (!(work->battleFlags & 0x20000000)) {
        btlQueueTintTransition(args->color, (u16)args->frames);
    }
    btlTintTransitionHoldCount++;
    return 1;
}

BtlRuntimeTask *sndCreateAcquireTask(s32 color, s32 frames) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlTintAcquireArgs));
    BtlTintAcquireArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 5;
    task->callback = btlQueueTintTransitionWhenEnabled;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->color = color;
    args->frames = frames;
    return task;
}

s32 sndTickFadeCounter(const BtlTintReleaseArgs *args) {
    s32 context = btlGetRuntime();

    if (btlTintTransitionHoldCount == 0) {
        return 1;
    }
    btlTintTransitionHoldCount--;
    if (btlTintTransitionHoldCount != 0) {
        return 1;
    }
    if ((((BtlState *)context)->battleFlags & 0x20000000) != 0) {
        return 1;
    }
    btlQueueTintTransitionToZero((u16)args->frames);
    return 1;
}


BtlRuntimeTask *sndCreateReleaseTask(u32 frames)
{
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlTintReleaseArgs));
    BtlTintReleaseArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 6;
    task->callback = sndTickFadeCounter;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->frames = frames;
    return task;
}

s64 func_00201718(const BtlTintReleaseArgs *args) {
    btlTintTransitionHoldCount = 1;
    return sndTickFadeCounter(args);
}

BtlRuntimeTask *btlCreateSoundReleaseTask(u32 frames) {
    BtlRuntimeTask *task = sndCreateReleaseTask(frames);
    task->taskId = 8;
    task->callback = func_00201718;
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

s32 sndLookupResourceType(s32 actor, s32 resourceIndex) {
    s32 (*hook)(s32, s32) = ((BtlState *)btlGetRuntime())->unk684;
    s32 type;
    if (hook != 0) {
        type = hook(actor, resourceIndex);
        if (type != -1) {
            return type;
        }
    }
    return ((BtlActionTableEntry *)datActionAnimationRecords)[resourceIndex].resourceType;
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_00201828);

void sndCreateSystemEffect(SoundResourceNode *effect) {
    if (effect->flags & 8) {
        if (effect->resourceHandle == 0) {
            if (effect->referenceCount == 0) {
                effect->resourceHandle = sndMixerClone(effect->sourceHandle);
                btlBossDebugPrintf("btl:system effect create[%p]\n", effect->resourceHandle);
            }
        }
    }
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
    BtlUnit *target;
    BtlUnit *source;
    args->effect = 0;
    sndCreateSystemEffect(args->source);
    effect = args->source;
    target = args->targetOwner.unit;
    source = args->sourceOwner.unit;
    effect->referenceCount = effect->referenceCount + 1;
    source->effectLink.referenceCount = source->effectLink.referenceCount + 1;
    target->effectLink.referenceCount = target->effectLink.referenceCount + 1;
}

extern void effBattleSetInputValue(BattleEffect *, s32);

s32 sndUpdateReferencedBattleEffect(SoundEffectReferenceArgs *args) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *owner;
    BtlUnit *source;

    if ((battle->battleFlags & 0x40000) == 0) {
        return 1;
    }
    owner = args->option == 2 ? args->targetOwner.unit : args->sourceOwner.unit;
    if (args->source->flags & 2) {
        if (args->effect == NULL) {
            args->effect = func_00168548(args->source->resourceHandle, args->option, owner, 0);
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
        if (effBTLFieldColorTestFlags(2)) {
            fileSetRenderFlag(EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH);
        } else if (effBTLFieldColorTestFlags(1)) {
            source = args->sourceOwner.unit;
            if (source != NULL && args->targetOwner.unit != NULL &&
                (((source->status.flags & 0x200) && !(source->effectLink.flags & 0x20)) ||
                 ((args->targetOwner.unit->status.flags & 0x400) && (source->effectLink.flags & 0x20)))) {
                fileSetRenderFlag(EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH);
            } else {
                fileClearRenderFlag(EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH);
            }
        }
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
        func_00168978(args->effect);
    }
    return 0;
}

void sndReleaseEffectReferences(SoundEffectReferenceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *target;
    BtlUnit *source;
    if (args->effect != 0) {
        effReleaseBattleVoiceOwner(args->effect);
    }
    effect = args->source;
    target = args->targetOwner.unit;
    source = args->sourceOwner.unit;
    effect->referenceCount = effect->referenceCount - 1;
    source->effectLink.referenceCount = source->effectLink.referenceCount - 1;
    target->effectLink.referenceCount = target->effectLink.referenceCount - 1;
    sndDeleteSystemEffect(effect);
}

extern s32 sndUpdateReferencedBattleEffect(SoundEffectReferenceArgs *);

BtlRuntimeTask *btlCreateReferencedSoundEffectTask(SoundResourceNode *effect, BtlUnit *source,
                                                 BtlUnit *actor, u16 frames) {
    s32 v1 = 0;
    s32 v2;
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundEffectReferenceArgs));
    SoundEffectReferenceArgs *args;
    BtlState *work;
    ActorEffectOwner sourceOwner;
    sourceOwner.unit = source;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x2E;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = actor->owner;
    task->onStart = sndAddEffectReferences;
    task->callback = sndUpdateReferencedBattleEffect;
    task->onFinish = sndReleaseEffectReferences;
    work = (BtlState *)btlGetRuntime();
    if (work->unk6E4 != 0) {
        v1 = work->unk6E4(actor);
    }
    if (v1 == 0) {
        v1 = sourceOwner.selectorKey;
    }
    v2 = 0;
    if (work->unk6E8 != 0) {
        v2 = work->unk6E8(actor);
    }
    if (v2 == 0) {
        v2 = sourceOwner.selectorKey;
    }
    if (work->selectSoundEffectTarget != 0) {
        BtlUnit *r = work->selectSoundEffectTarget(actor);
        if (r != 0) {
            actor = r;
        }
    }
    args = btlGetTaskArguments(task);
    args->source = effect;
    args->sourceOwner.unit = source;
    args->sourceSelector = v1;
    args->targetSelector = v2;
    args->option = frames;
    args->targetOwner.unit = actor;
    args->effect = 0;
    args->duration = 0;
    return task;
}

BtlRuntimeTask *sndCreateEffectWithTargets(SoundResourceNode *effect, BtlUnit *sourceOwner,
                                         s32 source, s32 target, BtlUnit *actor, s32 frames) {
    BtlRuntimeTask *task = btlCreateReferencedSoundEffectTask(effect, sourceOwner, actor, frames & 0xFFFF);
    SoundEffectReferenceArgs *args = btlGetTaskArguments(task);
    args->sourceSelector = source;
    args->targetSelector = target;
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

s32 btlUpdateActorSystemEffect(ActorEffectTaskArgs *args) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    ActorEffectOwner owner = args->owner;
    BtlUnit *unit = owner.unit;

    if ((battle->battleFlags & 0x40000) == 0) {
        return 1;
    }
    if (args->effect == 0) {
        args->effect = func_00168548(args->source->resourceHandle, 0, unit, 0);
        effBattleUpdateSelectedValue(args->effect, args->duration);
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
        func_00168978(args->effect);
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
    task->taskId = 0x2F;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = owner->owner;
    task->onStart = sndStartEffectTask;
    task->callback = btlUpdateActorSystemEffect;
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
            btlApplyScaledUnitEffectParameter(args->unit, args->channel, args->volume, 1.0f);
        }
        return 1;
    }
    args->frame++;
    return 0;
}

BtlRuntimeTask *sndCreateTimedUnitEffectTask(SoundResourceNode *effect, BtlUnit *actor, u16 frames,
                                          s32 channel, u32 volume) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(TimedUnitEffectArgs));
    TimedUnitEffectArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x31;
    task->ownerId = actor->owner;
    task->onStart = sndIncrementEffectActiveCount;
    task->callback = sndWaitEffectFramesAndApplyUnitParameter;
    task->onFinish = 0;
    args = btlGetTaskArguments(task);
    args->source = effect;
    args->unit = actor;
    args->option = frames;
    args->channel = channel;
    args->volume = volume;
    args->frame = 0;
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
    args->request = fileQueueDefaultCallbackRequest(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

s32 sndPollEffectLoad(EffectLoadArgs *args) {
    SoundResourceNode *effect = args->effect;
    struct SdfMemBlock *resource;
    if (effect->flags & 2) {
        return 1;
    }
    if (fileIsRequestReadyInCurrentMode(args->request) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:effect load end[%s]\n", args->name);
    resource = (struct SdfMemBlock *)fileGetResourceHandle(args->request);
    effect->resourceHandle = sndMixerClone((void *)sdfResourceRetainAddress(resource));
    sdfReleaseResourceAllocation(resource);
    filePollEntryCleanup(args->request);
    effect->flags = (effect->flags & ~1) | 2;
    return 0;
}

BtlRuntimeTask *sndCreateEffectLoadTask(SoundResourceNode *effect, char *name) {
    BtlRuntimeTask *task = btlAllocTask(strlen(name) + sizeof(EffectLoadArgs));
    EffectLoadArgs *args;
    char *copy;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x32;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->onStart = sndBeginEffectLoad;
    task->callback = sndPollEffectLoad;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    copy = (char *)(args + 1);
    args->effect = effect;
    args->name = copy;
    strcpy(copy, name);
    return task;
}

u32 btlWaitUnitListIdle(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    if (work->battleFlags & 0x40000000) {
        return 1;
    }
    for (unit = work->units; unit != 0; unit = unit->nextActor) {
    }
    return 1;
}

BtlRuntimeTask *btlCreateWaitUnitListIdleTask(u32 value) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x33;
    task->callback = btlWaitUnitListIdle;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

u32 sndApplyToActiveActors(taskArgs)
    s32 *taskArgs;
{
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 2) {
                if (unit->ext != 0) {
                    if (!(unit->status.flags & 0xE0)) {
                        btlBlendUnitColor(unit, unit->baseColor, *taskArgs);
                    }
                }
            }
        }
    }
    return 1;
}

BtlRuntimeTask *btlCreateApplyToActiveActorsTask(u32 value) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x34;
    task->callback = sndApplyToActiveActors;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

u32 btlCancelTimedFadeTask(void) {
    kwlnCancelConfiguredFadeFrames();
    return 1;
}

BtlRuntimeTask *btlCreateFadeStateResetTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlCancelTimedFadeTask;
    task->taskId = 0x35;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

void sndAddSourceReferences(SoundEffectSourceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *unit;
    args->effect = 0;
    sndCreateSystemEffect(args->source);
    effect = args->source;
    unit = args->owner.unit;
    effect->referenceCount = effect->referenceCount + 1;
    unit->effectLink.referenceCount = unit->effectLink.referenceCount + 1;
}

s32 sndUpdateEffectSourceFade(SoundEffectSourceArgs *args) {
    BtlUnit *owner;
    u32 flags;

    btlGetRuntime();
    owner = args->owner.unit;

    if (args->effect == NULL) {
        args->effect = func_00168548(args->source->resourceHandle, 0, owner, 0);
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
        func_00168978(args->effect);
    }
    args->frameCount++;
    return 0;
}

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

extern s32 sndUpdateEffectSourceFade(SoundEffectSourceArgs *);

BtlRuntimeTask *sndCreateEffectSourceTask(SoundResourceNode *effect, BtlUnit *owner, u64 resource) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundEffectSourceArgs));
    SoundEffectSourceArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x30;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = owner->owner;
    task->onStart = sndAddSourceReferences;
    task->callback = sndUpdateEffectSourceFade;
    task->onFinish = sndFinishEffectSourceTask;
    args = btlGetTaskArguments(task);
    args->source = effect;
    args->owner.unit = owner;
    args->resource = resource;
    args->effect = 0;
    args->frameCount = 0;
    args->fadeOutFrame = 0;
    return task;
}

u32 btlTaskStartFadeIn(u32 *taskArgs) {
    kwlnFadeStartIn(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateFadeInTask(u32 value) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x38;
    task->callback = btlTaskStartFadeIn;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

u32 btlTaskStartCustomFadeIn(u8 *taskArgs) {
    kwlnFadeInStart(*taskArgs, taskArgs[1], taskArgs[2], *(u32 *)(taskArgs + 4));
    return 1;
}

extern u32 btlTaskStartCustomFadeIn(u8 *);

BtlRuntimeTask *sndCreateCustomTask(s32 value, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x39;
    task->callback = btlTaskStartCustomFadeIn;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->value = value;
    args->option = option;
    return task;
}

u32 btlTaskSetBattleFlag40000(void) {
    s32 work;

    work = btlGetRuntime();
    ((BtlState *)work)->battleFlags = ((BtlState *)work)->battleFlags | 0x40000;
    mdlClearListedObjectFlag();
    return 1;
}

BtlRuntimeTask *sndCreateSetBattleFlagTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlTaskSetBattleFlag40000;
    task->taskId = 0x3A;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

u32 btlTaskClearBattleFlag40000(void) {
    s32 work;

    work = btlGetRuntime();
    ((BtlState *)work)->battleFlags = ((BtlState *)work)->battleFlags & 0xfffbffff;
    mdlSetListedObjectFlag();
    return 1;
}

BtlRuntimeTask *sndCreateClearBattleFlagTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlTaskClearBattleFlag40000;
    task->taskId = 0x3B;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

extern const char D_00436AE0[];

const char D_00418E28[16] __attribute__((aligned(8))) = "%s%03X.BED";

const char D_00418E38[32] __attribute__((aligned(8))) = "/efftool/bed/BTL_TEST.BED";

s32 btlFormatActionEventFilename(s32 index, char *output) {
    BtlState *state = (BtlState *)btlGetRuntime();
    if ((state->battleFlags & 0x10000000) == 0) {
        u16 assetId = ((BtlActionAnimationRecord *)datActionAnimationRecords)[index].displayCode;
        if (assetId == 0) return 0;
        func_0035C860(output, D_00418E28, D_00436AE0, assetId);
    } else {
        func_0035C860(output, D_00418E38);
    }
    return 1;
}

s32 sndSetEffectNodeParameter(SoundResourceNode *effect, u16 option) {
    return sndReadSelectedMixerBankValue(effect->resourceHandle, option);
}

s32 sndGetEffectNodeParameter(SoundResourceNode *effect, u16 option) {
    return func_00168448(effect->resourceHandle, option);
}

s32 sndIsResourceNodeReferencedOrActive(SoundResourceNode *effect) {
    if (effect->referenceCount != 0) {
        return 1;
    }
    return effect->activeCount != 0;
}

s32 sndHasActiveActor(void) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (unit->node318 != 0 && sndIsResourceNodeReferencedOrActive(unit->node318) != 0) {
            return 1;
        }
    }
    return 0;
}

/* Allocate a cleared resource node and prepend it to the runtime's resource list. */
SoundResourceNode *sndAllocResourceNode(void) {
    SoundResourceNode *node = sdfAllocAndClearQuadwords(0x20);
    BtlState *work;
    node->referenceCount = 0;
    node->activeCount = 0;
    node->fadeCountdown = 0;
    node->resourceHandle = 0;
    work = (BtlState *)btlGetRuntime();
    node->previous = 0;
    if (work->soundResourceHead != 0) {
        work->soundResourceHead->previous = node;
        node->next = work->soundResourceHead;
    } else {
        node->next = 0;
    }
    work->soundResourceHead = node;
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
    if (node->resourceHandle != 0) {
        sndReleaseAllVoices(node->resourceHandle);
    }
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        ((BtlState *)btlGetRuntime())->soundResourceHead = node->next;
    }
    sdfReleaseChipBlock(node);
}

/* Advance resource countdowns and battle tint, then tick slot-volume fades. */
void btlUpdateFadeColor(void) {
    BtlState *context = (BtlState *)btlGetRuntime();
    SoundResourceNode *node;

    for (node = context->soundResourceHead; node != 0; node = node->next) {
        if (node->referenceCount == 0) {
            node->fadeCountdown = 0;
        } else if (node->fadeCountdown > 0) {
            node->fadeCountdown = node->fadeCountdown - 1;
        }
    }
    if ((u32)(btlGetActiveUnitId() - 9) < 2 || context->boundTask != 0 || context->currentScene == 8) {
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
    SoundResourceNode *node;
    SoundResourceNode *next;
    for (node = ((BtlState *)btlGetRuntime())->soundResourceHead; node != 0; node = next) {
        next = node->next;
        sndFreeResourceNode(node);
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
    SoundResourceLink *link = sdfAllocAndClearQuadwords(sizeof(SoundResourceLink));
    link->owner = owner;
    link->effectHandle = 0;
    link->opaque0C = 0;
    link->effect = 0;
    return link;
}

void sndFreeResourceLink(SoundResourceLink *link) {
    if (link->effectHandle != 0) {
        effReleaseBattleVoiceOwner(link->effectHandle);
        link->effect->referenceCount--;
        sndDeleteSystemEffect(link->effect);
    }
    sdfReleaseChipBlock(link);
}

INCLUDE_ASM(const s32, "game/code_001EB5B0", func_002034A8);

void btlMarkTaskReady(SoundResourceLink *resource) {
    resource->refreshRequested = 1;
}

SoundLink *sndAllocLink(BtlUnit *owner) {
    SoundLink *link = sdfAllocAndClearQuadwords(0x10);
    link->owner = owner;
    link->effectHandle = 0;
    link->commandEffectId = 0;
    link->effect = 0;
    return link;
}

void sndFreeLink(SoundLink *link) {
    if (link->effectHandle != 0) {
        effReleaseBattleVoiceOwner(link->effectHandle);
        link->effect->referenceCount = link->effect->referenceCount - 1;
        sndDeleteSystemEffect(link->effect);
    }
    sdfReleaseChipBlock(link);
}

/* Blend the linked command effect through the SDK's packed-color vectors. */
void btlUpdateUnitCommandEffect(SoundLink *link) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *actor = link->owner;
    u16 effectId;

    if (actor->selectedEntryIndex > 0) {
        effectId = datCommandRecords[actor->selectedEntryIndex].unk2E;
    } else {
        effectId = 0;
    }
    if (effectId != 0 && !(actor->partyRecord.status & 0x4000)) {
        if (link->effectHandle == 0) {
            link->effect = work->resources[BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT];
            sndCreateSystemEffect(link->effect);
            link->effectHandle = func_00168548(link->effect->resourceHandle, 2, actor, 0);
            link->effect->referenceCount++;
            link->effectHandle->flags = (link->effectHandle->flags | 1) & ~6;
        }
        link->commandEffectId = effectId;
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
        func_00168978(link->effectHandle);
    }
}

/* Clear the fade gate and report completion; the frame updater may overwrite it. */
u32 btlDisableBattleFade(void) {
    BtlState *work;

    work = (BtlState *)btlGetRuntime();
    work->fadeEnabled = 0;
    return 1;
}

BtlRuntimeTask *sndCreateClearStateTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlDisableBattleFade;
    task->taskId = 0x36;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

/* Set the fade gate and report completion; the frame updater may overwrite it. */
u32 btlEnableBattleFade(void) {
    BtlState *work;

    work = (BtlState *)btlGetRuntime();
    work->fadeEnabled = 1;
    return 1;
}

BtlRuntimeTask *sndCreateSetStateTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->callback = btlEnableBattleFade;
    task->taskId = 0x37;
    task->onStart = 0;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    return task;
}

/* Consume archive records only for enabled SYSEFF rows; clear unavailable entries. */
INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418E58);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418E70);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418E88);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418EA0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418EB8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418ED0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418EE8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418F00);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418F18);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418F30);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418F48);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418F60);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418F78);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418F90);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418FA8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418FC0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418FD8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00418FF0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419008);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419020);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419040);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419060);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419080);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419098);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004190B0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004190C8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004190E0);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004190F8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419110);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419128);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419140);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419158);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419170);

void sndLoadSysEffLb(void) {
    const char *path = "/battle/SYSEFF.LB";
    s32 archive = (s32)fileQueuePlainDispatchRequest(path);
    s32 node;
    u32 i;

    func_002C81D0((struct FileRequest *)archive);
    btlBossDebugPrintf("btl:[%s]\n", path);
    node = *(s32 *)(archive + 0x60);
    i = 0;
    while (node != 0) {
        if (D_003BDE18[i].unk0 != 0) {
            u32 unk08 = *(u32 *)(node + 8);
            u32 unk0C = *(u32 *)(node + 0xC);
            D_003BDE18[i].unk8 = unk08;
            D_003BDE18[i].unk4 = unk0C;
            node = *(s32 *)node;
        } else {
            D_003BDE18[i].unk4 = 0;
            D_003BDE18[i].unk8 = 0;
        }
        i++;
    }
    for (; i < BTL_SOUND_ENTRY_COUNT; i++) {
        D_003BDE18[i].unk4 = 0;
        D_003BDE18[i].unk8 = 0;
    }
    func_002C7CE8((void *)archive);
}

/* Register available SYSEFF handles; unavailable slots are left untouched. */
void btlRefreshSoundEntries(void) {
    u32 i;
    btlGetRuntime();
    for (i = 0; i < BTL_SOUND_ENTRY_COUNT; i++) {
        if (D_003BDE18[i].unk4 != 0) {
            /* Bank entries retain their original data-address word representation. */
            btlCreateIndexedSoundResourceNode(i, (void *)D_003BDE18[i].unk4);
        }
    }
}

/* Free and clear slots selected by the archive table, without discarding metadata. */
void sndFreeBattleSoundEntries(void) {
    SoundResourceNode **slots = ((BtlState *)btlGetRuntime())->resources;
    u32 i;
    for (i = 0; i < BTL_SOUND_ENTRY_COUNT; i++) {
        if (D_003BDE18[i].unk4 != 0) {
            sndFreeResourceNode(slots[i]);
            slots[i] = 0;
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
    ((BtlState *)work)->resources[slotIndex] = node;
    node->flags = flags | 10;
}

typedef struct SoundHandleNode {
    struct FileQueue *queue;
    void *actor;
} SoundHandleNode;

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(8);
    SoundEntry *entry = &D_003BDE18[index];
    node->actor = actor;
    node->queue = fileCloneQueueEntries((struct FileQueue *)entry->unk4);
    return node;
}

void btlUpdateJobPositionFromModel(s32 *args) {
    SoundHandleNode *node = (SoundHandleNode *)args;
    f32 pos[4];
    if (sdfLoadMapRecordPositionVector(((MdlCtx *)node->actor)->inner, 1) == 0) {
        mdlLoadPrimaryVectorVU((MdlCtx *)node->actor);
        VU0_STORE_VF(vf10, pos);
        pos[1] -= 150.0f;
    } else {
        VU0_STORE_VF(vf10, pos);
    }
    fileQueueSetPosition(node->queue, pos);
    fileQueueUpdate(node->queue);
}

void sndDestroyFileQueueWrapper(u32 queue) {
    SoundHandleNode *node = (SoundHandleNode *)queue;
    fileQueueDestroy(node->queue);
    sdfReleaseChipBlock(node);
}

void func_00203EC0(void) {
}

void sndSetStationedSeVolume(u32 sequence) {
    sndSetSequenceVolumePan(sequence, 0x58, 0x3f);
}

void sndSetStationedSeHighVolume(u32 sequence) {
    sndSetSequenceVolumePan(sequence, 0x319c, 0x3f);
}

void btlSelectSceneAudioTrack(s32 soundIndex, s32 sceneIndex) {
    s32 work = btlGetRuntime();
    u32 v = 0;
    if (soundIndex != 0) {
        v = *(u16 *)(D_00435E0C + soundIndex * 0x190 + 4);
    }
    if (datBattleSceneRecords[sceneIndex].unk24 != 0) {
        v = datBattleSceneRecords[sceneIndex].unk24;
    } else if (*(u8 *)(work + 0x26E) == 3) {
        v = 1;
    }
    if (v == 0) {
        v = 5;
    }
    if (mnuPollTitleStreamStateLocked() != 0) {
        mnuResetTitleStreamLocked();
    }
    if (v == 5) {
        func_002A2200(4);
    } else {
        func_002A2200(v - 1);
    }
}

u8 sndIsStreamStatusTwoOrThree(void) {
    s32 status;

    status = mnuPollTitleStreamStateLocked();
    return status - 2U < 2;
}

void btlResetTitleStreamOnBattleFlag(void) {
    s32 work;

    work = btlGetRuntime();
    if ((((BtlState *)work)->battleFlags & 0x10000) != 0) {
        mnuResetTitleStreamAfterFileIdle();
        return;
    }
}

void btlAdvanceTitleState(void) {
    mnuAdvanceTitleStateUnderSemaphore();
}

void btlAdvanceTitleStateWithAudioCleanup(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    func_00341CD0();
    func_00341CA8();
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

s32 sndPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (sndLoadAndPlayStationedSe(soundId)) {
        btlBossDebugPrintf("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

BtlRuntimeTask *sndCreateStationedSeTask(u32 value) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x5A;
    task->callback = sndPlayStationedSe;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

/* Return whether the independent active-node list contains a node with flag 8. */
s32 sndHasFlaggedActiveNode(void) {
    s32 node = (s32)((BtlState *)btlGetRuntime())->soundList;
    while (node != 0) {
        if ((((ActiveSoundNode *)node)->flags & 8) != 0) {
            return 1;
        }
        node = (s32)((ActiveSoundNode *)node)->next;
    }
    return 0;
}

s32 sndPlaySkillSeTask(u32 *arg0) {
    u8 *task = (u8 *)arg0;
    BtlState *work = (BtlState *)btlGetRuntime();
    u32 value;

    if (*(u16 *)(task + 4) == 2 && work->unk28C < 6) {
        btlBossDebugPrintf("btl:skill SE ignore[frame:%d]\n", work->unk28C);
        return 1;
    }
    work->unk28C = 0;
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

BtlRuntimeTask *sndCreateSkillSeTask(s32 *taskArgs, u16 optionId) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x57;
    task->callback = sndPlaySkillSeTask;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    args->value = taskArgs[2];
    args->optionId = optionId;
    return task;
}

typedef struct SoundFileNode {
    u32 flags;
    u8 mode;
    u8 pad5[3];
    u32 position;
} SoundFileNode;

void sndStartFileLoad(SoundFileTaskArgs *args) {
    SoundFileNode *node = args->node;
    args->request = fileQueueDefaultCallbackRequest(args->filename);
    node->flags |= SOUND_FILE_STATE_REQUEST_PENDING;
    node->position = (args->frames + 0x200) << 16;
    node->mode = 2;
    btlBossDebugPrintf("btl:sound file load start[%s]\n", args->filename);
}

u32 sndPollMotSeFileAndSpu(SoundFileTaskArgs *request) {
    SoundFileNode *node = request->node;
    if (sndHasActiveFileLoad()) {
        btlBossDebugPrintf("btl:sound wait[motSE]\n");
        return 0;
    }
    if ((node->flags & SOUND_FILE_STATE_SOURCE_REQUEST_RESOLVED) == 0) {
        if (fileIsRequestReadyInCurrentMode(request->request)) {
            s32 size;
            s32 data;
            btlBossDebugPrintf("btl:sound file load end[%s]\n", request->filename);
            request->resourceAllocation = (struct SdfMemBlock *)fileGetResourceHandle(request->request);
            size = (s32)fileGetResourceSize(request->request);
            data = sdfResourceRetainAddress(request->resourceAllocation);
            if (sndFindPackedTrackLoadStatus(node->position) == 0) {
                func_003422F8(data, size);
                node->flags |= SOUND_FILE_STATE_SPU_LOAD_PENDING;
                btlBossDebugPrintf("btl:sound SPU load start[%X][size:%d]\n", (u16)(node->position >> 16), size);
            }
            node->flags = (node->flags & ~SOUND_FILE_STATE_REQUEST_PENDING) |
                          SOUND_FILE_STATE_SOURCE_REQUEST_RESOLVED;
        }
    } else if (sndFindPackedTrackLoadStatus(node->position) != 0) {
        btlBossDebugPrintf("btl:sound SPU load end[%X]\n", (u16)(node->position >> 16));
        sdfReleaseResourceAllocation(request->resourceAllocation);
        filePollEntryCleanup(request->request);
        node->flags = (node->flags & ~SOUND_FILE_STATE_SPU_LOAD_PENDING) |
                      SOUND_FILE_STATE_COMPLETE;
        return 1;
    }
    return 0;
}

BtlRuntimeTask *sndCreateFileLoadTask(SoundFileNode *node, s32 option, char *name) {
    BtlRuntimeTask *task = btlAllocTask(strlen(name) + sizeof(SoundFileTaskArgs));
    SoundFileTaskArgs *args;
    char *copy;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x58;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->onStart = sndStartFileLoad;
    task->callback = sndPollMotSeFileAndSpu;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    args = btlGetTaskArguments(task);
    copy = (char *)(args + 1);
    args->node = node;
    args->frames = option;
    args->filename = copy;
    strcpy(copy, name);
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
    task->taskId = 0x5B;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    return task;
}

s32 sndIsCommandBusySigned(void) {
    return (s8)sdfSoundIsCommandBusy();
}

s32 sndHasResourceFlagsOneOrEight(ActiveSoundNode *resource) {
    s32 flags;

    flags = resource->flags;
    if ((flags & 1) != 0) {
        return 1;
    }
    return (flags & 8) > 0;
}

void sndFormatResourceNameFromIndex(s32 source, char *output) {
    func_0035C860(output, D_004192D8, D_00436AE8, (u16)(source + 0x200));
}

void sndFormatResourceNameFromUnitMode(const BtlUnit *unit, char *output) {
    func_0035C860(output, D_004192E8, unit->partyRecord.unitId);
}

s32 sndResolveResourceId(s32 category, s32 id) {
    BtlState *work = (BtlState *)btlGetRuntime();
    s32 type = -1;
    if (work->unk688 != 0) {
        type = work->unk688(category, id);
    }
    if (type == -1) {
        type = sndLookupResourceType(category, id);
    }
    if (type < 26) {
        if (type == 0) {
            return -1;
        }
        if (type >= 11) {
            return type + work->sequenceHandle - 6;
        }
        return type + 0xFFFF;
    }
    return -1;
}

/* Allocate a cleared active-sound node and prepend it to its independent list. */
ActiveSoundNode *sndAllocListNode(void) {
    ActiveSoundNode *node = sdfAllocAndClearQuadwords(0x14);
    BtlState *work = (BtlState *)btlGetRuntime();
    node->previous = 0;
    if (work->soundList != 0) {
        work->soundList->previous = node;
        node->next = work->soundList;
    } else {
        node->next = 0;
    }
    work->soundList = node;
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
        ((BtlState *)btlGetRuntime())->soundList = node->next;
    }
    sdfReleaseChipBlock(node);
}

/* Clear active-sound nodes, saving next before each allocation is released. */
void sndClearList(void) {
    ActiveSoundNode *node;
    ActiveSoundNode *next;
    for (node = ((BtlState *)btlGetRuntime())->soundList; node != 0; node = next) {
        next = node->next;
        sndFreeListNode(node);
    }
}

typedef struct SoundSlotTableEntry {
    s16 resourceOffset;
    u16 fileId;
} SoundSlotTableEntry;

extern SoundSlotTableEntry *btlSelectSideIndexedActorParameterTable(s32, s32);

/* Return the category/id/slot's packed motion-SE key, or zero if unavailable. */
u32 sndBuildMotSeResourceKey(const SoundSlotOwner *sound, u32 slot) {
    u32 id = sound->id;
    u32 category = sound->category;
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
    resource = (scaledId + offset + 0x440) << 16;
    if (category != specialCategory) {
        return resource;
    }
    resource = 0;
    if (slot >= 23) {
        return resource;
    }
    return (id * 0x10 + offset + 0x1000) << 16;
}

extern char D_004192F8[];

extern char D_00419308[];

extern char D_00419318[];

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004192D8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004192E8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_004192F8);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419308);

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419318);

void sndLoadMotSeFiles(SoundSlotOwner *sound) {
    char filename[0x70];
    u32 slot = 0;
    s32 offset = 0x10;
    u8 *handleTable = (u8 *)sound + 8;
    do {
        u32 id = sndBuildMotSeResourceKey(sound, slot);
        if (id != 0) {
            if (slot != 0xB) {
                func_0035C860(filename, D_004192D8, D_00436AE8, id >> 16);
            } else if (sound->category == 0) {
                func_0035C860(filename, D_004192F8, D_00419308, sound->id);
            } else {
                func_0035C860(filename, D_00419318, D_00419308, sound->id);
            }
            *(struct FileRequest **)(handleTable + offset) = fileQueueDefaultCallbackRequest(filename);
            btlBossDebugPrintf("btl:motSE file load start[%d][%p][%s]\n", slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound->flags |= SOUND_SLOT_FILE_LOAD_PENDING;
}

/* Find the newest registered owner with both keys equal; return null if absent. */
SoundSlotOwner *sndFindListNodeForChannel(s32 category, s32 id) {
    SoundSlotOwner *node = ((BtlState *)btlGetRuntime())->soundSlotOwners;
    while (node != 0) {
        if (node->category == category && node->id == id) {
            return node;
        }
        node = node->next;
    }
    return 0;
}

/* Retain or register an owner; model flag 0xC0F suppresses initial file queuing. */
SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id) {
    SoundSlotOwner *owner = sndFindListNodeForChannel(category, id);
    BtlState *work;
    if (owner != 0) {
        btlBossDebugPrintf("btl:motSE search hit[%p]\n", owner);
        owner->work.refCount++;
        return owner;
    }
    owner = sdfAllocAndClearQuadwords(0x108);
    owner->category = category;
    owner->id = id;
    owner->work.refCount = 1;
    work = (BtlState *)btlGetRuntime();
    owner->prev = 0;
    if (work->soundSlotOwners != 0) {
        work->soundSlotOwners->prev = owner;
        owner->next = work->soundSlotOwners;
    } else {
        owner->next = 0;
    }
    work->soundSlotOwners = owner;
    if (mdlFlagTest(0xC0F) == 0) {
        sndLoadMotSeFiles(owner);
    }
    return owner;
}

/* The last reference cleans queued files and resource handles, then unlinks/frees. */
void sndReleaseSlotOwner(SoundSlotOwner *owner) {
    u32 i;
    if (--owner->work.refCount == 0) {
        for (i = 0; i < 0x1D; i++) {
            if (owner->work.fileRequests[i] != 0) {
                filePollEntryCleanup(owner->work.fileRequests[i]);
            }
            if (owner->work.resourceHandles[i] != 0) {
                sdfReleaseResourceAllocation(owner->work.resourceHandles[i]);
            }
        }
        if (owner->next != 0) {
            owner->next->prev = owner->prev;
        }
        if (owner->prev != 0) {
            owner->prev->next = owner->next;
        } else {
            ((BtlState *)btlGetRuntime())->soundSlotOwners = owner->next;
        }
        sdfReleaseChipBlock(owner);
    }
}

/* Release once per owner; preserve the next link before a final release may free. */
void sndReleaseAllSlotOwners(void) {
    SoundSlotOwner *owner;
    SoundSlotOwner *next;
    for (owner = ((BtlState *)btlGetRuntime())->soundSlotOwners; owner != 0; owner = next) {
        next = owner->next;
        sndReleaseSlotOwner(owner);
    }
}

void btlStartMoveOtherUnitsTask(void) {
    u64 task;

    task = btlCreateHookedUnitSoundTask();
    btlStartTask(task);
}

/* Flag 8 denotes packed-track loading, distinct from queued motion-SE files. */
s32 sndHasActiveFileLoad(void) {
    SoundSlotOwner *node = ((BtlState *)btlGetRuntime())->soundSlotOwners;
    while (node != 0) {
        if ((node->flags & SOUND_SLOT_TRACK_LOADING) != 0) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

/* Queue a ready owner's packed track; stream slot 11 only supplies its file ID. */
void btlQueueUnitSoundSlotFileLoad(SoundTaskArgs *args) {
    SoundSlotOwner *owner = (SoundSlotOwner *)((BtlUnit *)args->actor)->unk328;
    SoundSlotTableEntry *table;
    SoundSlotTableEntry *entry;
    if (owner == 0) {
        return;
    }
    if (owner->flags & SOUND_SLOT_FILE_LOAD_PENDING) {
        return;
    }
    if (!(owner->flags & SOUND_SLOT_FILES_READY)) {
        return;
    }
    if (owner->work.resourceHandles[args->unk_08] == 0) {
        return;
    }
    table = btlSelectSideIndexedActorParameterTable(owner->category, owner->id);
    entry = &table[args->unk_08];
    args->option = entry->fileId;
    if (args->unk_08 != 0xB) {
        owner->work.pendingSoundId = sndBuildMotSeResourceKey(owner, args->unk_08);
        owner->work.pendingSlot = args->unk_08;
        owner->flags |= SOUND_SLOT_TRACK_LOAD_REQUESTED;
        owner->flags &= ~SOUND_SLOT_TRACK_LOADING;
        owner->flags &= ~SOUND_SLOT_TRACK_READY;
    }
}

extern void mnuResetSoundBufferLocked(void);

extern void mnuClearInactiveSoundBufferState(void);

u32 sndPollMotionSePlayback(SoundTaskArgs *args) {
    BtlState *work = (BtlState *)btlGetRuntime();
    SoundSlotOwner *owner = (SoundSlotOwner *)((BtlUnit *)args->actor)->unk328;
    SoundSlotWork *soundWork;
    u32 key;
    u32 data;
    u32 size;

    if (owner == 0) {
        return 1;
    }
    if (owner->flags & SOUND_SLOT_FILE_LOAD_PENDING) {
        return 1;
    }
    if (!(owner->flags & SOUND_SLOT_FILES_READY)) {
        return 1;
    }
    soundWork = &owner->work;
    if (soundWork->resourceHandles[args->unk_08] == 0) {
        return 1;
    }
    if (args->unk_08 != 0xB) {
        key = sndBuildMotSeResourceKey(owner, args->unk_08);
        if (owner->flags & SOUND_SLOT_TRACK_READY) {
            sndSetStationedSeHighVolume(key);
            btlBossDebugPrintf("btl:motSE play[%X-%X]\n", key >> 16, key & 0xFFFF);
            return 1;
        }
        key >>= 16;
        btlBossDebugPrintf("btl:motSE load wait[%X]\n", key);
    } else {
        if (work->unk288 >= 0xB) {
            if (mnuGetSoundBufferStateLocked() != 0) {
                mnuResetSoundBufferLocked();
                mnuReleaseSoundBufferLocked();
            }
            data = (void *)sdfMemoryGetBlockAddress(soundWork->resourceHandles[args->unk_08]);
            size = sdfMemoryGetBlockSize(soundWork->resourceHandles[args->unk_08]);
            func_002A27A8(data, size, 2);
            mnuClearInactiveSoundBufferState();
            work->unk288 = 0;
            btlBossDebugPrintf("btl:motSE play(ATRAC3)\n");
        } else {
            btlBossDebugPrintf("btl:motSE ignore(ATRAC3)[frame:%d]\n", work->unk288);
        }
        return 1;
    }
    if ((s32)args->unk_0C > 90) {
        owner->flags &= ~SOUND_SLOT_TRACK_LOADING;
        owner->flags |= SOUND_SLOT_TRACK_READY;
        btlBossDebugPrintf("btl:motSE load time out[%X]\n", key);
        return 1;
    }
    args->unk_0C++;
    return 0;
}

struct BtlRuntimeTask *btlCreateHookedUnitSoundTask(unit, option)
    BtlUnit *unit;
    s32 option;
{
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    BtlState *work;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x59;
    task->ownerId = unit->owner;
    task->onStart = btlQueueUnitSoundSlotFileLoad;
    task->callback = sndPollMotionSePlayback;
    work = (BtlState *)btlGetRuntime();
    if (work->unk710 != 0) {
        option = work->unk710(unit, option);
    }
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->unk_08 = option;
    args->option = 0;
    args->unk_0C = 0;
    return task;
}

void func_002050D0(void) {
    s32 context = btlGetRuntime();
    ((BtlState *)context)->unk288 = -1;
    ((BtlState *)context)->unk28C = -1;
}

extern char D_00419408[]; /* "btl:sound load BSE SMG\n" */

void sndLoadBattleBank(void) {
    if (sndFindPackedTrackLoadStatus(0x10000) == 0) {
        sndEnsureMidiBankResident(0x10000);
        btlBossDebugPrintf(D_00419408);
    }
}

u8 sndIsBattleBankLoaded(void) {
    s64 status;

    status = sndFindPackedTrackLoadStatus(0x10000);
    return status != 0;
}

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419408);

void btlUpdateMotionSoundLoading(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    SoundSlotOwner *owner;
    s32 tracksReady;

    for (owner = state->soundSlotOwners; owner != NULL; owner = owner->next) {
        if ((owner->flags & SOUND_SLOT_FILES_READY) == 0) {
            u32 slot;
            s32 requestsPending = 0;

            for (slot = 0; slot < 0x1D; slot++) {
                if (owner->work.fileRequests[slot] != 0) {
                    if (fileIsRequestReadyInCurrentMode(
                            owner->work.fileRequests[slot]) != 0) {
                        u32 resource = fileGetResourceHandle(
                            owner->work.fileRequests[slot]);

                        struct FileRequest *completedRequest = owner->work.fileRequests[slot];

                        owner->work.resourceHandles[slot] = (struct SdfMemBlock *)resource;
                        filePollEntryCleanup(completedRequest);
                        owner->work.fileRequests[slot] = 0;
                    } else {
                        requestsPending = 1;
                    }
                }
            }
            if (requestsPending == 0) {
                owner->flags = (owner->flags & ~SOUND_SLOT_FILE_LOAD_PENDING) | SOUND_SLOT_FILES_READY;
                btlBossDebugPrintf("btl:motSE file load all end[%p]\n", owner);
            }
        }
    }

    if ((s32)state->unk288 >= 0) {
        state->unk288++;
    }
    if ((s32)state->unk28C >= 0) {
        state->unk28C++;
    }

    tracksReady = 1;
    for (owner = state->soundSlotOwners; owner != NULL; owner = owner->next) {
        if (owner->flags & SOUND_SLOT_TRACK_LOADING) {
            u32 loaded = sndFindPackedTrackLoadStatus((u32)owner->work.pendingSoundId);

            if (loaded != 0) {
                u32 flags = owner->flags;

                if (flags & SOUND_SLOT_TRACK_LOADING) {
                    tracksReady = 0;
                }
                owner->flags = (flags & ~SOUND_SLOT_TRACK_LOADING) | SOUND_SLOT_TRACK_READY;
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
                if (owner->flags & SOUND_SLOT_TRACK_LOAD_REQUESTED) {
                    SoundSlotWork *work = &owner->work;
                    struct SdfMemBlock *block = work->resourceHandles[owner->work.pendingSlot];
                    s32 size = sdfMemoryGetBlockSize(block);
                    u32 address = sdfMemoryGetBlockAddress(
                        work->resourceHandles[owner->work.pendingSlot]);

                    func_003422F8((s32)address, size);
                    owner->flags = (owner->flags & ~SOUND_SLOT_TRACK_LOAD_REQUESTED) | SOUND_SLOT_TRACK_LOADING;
                    owner->flags &= ~SOUND_SLOT_TRACK_READY;
                    break;
                }
            }
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001EB5B0", D_00419468);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", D_00436AB0);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", D_00436AB8);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", D_00436AC0);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", D_00436AD0);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", D_00436AD4);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", btlTintTransitionHoldCount);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", D_00436AE0);

INCLUDE_SDATA(const s32, "game/code_001EB5B0", D_00436AE8);

