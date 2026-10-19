#include "btl_vec3.h"
#include "btl_motion_transform.h"
#include "btl_camera_pose_api.h"
#include "btl_camera_cursor.h"
#include "btl_camera_path_preset.h"
#include "btl_pair_camera_preset.h"
#include "btl_camera_parameter_record.h"
#include "common.h"
#include "btl_action_camera_preset.h"
#include "btl_actor_status_table_api.h"
#include "btl_rain_effect_api.h"
#include "btl_blend_color_api.h"
#include "kwln_fade.h"
#include "mnu_title_stream.h"
#include "eff_random_mod.h"
#include "btl_tint_transition.h"
#include "eff_random.h"
#include "btl_camera_timed_instruction.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "fld_area_path.h"
#include "btl_scene_light_params.h"
#include "btl_scene_lighting_task_args.h"
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
#include "btl_actor_work.h"
#include "sdf_texture_offset_list.h"
#include "btl_state.h"
#include "btl_model_record.h"
#include "btl_task_args.h"
#include "btl_field_load_task_args.h"
#include "file_pac.h"
#include "btl_floor_load_task_args.h"
#include "btl_sound.h"
#include "btl_sound_release_task.h"
#include "actor_effect_task.h"
#include "eff_field_color.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "mdl.h"
#include "mdl_asset_request.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "sdf.h"
#include "file.h"
#include "dat_command.h"
#include "file_request_api.h"
#include "mdl_flag_api.h"
#include "dds3_world_counter.h"
#include "sdf_vu_matrix.h"
#include "kwln_fade_timing.h"
/* Declared before first use (were implicit int). */
extern s32 btlBuildEligibleTargetIndexList(void *actionArg, BtlIndexList *targets, s32 area, s32 rule, s32 excludedStatus);
extern s32 btlGetSelectedUnitProperty(s32 unitAddress);
extern s32 btlGetUnitMotionDurationFrames(BtlUnit *unit, s32 motionIndex);
extern void effBTLFieldColorResetFlags(void);
extern BattleEffect * effCreateBattleVoiceOwner(SoundMixer *mixer, u16 kind, void *owner, s32 value);
extern void effSweepFloorModelList(void);
extern void effTickSlotVolumeFade(void);
extern void evtDestroySecondaryWorldNode(void);
extern void evtDrainSecondaryWorldNodes(void);
extern void fileResetRenderFlags(void);
extern void fldDrawSkyGradient(void);
extern u32 fldGetSkyDrawState(void);
extern void fldSetSkyDrawState(u32 drawState);
extern void fldUpdateSelectedValueTransition(void);
extern void fldUpdateSwayOffset(void);
extern void mdlClearListedObjectFlag(void);
extern void mdlMarkAndProcessObjectNodes(void);
extern void mdlSetListedObjectFlag(void);
extern void * memset(void *, s32, u32);
extern void mnuAdvanceTitleStateUnderSemaphore(void);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern u32 sdfSoundIsCommandBusy(void);
extern s32 sdfSoundSendNamedCommand(const char *name, u8 channel);
extern void sndEnsureMidiBankResident(s32 packed);
extern s32 sndGetMixerBankCallbackFrame(SoundMixer *mixer, u16 kind);
extern s32 sndHasActiveFileLoad(void);
extern s32 sndIsCommandBusySigned(void);
extern char * strcpy(char *, const char *);
extern u32 strlen(const char *s);
/* Original-style implicit int, kept: matched code depends on it: effDrawSolidRectangle. */

extern void sndQueueNoPayloadCommand1A0(void);
extern void sndQueueNoPayloadCommand210(void);
extern f32 D_00359EE0[4];

extern s32 sprintf(char *dst, const char *format, ...);

extern void effObjSetOpacityPassEnabled(u32 enabled);

extern void btlBossDebugPrintf(const char *format, ...);


extern u64 btlStartTask(void *);

extern BtlCameraCursorView D_0035F100;

extern const BtlCameraTimedInstruction *D_0035D9F0[];

extern const BtlCameraTimedInstruction *D_0035DA08[];

extern void func_001E9DE0(BtlLinkedCommand *, BtlCamState *, const BtlCameraTimedInstruction *);

extern s32 func_001EB368(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceSkillCameraCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlInitializeRandomCameraCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceCommandCursor(BtlLinkedCommand *, BtlCamState *);

extern void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *, BtlCamState *);

extern void btlInitLinkedUnitActionCursor(BtlTask *);

extern s32 btlHasSingleLinkedResource(BtlLinkedCommand *);

extern BattleSoundArchiveEntry D_0035F748[BTL_SOUND_ENTRY_COUNT];

/* SYSEFF metadata and runtime registrations share these indices. */
enum { BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT = 0x2C };

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
extern char D_003A5158[]; /* "%sMIDI%04X.SMG" */

extern s32 sndFindPackedTrackLoadStatus(u32);

extern s64 btlForceSceneLightingRestore(SceneLightRestoreArgs *);

extern s32 btlRestoreSceneTransformLighting(SceneLightRestoreArgs *);

extern s64 btlForceReleaseTintTransition(const BtlTintReleaseArgs *args);

extern s32 sndPlaySkillSeTask(u32 *);

extern u32 sndFinishEarringPlayback(void);

extern void *btlCreateMoveOtherUnitsTask(u8 *, u32);


extern SoundResourceNode *sndAllocResourceNode(void);

extern u32 kwlnDrawControlFlags;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 D_003BB694;

extern u32 btlTintTransitionHoldCount;

extern u8 D_0035F5D0[];

extern char D_003BB6B0[];


extern struct EffWorldNode *evtSpawnActionObj9(s32);

extern s32 btlSetActorEffectParameter();


extern void btlSetDefaultActionCameraPose(BtlLinkedCommand *, BtlCamState *);

extern void btlPrepareUnitPoseWithTiltRotation(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlResetUnitEffectVector(BtlLinkedCommand *, BtlCamState *);

extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);
extern u8 *datActionAnimationRecords;

extern s32 btlArrangeFormationSlots(s32 filter);

extern EffRandState effSharedRandomState[];

extern void sndResetTransition(void);

extern void btlResetMotionSoundFrameCounters(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

extern EffRandState effSharedRandomState[];

extern void sndSetStationedSeVolume(u32 sequence);

extern void btlResetTitleStreamOnBattleFlag(void);

extern void btlAdvanceWorldCounterAndSpawnActionObject(void);


extern s32 btlRepositionPartyAroundBattleCenter(void);


extern s32 btlAreWorkBuffersReady(void);


s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *entry);

struct EvtUnit;

extern s32 btlIsCurrentValueBelowQuarterThreshold(void *);

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

s32 btlGetLoggedIndexedCommandItem(s32 index);


extern s32 btlConstrainCameraEndpointHeight(BtlLinkedCommand *, BtlCamState *, s8, s8);

typedef struct BattleVoiceEntry {
    u8 volume;
    u8 pad01[3];
    char fileName[0xC];
} BattleVoiceEntry;

#define CURSOR (&D_0035F100)

extern s32 btlRepositionPartyAroundBattleCenter(void);
extern void *btlAllocTask(s32);

extern void *btlAllocTask(s32);

extern struct SoundSlotOwner *sndAcquireSlotOwner(s32, s32);

extern void btlSetUnitPosition(BtlUnit *object, void *position);

extern void btlSetUnitRotation(BtlUnit *object, void *rotation);

extern s32 sdfLoadMapRecordPositionVector(SdfModel *, s32);

extern void mdlLoadPrimaryVectorVU(MdlCtx *);

extern void btlUnitGetBodyPosVU(u8 *);

extern s32 btlAimHorizontalDirectionVU(f32 *, f32 *);

extern s32 btlAimHorizontalDirectionClampedVU(void *, void *, f32);

extern u8 D_0037E110[];



extern void effBattleUpdate(BattleEffect *);

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];


extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];
extern f32 btlUnitGetTopY(BtlUnit *);

/* With a qualifying unit, the span query leaves its last muzzle point in VF10. */
extern f32 btlMeasureMaskedUnitBounds(s32, f32 *, f32 *);

extern void btlPrepareActionCameraPoseWithActorClearance(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlUpdateTiltedActionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlFrameLeftmostUnitCameraPair(BtlCamState *, BtlCamState *, BtlCamState *);

extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, BtlCamState *);

extern void btlInitializeCameraCursorForActorMode(BtlLinkedCommand *, BtlCamState *);

extern void btlFrameCommandTargetGroups(BtlLinkedCommand *);

extern void btlSetupActionCameraPair(BtlLinkedCommand *);

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);

extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);

extern void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *, BtlCamState *);

extern void btlRefreshActionPoseBlendSnapshot();

extern void btlAimEffectPoseAtUnit();

extern void btlBeginActionEndCameraPullback();

extern void btlUpdateApproachCameraPose(BtlLinkedCommand *, BtlCamState *);

extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

/* Test whether the operand is empty, subject to command-category and slot-kind exclusions. */ s32 btlActionEntryIsEmpty(s32 index, BtlOperandGroup *slot, BtlOperandEntry *entry);

s32 btlAdjustCameraDirectionForDefaultPlane(BtlCamState *state);

/* Adjust the pose direction using the supplied horizontal height plane. */ s32 btlAdjustCameraDirectionForPlane(BtlCamState *state, f32 height);

/* Allocate a native header followed by capacity pointer entries. */ BtlIndexList *btlAllocateIndexList(s32 capacity);

/* The caller keeps the live count within the allocated capacity. */ void btlAppendIndexListEntry(BtlIndexList *list, void *entry);

void btlApplyCombinedActorFlags(u8 *resource);

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode);

/* Check actor status, linked-group marks, and owner restrictions before use. */ s32 btlCanUseLinkedActor(BtlLinkedCommand *actor);

void btlClearAllUnitDefeatCandidatesTask(void);

void btlClearUnitDefeatCandidate(BtlUnit *object);


void btlCopyUnitRotationQuaternion(BtlUnit *unit, void *dst);

/* Return the oldest matching handle, or zero; unstarted tasks may have handle 0. */ s32 btlFindTaskByHandle(s64 handle);

void btlFlagLinkedGroupDefeatCandidatesTask(BtlLinkedCommand *command);

void btlFlagUnitDefeatCandidate(BtlUnit *unit);

void btlFreeIndexList(BtlIndexList *list);

u32 btlGetActiveUnitId(void);



/* Return the argument address recorded by allocation (zero for no arguments). */ void *btlGetTaskArguments(void *task);

s32 btlGetUnitModelFrameCount(BtlUnit *unit);

f32 btlGetUnitModelCurrentFrame(BtlUnit *unit);

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

void btlCopyUnitCurrentPosition(u8 *object, void *position);

/* vu0 routine: preserve the actor's primary position and rotation quaternion while * evaluating the requested model record; return the sampled vector in vf10. */ s32 btlSampleActorModelPositionVU(BtlUnit *unit, s32 value);

extern void btlApplyMarkedUnitActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

extern void btlSetupCameraAimDualUnit(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);

extern s32 btlRunTimedCameraSetupHook(s32 actor);
extern s32 btlRunTimedCameraFinishHook(s32 actor);

extern f32 fabsf(f32);
extern f32 btlUnitGetBottomY(BtlUnit *);
extern void btlFlagAllUnitsDefeatCandidate(void);

extern f32 tanf(f32);
extern void sdfVuLoadRotationMatrixZ(f32);

void btlBuildBattleUnitFramingCameraPose(BtlLinkedCommand *action, BtlCamState *out, f32 tiltDegrees) {
    BtlState *state;
    BtlUnit *unit;
    u32 count;
    s32 firstEnemy;
    u32 flags;
    f32 fov;
    f32 maxAllyTop;
    f32 minEnemyTop;
    f32 measure;
    f32 reachTotal;
    f32 reach;
    f32 sample;
    f32 horizontalSpan;
    f32 cameraDistance;
    f32 centers[3][4] __attribute__((aligned(16)));

    state = (BtlState *)btlGetRuntime();
    btlFlagAllUnitsDefeatCandidate();

    count = 0;
    maxAllyTop = 0.0f;
    measure = 0.0f;
    fov = action->camera.fov;
    minEnemyTop = 0.0f;
    reachTotal = 0.0f;
    unit = state->units;
    firstEnemy = 1;
    out->fov = fov;
    while (unit != NULL) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (!(unit->status.flags & 0xC0)) {
                reach = unit->reach * unit->scale;
                sample = btlUnitGetTopY(unit);
                flags = unit->status.flags;
                if ((flags & 0x200) != 0) {
                    if (maxAllyTop < sample) {
                        maxAllyTop = sample;
                    }
                } else if ((flags & 0x400) != 0) {
                    if (firstEnemy || sample < minEnemyTop) {
                        minEnemyTop = sample;
                        measure = btlUnitGetBottomY(unit);
                        firstEnemy = 0;
                    }
                }
                reachTotal += reach;
                count++;
            }
        }
        unit = unit->next;
    }

    if (count == 0) {
        return;
    }

    if (minEnemyTop > 600.0f) {
        minEnemyTop = (minEnemyTop - measure) * 0x1.666666p-2f + measure;
    } else if (minEnemyTop > 150.0f) {
        minEnemyTop = (minEnemyTop - measure) * 0.5f + measure;
    } else {
        minEnemyTop = (minEnemyTop - measure) * 0.25f + measure;
    }

    measure = reachTotal / (f32)count;
    btlMeasureMaskedUnitBounds(0x200, NULL, NULL);
    VU0_STORE_VF(vf10, centers[1]);
    btlMeasureMaskedUnitBounds(0x400, NULL, NULL);
    VU0_STORE_VF(vf10, centers[2]);
    centers[2][0] = centers[1][0];
    centers[2][1] = -minEnemyTop;
    centers[1][1] = -maxAllyTop;
    horizontalSpan = btlMeasureMaskedUnitBounds(0x600, NULL, NULL);
    cameraDistance = horizontalSpan;
    if (measure > 100.0f) {
        cameraDistance += measure;
    } else {
        cameraDistance += 100.0f;
    }
    VU0_STORE_VF(vf10, centers[0]);
    if (cameraDistance < 1200.0f) {
        cameraDistance += ffabsf(centers[0][0] - centers[1][0]);
    }

    sample = maxAllyTop - minEnemyTop;
    {
        f32 separation = ffabsf(centers[2][2] - centers[1][2]);
        f32 offset = ffabsf(centers[2][2] - centers[0][2]);
        centers[0][0] = centers[1][0];
        if (separation > 0.0f) {
            centers[0][1] = -(sample * offset / separation) - minEnemyTop;
        }
    }
    VU0_LOAD_VF(vf10, centers[0]);
    VU0_STORE_VF(vf10, out->position);

    if (action->camera.direction[0] > 0.0f) {
        sdfVuLoadRotationMatrixZ(tiltDegrees * 0x1.1df46ap-6f);
    } else {
        sdfVuLoadRotationMatrixZ(-tiltDegrees * 0x1.1df46ap-6f);
    }
    VU0_LOAD_VF(vf10, centers[2]);
    VU0_LOAD_VF(vf11, centers[1]);
    __asm__ volatile("vsub.xyzw vf10, vf10, vf11");
    VU0_NORMALIZE_VF10();
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);

    out->distance = cameraDistance / tanf(fov * 0x1.555554p-1f);
    btlAdjustCameraDirectionForDefaultPlane(out);
}

void btlSetDefaultActionCameraPose(BtlLinkedCommand *command, BtlCamState *pose) {
    btlBuildBattleUnitFramingCameraPose(command, pose, 27.5f);
}

extern void sdfVuLoadRotationMatrixZ(f32 angle);

void btlPrepareUnitPoseWithTiltRotation(command, pose, out)
BtlLinkedCommand *command;
BtlCamState *pose;
BtlCamState *out;
{
    btlBuildBattleUnitFramingCameraPose(command, pose, 20.0f);
    btlCopyMotionTransform(out, pose);
    if (pose->direction[0] > 0.0f) {
        sdfVuLoadRotationMatrixZ(-(20.0f * 0.017453293f));
    } else {
        sdfVuLoadRotationMatrixZ(20.0f * 0.017453293f);
    }
    VU0_STORE_VF(vf10, pose->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
}

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

extern f32 tanf(f32);

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
            if (btlSampleActorModelPositionVU(unit, 1) == 0) {
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
        tangent = tanf(halfFov);
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
            action->durationFrames = btlGetUnitMotionDurationFrames(unit, unit->motionId);
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
        span = btlMeasureMaskedUnitBounds(flags & 0x600, 0, 0) * 1.25f;
        VU0_STORE_VF(vf10, from->position);
        if (btlSampleActorModelPositionVU(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF(vf10, to->position);
        VU0_LOAD_VF(vf11, from->position);
        VU0_LERP_VF10(0.5f);
        VU0_STORE_VF(vf10, from->position);
        half = fov * 0.5f;
        distance = span / tanf(half);
        from->distance = distance;
        distance = unit->cameraRadius * unit->scale / tanf(half);
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

extern void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *, BtlCamState *, u8);

extern void btlBuildTargetGroupFramingCameraPose(BtlLinkedCommand *, BtlCamState *);

extern s32 btlGetUnitMotionDurationFrames(BtlUnit *, s32);

/* Build the from pose, then aim the to pose at the unit's muzzle using the pose row
 * for the side of the from view the muzzle lies on. */
void btlBuildSideSelectedActorCameraBlend(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from) {
    f32 muzzle[4];
    f32 quat[4];
    BattleSideCameraPreset poses[2] = {
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
            btlBuildTargetGroupFramingCameraPose(action, from);
        } else {
            btlBuildActorPairFramingCameraPose(action, from, 1);
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
        if (btlSampleActorModelPositionVU(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF(vf10, to->position);
        to->position[1] *= poses[pose].heightScale;
        distance = unit->cameraRadius * unit->scale / tanf(fov * 0.5f);
        to->distance = distance * poses[pose].distanceScale;
        VU0_LOAD_VF(vf10, poses[pose].quat);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->direction);
        btlAdjustCameraDirectionForDefaultPlane(to);
        action->durationFrames = btlGetUnitMotionDurationFrames(unit, unit->motionId);
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

/* Build action camera endpoints with a FOV-dependent minimum distance. */
void btlBuildDistanceClampedActionCameraBlend(BtlLinkedCommand *action, BtlCamState *to, BtlCamState *from) {
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
            btlBuildBattleUnitFramingCameraPose(action, from, 17.5f);
            btlCopyMotionTransform(to, from);
            btlInterpolateVectorStep(from->position);
            VU0_STORE_VF(vf10, targetPosition);
            if (btlSampleActorModelPositionVU(unit, 1) == 0) {
                btlUnitGetMuzzlePosVU(unit);
            }
            VU0_STORE_VF(vf10, to->position);
            VU0_LOAD_VF(vf11, targetPosition);
            VU0_SUB(vf10, vf10, vf11);
            VU0_SET_VF10_COMPONENT(x, 0.0f);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, to->direction);
            halfFov = to->fov * 0.5f;
            minimumDistance = unit->cameraRadius * unit->scale / tanf(halfFov);
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
                btlMeasureMaskedUnitBounds(targetFlags, 0, 0);
                VU0_STORE_VF_UNCLOBBERED(vf10, center);
                if (btlAimHorizontalDirectionVU(bodyPosition, center) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
                    btlSetUnitRotation(unit, rotation);
                }
            }
        } else {
            target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
            btlBuildActorPairFramingCameraPose(action, from, 2);
            halfFov = to->fov * 0.5f * 1.3333333f;
            btlCopyMotionTransform(to, from);
            minimumDistance = target->reach * target->scale / tanf(halfFov);
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

/* Start eligible linked-slot pullback at the exact scaled motion-end frame. */
void btlBeginActionEndCameraPullback(BtlLinkedCommand *action, BtlCamState *pose) {
    BtlUnit *user;
    BtlUnit *target;
    s32 frames;
    s32 idle;
    s32 eligible;

    if (action->motionProgress == 0 && (s32)btlGetIndexListCount(action->targetList) < 2) {
        user = action->task->unit;
        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
        frames = btlGetUnitMotionDurationFrames(user, user->motionId);
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
    if (btlSampleActorModelPositionVU(unit, 1) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_STORE_VF(vf10, &from->position);
    from->distance = unit->cameraRadius * unit->scale / tanf(fov * 0.5f);
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

extern const BtlActionCameraPreset D_003A4000[8];

/* Frame the first listed target from an action-filtered pair preset: both
 * cameras start at its muzzle, lowered by a quarter of its height, at a
 * distance that fits the larger of its reach (at least 135, wider view) and
 * half its height. */
void btlBuildRandomTargetCameraStates(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quaternion[4];
    f32 position[4];
    BtlActionCameraPreset presets[8];
    u16 candidates[8];
    BtlUnit *unit;
    f32 reach;
    f32 height;
    f32 fov;
    f32 distance;
    u32 i;
    u32 selected;
    u32 count;
    u32 category;

    memcpy(presets, D_003A4000, sizeof(presets));
    unit = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
    reach = unit->reach * unit->scale;
    height = unit->height * unit->scale;
    if (reach < 135.0f) {
        reach = 135.0f;
    }
    i = (u32)action->actionCode;
    if (i - 1 < 0x1FF) {
        category = ((BtlActionAnimationRecord *)datActionAnimationRecords)[i].flags;
        category &= 0x30;
        count = 0;
        for (i = 0; i < 8; i++) {
            if (category == 0 || (presets[i].actionMask & category) != 0) {
                candidates[count++] = i;
            }
        }
        selected = candidates[effMiscRandMod(0, count) & 0xFFFF];
    } else {
        selected = effMiscRandMod(0, 8);
    }
    btlCopyUnitRotationQuaternion(unit, quaternion);
    fov = action->camera.fov;
    from->fov = fov;
    to->fov = fov;
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF_UNCLOBBERED(vf10, position);
    position[1] -= height * 0.25f;
    VU0_LOAD_VF(vf10, position);
    VU0_STORE_VF(vf10, from->position);
    VU0_STORE_VF(vf10, to->position);
    fov *= 0.5f;
    height *= 0.5f;
    from->position[1] *= presets[selected].frontCameraYScale;
    if (height < reach) {
        distance = reach / tanf(fov * 0x1.555554p+0f);
    } else {
        distance = height / tanf(fov);
    }
    from->distance = distance * presets[selected].frontDistanceScale;
    to->distance = distance * presets[selected].backDistanceScale;

    VU0_LOAD_VF(vf10, presets[selected].frontQuaternion);
    VU0_LOAD_VF(vf11, quaternion);
    effMiscQuatMultiplyVU();
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, from->direction);

    VU0_LOAD_VF(vf10, presets[selected].backQuaternion);
    VU0_LOAD_VF(vf11, quaternion);
    effMiscQuatMultiplyVU();
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);

    btlAdjustCameraDirectionForDefaultPlane(from);
    btlAdjustCameraDirectionForDefaultPlane(to);
}

extern const BtlActionCameraPreset D_003A4190[8];
extern u8 D_003A4180[];

/* Frame all selected actor groups using an action-filtered camera preset. */
void btlBuildFilteredGroupCameraPair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 muzzle[4];
    BtlActionCameraPreset presets[8];
    u16 candidates[8];
    BtlState *work;
    BtlUnit *unit;
    f32 extent;
    f32 muzzleHeight;
    f32 fov;
    f32 halfFov;
    f32 minimum;
    f32 distance;
    f32 parameter;
    u32 mask;
    u32 i;
    u32 selector;
    u32 selected;
    u32 count;
    u32 rowFlags;
    u32 category;

    memcpy(presets, D_003A4190, sizeof(presets));
    work = (BtlState *)btlGetRuntime();
    minimum = 0.0f;
    selector = (u32)action->actionCode;
    if (selector - 1 < 0x1FF) {
        rowFlags = ((BtlActionAnimationRecord *)datActionAnimationRecords)[selector].flags;
        if (rowFlags & 0x80) {
            minimum = 500.0f;
        }
        category = rowFlags & 0x30;
        count = 0;
        for (i = 0; i < 8; i++) {
            if (category == 0 || (presets[i].actionMask & category) != 0) {
                candidates[count++] = i;
            }
        }
        selected = candidates[effMiscRandMod(0, count) & 0xFFFF];
    } else {
        selected = effMiscRandMod(0, 8);
    }
    /* The candidate count is finished; count now describes targets. */
    mask = 0;
    count = btlGetIndexListCount(action->targetList);
    for (i = 0; i < count; i++) {
        unit = btlGetIndexListEntry(action->targetList, i);
        mask |= unit->status.flags & 0x600;
    }
    muzzleHeight = 0.0f;
    /* Reuse the finished target count for eligible actors and their mean Y. */
    count = 0;
    for (unit = work->units; unit != NULL; unit = unit->next) {
        u32 flags = unit->status.flags;
        if (!(flags & 1)) {
            continue;
        }
        if (flags & 0xC0) {
            continue;
        }
        if (!(flags & mask)) {
            continue;
        }
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF(vf10, muzzle);
        muzzleHeight += muzzle[1];
        count++;
    }
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(mask);
    fov = action->camera.fov;
    from->fov = fov;
    to->fov = fov;
    extent = btlMeasureMaskedUnitBounds(mask, NULL, NULL);
    if (extent < minimum) {
        extent = minimum;
    }
    VU0_STORE_VF(vf10, from->position);
    VU0_STORE_VF(vf10, to->position);
    muzzleHeight = muzzleHeight / (f32)count;
    from->position[1] = muzzleHeight;
    to->position[1] = muzzleHeight;
    if (count >= 2) {
        halfFov = fov * 0x1.555554p+0f * 0.5f;
        distance = extent / tanf(halfFov);
    } else {
        halfFov = fov * 0.5f;
        distance = extent / tanf(halfFov);
    }
    from->distance = distance * presets[selected].frontDistanceScale;
    to->distance = distance * presets[selected].backDistanceScale;
    VU0_LOAD_VF(vf10, presets[selected].frontQuaternion);
    if (mask & 0x400) {
        VU0_LOAD_VF(vf11, D_003A4180);
        effMiscQuatMultiplyVU();
    }
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, from->direction);
    VU0_LOAD_VF(vf10, presets[selected].backQuaternion);
    if (mask & 0x400) {
        VU0_LOAD_VF(vf11, D_003A4180);
        effMiscQuatMultiplyVU();
    }
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);
    btlAdjustCameraDirectionForDefaultPlane(from);
    btlAdjustCameraDirectionForDefaultPlane(to);
    parameter = presets[selected].frontCameraYScale;
    action->flags |= 0x41;
    action->motionParameter = parameter;
}

extern void btlSetupCameraAimDualUnit(BtlLinkedCommand *, BtlCamState *, s32, f32, f32);
extern void btlClearAllUnitDefeatCandidates(void);
extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlBuildGroupSizeActionCameraPair(BtlLinkedCommand *command, BtlCamState *front, BtlCamState *back) {
    f32 position[4] __attribute__((aligned(16)));
    BtlState *runtime;
    BtlTask *task;
    BtlUnit *unit;
    BtlUnit *user;
    u32 groups = 0;
    u32 i;
    u32 count;
    u8 mode;
    f32 blend;

    runtime = (BtlState *)btlGetRuntime();
    count = btlGetIndexListCount(command->targetList);
    for (i = 0; i < count; i++) {
        unit = (BtlUnit *)btlGetIndexListEntry(command->targetList, (s32)i);
        groups |= unit->status.flags & 0x600;
    }

    count = 0;
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        u32 flags = unit->status.flags;
        if (!(flags & 1)) continue;
        if (flags & 0xC0) continue;
        if (flags & groups) count++;
    }

    if (count == 1) {
        btlBuildRandomTargetCameraStates(command, &command->frontCamera, &command->backCamera);
        command->motionParameter = 30.0f;
        command->flags |= 0x41;
        return;
    }

    task = command->task;
    user = task->unit;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(user->status.flags & 0x600);
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF_PTR(vf10, position);
    if (user->status.flags & 0x200) {
        mode = position[0] < 0.0f ? 2 : 3;
        btlSetupCameraAimDualUnit(command, front, mode, 60.0f, 0.1499999911f);
        btlSetupCameraAimDualUnit(command, back, mode, 5.0f, 0.0f);
        front->distance += 100.0f;
    } else {
        mode = 0.0f < position[0] ? 2 : 3;
        blend = 0.1999999881f;
        btlSetupCameraAimDualUnit(command, front, mode, 75.0f, blend);
        btlSetupCameraAimDualUnit(command, back, mode, 35.0f, blend);
        back->distance += 100.0f;
    }
    command->motionParameter = 40.0f;
    command->flags |= 0x41;
}


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
        btlSetDefaultActionCameraPose(command, pose);
        return;
    }
    if (mode == 3 || (mode == 1 && (effMiscRand((EffRandState *)effSharedRandomState) & 1))) {
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
    projected = btlMeasureMaskedUnitBounds(groups, NULL, NULL);
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
        projected *= tanf(radians);
        plane[2] += projected;
    } else {
        f32 radians = angle * 0.017453293f;
        plane[2] -= clearance;
        projected = pose->position[0];
        projected -= firstPosition[0];
        projected = ffabsf(projected);
        projected *= tanf(radians);
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
    distance += projected / tanf(fov);
    projected = btlProjectOnPlaneVU(plane, pose->position, lastPosition);
    VU0_STORE_VF(vf10, point);
    projected += clearance;
    {
        f32 secondDistance = projected / tanf(fov);
        VU0_LOAD_VF(vf10, point);
        VU0_LOAD_VF(vf11, pose->position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        secondDistance -= length;
        pose->distance = distance < secondDistance ? secondDistance : distance;
    }
    btlAdjustCameraDirectionForDefaultPlane(pose);
}

extern f32 atanf(f32);
extern f32 tanf(f32);
extern f32 sinf(f32);
extern void sdfVuLoadRotationMatrixZ(f32);

void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *action, BtlCamState *pose, u8 mode) {
    BtlUnit *actor;
    BtlUnit *target;
    f32 actorMuzzle[4];
    f32 targetMuzzle[4];
    f32 midpoint[4];
    f32 span;
    f32 midpointDistance;
    f32 scaledReach;
    f32 sideReach;
    f32 heightOffset;
    f32 angleScale;
    f32 angle;
    f32 halfFov;
    f32 sideLength;
    f32 tangent;
    s32 reverse;
    s32 alternate;
    f32 reachScale;

    switch (mode) {
    case 1:
        reverse = 0;
        alternate = 0;
        btlFlagAllUnitsDefeatCandidate();
        break;
    case 2:
        reverse = 1;
        alternate = 0;
        btlFlagAllUnitsDefeatCandidate();
        break;
    case 3:
        reverse = 0;
        alternate = 1;
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(action->task->unit->status.flags & 0x600);
        break;
    case 0:
    default:
        reverse = 0;
        alternate = 0;
        break;
    }

    /* Select the actor side and its framing reach multiplier. */
    switch (reverse) {
    case 0:
        actor = action->task->unit;
        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
        reachScale = 1.0f;
        break;
    default:
        target = action->task->unit;
        reachScale = 4.0f;
        actor = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
        break;
    }

    pose->fov = action->camera.fov;
    scaledReach = actor->reach * actor->scale;
    btlUnitGetMuzzlePosVU(actor);
    scaledReach *= reachScale;
    VU0_STORE_VF(vf10, actorMuzzle);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF(vf10, targetMuzzle);

    if (btlUnitGetTopY(target) < 600.0f) {
        heightOffset = 0.0f;
        actorMuzzle[1] -= actor->height * actor->scale * 0.25f;
    } else {
        tangent = tanf(pose->fov * 0.5f);
        heightOffset = actor->height * actor->scale * 0.5f / tangent;
    }

    VU0_LOAD_VF(vf10, targetMuzzle);
    VU0_LOAD_VF_FROM(vf11, *(const PcpVectorCopyF32 *)actorMuzzle);
    VU0_SCALAR_OP(0.5f, "vsubx.w vf3, vf0, vf2x\n\tvmulax.xyzw ACC, vf11, vf2x\n\tvmaddw.xyzw vf10, vf10, vf3w");
    VU0_STORE_VF(vf10, midpoint);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(midpointDistance);

    if (!reverse) {
        sideLength = fsqrtf(midpointDistance * midpointDistance - scaledReach * scaledReach);
        if (!alternate) {
            angleScale = scaledReach * 0x1.199998p+0f;
        } else {
            angleScale = scaledReach * 1.75f;
        }
    } else {
        sideReach = scaledReach / reachScale;
        sideLength = fsqrtf(midpointDistance * midpointDistance - sideReach * sideReach);
        angleScale = sideReach;
    }
    angle = atanf(angleScale / sideLength);

    if (!alternate) {
        if (targetMuzzle[0] > actorMuzzle[0]) {
            angle = -angle;
        }
    } else if (targetMuzzle[0] < actorMuzzle[0]) {
        if (actorMuzzle[2] < 0.0f) {
            angle = -angle;
        }
    } else if (0.0f < actorMuzzle[2]) {
        angle = -angle;
    }

    sdfVuLoadRotationMatrixZ(angle);
    VU0_LOAD_VF(vf10, actorMuzzle);
    VU0_LOAD_VF(vf11, midpoint);
    VU0_SUB(vf10, vf10, vf11);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_ADD(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, targetMuzzle);
    VU0_SUB(vf10, vf10, vf11);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, pose->position);
    VU0_MOVE_VF(vf11, vf12);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(span);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, pose->direction);

    halfFov = pose->fov * 0x1.555554p+0f * 0.5f;
    if (!reverse) {
        tangent = sinf(0x1.921fb4p+0f - halfFov);
        sideReach = scaledReach / tangent;
        tangent = tanf(halfFov);
        pose->distance = span + heightOffset + (angleScale + sideReach) / tangent;
    } else {
        tangent = tanf(halfFov);
        pose->distance = span + heightOffset + scaledReach / tangent;
    }

    btlUnitFaceTarget(actor, target);
    if (!alternate) {
        btlUnitFaceTargetScaled(target, actor, 0x1.0c1522p-1f);
    }
    btlAdjustCameraDirectionForDefaultPlane(pose);
}



extern void btlFlagAllUnitsDefeatCandidate(void);
extern s32 btlPointInBox(BtlVec3 *, BtlVec3 *, BtlVec3 *);
extern f32 tanf(f32);
extern void btlBuildActorPairFramingCameraPose(BtlLinkedCommand *, BtlCamState *, u8);

void btlBuildTargetGroupFramingCameraPose(BtlLinkedCommand *command, BtlCamState *pose) {
    f32 actorPoint[4] __attribute__((aligned(16)));
    f32 targetPoint[4] __attribute__((aligned(16)));
    f32 projection[4] __attribute__((aligned(16)));
    f32 currentMuzzle[4] __attribute__((aligned(16)));
    f32 facingRotation[4] __attribute__((aligned(16)));
    f32 firstExtreme[4] __attribute__((aligned(16)));
    f32 secondExtreme[4] __attribute__((aligned(16)));
    f32 framingDistances[2];
    f32 selectorScalar;
    f32 actorMargin, span, halfFov, length, distance;
    f32 fieldOfView;
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *actor = command->task->unit;
    BtlUnit *unit, *firstActor, *secondActor;
    u32 count, i, flags;

    if (btlHasSingleLinkedResource(command)) {
        if (actor->status.flags & 0x200) {
            btlBuildActorPairFramingCameraPose(command, pose, 1);
        } else {
            btlBuildActorPairFramingCameraPose(command, pose, 2);
        }
        return;
    }
    btlFlagAllUnitsDefeatCandidate();
    pose->fov = command->camera.fov;
    actorMargin = actor->reach * actor->scale;
    actorMargin *= 1.5f;
    count = btlGetIndexListCount(command->targetList);
    flags = 0;
    for (i = 0; i < count; i++) {
        flags |= ((BtlUnit *)btlGetIndexListEntry(command->targetList, i))->status.flags & 0x600;
    }
    if (flags == 0 || flags == 0x600) {
        btlSetDefaultActionCameraPose(command, pose);
        return;
    }
    span = btlMeasureMaskedUnitBounds(flags, 0, &selectorScalar);
    VU0_STORE_VF(vf10, targetPoint);
    if (count == 1) {
        if (500.0f < selectorScalar) {
            targetPoint[1] = -selectorScalar * 0x1.266666p-1f;
        } else {
            targetPoint[1] = -selectorScalar * 0x1.333332p-2f;
        }
    } else {
        targetPoint[1] = -selectorScalar * 0x1.ccccccp-2f;
    }
    firstActor = 0;
    secondActor = 0;
    for (unit = state->units; unit; unit = unit->next) {
        u32 unitFlags = unit->status.flags;
        if (unitFlags & 1) {
            if (!(unitFlags & 0xC0)) {
                if (unitFlags & flags) {
                    btlUnitGetMuzzlePosVU(unit);
                    VU0_STORE_VF(vf10, currentMuzzle);
                    if (firstActor == 0) {
                        firstActor = unit;
                        VU0_STORE_VF_UNCLOBBERED(vf10, firstExtreme);
                        secondActor = unit;
                        VU0_STORE_VF(vf10, secondExtreme);
                    } else {
                        f32 sampledX = currentMuzzle[0];
                        if (unit->status.flags & 0x200) {
                            if (sampledX < firstExtreme[0]) {
                                firstActor = unit;
                                PCP_COPY_VECTOR(firstExtreme, currentMuzzle);
                            }
                            if (secondExtreme[0] < sampledX) {
                                secondActor = unit;
                                PCP_COPY_VECTOR(secondExtreme, currentMuzzle);
                            }
                        } else {
                            if (firstExtreme[0] < sampledX) {
                                firstActor = unit;
                                PCP_COPY_VECTOR(firstExtreme, currentMuzzle);
                            }
                            if (sampledX < secondExtreme[0]) {
                                secondActor = unit;
                                PCP_COPY_VECTOR(secondExtreme, currentMuzzle);
                            }
                        }
                    }
                }
            }
        }
    }
    if (firstActor == 0 || secondActor == 0) {
        btlSetDefaultActionCameraPose(command, pose);
        return;
    }
    btlUnitGetMuzzlePosVU(actor);
    VU0_STORE_VF(vf10, actorPoint);
    actorPoint[1] = -btlUnitGetTopY(actor);
    fieldOfView = pose->fov;
    distance = ffabsf(targetPoint[0] - actorPoint[0]);
    halfFov = fieldOfView * 0x1.555554p+0f * 0.5f;
    if (actorMargin < distance) {
        distance = btlProjectOnPlaneVU(targetPoint, actorPoint, firstExtreme);
        distance += firstActor->reach * firstActor->scale * 0x1.599998p+0f;
        VU0_STORE_VF_UNCLOBBERED(vf10, projection);
        framingDistances[0] = distance / tanf(halfFov);
        VU0_LOAD_VF(vf10, projection);
        VU0_LOAD_VF(vf11, targetPoint);
        VU0_SUB(vf10, vf10, vf11);
        if (btlPointInBox((BtlVec3 *)targetPoint, (BtlVec3 *)actorPoint, (BtlVec3 *)projection)) {
            f32 projectedLength;
            VU0_LENGTH_VF10(projectedLength);
            framingDistances[0] += projectedLength;
        } else {
            f32 projectedLength;
            VU0_LENGTH_VF10(projectedLength);
            framingDistances[0] -= projectedLength;
        }
        distance = btlProjectOnPlaneVU(targetPoint, actorPoint, secondExtreme);
        distance += secondActor->reach * secondActor->scale * 0x1.599998p+0f;
        VU0_STORE_VF_UNCLOBBERED(vf10, projection);
        framingDistances[1] = distance / tanf(halfFov);
        VU0_LOAD_VF(vf10, projection);
        VU0_LOAD_VF(vf11, targetPoint);
        VU0_SUB(vf10, vf10, vf11);
        if (btlPointInBox((BtlVec3 *)targetPoint, (BtlVec3 *)actorPoint, (BtlVec3 *)projection)) {
            f32 projectedLength;
            VU0_LENGTH_VF10(projectedLength);
            framingDistances[1] += projectedLength;
        } else {
            f32 projectedLength;
            VU0_LENGTH_VF10(projectedLength);
            framingDistances[1] -= projectedLength;
        }
    } else {
        f32 secondMargin = secondActor->reach * secondActor->scale;
        f32 firstMargin = firstActor->reach * firstActor->scale;
        f32 tangent;
        if (firstMargin < secondMargin) {
            distance = secondMargin;
        } else {
            distance = firstMargin;
        }
        tangent = tanf(halfFov);
        framingDistances[0] = (span + distance * 0.5f) / tangent;
        framingDistances[0] += actorMargin / tanf(halfFov);
        framingDistances[1] = framingDistances[0];
    }
    VU0_LOAD_VF(vf10, targetPoint);
    VU0_STORE_VF(vf10, pose->position);
    VU0_LOAD_VF(vf11, actorPoint);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, pose->direction);
    length = distance + (actorMargin + actorMargin) / tanf(halfFov);
    if (framingDistances[1] < framingDistances[0]) {
        distance = framingDistances[0];
    } else {
        distance = framingDistances[1];
    }
    if (distance < length) {
        pose->distance = length;
    } else {
        pose->distance = distance;
    }
    if (actor->status.flags & 0x80000) {
        btlUnitGetBodyPosVU(actor);
        VU0_STORE_VF(vf10, actorPoint);
        if (btlAimHorizontalDirectionVU(actorPoint, targetPoint)) {
            VU0_STORE_VF_UNCLOBBERED(vf10, facingRotation);
            btlSetUnitRotation(actor, facingRotation);
        }
    }
    btlAdjustCameraDirectionForDefaultPlane(pose);
}


extern f32 cosf(f32);
extern f32 sinf(f32);
extern f32 acosf(f32);

/* vu0 routine: fit the action camera around one target and its reach. */
void btlFrameSingleTargetActionCamera(BtlLinkedCommand *action, BtlCamState *out) {
    BtlUnit *user;
    BtlUnit *target;
    f32 targetPosition[4];
    f32 center[4];
    f32 userPosition[4];
    f32 rotation[4];
    f32 height;
    f32 halfFov;
    f32 angle;
    f32 extent;
    f32 span;
    f32 t;
    f32 distance;
    f32 firstLength;
    f32 secondLength;
    f32 dot;
    f32 centerDistance;
    f32 minimum;
    f32 diameter;

    user = action->task->unit;
    if (btlGetIndexListCount(action->targetList) >= 2U) {
        btlSetDefaultActionCameraPose(action, out);
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    btlFlagAllUnitsDefeatCandidate();
    out->fov = action->camera.fov;
    extent = target->reach * target->scale;
    span = btlMeasureMaskedUnitBounds(user->status.flags & 0x600, 0, &height);
    VU0_STORE_VF_UNCLOBBERED(vf10, center);
    center[1] = -height * 0.5f;
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPosition);
    targetPosition[1] = -btlUnitGetTopY(target);
    halfFov = out->fov * 1.3333333f * 0.5f;
    VU0_LOAD_VF(vf10, targetPosition);
    VU0_LOAD_VF(vf11, center);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LENGTH_VF10(firstLength);
    VU0_LOAD_VF(vf10, targetPosition);
    VU0_SCALAR_OP(center[0], "vaddx.x vf10, vf0, vf2x");
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(secondLength);
    VU0_MOVE_VF(vf11, vf12);
    VU0_DOT_XYZ(dot, vf10, vf11);
    angle = acosf(dot / (firstLength * secondLength));
    t = span * cosf(angle);
    distance = t / tanf(halfFov);
    distance += t * tanf(angle);
    VU0_LOAD_VF(vf10, center);
    VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
    VU0_LOAD_VF(vf11, targetPosition);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(centerDistance);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_UNCLOBBERED(vf10, out->direction);
    diameter = extent + extent;
    t = diameter / sinf(1.5707963f - halfFov);
    minimum = centerDistance + (extent + t) / tanf(halfFov);
    if (distance < minimum) {
        out->distance = minimum;
    } else {
        out->distance = distance;
    }
    if (target->status.flags & 0x80000) {
        btlUnitGetBodyPosVU(user);
        VU0_STORE_VF_UNCLOBBERED(vf10, userPosition);
        if (btlAimHorizontalDirectionVU(targetPosition, userPosition) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
            btlSetUnitRotation(target, (s128 *)rotation);
        }
    }
    btlAdjustCameraDirectionForDefaultPlane(out);
}

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
        btlMeasureMaskedUnitBounds(mask, 0, 0);
        VU0_STORE_VF(vf10, &vec[0]);
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU((f32 *)&vec[1], (f32 *)&vec[0]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation(unit, &vec[2]);
        }
    }
}

extern f32 tanf(f32);

s32 btlBuildMaskedGroupFramingCameraPose(BtlLinkedCommand *command, BtlCamState *camera, s32 mask,
                   f32 *direction, f32 distance, f32 xOffset) {
    BtlUnit *unit;
    BtlUnit *leftActor = 0;
    BtlUnit *rightActor = 0;
    BtlState *state = (BtlState *)btlGetRuntime();
    f32 point[4] __attribute__((aligned(16)));
    f32 leftPoint[4] __attribute__((aligned(16)));
    f32 rightPoint[4] __attribute__((aligned(16)));
    f32 planePoint[4] __attribute__((aligned(16)));
    f32 height;
    f32 halfFov;
    f32 projected;
    f32 tangent;
    f32 lowerDistance;
    f32 upperDistance;
    f32 y;
    f32 pointDistance;

    for (unit = state->units; unit != 0; unit = unit->next) {
        u32 flags = (u32)unit->status.flags;
        if (flags & 1) {
            if ((flags & 0xC0) == 0) {
                if (flags & (u32)mask) {
                    btlUnitGetMuzzlePosVU(unit);
                    VU0_STORE_VF_TO_MEMORY(vf10, *(PcpVectorCopyF32 *)point);
                    if (!leftActor) {
                        leftActor = unit;
                        VU0_STORE_VF_UNCLOBBERED(vf10, leftPoint);
                        rightActor = unit;
                        VU0_STORE_VF(vf10, rightPoint);
                    } else {
                        f32 pointX = point[0];
                        if ((u32)unit->status.flags & 0x200) {
                            if (pointX < leftPoint[0]) {
                                leftActor = unit;
                                PCP_COPY_VECTOR_F32(leftPoint, point);
                            }
                            if (rightPoint[0] < pointX) {
                                rightActor = unit;
                                PCP_COPY_VECTOR_F32(rightPoint, point);
                            }
                        } else {
                            if (leftPoint[0] < pointX) {
                                leftActor = unit;
                                PCP_COPY_VECTOR_F32(leftPoint, point);
                            }
                            if (pointX < rightPoint[0]) {
                                rightActor = unit;
                                PCP_COPY_VECTOR_F32(rightPoint, point);
                            }
                        }
                    }
                }
            }
        }
    }

    rightPoint[1] = 0.0f;
    leftPoint[1] = 0.0f;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(mask);
    halfFov = command->camera.fov;
    camera->fov = halfFov;
    halfFov *= 0.5f;
    projected = btlMeasureMaskedUnitBounds(mask, &height, 0);
    VU0_STORE_VF_UNCLOBBERED(vf10, camera->position);
    camera->position[2] *= 0.75f;
    camera->position[1] = -(height * 0.5f);

    if (mask & 0x200) {
        if (direction[0] < 0.0f) {
            camera->position[0] += projected * xOffset;
        } else {
            camera->position[0] -= projected * xOffset;
        }
        if (projected < 350.0f) {
            rightPoint[0] = 350.0f - rightActor->reach * rightActor->scale;
            leftPoint[0] = -(350.0f - leftActor->reach * leftActor->scale);
        }
    } else {
        if (direction[0] > 0.0f) {
            camera->position[0] += projected * xOffset;
        } else {
            camera->position[0] -= projected * xOffset;
        }
        if (projected < 350.0f) {
            rightPoint[0] = -(350.0f - rightActor->reach * rightActor->scale);
            leftPoint[0] = 350.0f - leftActor->reach * leftActor->scale;
        }
    }

    y = -camera->position[1];
    if (y < distance) {
        camera->position[1] = -distance;
    } else {
        distance = y;
    }

    VU0_LOAD_VF(vf10, camera->position);
    VU0_LOAD_VF(vf11, direction);
    VU0_STORE_VF(vf11, camera->direction);
    VU0_SCALE_VF(vf11, -500.0f);
    VU0_ADD(vf11, vf11, vf10);
    VU0_STORE_VF(vf11, planePoint);

    if (((mask & 0x200) && planePoint[0] > 0.0f) ||
        ((mask & 0x400) && planePoint[0] < 0.0f)) {
        unit = leftActor;
        leftActor = rightActor;
        rightActor = unit;
        VU0_LOAD_VF(vf10, leftPoint);
        VU0_LOAD_VF(vf11, rightPoint);
        VU0_STORE_VF(vf11, leftPoint);
        VU0_STORE_VF(vf10, rightPoint);
    }

    projected = btlProjectOnPlaneVU(camera->position, planePoint, leftPoint);
    VU0_STORE_VF(vf10, camera->position);
    projected += leftActor->reach * leftActor->scale * 1.5f;
    tangent = tanf(halfFov * 0x1.555554p+0f);
    projected /= tangent;
    upperDistance = distance / tanf(halfFov);
    lowerDistance = upperDistance < projected ? projected : upperDistance;

    projected = btlProjectOnPlaneVU(camera->position, planePoint, rightPoint);
    projected += rightActor->reach * rightActor->scale * 1.5f;
    VU0_LOAD_VF(vf11, camera->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(pointDistance);
    xOffset = -pointDistance;
    tangent = tanf(halfFov * 0x1.555554p+0f);
    projected /= tangent;
    upperDistance = distance / tanf(halfFov);
    xOffset += upperDistance < projected ? projected : upperDistance;
    if (lowerDistance < xOffset) {
        camera->distance = xOffset;
    } else {
        camera->distance = lowerDistance;
    }
    return btlAdjustCameraDirectionForDefaultPlane(camera);
}

extern s32 btlBuildMaskedGroupFramingCameraPose(BtlLinkedCommand *, BtlCamState *, s32, f32 *, f32, f32);

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
void btlBuildRandomizedActionCameraPosePair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
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
    btlBuildMaskedGroupFramingCameraPose(action, from, groups, direction, distance, 0.0f);
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
        btlBuildMaskedGroupFramingCameraPose(action, to, groups, direction, distance, 0.0f);
    } else {
        btlBuildMaskedGroupFramingCameraPose(action, to, groups, direction, distance, -0x1.333332p-3f);
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
    span = btlGetUnitMotionDurationFrames(user, user->motionId);
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
    width = length * sinf(angle);
    length *= cosf(angle);
    length += (extent + width) / tanf(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (action->flags & 0x200) {
        angle = -angle;
    }
    sdfVuLoadRotationMatrixZ(angle);
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
        btlSetDefaultActionCameraPose(command, &command->camera);
        return;
    }
    btlFrameSingleTargetActionCamera(command, &command->frontCamera);
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
    frames = btlGetUnitMotionDurationFrames(user, user->motionId);
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
        extent += length * sinf(angle);
        length *= cosf(angle);
        out->distance = length + extent / tanf(out->fov * 1.3333333f * 0.5f);
        if (!(action->flags & 0x200)) {
            angle = -0.34906585f;
        }
        sdfVuLoadRotationMatrixZ(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, out->direction);
        btlAdjustCameraDirectionForDefaultPlane(out);
    }
}

extern u8 D_003A43E0[];
extern u8 D_003A43F0[];

/* vu0 routine: frame the action's target groups from the actor's side, widening
 * for several targets, then keep the actor clear of the near camera plane. */
INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4000);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4180);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4190);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4310);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4320);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A43E0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A43F0);

void btlFrameCommandTargetGroups(BtlLinkedCommand *action) {
    BtlState *state;
    BtlUnit *unit;
    BtlUnit *actor;
    f32 *cameraPosition;
    f32 heightPosition[4];
    f32 focus[4];
    f32 position[4];
    f32 height;
    f32 span;
    f32 fov;
    f32 framing;
    f32 zero = 0.0f;
    u32 count;
    s32 groups;
    u32 i;

    state = (BtlState *)btlGetRuntime();
    unit = action->task->unit;
    if (unit == 0) {
        return;
    }
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF(vf10, focus);
    cameraPosition = action->camera.position;

    count = btlGetIndexListCount(action->targetList);
    groups = 0;
    for (i = 0; i < count; i++) {
        BtlUnit *target = (BtlUnit *)btlGetIndexListEntry(action->targetList, i);
        groups |= target->status.flags & 0x600;
    }

    height = 0.0f;
    count = 0;
    actor = state->units;
    while (actor != 0) {
        s32 flags = actor->status.flags;
        if (flags & 1) {
            if (!(flags & 0xC0)) {
                if (flags & groups) {
                    btlUnitGetMuzzlePosVU(actor);
                    VU0_STORE_VF(vf10, heightPosition);
                    height += heightPosition[1];
                    count++;
                }
            }
        }
        actor = actor->next;
    }

    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(groups);
    fov = action->camera.fov;
    span = btlMeasureMaskedUnitBounds(groups, NULL, NULL);
    VU0_STORE_VF(vf10, cameraPosition);
    height /= (f32)count;
    action->camera.position[1] = height;

    span = (f32)((double)span * 1.3);
    if (span < 500.0f) {
        span = 500.0f;
    }
    if (count >= 2) {
        framing = 1.3333333f;
        framing = fov * framing;
        framing *= 0.5f;
    } else {
        framing = 0.5f;
        framing = fov * framing;
    }
    framing = span / tanf(framing);
    action->camera.distance = framing;

    if (focus[0] > zero) {
        VU0_LOAD_VF(vf10, D_003A43E0);
    } else {
        VU0_LOAD_VF(vf10, D_003A43F0);
    }
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, action->camera.direction);
    btlAdjustCameraDirectionForDefaultPlane(&action->camera);

    {
        u8 *statusTable = (u8 *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
        f32 clearance = *(f32 *)(statusTable + unit->motionId * 0x14 + 0x38) * unit->scale;
        btlCopyUnitCurrentPosition(unit, position);
        if (position[2] - clearance < -100.0f) {
            position[2] += -100.0f - (position[2] - clearance);
            if (position[2] > 2000.0f - unit->reach * unit->scale) {
                position[2] = 2000.0f - unit->reach * unit->scale;
            }
            btlSetUnitPosition(unit, position);
        }
    }
}

extern BtlUnit *btlFindFarthestUnit(u32, f32 *);

void btlBuildActorGroupProjectionCameraPose(BtlLinkedCommand *source, BtlCamState *out,
                   BtlUnit *actor, s32 side) {
    BtlState *state;
    BtlUnit *unit;
    BtlUnit *farthestUnit;
    f32 point[4] __attribute__((aligned(16)));
    /* Retail retains these secondary-center updates without a later read. */
    f32 shiftedCenter[4] __attribute__((aligned(16)));
    f32 farthestPos[4] __attribute__((aligned(16)));
    f32 projected[4] __attribute__((aligned(16)));
    f32 center[4] __attribute__((aligned(16)));
    f32 height;
    f32 span;
    f32 fov;
    f32 projectedDistance;
    f32 length;
    f32 distance;
    u32 markedCount;

    state = (BtlState *)btlGetRuntime();
    span = btlMeasureMaskedUnitBounds(0x400, &height, 0);
    VU0_STORE_VF_PTR(vf10, shiftedCenter);
    VU0_STORE_VF_PTR(vf10, center);

    /* sched1 issues the last use of y first in each pair of height stores. */
    if (height > 750.0f) {
        f32 y = -height * 0.4f;
        shiftedCenter[1] = y;
        center[1] = y;
    }

    markedCount = 0;
    for (unit = state->units; unit != NULL; unit = unit->next) {
        u32 flags = unit->status.flags;
        if ((flags & 3) != 0 && (flags & 0x200) != 0) {
            markedCount++;
        }
    }

    if (actor->lookupId != 1 && markedCount == 1) {
        f32 y = -height * 0.55f;
        shiftedCenter[1] = y;
        center[1] = y;
    }

    btlUnitGetMuzzlePosVU(actor);
    VU0_STORE_VF_PTR(vf10, point);
    point[1] = -btlUnitGetTopY(actor) * 0.775f;
    if (actor->lookupId != 1) {
        if (point[0] > 0.0f) {
            if (markedCount >= 2) {
                if (side == 0) {
                    point[0] += actor->reach * actor->scale * 0.25f;
                } else {
                    point[0] -= actor->reach * actor->scale * 1.75f;
                }
                shiftedCenter[0] += span;
            } else {
                if (side == 0) {
                    point[0] += actor->reach * actor->scale;
                } else {
                    point[0] -= actor->reach * actor->scale;
                }
            }
        } else if (markedCount >= 2) {
            if (side == 0) {
                point[0] -= actor->reach * actor->scale * 0.25f;
            } else {
                point[0] += actor->reach * actor->scale * 1.75f;
            }
            shiftedCenter[0] -= span;
        } else {
            if (side == 0) {
                point[0] -= actor->reach * actor->scale;
            } else {
                point[0] += actor->reach * actor->scale;
            }
        }
    } else {
        if (point[0] > 0.0f) {
            point[0] += actor->reach * actor->scale;
            shiftedCenter[0] += span;
            center[0] = -(actor->reach * actor->scale) * 0.5f;
        } else {
            point[0] -= actor->reach * actor->scale;
            shiftedCenter[0] -= span;
            center[0] = (actor->reach * actor->scale) * 0.5f;
        }
    }

    farthestUnit = btlFindFarthestUnit(0x200, point);
    btlUnitGetMuzzlePosVU(farthestUnit);
    VU0_STORE_VF_PTR(vf10, farthestPos);
    farthestPos[2] -= farthestUnit->reach * farthestUnit->scale;
    if (actor->lookupId != 1 && side != 0) {
        farthestPos[2] -= farthestUnit->reach * farthestUnit->scale * 0.75f;
    }

    fov = source->camera.fov;
    out->fov = fov;
    if (actor->lookupId == 1 || markedCount >= 2) {
        center[2] = -50.0f;
    }
    projectedDistance = btlProjectOnPlaneVU(center, point, farthestPos);
    /* VU-only scratch: no scalar readback or intervening call. The SDK bare
     * store also ends CSE reuse of the center address, as retail requires. */
    VU0_STORE_VF_UNCLOBBERED(vf10, projected);
    VU0_LOAD_VF_PTR(vf10, center);
    VU0_STORE_VF_PTR(vf10, out->position);
    VU0_LOAD_VF_PTR(vf11, projected);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF_PTR(vf10, out->direction);

    fov *= 0x1.555554p-1f;
    distance = length + projectedDistance / tanf(fov);
    if (markedCount == 1) {
        distance += (actor->reach * actor->scale) * 2.5f /
                    tanf(fov);
    }
    if (distance > 0.0f) {
        out->distance = distance;
    } else {
        out->distance = 0.0f;
    }
    if (height > 750.0f) {
        out->distance += 150.0f;
    }
    if (side == 1) {
        out->distance += 100.0f;
    }
}


extern BtlUnit *btlFindFarthestUnit(u32, f32 *);

extern BtlUnit *btlFindNearestUnit(u32, BtlUnit *);

extern void btlFlagAllUnitsDefeatCandidate(void);

extern f32 cosf(f32);

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
        tanHalf = tanf(halfFov);
        distance = (distance + refReach / cosf(halfFov)) / tanHalf;
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
        btlMeasureMaskedUnitBounds(0x200, &span[0], NULL);
        focus[1] = -span[0];
        fit = btlMeasureMaskedUnitBounds(0x400, &span[1], &span[2]);
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
            nearFit = fit * 1.2f / tanf(halfFov);
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
            fits[2] = offset + diameter / tanf(halfFov);
        } else {
            fits[2] = offset + span[0] / tanf(halfFov);
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

extern void btlBuildActorGroupProjectionCameraPose(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

extern void btlBuildShoulderActionCamera(BtlLinkedCommand *, BtlCamState *, BtlUnit *, s32);

/* Apply the marked actor's action camera, honoring the runtime camera-arrangement hook. */
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
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        s32 flags;

        if ((btlUnitStatusPair(unit) & 0x300) != 0x300) {
            flags = unit->status.flags;
        } else if (runtime->cameraPresetMode != 3) {
            flags = unit->status.flags;
        } else {
            if (unit->lookupId != 1) {
                btlCopyUnitCurrentPosition((u8 *)unit, vectors.position);
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
        btlBuildActorGroupProjectionCameraPose(action, pose, target, mode);
    } else {
        btlBuildShoulderActionCamera(action, pose, target, mode);
    }

    btlMeasureMaskedUnitBounds(0x400, NULL, NULL);
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

    btlMeasureMaskedUnitBounds(0x200, NULL, NULL);
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

extern f32 func_001F6E28(BtlIndexList *, f32 *, f32 *);

/* Native pointer-form transfer with a complete 16-byte memory operand. */
static inline void btlDefeatCameraStoreVector(f32 *dst) {
    __asm__ volatile (
        ".set noreorder\n\tsqc2 vf10, 0(%1)\n\t.set reorder"
        : "=m" (*(PcpVectorCopyF32 *)dst) : "r" (dst));
}

/* Use the defeat-camera hook or frame the selected actor set with the native extent rules. */
void btlBuildActorSetCameraPose(BtlLinkedCommand *action, BtlCamState *pose,
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
    f32 span;
    f32 height;
    f32 depth;
    f32 length;
    f32 tangent;
    f32 fov;

    if (callback != NULL && callback(action, pose, 1) != 0) {
        return;
    }

    if (mode == 0) {
        btlApplyMarkedUnitActionCamera(action, pose, startActor, 1);
        i = 0;
        count = btlGetIndexListCount(indices);
        if ((u32)(s32)mode < count) {
            for (; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(indices, (s32)i);
                if ((entry->status.flags & 0x20) != 0) {
                    btlInterpolateVectorStep(pose->position);
                    btlDefeatCameraStoreVector(vectors.interpolated);
                    btlMeasureMaskedUnitBounds(0x200, NULL, NULL);
                    btlDefeatCameraStoreVector(pose->position);
                    pose->position[1] = 0.0f;
                    pose->position[2] *= 0.5f;
                    __asm__ volatile (".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (pose->position), "m" (*(const PcpVectorCopyF32 *)(pose->position)));
                    __asm__ volatile (".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (vectors.interpolated), "m" (*(const PcpVectorCopyF32 *)(vectors.interpolated)));
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
        span = btlMeasureMaskedUnitBounds((s32)sideFlags, &height, &depth);
    } else {
        span = func_001F6E28(indices, &height, &depth);
    }
    btlDefeatCameraStoreVector(vectors.center);
    if (mode == 0 || runtime->cameraPresetMode == 3) {
        span += ffabsf(vectors.center[0]);
        vectors.center[0] = 0.0f;
        vectors.center[1] = -(height + depth) * 0.5f;
    } else {
        vectors.center[1] = -height * 0.75f;
        span *= 1.25f;
    }

    btlMeasureMaskedUnitBounds(0x400, NULL, &depth);
    btlDefeatCameraStoreVector(vectors.edge);
    vectors.edge[0] = 0.0f;
    if (depth > 150.0f) {
        vectors.edge[1] = -depth * 0.45f;
    } else {
        vectors.edge[1] = -depth * 0.25f;
    }
    if (mode == 0) {
        if (action->camera.direction[0] > 0.0f) {
            sdfVuLoadRotationMatrixZ(27.5f * 0.017453293f);
        } else {
            sdfVuLoadRotationMatrixZ(-(27.5f * 0.017453293f));
        }
    }

    __asm__ volatile (".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r" (vectors.edge), "m" (*(const PcpVectorCopyF32 *)(vectors.edge)));
    __asm__ volatile (".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r" (vectors.center), "m" (*(const PcpVectorCopyF32 *)(vectors.center)));
    VU0_LERP_VF10(0.5f);
    btlDefeatCameraStoreVector(pose->position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(length);
    VU0_NORMALIZE_VF10();
    if (mode == 0) {
        VU0_ROTATE_VEC(vf10, vf10);
    }
    btlDefeatCameraStoreVector(pose->direction);

    tangent = cosf(27.5f * 0.017453293f);
    length /= tangent;
    fov *= 1.3333333f;
    tangent = tanf(fov * 0.5f);
    length += span / tangent;
    pose->distance = length;
    btlAdjustCameraDirectionForDefaultPlane(pose);
}

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
    btlSetDefaultActionCameraPose(command, out);
    btlCopyMotionTransform(pose, out);
    span = btlMeasureMaskedUnitBounds(0x200, 0, 0) * 0.5f;
    pose->position[0] -= span;
    out->position[0] += span;
    out->distance *= 0.8f;
    btlUnitGetMuzzlePosVU(actor);
    VU0_SET_VF10_COMPONENT(y, -btlUnitGetTopY(actor));
    VU0_LOAD_VF(vf11, out);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    distance += (actor->cameraRadius * actor->scale * 2.0f) /
                tanf(out->fov * 0.5f);
    if (out->distance < distance) {
        out->distance = distance;
    }
}

/* Forward the linked action and both poses to the tilt-rotation preparation. */
void btlUpdateTiltedActionCameraPose(BtlLinkedCommand *command, BtlCamState *pose, BtlCamState *out) {
    btlPrepareUnitPoseWithTiltRotation(command, pose, out);
}

/* vu0 routine: frame the leftmost marked unit in two camera poses. */
void btlFrameLeftmostUnitCameraPair(BtlCamState *source, BtlCamState *from,
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
    btlMeasureMaskedUnitBounds(0x400, &height, 0);
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
                              tanf(fov * 0.5f);
    btlCopyMotionTransform(to, from);
    sdfVuLoadRotationMatrixZ(-(45.0f * 0.017453293f));
    VU0_LOAD_VF(vf10, to->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);
    to->distance = length + selected->cameraRadius * selected->scale * 3.0f /
                            tanf(fov * 0.5f);
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
    span = btlMeasureMaskedUnitBounds(0x200, &height, 0);
    VU0_STORE_VF(vf10, out->position);
    out->position[1] = -height * 1.15f;
    btlMeasureMaskedUnitBounds(0x400, &height, 0);
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
    out->distance = span / tanf(fov * 0.5f);
    btlAdjustCameraDirectionForDefaultPlane(out);
}

void btlResetUnitEffectVector(BtlLinkedCommand *command, BtlCamState *pose) {
    btlBuildBattleUnitFramingCameraPose(command, pose, 0.0f);
}

/* Front and back camera rotations of one animated-unit framing preset. */
typedef struct BtlCameraQuaternionPair {
    f32 front[4];
    f32 back[4];
} BtlCameraQuaternionPair;

extern const BtlCameraQuaternionPair D_003A4408[3];

/* vu0 routine: frame the marked kind-11 unit that is earliest in its
 * animation; the back camera sits between the front camera and the group. */
void btlFrameEarliestAnimatedUnitCameraPair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 position[4];
    BtlCameraQuaternionPair presets[3];
    BtlState *work;
    BtlUnit *unit;
    BtlUnit *selected;
    f32 frame;
    f32 earliest;
    f32 halfFov;
    f32 extent;
    s32 pose;
    u32 flags;

    memcpy(presets, D_003A4408, sizeof(presets));
    /* Initialize before the runtime lookup: sched2 ties this zero with the
     * selected-unit zero and issues the earlier initializer first. */
    earliest = 0.0f;
    work = (BtlState *)btlGetRuntime();
    selected = NULL;
    for (unit = work->units; unit != NULL; unit = unit->next) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if ((flags & 0xE0) && unit->motionId == 11) {
                    frame = btlGetUnitModelCurrentFrame(unit);
                    if (selected == NULL) {
                        earliest = frame;
                        selected = unit;
                    } else if (frame < earliest) {
                        earliest = frame;
                        selected = unit;
                    }
                }
            }
        }
    }
    if (selected == NULL) {
        btlSetDefaultActionCameraPose(action, &action->camera);
        return;
    }
    btlFlagAllUnitsDefeatCandidate();
    action->selectedUnit = selected;
    switch (work->cameraPresetMode) {
    case 0:
    case 1:
        pose = 1;
        break;
    case 2:
        pose = selected->lookupId ? 2 : 0;
        break;
    default:
        pose = selected->lookupId;
        break;
    }
    halfFov = action->camera.fov;
    from->fov = halfFov;
    to->fov = halfFov;
    halfFov *= 0.5f;
    btlUnitGetMuzzlePosVU(selected);
    VU0_STORE_VF(vf10, position);
    if (btlSampleActorModelPositionVU(selected, 1) == 0) {
        VU0_LOAD_VF(vf10, position);
    }
    VU0_STORE_VF(vf10, from->position);
    earliest = selected->reach * selected->scale;
    from->distance = (earliest + earliest) / tanf(halfFov * 1.3333333f);
    VU0_LOAD_VF(vf10, presets[pose].front);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    if (selected->status.flags & 0x400) {
        VU0_NEGATE_XYZ(vf10);
    }
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, from->direction);
    extent = btlMeasureMaskedUnitBounds(0x200, NULL, NULL);
    VU0_LOAD_VF(vf11, from->position);
    VU0_LERP_VF10(0.7f);
    VU0_STORE_VF(vf10, to->position);
    VU0_LOAD_VF(vf10, presets[pose].back);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    if (selected->status.flags & 0x400) {
        VU0_NEGATE_XYZ(vf10);
    }
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);
    extent *= 0.8f;
    if (extent < 200.0f) {
        extent = 200.0f;
    }
    to->distance = extent / tanf(halfFov);
    btlAdjustCameraDirectionForDefaultPlane(from);
    btlAdjustCameraDirectionForDefaultPlane(to);
    action->motionParameter = 100.0f;
    action->flags = (action->flags | 0x41) & ~0x4000;
}


/* vu0 routine: follow the tracked unit until its animation tail, then frame both groups. */
void btlStepUnitHeightFramedCamera(BtlLinkedCommand *action) {
    f32 userPos[4];
    f32 targetPos[4];
    f32 direction[4];
    f32 height[2];
    f32 length;
    f32 fov;
    BtlUnit *unit = action->selectedUnit;
    BtlCamState *from;
    BtlCamState *saved;
    s32 endFrame;

    if (unit == NULL) {
        return;
    }
    if (action->flags & 0x4000) {
        return;
    }
    from = &action->frontCamera;
    endFrame = btlGetUnitModelFrameCount(unit) + 30;
    saved = &action->backCamera;
    if (action->state < endFrame) {
        if (btlSetActorEffectParameter(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, from->position);
        if (saved->position[1] < from->position[1]) {
            saved->position[1] = from->position[1];
        }
        btlAdjustCameraDirectionForDefaultPlane(from);
        btlAdjustCameraDirectionForDefaultPlane(saved);
        return;
    }
    if (action->state == endFrame) {
        btlMeasureMaskedUnitBounds(0x400, &height[0], 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, targetPos);
        targetPos[0] = 0.0f;
        targetPos[1] = -height[0] * 0.5f;
        length = btlMeasureMaskedUnitBounds(0x200, &height[1], 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, userPos);
        userPos[1] = -height[1] * 0.25f;
        if (height[0] < 600.0f) {
            VU0_LOAD_VF(vf10, targetPos);
            VU0_LOAD_VF(vf11, userPos);
            VU0_SCALAR_OP(0.7f,
                          "vsubx.w vf3, vf0, vf2x\n\t"
                          "vmulax.xyzw ACC, vf11, vf2x\n\t"
                          "vmaddw.xyzw vf10, vf10, vf3w");
            VU0_STORE_VF_UNCLOBBERED(vf10, from->position);
            VU0_LOAD_VF(vf10, targetPos);
            VU0_LOAD_VF(vf11, userPos);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(length);
            length *= 0.5f;
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF_UNCLOBBERED(vf10, direction);
            VU0_LOAD_VF(vf10, D_00359EE0);
            effMiscQuaternionToMatrixVU();
            VU0_LOAD_VF(vf10, direction);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, from->direction);
            fov = action->camera.fov;
            from->distance = length / tanf(fov * 0.5f);
            btlCopyMotionTransform(saved, from);
            saved->distance += 250.0f;
        } else {
            VU0_LOAD_VF(vf10, targetPos);
            VU0_STORE_VF_UNCLOBBERED(vf10, from->position);
            VU0_LOAD_VF(vf11, userPos);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(from->distance);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF_UNCLOBBERED(vf10, from->direction);
            fov = action->camera.fov * 1.3333333f;
            if (height[0] * 0.5f < length) {
                from->distance += length / tanf(fov * 0.5f);
            } else {
                from->distance += height[0] * 0.5f / tanf(fov * 0.5f);
            }
            btlCopyMotionTransform(saved, from);
            saved->distance += 250.0f;
        }
        btlAdjustCameraDirectionForDefaultPlane(from);
        btlAdjustCameraDirectionForDefaultPlane(saved);
        action->motionParameter = 100.0f;
        action->state = 0;
        action->flags |= 0x4041;
    }
}

/* The pair preset row with an integer frame count in place of the motion parameter. */
typedef struct BattlePairCameraFramePreset {
    f32 fromQuaternion[4];
    f32 toQuaternion[4];
    f32 fromDistanceScale;
    f32 toDistanceScale;
    f32 fromHeightScale;
    s32 frames;
} BattlePairCameraFramePreset;

/* Frame the first listed target from one of two randomized pair presets: both
 * cameras start at its muzzle, lowered by the preset height scale, at a distance
 * that fits the larger of its reach (wider view) and half its height. */
INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4408);

void btlBuildRandomDefeatCandidateCameraPair(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quaternion[4];
    f32 position[4];
    BattlePairCameraFramePreset poses[2] = {
        {{0.058f, -0.871f, 0x1.1eb85p-3f, -0.443f},
         {-0.03f, -0x1.f0a3d6p-1f, 0.153f, 0x1.c6a7eep-4f},
         1.3f, 0x1.f33332p0f, 0.775f, 32},
        {{-0.042f, -0x1.e7ef9cp-1f, 0.117f, 0.232f},
         {0.03f, -0x1.deb85p-1f, 0x1.418936p-3f, -0x1.1eb85p-2f},
         1.3f, 0x1.f33332p0f, 0.775f, 32},
    };
    BtlUnit *unit = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    s32 pose;
    f32 fov;
    f32 height;
    f32 reach;

    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->status.flags & 0x600);
    reach = unit->reach * unit->scale;
    height = unit->height;
    height *= unit->scale;
    pose = effMiscRandMod(0, 2);
    btlCopyUnitRotationQuaternion(unit, quaternion);
    fov = action->camera.fov;
    from->fov = fov;
    to->fov = fov;
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF_UNCLOBBERED(vf10, position);
    VU0_LOAD_VF(vf10, position);
    VU0_STORE_VF(vf10, from->position);
    VU0_STORE_VF(vf10, to->position);
    to->position[1] = from->position[1] = -height * poses[pose].fromHeightScale;
    height *= 0.5f;
    fov *= 0.5f;
    if (height < reach) {
        height = reach / tanf(fov * 0x1.555554p+0f);
    } else {
        height = height / tanf(fov);
    }
    from->distance = height * poses[pose].fromDistanceScale;
    to->distance = height * poses[pose].toDistanceScale;

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
}

/* cameraKind 7 participant preparation; the camera argument is unused. */
void btlPrepareCameraMode7Participants(BtlLinkedCommand *action, BtlCamState *camera) {
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
    span = btlGetUnitMotionDurationFrames(user, user->motionId);
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
    width = length * sinf(angle);
    length *= cosf(angle);
    length += (extent + width) / tanf(out->fov * 1.3333333f * 0.5f);
    out->distance = length;
    if (action->flags & 0x200) {
        angle = -angle;
    }
    sdfVuLoadRotationMatrixZ(angle);
    VU0_LOAD_VF(vf10, dir);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, out->direction);
    btlAdjustCameraDirectionForDefaultPlane(out);
}

/* cameraKind 6 uses the same participant preparation through this separate entry. */
void btlPrepareCameraMode6Participants(BtlLinkedCommand *action, BtlCamState *camera) {
    btlPrepareCameraMode7Participants(action, camera);
}

/* Update the supplied pose with height-clamped actor/target approach framing. */
void btlUpdateApproachCameraPose(BtlLinkedCommand *action, BtlCamState *out) {
    btlBuildHeightClampedApproachCamera(action, out);
}

extern void btlBuildFilteredGroupCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

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
        btlBuildFilteredGroupCameraPair(action, &action->frontCamera, &action->backCamera);
        return;
    }
    btlPrepareUnitPoseWithTiltRotation(action, &action->frontCamera, &action->backCamera);
    action->motionParameter = 200.0f;
    action->flags |= 0x41;
}

void func_001E57F8(void) {
}

extern s16 D_003BB670[];
extern s16 D_003BB678[];
extern s16 D_003BB680[];
extern u32 btlNextScaledRandom(u32);
extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);
extern void func_001E6668(BtlLinkedCommand *, BtlCamState *, s32, s32);

/* Four retail flag/boolean sites: 1E5CE4/1E5CF0/1E5D3C/1E5D48; matched DDS2 predicate. */
static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

void btlInitSkillCommandCursor(BtlLinkedCommand *command, BtlCamState *pose) {
    s16 groupChoices[2];
    s16 pathChoices[4];
    s16 sideChoices[2];
    s16 choice;
    s32 orientation;
    s32 initialFlags;
    s32 pathSet;
    u16 skillId;
    BtlUnit *entryUnit = command->task->unit;
    BtlUnit *unit;

    memcpy(groupChoices, D_003BB670, sizeof(groupChoices));
    initialFlags = entryUnit->status.flags;
    memcpy(pathChoices, D_003BB678, sizeof(pathChoices));
    memcpy(sideChoices, D_003BB680, sizeof(sideChoices));
    if (initialFlags & 0x400) {
        btlActionAimUserAtTargets(command, &command->frontCamera, &command->backCamera);
        command->motionParameter = 20.0f;
        command->flags |= 0x41;
        return;
    }
    memset(CURSOR, 0, sizeof(*CURSOR));
    skillId = command->task->indexWork.skillId;
    switch (skillId) {
    case 0x1D4:
        CURSOR->scriptSetIndex = 1;
        CURSOR->unk_0E = 0;
        if (btlCanUseLinkedActor(command) == 0) {
            CURSOR->scriptIndex = 0x1E;
        } else {
            CURSOR->scriptIndex = 0x21;
        }
        btlBuildPresetCameraPose(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
        break;
    case 0x1D3:
        CURSOR->scriptSetIndex = 1;
        CURSOR->scriptIndex = 0x1A;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
        break;
    case 0x1DC:
        CURSOR->scriptSetIndex = 2;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        CURSOR->scriptIndex = 0x17;
        func_001E6668(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
        break;
    case 0x1BB:
        /* Keep the selected path live across preset initialization; see
         * docs/idioms.md, "Trace-justified skill-camera path selection". */
        pathSet = 3;
        CURSOR->scriptIndex = 0x1B;
        CURSOR->scriptSetIndex = pathSet;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1BF:
        CURSOR->scriptSetIndex = 2;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        CURSOR->scriptIndex = 0x16;
        func_001E6668(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
        break;
    case 0x1D2:
        CURSOR->scriptSetIndex = 2;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        CURSOR->scriptIndex = pathChoices[(s16)btlNextScaledRandom(4)];
        func_001E6668(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
        break;
    case 0x1D7:
        CURSOR->scriptSetIndex = 2;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, groupChoices[(s16)btlNextScaledRandom(2)]);
        if (btlCanUseLinkedActor(command) == 0) {
            CURSOR->scriptIndex = 0x1E;
        } else {
            CURSOR->scriptIndex = 0xB;
        }
        func_001E6668(command, pose, 0, 4);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
        break;
    case 0x1CF:
        CURSOR->scriptSetIndex = 3;
        CURSOR->scriptIndex = 0x1D;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1D1:
        CURSOR->scriptSetIndex = 3;
        CURSOR->scriptIndex = 0x1C;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1DD:
        CURSOR->scriptSetIndex = 3;
        CURSOR->scriptIndex = 0x15;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1DE:
        CURSOR->scriptSetIndex = 3;
        CURSOR->scriptIndex = 8;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1E0:
        CURSOR->scriptSetIndex = 3;
        CURSOR->scriptIndex = 2;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[(s16)btlNextScaledRandom(2)]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1E1:
    case 0x1E2:
    case 0x1E3:
    case 0x1E7:
    case 0x1E8:
    case 0x1E9:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        unit = command->task->unit;
        if (unit->lookupId > command->linkedA->lookupId) {
            orientation = !btlHasFlag(unit->status.flags, 0x1000);
        } else {
            orientation = btlHasFlag(unit->status.flags, 0x1000);
        }
        if (orientation == 0) {
            CURSOR->scriptIndex = 0x1F;
            choice = 0;
        } else {
            CURSOR->scriptIndex = 0x20;
            choice = 1;
        }
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[choice]);
        func_001E6668(command, pose, 0, 5);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
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
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        unit = command->task->unit;
        if (unit->lookupId > command->linkedA->lookupId) {
            orientation = !btlHasFlag(unit->status.flags, 0x1000);
        } else {
            orientation = btlHasFlag(unit->status.flags, 0x1000);
        }
        if (orientation == 0) {
            CURSOR->scriptIndex = 0x13;
            choice = 0;
        } else {
            CURSOR->scriptIndex = 0x14;
            choice = 1;
        }
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[choice]);
        func_001E6668(command, pose, 0, 5);
        btlClearAllUnitDefeatCandidatesTask();
        btlFlagLinkedGroupDefeatCandidatesTask(command);
        break;
    case 0x1B3:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[1]);
        CURSOR->scriptIndex = 0x11;
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1B7:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[1]);
        CURSOR->scriptIndex = 0x12;
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1C3:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[1]);
        CURSOR->scriptIndex = 0xE;
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1D5:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        switch (command->task->unit->lookupId) {
        case 0: choice = 0; break;
        case 2: choice = 1; break;
        default: choice = (s16)btlNextScaledRandom(2); break;
        }
        CURSOR->scriptIndex = choice + 0xC;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[choice]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1D6:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        switch (command->task->unit->lookupId) {
        case 0: CURSOR->scriptIndex = 9; break;
        case 2: CURSOR->scriptIndex = 10; break;
        default: CURSOR->scriptIndex = choice + 9; break;
        }
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[choice]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1DA:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        switch (command->task->unit->lookupId) {
        case 0: CURSOR->scriptIndex = 0xF; break;
        case 2: CURSOR->scriptIndex = 0x10; break;
        default: CURSOR->scriptIndex = choice + 0xF; break;
        }
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[choice]);
        func_001E6668(command, pose, 0, 5);
        break;
    case 0x1D8:
    case 0x1DF:
        CURSOR->scriptSetIndex = 4;
        CURSOR->scriptIndex = 5;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        choice += 8;
        btlBuildPresetCameraPose(command, pose, 1, choice);
        func_001E6668(command, pose, 0, 0xC);
        break;
    default:
        CURSOR->scriptSetIndex = 3;
        CURSOR->unk_0E = 0;
        choice = (s16)btlNextScaledRandom(2);
        CURSOR->scriptIndex = 0x19 - choice;
        btlBuildPresetCameraPose(command, pose, 1, sideChoices[choice]);
        func_001E6668(command, pose, 0, 5);
        break;
    }
}

extern const BtlCameraTimedInstruction *D_0035DA28[];

extern const BtlCameraTimedInstruction *D_0035DAD0[];

void btlAdvancePlayerCursorAnimation(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->task->unit->status.flags & 0x400)) {
        func_001E9DE0(action, state, D_0035DA28[CURSOR->scriptSetIndex]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

/* Reset global cursor frame; flag0x400 actors select a Linkage skill preset. */
void btlSelectLinkageSkillCameraPreset(BtlLinkedCommand *action) {
    u16 selection;

    CURSOR->frame = 0;
    if (!(action->task->unit->status.flags & 0x400)) {
        return;
    }

    memset(CURSOR, 0, 0x130);
    selection = action->task->indexWork.skillId;
    switch (selection) {
    case 0x1BB:
        CURSOR->scriptIndex = 0x1B;
        CURSOR->unk_0E = 0;
        break;
    case 0x1BF:
        CURSOR->scriptIndex = 0x16;
        CURSOR->unk_0E = 0;
        break;
    case 0x1B3:
        CURSOR->scriptIndex = 0x11;
        CURSOR->unk_0E = 0;
        break;
    case 0x1B7:
        CURSOR->scriptIndex = 0x22;
        CURSOR->unk_0E = 0;
        break;
    case 0x1C3:
        CURSOR->scriptIndex = 0x23;
        CURSOR->unk_0E = 0;
        break;
    }
}

extern const BtlCameraTimedInstruction *D_0035DA40[];

/* Evaluate the skill camera script, apply its pose, then advance the cursor. */
void btlAdvanceSkillCameraCursor(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->task->unit->status.flags & 0x400)) {
        func_001E9DE0(action, state, D_0035DA40[CURSOR->scriptIndex]);
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
            func_001E9DE0(action, state, D_0035DA40[CURSOR->scriptIndex]);
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

/* Choose a camera preset by marked-party count, actor category and random draw. */
void btlInitializeRandomCameraCursor(BtlLinkedCommand *action, BtlCamState *state) {
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

void btlAdvanceCommandCursor(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->mode == 0) {
        func_001E9DE0(action, state, D_0035D9F0[CURSOR->index]);
    } else {
        func_001E9DE0(action, state, D_0035DA08[CURSOR->index]);
    }
    func_001EB368(action, state);
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

extern BtlCameraPathPreset D_00359EF0[18];
extern BtlCameraPathPreset D_0035A5B0[2];
extern BtlCameraPathPreset D_0035A670[2];
extern void btlUnitGetPosVU(BtlUnit *, u8);

/* vu0 routine: initialize the selected battle-camera path preset. */
void func_001E6668(BtlLinkedCommand *action, BtlCamState *camera,
                   s32 bank, s32 presetIndex) {
    BtlCameraPathPreset *preset;
    BtlIndexList *indices;
    BtlTask *task;
    /* The endpoint output and scalar input use successive float-data addresses.
     * camera_preset_float_data_probe (CSE1, r91) shows that the shared temporary
     * expires the endpoint address before the cursor-root reload. */
    f32 *data;
    f32 endpoint[4] __attribute__((aligned(16)));
    f32 angle;
    f32 distance;
    f32 extraDistance;
    f32 savedDistance;
    f32 span;
    u32 count;

    switch (bank) {
    case 0:
        preset = &D_00359EF0[presetIndex];
        break;
    case 2:
        preset = &D_0035A5B0[presetIndex];
        break;
    case 4:
        preset = &D_0035A670[presetIndex];
        break;
    default:
        preset = &D_00359EF0[presetIndex];
        break;
    }

    angle = preset->angleScale * 0x1.1df46ap-6f * 0x1.555554p+0f * 0.5f;
    CURSOR->kind = preset->kind;

    switch (preset->operation) {
    case 1:
        task = action->task;
        indices = btlAllocateIndexList(13);
        btlBuildEligibleTargetIndexList(task, indices, 1, 0, 0);
        count = btlGetIndexListCount(indices);

        btlUnitGetPosVU((BtlUnit *)indices->entries[0], 0);
        VU0_STORE_VF(vf10, endpoint);
        endpoint[0] += preset->firstOffset[0];
        endpoint[1] += preset->firstOffset[1];
        endpoint[2] += preset->firstOffset[2];
        VU0_LOAD_VF(vf10, endpoint);
        VU0_STORE_VF(vf10, CURSOR->pathEnd);

        btlUnitGetPosVU((BtlUnit *)indices->entries[count - 1], 0);
        VU0_STORE_VF(vf10, endpoint);
        endpoint[0] += preset->secondOffset[0];
        endpoint[1] += preset->secondOffset[1];
        endpoint[2] += preset->secondOffset[2];
        VU0_LOAD_VF(vf10, endpoint);
        VU0_STORE_VF(vf10, CURSOR->pathStart);
        btlFreeIndexList(indices);
        break;

    case 2:
        span = btlMeasureMaskedUnitBounds(0x200, NULL, NULL);
        /* Retail retains this discarded vector subtraction before zero-offset
         * framing: DDS1 001E6894..001E68B0, DDS2 001F554C..001F5568. */
        VU0_STORE_VF(vf10, endpoint);
        VU0_LOAD_VF(vf10, endpoint);
        VU0_LOAD_VF(vf11, camera->position);
        VU0_SUB(vf10, vf10, vf11);
        extraDistance = 0.0f;
        distance = (span + extraDistance) / tanf(angle);
        VU0_LOAD_VF(vf10, camera->position);
        VU0_STORE_VF(vf10, endpoint);
        endpoint[0] += preset->firstOffset[0];
        endpoint[1] += preset->firstOffset[1];
        endpoint[2] += preset->firstOffset[2];
        VU0_LOAD_VF(vf10, endpoint);
        VU0_STORE_VF(vf10, CURSOR->pathEnd);
        if (distance >= 1000.0f) {
            extraDistance = distance - 1000.0f;
            VU0_LOAD_VF(vf10, camera->direction);
            VU0_NEGATE_XYZ(vf10);
            VU0_SCALE_VF(vf10, extraDistance);
            VU0_LOAD_VF(vf11, endpoint);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, endpoint);
            camera->distance += extraDistance;
            VU0_LOAD_VF(vf10, endpoint);
            VU0_STORE_VF(vf10, CURSOR->pathEnd);
            distance = 1000.0f;
        }
        VU0_LOAD_VF(vf10, camera->direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, distance);
        VU0_LOAD_VF(vf11, endpoint);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, endpoint);
        endpoint[0] += preset->secondOffset[0];
        endpoint[1] += preset->secondOffset[1];
        endpoint[2] += preset->secondOffset[2];
        VU0_LOAD_VF(vf10, endpoint);
        data = CURSOR->pathStart;
        VU0_STORE_VF(vf10, data);
        data = &camera->distance;
        CURSOR->kind = 0;
        CURSOR->distance = *data;
        break;

    case 3:
        VU0_LOAD_VF(vf10, camera->direction);
        VU0_NEGATE_XYZ(vf10);
        savedDistance = camera->distance;
        VU0_SCALE_VF(vf10, savedDistance);
        VU0_LOAD_VF(vf11, camera->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, endpoint);
        VU0_STORE_VF(vf10, CURSOR->pathEnd);

        VU0_LOAD_VF(vf10, camera->direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALE_VF(vf10, preset->firstOffset[0]);
        VU0_LOAD_VF(vf11, CURSOR->pathEnd);
        VU0_ADD(vf10, vf10, vf11);
        data = CURSOR->pathStart;
        VU0_STORE_VF(vf10, data);
        data = &camera->distance;
        CURSOR->distance = *data;
        VU0_LOAD_VF(vf10, camera->direction);
        VU0_STORE_VF(vf10, CURSOR->direction);
        break;
    }

    CURSOR->startFrame = preset->startFrame;
    CURSOR->duration = preset->duration;
    CURSOR->parameterD0 = preset->unk30;
    CURSOR->parameterE0 = preset->unk40;
    CURSOR->blendRate = preset->blendRate;
    CURSOR->rangeStart = preset->firstOffset[0];
    CURSOR->rangeEnd = preset->secondOffset[0];
}

/* Camera clearance for a unit: mode 1 is the scaled camera radius plus any
 * negative horizontal offset; mode 0 is the larger of the scaled reach plus the
 * offset and half the scaled height plus any negative vertical offset. */
f32 btlGetUnitCameraClearance(BtlUnit *unit, u8 mode, f32 horizontalOffset, f32 verticalOffset) {
    f32 result = 0.0f;
    f32 verticalBias = 0.0f;
    f32 clearance = 0.0f;
    f32 halfHeight;

    if (mode != 0) {
        if (mode == 1) {
            if (horizontalOffset < 0.0f) {
                clearance = -horizontalOffset;
            }
            /* The return sits in a one-pass loop, which retail's register
             * choice for the compare needs. */
            do {
                return unit->cameraRadius * unit->scale + clearance;
            } while (0);
        }
    } else {
        if (verticalOffset < 0.0f) {
            verticalBias = -verticalOffset;
        }
        clearance = unit->height * unit->scale;
        verticalBias += clearance;
        halfHeight = verticalBias * 0.5f;
        result = unit->reach * unit->scale + horizontalOffset;
        if (result < halfHeight) {
            clearance = halfHeight;
        } else {
            clearance = result;
        }
        return clearance;
    }
    return result;
}

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

extern f32 btlGetUnitCameraClearance(BtlUnit *, u8, f32, f32);

extern BtlUnit *btlFindActiveActorById(s32 id);

extern u32 btlCountUnitsByFlags(u32);

extern f32 func_001ED5C8(s32, f32 *, f32 *, const f32 *, const f32 *, const f32 *, f32);

extern f32 func_001EDB20(s32, BtlUnit *, BtlUnit *, s8, f32 *, f32 *, const f32 *, const f32 *);

extern f32 func_001EE160(s32, BtlUnit *, BtlUnit *, s8, f32 *, f32 *, const f32 *, const f32 *, s8);

extern f32 btlBuildProjectedActorPairBoundsVU(s32, BtlUnit *, BtlUnit *, s8, s8, f32 *, f32 *, const f32 *, const f32 *);

extern f32 D_0035F590[4];

extern f32 D_0035F5A0[4];

extern f32 D_0035F5B0[4];

extern f32 D_003BB690;

/* Select a camera policy, construct its focus and eye, and initialize the
 * optional endpoint used by the camera cursor. */
INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4668);

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
        span = btlGetUnitCameraClearance(unit, p->focusPolicy.mode,
                            p->focusPolicy.distance, p->focusPolicy.height);
        framingDistance = span / tanf(halfFov);
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
        span = btlGetUnitCameraClearance(unit, p->focusPolicy.mode,
                            p->focusPolicy.distance, p->focusPolicy.height);
        framingDistance = span / tanf(halfFov);
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
            btlMeasureMaskedUnitBounds(0x200, NULL, NULL);
            VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        } else {
            btlBuildProjectedActorPairBoundsVU(0x200, action->task->unit, action->linkedA,
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
        span = btlGetUnitCameraClearance(action->targetList->entries[0],
                            p->focusPolicy.mode, 0.0f, 0.0f);
        framingDistance = span / tanf(halfFov);
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
        framingDistance = span / tanf(halfFov);
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
        btlMeasureMaskedUnitBounds(0x400, NULL, NULL);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        span = func_001ED5C8(boundsMask, NULL, NULL, NULL, NULL,
                            terminalPoint, 600.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        framingDistance = span / tanf(halfFov);
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
        span = btlGetUnitCameraClearance(unit, p->focusPolicy.mode,
                            p->focusPolicy.distance, p->focusPolicy.height);
        framingDistance = span / tanf(halfFov);
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
            btlMeasureMaskedUnitBounds(0x400, NULL, NULL);
            VU0_STORE_VF_UNCLOBBERED(vf10, destination);
            terminalPoint[0] = destination[0];
        }
        span = func_001ED5C8(0x400, NULL, NULL, NULL, NULL,
                            terminalPoint, 600.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        framingDistance = span / tanf(halfFov);
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
        span = btlGetUnitCameraClearance(unit, p->focusPolicy.mode, 1.0f, 1.0f);
        framingDistance = span / tanf(halfFov);
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
        span = btlBuildProjectedActorPairBoundsVU(0x600, unit, action->task->unit,
                            (s8)p->focusPolicy.mode, (s8)p->eyePolicy.mode,
                            0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        framingDistance = span / tanf(halfFov);
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
        span = btlBuildProjectedActorPairBoundsVU(0x600, unit, otherUnit,
                            (s8)p->focusPolicy.mode, (s8)p->eyePolicy.mode,
                            0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, focus);
        VU0_STORE_VF_UNCLOBBERED(vf10, out->position);
        framingDistance = span / tanf(halfFov);
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
        measuredDistance = span / tanf(halfFov);
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
        btlMeasureMaskedUnitBounds(centerMask, 0, 0);
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
        measuredDistance = span / tanf(halfFov);
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
        measuredDistance = span / tanf(halfFov);
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
        span = btlMeasureMaskedUnitBounds(0x200, 0, 0);
        measuredDistance = span / tanf(halfFov);
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
            span = btlMeasureMaskedUnitBounds(0x200, 0, 0);
            measuredDistance = span / tanf(halfFov);
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
            span = btlBuildProjectedActorPairBoundsVU(0x200, action->task->unit, action->linkedA,
                                (s8)p->eyePolicy.mode, (s8)p->eyePolicy.mode, 0, 0, 0, 0);
            measuredDistance = span / tanf(halfFov);
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
        btlMeasureMaskedUnitBounds(mask, 0, 0);
        tanf(halfFov); /* scalar ignored, call exists, vf10 preserved */
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
        measuredDistance = span / tanf(halfFov);
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
        actorExtent = btlGetUnitCameraClearance(unit, p->eyePolicy.mode, 1.0f, 1.0f);
        framingDistance = actorExtent / tanf(halfFov);
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
        span = btlBuildProjectedActorPairBoundsVU(0x600, action->task->unit, otherUnit,
                            (s8)p->eyePolicy.mode, (s8)p->eyePolicy.mode, 0, 0, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, destination);
        measuredDistance = span / tanf(halfFov);
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
        span = btlMeasureMaskedUnitBounds(mask, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        measuredDistance = span / tanf(halfFov);
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
        span = btlMeasureMaskedUnitBounds(mask, 0, 0);
        measuredDistance = span / tanf(halfFov);
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
            btlMeasureMaskedUnitBounds(0x400, 0, 0);
            VU0_STORE_VF_UNCLOBBERED(vf10, destination);
            terminalPoint[0] = destination[0];
        }
        span = func_001ED5C8(0x400, 0, 0, focus, eye, terminalPoint, p->boundsRadius);
        measuredDistance = span / tanf(halfFov);
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
        btlMeasureMaskedUnitBounds(0x400, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint);
        VU0_LOAD_VF(vf10, angles);
        VU0_STORE_VF_UNCLOBBERED(vf10, terminalPoint); /* the first store is intentionally overwritten */
        boundsMask = 0x600;
        func_001ED5C8(boundsMask, 0, 0, 0, 0, terminalPoint, 600.0f);
        VU0_STORE_VF_UNCLOBBERED(vf10, eye);
        tanf(halfFov); /* native ignored scalar */
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
        measuredDistance = span / tanf(halfFov);
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
        btlMeasureMaskedUnitBounds(mask, 0, 0);
        tanf(halfFov); /* ignored scalar, vf10 survives */
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
        btlMeasureMaskedUnitBounds(0x600, 0, 0);
        tanf(halfFov); /* ignored scalar, vf10 survives */
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

extern void btlApplyScriptCameraTrack(BtlLinkedCommand *, BtlCamState *,
                         const BtlCameraTimedInstruction *, s32 *,
                         const BtlCameraParameterRecord *);
extern void btlApplyRadialLinkageFormation(BtlLinkedCommand *);

typedef struct BtlCameraStoredPose {
    BtlCamState pose;
    u8 pad28[8];
} BtlCameraStoredPose;

typedef struct BtlCameraParameterSnapshot {
    f32 pathEnd[4];
    f32 pathStart[4];
    f32 eyeFrom[4];
    f32 eyeTo[4];
    f32 focusFrom[4];
    f32 focusTo[4];
    f32 direction[4];
    f32 distance;
    u8 pad74[0xC];
    f32 angleVector[4];
    f32 startFrame;
    u8 pad94[0xC];
    f32 duration;
    u8 padA4[0xC];
    u8 parameterD0;
    u8 padB1[0xF];
    f32 parameterE0;
    u8 padC4[0xC];
    f32 blendRate;
    u8 padD4[0xC];
    f32 rangeStart;
    u8 padE4[0xC];
    f32 rangeEnd;
    u8 padF4[0xC];
} BtlCameraParameterSnapshot;

typedef char CameraSnapshotExtent[(sizeof(BtlCameraParameterSnapshot) == 0x100) ? 1 : -1];
typedef char CameraStoredPoseExtent[(sizeof(BtlCameraStoredPose) == 0x30) ? 1 : -1];
extern BtlCameraParameterSnapshot D_0035F230;
extern BtlCameraParameterSnapshot D_0035F330;
extern BtlCameraStoredPose D_0035F430;
extern BtlCameraStoredPose D_0035F560;
extern BtlCameraParameterSnapshot D_0035F460;
extern BtlCameraParameterRecord D_0035E000[];
extern BtlCameraParameterRecord D_0035E080[];
extern BtlCameraParameterRecord D_0035DD00[];
extern BtlCameraParameterRecord D_0035E100[];
extern BtlCameraParameterRecord D_0035E480[];
extern BtlCameraParameterRecord D_0035E780[];
extern BtlCameraParameterRecord D_0035E800[];
extern BtlCameraParameterRecord D_0035DB00[];
extern BtlCameraParameterRecord D_0035EB00[];
extern BtlCameraParameterRecord D_0035F080[];
extern s16 D_0035D800[];





typedef char CameraCursorExtent[(sizeof(BtlCameraCursorView) == 0x130) ? 1 : -1];
typedef char CameraCursorRestoreOffset[((u32)&((BtlCameraCursorView *)0)->restoreAlternate == 0x15) ? 1 : -1];
typedef char CameraCursorD0Offset[((u32)&((BtlCameraCursorView *)0)->parameterD0 == 0xD0) ? 1 : -1];
typedef char CameraCursorE0Offset[((u32)&((BtlCameraCursorView *)0)->parameterE0 == 0xE0) ? 1 : -1];
typedef char CameraCursorIndexOffset[((u32)&((BtlCameraCursorView *)0)->parameterIndex12C == 0x12C) ? 1 : -1];



void func_001E9DE0(BtlLinkedCommand *action, BtlCamState *pose,
                   const BtlCameraTimedInstruction *instruction) {
    s32 done = 0;
    s32 kind;
    u32 upper;
    u32 lower;
    s32 random;
    f32 distance;
    s16 signedIndex;
    BtlUnit *unit;
    BtlUnit *target;
    BtlCameraParameterSnapshot *savedParameters;
    BtlCameraStoredPose *savedPose;
    f32 quaternion[4];
    f32 oldFocus[4];
    f32 actorPosition[4];
    f32 facing[4];
    f32 forward[4];

    memset(forward, 0, sizeof(forward));
    forward[2] = -1.0f;
    do {
        kind = instruction->kind;

        switch (kind) {
        case 0:
            CURSOR->parameterIndex114 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex114, D_0035E000);
            break;
        case 1:
            CURSOR->parameterIndex118 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex118, D_0035E080);
            break;
        case 2:
            CURSOR->parameterIndex11C = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex11C, D_0035DD00);
            break;
        case 3:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex120, D_0035E100);
            break;
        case 10:
        case 11:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex120, D_0035E480);
            break;
        case 12:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex120, D_0035E780);
            break;
        case 13:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex120, D_0035E800);
            break;
        case 14:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlClearRuntimeFlag2000();
            }
            ++instruction;
            break;
        case 16:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                ++instruction;
                btlClearAllUnitDefeatCandidatesTask();
                btlFlagLinkedGroupDefeatCandidatesTask(action);
            } else {
                ++instruction;
            }
            break;
        case 17:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                if (action->task->unit->status.flags & 0x200) {
                    btlFlagMatchingUnitsDefeatCandidate(0x200);
                } else {
                    btlFlagMatchingUnitsDefeatCandidate(0x400);
                }
            }
            ++instruction;
            break;
        case 18:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                if (action->task->unit->status.flags & 0x200) {
                    btlFlagMatchingUnitsDefeatCandidate(0x400);
                } else {
                    btlFlagMatchingUnitsDefeatCandidate(0x200);
                }
            }
            ++instruction;
            break;
        case 39:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                /* Native integer-address hook transports this command address unchanged. */
                if (btlRunTimedCameraSetupHook((s32)action) != 0) {
                    CURSOR->busy = 1;
                }
            }
            if (CURSOR->busy == 0) {
                ++instruction;
                break;
            }
            ++instruction;
            done = 1;
            break;
        case 43:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                /* Native integer-address hook transports this command address unchanged. */
                if (btlRunTimedCameraFinishHook((s32)action) != 0) {
                    CURSOR->busy = 1;
                }
            }
            if (CURSOR->busy == 0) {
                ++instruction;
            } else {
                ++instruction;
                done = 1;
            }
            break;

        case 37:
        case 38:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                unit = ((BtlState *)btlGetRuntime())->units;
                if (action->task->unit->lookupId == 1) {
                    while (unit != NULL) {
                        if ((btlUnitStatusPair(unit) & 0x201) == 0x201 &&
                            unit->lookupId != action->task->unit->lookupId) {
                            if (instruction->kind == 37) {
                                lower = unit->lookupId;
                                upper = action->task->unit->lookupId;
                            } else {
                                lower = action->task->unit->lookupId;
                                upper = unit->lookupId;
                            }
                            if (lower < upper &&
                                ((unit->status.flags & 0x20) != 0 ||
                                 (unit->partyRecord.status & 0x7FFF) != 0)) {
                                btlClearUnitDefeatCandidate(unit);
                            }
                        }
                        unit = unit->next;
                    }
                }
            }
            ++instruction;
            break;

        case 34:
        case 35:
        case 36:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                unit = ((BtlState *)btlGetRuntime())->units;
                while (unit != NULL) {
                    if ((btlUnitStatusPair(unit) & 0x201) == 0x201 &&
                        unit->lookupId != action->task->unit->lookupId) {
                        switch ((u32)instruction->kind) {
                        case 34:
                            lower = unit->lookupId;
                            upper = action->task->unit->lookupId;
                            break;
                        case 35:
                            lower = action->task->unit->lookupId;
                            upper = unit->lookupId;
                            break;
                        case 36:
                        default:
                            if (CURSOR->counter == 0) {
                                lower = action->task->unit->lookupId;
                                upper = unit->lookupId;
                            } else {
                                lower = unit->lookupId;
                                upper = action->task->unit->lookupId;
                            }
                            break;
                        }
                        if (lower < upper) {
                            btlClearUnitDefeatCandidate(unit);
                        }
                    }
                    unit = unit->next;
                }
            }
            ++instruction;
            break;
        case 19:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlFlagMatchingUnitsDefeatCandidate(0x600);
            }
            ++instruction;
            break;
        case 20:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                func_001E6668(action, pose, 0, instruction->parameterIndex);
            }
            ++instruction;
            break;
        case 21:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                CURSOR->kind = 0;
            }
            ++instruction;
            break;
        case 15:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                ++instruction;
                btlClearAllUnitDefeatCandidatesTask();
                btlApplyCombinedActorFlags((u8 *)action);
            } else {
                ++instruction;
            }
            break;
        case 4:
            CURSOR->parameterIndex124 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex124, D_0035DB00);
            break;
        case 5:
            CURSOR->parameterIndex128 = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex128, D_0035EB00);
            break;
        case 8:
            CURSOR->parameterIndex12C = instruction->parameterIndex;
            btlApplyScriptCameraTrack(action, pose, instruction++, &CURSOR->parameterIndex12C, D_0035F080);
            break;
        case 6:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlBuildPresetCameraPose(action, pose, 0, instruction->parameterIndex);
            }
            ++instruction;
            break;
        case 22:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlApplyRadialLinkageFormation(action);
            }
            ++instruction;
            break;
        case 31:
        case 32:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                ((f32)CURSOR->frame < instruction->startFrame + instruction->duration ||
                 instruction->duration == 1000.0f)) {
                unit = (kind == 31) ? btlGetIndexListEntry(action->targetList, 0) : action->task->unit;
                btlUnitGetPosVU(unit, 1);
                VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                VU0_LOAD_VF(vf10, pose->direction);
                VU0_NEGATE_XYZ(vf10);
                VU0_SCALE_VF(vf10, pose->distance);
                VU0_LOAD_VF(vf11, pose->position);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, oldFocus);
                VU0_LOAD_VF(vf10, actorPosition);
                VU0_STORE_VF_UNCLOBBERED(vf10, pose->position);
                VU0_LOAD_VF(vf11, oldFocus);
                VU0_SUB(vf10, vf10, vf11);
                VU0_LENGTH_VF10(distance);
                pose->distance = distance;
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
            }
            ++instruction;
            break;

        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                switch (kind) {
                case 26:
                    unit = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
                    break;
                case 27:
                case 30:
                    unit = action->task->unit;
                    break;
                case 28:
                    unit = action->linkedA;
                    break;
                case 29:
                default:
                    unit = action->linkedB;
                    break;
                }
                if (unit->status.flags & 0x80000) {
                    btlUnitGetPosVU(unit, 0);
                    VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                    btlCopyUnitRotationQuaternion(unit, quaternion);
                    VU0_LOAD_VF(vf10, quaternion);
                    effMiscQuaternionToMatrixVU();
                    VU0_LOAD_VF(vf10, forward);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_SCALE_VF(vf10, 1.0f);
                    VU0_LOAD_VF(vf11, actorPosition);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, oldFocus);
                    switch ((u32)instruction->kind) {
                    case 26:
                        btlUnitGetPosVU(action->task->unit, 0);
                        VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                        break;
                    case 27:
                    case 28:
                    case 29:
                        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
                        btlUnitGetPosVU(target, 0);
                        VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                        break;
                    case 30:
                        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
                        if (target->status.flags & 0x400) {
                            btlMeasureMaskedUnitBounds(0x400, NULL, NULL);
                        } else {
                            btlMeasureMaskedUnitBounds(0x200, NULL, NULL);
                        }
                        VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                        break;
                    }
                    btlAimHorizontalDirectionVU(oldFocus, actorPosition);
                    VU0_STORE_VF_UNCLOBBERED(vf10, facing);
                    btlSetUnitRotation(unit, (s128 *)facing);
                }
            }
            ++instruction;
            break;

        case 25:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlClearAllUnitDefeatCandidatesTask();
                switch (instruction->parameterIndex) {
                case 0:
                    btlFlagUnitDefeatCandidate(action->task->unit);
                    break;
                case 4:
                    btlFlagUnitDefeatCandidate((BtlUnit *)btlGetIndexListEntry(action->targetList, 0));
                    break;
                }
            }
            ++instruction;
            break;
        case 24:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                switch (instruction->parameterIndex) {
                case 0:
                    btlFlagUnitDefeatCandidate(action->task->unit);
                    break;
                case 4:
                    btlFlagUnitDefeatCandidate((BtlUnit *)btlGetIndexListEntry(action->targetList, 0));
                    break;
                }
            }
            ++instruction;
            break;
        case 23:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlClearUnitDefeatCandidate(action->task->unit);
            }
            ++instruction;
            break;
        case 7:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlBuildPresetCameraPose(action, pose, 1, instruction->parameterIndex);
            }
            ++instruction;
            break;
        case 40:
        case 42:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                if (kind == 42) {
                    if (btlCanUseLinkedActor(action) == 0) {
                        CURSOR->restoreAlternate = 1;
                    } else {
                        CURSOR->restoreAlternate = 0;
                        ++instruction;
                        break;
                    }
                }
                VU0_LOAD_VF(vf10, CURSOR->pathEnd);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.pathEnd);
                VU0_LOAD_VF(vf10, CURSOR->pathStart);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.pathStart);
                VU0_LOAD_VF(vf10, CURSOR->eyeFrom);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.eyeFrom);
                VU0_LOAD_VF(vf10, CURSOR->eyeTo);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.eyeTo);
                VU0_LOAD_VF(vf10, CURSOR->focusFrom);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.focusFrom);
                VU0_LOAD_VF(vf10, CURSOR->focusTo);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.focusTo);
                VU0_LOAD_VF(vf10, CURSOR->direction);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.direction);
                VU0_LOAD_VF(vf10, &CURSOR->fov);
                VU0_STORE_VF_UNCLOBBERED(vf10, D_0035F230.angleVector);
                D_0035F230.distance = CURSOR->distance;
                D_0035F230.startFrame = CURSOR->startFrame;
                D_0035F230.duration = CURSOR->duration;
                D_0035F230.parameterD0 = CURSOR->parameterD0;
                D_0035F230.parameterE0 = CURSOR->parameterE0;
                D_0035F230.blendRate = CURSOR->blendRate;
                D_0035F230.rangeStart = CURSOR->rangeStart;
                D_0035F230.rangeEnd = CURSOR->rangeEnd;
                if (instruction->kind == 40) {
                    savedParameters = &D_0035F330;
                    savedPose = &D_0035F430;
                } else {
                    savedParameters = &D_0035F460;
                    savedPose = &D_0035F560;
                }
                memcpy(savedPose, pose, sizeof(*savedPose));
                btlBuildPresetCameraPose(action, &savedPose->pose, 1, instruction->parameterIndex);

                    VU0_LOAD_VF(vf10, CURSOR->pathEnd);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->pathEnd);
                    VU0_LOAD_VF(vf10, CURSOR->pathStart);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->pathStart);
                    VU0_LOAD_VF(vf10, CURSOR->eyeFrom);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->eyeFrom);
                    VU0_LOAD_VF(vf10, CURSOR->eyeTo);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->eyeTo);
                    VU0_LOAD_VF(vf10, CURSOR->focusFrom);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->focusFrom);
                    VU0_LOAD_VF(vf10, CURSOR->focusTo);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->focusTo);
                    VU0_LOAD_VF(vf10, CURSOR->direction);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->direction);
                    VU0_LOAD_VF(vf10, &CURSOR->fov);
                    VU0_STORE_VF_UNCLOBBERED(vf10, savedParameters->angleVector);
                    savedParameters->distance = CURSOR->distance;
                    savedParameters->startFrame = CURSOR->startFrame;
                    savedParameters->duration = CURSOR->duration;
                    savedParameters->parameterD0 = CURSOR->parameterD0;
                    savedParameters->parameterE0 = CURSOR->parameterE0;
                    savedParameters->blendRate = CURSOR->blendRate;
                    savedParameters->rangeStart = CURSOR->rangeStart;
                    savedParameters->rangeEnd = CURSOR->rangeEnd;
                VU0_LOAD_VF(vf10, D_0035F230.pathEnd);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->pathEnd);
                VU0_LOAD_VF(vf10, D_0035F230.pathStart);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->pathStart);
                VU0_LOAD_VF(vf10, D_0035F230.eyeFrom);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->eyeFrom);
                VU0_LOAD_VF(vf10, D_0035F230.eyeTo);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->eyeTo);
                VU0_LOAD_VF(vf10, D_0035F230.focusFrom);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->focusFrom);
                VU0_LOAD_VF(vf10, D_0035F230.focusTo);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->focusTo);
                VU0_LOAD_VF(vf10, D_0035F230.direction);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->direction);
                VU0_LOAD_VF(vf10, D_0035F230.angleVector);
                VU0_STORE_VF_UNCLOBBERED(vf10, &CURSOR->fov);
                CURSOR->distance = D_0035F230.distance;
                CURSOR->startFrame = D_0035F230.startFrame;
                CURSOR->duration = D_0035F230.duration;
                CURSOR->parameterD0 = D_0035F230.parameterD0;
                CURSOR->parameterE0 = D_0035F230.parameterE0;
                CURSOR->blendRate = D_0035F230.blendRate;
                CURSOR->rangeStart = D_0035F230.rangeStart;
                CURSOR->rangeEnd = D_0035F230.rangeEnd;
            }
            ++instruction;
            break;

        case 41:
            if ((instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration)) {
                if (CURSOR->restoreAlternate == 0) {
                    savedParameters = &D_0035F330;
                    savedPose = &D_0035F430;
                } else {
                    savedParameters = &D_0035F460;
                    savedPose = &D_0035F560;
                }
                memcpy(pose, savedPose, sizeof(*savedPose));

                VU0_LOAD_VF(vf10, savedParameters->pathEnd);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->pathEnd);
                VU0_LOAD_VF(vf10, savedParameters->pathStart);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->pathStart);
                VU0_LOAD_VF(vf10, savedParameters->eyeFrom);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->eyeFrom);
                VU0_LOAD_VF(vf10, savedParameters->eyeTo);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->eyeTo);
                VU0_LOAD_VF(vf10, savedParameters->focusFrom);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->focusFrom);
                VU0_LOAD_VF(vf10, savedParameters->focusTo);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->focusTo);
                VU0_LOAD_VF(vf10, savedParameters->direction);
                VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->direction);
                VU0_LOAD_VF(vf10, savedParameters->angleVector);
                VU0_STORE_VF_UNCLOBBERED(vf10, &CURSOR->fov);
                CURSOR->distance = savedParameters->distance;
                CURSOR->startFrame = savedParameters->startFrame;
                CURSOR->duration = savedParameters->duration;
                CURSOR->parameterD0 = savedParameters->parameterD0;
                CURSOR->parameterE0 = savedParameters->parameterE0;
                CURSOR->blendRate = savedParameters->blendRate;
                CURSOR->rangeStart = savedParameters->rangeStart;
                CURSOR->rangeEnd = savedParameters->rangeEnd;
            }
            ++instruction;
            break;

        case 33:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlBuildPresetCameraPose(action, pose, 2, instruction->parameterIndex);
            }
            ++instruction;
            break;

        case 9:
            if (instruction->startFrame <= (f32)CURSOR->frame) {
                if ((f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                    CURSOR->mode = 1;
                }
                random = btlNextScaledRandom(8);
                CURSOR->index = random;
                CURSOR->parameterIndex114 = 0;
                CURSOR->parameterIndex118 = 0;
                CURSOR->parameterIndex11C = 0;
                CURSOR->parameterIndex120 = 0;
                CURSOR->parameterIndex124 = 0;
                CURSOR->parameterIndex128 = 0;
                CURSOR->parameterIndex12C = 0;
                CURSOR->frame = 0;
                CURSOR->kind = 0;
                signedIndex = (s16)random;
                func_001E6668(action, pose, 0, D_0035D800[signedIndex]);
            }
            done = 1;
            break;

        case 44:
        default:
            ++instruction;
            done = 1;
            break;
        }
    } while (done == 0);
}

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

s32 func_001EB368(BtlLinkedCommand *action, BtlCamState *pose) {
    f32 point[4];
    f32 delta[4];
    f32 endpoint[4];
    f32 axis[4];
    f32 position[4];
    f32 distance;
    f32 progress;

    if (CURSOR->busy != 0) {
        return 0;
    }

    switch (CURSOR->kind) {
    case 0:
    case 2:
    default:
        break;

    case 6: {
        u16 actorCameraMode;
        BtlUnit *actor;

        BtlUnit *targetUnit = btlGetIndexListEntry(action->targetList, 0);
        BtlUnit *sourceUnit = action->task->unit;
        s8 mode40 = (s8)(s32)CURSOR->eyeFrom[0];
        s8 mode50 = (s8)(s32)CURSOR->eyeTo[0];

        distance = btlBuildProjectedActorPairBoundsVU(0x600, targetUnit, sourceUnit,
                                 mode40, mode50, NULL, NULL, NULL, NULL);
        VU0_STORE_VF_UNCLOBBERED(vf10, endpoint);
        distance /= tanf(CURSOR->fov);
        CURSOR->distance = distance;

        actor = action->task->unit;
        actorCameraMode = actor->partyRecord.unitId;
        switch (actorCameraMode) {
        case 1:
        case 3:
        case 5: {
            f32 desiredDistance;
            BtlCameraCursorView *cursor;
            s32 activePhase;
            if (pose->position[1] < endpoint[1]) {
                endpoint[1] = pose->position[1];
            }
            cursor = CURSOR;
            desiredDistance = cursor->distance;
            activePhase = 1;
            if (cursor->phase == activePhase) {
                if (desiredDistance <= pose->distance) {
                    return 1;
                }
                endpoint[0] = pose->position[0];
            }
            if (pose->distance < desiredDistance) {
                cursor->phase = activePhase;
            }
            if (desiredDistance <= 450.0f) {
                cursor->distance = 450.0f;
                desiredDistance = 450.0f;
                endpoint[0] = pose->position[0];
            }
            pose->distance = desiredDistance;
            VU0_LOAD_VF(vf10, endpoint);
            VU0_STORE_VF(vf10, pose->position);

            break;
        }
        case 4: {
            if (CURSOR->phase == 0) {
                CURSOR->counter++;
                CURSOR->counter = CURSOR->counter <= 0 ? 0 :
                                     CURSOR->counter >= 0x21 ? 0x20 : CURSOR->counter;
                if (CURSOR->counter >= 0x1A) {
                    CURSOR->phase = 2;
                }
            }
            if (pose->position[1] < endpoint[1]) {
                endpoint[1] = pose->position[1];
                if (CURSOR->phase == 0) {
                    CURSOR->phase = 1;
                }
            }
            if (CURSOR->phase == 2) {
                CURSOR->distance = pose->distance;
                endpoint[0] = pose->position[0];
                endpoint[2] = pose->position[2];
            }
            if (CURSOR->phase == 1 && pose->position[1] > endpoint[1]) {
                CURSOR->phase = 2;
            }
            pose->distance = CURSOR->distance;
            VU0_LOAD_VF(vf10, endpoint);
            VU0_STORE_VF(vf10, pose->position);
            break;
        }
        }

        break;
    }

    case 7: {
        f32 focusDistance;
        f32 motionParameter = CURSOR->blendRate;

        action->motionParameter = motionParameter;
        if (CURSOR->startFrame == (f32)CURSOR->frame) {
            action->durationFrames = (s32)motionParameter;
            btlScalarRangeInitQuadratic(&action->quadraticRange,
                                        (f32)(action->durationFrames * 2));
        }
        progress = btlScalarRangeStepQuadratic(&action->quadraticRange, 1.0f);
        if (progress >= 0.999999f) {
            progress = 0.999999f;
        }

        VU0_LOAD_VF(vf10, CURSOR->focusTo);
        VU0_LOAD_VF(vf11, CURSOR->focusFrom);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(distance);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, axis);
        VU0_LOAD_VF(vf10, CURSOR->focusFrom);
        VU0_STORE_VF(vf10, point);
        VU0_LOAD_VF(vf10, axis);
        VU0_SCALAR_OP(distance * CURSOR->rangeStart, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, point);

        distance *= (CURSOR->rangeEnd - CURSOR->rangeStart);
        distance *= progress;
        VU0_LOAD_VF(vf10, axis);
        VU0_SCALAR_OP(distance, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, endpoint);
        VU0_LOAD_VF(vf10, pose->position);
        VU0_LOAD_VF(vf11, endpoint);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(focusDistance);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, pose->direction);
        break;
    }

    case 1: {
        f32 timedFraction;
        f32 focusDistance;

        f32 frame = (f32)CURSOR->frame;
        if (CURSOR->startFrame <= frame &&
            frame <= CURSOR->startFrame + CURSOR->duration) {
            VU0_LOAD_VF(vf10, pose->direction);
            VU0_NEGATE_XYZ(vf10);
            VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, pose->position);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, endpoint);
            VU0_LOAD_VF(vf10, CURSOR->pathStart);
            VU0_LOAD_VF(vf11, endpoint);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(timedFraction);
            VU0_STORE_VF(vf10, delta);
            VU0_LOAD_VF(vf10, CURSOR->pathEnd);
            VU0_LOAD_VF(vf11, endpoint);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(timedFraction);
            VU0_STORE_VF(vf10, point);
            VU0_LOAD_VF(vf10, delta);
            VU0_LOAD_VF(vf11, point);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(distance);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF_UNCLOBBERED(vf10, axis);
            distance = (distance / CURSOR->duration) *
                    ((f32)CURSOR->frame - CURSOR->startFrame);
            VU0_LOAD_VF(vf10, axis);
            VU0_SCALAR_OP(distance, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, CURSOR->pathEnd);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, position);
            VU0_LOAD_VF(vf10, position);
            VU0_STORE_VF(vf10, pose->position);
            VU0_LOAD_VF(vf10, position);
            VU0_LOAD_VF(vf11, endpoint);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(focusDistance);
            pose->distance = focusDistance;
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, pose->direction);
        }
        break;
    }

    case 3:
    case 4:
    case 5: {
        u16 phaseMode;
        f32 pathLength;
        f32 focusDistance;

        VU0_LOAD_VF(vf10, pose->direction);
        VU0_NEGATE_XYZ(vf10);
        VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, pose->position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, endpoint);
        VU0_LOAD_VF(vf10, CURSOR->eyeTo);
        VU0_LOAD_VF(vf11, CURSOR->eyeFrom);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(pathLength);
        distance = pathLength;
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, axis);

        progress = CURSOR->blendRate;
        action->motionParameter = progress;
        VU0_LOAD_VF(vf10, CURSOR->eyeFrom);
        VU0_STORE_VF(vf10, point);
        phaseMode = CURSOR->kind;
        if (phaseMode == 5) {
            VU0_LOAD_VF(vf10, axis);
            VU0_SCALAR_OP(pathLength * CURSOR->rangeStart,
                          "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, point);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, point);
            distance = pathLength * (CURSOR->rangeEnd - CURSOR->rangeStart);
        }

        if (phaseMode == 4) {
            if (CURSOR->startFrame == (f32)CURSOR->frame) {
                action->durationFrames = (s32)progress;
                btlScalarRangeInitQuadratic(&action->quadraticRange,
                                            (f32)(action->durationFrames * 2));
            }
            progress = btlScalarRangeStepQuadratic(&action->quadraticRange, 1.0f);
        } else {
            if (CURSOR->startFrame == (f32)CURSOR->frame) {
                btlScalarRangeSetStartClearEnd(&action->exponentialRange,
                                               action->motionParameter);
            }
            progress = btlScalarRangeStepExponential(&action->exponentialRange);
        }
        if (progress >= 0.999999f) {
            progress = 0.999999f;
        }
        distance *= progress;
        VU0_LOAD_VF(vf10, axis);
        VU0_SCALE_VF(vf10, distance);
        VU0_LOAD_VF(vf11, point);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, pose->position);
        if (CURSOR->kind == 5) {
            VU0_LOAD_VF(vf10, CURSOR->focusTo);
            VU0_LOAD_VF(vf11, CURSOR->focusFrom);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(distance);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, axis);
            VU0_LOAD_VF(vf10, CURSOR->focusFrom);
            VU0_STORE_VF(vf10, point);
            VU0_LOAD_VF(vf10, axis);
            VU0_SCALAR_OP(distance * CURSOR->rangeStart, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, point);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, point);
            distance *= (CURSOR->rangeEnd - CURSOR->rangeStart);
            distance *= progress;
            VU0_LOAD_VF(vf10, axis);
            VU0_SCALAR_OP(distance, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, point);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, endpoint);
        }
        VU0_LOAD_VF(vf10, pose->position);
        VU0_LOAD_VF(vf11, endpoint);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(focusDistance);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, pose->direction);
        if (CURSOR->kind == 4) {
            pose->distance = focusDistance;
        }
        break;
    }

    case 8: {
        f32 focusDistance;

        f32 frame = (f32)CURSOR->frame;
        if (CURSOR->startFrame <= frame) {
            VU0_LOAD_VF(vf10, pose->direction);
            VU0_NEGATE_XYZ(vf10);
            VU0_SCALAR_OP(pose->distance, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, pose->position);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, endpoint);
            VU0_LOAD_VF(vf10, CURSOR->eyeTo);
            VU0_LOAD_VF(vf11, CURSOR->eyeFrom);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(distance);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, axis);
            progress = CURSOR->blendRate;
            action->motionParameter = progress;
            VU0_LOAD_VF(vf10, CURSOR->eyeFrom);
            VU0_STORE_VF(vf10, point);
            if (CURSOR->startFrame == (f32)CURSOR->frame) {
                btlScalarRangeSetStartClearEnd(&action->exponentialRange,
                                               action->motionParameter);
            }
            progress = btlScalarRangeStepExponential(&action->exponentialRange);
            if (progress >= 0.999999f) {
                progress = 0.999999f;
            }
            distance *= progress;
            VU0_LOAD_VF(vf10, axis);
            VU0_SCALAR_OP(distance, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_LOAD_VF(vf11, point);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, pose->position);
            VU0_LOAD_VF(vf10, pose->position);
            VU0_LOAD_VF(vf11, endpoint);
            VU0_SUB(vf10, vf10, vf11);
            VU0_LENGTH_VF10(focusDistance);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, pose->direction);
        }
        break;
    }
    }
    return 1;
}

/* Camera parameter banks are indexed with a native 0x80-byte stride. */

extern void func_001EBE88(BtlLinkedCommand *, BtlCamState *,
                         const BtlCameraTimedInstruction *, const s32 *,
                         const BtlCameraParameterRecord *, f32 *);

extern void sdfVuLoadRotationMatrixX(f32);


extern f32 D_0035F590[4];

extern f32 D_0035F5A0[4];

extern f32 D_0035F5B0[4];

extern f32 D_003BB690;

INCLUDE_ASM(const s32, "game/code_001DEFE0", func_001EBE88);

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
                    sdfVuLoadRotationMatrixX(value[0]);
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
                    sdfVuLoadRotationMatrixZ(value[1]);
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
                    sdfVuLoadRotationMatrixZ(value[1]);
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

    for (node = *(BtlUnit **)((u8 *)btlGetRuntime() + 0x228); node != 0; node = *(BtlUnit **)((u8 *)node + 0x344)) {
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

f32 func_001ED5C8(s32 sides, f32 *top, f32 *bottom, const f32 *from, const f32 *to, const f32 *extra, f32 extraRadius) {
    f32 pos[4];
    f32 target[4];
    f32 direction[4];
    f32 side[4];
    f32 seed[4];
    f32 origin[4];
    BtlState *work;
    BtlUnit *unit;
    s32 first;
    s32 pass;
    s32 valid;
    f32 high;
    f32 low;
    f32 maxX;
    f32 minX;
    f32 maxZ;
    f32 minZ;
    /* Retail retains the unused maximum at 001ED890..001ED898. */
    f32 maxRadius;
    f32 height;
    /* One radius workspace serves the point reach and framing radius, as in
     * the adjacent group-bounds loops. */
    f32 radius;
    f32 length;
    f32 x0;
    f32 x1;
    f32 z0;
    f32 z1;

    memset(origin, 0, sizeof(origin));
    work = (BtlState *)btlGetRuntime();
    if (from == NULL || to == NULL) {
        VU0_LOAD_VF_PTR(vf10, origin);
        VU0_STORE_VF_PTR(vf10, target);
    } else {
        VU0_LOAD_VF_PTR(vf10, from);
        VU0_STORE_VF_PTR(vf10, origin);
        VU0_LOAD_VF_PTR(vf10, to);
        VU0_STORE_VF_PTR(vf10, target);
        VU0_LOAD_VF_PTR(vf10, to);
        VU0_LOAD_VF_PTR(vf11, from);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF_PTR(vf10, direction);
    }
    first = 1;
    pass = 1;
    unit = work->units;
    /* Reset box extrema separately from the current-point radius and corners. */
    minZ = maxZ = minX = maxX = low = maxRadius = high = 0.0f;
    height = z0 = z1 = x0 = x1 = radius = 0.0f;
    while (pass != 0 && unit != NULL) {
        valid = 0;
        if (pass == 1) {
            if (extra == NULL) {
                seed[0] = 0.0f;
                seed[1] = 0.0f;
                seed[2] = 0.0f;
                seed[3] = 0.0f;
            } else {
                VU0_LOAD_VF_PTR(vf10, extra);
                VU0_STORE_VF_PTR(vf10, seed);
            }
            pos[0] = seed[0];
            pos[1] = seed[1];
            pos[2] = seed[2];
            pos[3] = seed[3];
            height = pos[1];
            x1 = pos[0] + extraRadius;
            z1 = pos[2] + extraRadius;
            x0 = pos[0] - extraRadius;
            z0 = pos[2] - extraRadius;
            /* Retail 001ED764..001ED794 canonicalizes a signed-zero seed. */
            if (height == 0.0f) {
                height = 0.0f;
            }
            radius = extraRadius;
            valid = 1;
        } else {
            if (unit != NULL) {
                u32 flags;
                flags = unit->status.flags;
                if (flags & 1) {
                    if ((flags & 0xC0) == 0) {
                        if (flags & sides) {
                            btlUnitGetMuzzlePosVU(unit);
                            VU0_STORE_VF_PTR(vf10, pos);
                            radius = unit->reach * unit->scale;
                            height = unit->height * unit->scale * 0.5f - pos[1];
                            x1 = pos[0] + radius;
                            z1 = pos[2] + radius;
                            x0 = pos[0] - radius;
                            z0 = pos[2] - radius;
                            valid = 1;
                        }
                    }
                }
            } else {
                pass = 0;
                /* Retail retains this invalid-sample clear at 001ED828 beside
                 * the live per-iteration clear at 001ED724. */
                valid = 0;
            }
        }
        if (valid) {
            if (first) {
                maxRadius = radius;
                if (x0 < x1) {
                    maxX = x1;
                    minX = x0;
                } else {
                    maxX = x0;
                    minX = x1;
                }
                if (z0 < z1) {
                    maxZ = z1;
                    minZ = z0;
                } else {
                    maxZ = z0;
                    minZ = z1;
                }
                low = height;
                high = low;
                first = 0;
            } else {
                if (maxRadius < radius) {
                    maxRadius = radius;
                }
                if (maxX < x1) {
                    maxX = x1;
                }
                if (x1 < minX) {
                    minX = x1;
                }
                if (maxX < x0) {
                    maxX = x0;
                }
                if (x0 < minX) {
                    minX = x0;
                }
                if (maxZ < z1) {
                    maxZ = z1;
                }
                if (z1 < minZ) {
                    minZ = z1;
                }
                if (maxZ < z0) {
                    maxZ = z0;
                }
                if (z0 < minZ) {
                    minZ = z0;
                }
                if (high < height) {
                    high = height;
                }
                if (height < low) {
                    low = height;
                }
            }
        }
        if (pass == 1) {
            pass = 2;
        } else if (pass == 2) {
            unit = unit->next;
        }
    }
    {
        f32 halfY = (high - low) * 0.5f;
        f32 halfX;
        f32 halfZ;
        pos[0] = (maxX + minX) * 0.5f;
        pos[1] = (low - high) * 0.5f;
        halfX = (maxX - minX) * 0.5f;
        pos[2] = (maxZ + minZ) * 0.5f;
        halfZ = (maxZ - minZ) * 0.5f;
        pos[3] = 0.0f;
        if (halfZ < halfX) {
            radius = halfY;
            if (radius < halfX) {
                radius = halfX;
            }
        } else {
            radius = halfZ;
            if (!(halfY < radius)) {
                radius = halfY;
            }
        }
    }
    VU0_LOAD_VF_PTR(vf10, pos);
    if (from != NULL) {
        f32 value;
        f32 x;
        f32 z;
        f32 shift;
        sdfVuLoadRotationMatrixZ(4.71225f);
        VU0_LOAD_VF_PTR(vf10, direction);
        VU0_APPLY_MATRIX(vf10, vf10);
        /* Same SDK side scratch contract as the adjacent group-bounds routine. */
        VU0_STORE_VF_UNCLOBBERED(vf10, side);
        value = side[1];
        if (0.0f <= direction[0]) {
            if (0.0f <= direction[2]) {
                VU0_LOAD_VF_PTR(vf10, side);
                VU0_NEGATE_XYZ(vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, side);
            }
        } else if (direction[2] < 0.0f) {
            VU0_LOAD_VF_PTR(vf10, side);
            VU0_NEGATE_XYZ(vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, side);
        }
        /* Reflect the horizontal side direction without changing its height. */
        z = ffabsf(side[2]);
        x = ffabsf(side[0]);
        side[1] = value;
        if (x < z) {
            value = x / z;
        } else {
            value = z / x;
        }
        shift = (radius * 0.3f) * value;
        VU0_LOAD_VF_PTR(vf10, side);
        VU0_SCALE_VF(vf10, shift);
        VU0_LOAD_VF_PTR(vf11, pos);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_PTR(vf10, pos);
    }
    if (top != NULL) {
        *top = high;
    }
    if (bottom != NULL) {
        *bottom = low;
    }
    return radius;
}

#include "sdf_vu_matrix.h"
f32 func_001EDB20(s32 mask, BtlUnit *included, BtlUnit *excluded, s8 mode,
                  f32 *outMaxHeight, f32 *outMinHeight, const f32 *from, const f32 *to) {
    f32 center[4] __attribute__((aligned(16)));
    f32 rotated[4] __attribute__((aligned(16)));
    f32 edge[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 endpoint[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    BtlState *state;
    BtlUnit *unit;
    BtlUnit *maxActor;
    BtlUnit *minActor;
    s32 count;
    s32 first;
    f32 maxHeight, minHeight;
    f32 maxX, minX, maxZ, minZ;
    f32 trackedMaxX, trackedMinX;
    f32 actorRadius;
    f32 verticalOffset;
    f32 radius;
    f32 length;

    memset(anchor, 0, sizeof(anchor));
    state = (BtlState *)btlGetRuntime();
    count = 0;
    if (from == 0 || to == 0) {
        VU0_LOAD_VF(vf10, anchor);
        VU0_STORE_VF(vf10, endpoint);
    } else {
        VU0_LOAD_VF(vf10, from);
        VU0_STORE_VF(vf10, anchor);
        VU0_LOAD_VF(vf10, to);
        VU0_STORE_VF(vf10, endpoint);
        VU0_LOAD_VF(vf10, to);
        VU0_LOAD_VF(vf11, from);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, direction);
    }
    maxHeight = 0.0f;
    actorRadius = 0.0f;
    minHeight = 0.0f;
    maxX = 0.0f;
    minX = 0.0f;
    maxZ = 0.0f;
    minZ = 0.0f;
    trackedMinX = 0.0f;
    first = 1;
    unit = state->units;
    maxActor = unit;
    minActor = unit;
    trackedMaxX = 0.0f;
    verticalOffset = 0.0f;
    for (; unit != 0; unit = unit->next) {
        s32 flags = unit->status.flags;
        f32 lowerX, upperX, lowerZ, upperZ;
        if (!(flags & 1)) continue;
        if (flags & 0xC0) continue;
        if (included != 0) {
            if (!(flags & mask) && unit->identity != included->identity) {
                continue;
            }
        } else if (!(flags & mask)) {
            continue;
        }
        if (excluded != 0 && unit->identity == excluded->identity) {
            continue;
        }
        if (mode == 1) {
            btlSetActorEffectParameterOrMuzzlePosition(unit, 1);
            VU0_STORE_VF(vf10, center);
            radius = unit->cameraRadius * unit->scale;
            verticalOffset = radius * 0.5f;
        } else {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF(vf10, center);
            verticalOffset = (unit->height * unit->scale) * 0.5f;
            radius = unit->reach * unit->scale;
        }
        count++;
        upperX = center[0] + radius;
        upperZ = center[2] + radius;
        lowerX = center[0] - radius;
        lowerZ = center[2] - radius;
        verticalOffset -= center[1];
        if (first) {
            actorRadius = radius;
            if (lowerX < upperX) {
                maxX = upperX;
                minX = lowerX;
            } else {
                maxX = lowerX;
                minX = upperX;
            }
            if (lowerZ < upperZ) {
                maxZ = upperZ;
                minZ = lowerZ;
            } else {
                maxZ = lowerZ;
                minZ = upperZ;
            }
            minHeight = maxHeight = verticalOffset;
            first = 0;
        } else {
            if (actorRadius < radius) actorRadius = radius;
            if (maxX < upperX) {
                trackedMaxX = upperX;
                maxActor = unit;
                maxX = trackedMaxX;
            }
            if (upperX < minX) {
                minX = upperX;
                minActor = unit;
                trackedMinX = minX;
            }
            if (maxX < lowerX) {
                maxX = lowerX;
                maxActor = unit;
                trackedMaxX = maxX;
            }
            if (lowerX < minX) {
                minX = lowerX;
                minActor = unit;
                trackedMinX = minX;
            }
            if (maxZ < upperZ) maxZ = upperZ;
            if (upperZ < minZ) minZ = upperZ;
            if (maxZ < lowerZ) maxZ = lowerZ;
            if (lowerZ < minZ) minZ = lowerZ;
            if (maxHeight < verticalOffset) maxHeight = verticalOffset;
            if (verticalOffset < minHeight) minHeight = verticalOffset;
        }
    }
    {
        f32 halfHeight = maxHeight * 0.5f;
        f32 halfX;
        f32 halfZ;
        center[0] = (maxX + minX) * 0.5f;
        center[1] = -maxHeight * 0.5f;
        halfX = (maxX - minX) * 0.5f;
        center[2] = (maxZ + minZ) * 0.5f;
        halfZ = (maxZ - minZ) * 0.5f;
        center[3] = 0.0f;
        if (halfZ < halfX) {
            radius = halfHeight;
            if (radius < halfX) radius = halfX;
        } else {
            radius = halfZ;
            if (!(halfHeight < radius)) radius = halfHeight;
        }
    }
    VU0_LOAD_VF(vf10, center);
    if (from != 0) {
        f32 value;
        f32 x;
        f32 z;
        f32 shift;
        sdfVuLoadRotationMatrixZ(0x1.2d958p+2f);
        VU0_LOAD_VF(vf10, direction);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, rotated);
        value = rotated[1];
        if (direction[0] >= 0.0f) {
            if (direction[2] >= 0.0f) {
                VU0_LOAD_VF(vf10, rotated);
                VU0_NEGATE_XYZ(vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, rotated);
            }
        } else if (direction[2] < 0.0f) {
            VU0_LOAD_VF(vf10, rotated);
            VU0_NEGATE_XYZ(vf10);
            VU0_STORE_VF_UNCLOBBERED(vf10, rotated);
        }
        z = ffabsf(rotated[2]);
        x = ffabsf(rotated[0]);
        rotated[1] = value;
        if (x < z) value = x / z;
        else value = z / x;
        shift = (radius * 0x1.333332p-2f) * value;
        VU0_LOAD_VF(vf10, rotated);
        VU0_SCALE_VF(vf10, shift);
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, center);
    }
    if (outMaxHeight != 0) *outMaxHeight = maxHeight;
    if (outMinHeight != 0) *outMinHeight = minHeight;
    if (count >= 2) {
        f32 absMax = ffabsf(trackedMaxX);
        f32 absMin = ffabsf(trackedMinX);
        f32 extent;
        f32 distance;
        unit = maxActor;
        if (!(absMin <= absMax)) unit = minActor;
        if (mode == 1) {
            btlSetActorEffectParameterOrMuzzlePosition(unit, 1);
            VU0_STORE_VF(vf10, edge);
            extent = unit->cameraRadius * unit->scale;
        } else {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF(vf10, edge);
            extent = unit->reach * unit->scale;
        }
        {
            f32 edgeX;
            f32 edgeY;
            if (absMin <= absMax) {
                edgeX = edge[0] + extent;
                edgeY = edge[1];
            } else {
                edgeX = edge[0] - extent;
                edgeY = edge[1];
            }
            edgeY += verticalOffset;
            edge[0] = edgeX;
            edge[1] = edgeY;
        }
        VU0_LOAD_VF(vf10, edge);
        VU0_LOAD_VF(vf11, center);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(distance);
        if (radius <= distance) radius = distance;
        VU0_LOAD_VF(vf10, center);
        return radius;
    }
    VU0_LOAD_VF(vf10, center);
    return radius;
}

f32 func_001EE160(s32 mask, BtlUnit *included, BtlUnit *excluded, s8 mode,
                  f32 *outMaxHeight, f32 *outMinHeight, const f32 *from, const f32 *to, s8 keepSide) {
    f32 center[4] __attribute__((aligned(16)));
    f32 rotated[4] __attribute__((aligned(16)));
    f32 anchor[4] __attribute__((aligned(16)));
    f32 endpoint[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    BtlState *state;
    BtlUnit *unit;
    s32 first;
    f32 maxHeight, minHeight;
    f32 maxX, minX, maxZ, minZ;
    /* Retail retains this unused radius maximum beside the live extrema
     * at 001EE3E0, as in the adjacent func_001EDB20 bounds loop. */
    f32 actorRadius;
    f32 verticalOffset;
    f32 radius;
    f32 length;

    memset(anchor, 0, sizeof(anchor));
    state = (BtlState *)btlGetRuntime();
    if (from == 0 || to == 0) {
        VU0_LOAD_VF(vf10, anchor);
        VU0_STORE_VF(vf10, endpoint);
    } else {
        VU0_LOAD_VF(vf10, from);
        VU0_STORE_VF(vf10, anchor);
        VU0_LOAD_VF(vf10, to);
        VU0_STORE_VF(vf10, endpoint);
        VU0_LOAD_VF(vf10, to);
        VU0_LOAD_VF(vf11, from);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(length);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, direction);
    }
    maxHeight = 0.0f;
    actorRadius = 0.0f;
    minHeight = 0.0f;
    maxX = 0.0f;
    minX = 0.0f;
    maxZ = 0.0f;
    minZ = 0.0f;
    first = 1;
    unit = state->units;
    for (; unit != 0; unit = unit->next) {
        s32 flags = unit->status.flags;
        f32 lowerX, upperX, lowerZ, upperZ;
        if (!(flags & 1)) continue;
        if (flags & 0xC0) continue;
        if (included != 0) {
            if (!(flags & mask) && unit->identity != included->identity) {
                continue;
            }
        } else if (!(flags & mask)) {
            continue;
        }
        if (excluded != 0 && unit->identity == excluded->identity) {
            continue;
        }
        if (mode == 1) {
            btlSetActorEffectParameterOrMuzzlePosition(unit, 1);
            VU0_STORE_VF(vf10, center);
            radius = unit->cameraRadius * unit->scale;
            verticalOffset = radius * 0.5f;
        } else {
            btlUnitGetMuzzlePosVU(unit);
            VU0_STORE_VF(vf10, center);
            verticalOffset = (unit->height * unit->scale) * 0.5f;
            radius = unit->reach * unit->scale;
        }
        upperX = center[0] + radius;
        upperZ = center[2] + radius;
        lowerX = center[0] - radius;
        lowerZ = center[2] - radius;
        verticalOffset -= center[1];
        if (first) {
            actorRadius = radius;
            if (lowerX < upperX) {
                maxX = upperX;
                minX = lowerX;
            } else {
                maxX = lowerX;
                minX = upperX;
            }
            if (lowerZ < upperZ) {
                maxZ = upperZ;
                minZ = lowerZ;
            } else {
                maxZ = lowerZ;
                minZ = upperZ;
            }
            minHeight = maxHeight = verticalOffset;
            first = 0;
        } else {
            if (actorRadius < radius) actorRadius = radius;
            if (maxX < upperX) maxX = upperX;
            if (upperX < minX) minX = upperX;
            if (maxX < lowerX) maxX = lowerX;
            if (lowerX < minX) minX = lowerX;
            if (maxZ < upperZ) maxZ = upperZ;
            if (upperZ < minZ) minZ = upperZ;
            if (maxZ < lowerZ) maxZ = lowerZ;
            if (lowerZ < minZ) minZ = lowerZ;
            if (maxHeight < verticalOffset) maxHeight = verticalOffset;
            if (verticalOffset < minHeight) minHeight = verticalOffset;
        }
    }
    {
        f32 halfHeight = maxHeight * 0.5f;
        f32 halfX;
        f32 halfZ;
        center[0] = (maxX + minX) * 0.5f;
        center[1] = -maxHeight * 0.5f;
        halfX = (maxX - minX) * 0.5f;
        center[2] = (maxZ + minZ) * 0.5f;
        halfZ = (maxZ - minZ) * 0.5f;
        center[3] = 0.0f;
        if (halfZ < halfX) {
            radius = halfHeight;
            if (radius < halfX) radius = halfX;
        } else {
            radius = halfZ;
            if (!(halfHeight < radius)) radius = halfHeight;
        }
    }
    VU0_LOAD_VF(vf10, center);
    if (from != 0) {
        f32 value;
        f32 x;
        f32 z;
        f32 shift;
        sdfVuLoadRotationMatrixZ(0x1.2d958p+2f);
        VU0_LOAD_VF(vf10, direction);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, rotated);
        value = rotated[1];
        if (!keepSide) {
            if (direction[0] >= 0.0f) {
                if (direction[2] >= 0.0f) {
                    VU0_LOAD_VF(vf10, rotated);
                    VU0_NEGATE_XYZ(vf10);
                    VU0_STORE_VF_UNCLOBBERED(vf10, rotated);
                }
            } else if (direction[2] < 0.0f) {
                VU0_LOAD_VF(vf10, rotated);
                VU0_NEGATE_XYZ(vf10);
                VU0_STORE_VF_UNCLOBBERED(vf10, rotated);
            }
        }
        z = ffabsf(rotated[2]);
        x = ffabsf(rotated[0]);
        rotated[1] = value;
        if (x < z) value = x / z;
        else value = z / x;
        shift = (radius * 0x1.333332p-2f) * value;
        VU0_LOAD_VF(vf10, rotated);
        VU0_SCALE_VF(vf10, shift);
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, center);
    }
    if (outMaxHeight != 0) *outMaxHeight = maxHeight;
    if (outMinHeight != 0) *outMinHeight = minHeight;
    return radius;
}

f32 btlBuildProjectedActorPairBoundsVU(s32 mask, BtlUnit *first, BtlUnit *second, s8 firstMode, s8 secondMode,
                  f32 *maxY, f32 *minY, const f32 *lineStart, const f32 *lineEnd) {
    union {
        u128 quadword;
        f32 component[4];
    } vectors[4];
    f32 lineLength;
    f32 largestActorRadius;
    f32 minX;
    f32 maxX;
    f32 minZ;
    f32 maxZ;
    f32 highestOffset;
    f32 lowestOffset;
    f32 radius;
    s32 found;
    s32 i;

    memset(&vectors[1], 0, sizeof(vectors[1]));
    btlGetRuntime();
    if (lineStart == 0 || lineEnd == 0) {
        VU0_LOAD_VF(vf10, &vectors[1]);
        VU0_STORE_VF(vf10, &vectors[2]);
    } else {
        VU0_LOAD_VF(vf10, lineEnd);
        VU0_LOAD_VF(vf11, lineStart);
        VU0_SUB(vf10, vf10, vf11);
        VU0_LENGTH_VF10(lineLength);
        VU0_NORMALIZE_VF10();
        /* VU-only direction scratch, immediately reloaded with no scalar read. */
        VU0_STORE_VF_UNCLOBBERED(vf10, &vectors[3]);
        VU0_LOAD_VF(vf10, &vectors[3]);
        VU0_SCALAR_OP_CLOBBER(1.0f, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, &vectors[1]);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, &vectors[2]);
    }

    minZ = maxZ = minX = maxX = lowestOffset =
        largestActorRadius = highestOffset = 0.0f;
    found = 1;
    for (i = 0; i < 2; i++) {
        BtlUnit *unit;
        s32 mode;
        f32 halfHeight;
        f32 lowerX;
        f32 upperX;
        f32 lowerZ;
        f32 upperZ;
        f32 projection;
        u32 flags;

        if (i == 0) {
            if (first == 0) {
                continue;
            }
            mode = firstMode;
            unit = first;
        } else {
            if (second == 0) {
                continue;
            }
            mode = secondMode;
            unit = second;
        }
        flags = unit->status.flags;
        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 0xC0) != 0) {
            continue;
        }
        if ((flags & mask) == 0) {
            continue;
        }

        if (mode == 1) {
            btlSetActorEffectParameterOrMuzzlePosition(unit, 1);
            radius = unit->cameraRadius * unit->scale;
            halfHeight = radius * 0.5f;
        } else {
            btlUnitGetMuzzlePosVU(unit);
            radius = unit->reach * unit->scale;
            halfHeight = unit->height * unit->scale * 0.5f;
        }

        VU0_STORE_VF(vf10, &vectors[0]);
        projection = btlProjectOnPlaneVU((f32 *)&vectors[1], (f32 *)&vectors[2], (f32 *)&vectors[0]);
        VU0_STORE_VF(vf10, &vectors[0]);
        upperX = (vectors[0].component[0] + projection) + radius;
        lowerX = (vectors[0].component[0] - projection) - radius;
        upperZ = (vectors[0].component[2] + projection) + radius;
        lowerZ = (vectors[0].component[2] - projection) - radius;
        halfHeight -= vectors[0].component[1];

        if (found != 0) {
            largestActorRadius = radius;
            if (lowerX < upperX) {
                maxX = upperX;
                minX = lowerX;
            } else {
                maxX = lowerX;
                minX = upperX;
            }
            if (lowerZ < upperZ) {
                maxZ = upperZ;
                minZ = lowerZ;
            } else {
                maxZ = lowerZ;
                minZ = upperZ;
            }
            lowestOffset = halfHeight;
            highestOffset = lowestOffset;
            found = 0;
        } else {
            if (largestActorRadius < radius) {
                largestActorRadius = radius;
            }
            if (maxX < upperX) {
                maxX = upperX;
            }
            if (upperX < minX) {
                minX = upperX;
            }
            if (maxX < lowerX) {
                maxX = lowerX;
            }
            if (lowerX < minX) {
                minX = lowerX;
            }
            if (maxZ < upperZ) {
                maxZ = upperZ;
            }
            if (upperZ < minZ) {
                minZ = upperZ;
            }
            if (maxZ < lowerZ) {
                maxZ = lowerZ;
            }
            if (lowerZ < minZ) {
                minZ = lowerZ;
            }
            if (highestOffset < halfHeight) {
                highestOffset = halfHeight;
            }
            if (halfHeight < lowestOffset) {
                lowestOffset = halfHeight;
            }
        }
    }

    vectors[0].component[0] = (maxX + minX) * 0.5f;
    vectors[0].component[1] = -highestOffset * 0.5f;
    vectors[0].component[2] = (maxZ + minZ) * 0.5f;
    vectors[0].component[3] = 0.0f;
    {
        f32 halfWidth = (maxX - minX) * 0.5f;
        f32 halfDepth = (maxZ - minZ) * 0.5f;
        f32 halfHeight = highestOffset * 0.5f;

        if (halfDepth < halfWidth) {
            radius = halfHeight;
            if (radius < halfWidth) {
                radius = halfWidth;
            }
        } else {
            radius = halfDepth;
            if (!(halfHeight < radius)) {
                radius = halfHeight;
            }
        }
    }
    VU0_LOAD_VF(vf10, &vectors[0]);
    if (maxY != 0) {
        *maxY = highestOffset;
    }
    if (minY != 0) {
        *minY = lowestOffset;
    }
    return radius;
}

u32 btlCountUnitsByFlags(u32 mask) {
    s32 context = (s32)(u32)btlGetRuntime();
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
        CURSOR->scriptIndex = 2;
        func_001E6668(action, state, 2, 1);
        CURSOR->useTimedScript = 1;
        break;
    case 4:
        btlBuildPresetCameraPose(action, state, 2, 0);
        CURSOR->scriptIndex = 0;
        func_001E6668(action, state, 2, 1);
        CURSOR->useTimedScript = 1;
        break;
    case 3:
    case 5:
    case 6:
        btlBuildPresetCameraPose(action, state, 2, 2);
        CURSOR->scriptIndex = 1;
        func_001E6668(action, state, 2, 1);
        CURSOR->useTimedScript = 1;
        break;
    case 2:
        return;
    default:
        if (btlHasSingleLinkedResource(action) != 0) {
            btlBuildActorPairFramingCameraPose(action, (BtlCamState *)action, 1);
        }
        break;
    }
}

extern const BtlCameraTimedInstruction *D_0035DAE0[];

void btlAdvanceCursorForUnmarkedUnit(BtlLinkedCommand *action, BtlCamState *state) {
    BtlCameraCursorView *cursor = &D_0035F100;

    if (cursor->useTimedScript == 1) {
        if (!(action->task->unit->status.flags & 0x400)) {
            func_001E9DE0(action, state, D_0035DAE0[cursor->scriptIndex]);
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
    if (CURSOR->useTimedScript == 1) {
        if (action->task->unit->status.flags & 0x400) {
            return;
        }
        func_001E9DE0(action, state, D_0035DAF0[CURSOR->scriptIndex]);
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
        CURSOR->scriptIndex = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->useTimedScript = 1;
        break;
    case 2:
        return;
    case 3:
        btlBuildPresetCameraPose(action, state, 4, 0);
        CURSOR->scriptIndex = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->useTimedScript = 1;
        break;
    case 4:
        btlBuildPresetCameraPose(action, state, 4, 0);
        CURSOR->scriptIndex = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->useTimedScript = 1;
        break;
    case 5:
        btlBuildPresetCameraPose(action, state, 4, 0);
        CURSOR->scriptIndex = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->useTimedScript = 1;
        break;
    case 6:
        btlFlagUserAndTargetDefeat(action, action);
        break;
    }
}

/* Advance timed camera instructions, or use the action's approach camera. */
void btlAdvanceTimedCameraOrApproach(BtlLinkedCommand *action, BtlCamState *state) {
    if (CURSOR->useTimedScript == 1) {
        if (action->task->unit->status.flags & 0x400) {
            return;
        }
        func_001E9DE0(action, state, D_0035DAF8[CURSOR->scriptIndex]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera(action, &action->camera);
    }
}

void btlInitCommandCursorForFirstActor(BtlLinkedCommand *action, BtlCamState *state) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *first = btlGetIndexListEntry(action->targetList, 0);
    memset(CURSOR, 0, 0x130);
    btlRefreshUnitEffectMotionAndEntry(first);
    if (btlHasFirstLinkedCategoryFlag1000(action) != 0) {
        btlBuildPresetCameraPose(action, state, 5, 1);
    } else {
        btlBuildPresetCameraPose(action, state, 5, 0);
    }
    if (work->battleMode == 0x10E) {
        CURSOR->scriptIndex = 3;
    } else {
        CURSOR->scriptIndex = 0;
    }
    func_001E6668(action, state, 0, 0x11);
}

void btlAdvanceTargetCursorAnimation(BtlLinkedCommand *action, BtlCamState *state) {
    if (!(action->task->unit->status.flags & 0x400)) {
        func_001E9DE0(action, state, D_0035DAD0[CURSOR->scriptIndex]);
        func_001EB368(action, state);
        btlConstrainCameraEndpointHeight(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

extern void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit);

extern void btlBuildPresetCameraPose(BtlLinkedCommand *, BtlCamState *, s16, s16);

extern void func_001E6668(BtlLinkedCommand *, BtlCamState *, s32, s32);

void btlInitLinkedUnitActionCursor(BtlTask *task) {
    BtlLinkedCommand *command = &((BtlState *)btlGetRuntime())->cameraCommand;
    command->task = task;
    memset(CURSOR, 0, 0x130);
    CURSOR->scriptSetIndex = 0;
    CURSOR->unk_0E = 0;
    btlRefreshUnitEffectMotionAndEntry(task->unit);
    btlBuildPresetCameraPose(command, &command->camera, 6, 0);
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(task->unit);
    CURSOR->scriptIndex = 0;
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
            btlMeasureMaskedUnitBounds(0x400, 0, 0);
        } else {
            btlMeasureMaskedUnitBounds(0x200, 0, 0);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlAimHorizontalDirectionVU(aimPosition, position);
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
    D_0035F100.scriptIndex = 2;
}

void btlAdvanceWorldCounterAndSpawnActionObject(void) {
    u32 counter;
    BtlState *context;
    struct EffWorldNode *object;

    context = (BtlState *)btlGetRuntime();
    counter = dds3AdvanceWorldCounter();
    object = evtSpawnActionObj9(counter);
    context->sceneLightNode = object;
    D_003BB694 = 0;
}

s32 btlAreWorkBuffersReady(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->frontFloorResource != 0) {
        if (work->sideFloorResource != 0) {
            return 1;
        }
    }
    return 0;
}

void btlReleaseWorkBuffers(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->sideFloorResource != 0) {
        sdfReleaseChipOrRetainedResource(work->sideFloorResource);
        work->sideFloorResource = 0;
    }
    if (work->frontFloorResource != 0) {
        sdfReleaseChipOrRetainedResource(work->frontFloorResource);
        work->frontFloorResource = 0;
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
    temp_v1 = (s32)(u32)btlGetRuntime();
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

void btlInitTintTransitionResource(u32 resource, u16 soundId);

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

void btlBuildActionLightParameters(s32 skillIndex, BtlSceneLightParams *light) {
    u8 *work = (u8 *)btlGetRuntime();
    BtlActionTableEntry *action;
    f32 *vector;
    f32 *defaultColor;
    u32 color;

    if ((*(u32 *)(work + 0x1F4) & 0x30000000) == 0) {
        /* Index the record table by integer address arithmetic, as retail does. */
        action = (BtlActionTableEntry *)(skillIndex * sizeof(BtlActionTableEntry) +
                                        (u32)datActionAnimationRecords);
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

s32 btlGetActionDefaultOrOverride(s32 skillIndex) {
    s32 context = (s32)(u32)btlGetRuntime();
    if (*(u32 *)(context + 0x1F4) & 0x30000000) {
        return *(s32 *)(context + 0x6A0);
    }
    return *(s32 *)(datActionAnimationRecords + skillIndex * 32 + 0x18);
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

void btlQueueTintTransition(u32 color, u16 frames) {
    BtlTintTransition *transition;
    if (frames == 0) {
        transition = (BtlTintTransition *)D_0035F5D0;
        transition->framesRemaining = 0;
        transition->currentColor = color;
        transition->targetColor = color;
        return;
    }
    transition = (BtlTintTransition *)D_0035F5D0;
    transition->framesRemaining = frames;
    transition->durationFrames = frames;
    transition->sourceColor = transition->currentColor;
    transition->targetColor = color;
}

void btlQueueTintTransitionToZero(u16 frames) {
    BtlTintTransition *transition;
    if (frames == 0) {
        transition = (BtlTintTransition *)D_0035F5D0;
        transition->framesRemaining = 0;
        transition->currentColor = 0;
        transition->targetColor = 0;
        return;
    }
    transition = (BtlTintTransition *)D_0035F5D0;
    transition->sourceColor = transition->currentColor;
    transition->targetColor = 0;
    transition->framesRemaining = frames;
    transition->durationFrames = frames;
}

void btlStepBlendColor(void) {
    BtlTintTransition *transition = (BtlTintTransition *)D_0035F5D0;

    if (transition->framesRemaining != 0) {
        transition->currentColor = btlBlendColor(transition->targetColor, transition->sourceColor, (f32)transition->framesRemaining / (f32)transition->durationFrames);
        transition->framesRemaining--;
    } else {
        transition->currentColor = transition->targetColor;
    }
    btlStepTintTransition();
}

void btlDrawTintIfVisible(void) {
    BtlTintTransition *transition = (BtlTintTransition *)D_0035F5D0;

    if ((transition->currentColor & 0xFF000000) != 0) {
        effDrawSolidRectangle(transition);
    }
}

void sndResetTransition(void) {
    BtlTintTransition *transition = (BtlTintTransition *)D_0035F5D0;
    D_003BB694 = 0;
    btlTintTransitionHoldCount = 0;
    transition->framesRemaining = 0;
    transition->currentColor = 0;
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
    s32 temp_v0 = (s32)(u32)btlGetRuntime();

    if ((((*(u32 *)(temp_v0 + 500) & 0x20000) != 0) && ((*(u32 *)(temp_v0 + 0x1f8) & 0x20) == 0)) &&
          ((*(u32 *)(temp_v0 + 0x1fc) & 0x4000000) == 0)) {
        fldUpdateSelectedValueTransition();
        fldUpdateSwayOffset();
        fldDrawSkyGradient();
    }
    btlDrawTintIfVisible();
}

extern WorldTransformSetup D_003D74A0;

extern void dds3LoadWorldTransformSetup(EffWorldNode *, WorldTransformSetup *);

extern void evtBeginUnitValueColorTransition(EffWorldNode *object, s32 blendFrames);

/* Stage light/ambient RGB on the scene node and start its frames-long blend. */
void btlBeginSceneLightColorTransition(f32 *lightColor, f32 *ambientColor, s32 frames) {
    BtlState *work = (BtlState *)btlGetRuntime();
    EffWorldNode *listener = work->sceneLightNode;

    D_003D74A0.transform.position[0] = lightColor[0];
    D_003D74A0.transform.position[1] = lightColor[1];
    D_003D74A0.transform.position[2] = lightColor[2];
    D_003D74A0.transform.scale[0] = ambientColor[0];
    D_003D74A0.transform.scale[1] = ambientColor[1];
    D_003D74A0.transform.scale[2] = ambientColor[2];
    D_003D74A0.unk00 = 0;
    D_003D74A0.flags = 0;
    D_003D74A0.mode = 0;
    D_003D74A0.transform.rotation[0] = 0.0f;
    D_003D74A0.transform.rotation[1] = 0.0f;
    D_003D74A0.transform.rotation[2] = 0.0f;
    D_003D74A0.transform.rotation[3] = 0.0f;
    dds3LoadWorldTransformSetup(listener, &D_003D74A0);
    evtBeginUnitValueColorTransition(work->sceneLightNode, frames);
}

extern SdfFlagListParams D_0035F5F8;

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4AD8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4AF0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4B08);

void btlCreateRainEffect(u32 majorId, u32 minorId) {
    BtlState *work = (BtlState *)btlGetRuntime();

    switch (majorId) {
    case 0xDF:
        switch (minorId) {
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
        if (minorId == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_0035F5F8);
        }
        break;
    case 0xE1:
        if (minorId == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(&D_0035F5F8);
        }
        break;
    }
    if (work->soundTransitionTask != 0) {
        btlBossDebugPrintf("btl:rain create[f%03X_%03X]\n", majorId, minorId);
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

extern s32 evtCreateWorldObjectFromResource(s32 area, s32 room, s32 resource0Data, s32 resource1Data,
                                            const SdfTextureOffsetListHeader *textureOffsets, s32 flags);

extern char D_003A4B50[]; /* "%sf%03d_%03d.LB" */

extern char D_003A4B60[]; /* "btl:field load[%s]\n" */

extern char D_003A4B78[]; /* "btl:field load end[f%03d_%03d]\n" */

u32 btlPollFieldArchiveLoad(BtlFieldLoadArgs *args) {
    BtlState *blocks = (BtlState *)btlGetRuntime();
    char directory[0x80];
    char path[0x80];
    PacWork *node;
    u32 i;

    if (args->frame == 0) {
        btlFreeFieldBlocks();
        fldFormatAreaDirectory(directory, args->stage, 1);
        sprintf(path, D_003A4B50, directory, args->stage, args->variant);
        btlBossDebugPrintf(D_003A4B60, path);
        args->request = (struct FilePacRequest *)fileQueuePlainDispatchRequest(path);
        args->fieldF1 = NULL;
        args->fieldF2 = NULL;
        args->fieldTB = NULL;
    } else {
        if (args->request != NULL && fileRequestIsReady((struct FileRequest *)args->request)) {
            FilePacRequest *request = args->request;
            node = request->packet.queueHead;
            i = 0;
            while (node != NULL) {
                switch (i) {
                case 0:
                    blocks->fieldTBResourceId = (s32)(u32)node->resourceHandle;
                    args->fieldTB = (const SdfTextureOffsetListHeader *)node->dataCursor;
                    break;
                case 1:
                    blocks->fieldF2ResourceId = (s32)(u32)node->resourceHandle;
                    args->fieldF2 = node->dataCursor;
                    break;
                case 2:
                    blocks->fieldF1ResourceId = (s32)(u32)node->resourceHandle;
                    args->fieldF1 = node->dataCursor;
                    break;
                }
                node = node->next;
                i++;
            }
            btlStageTaskCleanupCallback(request);
            args->request = NULL;
        }
        if (args->fieldF1 != NULL && args->fieldF2 != NULL && args->fieldTB != NULL) {
            evtCreateWorldObjectFromResource(args->stage, args->variant,
                                             (s32)args->fieldF1, (s32)args->fieldF2,
                                             args->fieldTB, 0);
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

s32 fldCreateSceneTileTask(u32 stage, u32 variant) {
    u8 *task = btlAllocTask(40);
    BtlFieldLoadArgs *args;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u16 *)(task + 0x20) = 1;
    *(void **)(task + 0x4C) = btlPollFieldArchiveLoad;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    args = (BtlFieldLoadArgs *)btlGetTaskArguments(task);
    memset(args, 0, 40);
    args->stage = stage;
    args->variant = variant;
    args->frame = 0;
    return (s32)task;
}

s32 btlPollFloorLoadTask(BtlFloorLoadArgs *args) {
    s32 result = 1;
    BtlState *work = (BtlState *)btlGetRuntime();
    s32 stage = args->stage;
    s32 variant = args->variant;
    char path[0x70];

    if (args->state == 0) {
        sprintf(path, D_003A4B98, stage, stage, variant);
        result = 0;
        args->frontHandle = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A4BB8, path);
        sprintf(path, D_003A4BC8, stage, stage, variant);
        args->sideHandle = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A4BE8, path);
    } else {
        if (args->frontHandle != 0) {
            if (fileIsRequestReadyInCurrentMode(args->frontHandle) != 0) {
                work->frontFloorResource =
                    (void *)sdfResourceRetainAddress(fileGetResourceHandle(args->frontHandle));
                filePollEntryCleanup(args->frontHandle);
                args->frontHandle = 0;
                btlBossDebugPrintf(D_003A4BF8);
            } else {
                result = 0;
            }
        }
        if (args->sideHandle != 0) {
            if (fileIsRequestReadyInCurrentMode(args->sideHandle) != 0) {
                work->sideFloorResource =
                    (void *)sdfResourceRetainAddress(fileGetResourceHandle(args->sideHandle));
                filePollEntryCleanup(args->sideHandle);
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

u8 *btlCreateFloorLoadTask(u32 stage, u32 variant) {
    u8 *task = btlAllocTask(20);
    BtlFloorLoadArgs *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    *(u16 *)(task + 0x24) &= ~BTL_TASK_FLAG_REGISTERED;
    *(u16 *)(task + 0x20) = 2;
    *(void **)(task + 0x4C) = btlPollFloorLoadTask;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    arguments = btlGetTaskArguments(task);
    arguments->frontHandle = 0;
    arguments->sideHandle = 0;
    arguments->stage = stage;
    arguments->variant = variant;
    arguments->state = 0;
    return task;
}

extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply the scene blend and per-unit light colors, then retain a lighting hold. */
u32 btlApplySceneLightingTransition(SceneLightTransitionArgs *args) {
    f32 *first = args->lightColor;
    f32 *second = args->ambientColor;
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    f32 *lightColor;
    f32 *ambientColor;
    u32 firstColor;
    u32 secondColor;
    u32 transitionCount;

    btlBeginSceneLightColorTransition(first, second, args->value);
    lightColor = work->lightColor;
    PCP_COPY_VECTOR_F32(lightColor, first);
    ambientColor = work->ambientColor;
    PCP_COPY_VECTOR_F32(ambientColor, second);
    PCP_COPY_VECTOR_F32(work->lightDirection, args->direction);
    for (unit = work->units; unit != NULL; unit = unit->next) {
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
    transitionCount = D_003BB694;
    D_00324770[0][4] = args->direction[0];
    D_00324770[0][5] = args->direction[1];
    D_00324770[0][6] = args->direction[2];
    D_003BB694 = transitionCount + 1;
    return 1;
}

extern u32 btlApplySceneLightingTransition(SceneLightTransitionArgs *);

void *btlCreateEffectTaskWithSourceParams(u8 *source, u32 value) {
    u8 *task = btlAllocTask(0x34);
    u8 *arguments;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(u16 *)(task + 0x20) = 3;
    *(u16 *)(task + 0x24) |= BTL_TASK_FLAG_DEFERRED;
    *(void **)(task + 0x4C) = btlApplySceneLightingTransition;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    *(u32 *)(arguments + 0x30) = value;
    if (source != 0) {
        memcpy(arguments, source, 0x30);
    } else {
        memcpy(arguments, ((u8 *)btlGetRuntime() + 0x10), 0x30);
    }
    return task;
}

s32 btlRestoreSceneTransformLighting(SceneLightRestoreArgs *args) {
    BtlState *work;
    BtlUnit *unit;
    EffWorldNode *listener;
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
        listener = work->sceneLightNode;
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
        dds3LoadWorldTransformSetup(listener, &D_003D74A0);
        evtBeginUnitValueColorTransition(work->sceneLightNode, args->value);
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

/* Create the deferred lighting-restore task with its native four-byte argument. */
BtlRuntimeTask *btlCreateSceneLightingRestoreTask(u32 value) {
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

/* Collapse lighting holds to one before the normal restore callback. */
s64 btlForceSceneLightingRestore(SceneLightRestoreArgs *arguments) {
    D_003BB694 = 1;
    return btlRestoreSceneTransformLighting(arguments);
}

BtlRuntimeTask *btlCreateSoundUpdateTask(u32 value) {
    BtlRuntimeTask *task = btlCreateSceneLightingRestoreTask(value);
    task->taskId = 7;
    task->callback = btlForceSceneLightingRestore;
    return task;
}

s32 btlQueueTintTransitionWhenEnabled(const BtlTintAcquireArgs *args) {
    s32 context = (s32)(u32)btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 0x20000000) == 0) {
        btlQueueTintTransition(args->color, (u16)args->frames);
    }
    btlTintTransitionHoldCount++;
    return 1;
}

BtlRuntimeTask *sndCreateAcquireTask(u32 color, u32 frames) {
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
    s32 context = (s32)(u32)btlGetRuntime();

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
    btlQueueTintTransitionToZero((u16)args->frames);
    return 1;
}

BtlRuntimeTask *sndCreateReleaseTask(u32 frames) {
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

/* Collapse tint holds to one before the existing tint-release callback. */
s64 btlForceReleaseTintTransition(const BtlTintReleaseArgs *args) {
    btlTintTransitionHoldCount = 1;
    return sndTickFadeCounter(args);
}

BtlRuntimeTask *btlCreateSoundReleaseTask(u32 frames) {
    BtlRuntimeTask *task = sndCreateReleaseTask(frames);
    task->taskId = 8;
    task->callback = btlForceReleaseTintTransition;
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
    s32 (*lookup)(s32, s32) = *(s32 (**)(s32, s32))((u8 *)btlGetRuntime() + 0x644);
    if (lookup != 0) {
        s32 value = lookup(sound, index);
        if (value != -1) {
            return value;
        }
    }
    return *(u8 *)(datActionAnimationRecords + index * 0x20 + 3);
}

void func_001F0CA0(BtlTask *task, BattleIndexWork *work) {
    u32 targetCount = btlGetIndexListCount(work->indices);
    BtlOperandGroup *group = work->groups;
    BtlUnit *target;
    s32 skillId;
    u32 targetIndex;
    u32 entryIndex;
    u32 entryCount;
    u32 commandByteOffset;
    s32 groupKind;
    u16 entryFlags;
    u8 reflected;
    u8 inactive;

    if (work->phase == 4) {
        skillId = btlGetLoggedIndexedCommandItem(work->reference);
    } else {
        skillId = work->skillId;
    }

    {
        BtlUnit *actor = task->unit;
        BtlUnit *companion = work->companionA;
        actor->effectLink.flags = 0;
        if (companion != NULL) {
            companion->effectLink.flags = 0;
        }
    }
    if (work->companionB != NULL) {
        work->companionB->effectLink.flags = 0;
    }
    for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
        target = (BtlUnit *)btlGetIndexListEntry(work->indices, targetIndex);
        target->effectLink.flags = 0;
    }

    targetIndex = 0;
    if (targetCount != 0) {
        commandByteOffset = (u32)skillId * sizeof(DatCommandRecord);
        do {
            target = (BtlUnit *)btlGetIndexListEntry(work->indices, targetIndex);
            groupKind = group->kind;
            if (groupKind == 2) {
                target->effectLink.flags |= 0x10;
            }
            if (groupKind == 0x10000) {
                target->effectLink.flags |= 0x10;
            }

            reflected = group->reflected;
            if (reflected != 0 || groupKind == 0x40000) {
                BtlUnit *actor;
                if ((u32)skillId < 0x260) {
                    s32 animationKind = ((BtlActionAnimationRecord *)datActionAnimationRecords)[skillId].effectKind;
                    switch (animationKind) {
                    case 3:
                        actor = task->unit;
                        target->effectLink.flags |= 0x10;
                        break;
                    case 4:
                        target->effectLink.flags |= 0x10;
                    case 0:
                    case 1:
                    case 2:
                    default:
                        actor = task->unit;
                        target->effectLink.flags |= 0x20;
                        actor->effectLink.flags |= 0x20;
                        break;
                    }
                } else {
                    actor = task->unit;
                }
                target = actor;
            }

            if (skillId >= 0) {
                DatCommandRecord *record = (DatCommandRecord *)(commandByteOffset + (u32)datCommandRecords);
                if (record->effectType == 2 || (record->flags & 0x20) != 0) {
                    BtlUnit *actor = task->unit;
                    target->effectLink.flags |= 8;
                    actor->effectLink.flags |= 8;
                }
            }
            inactive = group->inactive;
            if (inactive != 0) {
                target->effectLink.flags |= 1;
            }

            entryCount = group->count;
            entryIndex = 0;
            if (entryCount != 0) {
                goto entry_dispatch;
            entry_loop:
                groupKind = group->kind;
            entry_dispatch:
                do {
                    switch ((u32)groupKind) {
                    case 2:
                    case 4:
                    case 0x10000:
                    case 0x40000:
                        break;
                    default:
                        if (btlActionEntryIsEmpty(skillId, group, &group->entries[entryIndex]) == 0) {
                            target->effectLink.flags |= 4;
                        }
                        break;
                    }
                } while (0);

                entryFlags = group->entries[entryIndex].flags;
                if ((entryFlags & 1) != 0) {
                    {
                        BtlUnit *actor = task->unit;
                        target->effectLink.flags |= 2;
                        actor->effectLink.flags |= 2;
                    }
                    entryFlags = group->entries[entryIndex].flags;
                }
                if ((entryFlags & 2) != 0) {
                    target->effectLink.flags |= 1;
                }
                entryIndex++;
                if (entryIndex < entryCount) {
                    goto entry_loop;
                }
            }
            targetIndex++;
            group++;
        } while (targetIndex < targetCount);
    }
}

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4B40);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4B50);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4B60);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4B78);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4B98);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4BB8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4BC8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4BE8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4BF8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4C10);

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

/* Advance the referenced battle effect; return one when its battle or duration ends. */
s32 btlUpdateReferencedBattleEffect(SoundEffectReferenceArgs *args) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *owner;

    if ((battle->battleFlags & 0x40000) == 0) {
        return 1;
    }
    owner = args->option == 2 ? args->targetOwner.unit : args->sourceOwner.unit;
    if (args->source->flags & 2) {
        if (args->effect == NULL) {
            args->effect = effCreateBattleVoiceOwner(args->source->resourceHandle, args->option, owner, 0);
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
        effBattleUpdate(args->effect);
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

/* Create a deferred effect task whose start/finish callbacks balance resource and actor references. */
BtlRuntimeTask *btlCreateReferencedBattleEffectTask(SoundResourceNode *effect, BtlUnit *source, BtlUnit *owner, u16 variant) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundEffectReferenceArgs));
    SoundEffectReferenceArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x2B;
    task->flags |= BTL_TASK_FLAG_DEFERRED;
    task->ownerId = owner->identity;
    task->onStart = sndAddEffectReferences;
    task->callback = btlUpdateReferencedBattleEffect;
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
    BtlRuntimeTask *task = btlCreateReferencedBattleEffectTask(effect, sourceOwner, owner, variant);
    SoundEffectReferenceArgs *data = btlGetTaskArguments(task);
    data->sourceSelector = source;
    data->targetSelector = target;
    return task;
}

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
        args->effect = effCreateBattleVoiceOwner(args->source->resourceHandle, 0, unit, 0);
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
        effBattleUpdate((u8 *)args->effect);
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
    args->request = fileQueueDefaultCallbackRequest(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

extern char D_003A4C88[];

s32 sndPollEffectLoad(EffectLoadArgs *args) {
    SoundResourceNode *effect = args->effect;
    struct SdfMemBlock *resource;

    if (effect->flags & 2) {
        return 1;
    }
    if (!fileIsRequestReadyInCurrentMode(args->request)) {
        return 0;
    }
    btlBossDebugPrintf(D_003A4C88, args->name);
    resource = fileGetResourceHandle(args->request);
    effect->resourceHandle =
        sndMixerClone((void *)sdfResourceRetainAddress(resource));
    sdfReleaseResourceAllocation(resource);
    filePollEntryCleanup(args->request);
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
    s32 context = (s32)(u32)btlGetRuntime();
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
    BtlUnit *object = *(BtlUnit **)((u8 *)btlGetRuntime() + 0x228);
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

s32 sndUpdateEffectSourceFade(SoundEffectSourceArgs *args) {
    BtlUnit *owner;
    u32 flags;

    btlGetRuntime();
    owner = args->owner.unit;

    if (args->effect == NULL) {
        args->effect = effCreateBattleVoiceOwner(args->source->resourceHandle, 0, owner, 0);
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
        effBattleUpdate(args->effect);
    }
    args->frameCount++;
    return 0;
}

extern s32 sndUpdateEffectSourceFade(SoundEffectSourceArgs *);

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
    task->callback = sndUpdateEffectSourceFade;
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

u32 btlTaskStartFadeIn(u32 *taskArgs) {
    kwlnFadeStartIn(*taskArgs);
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

u32 btlTaskStartCustomFadeIn(u8 *taskArgs) {
    kwlnFadeInStart(*taskArgs, taskArgs[1], taskArgs[2], *(u32 *)(taskArgs + 4));
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

    temp_v0 = (s32)(u32)btlGetRuntime();
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

    temp_v0 = (s32)(u32)btlGetRuntime();
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

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4C88);

const char D_003A4CA8[16] __attribute__((aligned(8))) = "%s%03X.BED";

const char D_003A4CB8[32] __attribute__((aligned(8))) = "/efftool/bed/BTL_TEST.BED";

s32 btlFormatActionEventFilename(s32 skillIndex, char *output) {
    BtlState *state = (BtlState *)btlGetRuntime();

    if ((state->battleFlags & 0x10000000) == 0) {
        u16 assetId = ((BtlActionAnimationRecord *)datActionAnimationRecords)[skillIndex].displayCode;
        if (assetId == 0) {
            return 0;
        }
        sprintf(output, D_003A4CA8, D_003BB6A0, assetId);
    } else {
        sprintf(output, D_003A4CB8);
    }
    return 1;
}

s32 sndSetEffectNodeParameter(SoundResourceNode *effect, u16 option) {
    return sndReadSelectedMixerBankValue(effect->resourceHandle, option);
}

s32 sndGetEffectNodeParameter(SoundResourceNode *effect, u16 option) {
    return sndGetMixerBankCallbackFrame(effect->resourceHandle, option);
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

void sndClearResourceNodes(void);

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
    node->opaque0C = 0;
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

INCLUDE_ASM(const s32, "game/code_001DEFE0", func_001F2818);

void btlMarkTaskReady(SoundResourceLink *link) {
    link->refreshRequested = 1;
}

SoundLink *sndAllocLink(BtlUnit *owner) {
    SoundLink *node = sdfAllocAndClearQuadwords(sizeof(SoundLink));
    node->owner = owner;
    node->effectHandle = 0;
    node->commandEffectId = 0;
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
        effectId = datCommandRecords[actor->selectedEntryIndex].commandEffectId;
    } else {
        effectId = 0;
    }
    if (effectId != 0 && !(actor->partyRecord.status & 0x4000)) {
        if (link->effectHandle == 0) {
            link->effect = work->soundResourceSlots[BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT];
            sndCreateSystemEffect(link->effect);
            link->effectHandle = effCreateBattleVoiceOwner(link->effect->resourceHandle, 2, actor, 0);
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
        effBattleUpdate((u8 *)link->effectHandle);
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
    FilePacRequest *archive = (FilePacRequest *)fileQueuePlainDispatchRequest(path);
    PacWork *node;
    u32 i;

    fileWaitForRequestReady((struct FileRequest *)archive);
    btlBossDebugPrintf(D_003A5020, path);
    node = archive->packet.queueHead;
    i = 0;
    while (node != NULL) {
        if (D_0035F748[i].enabled != 0) {
            u32 unk08 = (u32)node->resourceHandle;
            u32 unk0C = (u32)node->dataCursor;
            D_0035F748[i].opaque08 = unk08;
            D_0035F748[i].archiveData = unk0C;
            node = node->next;
        } else {
            D_0035F748[i].archiveData = 0;
            D_0035F748[i].opaque08 = 0;
        }
        i++;
    }
    for (; i < BTL_SOUND_ENTRY_COUNT; i++) {
        D_0035F748[i].archiveData = 0;
        D_0035F748[i].opaque08 = 0;
    }
    btlStageTaskCleanupCallback(archive);
}

void btlCreateIndexedSoundResourceNode(s32 slotIndex, void * handle);

/* Register available SYSEFF handles; unavailable slots are left untouched. */
void btlRefreshSoundEntries(void) {
    u32 i;

    btlGetRuntime();
    for (i = 0; i < BTL_SOUND_ENTRY_COUNT; i++) {
        if (D_0035F748[i].archiveData != 0) {
            /* Bank entries retain their original data-address word representation. */
            btlCreateIndexedSoundResourceNode(i, (void *)D_0035F748[i].archiveData);
        }
    }
}

/* Free and clear slots selected by the archive table, without discarding metadata. */
void sndFreeBattleSoundEntries(void) {
    s32 context = (s32)(u32)btlGetRuntime();
    SoundResourceNode **node = ((BtlActorWork *)context)->soundResourceSlots;
    u32 i;

    for (i = 0; i < BTL_SOUND_ENTRY_COUNT; i++, node++) {
        if (D_0035F748[i].archiveData != 0) {
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

    work = (s32)(u32)btlGetRuntime();
    node = sndAllocResourceNode();
    flags = node->flags;
    node->sourceHandle = handle;
    ((BtlActorWork *)work)->soundResourceSlots[slotIndex] = node;
    node->flags = flags | 10;
}

#include "btl_sound_handle_node.h"

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(sizeof(SoundHandleNode));
    BattleSoundArchiveEntry *entry = &D_0035F748[index];
    node->actor = actor;
    node->queue = fileCloneQueueEntries((struct FileQueue *)entry->archiveData);
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

void sndDestroyFileQueueWrapper(u32 handle) {
    SoundHandleNode *node = (SoundHandleNode *)handle;
    fileQueueDestroy(node->queue);
    sdfReleaseChipBlock(node);
}

void func_001F3230(void) {
}

void sndSetStationedSeVolume(u32 sequence) {
    sndSetSequenceVolumePan(sequence, 0x58, 0x3f);
}

void sndSetStationedSeHighVolume(u32 sequence) {
    sndSetSequenceVolumePan(sequence, 0x7f, 0x3f);
}



extern s32 btlSceneStreamCycleEntries[];

extern u32 D_003BB6A8;

void sndStartBattleSceneStream(s32 adjustmentIndex, s32 sceneIndex) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    s32 selection = 0;

    if (adjustmentIndex != 0) {
        selection = D_003BAA3C[adjustmentIndex].streamSelection;
    }
    if (((DatBattleSceneAudioSelectionView *)&datBattleSceneRecords[sceneIndex])->audioStreamSelectionOverride != 0) {
        selection = ((DatBattleSceneAudioSelectionView *)&datBattleSceneRecords[sceneIndex])->audioStreamSelectionOverride;
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
        mnuQueueTitleSoundStreamEntry(btlSceneStreamCycleEntries[D_003BB6A8 % 5]);
        D_003BB6A8++;
    } else {
        mnuQueueTitleSoundStreamEntry(selection - 1);
    }
}

u8 sndIsStreamStatusTwoOrThree(void) {
    s32 status;

    status = mnuPollTitleStreamStateLocked();
    return status - 2U < 2;
}

void btlResetTitleStreamOnBattleFlag(void) {
    s32 temp_v0;

    temp_v0 = (s32)(u32)btlGetRuntime();
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
    sndQueueNoPayloadCommand210();
    sndQueueNoPayloadCommand1A0();
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

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4CD8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4CF0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4D08);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4D20);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4D38);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4D50);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4D68);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4D80);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4D98);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4DB0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4DC8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4DE0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4DF8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4E10);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4E28);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4E40);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4E58);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4E70);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4E88);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4EA0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4EC0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4EE0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4F00);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4F18);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4F30);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4F48);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4F60);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4F78);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4F90);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4FA8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4FC0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4FD8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A4FF0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5008);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5020);

s32 sndPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (sndLoadAndPlayStationedSe(soundId)) {
        btlBossDebugPrintf("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

BtlRuntimeTask *sndCreateStationedSeTask(u32 soundId) {
    BtlRuntimeTask *task = btlAllocTask(4);
    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x55;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->callback = sndPlayStationedSe;
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

s32 sndPlaySkillSeTask(u32 *taskArgs) {
    u8 *taskData = (u8 *)taskArgs;
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    u32 value;

    if (*(u16 *)(taskData + 4) == 2 && work->skillSeLoadFrame < 6) {
        btlBossDebugPrintf("btl:skill SE ignore[frame:%d]\n", work->skillSeLoadFrame);
        return 1;
    }
    work->skillSeLoadFrame = 0;
    if (sndFindPackedTrackLoadStatus(*(u32 *)taskData) != 0) {
        switch (*(u16 *)(taskData + 4)) {
        case 1:
            value = *(u32 *)taskData;
            break;
        case 2:
            value = *(u32 *)taskData | 1;
            break;
        case 3:
            value = *(u32 *)taskData | 1;
            break;
        default:
            value = 0;
            break;
        }
        sndSetStationedSeVolume(value);
        btlBossDebugPrintf("btl:sound skillSE play[%X-%X]\n", value >> 16, value & 0xFFFF);
        return 1;
    }
    btlBossDebugPrintf("btl:sound load wait[%X]\n", *(u16 *)(taskData + 2));
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

void sndStartFileLoad(SoundFileTaskArgs *request) {
    ActiveSoundNode *node = request->node;

    request->request = fileQueueDefaultCallbackRequest(request->filename);
    node->flags |= SOUND_FILE_STATE_REQUEST_PENDING;
    node->packedTrackPosition = (request->blockIndex + 0x200) << 16;
    node->loadState = 2;
    btlBossDebugPrintf("btl:sound file load start[%s]\n", request->filename);
}

extern char D_003A50D8[];

extern char D_003A50F0[];

extern char D_003A5110[];

extern char D_003A5138[];

extern void func_002E9450(s32, s32);

u32 sndPollMotSeFileAndSpu(SoundFileTaskArgs *request) {
    ActiveSoundNode *node = request->node;
    if (sndHasActiveFileLoad()) {
        btlBossDebugPrintf(D_003A50D8);
        return 0;
    }
    if ((node->flags & SOUND_FILE_STATE_SOURCE_REQUEST_RESOLVED) == 0) {
        if (fileIsRequestReadyInCurrentMode(request->request)) {
            s32 size;
            s32 data;
            btlBossDebugPrintf(D_003A50F0, request->filename);
            request->resourceAllocation = fileGetResourceHandle(request->request);
            size = (s32)fileGetResourceSize(request->request);
            data = sdfResourceRetainAddress(request->resourceAllocation);
            if (sndFindPackedTrackLoadStatus(node->packedTrackPosition) == 0) {
                func_002E9450(data, size);
                node->flags |= SOUND_FILE_STATE_SPU_LOAD_PENDING;
                /* The packed position stores the sound block number in its upper halfword. */
                btlBossDebugPrintf(D_003A5110, (u16)(node->packedTrackPosition >> 16), size);
            }
            node->flags = (node->flags & ~SOUND_FILE_STATE_REQUEST_PENDING) |
                          SOUND_FILE_STATE_SOURCE_REQUEST_RESOLVED;
        }
    } else if (sndFindPackedTrackLoadStatus(node->packedTrackPosition) != 0) {
        btlBossDebugPrintf(D_003A5138, (u16)(node->packedTrackPosition >> 16));
        sdfReleaseResourceAllocation(request->resourceAllocation);
        filePollEntryCleanup(request->request);
        node->flags = (node->flags & ~SOUND_FILE_STATE_SPU_LOAD_PENDING) |
                      SOUND_FILE_STATE_COMPLETE;
        return 1;
    }
    return 0;
}

BtlRuntimeTask *sndCreateFileLoadTask(ActiveSoundNode *node, u32 variant, const char *filename) {
    u8 *task = btlAllocTask(strlen(filename) + 20);
    SoundFileTaskArgs *request;
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
    request->filename = name;
    strcpy(name, filename);
    return (BtlRuntimeTask *)task;
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

void sndFormatResourceNameFromIndex(s32 skillIndex, char *output) {
    sprintf(output, D_003A5158, D_003BB6B0, (skillIndex + 0x200) & 0xffff);
}

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A50D8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A50F0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5110);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5138);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5158);

void sndFormatResourceNameFromUnitMode(const BtlUnit *unit, char *output) {
    sprintf(output, "MDD_%03X.ADB", unit->partyRecord.unitId);
}

s32 sndMapResourceType(s32 sound, s32 index) {
    s32 type = sndLookupResourceType(sound, index);
    if (type < 0x1A && type != 0) {
        if (type >= 0xB) {
            return type + *(s32 *)((u8 *)btlGetRuntime() + 0x1E4) - 6;
        }
        return type + 0xFFFF;
    }
    return -1;
}

/* Allocate a cleared active-sound node and prepend it to its independent list. */
ActiveSoundNode *sndAllocListNode(void) {
    ActiveSoundNode *node = sdfAllocAndClearQuadwords(sizeof(ActiveSoundNode));
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

typedef struct SoundTaskArgs {
    BtlUnit *actor;
    s32 option;
    u32 slot;
    s32 waitFrames;
} SoundTaskArgs;

extern SoundSlotTableEntry *btlSelectSideIndexedActorParameterTable(s32 category, s32 actorId);

/* Return the category/id/slot's packed motion-SE key, or zero if unavailable. */
u32 sndBuildMotSeResourceKey(const SoundSlotOwner *sound, u32 slot) {
    u32 id = sound->work.id;
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
void sndLoadMotSeFiles(SoundSlotOwner *sound) {
    char filename[0x70];
    u32 slot = 0;
    s32 offset = 0x10;
    u8 *handleTable = (u8 *)sound + 8;
    do {
        u32 id = sndBuildMotSeResourceKey(sound, slot);
        if (id != 0) {
            if (slot != 0xB) {
                sprintf(filename, D_003A5158, D_003BB6B0, id >> 16);
            } else if (sound->category == 0) {
                sprintf(filename, D_003A5178, D_003A5188, sound->work.id);
            } else {
                sprintf(filename, D_003A5198, D_003A5188, sound->work.id);
            }
            *(struct FileRequest **)(handleTable + offset) = fileQueueDefaultCallbackRequest(filename);
            btlBossDebugPrintf(D_003A51A8, slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound->flags |= SOUND_SLOT_FILE_LOAD_PENDING;
}

/* Find the shared category/id owner, returning null if absent. */
SoundSlotOwner *sndFindListNodeForChannel(s32 category, s32 id) {
    s32 context = (s32)(u32)btlGetRuntime();
    SoundSlotOwner *node = ((BtlActorWork *)context)->soundSlotOwners;
    while (node != 0) {
        if (node->category == category && node->work.id == id) {
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
    node = sdfAllocAndClearQuadwords(sizeof(SoundSlotOwner));
    node->category = category;
    node->work.id = id;
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
        sndLoadMotSeFiles(node);
    }
    return node;
}

/* The last reference cleans queued files and resource handles, then unlinks/frees. */
void sndReleaseSlotOwner(SoundSlotOwner *node) {
    u32 count = node->work.refCount - 1;
    node->work.refCount = count;
    if (count == 0) {
        u32 i = 0;
        struct SdfMemBlock **resources = node->work.resourceHandles;
        struct FileRequest **requests = node->work.fileRequests;
        for (; i < 0x1D; i++, requests++, resources++) {
            if (*requests != 0) {
                filePollEntryCleanup(*requests);
            }
            if (*resources != 0) {
                sdfReleaseResourceAllocation(*resources);
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
        if (node->flags & SOUND_SLOT_TRACK_LOADING) {
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
    if (owner->flags & SOUND_SLOT_FILE_LOAD_PENDING) {
        return;
    }
    if (!(owner->flags & SOUND_SLOT_FILES_READY)) {
        return;
    }
    if (owner->work.resourceHandles[args->slot] == 0) {
        return;
    }
    table = btlSelectSideIndexedActorParameterTable(owner->category, owner->work.id);
    entry = &table[args->slot];
    args->option = entry->fileId;
    if (args->slot != 0xB) {
        owner->work.pendingSoundId = sndBuildMotSeResourceKey(owner, args->slot);
        owner->work.pendingSlot = args->slot;
        owner->flags |= SOUND_SLOT_TRACK_LOAD_REQUESTED;
        owner->flags &= ~SOUND_SLOT_TRACK_LOADING;
        owner->flags &= ~SOUND_SLOT_TRACK_READY;
    }
}

extern s32 mnuGetSoundBufferStateLocked(void);

extern void mnuResetSoundBufferLocked(void);

extern void mnuReleaseSoundBufferLocked(void);

extern void mnuPrintTitleDebugBanner(void);

extern void mnuInstallTitleSoundBufferFromData(u32, u32, s32);

#include "sdf_debug_api.h"

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5178);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5188);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5198);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A51A8);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A51D0);

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
    if (owner->flags & SOUND_SLOT_FILE_LOAD_PENDING) {
        return 1;
    }
    if (!(owner->flags & SOUND_SLOT_FILES_READY)) {
        return 1;
    }
    soundWork = &owner->work;
    if (soundWork->resourceHandles[args->slot] == 0) {
        return 1;
    }
    if (args->slot != 0xB) {
        key = sndBuildMotSeResourceKey(owner, args->slot);
        if (owner->flags & SOUND_SLOT_TRACK_READY) {
            sndSetStationedSeHighVolume(key);
            btlBossDebugPrintf("btl:motSE play[%X-%X]\n", key >> 16, key & 0xFFFF);
            return 1;
        }
        key >>= 16;
        btlBossDebugPrintf("btl:motSE load wait[%X]\n", key);
    } else {
        if (work->motionSeLoadFrame >= 0xB) {
            if (mnuGetSoundBufferStateLocked() != 0) {
                mnuResetSoundBufferLocked();
                mnuReleaseSoundBufferLocked();
            }
            data = (void *)sdfMemoryGetBlockAddress(soundWork->resourceHandles[args->slot]);
            size = sdfMemoryGetBlockSize(soundWork->resourceHandles[args->slot]);
            mnuInstallTitleSoundBufferFromData(data, size, 2);
            sdfDebugPrintf("%%%%%%%%%%%%%%%% EARRING : %d\n", args->slot);
            mnuPrintTitleDebugBanner();
            work->motionSeLoadFrame = 0;
            btlBossDebugPrintf("btl:motSE play(ATRAC3)\n");
        } else {
            btlBossDebugPrintf("btl:motSE ignore(ATRAC3)[frame:%d]\n", work->motionSeLoadFrame);
        }
        return 1;
    }
    if (args->waitFrames > 90) {
        owner->flags &= ~SOUND_SLOT_TRACK_LOADING;
        owner->flags |= SOUND_SLOT_TRACK_READY;
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

/* Disable the motion- and skill-SE load-frame counters with their -1 sentinel. */
void btlResetMotionSoundFrameCounters(void) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();

    work->motionSeLoadFrame = -1;
    work->skillSeLoadFrame = -1;
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

void btlPollMotionSoundResources(void) {
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
                        struct SdfMemBlock *resource = fileGetResourceHandle(
                            owner->work.fileRequests[slot]);

                        struct FileRequest *completedRequest = owner->work.fileRequests[slot];

                        owner->work.resourceHandles[slot] = resource;
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

    if ((s32)state->motionSeLoadFrame >= 0) {
        state->motionSeLoadFrame++;
    }
    if ((s32)state->skillSeLoadFrame >= 0) {
        state->skillSeLoadFrame++;
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
                    u32 address = sdfMemoryGetBlockAddress(work->resourceHandles[owner->work.pendingSlot]);

                    func_002E9450((s32)address, size);
                    owner->flags = (owner->flags & ~SOUND_SLOT_TRACK_LOAD_REQUESTED) | SOUND_SLOT_TRACK_LOADING;
                    owner->flags &= ~SOUND_SLOT_TRACK_READY;
                    break;
                }
            }
        }
    }
}

/* Return whether a not-yet-file-ready owner still has an outstanding request. */
s32 sndHasOccupiedNodeSlots(void) {
    s32 context = (s32)(u32)btlGetRuntime();
    SoundSlotOwner *node = ((BtlActorWork *)context)->soundSlotOwners;
    while (node != 0) {
        if ((node->flags & SOUND_SLOT_FILES_READY) == 0) {
            u32 index = 0;
            struct FileRequest **requests = node->work.fileRequests;
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
        sdfDebugPrintf("%%%%%%%%%%%%%%%% EARRING(2)\n");
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

extern BattleVoiceEntry D_00377650[];

extern char D_003A5328[];

extern char D_003A5340[];

extern char D_003A5358[];

s32 sndPollAtrac3SELoadTask(Atrac3LoadTaskArgs *args) {
    char path[0x80];
    struct SdfMemBlock *resource;
    u32 data;
    u32 size;

    if (args->state == 0) {
        sprintf(path, D_003A5328, D_00377650[args->entryIndex].fileName);
        args->request = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A5340, path);
    } else if (fileIsRequestReadyInCurrentMode(args->request) != 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        resource = fileGetResourceHandle(args->request);
        data = sdfResourceRetainAddress(resource);
        size = (s32)fileGetResourceSize(args->request);
        filePollEntryCleanup(args->request);
        mnuInstallTitleSoundBufferFromData(data, size, D_00377650[args->entryIndex].volume);
        sdfReleaseResourceAllocation(resource);
        btlBossDebugPrintf(D_003A5358);
        return 1;
    }
    args->state++;
    return 0;
}

BtlRuntimeTask *sndCreateAtracEffectLoadTask(u32 entryIndex) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(Atrac3LoadTaskArgs));
    Atrac3LoadTaskArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->taskId = 0x58;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->callback = sndPollAtrac3SELoadTask;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    arguments = btlGetTaskArguments(task);
    arguments->request = 0;
    arguments->state = 0;
    arguments->entryIndex = entryIndex;
    return task;
}

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
            sprintf(path, D_003A5178, D_003A5188, id);
        } else {
            sprintf(path, D_003A5198, D_003A5188, unit->partyRecord.unitId);
        }
        args->request = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A5370, path);
    }
    ++*(u16 *)(work + 0x260);
}

extern char D_003A5390[];

extern char D_003A53B0[];

extern char D_003A53D0[];

u32 sndUpdateEarringDeadPlayback(BtlDeadLoadArgs *args) {
    u32 data;
    u32 size;

    if (args->request == 0) {
        return 1;
    }
    if (args->resourceAllocation == 0) {
        if (fileIsRequestReadyInCurrentMode(args->request) != 0) {
            args->resourceAllocation =
                fileGetResourceHandle(args->request);
            data = sdfResourceRetainAddress(args->resourceAllocation);
            size = (s32)fileGetResourceSize(args->request);
            filePollEntryCleanup(args->request);
            mnuInstallTitleSoundBufferFromData(data, size, 2);
            mnuPrintTitleDebugBanner();
            sdfDebugPrintf(D_003A5390);
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

void sndFinishEarringPlaybackTask(BtlDeadLoadArgs *args) {
    u8 *state = (u8 *)btlGetRuntime();
    if (args->resourceAllocation != 0) {
        sdfReleaseResourceAllocation(args->resourceAllocation);
    }
    --*(u16 *)(state + 0x260);
}

BtlRuntimeTask *sndCreateEarringPlaybackTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlDeadLoadArgs));
    BtlDeadLoadArgs *arguments;

    task->startCondition.kind = BTL_TASK_CONDITION_ALWAYS;
    task->endCondition.kind = BTL_TASK_CONDITION_NEVER;
    task->taskId = 0x59;
    task->flags &= ~BTL_TASK_FLAG_REGISTERED;
    task->ownerId = unit->identity;
    task->onStart = sndStartDeadAtracLoad;
    task->callback = sndUpdateEarringDeadPlayback;
    task->onFinish = sndFinishEarringPlaybackTask;
    arguments = btlGetTaskArguments(task);
    arguments->unit = unit;
    arguments->request = 0;
    arguments->resourceAllocation = 0;
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
    btlMeasureMaskedUnitBounds(0x400, 0, 0);
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

extern f32 atanf(f32);

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
            f32 halfWidth = unit->bodyRadius * unit->scale;
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
    spacingAngle = atanf(spacing / radius);
    totalArcAngle = -spacingAngle;
    for (i = 0; i < count; i++) {
        unit = actors[i];
        distance = unit->bodyRadius * unit->scale;
        halfAngle = atanf(distance / radius);
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
        halfAngle = atanf(unit->bodyRadius * unit->scale / radius);
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

s64 btlArrangeOpponentFormationSlots(void) {
    return btlArrangeFormationSlots(0x400);
}



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
    radius = btlMeasureMaskedUnitBounds(0x400, 0, 0) + 200.0f;
    VU0_STORE_VF_UNCLOBBERED(vf10, center);
    pos[0] = center[0];
    pos[1] = 0.0f;
    pos[2] = center[2] - radius;
    btlSetUnitPosition(slot[1], pos);
    if (btlAimHorizontalDirectionVU(pos, center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[1], (s128 *)rotation);
    }
    angle = 30.0f * 0.017453293f;
    pos[0] = center[0] - sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition(slot[0], pos);
    if (btlAimHorizontalDirectionVU(pos, center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(slot[0], (s128 *)rotation);
    }
    pos[0] = center[0] + sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition(slot[2], pos);
    if (btlAimHorizontalDirectionVU(pos, center) != 0) {
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
        radius = target->bodyRadius * target->scale;
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
    radius = slot[1]->bodyRadius * slot[1]->scale;
    radius += 100.0f;
    VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_STORE_VF_UNCLOBBERED(vf10, center);
    btlUnitGetMuzzlePosVU(slot[1]);
    VU0_LOAD_VF(vf11, center);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, center);
    center[1] = 0.0f;
    for (i = 0; i < 3; i++) {
        radius = slot[i]->bodyRadius * slot[i]->scale;
        radius += 100.0f;
        if (i != 1) {
            if (i == 0) {
                sdfVuLoadRotationMatrixZ(2.094395f);
            } else if (i == 2) {
                sdfVuLoadRotationMatrixZ(-2.094395f);
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
        btlMeasureMaskedUnitBounds(target->status.flags & 0x600, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU((f32 *)&vec[0], (f32 *)&vec[1]) != 0) {
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
            btlCopyUnitCurrentPosition((u8 *)slot[i], position);
            position[0] += offsetX;
            position[2] += offsetZ;
            btlSetUnitPosition(slot[i], position);
            btlUnitFaceTarget(slot[i], target);
        }
    }
}

/* Enemy formation row kind 7 deliberately leaves the supplied participants unchanged. */
void btlEnemyMode7FormationNoOp(BtlTask *link, BtlUnit *first, BtlUnit *second) {
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

extern BattleFormationActionTable *D_003BAA64;

/* Read-only spacing cells from the actor resource archive: first float at
 * +0x38, then one cell per 0x14-byte slot. This view owns no archive storage. */
typedef struct BtlFormationSpacingCell {
    f32 spacing;
    u8 unread04[0x10];
} BtlFormationSpacingCell;

typedef char BtlFormationSpacingCellSizeCheck[(sizeof(BtlFormationSpacingCell) == 0x14) ? 1 : -1];

void btlApplyRadialLinkageFormation(BtlLinkedCommand *command) {
    BtlUnit *participants[3];
    f32 targetPosition[4] __attribute__((aligned(16)));
    f32 oldPosition[4] __attribute__((aligned(16)));
    f32 newPosition[4] __attribute__((aligned(16)));
    BtlUnit *target;
    const BtlFormationSpacingCell *spacing;
    u8 *actorStatusAddress;
    s32 actionCode;
    s32 selector;
    u32 i;
    f32 distance;
    f32 flatY;

    actionCode = command->actionCode;
    if (((u32)actionCode - BATTLE_FORMATION_ACTION_FIRST) >= BATTLE_FORMATION_ACTION_COUNT) {
        return;
    }
    if (btlGetIndexListCount(command->targetList) != 1) {
        return;
    }
    target = btlGetIndexListEntry(command->targetList, 0);
    btlUnitGetMuzzlePosVU(target);
    /* Audited SDK store: this vector is read only by the VU load below. */
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPosition);
    participants[0] = command->linkedA;
    participants[1] = command->linkedB;
    participants[2] = command->task->unit;
    flatY = 0.0f;
    for (i = 0; i < 3; i++) {
        BtlUnit *actor = participants[i];
        const BattleFormationActionRow *row;
        if (actor == NULL) {
            continue;
        }
        row = (const BattleFormationActionRow *)((u32)D_003BAA64 +
              actionCode * sizeof(BattleFormationActionRow) -
              BATTLE_FORMATION_ACTION_FIRST * sizeof(BattleFormationActionRow));
        selector = row->actorSelector[i];
        if (selector == -1) {
            continue;
        }
        actorStatusAddress = btlGetSideIndexedActorStatusTable(actor->resourceKind, actor->species);
        spacing = (const BtlFormationSpacingCell *)(selector * sizeof(BtlFormationSpacingCell) + (s32)(u32)actorStatusAddress + 0x38);
        distance = spacing->spacing * participants[i]->scale + target->reach * target->scale;
        btlCopyUnitCurrentPosition((u8 *)participants[i], oldPosition);
        btlUnitGetMuzzlePosVU(participants[i]);
        VU0_LOAD_VF(vf11, targetPosition);
        VU0_SUB(vf10, vf10, vf11);
        VU0_SET_VF10_COMPONENT(y, flatY);
        VU0_NORMALIZE_VF10();
        VU0_SCALAR_OP_CLOBBER(distance, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, targetPosition);
        VU0_ADD(vf10, vf10, vf11);
        /* The vector is complete here; Y is replaced, then the callee consumes it. */
        VU0_STORE_VF_UNCLOBBERED(vf10, newPosition);
        newPosition[1] = oldPosition[1];
        btlSetUnitPosition(participants[i], newPosition);
    }
}

typedef struct BattleFormationActionArgs {
    BtlTask *link;
    BtlUnit *first;
    BtlUnit *second;
    u32 actionId;
    u32 unk10;
} BattleFormationActionArgs;


/* Select the linkage formation by command row; task completion is always one. */
INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5328);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5340);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5358);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5370);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5390);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A53B0);

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A53D0);

u32 btlApplyLinkageFormation(BattleFormationActionArgs *args) {
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
            btlEnemyMode7FormationNoOp(args->link, first, args->second);
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

extern u32 btlApplyLinkageFormation(BattleFormationActionArgs *);

/* Package the action, two companion addresses, skill and flags for formation. */
u8 *btlCreateLinkageFormationTask(u8 *owner, u32 firstActorAddress, u32 secondActorAddress, u32 skillId, u32 flags) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;
    u8 *unit;

    task[0] = BTL_TASK_CONDITION_ALWAYS;
    task[0x10] = BTL_TASK_CONDITION_NEVER;
    *(void **)(task + 0x4C) = btlApplyLinkageFormation;
    unit = *(u8 **)(owner + 0x18);
    *(u16 *)(task + 0x20) = 0x5F;
    *(u32 *)(task + 0x48) = 0;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = firstActorAddress;
    arguments[2] = secondActorAddress;
    arguments[3] = skillId;
    arguments[4] = flags;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001DEFE0", D_003A5410);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB670);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB678);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB680);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB690);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB694);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", btlTintTransitionHoldCount);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB6A0);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB6A8);

INCLUDE_SDATA(const s32, "game/code_001DEFE0", D_003BB6B0);

