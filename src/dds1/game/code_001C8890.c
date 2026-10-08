#include "common.h"
#include "sdf_model.h"
#include "btl.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "evt_unit.h"
#include "mdl.h"
#include "btl_action.h"
#include "sdf.h"
#include "file.h"


extern s32 mdlGetNodeField2C(MdlCtx *, s32);
extern void effObjSetOpacityPassEnabled(u32 enabled);
extern u32 mdlGetBroadcastValue(MdlCtx *);

extern void sdfReleaseChipBlock(void *block);
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
extern s32 kwlnTaskIsRegistered(s32);
extern void btlDebugPrintf(const char *, ...);
extern void btlBossDebugPrintf(const char *format, ...);
extern u32 effMiscRandMod(void *state, u32 modulus);
extern u64 btlStartTask(void *);
extern void btlDispatchStateHandler(void *, s32);


typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;


struct CameraPoseAction;

typedef struct BattleController {
    u8 pad_000[0x160];
    u32 runtimeFlags;
    void *activeSlot;
    u8 pad_168[0x20];
    struct BtlIndexList *pendingSoundList;
    u8 pad_18C[0x68];
    u32 flags;
    u8 pad_1F8[0x30];
    BtlUnit *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad_254[0x28];
    s32 mode;
    u8 pad_280[0x10];
    s32 fieldF1; /* 0x290: field resource allocations */
    s32 fieldF2;
    s32 fieldF3;
    s32 taskParent;
    u8 pad_2A0[4];
    s32 boundTask;
    u8 pad_2A8[4];
    s32 spriteObject;
    u8 pad_2B0[0x24];
    SceneSlot slots[8];
    BtlTask *groupPrimary[20];
    BtlTask *groupSecondary[45];
    BtlTask *groupTertiary[15];
    u8 pad_42C[0x184];
    s32 (*sceneCallback)();
    u8 pad_5B4[0x5C];
    s32 (*actionCameraInitHook)(struct CameraPoseAction *); /* 0x610 */
    u8 pad_614[0x18];
    s32 (*actionCameraStepHook)(u8 *); /* 0x62C */
} BattleController;

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
    u8 pad80[0x10];
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

extern s32 D_0035D9F0[];

extern s32 D_0035DA08[];

extern void func_001E9DE0(s32, s32, s32);

extern void func_001EB368(s32, s32);

extern s32 btlCountTasksForOwner(s64);


typedef struct SoundBankEntry {
    u32 unk_00;
    u32 resource;
    u32 unk_08;
} SoundBankEntry;

extern SoundBankEntry D_0035F748[];


typedef struct EffectLoadArgs {
    SoundEffectNode *effect;
    void *loadHandle;
    const char *name;
} EffectLoadArgs;

typedef struct BtlPosLerpTaskArgs BtlPosLerpTaskArgs;
extern s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *);

extern void btlUnitGetMuzzlePosVU(void *);

typedef struct BtlRotationTaskArgs BtlRotationTaskArgs;
extern s32 btlStepUnitRotationNlerp(BtlRotationTaskArgs *);

extern u64 btlAdvanceRuntimeSequenceCounter();

extern u8 *btlAllocateIndexedUnitEffectTask(u8 *, s32, s32, f32);


/* SYSEFF metadata and runtime registrations share these indices. */
enum {
    BTL_SOUND_ENTRY_COUNT = 0x31,
    BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26,
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
    void *owner;
    SoundVoice *effectHandle;
    u32 *effect;
    u16 variant;
    u16 unk_0E;
} SoundLink;

extern void btlUpdateUnitCommandEffect(SoundLink *);

typedef struct SoundResourceLink {
    void *owner;
    void *sound;
    u32 *task; /* SYSEFF word-array record passed to sndDeleteSystemEffect */
    u32 variant;
    u8 refreshRequested;
    u8 pad11[3];
} SoundResourceLink;

typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    s8 edge22;
    s8 edge23;
    u8 pad24[2];
    s8 prev;
    s8 next;
} SndPad;

extern SndPad D_00324510;

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern char D_003A5158[]; /* "%sMIDI%04X.SMG" */

extern s32 sndFindPackedTrackLoadStatus(u32);

typedef struct SceneLightRestoreArgs { u32 value; } SceneLightRestoreArgs;
extern s64 func_001F0998(SceneLightRestoreArgs *);
extern void evtSetUnitStatusFlags(EvtUnit *);
extern void evtInitializeUnitColorTransition(EvtUnit *, s32, u32, u32);
extern void evtSetUnitNormalizedDirection(EvtUnit *, s32);

extern s32 func_001F06E0(SceneLightRestoreArgs *);

extern s64 func_001F0B90(void);

extern s32 sndPlaySkillSeTask(u32 *);

extern void sndFormatResourceNameFromUnitMode(s32, s32);

extern u32 sndFinishEarringPlayback(void);

extern s32 sndTickFadeCounter();

extern s32 btlQueueTintTransitionWhenEnabled(u32 *);

extern void *sdfAllocAndClearQuadwords(s32);

extern s32 btlCreateMoveOtherUnitsTask();

extern s32 mnuPollTitleStreamStateLocked(void);

extern SoundResourceNode *sndAllocResourceNode(void);

extern u32 kwlnDrawControlFlags;
extern BtlUnit *btlCreateUnit(void);
extern void btlDestroyUnit(u8 *);
extern void func_001D4E60(BtlUnit *, BtlUnit *);
extern void sdfQueueNonzeroResourceId(s32);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 D_003BB694;

extern u32 btlTintTransitionHoldCount;

extern u8 D_0035F5D0[];

extern char D_003BB6B0[];

extern u32 dds3AdvanceWorldCounter(void);

extern struct EffWorldNode *evtSpawnActionObj9(s32);

extern s32 btlSetActorEffectParameter();

extern s32 mdlFlagTest(u32);

extern SoundTask *btlDeferredTaskTail;

extern SoundTask *btlDeferredTaskHead;

extern s32 func_00214868(void);

u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

extern s32 datEnemyRecords;

extern void func_001DEFE0(s32, s32, f32);

extern void func_001B83D8(BtlTask *, s8, s8);

extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);

extern s32 btlGetRuntime(void);

extern s32 datItemSkillRecords;

extern s32 datCommandSelectors;

extern s32 datCommandRecords;

extern s32 datActionAnimationRecords;

extern s32 func_001F5028(s32 arg0);

extern s8 effSharedRandomState[];

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfResourceRetainAddress(u32);

extern void sndResetTransition(void);

extern void func_001F4430(void);

extern void btlResetDeferredTaskQueue(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

void btlAdjustUnitHp(u8 *object, s32 value);

void btlAdjustUnitMp(u8 *object, s32 value);

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(DatPartyRecord *object);

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(DatPartyRecord *object);

void func_001A1948();

extern s8 effSharedRandomState[];

extern SndPad D_00324510;

extern void func_001B83D8(BtlTask *, s8, s8);

extern void sndSetStationedSeVolume(u32);

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfResourceRetainAddress(u32);

extern void btlResetTitleStreamOnBattleFlag(void);

extern void func_001DC0E8(void);

extern void btlAdvanceWorldCounterAndSpawnActionObject(void);

extern void btlCreateRainEffect(u32, u32);

extern void btlRepositionPartyAroundBattleCenter(void);

extern s32 func_001A3638(void);

extern void kwlnFadeStartIn(s32);

extern s32 btlAreWorkBuffersReady(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

s32 btlBothSidesActive(BtlUnit *unit);

/* No selected entry is represented by -1. */ void btlClearActorSelectedEntryIndex(s32 actor);

void btlClearAllActorEntrySlots(u32 arg0);

void btlClearSceneTaskActiveFlag(s32 arg0);

s32 btlGetActorBedAssetIdFromIndex(s32 arg0);

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *entry);

struct EvtUnit;
extern void evtSetUnitRgbTransition(struct EvtUnit *, s32, u32);
extern void evtSetUnitAlphaTransition(struct EvtUnit *, s32, u32);
/* Retail passes the full motion index; only the stored field is a halfword. */
extern void evtUnitSetStoredParameter(struct EvtUnit *, s32);
extern void evtSetTransitionMotionScale(struct EvtUnit *, f32);
extern s32 btlIsCurrentValueBelowQuarterThreshold(void *);
extern s32 btlTestActorStatusPredicate(BtlUnit *);
extern void btlApplyUnitModelScaledValue(BtlUnit *);
extern s32 btlIsActorModeAcceptedByBattleHook(u8 *);
extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);
extern void btlRefreshUnitMotionSelection(u8 *);
extern void btlRefreshUnitEffectMotionAndEntry(u8 *);
extern void mdlAddEntryFlagged(void *, s32, s32);
extern void mdlAddEntryPlain(void *, s32, s32);
extern void sdfMotionSampleAtFrame(Motion *, f32);
extern void evtPrepareUnitMotionState(struct EvtUnit *, s32, s32, s32, s32);
extern void evtStoreUnitMotionShortParameters(struct EvtUnit *, s32, s32);
extern s32 btlGetSlotRateKind(u8 *, s32);
extern u8 *btlCreateStiffenDamageShakeTask(u8 *, f32);

s32 btlGetLoggedIndexedCommandItem(s32 index);

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta);

/* Set the actor's selected entry index. */ void btlSetActorSelectedEntryIndex(s32 actor, u32 index);

void btlSetTrackedTaskDisplayMode(s32 mode);

void btlSyncPlayerWork(BtlUnit *actor);

void fldAppendTaskToGroup(BtlTask *task);

void fldCreateSceneSpriteTask(s32 arg0);

void func_001A1960(DatPartyRecord *record, s32 mask);

typedef struct BattleActionLinkState {
    u8 pad00[0x18];
    BtlUnit *unit;
    u8 pad1C[4];
    s32 command; /* 0x20: command kind passed to the operand dispatcher */
    union {
        u32 cursorKind;
        u16 cursorKindLow;
    };
    u8 pad28[0x38];
    struct BtlIndexList *actorIndices;
    u8 pad64[4];
    s64 owner; /* 0x68: sole indexed actor's identity */
    u8 pad70[0x10];
    struct BtlOperandGroup *groups; /* 0x80 */
} BattleActionLinkState;

typedef struct CameraPoseTransform {
    f32 position[4];
    f32 direction[4];
    f32 distance;
    f32 fov;
} CameraPoseTransform;

typedef struct CameraPoseAction {
    CameraPoseTransform transform;
    u8 pad28[8];
    CameraPoseTransform fromPose;
    u8 pad58[0x68];
    CameraPoseTransform savedPose;
    u8 padE8[8];
    u32 flags;
    BattleActionLinkState *link;
    u8 padF8[0xC];
    u32 status;
    s32 actionKind;
    u16 cameraKind;
    u8 pad10E[2];
    s32 state;
    s32 actionId; /* 0x114: command record and action-animation index */
    struct BtlIndexList *actorIndices;
    s32 unk11C;
    u8 pad120[0xC];
    s32 unk12C;
    f32 unk130;
} CameraPoseAction;

extern s32 func_001EB1B0(s32, CameraPoseTransform *, s8, s8);

typedef struct BtlActionPoseRuntime {
    u8 pad00[0x1FC];
    u32 flags;
    u8 pad200[0x28];
    BtlUnit *actorHead;
} BtlActionPoseRuntime;

typedef struct BattlePoseBlendState {
    u8 pad00[0x130];
    f32 duration;
} BattlePoseBlendState;

typedef struct BtlWorkPoseBlendHook {
    u8 pad00[0x628];
    s32 (*hook628)(BtlUnit *, s32, s32);
} BtlWorkPoseBlendHook;

typedef struct BtlCategoryTableEntry {
    u8 flags00;
    u8 pad01[2];
    u8 kind03;
    u8 pad04[0x2A];
    u16 unk2E;
    s32 categoryType;
    u8 pad34[4];
} BtlCategoryTableEntry;

typedef struct BtlStatArgs {
    BtlUnit *unit;
    s32 amount;
    s32 category;
} BtlStatArgs;


typedef struct BtlFxSrcA {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
} BtlFxSrcA;

/* Shared effect-resource records also provide approach reach and frame limits. */
typedef struct BtlEffectNode {
    s16 triggerKind;
    u8 pad02[2];
    s16 rateKind;
    u8 pad06[2];
    f32 scale;
    f32 reachOffset;
    u8 pad10[2];
    u16 frameCount;
} BtlEffectNode;

typedef struct BtlEffectResource {
    s128 vec0;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    f32 f20;
    u8 pad24[8];
    BtlEffectNode nodes[1];
} BtlEffectResource;


typedef struct BtlApproachTaskArgs {
    BtlUnit *unit;
    BtlUnit *target;
    f32 offset;
    f32 scale;
    s32 unk10;
    s32 count;
} BtlApproachTaskArgs;

typedef struct BtlRotationTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
} BtlRotationTaskArgs;

/* Constructor-owned model-change arguments; btlAllocTask reserves 0x1C bytes. */
typedef struct BtlModelChangeArgs {
    BtlUnit *unit;
    u32 resourceKind;
    u32 resourceId;
    s32 delay;
    u32 duration;
    s32 elapsed;
    u8 phase;
    u8 transitionMode;
    u8 reserved1A[2];
} BtlModelChangeArgs;
typedef char BtlModelChangeArgsSizeCheck[(sizeof(BtlModelChangeArgs) == 0x1C) ? 1 : -1];
typedef char BtlModelChangeArgsPhaseCheck[((u32)&((BtlModelChangeArgs *)0)->phase == 0x18) ? 1 : -1];

typedef struct BtlFadeArgs {
    BtlUnit *unit;
    s32 fadeIn;
    s32 fadeOut;
    u32 count;
    u32 color;
} BtlFadeArgs;

typedef struct BtlEffObjInner {
    u8 pad00[0x60];
    f32 vec60[4];
    u8 pad70[0x50];
    u32 flagsC0;
} BtlEffObjInner;

typedef struct BtlEffObj {
    u8 pad00[0x1C];
    BtlEffObjInner *inner;
} BtlEffObj;

typedef struct BtlCameraResetWork {
    u8 pad_000[0x174];
    s32 activeUnitId;
    u8 pad_178[0xB0];
    BtlUnit *actorList;
} BtlCameraResetWork;



typedef struct BtlActionLink {
    u8 pad_00[0x18];
    BtlUnit *unit;
} BtlActionLink;

typedef struct BtlAction {
    u8 pad_00[0xF4];
    BtlActionLink *link;
} BtlAction;


typedef struct BtlCursorActionLink {
    u8 pad_00[0x18];
    BtlUnit *unit;
} BtlCursorActionLink;

typedef struct BtlCursorAction {
    u8 pad_00[0xF4];
    BtlCursorActionLink *link;
} BtlCursorAction;


typedef struct BattleVoiceEntry {
    u8 volume;
    u8 pad01[3];
    char fileName[0xC];
} BattleVoiceEntry;


#define CURSOR (&D_0035F100)

void btlActionSeqStateSelect(u8 *task) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *unit = *(u8 **)(task + 0x18);
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
        flags = *(u32 *)(unit + 0x110);
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
    u8 *unit = *(u8 **)(task + 0x18);
    u32 flags = *(u32 *)(unit + 0x110);
    if (flags & 0x200) {
        if (flags & 0x1000) {
            if ((*(u32 *)(unit + 0x114) & 0x40) && !(*(u16 *)(unit + 0x12E) & 0x7C0E) &&
                !(*(u32 *)(task + 8) & 0x100)) {
                *(u16 *)(task + 0x50) = 4;
                *(s32 *)(task + 0x54) = effOffsetIfOwnerFlagClear(unit, 0xA4);
                *(u32 *)(unit + 0x110) = (*(u32 *)(unit + 0x110) & ~0x20) | 0x400000;
                *(u16 *)(unit + 0x120) |= 0x4000;
                *(u32 *)(unit + 0x114) |= 0x2000;
                btlDispatchStateHandler(task, 0x10);
            } else {
                btlDispatchStateHandler(task, 0x1D);
            }
            *(u32 *)(unit + 0x114) &= ~0x40;
        } else {
            btlDispatchStateHandler(task, 0x1D);
        }
    } else {
        btlDispatchStateHandler(task, 0x1D);
    }
}

extern s32 sndIsResourceNodeReferencedOrActive(s32);
extern s32 sndHasResourceFlagsOneOrEight(s32);
extern void sndFreeResourceNode(SoundResourceNode *);
extern void sndFreeListNode(ActiveSoundNode *);
extern u32 sndGetResourceStatus(u32 *);
extern s32 btlCountTasksByKind(u16 kind);

s32 fldCheckSceneResourcesIdle(s32 self) {
    BtlActorWork *scene = (BtlActorWork *)btlGetRuntime();
    BtlUnit *actor;

    for (actor = scene->actorList; actor != 0; actor = actor->next) {
        if (actor->resourceNode != 0) {
            if (sndIsResourceNodeReferencedOrActive(actor->resourceNode) != 0) {
                if ((s32)actor == self) {
                    return 0;
                }
                sndGetResourceStatus((u32 *)actor->resourceNode);
                return 0;
            }
            sndFreeResourceNode((SoundResourceNode *)actor->resourceNode);
            actor->resourceNode = 0;
        }
    }
    if (((BtlUnit *)self)->listNode != 0) {
        if (sndHasResourceFlagsOneOrEight(((BtlUnit *)self)->listNode) != 0) {
            return 0;
        }
        sndFreeListNode((ActiveSoundNode *)((BtlUnit *)self)->listNode);
        ((BtlUnit *)self)->listNode = 0;
    }
    for (actor = scene->actorList; actor != 0; actor = actor->next) {
        if (actor->flags & 0x200) {
            if (actor->flags & 2) {
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
    if (actor->unk314 != 0) {
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

void btlReleaseIdleUnitSoundAndAdvanceTask(u8 *task) {
    u8 *unit = *(u8 **)(task + 0x18);
    u32 flags;
    u32 masked;
    s32 resource = *(s32 *)(unit + 0x2F8);

    *(u32 *)(task + 8) &= ~0x100;
    if (resource != 0) {
        if (sndIsResourceNodeReferencedOrActive(resource) == 0) {
            sndFreeResourceNode(*(s32 *)(unit + 0x2F8));
            *(s32 *)(unit + 0x2F8) = 0;
        }
    }
    flags = *(u32 *)(unit + 0x110);
    if ((flags & 0x400) == 0 && (*(u16 *)(unit + 0x12E) & 0x4000) == 0) {
        if (*(s32 *)(unit + 0xC8) == 0x1F) {
            func_001A1948(&((BtlUnit *)unit)->partyRecord, 0x1000);
            flags = *(u32 *)(unit + 0x110);
        }
        masked = flags & ~0x20;
        masked &= ~0x08000000;
        *(u32 *)(unit + 0x110) = masked;
        btlFlagUnitDefeatCandidate(unit);
        btlRefreshUnitMotionSelection(unit);
        btlStartTask(btlAllocateIndexedUnitEffectTask(unit, 0xE, 0, 1.0f));
        fldAppendTaskToGroup((BtlTask *)task);
        btlDispatchStateHandler(task, 2);
    }
    return;
}

void func_001C8E60(s32 arg0) {
    *(u32 *)(arg0 + 8) = (*(u32 *)(arg0 + 8) | 0x10) & ~0x200;
}

void btlTaskUpdateFlags(u8 *task) {
    u8 *unit = *(u8 **)(task + 0x18);
    u32 flags;
    if (!(*(u32 *)(unit + 0x110) & 0x20)) {
        *(u32 *)(task + 8) &= ~0x100;
    }
    if (*(s32 *)(unit + 0x2F8) != 0 && sndIsResourceNodeReferencedOrActive(*(s32 *)(unit + 0x2F8)) == 0) {
        sndFreeResourceNode(*(s32 *)(unit + 0x2F8));
        *(s32 *)(unit + 0x2F8) = 0;
    }
    flags = *(u32 *)(unit + 0x110);
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
    BattleController *scene = (BattleController *)btlGetRuntime();
    u32 flags = scene->flags;
    s32 unit = *(s32 *)(task + 0x18);
    BtlUnit *actor;
    if (!(flags & 0x20)) {
        for (actor = scene->actors; actor != 0; actor = actor->next) {
            u32 actorFlags = actor->flags;
            if (actorFlags & 0x4000) {
                return;
            }
            if (actorFlags & 0x30400000) {
                return;
            }
        }
        if (!(flags & 0x8000) || fldCheckSceneResourcesIdle(unit) != 0) {
            if (btlBothSidesActive((BtlUnit *)unit) == 0) {
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

typedef struct BattleDeltaSpec {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
} BattleDeltaSpec;

void *btlCreateActorParameterDeltaTask(u8 *owner, BattleDeltaSpec *spec);
extern s32 evtRunContext(s32, s32, s32, s32, u16);
extern s32 btlRollAiBucket(void);
extern BtlRuntimeTask *btlCreateEffObjB();

/* Try to clear the unit's condition: 2 and 4 always clear, 1 needs battle mode 2, and the
 * others roll a script-supplied chance (capped at 70, scaled by ability 0x232). */
void func_001C9098(BattleActionLinkState *link) {
    BattleDeltaSpec spec;
    BtlUnit *unit;
    s32 chance;
    f32 scale;
    BtlRuntimeTask *task;

    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    unit = link->unit;
    unit->flags |= 0x4000;
    switch (unit->partyRecord.status & 0x7FFF) {
    case 8:
    case 0x20:
    case 0x200:
    case 0x1000:
        if (unit->stateFlags & 4) {
            unit->stateFlags &= ~4;
            break;
        }
        /* fallthrough */
    case 1:
        unit->stateFlags &= ~4;
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
        spec.unk_0C = 0x122F;
        btlStartTask(btlCreateActorParameterDeltaTask((u8 *)unit, &spec));
        if (unit->partyRecord.status & 0x1000) {
            unit->flags |= 0x20000000;
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
    u8 *ctx;
    u8 *task;
    u64 value;
    s32 count;

    if (sndHasActiveActor() != 0) {
        return;
    }
    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    ctx = *(u8 **)(arg0 + 0x18);
    value = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    if (*(u32 *)(ctx + 0x110) & 0x200) {
        btlStartTask(btlCreateCommandSoundTask(arg0, 9));
    } else {
        btlStartTask(btlCreateCommandSoundTask(arg0, 3));
    }
    switch (*(u16 *)(ctx + 0x12E) & 0x7FFF) {
    case 0x200:
        func_001FF0C8(arg0, 0);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x2000:
        func_001FF0C8(arg0, 1);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x20:
        if (*(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) & 0x200) {
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
        *(u64 *)(task + 0x40) = *(u64 *)(ctx + 0x108);
        btlStartTask(task);
        btlDispatchStateHandler(arg0, 0x1A);
        break;
    case 0x800:
        task = fldCreateSceneGroupAction(arg0, 0x64, 1);
        *task = 7;
        *(u64 *)(task + 8) = value;
        *(u64 *)(task + 0x40) = *(u64 *)(ctx + 0x108);
        btlStartTask(task);
        btlDispatchStateHandler(arg0, 0x1A);
        break;
    }
    count = func_001FD170(arg0);
    if (count > 0) {
        task = btlCreateEffObjB(ctx, count);
        *(u64 *)(task + 0x40) = value;
        btlStartTask(task);
    }
    *(u32 *)(arg0 + 8) |= 0x200;
}

void func_001C9628(u32 arg0) {
    btlGetRuntime();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    func_001BF4C0(arg0);
}

extern s32 fldGetSceneObjectState(void);
extern void fldSetSceneObjectAndGroupStates(void);
extern s32 btlAiCheckStatusRollEligibility(BtlTask *task);

void func_001C9660(BtlTask *task) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    s32 state;

    if (scene->flags & 0x20) {
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
    } else if (scene->flags & 0x8000) {
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
            btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0x82));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 6:
            btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0xB0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 7:
            btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0xB2));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 9:
            btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0xD0));
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

void func_001C9960(BtlTask *task) {
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
    BattleController *scene = (BattleController *)btlGetRuntime();
    u16 index;

    if (!(scene->flags & 0x20)) {
        if (sndHasActiveActor() == 0) {
            if (fldCheckSceneResourcesIdle((s32)task->unit) != 0) {
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
                    scene->flags &= ~0x100000;
                }
                if (scene->boundTask == 0) {
                    scene->flags |= 0x100000;
                    if (btlAiCheckStatusRollEligibility(task) != 0) {
                        btlDispatchStateHandler(task, 0xB);
                    } else {
                        btlDispatchStateHandler(task, 0xC);
                    }
                } else if (kwlnTaskIsRegistered(scene->boundTask) == 0) {
                    if (task->indexWork.phase == -1) {
                        btlBossDebugPrintf("btl:AI script return NULL[%p]\n", task);
                        btlDebugPrintf("AI script return NULL\n");
                        btlRunRandomWeightedAiTableAction(task);
                    }
                    scene->flags |= 0x100000;
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
        func_001D3FE8(arg0, arg0 + 0x20);
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
    u8 *model;
    if (sndHasActiveActor() != 0) {
        return;
    }
    model = (u8 *)btlGetIndexListEntry(*(struct BtlIndexList **)(actor + 0x60), 0);
    if (*(u32 *)(actor + 0x20) == 1 &&
        (btlIsActiveActor((s32)model) == 0 ||
         (*(u64 *)(model + 0x110) & 0xE1) != 1)) {
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
    u8 *unit;
    s64 ownerId;
    s32 effect;
    u8 *object;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x45) == 0) {
        unit = *(u8 **)(task + 0x18);
        ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        if (*(u32 *)(unit + 0x110) & 0x200) {
            btlStartTask(btlCreateCommandSoundTask(task, 9));
        } else {
            btlStartTask(btlCreateCommandSoundTask(task, 3));
        }
        if ((*(u16 *)(unit + 0x12E) & 0x7FFF) == 0x20) {
            if (*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x200) {
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

void btlCommandPrintAndFetchOwner(u8 *task) {
    BattleActionLinkState *link = (BattleActionLinkState *)task;
    s32 *commandPtr;
    s32 command;
    btlBossDebugPrintf(D_003A3648, link->command);
    commandPtr = &link->command;
    func_001D12A0(task, commandPtr);
    command = *commandPtr;
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
    if (btlGetIndexListCount(link->actorIndices) == 1) {
        link->owner = ((BtlUnit *)btlGetIndexListEntry(link->actorIndices, 0))->identity;
    }
}

void btlDispatchEffectCommandWhenActorReady(u8 *command) {
    u8 *actor = *(u8 **)(command + 0x18);
    if (fldCheckSceneResourcesIdle(actor) == 0) {
        return;
    }
    if (*(u16 *)(command + 0x50) == 2) {
        btlStartTask(btlCreateEffObjB(actor, *(u32 *)(command + 0x54)));
    }
    func_001F0CA0(command, command + 0x20);
    btlDispatchCommandViaHookOrDefault(command, command + 0x20);
}

void func_001CA1F0(void) {
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CA1F8);

void func_001CB408(void) {
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3648);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3658);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3670);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CB410);

void func_001CCD10(void) {
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CCD18);

void btlMarkLinkedActorStatusFlag(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
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
    SoundTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = (u16 *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
    if (!(unit->flags & 0x20)) {
        motion = func_001D6050(unit, 0x11) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->flags & 0x400020)) {
        unit->flags &= ~0x1000;
        unit->partyRecord.flags &= ~0x1000;
        sound = (SoundTask *)btlCreateMoveOtherUnitsTask(unit, 0x11);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = (SoundTask *)btlScheduleRefreshTask((u8 *)other);
                spawned->startCondition.kind = 4;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->modelId, unit->modelVariant, motion, 0x18,
                                                       0);
        change->startCondition.kind = 4;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = (SoundTask *)btlCreateModelLoadPollTask((u8 *)other, other->resourceKind, other->species, 0);
                load->startCondition.kind = 4;
                load->startCondition.value.handle = change->handle;
                load->owner = unit->identity;
                btlStartTask(load);
                spawned = (SoundTask *)btlCreateUnitFadeInTask((u8 *)other, 0, 0);
                spawned->startCondition.kind = 4;
                spawned->startCondition.value.handle = load->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask((u32)state->resources[45], (u8 *)unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = (SoundTask *)btlCreateEffObjA(unit, task->indexWork.phase);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateCommandSoundTask((s32)task, 0xE);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlAllocateIndexedUnitEffectTask((u8 *)unit, 0x11,
                                                                btlGetSlotRateKind((u8 *)unit, 0x11), 1.0f);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        spawned->startCondition.kind = 4;
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
            unit->flags &= ~0x20;
            if (unit->partyRecord.hp == 0) {
                unit->partyRecord.hp = 1;
                btlRefreshUnitMotionSelection((u8 *)unit);
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
        unit->flags &= ~0x1000;
        unit->partyRecord.flags &= ~0x1000;
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->modelId, unit->modelVariant, motion, 0x12,
                                                       3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask((u32)state->resources[47], (u8 *)unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->flags &= ~0x400000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1B);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

void func_001CE5D8(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
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
    SoundTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = (u16 *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
    if (!(unit->flags & 0x20)) {
        motion = func_001D6050(unit, 0x10) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->flags & 0x20000020)) {
        unit->flags |= 0x1000;
        unit->partyRecord.flags |= 0x1000;
        sound = (SoundTask *)btlCreateMoveOtherUnitsTask(unit, 0x10);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = (SoundTask *)btlScheduleRefreshTask((u8 *)other);
                spawned->startCondition.kind = 4;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->unkDC, unit->displaySpecies, motion, 0x18,
                                                       0);
        change->startCondition.kind = 4;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->next) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = (SoundTask *)btlCreateModelLoadPollTask((u8 *)other, other->resourceKind, other->species, 0);
                load->startCondition.kind = 4;
                load->startCondition.value.handle = change->handle;
                load->owner = unit->identity;
                btlStartTask(load);
                spawned = (SoundTask *)btlCreateUnitFadeInTask((u8 *)other, 0, 0);
                spawned->startCondition.kind = 4;
                spawned->startCondition.value.handle = load->handle;
                spawned->owner = unit->identity;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask((u32)state->resources[45], (u8 *)unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = (SoundTask *)btlCreateEffObjA(unit, task->indexWork.phase);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlCreateCommandSoundTask((s32)task, 0xE);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)btlAllocateIndexedUnitEffectTask((u8 *)unit, 0x10,
                                                                btlGetSlotRateKind((u8 *)unit, 0x10), 1.0f);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = (SoundTask *)fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        spawned->owner = unit->identity;
        btlStartTask(spawned);
        if (task->unit->partyRecord.status & 0x480) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    } else {
        unit->flags |= 0x1000;
        unit->partyRecord.flags |= 0x1000;
        change = (SoundTask *)btlCreateModelChangeTask((u8 *)unit, unit->unkDC, unit->displaySpecies, motion, 0x12,
                                                       3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask((u8 *)unit));
        btlStartTask(sndCreateEffectSourceTask((u32)state->resources[47], (u8 *)unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->flags &= ~0x20000000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1B);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

void func_001CEA58(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

extern u8 *btlCreateModelChangeTask(u8 *, s32, s32, s32, s32, u8);

typedef struct BtlActorMotionSlot {
    u8 pad00[4];
    s16 kind; /* 0x04: motion/effect-kind discriminator */
    s16 alphaStartFrame; /* 0x06 */
    f32 alphaFrameScale; /* 0x08 */
    u8 pad0C[4];
    s16 alphaDuration; /* 0x10 */
    u8 pad12[2];
} BtlActorMotionSlot;

typedef struct BtlActorStatusRecord {
    u8 pad00[0x2A];
    u16 model; /* 0x2A: model-change task operand */
    BtlActorMotionSlot motions[29]; /* 0x2C: record stride is 0x270 */
} BtlActorStatusRecord;

void func_001CEA70(u8 *task) {
    u8 *actor;
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
    actor = *(u8 **)(task + 0x18);
    statusTable = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
        *(s32 *)(actor + 0xC4), *(s32 *)(actor + 0xC8));
    model = statusTable->model;
    effectTask = (u8 *)btlCreateEffObjB(actor, 0x7E);
    btlStartTask(effectTask);
    modelTask = btlCreateModelChangeTask(actor, 0, 0x1F, model, 0x12, 1);
    btlStartTask(modelTask);

    *(u32 *)(actor + 0x110) = (*(u32 *)(actor + 0x110) | 0x1000) & 0xEFFFFFFF;
    *(u16 *)(actor + 0x120) |= 0x1000;
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

    if ((*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x200) != 0 &&
        (*(u32 *)(task + 8) & 0x20) != 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
    }
    switch (*(s32 *)(task + 0x20)) {
    case 14:
        *(u32 *)(*(u8 **)(task + 0x18) + 0x110) |= 0x2000000;
        effectId = 0xF;
        countdown = 0x64;
        break;
    case 10:
        if ((*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x400) != 0) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
        }
        effectId = (*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x200) ? 0xF : 0x1E;
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
    u8 *linked = *(u8 **)(task + 0x34);
    u8 *actor;
    u8 *object;

    if (linked != 0) {
        actor = linked;
    } else {
        actor = *(u8 **)(task + 0x18);
    }
    if (*(u32 *)(actor + 0x110) & 0x200) {
        btlBossDebugPrintf("return:player=%X[%X]\n", actor[0x2C4], *(u16 *)(actor + 0x124));
    } else {
        btlBossDebugPrintf("return:enemy=%X\n", *(u16 *)(actor + 0x124));
    }
    if (*(u32 *)(actor + 0x110) & 0x400) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
    }
    if (*(u32 *)(actor + 0x110) & 0x200) {
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
        btlStartTask(btlCreateEffObjA(actor, *(u32 *)(task + 0x20)));
    } else if ((*(u32 *)(task + 8) & 0x200) == 0) {
        btlStartTask(btlCreateCommandSoundTask((s32)task, 0x10));
        btlStartTask(btlCreateEffObjB(actor, 0xF));
    }
    if (*(u32 *)(actor + 0x110) & 0x200) {
        btlSyncPlayerWork((BtlUnit *)actor);
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

extern void btlRepositionPartyAroundBattleCenter(void);

extern s32 func_001A3638(void);

extern void fldUpdateSceneGroupTask(s32 task);

extern void btlRemoveTaskFromSceneGroup(BtlTask *task);


void btlCommandTaskReturnUpdate(s32 task) {
    s32 unit = *(s32 *)(task + 0x18);
    u32 flags = *(u32 *)(unit + 0x110);

    *(u32 *)(unit + 0x110) = flags & ~1;
    if (flags & 0x200) {
        btlRepositionPartyAroundBattleCenter();
        func_001A3638();
    }
    if (btlCountTasksByKind(0x3C) != 0) {
        return;
    }
    if (btlCountTasksByKind(0x42) != 0) {
        return;
    }
    if ((*(u32 *)(unit + 0x110) & 0x40) == 0) {
        return;
    }
    if (fldReleaseIdleSceneActorResources((BtlUnit *)*(s32 *)(task + 0x18)) != 0) {
        if (*(u32 *)(unit + 0x110) & 0x200) {
            func_001A1960(&((BtlUnit *)unit)->partyRecord, 8);
            func_001A2258(unit);
        }
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup((BtlTask *)task);
        btlDispatchStateHandler(task, 0x1E);
    }
}

void btlStartLinkedActorEffectTask(s32 task) {
    s32 unit = *(s32 *)(task + 0x18);
    s32 entry;
    if ((*(u32 *)(unit + 0x110) & 0x200) == 0 && func_001A8CE0(unit) == 0) {
        entry = (s32)btlCreateEffObjB(*(s32 *)(task + 0x18), 0x67);
        *(u8 *)(entry + 0) = 0xA;
        *(u16 *)(entry + 8) = 0x40;
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
    s32 owner = arguments[0x34 / 4];
    s32 value = btlCreateEffObjA(owner, arguments[0x20 / 4]);
    btlStartTask(value);
    value = func_001D9468(owner, 1);
    *(s32 *)(value + 0x28) = 7;
    btlStartTask(value);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CF7A0);

extern s32 func_001A8640(BtlUnit *);

void btlRecordLinkedActorOutcome(BtlTask *object) {
    s32 context = btlGetRuntime();
    BtlUnit *target = object->unit;
    *(s32 *)(context + 0x254) += 1;
    if (func_001A8640(target)) {
        *(u32 *)(context + 0x1F4) |= 0x2000;
    } else {
        *(u32 *)(context + 0x1F4) |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CFB10);

void func_001CFD70(void) {
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CFD78);

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
    s32 owner = *(s32 *)(object + 0x18);
    if (btlCountTasksForOwner(*(s64 *)(owner + 0x108)) == 0) {
        *(u32 *)(owner + 0x110) &= ~0x4000;
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
    owner->flags &= ~0x4000;
    fldUpdateSceneGroupTask(task);
    if (command->unit->flags & 0x20) {
        btlUnitTurnEndStateSelect((u8 *)task);
    } else {
        btlDispatchStateHandler(task, 2);
    }
}

extern void btlAccumulateEnemyDefeatRewards(s32);
extern u8 *btlCreateActorModelBlendTask(u8 *, u32, u32, u32, f32);
extern u8 *btlCreateSelectedEffectUpdateTask(u8 *);
extern u8 *func_001D9468(u8 *, u32);
extern SoundTask *sndCreateStationedSeTask(u32);

void btlStartActorDefeatTransition(s32 command) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    BtlTask *commandTask = (BtlTask *)command;
    BtlUnit *actor = commandTask->unit;
    DatPartyRecord *profile = &actor->partyRecord;
    SoundTask *soundTask;
    SoundTask *object;
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
    if (actor->flags & 0x200) {
        if (!(commandTask->flags & 0x100)) {
            if (!(actor->flags & 0x8000000) && actor->unkEC != 11) {
                object = (SoundTask *)btlCreateActorModelBlendTask((u8 *)actor, 0, 11, 2, 1.0f);
                object->startCondition.kind = 4;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
            }
            btlRefreshUnitMotionSelection((u8 *)actor);
        }
    } else if (actor->flags & 0x400) {
        btlAccumulateEnemyDefeatRewards((s32)actor);
        if (actor->flags & 0x8000000) {
            object = (SoundTask *)btlCreateSelectedEffectUpdateTask((u8 *)actor);
            object->startCondition.kind = 4;
            object->startCondition.value.handle = sequence;
            btlStartTask(object);
            object = sndCreateStationedSeTask(0x1000E);
            object->startCondition.kind = 4;
            object->startCondition.value.handle = sequence;
            btlStartTask(object);
            actor->flags &= ~1;
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
                        object = (SoundTask *)btlCreateActorModelBlendTask((u8 *)actor, 0, 11, 2, 1.0f);
                        object->startCondition.kind = 4;
                        object->startCondition.value.handle = sequence;
                        btlStartTask(object);
                    }
                    btlRefreshUnitMotionSelection((u8 *)actor);
                }
            } else {
                object = (SoundTask *)func_001D9468((u8 *)actor, 0);
                object->startCondition.kind = 4;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
                actor->flags &= ~1;
            }
        }
        actor->partyRecord.flags &= ~2;
    }
}


void btlRemoveEligibleActorSceneTask(BtlTask *task) {
    BtlUnit *actor = task->unit;
    s32 entryFlags;

    if (actor->flags & 0x200) {
        if (fldReleaseIdleSceneActorResources(actor) == 0) {
            return;
        }
        btlResetIndexWork(&task->indexWork);
        actor->unk2F4 = -1;
        btlClearAllActorEntrySlots((u32)actor);
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup(task);
        btlDispatchStateHandler(task, 1);
    } else {
        if ((actor->flags & 0x400) == 0) {
            return;
        }
        entryFlags = btlGetEntryFlagsUnlessDisabled(&actor->partyRecord);
        if ((actor->flags & 0x40) == 0 && !(entryFlags & 0x200)) {
            return;
        }
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup(task);
        if ((entryFlags & 0x200) && (actor->flags & 0x40) == 0) {
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
                u8 *fx = (u8 *)*(s32 *)(arg0 + 0x18);
                *(u32 *)(fx + 0x54) = 0x80808080;
                *(u32 *)(fx + 0x84) = 0x80808080;
                *(u32 *)(fx + 0x110) = *(u32 *)(fx + 0x110) & 0x700;
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
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
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

s32 btlFindUnitByActor(s32 target) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x224);
    while (node != 0) {
        if (*(s32 *)(node + 0x18) == target) {
            return node;
        }
        node = *(s32 *)(node + 0x16C);
    }
    return 0;
}

extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);
extern char D_003BB5E0[];
extern char D_003BB5E8[];
extern char D_003A37C8[];

/* Display the eight slot entries, retaining each group's last valid task. */
void btlDebugPrintActionOrder(s32 x, s32 y) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    BtlTask **primary;
    BtlTask **secondary;
    BtlTask **tertiary;
    BtlTask *task;
    s32 color;
    u32 i;

    if ((controller->flags & 4) == 0) {
        return;
    }
    btlBossDebugPrintfN(x, y, 0, (s32)D_003A37C8);
    primary = controller->groupPrimary;
    secondary = controller->groupSecondary;
    tertiary = controller->groupTertiary;
    for (i = 0; i < 8; i++) {
        switch (controller->slots[i].a) {
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
        if (controller->slots[i].b == 100) {
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

void btlDispatchCommandViaHookOrDefault(u8 *command, u8 *argument) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x604);

    if (handler != 0) {
        s32 result = handler((s32)command, (s32)argument);
        if (result != -1) {
            btlDispatchStateHandler(command, result);
            return;
        }
    }
    switch (*(s32 *)argument) {
    case 1:
        btlDispatchStateHandler(command, 0xD);
        break;
    case 4:
        *(s32 *)(argument + 4) = btlGetLoggedIndexedCommandItem(*(s32 *)(argument + 8));
        /* fallthrough */
    case 2:
    case 3:
    case 7:
    case 8:
        if (*(s8 *)(datCommandSelectors + *(s32 *)(argument + 4) * 2 + 1) != 1) {
            btlDispatchStateHandler(command, 0xE);
        } else {
            if (*(u32 *)(*(s32 *)(command + 0x18) + 0x110) & 0x200) {
                scrSetGlobalSeenBit(*(u16 *)(argument + 4));
            }
            btlDispatchStateHandler(command, 0xF);
        }
        break;
    case 5:
        if (*(u32 *)(*(s32 *)(command + 0x18) + 0x110) & 0x1000) {
            btlDispatchStateHandler(command, 0x10);
        } else {
            btlDispatchStateHandler(command, 0x11);
        }
        break;
    case 10:
    case 13:
    case 14:
        btlDispatchStateHandler(command, 0x13);
        break;
    case 9:
        if (*(u32 *)(*(s32 *)(command + 0x34) + 0x110) & 1) {
            btlDispatchStateHandler(command, 0x16);
        } else {
            btlDispatchStateHandler(command, 0x15);
        }
        break;
    case 12:
        btlDispatchStateHandler(command, 0x15);
        break;
    case 6: {
        u32 flags = *(u32 *)(*(s32 *)(command + 0x18) + 0x110);
        if (flags & 0x200) {
            btlDispatchStateHandler(command, 0x17);
        } else if (flags & 0x400) {
            btlDispatchStateHandler(command, 0x14);
        }
        break;
    }
    case 11:
        btlDispatchStateHandler(command, 0x14);
        break;
    case 15:
        btlDispatchStateHandler(command, 0x18);
        break;
    case 16:
        btlDispatchStateHandler(command, 0x19);
        break;
    case 17:
        btlDispatchStateHandler(command, 0x1F);
        break;
    }
}

s32 btlIsSupportedCommandKind(s32 *state) {
    switch (*state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
        return 1;
    default:
        return 0;
    }
}

s32 btlResolveActionOperand(BtlUnit *unit, s32 *argument) {
    switch (argument[0]) {
    case 1:
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200) {
            return btlGetActorBedAssetIdFromIndex(unit->partyRecord.menuValue);
        }
        if (argument[1] > 0) {
            return argument[1];
        }
        return 0;
    case 4:
        return btlGetLoggedIndexedCommandItem(argument[2]);
    case 2:
    case 3:
    case 7:
    case 8:
        return argument[1];
    default:
        return -1;
    }
}

u32 btlClassifyActionOperand(u8 *actor, u8 *argument) {
    switch (*(s32 *)argument) {
    case 1: {
        u32 count = btlGetIndexListCount(*(struct BtlIndexList **)(argument + 0x40));
        if ((*(u64 *)(actor + 0x110) & 0x1200) == 0x1200 &&
            (*(u16 *)(actor + 0x12E) & 0x1000) == 0 &&
            count == 1) {
            u8 *option = *(u8 **)(argument + 0x60);
            if (*(s32 *)(option + 0xC) == 2 && option[0x14] == 0) {
                return 0x17;
            }
        }
        return 3;
    }
    case 4:
        return (*(u32 *)(actor + 0x110) & 0x200) ? 0xC : 4;
    case 2:
    case 3:
    case 7:
    case 8:
        return *(u8 *)(datActionAnimationRecords + *(s32 *)(argument + 4) * 0x20);
    default:
        return 0;
    }
}

s32 btlClassifyActionResult(u8 *arg0, u32 arg1, s32 arg2, u32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 code;

    btlGetEntryFlagsUnlessDisabled(&((BtlUnit *)arg0)->partyRecord);
    if (arg6 >= 0) {
        switch (*(u32 *)(datCommandRecords + arg6 * 56 + 0x30)) {
        case 1:
        case 2:
        case 9:
        case 10:
            return -1;
        }
    }
    if ((*(u32 *)(datCommandRecords + arg6 * 56 + 0x24) & 0x400000FF) == 0x40000002) {
        return -1;
    }
    if (arg1 & 0x50004) {
        return -1;
    }
    if (arg3 & 0xE0001) {
        code = -1;
    } else if ((*(u32 *)(arg0 + 0x110) & 0x200) != 0 && arg2 == 2 && arg4 == 1 && arg5 == 0) {
        code = 0x12;
    } else {
        code = 1;
    }
    if (arg5 != 0 && (*(u64 *)(arg0 + 0x110) & 0x4000000200) == 0x200) {
        code = 0xB;
    }
    if ((arg1 & 0x20001) == 0) {
        code = -1;
    }
    return code;
}

typedef struct BtlOperandSlot {
    u8 pad00[8];
    s32 kind;
} BtlOperandSlot;

/* Test whether the operand is empty, subject to command-category and slot-kind exclusions. */
s32 btlActionEntryIsEmpty(s32 index, BtlOperandSlot *slot, BtlOperandEntry *entry) {
    s32 kind;

    if (index >= 0) {
        switch (((BtlCategoryTableEntry *)datCommandRecords)[index].categoryType) {
        case 1:
        case 2:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
            return 0;
        }
    }
    if (slot != 0) {
        kind = slot->kind;
        if (kind == 2 || kind == 0x10000) {
            return 0;
        }
    }
    if ((entry->flags & 1) != 0) {
        return 0;
    }
    if ((entry->flags & 2) != 0) {
        return 0;
    }
    if (entry->unk00 == 0) {
        if (entry->unk04 == 0) {
            if (entry->unk08 == 0) {
                if (entry->unk0C == 0) {
                    if (entry->unk1C == 0) {
                        if (entry->unk20 == 0) {
                            if (entry->unk10 == 0) {
                                if (entry->unk18 == 0) {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Return whether any operand in the live groups has flag bit zero set. */
s32 func_001D1218(s32 unused, BattleIndexWork *state) {
    u32 groupIndex;
    u32 entryIndex;
    u32 groupCount = btlGetIndexListCount(state->indices);
    BtlOperandGroup *group = state->groups;

    for (groupIndex = 0; groupIndex < groupCount; groupIndex++, group++) {
        u32 entryCount = group->count;

        for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
            if (group->entries[entryIndex].flags & 1) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D12A0);

/* Reset work status and group headers, preserving the retained payload and allocation. */
void btlResetIndexWork(BattleIndexWork *work) {
    u32 i;
    work->phase = -1;
    work->skillId = -1;
    work->reference = -1;
    work->unk0C = 0;
    work->unk10 = 0;
    work->linkedUnit = 0;
    work->unk18 = -1;
    work->stageValue = 0;
    work->unk20 = 8;
    work->stage = 0;
    work->parameter = 0;
    work->wait = -1;
    work->unk3C = 0;
    work->unk2D = 0;
    work->unk2E = 0;
    work->unk50 = 0;
    work->unk54 = 0;
    work->unk58 = 0;
    work->flags = 0;
    work->unk5E = 0;
    for (i = 0; i < 13; i++) {
        work->groups[i].count = 0;
        work->groups[i].kind = 0;
        work->groups[i].unk14 = 0;
        work->groups[i].skipped = 0;
    }
    btlClearIndexList(work->indices);
}

/* Allocate the index list and retained groups, then initialize their headers. */
void btlInitBattleIndexWork(BattleIndexWork *object) {
    u32 handle;
    u32 value;
    object->indices = btlAllocateIndexList(13);
    handle = sdfAllocGeneralBlock(0x836C);
    value = sdfResourceRetainAddress(handle);
    object->allocationHandle = handle;
    object->groups = (BtlOperandGroup *)value;
    object->unk48 = 0;
    btlResetIndexWork(object);
}


/* Release each owned buffer once. The cached group address is deliberately not cleared. */
void btlReleaseObjectBuffers(BattleIndexWork *object) {
    u32 handle = object->allocationHandle;
    if (handle != 0) {
        sdfReleaseResourceAllocation(handle);
        object->allocationHandle = 0;
    }
    if (object->indices != 0) {
        btlFreeIndexList(object->indices);
        object->indices = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D2C78);

extern u32 func_001D2C78(void *);

extern void *btlAllocTask(s32);


void *btlCreateActorParameterDeltaTask(u8 *owner, BattleDeltaSpec *spec) {
    u8 *task = btlAllocTask(0x2C);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x45;
    *(void **)(task + 0x4C) = func_001D2C78;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}

u32 btlApplyDeferredActorStats(u8 *arguments) {
    s32 context = btlGetRuntime();
    u8 *actor = *(u8 **)arguments;
    s32 primary;
    u8 *resource;
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    primary = *(s32 *)(arguments + 0x20);
    if (primary == 0 && *(s32 *)(arguments + 0x24) == 0) {
        return 1;
    }
    if (*(u32 *)(actor + 0x110) & 0x60) {
        return 1;
    }
    resource = actor + 0x120;
    btlAdjustUnitHp(resource, primary);
    btlAdjustUnitMp(resource, *(s32 *)(arguments + 0x24));
    btlRefreshUnitMotionSelection(actor);
    btlIsUnitDefeatTriggeredByValueDelta((BtlUnit *)actor, 0);
    return 1;
}

void *btlCreateDeferredActorStatsTask(u8 *owner, BattleDeltaSpec *spec) {
    u8 *task = btlAllocTask(0x2C);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x46;
    *(void **)(task + 0x4C) = btlApplyDeferredActorStats;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}

u32 btlApplyDeferredUnitStatus(void *argument) {
    u32 *args = (u32 *)argument;
    s32 context = btlGetRuntime();
    u8 *owner = (u8 *)args[0];
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    func_001A1948(&((BtlUnit *)owner)->partyRecord, args[1]);
    btlRefreshUnitMotionSelection(owner);
    btlIsUnitDefeatTriggeredByValueDelta((BtlUnit *)owner, 0);
    return 1;
}

extern void *btlAllocTask(s32);


extern u32 btlApplyDeferredUnitStatus(void *);

void *btlCreateDeferredUnitStatusTask(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(8);
    s64 data;
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x47;
    *(void **)(task + 0x4C) = btlApplyDeferredUnitStatus;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlApplyCategoryStatDamage(BtlStatArgs *args) {
    s32 context = btlGetRuntime();
    BtlUnit *unit = args->unit;
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    if (((BtlCategoryTableEntry *)datCommandRecords)[args->category].flags00 & 8) {
        btlAdjustUnitHp((u8 *)&unit->partyRecord, -0x7FFF);
        func_001A1948(&unit->partyRecord, 0x4000);
        unit->flags |= 0x20;
    }
    if (args->amount == 0) {
        return 1;
    }
    switch (((BtlCategoryTableEntry *)datCommandRecords)[args->category].kind03) {
    case 1:
        btlAdjustUnitHp((u8 *)&unit->partyRecord, -args->amount);
        return 1;
    case 2:
        btlAdjustUnitMp((u8 *)&unit->partyRecord, -args->amount);
        return 1;
    default:
        return 1;
    }
}
extern u32 btlApplyCategoryStatDamage(BtlStatArgs *);

void *btlCreateCategoryStatDamageTask(u8 *owner, u32 value, u32 extra) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlApplyCategoryStatDamage;
    *(u16 *)(task + 0x20) = 0x48;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[2] = value;
    arguments[1] = extra;
    return task;
}

s32 func_001D33D0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    func_001A4C68(*(s32 *)temp_v0, *(s32 *)(temp_v0 + 0x14), *(s16 *)(temp_v0 + 0x18));
    return 1;
}

void *func_001D3400(u8 *owner, BattleDeltaSpec *spec) {
    u8 *task = btlAllocTask(0x2C);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x49;
    *(void **)(task + 0x4C) = func_001D33D0;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}
u32 btlApplyQueuedActorEntrySelection(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        btlSetActorSelectedEntryIndex(*arg0, arg0[7]);
        btlRefreshUnitMotionSelection(*arg0);
    }
    return 1;
}

void *func_001D3510(u8 *owner, BattleDeltaSpec *spec) {
    u8 *task = btlAllocTask(0x2C);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4A;
    *(void **)(task + 0x4C) = btlApplyQueuedActorEntrySelection;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}
u32 btlClearQueuedActorEntrySelection(u32 *arg0) {
    btlClearActorSelectedEntryIndex(*arg0);
    btlRefreshUnitMotionSelection(*arg0);
    return 1;
}

void *func_001D3618(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlClearQueuedActorEntrySelection;
    *(u16 *)(task + 0x20) = 0x4B;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    return task;
}

/* Complete eight-byte argument allocation owned by the hunt-EP task. */
typedef struct BtlHuntExpArgs {
    BtlUnit *actor;
    u32 amount;
} BtlHuntExpArgs;
typedef char BtlHuntExpArgsSizeCheck[sizeof(BtlHuntExpArgs) == 8 ? 1 : -1];
extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);

u32 func_001D3688(s32 address) {
    BtlHuntExpArgs *args = (BtlHuntExpArgs *)address;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *head;
    DatPartyRecord *record;
    u32 count;
    u32 index;
    u32 share;

    if (args->amount == 0) {
        return 1;
    }
    if (args->actor->flags & 0x400) {
        return 1;
    }
    if (btlCheckSpecialAbility(&args->actor->partyRecord, 0x24C)) {
        count = 0;
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2C4);
        record->huntExp += args->amount;
        head = battle->units;
        for (unit = head; unit != NULL; unit = unit->next) {
            u32 flags = unit->flags;
            if (flags & 0x200) {
                if (flags & 1) {
                    if (args->actor != unit && !(flags & 0x20) && !(unit->partyRecord.status & 0x40)) {
                        count++;
                    }
                }
            }
        }
        for (index = 0; index < 5; index++) {
            u16 flags = datGameState->party[index].flags;
            if (flags & 1) {
                if (!(flags & 2) && !(datGameState->party[index].status & 0x4040)) {
                    count++;
                }
            }
        }
        if (count == 0) {
            return 1;
        }
        share = (u32)((f32)args->amount / (f32)count);
        for (unit = head; unit != NULL; unit = unit->next) {
            u32 flags = unit->flags;
            if (flags & 0x200) {
                if (flags & 1) {
                    if (args->actor != unit && !(flags & 0x20) && !(unit->partyRecord.status & 0x40)) {
                        record = btlGetIndexedPartyEntryRecord(unit->unk2C4);
                        record->huntExp += share;
                    }
                }
            }
        }
        for (index = 0; index < 5; index++) {
            u16 flags = datGameState->party[index].flags;
            if (flags & 1) {
                if (!(flags & 2) && !(datGameState->party[index].status & 0x4040)) {
                    datGameState->party[index].huntExp += share;
                }
            }
        }
        btlBossDebugPrintf("btl:AUTO ep=%d[%d],count=%d\n", share, args->amount, count);
    } else {
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2C4);
        record->huntExp += args->amount;
        btlBossDebugPrintf("btl:hunt ep=%d[%p]\n", args->amount, record);
    }
    return 1;
}


extern u32 func_001D3688(s32);

u8 *btlCreateActorSoundOptionTask(BtlUnit *arg0, s32 arg1) {
    u8 *task = btlAllocTask(8);
    BtlHuntExpArgs *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4C;
    *(void **)(task + 0x4C) = func_001D3688;
    *(u64 *)(task + 0x40) = arg0->identity;
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data->actor = arg0;
    data->amount = (u32)arg1;
    return task;
}

u32 func_001D3A20(s32 arg0) {
    if ((*(u8 *)(*(s32 *)(arg0 + 4) * 8 + datItemSkillRecords + 1) & 4) != 0) {
        ptyAdjustItemQuantity(*(s32 *)(arg0 + 4), 0xffffffffffffffff);
    }
    return 1;
}

u8 *btlCreatePermittedBattleVoiceTask(s32 arg0, s32 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4D;
    *(void **)(task + 0x4C) = func_001D3A20;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 btlPlayQueuedBattleVoice(s32 arg0) {
    ptyAdjustItemQuantity(*(u16 *)(arg0 + 4), 1);
    return 1;
}

u8 *btlCreateQueuedBattleVoiceTask(u8 *arg0, u16 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4E;
    *(void **)(task + 0x4C) = btlPlayQueuedBattleVoice;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data[0] = (u32)arg0;
    ((u16 *)data)[2] = arg1;
    return task;
}

u32 btlAddEpFromPacket(s32 arg0) {
    s32 context = btlGetRuntime();

    if (((s32 *)arg0)[1] == 0) {
        return 1;
    }
    if (*(u32 *)(((s32 *)arg0)[0] + 0x110) & 0x400) {
        return 1;
    }
    *(s32 *)(context + 0x2CC) += ((s32 *)arg0)[1];
    btlBossDebugPrintf("btl:epall=%d[%d](packet)\n", *(s32 *)(context + 0x2CC), ((s32 *)arg0)[1]);
    return 1;
}

extern u32 btlAddEpFromPacket(s32);

u8 *btlScheduleEpPacketTask(u8 *owner, s32 value) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4F;
    *(void **)(task + 0x4C) = btlAddEpFromPacket;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlAddMoneyFromPacket(void *arg0) {
    s32 context = btlGetRuntime();

    if (((s32 *)arg0)[1] == 0) {
        return 1;
    }
    if (*(u32 *)(((s32 *)arg0)[0] + 0x110) & 0x400) {
        return 1;
    }
    *(s32 *)(context + 0x2C0) += ((s32 *)arg0)[1];
    btlBossDebugPrintf("btl:money=%d[%d](packet)\n", *(s32 *)(context + 0x2C0), ((s32 *)arg0)[1]);
    return 1;
}

extern u32 btlAddMoneyFromPacket(void *);

void *btlScheduleMoneyPacketTask(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(8);
    s64 data;
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x50;
    *(void **)(task + 0x4C) = btlAddMoneyFromPacket;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlRefreshEligibleActors(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u8 *actor = *(u8 **)(context + 0x228);
    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 &&
                    (u16)(*(u16 *)(actor + 0x124) - 1) < 0x17F) {
                    u32 entry = *(u32 *)(datEnemyRecords + *(u16 *)(actor + 0x124) * 76);
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((*(u32 *)(actor + 0x114) & 8) == 0) {
                                u16 prior = *(u16 *)(actor + 0x12E);
                                func_001A1948(&((BtlUnit *)actor)->partyRecord, 1);
                                btlRefreshUnitMotionSelection(actor);
                                if (*(u16 *)(actor + 0x12E) == 1 &&
                                    prior != *(u16 *)(actor + 0x12E)) {
                                    *(u32 *)(actor + 0x114) |= 4;
                                    *(u32 *)(context + 0x1F8) |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return 1;
}

void *btlCreateRefreshEligibleActorsTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = 1;
    *(void **)(task + 0x4C) = btlRefreshEligibleActors;
    *(u16 *)(task + 0x20) = 0x51;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = 0;
    return task;
}

extern char D_003A3A40[];

extern char D_003A3A50[];

void btlUpdateAutoMusic(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u32 flags = *(u32 *)(context + 0x1F4);
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if (D_00324510.edge22 < 0 || D_00324510.edge23 < 0) {
            *(u32 *)(context + 0x1F4) = flags & ~0x8000;
            sndSetSequenceVolumePan(6, 0x7F, 0x3F);
            btlSetTrackedTaskDisplayMode(0);
            btlBossDebugPrintf(D_003A3A40);
        }
    } else if (D_00324510.edge22 < 0) {
        *(u32 *)(context + 0x1F4) = flags | 0x8000;
        sndSetSequenceVolumePan(5, 0x7F, 0x3F);
        btlSetTrackedTaskDisplayMode(1);
        btlBossDebugPrintf(D_003A3A50);
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D3FE8);

void btlFindSoundTaskByWorkValue(void) {
}

/* Return the oldest matching handle, or zero; unstarted tasks may have handle 0. */
s32 btlFindTaskByHandle(s64 handle) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if ((s64)task->handle == handle) {
            return (s32)task;
        }
        task = task->next;
    }
    return 0;
}

/* Return the oldest task with this owner, or zero. */
s32 btlFindTaskByOwner(s64 owner) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if ((s64)task->owner == owner) {
            return (s32)task;
        }
        task = task->next;
    }
    return 0;
}

/* Return the oldest registered task of this kind, or zero. */
s32 btlFindTaskByKind(u16 kind) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    while (task != 0) {
        if (task->taskId == kind) {
            return (s32)task;
        }
        task = task->next;
    }
    return 0;
}

/* Count all registrations, including tasks awaiting startup or release. */
s32 btlCountRegisteredTasks(void) {
    s32 address;
    s32 count;

    address = btlGetRuntime();
    count = 0;
    for (address = (s32)((BtlActorWork *)address)->taskHead; address != 0; address = (s32)((SoundTask *)address)->next) {
        count = count + 1;
    }
    return count;
}

/* Count registrations with this full-width owner key. */
s32 btlCountTasksForOwner(s64 key) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    s32 count = 0;
    while (task != 0) {
        s64 owner = task->owner;
        task = task->next;
        if (owner == key) {
            count++;
        }
    }
    return count;
}

/* Count registrations of the requested task kind. */
s32 btlCountTasksByKind(u16 kind) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    s32 count = 0;
    while (task != 0) {
        u16 taskKind = task->taskId;
        task = task->next;
        if (taskKind == kind) {
            count++;
        }
    }
    return count;
}

/* Walk newest first and request release for tasks carrying allocation bit 1. */
void btlFlagTasksForUpdate(void) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskTail;
    while (task != 0) {
        u16 flags = task->flags;
        SoundTask *next = task->prev;
        if ((flags & 1) != 0) {
            task->flags = flags | 4;
        }
        task = next;
    }
}

/* Return whether the predicate is satisfied by value or registered tasks.
 * Kinds 5/8 accept running (phase 2) or absent, not an existing finishing task. */
INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3A40);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3A50);

s32 btlEvalTaskCondition(TaskCondition *condition, s32 value) {
    s32 result = 0;
    SoundTask *task;

    switch (condition->kind) {
    case 0:
        break;
    case 1:
        result = 1;
        break;
    case 2:
        if (!(value < condition->value.count)) {
            result = 1;
        }
        break;
    case 3:
        if (btlFindTaskByHandle(condition->value.handle) != 0) {
            result = 1;
        }
        break;
    case 4:
        if (btlFindTaskByHandle(condition->value.handle) == 0) {
            result = 1;
        }
        break;
    case 5:
        task = (SoundTask *)btlFindTaskByHandle(condition->value.handle);
        if (task != 0) {
            if (task->state == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 6:
        if (btlFindTaskByOwner(condition->value.owner) != 0) {
            result = 1;
        }
        break;
    case 7:
        if (btlFindTaskByOwner(condition->value.owner) == 0) {
            result = 1;
        }
        break;
    case 8:
        task = (SoundTask *)btlFindTaskByOwner(condition->value.owner);
        if (task != 0) {
            if (task->state == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 9:
        if (btlFindTaskByKind(condition->value.taskKind) != 0) {
            result = 1;
        }
        break;
    case 10:
        result = btlFindTaskByKind(condition->value.taskKind) == 0;
        break;
    }
    return result;
}

/* Append a cleared task; positive size exposes argument bytes after the header. */
void *btlAllocTask(s32 size) {
    SoundTask *task = sdfAllocAndClearQuadwords(size + 0x70);
    BtlActorWork *context;
    SoundTask *tail;

    if (size > 0) {
        task->args = (u8 *)task + 0x70;
    } else {
        task->args = 0;
    }
    context = (BtlActorWork *)btlGetRuntime();
    task->next = 0;
    tail = context->taskTail;
    if (tail != 0) {
        tail->next = task;
        task->prev = context->taskTail;
    } else {
        context->taskHead = task;
        task->prev = 0;
    }
    context->taskTail = task;
    task->flags |= 1;
    return task;
}

/* Return the argument address recorded by allocation (zero for no arguments). */
void *btlGetTaskArguments(void *task) {
    return ((SoundTask *)task)->args;
}

/* Invoke the finish hook before unlinking, then release the task block. */
void btlFreeTask(s32 taskAddress) {
    BtlActorWork *context;
    SoundTask *next;
    SoundTask *previous;
    void (*cleanup)(u32 *);
    SoundTask *task = (SoundTask *)taskAddress;
    cleanup = task->onFinish;
    if (cleanup != 0) {
        cleanup(task->args);
    }
    context = (BtlActorWork *)btlGetRuntime();
    previous = task->prev;
    if (previous != 0) {
        previous->next = task->next;
    } else {
        context->taskHead = task->next;
    }
    next = task->next;
    if (next != 0) {
        next->prev = task->prev;
    } else {
        context->taskTail = task->prev;
    }
    sdfReleaseChipBlock(taskAddress);
}

/* Install a fresh handle/reset phase counters, invoke startup, then reread handle. */
u64 btlStartTask(void *taskObject) {
    u64 value = btlAdvanceRuntimeSequenceCounter();
    SoundTask *task = taskObject;
    void (*callback)(u32) = task->onStart;
    task->flags |= 8;
    task->handle = value;
    task->pollCount = 0;
    task->runCount = 0;
    task->state = 0;
    task->deferNext = 0;
    task->deferPrev = 0;
    if (callback != 0) {
        callback((u32)task->args);
    }
    return task->handle;
}

void btlResetDeferredTaskQueue(void) {
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Advance a started task through wait/delay/update/release.
 * Fallthrough is intentional: zero delays permit all phases in one poll. */
void btlRunTask(s32 taskAddress) {
    SoundTask *task = (SoundTask *)taskAddress;
    u32 counter;

    if (!(task->flags & 8)) {
        return;
    }
    if (task->flags & 4) {
        btlFreeTask(taskAddress);
        return;
    }
    counter = task->pollCount;
    task->pollCount = counter + 1;
    switch (task->state) {
    case 0:
        if (btlEvalTaskCondition(&task->startCondition, counter) == 0) {
            break;
        }
        task->state = 1;
    case 1:
        if (task->startDelay <= 0) {
            task->state = 2;
        } else {
            task->startDelay = task->startDelay - 1;
            break;
        }
    case 2:
        if (btlEvalTaskCondition(&task->endCondition, task->runCount) != 0) {
            task->state = 3;
        } else if (task->callback.run(task->args) != 0) {
            task->state = 3;
        } else {
            task->runCount = task->runCount + 1;
            break;
        }
    case 3:
        if (task->endDelay <= 0) {
            btlFreeTask(taskAddress);
        } else {
            task->endDelay = task->endDelay - 1;
        }
        break;
    }
}

/* Run ordinary registrations now; queue deferred registrations for the later pass. */
void btlSweepFinishedTasks(void) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskHead;
    SoundTask *next;
    while (task != 0) {
        next = task->next;
        if (!(task->flags & 2)) {
            btlRunTask((s32)task);
        } else {
            task->deferNext = 0;
            if (btlDeferredTaskTail != 0) {
                btlDeferredTaskTail->deferNext = task;
                task->deferPrev = btlDeferredTaskTail;
            } else {
                btlDeferredTaskHead = task;
                task->deferPrev = 0;
            }
            btlDeferredTaskTail = task;
        }
        task = next;
    }
}

/* Run deferred tasks, saving the next link before callbacks may free the task. */
void btlClearDeferredTasks(void) {
    SoundTask *node = btlDeferredTaskHead;
    while (node != 0) {
        SoundTask *next = node->deferNext;
        btlRunTask((s32)node);
        node = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Release newest first; cache the previous registration before its block is freed. */
void btlClearTaskLists(void) {
    SoundTask *task = ((BtlActorWork *)btlGetRuntime())->taskTail;
    while (task != 0) {
        SoundTask *next = task->prev;
        btlFreeTask((s32)task);
        task = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

u32 func_001D4B28(void) {
    return 1;
}

void *btlCreateImmediateCompletionTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = 1;
    *(void **)(task + 0x4C) = func_001D4B28;
    *(u16 *)(task + 0x20) = 0x63;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = 0;
    return task;
}


void btlDumpTaskQueue(void) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x230);
    while (node != 0) {
        btlBossDebugPrintf("btl:packet[%d]\n", *(u16 *)(node + 0x20));
        node = *(s32 *)(node + 0x58);
    }
    btlBossDebugPrintf("btl:packet head[%p]\n", *(void **)(context + 0x22C));
    btlBossDebugPrintf("btl:packet tail[%p]\n", *(void **)(context + 0x230));
}

extern u128 D_00359CC0;

void btlInitUnitFxDefaults(u8 *fx) {
    PCP_COPY_VECTOR(fx + 0x90, &D_00359CC0);
    *(f32 *)(fx + 0xB0) = 220.0f;
    *(f32 *)(fx + 0xB4) = 80.0f;
    *(f32 *)(fx + 0xC0) = 75.0f;
}

extern u128 D_00359CD0;

extern u128 D_00359CE0;

void btlInitFxLights(u8 *fx) {
    PCP_COPY_VECTOR(fx + 0x30, &D_00359CD0);
    PCP_COPY_VECTOR(fx + 0x40, &D_00359CE0);
    *(u32 *)(fx + 0x58) = 0;
    *(f32 *)(fx + 0x50) = 1.0f;
    *(u32 *)(fx + 0x54) = 0x80808080;
    PCP_COPY_VECTOR(fx + 0x60, &D_00359CD0);
    PCP_COPY_VECTOR(fx + 0x70, &D_00359CE0);
    *(f32 *)(fx + 0x80) = 1.0f;
    *(u32 *)(fx + 0x84) = 0x80808080;
    *(u32 *)(fx + 0x88) = 0;
}

void btlInitializeEffectVectorsFromSourceRecords(BtlUnit *fx, s32 kind, s32 index) {
    BtlFxSrcA *alt = (BtlFxSrcA *)btlSelectSharedOrIndexedTransformParameters(kind, index);
    BtlEffectResource *base = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(kind, index);

    if (alt->fC == 0.0f) {
        PCP_COPY_VECTOR(fx->bodyOffset, base);
        fx->reach = base->f18;
        fx->height = base->f1C;
        fx->cameraRadius = base->f20;
    } else {
        fx->bodyOffset[0] = alt->f0;
        fx->bodyOffset[1] = alt->f4;
        fx->bodyOffset[2] = alt->f8;
        fx->bodyOffset[3] = 0.0f;
        fx->reach = alt->f10;
        fx->height = alt->f14;
    }
    PCP_COPY_VECTOR(fx->muzzleOffset, base);
    fx->unkBC = base->f18;
    fx->unkB8 = base->f1C;
    fx->cameraRadius = base->f20;
    fx->scale = base->f10;
    fx->effectScale = base->f10;
    fx->zOffset = base->f14;
    fx->unk58 = base->f14;
}
extern u16 mdlGetContextResourceGroup(MdlCtx *);

extern u16 mdlGetContextResourceId(MdlCtx *);

s32 btlHasMatchingModel(s32 effect, s32 model) {
    s32 context = btlGetRuntime();
    BtlUnit *node = ((BtlState *)context)->units;
    while (node != 0) {
        if ((node->flags & 2) != 0 &&
            node->ext != 0 &&
            node->soundSlotOwner != 0 &&
            mdlGetContextResourceGroup(node->ext->owner) == effect &&
            mdlGetContextResourceId(node->ext->owner) == model) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D4E60);

/* Kind-5 world-node payload attaches the event unit at +8. */
typedef struct EventUnitData {
    u8 pad00[8];
    EvtUnit *unit;
} EventUnitData;

extern s32 mdlSpawnCameraSlotViewerObject(s32, s32);
extern void *dds3GetWorldObject(void);
extern EffWorldNode *dds3FindWorldObjectNodeByKey(EffWorldNode *, u32, s32);
extern void dds3RemoveWorldObjectNode(EffWorldNode *);
extern void dds3ClearObjectFlags(void *, s32);
extern void dds3SetObjectFlags(void *, s32);
extern void mdlStoreTertiaryVectorVU(MdlCtx *);
extern void mdlSetAmountOnAllContextResources(MdlCtx *, f32);
extern struct SoundSlotOwner *sndAcquireSlotOwner(s32, s32);
extern void btlMarkTaskReady(SoundResourceLink *);
extern void btlResetUnitModelProgress(BtlUnit *);
extern void btlSetUnitPosition(u8 *, void *);
extern void btlSetUnitRotation(u8 *, void *);

/* vu0 routine: initialize the actor world transform with the SDK unit vector. */
void func_001D4E98(BtlUnit *unit, u32 kind, u32 id) {
    BtlUnit *reused = NULL;
    BtlState *battle = (BtlState *)btlGetRuntime();
    MdlCtx *model;
    BtlActorStatusRecord *status;
    EventUnitData *data;
    s32 key;

    unit->resourceKind = kind;
    unit->species = id;
    unit->unkCC = 0;
    btlInitializeEffectVectorsFromSourceRecords(unit, kind, id);
    if (battle->findReusableUnit) {
        reused = battle->findReusableUnit(kind, id);
        if (reused) {
            if (reused->resourceKind != kind || reused->species != id) {
                key = mdlSpawnCameraSlotViewerObject(kind, id);
                dds3RemoveWorldObjectNode(dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), key, 5));
            }
            func_001D4E60(unit, reused);
        }
    }
    if (battle->prepareModelUnit) {
        battle->prepareModelUnit(unit);
    }
    if (reused == NULL) {
        key = mdlSpawnCameraSlotViewerObject(kind, id);
        unit->effectObject = dds3FindWorldObjectNodeByKey(dds3GetWorldObject(), key, 5);
        data = unit->effectObject->data;
        unit->ext = data->unit;
        model = unit->ext->owner;
        unit->ext->flags |= 0x200000;
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->effectScale, "vmulx.xyzw vf10, vf10, vf2x");
        unit->effectObject->inner->flags = (unit->effectObject->inner->flags | 1) & ~2;
        VU0_STORE_VF(vf10, unit->effectObject->inner->scale);
        mdlStoreTertiaryVectorVU(model);
        if (unit->effectScale != 1.0f) {
            mdlSetAmountOnAllContextResources(model, unit->effectScale);
        }
        dds3ClearObjectFlags(unit->effectObject, 0x400);
        unit->flags |= 8;
        unit->soundSlotOwner = sndAcquireSlotOwner(kind, id);
        unit->flags |= 2;
    }
    btlSetUnitPosition((u8 *)unit, unit->position);
    btlSetUnitRotation((u8 *)unit, unit->rotation);
    unit->updateFlags = 0;
    unit->effectTimerA = 0;
    unit->unkEC = -1;
    unit->effectTimerB = 0;
    btlRefreshUnitMotionSelection((u8 *)unit);
    if (unit->effectArgA != 0xB) {
        btlApplyScaledUnitEffectParameter((u8 *)unit, unit->effectArgA, 1, 1.0f);
    } else {
        btlApplyScaledUnitEffectParameter((u8 *)unit, 0xB, 2, 1.0f);
    }
    if (unit->resourceLink) {
        btlMarkTaskReady(unit->resourceLink);
    }
    unit->flags |= 0x40000000;
    if (unit->flags & 0x20) {
        model = unit->ext->owner;
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        unit->ext->flags &= ~0xA0;
        mdlAddEntryPlain(model, 0, 0xB);
        unit->unkEC = 0xB;
        sdfMotionSampleAtFrame(model->first, model->first->frameCount);
        unit->flags = unit->flags & 0x7FFFFFFF & 0xBFFFFFFF;
    } else if (btlTestActorStatusPredicate(unit)) {
        unit->ext->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        model = unit->ext->owner;
        unit->ext->flags &= ~0xA0;
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(kind, id);
        mdlAddEntryPlain(model, 0, 1);
        unit->unkEC = 1;
        sdfMotionSampleAtFrame(model->first, status->model);
        btlResetUnitModelProgress(unit);
        unit->flags = (unit->flags | 0x2000) & 0x7FFFFFFF & 0xBFFFFFFF;
    }
    unit->flags |= 0x80004;
    if (battle->finishModelUnit) {
        battle->finishModelUnit(unit);
    }
}

extern const char D_003A3AD0[];

void btlReleaseActorModelResources(u8 *object) {
    s32 sound;
    s32 load;
    EffWorldNode *model;
    u32 state;
    u32 flags;
    if (object[0xCC] == 0) {
        sound = *(s32 *)(object + 0x308);
        if (sound != 0) {
            sndReleaseSlotOwner(sound);
            *(s32 *)(object + 0x308) = 0;
        }
        load = *(s32 *)(object + 0x324);
        if (load != 0) {
            sdfReleaseDevSlot(load, 1, 1);
            *(s32 *)(object + 0x324) = 0;
            btlBossDebugPrintf(D_003A3AD0, object);
        }
        model = *(EffWorldNode **)(object + 0x31C);
        if (model != 0) {
            dds3RemoveWorldObjectNode(model);
            *(s32 *)(object + 0x31C) = 0;
            *(s32 *)(object + 0x320) = 0;
        }
    } else {
        *(s32 *)(object + 0x308) = 0;
        *(s32 *)(object + 0x324) = 0;
        *(s32 *)(object + 0x31C) = 0;
        *(s32 *)(object + 0x320) = 0;
    }
    state = *(u32 *)(object + 0x118) & ~1;
    flags = *(u32 *)(object + 0x110) & ~2;
    state &= ~2;
    *(u32 *)(object + 0x110) = flags;
    *(u32 *)(object + 0x118) = state;
}

void btlRequestModelAssetByMode(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        func_002118D8(arg1, arg2);
        return;
    }
    mdlRequestAsset(arg1, arg2, 0);
}

void btlReleaseModelAssetByMode(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        btlReleaseFoundModelEntry(arg1, arg2);
        return;
    }
}

s32 btlCheckModelAssetByMode(u8 *object, u32 effect, u32 model) {
    if (mdlFlagTest(0xC0F) != 0) {
        if (func_002118D8(effect, model, 0) != 0) {
            return 1;
        }
    } else {
        if (mdlRequestAsset(effect, model, 0) != 0 && mdlRequestAsset(effect, model, 0) != -1) {
            return 1;
        }
    }
    return 0;
}
void btlFlagUnitDefeatCandidate(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->hook654;
    if (hook == 0 || hook(unit) != 0) {
        unit->flags |= 4;
        if (!(unit->flags & 0x8000000)) {
            unit->flags |= 8;
            if (unit->flags & 2) {
                unit->ext->owner->flags &= ~1;
            }
        }
    }
}

void btlClearUnitDefeatCandidate(BtlUnit *object) {
    s32 (*callback)(BtlUnit *);
    u32 flags;
    u32 masked;

    callback = ((BtlState *)btlGetRuntime())->hook658;
    if (callback != 0 && callback(object) == 0) {
        return;
    }
    flags = object->flags;
    masked = flags & ~4;
    masked &= ~8;
    object->flags = masked;
    if ((flags & 2) != 0) {
        MdlCtx *resource = object->ext->owner;
        resource->flags |= 1;
    }
}

u32 btlIsUnitInfoFlagOneEligible(BtlUnit *unit) {
    u32 flags = unit->flags;
    u8 modelFlags;

    if (flags & 0x08000000) {
        return 0;
    }
    if (!(flags & 1)) {
        return 0;
    }
    if (!(flags & 2)) {
        return 0;
    }
    modelFlags = unit->ext->owner->flags;
    return modelFlags & 1;
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3AD0);

void btlApplyUnitMotionSelection(BtlUnit *unit, u32 index, s32 mode, f32 rate) {
    u8 *context;
    BtlEffectResource *table;
    u8 *task;
    MdlCtx *model;
    s32 selected;
    s32 node;
    s32 start;
    s32 end;
    u16 frameCount;
    f32 scale;
    u32 color;
    s32 (*chooseMotion)(BtlUnit *, s32, s32);
    s32 (*keepRange)(BtlUnit *, s32);

    if ((unit->flags & 2) == 0) {
        return;
    }
    if (unit->updateFlags & 1) {
        if (unit->flags & 0x2000) {
            switch (index) {
            case 1:
            case 11:
            case 18:
                task = btlCreateStiffenDamageShakeTask(unit, 8.0f);
                *(s32 *)(task + 0x28) = 1;
                *(u64 *)(task + 0x40) = 0;
                btlStartTask(task);
                break;
            }
        }
        return;
    }
    context = (u8 *)btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, color);
        evtSetUnitAlphaTransition(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind,
                                                                unit->species);
    if (table->nodes[index].rateKind == 2) {
        unit->updateFlags |= 6;
    }
    chooseMotion = *(s32 (**)(BtlUnit *, s32, s32))(context + 0x5A0);
    if (chooseMotion != 0) {
        selected = chooseMotion(unit, index, 0);
        if (selected == -1) {
            return;
        }
        if (index != selected) {
            scale = 1.0f;
            if (table->nodes[index].scale > 0.0f) {
                scale = rate / table->nodes[index].scale;
            }
            index = selected;
            mode = btlGetSlotRateKind(unit, index);
            rate = scale * table->nodes[index].scale;
        }
    }
    if (unit->unkEC == -1) {
        start = 0;
        end = 0;
    } else {
        switch (index) {
        case 11:
            mode = 2;
        case 0: case 2: case 9: case 10:
            start = unit->effectTimerA;
            end = unit->effectTimerB;
            break;
        case 1: case 18:
            start = 0;
            end = 1;
            break;
        case 3: case 4: case 5: case 6: case 7: case 8:
        case 12: case 19: case 20: case 21: case 22: case 23: case 24:
            node = mdlGetNodeField2C(unit->ext->owner, 0);
            switch (node) {
            case 0: case 2: case 9: case 10: case 11:
                start = 0;
                end = 5;
                break;
            default:
                start = 0;
                end = 0;
                break;
            }
            break;
        case 15:
            start = 0;
            end = 0;
            break;
        default:
            start = 0;
            end = 5;
            break;
        }
    }
    keepRange = *(s32 (**)(BtlUnit *, s32))(context + 0x5A8);
    if (keepRange != 0 && keepRange(unit, index) != 0) {
        start = unit->effectTimerA;
        end = unit->effectTimerB;
    }
    if (mode & 0x100) {
        end = 8;
        mode &= ~0x100;
    }
    unit->motionRate = rate;
    unit->unkEC = index;
    unit->effectState = mode;
    rate = rate * (30.0f / *(s8 *)(context + 0x490));
    rate *= *(f32 *)(context + 0x494);
    evtPrepareUnitMotionState(unit->ext, index, start, end, mode);
    model = unit->ext->owner;
    model->first->frameStep = rate;
    if (end == 0) {
        mdlAddEntryFlagged(model, 0, index);
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
    }
    unit->effectTimerA = 0;
    frameCount = table->nodes[index].frameCount;
    unit->effectTimerB = frameCount;
    if (mode != 0 && mode != 3) {
        return;
    }
    evtStoreUnitMotionShortParameters(unit->ext, 0, (s16)frameCount);
    unit->effectTimerA = 0;
    unit->effectTimerB = table->nodes[unit->effectArgA].frameCount;
}

void btlRefreshUnitMotionSelection(u8 *unit) {
    s32 entryFlags;
    u8 *context;
    s32 index;
    s32 selected;
    s32 mode;
    u8 *rates;
    f32 rate;
    f32 speed;
    u32 color;
    s32 (*chooseStatus)(u8 *);
    s32 (*chooseMotion)(u8 *, s32, s32);

    if ((*(u32 *)(unit + 0x110) & 2) == 0) {
        return;
    }
    entryFlags = btlGetEntryFlagsUnlessDisabled(&((BtlUnit *)unit)->partyRecord);
    context = (u8 *)btlGetRuntime();
    if (*(u32 *)(unit + 0xE8) & 2) {
        color = (*(u32 *)(unit + 0x84) & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(*(struct EvtUnit **)(unit + 0x320), 0, color);
        evtSetUnitAlphaTransition(*(struct EvtUnit **)(unit + 0x320), 0, color);
        *(u32 *)(unit + 0x84) = color;
        *(u32 *)(unit + 0xE8) &= ~4;
        *(u32 *)(unit + 0xE8) &= ~2;
    }
    index = 0;
    if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0 &&
        ((*(u32 *)(unit + 0x110) & 0x200) || (entryFlags & 0x200))) {
        index = 10;
    }
    if (*(s32 *)(unit + 0x2F0) > 0 &&
        ((*(u32 *)(unit + 0x110) & 0x200) || (entryFlags & 0x200))) {
        index = 9;
    }
    switch (*(u16 *)(unit + 0x12E) & 0x7FFF) {
    case 1: case 8: case 0x10: case 0x20: case 0x40:
    case 0x80: case 0x100: case 0x200: case 0x400: case 0x2000:
        index = 2;
        break;
    }
    if (btlTestActorStatusPredicate((BtlUnit *)unit) != 0) {
        if ((*(u32 *)(unit + 0x110) & 0x2000) == 0) {
            *(u32 *)(unit + 0x110) |= 0x80002000;
        }
    } else if (*(u32 *)(unit + 0x110) & 0x2000) {
        btlApplyUnitModelScaledValue(unit);
        *(u32 *)(unit + 0x110) &= 0x7FFFFFFF;
        *(u32 *)(unit + 0x110) &= ~0x2000;
    }
    chooseStatus = *(s32 (**)(u8 *))(context + 0x5A4);
    if (chooseStatus != 0) {
        selected = chooseStatus(unit);
        if (selected >= 0) {
            index = selected;
        }
    }
    if (btlIsUnitDefeatTriggeredByValueDelta((BtlUnit *)unit, 0) != 0 &&
        ((*(u32 *)(unit + 0x110) & 0x200) || (entryFlags & 0x200)) &&
        ((*(u32 *)(unit + 0x114) & 0x40) == 0)) {
        index = 11;
        btlApplyUnitModelScaledValue(unit);
        *(u32 *)(unit + 0x110) &= 0x7FFFFFFF;
        *(u32 *)(unit + 0x110) &= ~0x2000;
    }
    rates = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(unit + 0xC4),
                                                  *(s32 *)(unit + 0xC8)) + 0x14;
    rate = *(f32 *)(rates + index * 20 + 0x20);
    chooseMotion = *(s32 (**)(u8 *, s32, s32))(context + 0x5A0);
    if (chooseMotion != 0) {
        selected = chooseMotion(unit, index, 1);
        if (selected == -1) {
            return;
        }
        if (index != selected) {
            index = selected;
            rate = *(f32 *)(rates + index * 20 + 0x20);
        }
    }
    *(f32 *)(unit + 0x104) = rate;
    speed = rate * (30.0f / *(s8 *)(context + 0x490));
    speed *= *(f32 *)(context + 0x494);
    *(s32 *)(unit + 0xFC) = index;
    evtUnitSetStoredParameter(*(struct EvtUnit **)(unit + 0x320), index);
    evtSetTransitionMotionScale(*(struct EvtUnit **)(unit + 0x320), speed);
    mode = 1;
    if (index == 11) {
        mode = 2;
    }
    *(s32 *)(unit + 0x100) = mode;
    if (*(s32 *)(unit + 0xEC) != 11 &&
        (btlIsActorModeAcceptedByBattleHook(unit) != 0 || index == 11) &&
        *(s32 *)(unit + 0xEC) != index) {
        btlApplyUnitMotionSelection(unit, index, mode, rate);
    }
}

s32 btlIsActorModeAcceptedByBattleHook(u8 *object) {
    s32 value;
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    {
        s32 (*callback)(s32) = *(s32 (**)(s32))(btlGetRuntime() + 0x5A4);
        if (callback != 0 && callback(0) == *(s32 *)(object + 0xEC)) {
            return 1;
        }
    }
    value = *(s32 *)(object + 0xEC);
    switch (value) {
    case 0:
    case 2:
    case 9:
    case 10:
    case 11:
        return 1;
    }
    return 0;
}

extern void btlApplyUnitMotionSelection(BtlUnit *, u32, s32, f32);

void btlApplyScaledUnitEffectParameter(u8 *object, s32 index, s32 argument, f32 scale) {
    u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    f32 value = *(f32 *)(resource + index * 20 + 0x34);
    btlApplyUnitMotionSelection(object, index, argument, value * scale);
}

s32 btlGetSlotRateKind(u8 *object, s32 index) {
    u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    s32 value = *(s16 *)(resource + index * 20 + 0x30);

    switch (value) {
    case 0:
        return 0;
    case 1:
    case 2:
    case 3:
        return 2;
    }
    return 0;
}

void btlUpdateUnitEffects(void) {
    s32 context = btlGetRuntime();
    u8 *object = *(u8 **)(context + 0x228);

    while (object != 0) {
        if (*(u32 *)(object + 0x110) & 2) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4),
                                                  *(s32 *)(object + 0xC8));
            MdlCtx *model = ((BtlUnit *)object)->ext->owner;
            s32 node = mdlGetNodeField2C(model, 0);
            if (*(s16 *)(resource + node * 20 + 0x30) == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                btlRefreshUnitMotionSelection(object);
                btlApplyUnitMotionSelection(object, *(s32 *)(object + 0xFC),
                              *(s32 *)(object + 0x100),
                              *(f32 *)(object + 0x104));
            }
        }
        object = *(u8 **)(object + 0x344);
    }
}

void btlApplyUnitModelScaledValue(BtlUnit *object) {
    s32 context;
    f32 volume;
    if ((object->flags & 2) == 0) {
        return;
    }
    context = btlGetRuntime();
    object->updateFlags &= ~1;
    volume = object->motionRate;
    object->ext->owner->first->frameStep =
        volume * (30.0f / (f32)*(s8 *)(context + 0x490));
}

void btlResetUnitModelProgress(BtlUnit *object) {
    if ((object->flags & 2) != 0) {
        object->updateFlags |= 1;
        object->ext->owner->first->frameStep = 0.0f;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D6050);

f32 btlGetUnitModelValue1C(BtlUnit *unit) {
    if ((unit->flags & 2) == 0) {
        return 0.0f;
    }
    return unit->ext->owner->first->currentFrame;
}

/* The caller's frame argument is forwarded unchanged to the sampler. */
void btlAdvanceUnitModelFrame(BtlUnit *unit, f32 frame) {
    if ((unit->flags & 2) != 0) {
        sdfMotionSampleAtFrame(unit->ext->owner->first, frame);
        return;
    }
}

s32 btlGetUnitModelFrameCount(BtlUnit *unit) {
    if ((unit->flags & 2) == 0) {
        return 0;
    }
    return unit->ext->owner->first->frameCount;
}

void btlSeekUnitModelFrameZero(BtlUnit *unit) {

    if ((unit->flags & 2) == 0) {
        return;
    }
    sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
}

void btlSeekRandomModelFrame(BtlUnit *object) {
    s32 duration;
    u32 randomFrame;
    f32 frame;

    if ((object->flags & 2) == 0) {
        return;
    }
    duration = btlGetUnitModelFrameCount(object);
    if (duration > 0) {
        randomFrame = effMiscRandMod(0, duration);
        frame = (f32)randomFrame;
        sdfMotionSampleAtFrame(object->ext->owner->first, frame);
    }
}

u32 btlIsUnitModelStateFive(BtlUnit *object) {
    if ((object->flags & 2) == 0) {
        return 1;
    }
    if (object->effectState != 2) {
        return 1;
    }
    return object->ext->owner->first->state == 5;
}

void btlSetUnitPosition(u8 *object, void *position) {
    f32 world[4] __attribute__((aligned(16)));
    s32 context;
    if ((*(u32 *)(object + 0x114) & 0x80) != 0) {
        return;
    }
    context = btlGetRuntime();
    VU0_LOAD_VF(vf10, position);
    VU0_STORE_VF(vf10, object + 0x60);
    VU0_LOAD_VF(vf11, context);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, world);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        world[2] += *(f32 *)(object + 0x88);
        effObjSetInnerFirstVec(*(s32 *)(object + 0x31C), world);
    }
}

void func_001D6300(u8 *object, void *position) {
    PCP_COPY_VECTOR(position, object + 0x60);
}

void btlGetUnitWorldPos(u8 *object, void *worldPosition) {
    s32 context = btlGetRuntime();
    VU0_LOAD_VF(vf10, object + 0x60);
    VU0_LOAD_VF(vf11, context);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, worldPosition);
}

typedef struct SdfTextParam SdfTextParam;
extern s32 sdfLoadMapRecordPositionVector(SdfTextParam *, s32);
extern void mdlLoadPrimaryVectorVU(MdlCtx *);
extern void mdlLoadSecondaryVectorVU(MdlCtx *);
extern void mdlStorePrimaryVectorVU(MdlCtx *);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);
extern void sdfModelUpdateCurrentFrameTransforms(SdfModel *);

extern void btlRefreshUnitFxVectors(BtlUnit *);

s32 btlSetActorEffectParameter(object, value)
BtlUnit *object;
s32 value;
{
    s32 (*callback)(BtlUnit *, s32);
    if ((object->flags & 2) == 0) {
        return 0;
    }
    callback = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        MdlCtx *owner = object->ext->owner;
        return (s8)sdfLoadMapRecordPositionVector((SdfTextParam *)owner->inner, value);
    }
}

void btlSetActorEffectParameterOrMuzzlePosition(u32 arg0, s32 arg1) {
    s64 temp_v0;

    temp_v0 = btlSetActorEffectParameter();
    if (temp_v0 == 0) {
        btlUnitGetMuzzlePosVU(arg0);
        return;
    }
}

/* vu0 routine: preserve the actor's primary and secondary vectors while
 * evaluating the requested model record; return the sampled vector in vf10. */
s32 func_001D6428(BtlUnit *unit, s32 value) {
    f32 currentVector[4] __attribute__((aligned(16)));
    f32 primaryVector[4] __attribute__((aligned(16)));
    f32 secondaryVector[4] __attribute__((aligned(16)));
    s32 (*callback)(BtlUnit *, s32);
    s8 result;

    if ((unit->flags & 2) == 0) {
        return 0;
    }
    callback = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (callback != NULL) {
        value = callback(unit, value);
    }
    mdlLoadPrimaryVectorVU(unit->ext->owner);
    VU0_STORE_VF_UNCLOBBERED(vf10, primaryVector);
    mdlLoadSecondaryVectorVU(unit->ext->owner);
    VU0_STORE_VF_UNCLOBBERED(vf10, secondaryVector);
    btlRefreshUnitFxVectors(unit);
    result = sdfLoadMapRecordPositionVector((SdfTextParam *)unit->ext->owner->inner, value);
    VU0_STORE_VF_UNCLOBBERED(vf10, currentVector);
    VU0_LOAD_VF(vf10, primaryVector);
    mdlStorePrimaryVectorVU(unit->ext->owner);
    VU0_LOAD_VF(vf10, secondaryVector);
    mdlUpdateContextRotationBasisFromQuaternion(unit->ext->owner);
    sdfModelUpdateCurrentFrameTransforms(unit->ext->owner->inner);
    VU0_LOAD_VF(vf10, currentVector);
    return result;
}

extern s32 sdfLoadMapRecordLookAtBasis(SdfTextParam *, s32);

s32 btlSetActorAlternateEffectParameter(object, value)
BtlUnit *object;
s32 value;
{
    s32 (*callback)(BtlUnit *, s32);
    if ((object->flags & 2) == 0) {
        return 0;
    }
    callback = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        MdlCtx *owner = object->ext->owner;
        return (s8)sdfLoadMapRecordLookAtBasis((SdfTextParam *)owner->inner, value);
    }
}

void btlSetAlternateEffectParameterOrMuzzlePosition(void) {
    if (btlSetActorAlternateEffectParameter() != 0) {
        return;
    }
    VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
}

s32 btlIsUnitAtStoredPosition(u8 *object) {
    f32 position[3];
    func_001D6300(object, position);
    if (*(f32 *)(object + 0x30) == position[0] &&
        *(f32 *)(object + 0x34) == position[1] &&
        *(f32 *)(object + 0x38) == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_003A3B70[];

extern void effMiscQuatMultiplyVU(void);

extern void effObjSetInnerSecondVec(EffWorldNode *, void *);

void btlSetUnitRotation(u8 *object, void *rotation) {
    u8 vector[16];
    if ((*(u32 *)(object + 0x114) & 0x100) != 0) {
        return;
    }
    VU0_LOAD_VF(vf10, rotation);
    if ((*(u32 *)(object + 0x110) & 0x10) != 0) {
        VU0_LOAD_VF(vf11, D_003A3B70);
        effMiscQuatMultiplyVU();
    }
    VU0_STORE_VF_UNCLOBBERED(vf10, object + 0x70);
    VU0_LOAD_VF(vf11, D_003A3B70);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF_UNCLOBBERED(vf10, vector);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        effObjSetInnerSecondVec(*(s32 *)(object + 0x31C), vector);
    }
}

void btlCopyUnitRotationQuaternion(u8 *object, void *position) {
    PCP_COPY_VECTOR(position, object + 0x70);
}

extern void evtSetUnitRgbTransition(struct EvtUnit *, s32, u32);

void btlSetUnitColor(u8 *unit, u32 color, s32 mode) {
    if (*(u32 *)(unit + 0x110) & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        *(u32 *)(unit + 0x54) = (*(u32 *)(unit + 0x54) & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition(*(u32 *)(unit + 0x320), mode, color);
    }
}

void btlBlendUnitColor(u8 *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if (*(u32 *)(unit + 0x110) & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (*(u32 *)(unit + 0x54) & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        *(u32 *)(unit + 0x84) = (*(u32 *)(unit + 0x84) & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition(*(u32 *)(unit + 0x320), mode, blended);
    }
}

extern void mdlReleaseInnerResourceHandle(MdlCtx *, s32, f32);

void btlReleaseUnitModelColorResource(BtlUnit *unit, s32 value, f32 scalar) {
    mdlReleaseInnerResourceHandle(unit->ext->owner, (value & 0xffffff) | 0x80000000, scalar);
}

extern void effObjFetchInnerFirstVec(EffWorldNode *);

extern void effObjFetchInnerSecondVecNorm(EffWorldNode *);


void btlRefreshUnitFxVectors(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return;
    }
    effObjFetchInnerFirstVec(unit->effectObject);
    mdlStorePrimaryVectorVU(unit->ext->owner);
    effObjFetchInnerSecondVecNorm(unit->effectObject);
    mdlUpdateContextRotationBasisFromQuaternion(unit->ext->owner);
    sdfModelUpdateCurrentFrameTransforms(unit->ext->owner->inner);
}

extern void btlUnitGetBodyPosVU(u8 *);

extern s32 btlAimHorizontalDirectionVU(void *, void *);

void btlUnitFaceTarget(u8 *object, u8 *target) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if ((*(u32 *)(object + 0x110) & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        VU0_STORE_VF_UNCLOBBERED(vf10, first);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, second);
        if (btlAimHorizontalDirectionVU(first, second) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, result);
            btlSetUnitRotation(object, result);
        }
    }
}

extern s32 btlAimHorizontalDirectionClampedVU(void *, void *, f32);

void btlUnitFaceTargetScaled(u8 *object, u8 *target, f32 scale) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if ((*(u32 *)(object + 0x110) & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        VU0_STORE_VF_UNCLOBBERED(vf10, first);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, second);
        btlAimHorizontalDirectionClampedVU(first, second, scale);
        VU0_STORE_VF_UNCLOBBERED(vf10, result);
        btlSetUnitRotation(object, result);
    }
}

void btlCopyUnitStats(s32 arg0, s32 arg1) {
    DatPartyRecord *record = &((BtlUnit *)arg0)->partyRecord;
    *record = *(DatPartyRecord *)arg1;
    btlRefreshUnitMaximumHpAndClampCurrentHp(record);
    btlRefreshUnitMaximumMpAndClampCurrentMp(record);
}

extern void mdlSetAllResourceFrames(MdlCtx *, u32);
extern void mdlDispatchViewerAnchorRecord(MdlCtx *, MdlResourceItem *);
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u64 *);
extern void func_002D9748(SdfModel *, SdfModel *);
extern void func_002D9238(SdfPoolNode **, SdfModel *);
extern u64 D_00359CF0[4];
extern void mdlBroadcastMasked(MdlCtx *, u32);
extern void mdlProcessContextNodesAndTransforms(MdlCtx *, s32);

/* Draw the model into four surfaces in three GS TEST passes, then update its anchors. */
void func_001D6A80(BtlUnit *unit, MdlCtx *model, SdfModel *overlay, SdfPoolNode **surfaces, u32 frame) {
    SdfListHead *list;
    u64 *packet;
    MdlResourceItem *item;
    u16 savedFlags;
    s32 i;

    if (model->flags & 1) {
        return;
    }
    mdlBroadcastMasked(model, frame);
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x72801;
        packet[5] = 0x47;
        sdfAppendPacket(list, packet);
        surfaces[i]->append((SdfListHead *)surfaces[i], list);
    }
    savedFlags = model->inner->unk1A;
    model->flags |= 2;
    model->inner->unk1A = 0x2000;
    mdlProcessContextNodesAndTransforms(model, (s32)surfaces);
    model->flags &= ~2;
    model->inner->unk1A = savedFlags;
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x51801;
        packet[5] = 0x47;
        sdfAppendPacket(list, packet);
        surfaces[i]->append((SdfListHead *)surfaces[i], list);
    }
    func_002D9748(overlay, model->inner);
    if (unit->flags & 2) {
        overlay->lighting = unit->ext->endpointWork;
    } else {
        overlay->lighting = NULL;
    }
    func_002D9238(surfaces, overlay);
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = D_00359CF0[i];
        packet[5] = 0x47;
        sdfAppendPacket(list, packet);
        surfaces[i]->append((SdfListHead *)surfaces[i], list);
    }
    mdlSetAllResourceFrames(model, frame);
    for (item = model->resourceItems; item != NULL; item = item->next) {
        mdlDispatchViewerAnchorRecord(model, item);
    }
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3B70);

void btlCreateUnitTransparency(BtlUnit *unit) {
    BattleGroupNode *shape;
    if ((unit->flags & 2) == 0) {
        return;
    }
    if (unit->transparencyModel != 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    shape = unit->ext->owner->sub;
    unit->transparencyModel = (s32)sdfModelCreateWithItems(shape->resourceList, shape->itemList);
    dds3SetObjectFlags(unit->effectObject, 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

extern void sdfReleaseDevSlot(s32, s32, s32);
extern void mdlBroadcastMasked(MdlCtx *, u32);
extern void mdlProcessContextNodesAndTransforms(MdlCtx *, s32);
extern s32 D_00325788[];
extern SdfPoolNode *D_00359D10[];

void btlUpdateUnitTransparency(BtlUnit *unit) {
    u32 flags = unit->flags;
    u32 color;
    MdlCtx *info;
    u32 alpha;

    if (flags & 2) {
        if (unit->unkCC == 0) {
            color = unit->overlayColor;
            info = unit->ext->owner;
            alpha = color >> 24;
            if (!(flags & 0x20000)) {
                if (unit->transparencyModel != 0) {
                    sdfReleaseDevSlot(unit->transparencyModel, 1, 1);
                    unit->transparencyModel = 0;
                    if (unit->flags & 2) {
                        info->inner->lighting = unit->ext->endpointWork;
                    } else {
                        info->inner->lighting = 0;
                    }
                    mdlBroadcastMasked(info, color);
                    mdlProcessContextNodesAndTransforms(info, (s32)D_00325788);
                    dds3ClearObjectFlags(unit->effectObject, 1);
                    btlBossDebugPrintf(D_003A3AD0, unit);
                }
            } else if (alpha == 0) {
                dds3SetObjectFlags(unit->effectObject, 1);
            } else if (unit->transparencyModel == 0) {
                btlCreateUnitTransparency(unit);
            } else {
                func_001D6A80(unit, info, (SdfModel *)unit->transparencyModel, D_00359D10, color);
            }
        }
    }
}

extern SdfGraphObj D_003980E0;
extern SdfPoolNode *D_00359D20[];
extern SdfPoolNode *D_00359D30[];
extern s32 sdfAllocPacketAligned(s32 size);
extern u32 sdfAllocatePacketList(s32);
extern void sdfCreateResourcePacket(u32, SdfTexResource *, s32, s32, s32, s32, s32, s32, s32, s32);
extern void sdfCreateDescriptorPacket(u32, SdfTexResource *, s32, s32, s32, s32, s32, s32);

/* Draw the unit's transparency model into its mirror's packet buffer, then draw the mirror from it. Either
 * model is created (and the update ends) on the first frame it is missing. */
void func_001D6FB0(BtlUnit *unit) {
    MdlCtx *info;
    u32 packet;

    if ((unit->flags & 2) == 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    unit->mirror->unk32C = sdfAllocPacketAligned(0x70000);
    packet = sdfAllocatePacketList(0);
    sdfCreateResourcePacket(packet, D_003980E0.buffers[2], 0, 0, 0x200, 0xE0, unit->mirror->unk32C, 0, 0, 0);
    D_00359D20[0]->append((SdfListHead *)D_00359D20[0], (SdfListHead *)packet);
    info = unit->ext->owner;
    if (unit->transparencyModel == 0) {
        unit->transparencyModel = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->effectObject, 1);
        return;
    }
    func_001D6A80(unit, info, (SdfModel *)unit->transparencyModel, D_00359D20, unit->overlayColor);
    info = unit->mirror->ext->owner;
    if (unit->mirror->transparencyModel == 0) {
        unit->mirror->transparencyModel = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->mirror->effectObject, 1);
        return;
    }
    packet = sdfAllocatePacketList(0);
    sdfCreateDescriptorPacket(packet, D_003980E0.buffers[2], 0, 0, 0x200, 0xE0, unit->mirror->unk32C, 0);
    D_00359D30[0]->append((SdfListHead *)D_00359D30[0], (SdfListHead *)packet);
    func_001D6A80(unit->mirror, info, (SdfModel *)unit->mirror->transparencyModel, D_00359D30, unit->mirror->overlayColor);
}

extern char D_003A3BA8[];

extern char D_003BB5F8[];

s32 btlFormatUnitBedName(u8 *actor, char *filename) {
    u32 flags;
    btlGetRuntime();
    flags = *(u32 *)(actor + 0x110);
    if (!(flags & 0x200)) {
        return 0;
    }
    if (flags & 0x1000) {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8, 0,
                      *(u16 *)(actor + 0x124));
    } else {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8,
                      btlGetActorBedAssetIdFromIndex(((BtlUnit *)actor)->partyRecord.menuValue),
                      *(u16 *)(actor + 0x124));
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3BA8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D7258);

void btlRefreshUnitEffectMotionAndEntry(u8 *unit) {
    u32 flags = *(u32 *)(unit + 0x110);
    if ((flags & 2) == 0) {
        return;
    }
    if (*(s32 *)(unit + 0xEC) != *(s32 *)(unit + 0xFC)) {
        btlRefreshUnitMotionSelection(unit);
        *(u16 *)(unit + 0xF8) = 0;
        *(u16 *)(unit + 0xFA) = 0;
        btlApplyUnitMotionSelection(unit, *(s32 *)(unit + 0xFC),
                      *(s32 *)(unit + 0x100), *(f32 *)(unit + 0x104));
        flags = *(u32 *)(unit + 0x110);
    }
    if ((flags & 0x2000) == 0) {
        s32 index = *(s32 *)(unit + 0xFC);
        if (index != 11) {
            mdlAddEntryFlagged(((BtlUnit *)unit)->ext->owner, 0, index);
        } else {
            mdlAddEntryPlain(((BtlUnit *)unit)->ext->owner, 0, 11);
        }
        sdfMotionSampleAtFrame(((BtlUnit *)unit)->ext->owner->first, 0.0f);
    }
}

u32 btlApplyIndexedUnitEffectTask(u8 *arguments) {
    s32 index = *(s32 *)(arguments + 4);
    if (index >= 0) {
        btlApplyScaledUnitEffectParameter(*(u8 **)arguments, index, *(s32 *)(arguments + 8),
                        *(f32 *)(arguments + 0xC));
    }
    return 1;
}

u8 *btlAllocateIndexedUnitEffectTask(u8 *owner, s32 index, s32 value, f32 scale) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 9;
    *(void **)(task + 0x4C) = btlApplyIndexedUnitEffectTask;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(f32 *)(arguments + 3) = scale;
    return task;
}

u32 btlApplyScaledUnitModelTask(u32 *arg0) {
    btlApplyUnitModelScaledValue(*arg0);
    return 1;
}

void *btlCreateScaledUnitModelTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlApplyScaledUnitModelTask;
    *(u16 *)(task + 0x20) = 10;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments(task) = (u32)owner;
    return task;
}

u32 btlPollThresholdTask(s32 *arguments) {
    if ((s32)btlGetUnitModelValue1C((BtlUnit *)arguments[0]) >= arguments[1]) {
        if ((*(u32 *)(arguments[0] + 0xE8) & 1) == 0) {
            btlResetUnitModelProgress((BtlUnit *)arguments[0]);
        }
        return 1;
    }
    return 0;
}

void *btlScheduleThresholdTask(u8 *owner, u32 threshold) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0xB;
    *(void **)(task + 0x4C) = btlPollThresholdTask;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = threshold;
    return task;
}

extern void effMiscQuaternionToMatrixVU(void);

s32 btlApproachTargetTask(BtlApproachTaskArgs *args) {
    BtlUnit *unit = args->unit;
    BtlUnit *target = args->target;
    f32 scale;
    f32 reach;
    f32 dist;
    f32 pos[4];
    s128 fromPos;
    s128 toPos;
    scale = args->scale == 0.0f ? 1.0f : args->scale;
    if (args->count == 0) {
        BtlEffectResource *table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->species);
        args->offset = table->nodes[unit->unkEC].reachOffset * unit->scale;
    }
    reach = args->offset + target->reach * target->scale;
    btlUnitGetMuzzlePosVU(unit);
    VU0_STORE_VF_UNCLOBBERED(vf10, &fromPos);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, &toPos);
    ((f32 *)&toPos)[1] = ((f32 *)&fromPos)[1];
    VU0_LOAD_VF(vf10, &fromPos);
    VU0_LOAD_VF(vf11, &toPos);
    VU0_MOVE_VF(vf12, vf10);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_SCALAR_OP(reach, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf12);
    VU0_LERP_VF10(0.8f / scale);
    VU0_STORE_VF(vf10, pos);
    VU0_LOAD_VF(vf11, &fromPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(dist);
    VU0_LOAD_VF(vf10, &unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, &unit->bodyOffset);
    VU0_SCALAR_OP(unit->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_NEGATE_XYZ(vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, pos);
    pos[2] -= unit->zOffset;
    btlSetUnitPosition((u8 *)args->unit, pos);
    btlUnitFaceTarget((u8 *)unit, (u8 *)target);
    if (dist < 1.0f) {
        return 1;
    }
    args->count++;
    return 0;
}
extern s32 btlApproachTargetTask(BtlApproachTaskArgs *);

u8 *btlAllocateApproachTargetTask(u8 *owner, s32 index, f32 scale) {
    u8 *task = btlAllocTask(24);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlApproachTargetTask;
    *(u16 *)(task + 0x20) = 0xE;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    *(f32 *)(arguments + 3) = scale;
    arguments[2] = 0;
    arguments[5] = 0;
    return task;
}

struct BtlPosLerpTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
};

s32 btlUpdateUnitPositionInterpolationTask(BtlPosLerpTaskArgs *args) {
    s128 pos;
    f32 t = args->t;
    f32 rate;
    BtlUnit *unit = args->unit;

    if (t < 1.0f && args->rate > 0.0f && args->rate < 1.0f) {
        rate = args->rate;
        if (args->count == 0) {
            PCP_COPY_VECTOR(&args->from, unit->currentPosition);
        }
        args->t = t + (1.0f - t) * rate;
        if (args->t > 0.999f) {
            args->t = 1.0f;
        }
        VU0_LOAD_VF(vf10, &args->from);
        VU0_LOAD_VF(vf11, &args->to);
        VU0_LERP_VF10(args->t);
        VU0_STORE_VF(vf10, &pos);
        btlSetUnitPosition((u8 *)unit, &pos);
    } else {
        btlSetUnitPosition((u8 *)unit, &args->to);
        return 1;
    }
    args->count++;
    return 0;
}
SoundTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, f32 *target, f32 scale) {
    SoundTask *task = (SoundTask *)btlAllocTask(0x30);
    u8 *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0xC;
    task->owner = unit->identity;
    *(void **)((u8 *)task + 0x4C) = btlUpdateUnitPositionInterpolationTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *(f32 *)(args + 0x20) = scale;
    *(u32 *)(args + 0x2C) = (u32)unit;
    *(u32 *)(args + 0x24) = 0;
    *(u32 *)(args + 0x28) = 0;
    PCP_COPY_VECTOR(args, unit->currentPosition);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

extern void effMiscQuaternionNlerpVU(f32);

s32 btlStepUnitRotationNlerp(BtlRotationTaskArgs *args) {
    s128 quat;
    f32 t;
    f32 rate;
    BtlUnit *unit = args->unit;
    if (!(unit->flags & 0x80000)) {
        return 1;
    }
    t = args->t;
    if (t < 1.0f) {
        rate = args->rate;
        if (rate > 0.0f && rate < 1.0f) {
            if (args->count == 0) {
                PCP_COPY_VECTOR(&args->from, unit->orientation);
            }
            args->t = t + (1.0f - t) * rate;
            if (args->t > 0.999f) {
                args->t = 1.0f;
            }
            VU0_LOAD_VF(vf10, &args->from);
            VU0_LOAD_VF(vf11, &args->to);
            effMiscQuaternionNlerpVU(args->t);
            VU0_STORE_VF(vf10, &quat);
            btlSetUnitRotation((u8 *)unit, &quat);
            return 0;
        }
    }
    btlSetUnitRotation((u8 *)unit, &args->to);
    return 1;
}
SoundTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, f32 *target, f32 scale) {
    SoundTask *task = (SoundTask *)btlAllocTask(0x30);
    u8 *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0xD;
    task->owner = unit->identity;
    *(void **)((u8 *)task + 0x4C) = btlStepUnitRotationNlerp;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *(f32 *)(args + 0x20) = scale;
    *(u32 *)(args + 0x2C) = (u32)unit;
    *(u32 *)(args + 0x24) = 0;
    *(u32 *)(args + 0x28) = 0;
    PCP_COPY_VECTOR(args, unit->orientation);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

extern char D_003A3BC8[];

extern char D_003A3BE8[];


void btlRequestModelOrReuse(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        func_001D4E98((BtlUnit *)object, effect, model);
        if (*(char *)(arguments + 3) == 0) {
            btlClearUnitDefeatCandidate(object);
            evtSetUnitAlphaTransition(*(u32 *)(object + 0x320), 0, 0);
            *(u32 *)(object + 0x84) = *(u32 *)(object + 0x54) & 0xFFFFFF;
        }
        btlBossDebugPrintf(D_003A3BC8, effect, model);
    } else {
        btlRequestModelAssetByMode(object, effect, model);
        *(u32 *)(object + 0x118) |= 1;
        btlBossDebugPrintf(D_003A3BE8, effect, model);
    }
}

extern char D_003A3C08[];

extern s32 btlCheckModelAssetByMode(u8 *, u32, u32);

u32 btlPollModelLoadCompletion(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        if (!btlCheckModelAssetByMode(object, effect, model)) {
            return 0;
        }
        func_001D4E98((BtlUnit *)object, effect, model);
        btlReleaseModelAssetByMode(object, effect, model);
        btlBossDebugPrintf(D_003A3C08, effect, model, object);
    }
    if (*(s8 *)(arguments + 3) == 0) {
        btlClearUnitDefeatCandidate(object);
        evtSetUnitAlphaTransition(*(u32 *)(object + 0x320), 0, 0);
        *(u32 *)(object + 0x84) = *(u32 *)(object + 0x54) & 0xFFFFFF;
    }
    *(u32 *)(object + 0x118) = (*(u32 *)(object + 0x118) & ~1) | 2;
    return 1;
}

u8 *btlCreateModelLoadPollTask(u8 *owner, u32 index, u32 value, s8 mode) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x18;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlRequestModelOrReuse;
    *(void **)(task + 0x4C) = btlPollModelLoadCompletion;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(s8 *)(arguments + 3) = mode;
    return task;
}

u32 btlReleaseUnitModelTask(u32 *arg0) {
    btlClearUnitDefeatCandidate(*arg0);
    btlReleaseActorModelResources(*arg0);
    return 1;
}

void *btlScheduleRefreshTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlReleaseUnitModelTask;
    *(u16 *)(task + 0x20) = 0x19;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    return task;
}

/* Task-start callbacks do not return a status to the scheduler. */
INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3BC8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3BE8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3C08);

void btlBeginModelChange(u32 argumentsAddress) {
    BtlModelChangeArgs *arguments = (BtlModelChangeArgs *)argumentsAddress;
    BtlUnit *owner = arguments->unit;
    u32 model = arguments->resourceKind;
    u32 variant = arguments->resourceId;
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        btlRequestModelAssetByMode((u32)owner, model, variant);
        owner->gunResourceFlags = (owner->gunResourceFlags | 1) & ~2;
        btlBossDebugPrintf("btl:model change start[%X,%X]\n", model, variant);
    }
}

/* Complete model loading, cross-fade the retained actor, and release it. */
u32 func_001D8190(BtlModelChangeArgs *args) {
    BtlUnit *unit = args->unit;
    u32 resourceKind = args->resourceKind;
    u32 resourceId = args->resourceId;
    u32 packedStart[4];
    u32 packedEnd[4];
    u32 alpha;
    u32 color;
    s32 entryFlags;

    switch (args->phase) {
    case 0:
        if (args->delay > args->elapsed) {
            break;
        }
        if (!btlCheckModelAssetByMode((u8 *)unit, resourceKind, resourceId)) {
            break;
        }
        if (args->duration != 0) {
            unit->mirror = btlCreateUnit();
            unit->mirror->flags |= 0x40000;
            func_001D4E60(unit->mirror, unit);
            if (unit->unkCC == 0) {
                unit->mirror->unkCC = 0;
                unit->unkCC = 1;
            }
            btlSetUnitPosition((u8 *)unit->mirror, unit->currentPosition);
            btlSetUnitRotation((u8 *)unit->mirror, unit->orientation);
            btlSetUnitColor((u8 *)unit->mirror, unit->baseColor, 0);
            unit->mirror->flags |= 8;
            args->phase = 1;
            args->elapsed = 0;
        } else {
            args->phase = 2;
        }
        btlReleaseActorModelResources((u8 *)unit);
        btlRefreshUnitMaximumHpAndClampCurrentHp(&unit->partyRecord);
        btlRefreshUnitMaximumMpAndClampCurrentMp(&unit->partyRecord);
        func_001D4E98(unit, resourceKind, resourceId);
        btlReleaseModelAssetByMode((u32)unit, resourceKind, resourceId);
        if (args->duration == 0) {
            kwlnDrawControlFlags |= 0x2000000;
        }
        btlSetUnitPosition((u8 *)unit, unit->currentPosition);
        btlSetUnitRotation((u8 *)unit, unit->orientation);
        btlSetUnitColor((u8 *)unit, unit->baseColor, 0);
        if (unit->stateFlags & 0x10) {
            u32 firstColor;
            u32 secondColor;

            VU0_LOAD_VF(vf10, unit->colorStart);
            EE_MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
            firstColor = packedStart[0];
            VU0_LOAD_VF(vf10, unit->colorEnd);
            EE_MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
            secondColor = packedEnd[0];
            evtInitializeUnitColorTransition(unit->ext, 0, firstColor, secondColor);
        }
        unit->gunResourceFlags = (unit->gunResourceFlags & ~1) | 2;
        if (args->phase == 1) {
            u32 baseRgb = unit->baseColor & 0xFFFFFF;

            unit->overlayColor = baseRgb;
            unit->mirror->overlayColor = baseRgb | 0x80000000;
            evtSetUnitAlphaTransition(unit->ext, 0, 0);
        }
        break;
    case 1:
        if (args->elapsed == 1 && args->duration != 0) {
            entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
            if (unit->flags & 0x20) {
                btlApplyScaledUnitEffectParameter((u8 *)unit, 0xB,
                    btlGetSlotRateKind((u8 *)unit, 0xB), 1.0f);
            } else if (args->transitionMode == 2) {
                unit->unkEC = -1;
                btlApplyScaledUnitEffectParameter((u8 *)unit, 0xE,
                    btlGetSlotRateKind((u8 *)unit, 0xE), 1.0f);
            } else if (args->transitionMode != 3 &&
                       ((unit->flags & 0x200) || (entryFlags & 0x200)) &&
                       args->resourceId != 0x1F &&
                       !(unit->partyRecord.status & 0x2000)) {
                unit->unkEC = -1;
                if (unit->flags & 0x1000) {
                    btlApplyScaledUnitEffectParameter((u8 *)unit, 0x10,
                        btlGetSlotRateKind((u8 *)unit, 0x10), 1.0f);
                } else {
                    btlApplyScaledUnitEffectParameter((u8 *)unit, 0x11,
                        btlGetSlotRateKind((u8 *)unit, 0x11), 1.0f);
                }
            }
        }
        if ((u32)args->elapsed < args->duration) {
            if (args->transitionMode == 0) {
                alpha = (u32)((f32)args->elapsed / (f32)args->duration * 128.0f);
                color = unit->baseColor & 0xFFFFFF;
                unit->overlayColor = (alpha << 24) | color;
                unit->mirror->overlayColor = ((128 - alpha) << 24) | color;
                unit->flags |= 0x10000;
            } else {
                if (args->elapsed == 1) {
                    btlFlagUnitDefeatCandidate(unit->mirror);
                    evtSetUnitRgbTransition(unit->mirror->ext, args->duration >> 2, 0x80000000);
                } else if ((u32)args->elapsed == (args->duration >> 2)) {
                    evtSetUnitAlphaTransition(unit->mirror->ext, args->elapsed, 0);
                    unit->mirror->flags |= 0x200000;
                    btlFlagUnitDefeatCandidate(unit);
                    alpha = unit->baseColor & 0xFF000000;
                    evtSetUnitRgbTransition(unit->ext, 0, 0);
                    evtSetUnitAlphaTransition(unit->ext, args->duration >> 2, alpha);
                    unit->flags |= 0x100000;
                } else if ((u32)args->elapsed == (args->duration >> 1)) {
                    unit->overlayColor = unit->baseColor;
                    evtSetUnitRgbTransition(unit->ext, args->duration >> 1, unit->baseColor);
                }
            }
        } else {
            if (args->transitionMode == 0) {
                unit->flags &= ~0x10000;
            }
            unit->overlayColor = unit->baseColor;
            unit->mirror->overlayColor = 0;
            args->phase = 2;
        }
        break;
    case 2:
        if (unit->mirror != NULL) {
            btlDestroyUnit((u8 *)unit->mirror);
            unit->mirror = NULL;
            if (unit->unk330 != 0) {
                sdfQueueNonzeroResourceId(unit->unk330);
                unit->unk330 = 0;
                unit->unk32C = 0;
            }
        }
        btlBossDebugPrintf("btl:model change end[%X,%X]\n", resourceKind, resourceId);
        return 1;
    }
    args->elapsed++;
    return 0;
}


extern u32 func_001D8190(BtlModelChangeArgs *);

u8 *btlCreateModelChangeTask(u8 *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    u8 *task = btlAllocTask(0x1C);
    BtlModelChangeArgs *args;
    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x1A;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlBeginModelChange;
    *(void **)(task + 0x4C) = func_001D8190;
    args = btlGetTaskArguments(task);
    args->unit = (BtlUnit *)unit;
    args->resourceKind = arg1;
    args->resourceId = arg2;
    args->delay = arg3;
    args->duration = arg4;
    args->transitionMode = arg5;
    args->phase = 0;
    args->elapsed = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        evtSetUnitStatusFlags(((BtlUnit *)*(u32 *)(arg0 + 0xc))->ext);
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(u32 *arg0) {
    if ((*(u64 *)(arg0[3] + 0x110) & 0x1000000002) == 0x1000000002) {
        evtInitializeUnitColorTransition(((BtlUnit *)arg0[3])->ext, arg0[2], *arg0, arg0[1]);
    }
    return 1;
}

u8 *btlCreateUnitTask0F(u8 *unit, s32 arg1, s32 arg2, s32 arg3) {
    u8 *task = btlAllocTask(16);
    u32 *args;
    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0xF;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlApplyLinkedUnitStatusWhenActorActive;
    *(void **)(task + 0x4C) = btlApplyUnitFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args[3] = (u32)unit;
    args[0] = arg1;
    args[1] = arg2;
    args[2] = arg3;
    return task;
}

void btlPrepareUnitStatusFxOnStart(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x110) & 2) != 0) {
        evtSetUnitStatusFlags(((BtlUnit *)*(u32 *)(arg0 + 0x14))->ext);
        return;
    }
}

u32 btlApplyUnitVectorFxWhenLoaded(u8 *arguments) {
    u8 *object = *(u8 **)(arguments + 0x14);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        VU0_LOAD_VF(vf10, arguments);
        evtSetUnitNormalizedDirection(((BtlUnit *)object)->ext, *(s32 *)(arguments + 0x10));
    }
    return 1;
}

u8 *btlCreateUnitTask10(u8 *unit, f32 *spawnPosition, s32 value) {
    u8 *task = btlAllocTask(0x18);
    u32 *args;
    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x10;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlPrepareUnitStatusFxOnStart;
    *(void **)(task + 0x4C) = btlApplyUnitVectorFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args[5] = (u32)unit;
    args[4] = value;
    PCP_COPY_VECTOR(args, spawnPosition);
    return task;
}

extern void evtSetUnitAlphaTransition(struct EvtUnit *, s32, u32);

u32 btlUnitFadeInTask(BtlFadeArgs *args) {
    u32 total = args->fadeIn + args->fadeOut;
    BtlUnit *unit = args->unit;
    if (total != 0) {
        if (args->count == 0) {
            args->color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
            btlFlagUnitDefeatCandidate(unit);
            mdlBroadcastMasked(unit->ext->owner, 0);
            evtSetUnitRgbTransition(unit->ext, 0, 0);
            evtSetUnitAlphaTransition(unit->ext, 0, 0);
            evtSetUnitAlphaTransition(unit->ext, args->fadeIn, args->color);
            unit->flags |= 0x100000;
        }
        if (args->count == args->fadeIn - 1) {
            unit->overlayColor = args->color;
            evtSetUnitRgbTransition(unit->ext, args->fadeOut, args->color);
        }
    } else {
        unit->overlayColor = args->color;
    }
    if (!(unit->flags & 0x100000)) {
        if (args->count >= total) {
            evtSetUnitRgbTransition(unit->ext, 0, args->color);
            evtSetUnitAlphaTransition(unit->ext, 0, args->color);
            return 1;
        }
    }
    args->count++;
    return 0;
}
extern u32 btlUnitFadeInTask(BtlFadeArgs *);

u8 *btlCreateUnitFadeInTask(u8 *owner, u32 value, u32 variant) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlUnitFadeInTask;
    *(u16 *)(task + 0x20) = 0x11;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[4] = 0x80808080;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

u32 btlUnitFadeOutTask(BtlFadeArgs *args) {
    u32 total = args->fadeIn + args->fadeOut;
    BtlUnit *unit = args->unit;
    if (total != 0) {
        if (args->count == 0) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitRgbTransition(unit->ext, args->fadeOut, 0x80000000);
            unit->flags |= 0x200000;
        }
        if (args->count == args->fadeOut - 1) {
            unit->overlayColor = 0x80000000;
            evtSetUnitRgbTransition(unit->ext, 0, 0);
            evtSetUnitAlphaTransition(unit->ext, args->fadeIn, 0);
        }
    } else {
        unit->overlayColor = 0x80000000;
    }
    if (!(unit->flags & 0x200000)) {
        if (args->count >= total) {
            return 1;
        }
    }
    args->count++;
    return 0;
}
extern u32 btlUnitFadeOutTask(BtlFadeArgs *);

u8 *btlCreateUnitFadeOutTask(u8 *owner, u32 value, u32 variant) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlUnitFadeOutTask;
    *(u16 *)(task + 0x20) = 0x12;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

u32 btlStepUnitDefeatFadeIn(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    u32 alpha;

    if ((s32)arguments[1] == -1) {
        unit->overlayColor = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, unit->overlayColor);
        evtSetUnitAlphaTransition(unit->ext, 0, unit->overlayColor);
        btlClearUnitDefeatCandidate(unit);
        return 1;
    }
    if (arguments[2] == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if (arguments[1] == 0) {
        evtSetUnitRgbTransition((struct EvtUnit *)unit->ext, 0, unit->overlayColor);
        evtSetUnitAlphaTransition((struct EvtUnit *)unit->ext, 0,
                                  (unit->overlayColor & 0xFFFFFF) | 0x80000000);
    }
    if ((s32)arguments[2] >= (s32)arguments[1]) {
        unit->flags &= ~0x20000;
        unit->overlayColor = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        return 1;
    }
    unit->flags |= 0x20000;
    alpha = (u32)((f32)(s32)arguments[2] * 128.0f / (f32)(s32)arguments[1]);
    alpha <<= 24;
    unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
    arguments[2]++;
    return 0;
}

u8 *func_001D9038(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x13;
    *(void **)(task + 0x4C) = btlStepUnitDefeatFadeIn;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

u32 btlStepUnitDefeatFadeOut(u32 *arguments) {
    u8 *unit = (u8 *)arguments[0];
    u32 alpha;

    if (arguments[2] == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if ((s32)arguments[2] >= (s32)arguments[1]) {
        *(u32 *)(unit + 0x110) &= ~0x20000;
        *(u32 *)(unit + 0x84) = *(u32 *)(unit + 0x54) & 0xFFFFFF;
        return 1;
    }
    *(u32 *)(unit + 0x110) |= 0x20000;
    alpha = (u32)((1.0f - (f32)(s32)arguments[2] / (f32)(s32)arguments[1]) * 128.0f);
    alpha <<= 24;
    *(u32 *)(unit + 0x84) = alpha | (*(u32 *)(unit + 0x54) & 0xFFFFFF);
    arguments[2]++;
    return 0;
}


u8 *func_001D91E0(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x14;
    *(void **)(task + 0x4C) = btlStepUnitDefeatFadeOut;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

u32 func_001D9268(u32 *arguments) {
    u8 *unit = (u8 *)arguments[0];
    s32 finished = 0;
    u32 alpha;

    if (arguments[2] == 0) {
        *(u32 *)(unit + 0x110) |= 0x80;
        btlFlagUnitDefeatCandidate(unit);
    }

    switch (arguments[1]) {
    case 0:
        if (arguments[2] == 1) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitRgbTransition(*(struct EvtUnit **)(unit + 0x320),
                                    10, 0x80000000);
        } else if (arguments[2] == 10) {
            evtSetUnitRgbTransition(*(struct EvtUnit **)(unit + 0x320),
                                    0, 0x80000000);
            evtSetUnitAlphaTransition(*(struct EvtUnit **)(unit + 0x320), 6, 0);
            *(u32 *)(unit + 0x110) |= 0x200000;
        }
        if ((*(u32 *)(unit + 0x110) & 0x200000) == 0 &&
            (s32)arguments[2] >= 16) {
            finished = 1;
        }
        break;

    case 1:
        if ((s32)arguments[2] >= 8) {
            *(u32 *)(unit + 0x110) &= ~0x20000;
            *(u32 *)(unit + 0x84) = *(u32 *)(unit + 0x54) & 0xFFFFFF;
            finished = 1;
        } else {
            *(u32 *)(unit + 0x110) |= 0x20000;
            alpha = (u32)((1.0f - (f32)(s32)arguments[2] * 0.125f) * 128.0f);
            alpha <<= 24;
            *(u32 *)(unit + 0x84) = alpha | (*(u32 *)(unit + 0x54) & 0xFFFFFF);
        }
        break;
    }

    arguments[2]++;
    if (finished != 0) {
        *(u32 *)(unit + 0x110) = (*(u32 *)(unit + 0x110) & ~0x80) | 0x40;
        return 1;
    }
    return 0;
}


u8 *func_001D9468(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x15;
    *(void **)(task + 0x4C) = func_001D9268;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}


typedef struct UnitEffectTaskArgs {
    BtlUnit *unit;
    SoundMixer *mixer;
    SoundVoice *effect;
    s32 duration;
    s32 counter;
} UnitEffectTaskArgs;

extern void *func_00160958(SoundMixer *, u16, void *, s32);

extern void effBattleUpdateSelectedValue(u8 *, s32);

extern void func_00160D88(u8 *);

/* Start from the selected-unit SYSEFF source, then update through its duration.
 * Return one for an ineligible unit or expiry, zero while updating. */
u32 btlUpdateSelectedUnitEffect(UnitEffectTaskArgs *arguments) {
    BtlUnit *unit = arguments->unit;
    SoundResourceNode *work;
    void *handle;

    if (!(unit->flags & 2)) {
        return 1;
    }
    {
        BtlState *battle = (BtlState *)btlGetRuntime();
        if (arguments->effect == 0) {
            work = battle->resources[BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT];
            handle = work->sourceHandle;
            unit->flags |= 0x80;
            arguments->mixer = sndMixerClone(handle);
            arguments->effect = func_00160958(arguments->mixer, 2, unit, 0);
            arguments->duration = 0xE;
            arguments->effect->flags &= 0xFFF9;
            effBattleUpdateSelectedValue(arguments->effect, 0xE);
            unit->flags &= ~8;
            if (unit->flags & 2) {
                unit->ext->owner->flags |= 1;
            }
        }
        arguments->counter = arguments->counter + 1;
        if (arguments->counter >= arguments->duration) {
            unit->flags = (unit->flags & ~0x80) | 0x40;
            return 1;
        }
        func_00160D88(arguments->effect);
        return 0;
    }
}

extern u32 btlUpdateSelectedUnitEffect(UnitEffectTaskArgs *);

void btlFinishSelectedUnitEffect(UnitEffectTaskArgs *arguments) {
    SoundVoice *voice = arguments->effect;
    if (voice != 0) {
        effReleaseBattleVoiceOwner(voice);
    }
    if (arguments->mixer != 0) {
        sndReleaseAllVoices(arguments->mixer);
    }
    btlClearUnitDefeatCandidate(arguments->unit);
    arguments->unit->flags |= 0x40;
}

u8 *btlCreateSelectedEffectUpdateTask(u8 *owner) {
    u8 *task = btlAllocTask(20);
    UnitEffectTaskArgs *arguments;
    BtlUnit *unit = (BtlUnit *)owner;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x16;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = unit->identity;
    *(void **)(task + 0x4C) = btlUpdateSelectedUnitEffect;
    *(void **)(task + 0x50) = btlFinishSelectedUnitEffect;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = unit;
    arguments->effect = 0;
    arguments->counter = 0;
    arguments->duration = 0;
    return task;
}

u32 btlUpdateCommandSoundTask(void) {
    btlUpdateUnitActors();
    return 1;
}

SoundTask *btlCreateCommandSoundUpdateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback.process = btlUpdateCommandSoundTask;
    task->taskId = 0x1B;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

u32 btlUpdateCommandSoundTaskSecondary(void) {
    btlRefreshUnitEffects();
    return 1;
}

u8 *btlCreateSecondaryCommandSoundTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x1C;
    *(u16 *)(task + 0x24) |= 2;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(void **)(task + 0x4C) = btlUpdateCommandSoundTaskSecondary;
    return task;
}

u32 func_001D97D0(void) {
    return 1;
}

SoundTask *func_001D97D8(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback.process = func_001D97D0;
    task->taskId = 0x20;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

typedef struct BtlUnitBaseLightArgs {
    BtlUnit *unit;
    s32 delay;
} BtlUnitBaseLightArgs;
extern f32 *D_00324770[];
extern u8 kwlnDefaultColorVector[];

u32 btlUnitBaseLightTask(BtlUnitBaseLightArgs *work) {
    BtlUnit *unit = work->unit;
    EvtUnit *ext;
    EvtTargetInfo *info;
    EffWorldNode *target;
    if (unit->flags & 2) {
        if (work->delay >= 2) {
            ext = unit->ext;
            evtSetUnitStatusFlags(ext);
            target = (EffWorldNode *)ext->currentTransitionValue;
            if (target != NULL && (ext->flags & 0x40000)) {
                info = target->data;
                PCP_COPY_VECTOR(unit->colorStart, info->firstColor);
                PCP_COPY_VECTOR(unit->colorEnd, info->secondColor);
                PCP_COPY_VECTOR(unit->lightDirection, info->direction);
            } else {
                f32 *defaultLight = D_00324770[0];
                PCP_COPY_VECTOR(unit->colorStart, defaultLight);
                PCP_COPY_VECTOR(unit->colorEnd, kwlnDefaultColorVector);
                PCP_COPY_VECTOR(unit->lightDirection, defaultLight + 4);
                btlBossDebugPrintf("btl:base light error[%p]\n", unit);
            }
            unit->stateFlags |= 0x10;
            btlBossDebugPrintf("btl:base light set[%p]\n", unit);
            return 1;
        }
        work->delay++;
    }
    return 0;
}

void *btlCreateUnitBaseLightTask(BtlUnit *owner) {
    u8 *task = btlAllocTask(8);
    BtlUnitBaseLightArgs *arguments;
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlUnitBaseLightTask;
    *(u16 *)(task + 0x20) = 0x21;
    *(u64 *)(task + 0x40) = owner->identity;
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = owner;
    arguments->delay = 0;
    return task;
}

extern char D_003A3CA0[];

extern f32 effMiscRandUnitFloat(void *);

typedef struct BtlDamageShakeArgs {
    BtlUnit *unit;
    f32 amplitude;
    s32 tick;
} BtlDamageShakeArgs;

u32 btlStiffenDamageShakeStep(BtlDamageShakeArgs *task) {
    f32 pos[4] __attribute__((aligned(16)));
    f32 scale;
    s32 node;

    if (!(task->unit->flags & 2)) {
        return 1;
    }
    if (task->tick == 0) {
        node = mdlGetNodeField2C(task->unit->ext->owner, 0);
        if (node < 0x1D) {
            BtlActorStatusRecord *resource = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(task->unit->resourceKind,
                                                 task->unit->species);
            if (resource->motions[node].kind == 2) {
                btlRefreshUnitEffectMotionAndEntry((s32)task->unit);
                btlBossDebugPrintf(D_003A3CA0);
            }
        }
    }
    if (0.5f < task->amplitude) {
        scale = task->amplitude * (effMiscRandUnitFloat(effSharedRandomState) * 0.5f + 0.5f);
        if (task->tick & 1) {
            scale = -scale;
        }
        if (btlUnitStatusPair(task->unit) & 0x808000000000) {
            effObjFetchInnerFirstVec(task->unit->effectObject);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
            pos[0] += scale;
        } else {
            func_001D6300((u8 *)task->unit, pos);
            pos[0] += scale;
            pos[2] += task->unit->zOffset;
        }
        effObjSetInnerFirstVec(task->unit->effectObject, pos);
        task->amplitude *= 0.85f;
    } else {
        if (btlUnitStatusPair(task->unit) & 0x808000000000) {
            effObjFetchInnerFirstVec(task->unit->effectObject);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        } else {
            func_001D6300((u8 *)task->unit, pos);
            pos[2] += task->unit->zOffset;
        }
        effObjSetInnerFirstVec(task->unit->effectObject, pos);
        return 1;
    }
    task->tick += 1;
    return 0;
}

extern u32 btlStiffenDamageShakeStep(BtlDamageShakeArgs *);

u8 *btlCreateStiffenDamageShakeTask(u8 *owner, f32 value) {
    u8 *task = btlAllocTask(sizeof(BtlDamageShakeArgs));
    BtlDamageShakeArgs *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x1D;
    *(void **)(task + 0x4C) = btlStiffenDamageShakeStep;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments->unit = (BtlUnit *)owner;
    arguments->amplitude = value;
    arguments->tick = 0;
    return task;
}

typedef struct BtlPositionEffectArgs {
    BtlUnit *unit;
    s32 tick;
    f32 amount;
    f32 velocity;
} BtlPositionEffectArgs;

u32 func_001D9C28(BtlPositionEffectArgs *task) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    BtlUnit *unit = task->unit;
    s32 flags;
    s32 approved;
    s32 parameter;
    f32 amplitude;
    f32 offset;
    f32 velocity;
    f32 delta;
    f32 position[4];

    if (!(unit->flags & 2)) {
        return 1;
    }
    flags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
    if ((unit->flags & 0x200) || (flags & 0x200)) {
        approved = 1;
        if (runtime->allowPositionEffect != NULL) {
            approved = runtime->allowPositionEffect(unit);
        }
        if (approved) {
            parameter = 13;
            if (runtime->chooseMotion != NULL) {
                parameter = runtime->chooseMotion(unit, 13, 0);
            }
            if (parameter != -1) {
                btlApplyScaledUnitEffectParameter((u8 *)unit, parameter, 0, 1.0f);
                return 1;
            }
        }
    }
    amplitude = unit->unkBC * unit->scale * 0.8f;
    if (amplitude > 100.0f) {
        amplitude = 100.0f;
    }
    if (task->tick == 0) {
        task->amount = 0.0f;
        task->velocity = 0.3f;
    }
    velocity = task->velocity;
    if (velocity >= 0.0f) {
        offset = amplitude * task->amount;
        task->velocity = velocity + 0.02f;
        delta = (1.0f - task->amount) * velocity;
        task->amount += delta;
        if (task->amount >= 0.99f) {
            task->velocity = -0.17999998f;
        }
    } else {
        offset = amplitude * task->amount;
        task->velocity = velocity - 0.01f;
        delta = task->amount * -velocity;
        task->amount -= delta;
        if (task->amount <= 0.01f) {
            func_001D6300((u8 *)task->unit, position);
            position[2] += task->unit->zOffset;
            effObjSetInnerFirstVec(task->unit->effectObject, position);
            return 1;
        }
    }
    func_001D6300((u8 *)task->unit, position);
    position[0] += offset;
    position[2] += task->unit->zOffset;
    effObjSetInnerFirstVec(task->unit->effectObject, position);
    task->tick++;
    return 0;
}


extern u32 func_001D9C28(BtlPositionEffectArgs *);

u8 *func_001D9E48(u8 *arg0) {
    u8 *task = btlAllocTask(0x10);
    u32 *data;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9C28;
    *(u16 *)(task + 0x20) = 0x1E;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = btlGetTaskArguments(task);
    data[0] = (u32)arg0;
    data[1] = 0;
    return task;
}

u32 func_001D9EC0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *(u32 *)(temp_v0 + 0x110) = *(u32 *)(temp_v0 + 0x110) & 0xffffffef;
    btlSetUnitRotation(temp_v0, temp_v0 + 0x40);
    return 1;
}

void *btlScheduleActorUpdate(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9EC0;
    *(u16 *)(task + 0x20) = 0x1F;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    return task;
}

u32 btlRefreshUnitFxVectorTask(u32 *arg0) {
    btlRefreshUnitFxVectors(*arg0);
    return 1;
}

void *btlCreateUnitFxVectorRefreshTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlRefreshUnitFxVectorTask;
    *(u16 *)(task + 0x20) = 0x22;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments(task) = (u32)owner;
    return task;
}

extern void sdfFreeMemoryFromEitherHeap(s32);

extern s32 fileQueueAlternateCallbackRequest(char *);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3CA0);

void btlStartGunFinishLoad(s32 task) {
    char filename[0x70];
    u8 *actor = *(u8 **)task;
    if ((*(u32 *)(actor + 0x110) & 0x400) != 0) {
        return;
    }
    if (*(s32 *)(actor + 0x30C) != 0) {
        sdfFreeMemoryFromEitherHeap(*(s32 *)(actor + 0x30C));
        *(s32 *)(actor + 0x30C) = 0;
    }
    if (btlFormatUnitBedName(actor, filename)) {
        s32 handle = fileQueueAlternateCallbackRequest(filename);
        *(s32 *)(task + 4) = handle;
        btlBossDebugPrintf("btl:gun & finish load start[%s][%p]\n", filename, handle);
    }
    *(u32 *)(actor + 0x118) = (*(u32 *)(actor + 0x118) | 4) & ~8;
}


typedef struct GunLoadArgs {
    BtlUnit *unit;
    s32 handle;
} GunLoadArgs;

u32 btlPollGunLoad(u32 *arg) {
    GunLoadArgs *args = (GunLoadArgs *)arg;
    BtlUnit *unit = args->unit;
    if (args->handle == 0) {
        return 1;
    }
    if (fileIsRequestReadyInCurrentMode(args->handle) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:gun & finish load end[%p]\n", args->handle);
    unit->gunResource = sdfResourceRetainAddress(fileGetResourceHandle(args->handle));
    filePollEntryCleanup(args->handle);
    unit->gunResourceFlags = (unit->gunResourceFlags & ~4) | 8;
    return 1;
}

extern void btlStartGunFinishLoad(s32);

extern u32 btlPollGunLoad(u32 *);

u8 *btlCreateGunLoadPollTask(u8 *owner) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x23;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlStartGunFinishLoad;
    *(void **)(task + 0x4C) = btlPollGunLoad;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    return task;
}

u32 btlUpdateUnitEffectsTask(void) {
    btlUpdateUnitEffects();
    return 1;
}

SoundTask *btlCreateUpdateUnitEffectsTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback.process = btlUpdateUnitEffectsTask;
    task->taskId = 0x24;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

extern void btlCreateUnitTransparency(BtlUnit *);

typedef struct BtlActorTransparencyArgs {
    BtlUnit *unit;
} BtlActorTransparencyArgs;

s32 btlCreateActorTransparency(BtlActorTransparencyArgs *arguments) {
    BtlUnit *actor = arguments->unit;
    if ((actor->flags & 2) == 0) {
        return 0;
    }
    btlCreateUnitTransparency(actor);
    arguments->unit->flags |= 0x20000;
    return 1;
}

void *btlCreateActorTransparencyTask(BtlUnit *actor) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x25;
    *(void **)(task + 0x4C) = btlCreateActorTransparency;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    ((BtlActorTransparencyArgs *)btlGetTaskArguments(task))->unit = actor;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DA2E0);

extern s32 func_001DA2E0(u32 *);

u8 *btlCreateActorModelBlendTask(u8 *actor, u32 target, u32 index, u32 value, f32 scale) {
    u8 *task = btlAllocTask(0x1C);
    u32 *arguments;
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001DA2E0;
    *(u16 *)(task + 0x20) = 0x26;
    *(u64 *)(task + 0x40) = *(u64 *)(actor + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = (u32)actor;
    arguments[1] = target;
    arguments[2] = index;
    arguments[4] = value;
    *(f32 *)(arguments + 5) = scale;
    arguments[3] = -1;
    arguments[6] = 0;
    return task;
}

/* Update motion completion, alpha transitions, and the selected-unit color pulse. */
void func_001DA468(void) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlActorStatusRecord *status;
    s32 parameter;
    s32 frame;
    s32 index;
    f32 pulse;
    u32 packed[4]; /* SDK color workspace; the low word holds RGBA8888. */

    if (runtime->beforeMotionUpdate != NULL) {
        runtime->beforeMotionUpdate();
    }
    for (unit = runtime->units; unit != NULL; unit = unit->next) {
        if (!(unit->flags & 0x600) || !(unit->flags & 2)) {
            continue;
        }
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
            unit->resourceKind, unit->species);
        if (unit->flags & 0x40000000) {
            btlSeekRandomModelFrame(unit);
            unit->flags &= ~0x40000000;
        }
        if (unit->flags & 0x80000000) {
            parameter = 1;
            if (runtime->chooseMotion != NULL) {
                parameter = runtime->chooseMotion(unit, 1, 0);
            }
            if (unit->unkEC != parameter && parameter != -1) {
                btlApplyScaledUnitEffectParameter((u8 *)unit, parameter, 0, 1.0f);
            } else {
                frame = (s32)btlGetUnitModelValue1C(unit);
                if (frame >= status->model) {
                    sdfMotionSampleAtFrame(unit->ext->owner->first, (f32)status->model);
                    btlResetUnitModelProgress(unit);
                    unit->flags &= ~0x80000000;
                }
            }
        }
        if (unit->updateFlags & 2) {
            if (unit->updateFlags & 4) {
                frame = (s32)btlGetUnitModelValue1C(unit);
                index = unit->unkEC;
                if (index == mdlGetNodeField2C(unit->ext->owner, 0)) {
                    if (frame >= status->motions[index].alphaStartFrame) {
                        evtSetUnitAlphaTransition(unit->ext,
                            (s32)((f32)status->motions[index].alphaDuration /
                                (status->motions[index].alphaFrameScale * runtime->modelFrameScale)),
                            unit->overlayColor & 0xFFFFFF);
                        unit->updateFlags &= ~4;
                    }
                }
            }
            unit->overlayColor = mdlGetBroadcastValue(unit->ext->owner);
        }
        if (unit->flags & 0x8000) {
            pulse = (f32)(runtime->frame % 30) / 15.0f;
            if (pulse > 1.0f) {
                pulse = 2.0f - pulse;
            }
            VU0_SET_ONES_XYZ(vf10);
            VU0_SCALAR_OP(pulse * 1.6f + 0.3f, "vmulx.xyzw vf10, vf10, vf2x");
            EE_MMI_RGBA_PACK(packed[0]);
            btlBlendUnitColor((u8 *)unit, (packed[0] & 0xFFFFFF) | 0x80000000, 0);
        }
    }
}

void btlUpdateActorModelColorAndLinks(void) {
    BtlUnit *unit = ((BtlActorWork *)btlGetRuntime())->actorList;
    s32 color;

    for (; unit != 0; unit = unit->next) {
        if (unit->flags & 2) {
            unit->unkEC = mdlGetNodeField2C(unit->ext->owner, 0);
            if (!(unit->flags & 0x40000)) {
                if (unit->flags & 0x100000) {
                    color = mdlGetBroadcastValue(unit->ext->owner);
                    unit->overlayColor = color;
                    if ((color & 0xFF000000) == 0x80000000) {
                        btlFlagUnitDefeatCandidate(unit);
                        unit->flags &= ~0x100000;
                    }
                } else if (unit->flags & 0x200000) {
                    color = mdlGetBroadcastValue(unit->ext->owner);
                    unit->overlayColor = color;
                    if ((color & 0xFF000000) == 0) {
                        if (!(unit->flags & 0xC0)) {
                            btlClearUnitDefeatCandidate(unit);
                        }
                        unit->flags &= ~0x200000;
                    }
                }
                if (unit->flags & 0x10000) {
                    func_001D6FB0(unit);
                } else {
                    btlUpdateUnitTransparency(unit);
                }
            }
            func_001F2818(unit->resourceLink);
            btlUpdateUnitCommandEffect((SoundLink *)unit->link);
            func_001FC998(unit);
        }
    }
}

void btlResetUnitLinks(u8 *actor) {
    *(s32 *)(actor + 0x2F0) = -1;
    *(s32 *)(actor + 0x2F4) = -1;
    *(u32 *)(actor + 0x110) = 0;
    *(u32 *)(actor + 0x114) = 0;
    *(u32 *)(actor + 0x118) = 0;
    *(u16 *)(actor + 0x310) = 0;
    btlClearAllActorEntrySlots((u32)actor);
    *(u32 *)(actor + 0x2FC) = (u32)sndAllocResourceLink(actor);
    *(u32 *)(actor + 0x300) = (u32)sndAllocLink(actor);
}

BtlUnit *btlCreateUnit(void) {
    u32 handle = sdfAllocGeneralBlock(0x348);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(handle);
    BtlActorWork *work;
    memset(unit, 0, 0x348);
    unit->handle = handle;
    unit->identity = btlAdvanceRuntimeSequenceCounter();
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->lookupId = unit->selectedEntryIndex = -1;
    unit->unk2C4 = 6;
    unit->gunResourceFlags = 0;
    unit->unk314 = 0;
    unit->resourceNode = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults((u8 *)unit);
    btlInitFxLights((u8 *)unit);
    btlResetUnitLinks((u8 *)unit);
    work = (BtlActorWork *)btlGetRuntime();
    unit->previousActor = 0;
    if (work->actorList != 0) {
        work->actorList->previousActor = unit;
        unit->next = work->actorList;
    } else {
        unit->next = 0;
    }
    work->actorList = unit;
    btlBossDebugPrintf("btl:unit create[%p]\n", unit);
    return unit;
}

void btlReleaseUnitResources(BtlUnit *unit) {
    btlBossDebugPrintf("btl:unit data free[%p]\n", unit);
    if (unit->resourceNode != 0) {
        sndFreeResourceNode(unit->resourceNode);
        unit->resourceNode = 0;
    }
    if (unit->resourceLink != 0) {
        sndFreeResourceLink(unit->resourceLink);
        unit->resourceLink = 0;
    }
    if (unit->link != 0) {
        sndFreeLink(unit->link);
        unit->link = 0;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
        unit->gunResourceFlags &= ~4;
        unit->gunResourceFlags &= ~8;
    }
    if (unit->listNode != 0) {
        sndFreeListNode(unit->listNode);
        unit->listNode = 0;
    }
    btlReleaseActorModelResources((u8 *)unit);
    if (unit->unk330 != 0) {
        sdfQueueNonzeroResourceId(unit->unk330);
        unit->unk330 = 0;
        unit->unk32C = 0;
    }
}

extern char D_003A3D38[]; /* "btl:unit delete[%p]\n" */

void btlDestroyUnit(u8 *actor) {
    u8 *next;
    u8 *previous;

    btlBossDebugPrintf(D_003A3D38, actor);
    btlReleaseUnitResources(actor);
    next = *(u8 **)(actor + 0x344);
    if (next != 0) {
        *(u8 **)(next + 0x340) = *(u8 **)(actor + 0x340);
    }
    previous = *(u8 **)(actor + 0x340);
    if (previous != 0) {
        *(u8 **)(previous + 0x344) = *(u8 **)(actor + 0x344);
    } else {
        *(u8 **)(btlGetRuntime() + 0x228) = *(u8 **)(actor + 0x344);
    }
    sdfReleaseResourceAllocation(*(s32 *)(actor + 0x33C));
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlActorWork *)btlGetRuntime())->actorList; unit != 0; unit = next) {
        next = unit->next;
        btlDestroyUnit((u8 *)unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        s32 next = *(s32 *)(actor + 0x344);
        if (*(u32 *)(actor + 0x110) & mask) {
            btlDestroyUnit(actor);
        }
        actor = next;
    }
}

s32 btlFindActorForOwner(s64 target) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        if (*(s64 *)(actor + 0x108) == target) {
            return actor;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

s32 btlIsActiveActor(s32 candidate) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        if (actor == candidate) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

s32 btlFindUnitByModeClear(s32 arg0) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) == 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

s32 btlFindUnitByModeFlagged(s32 arg0) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) != 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

/* Allocate a native header followed by capacity pointer entries. */
BtlIndexList *btlAllocateIndexList(s32 capacity) {
    BtlIndexList *list = sdfAllocAndClearQuadwords(capacity * 4 + 12);
    list->capacity = capacity;
    list->entries = (void **)(list + 1);
    list->count = 0;
    return list;
}

void btlFreeIndexList(BtlIndexList *list) {
    sdfReleaseChipBlock(list);
}

/* The caller keeps the live count within the allocated capacity. */
void btlAppendIndexListEntry(BtlIndexList *list, void *entry) {
    s32 index;

    index = list->count;
    list->count = index + 1;
    list->entries[index] = entry;
}

/* Clear the live count without changing storage or its existing entries. */
void btlClearIndexList(BtlIndexList *list) {
    list->count = 0;
}

/* Return the number of live entries, not the allocated capacity. */
u32 btlGetIndexListCount(BtlIndexList *list) {
    return list->count;
}

/* The caller supplies an in-range index. */
void *btlGetIndexListEntry(BtlIndexList *list, s32 index) {
    return list->entries[index];
}

void btlCopyIndexList(BtlIndexList *destination, BtlIndexList *source) {
    u32 count;
    u32 index;

    btlClearIndexList(destination);
    count = btlGetIndexListCount(source);
    for (index = 0; index < count; index++) {
        btlAppendIndexListEntry(destination, btlGetIndexListEntry(source, index));
    }
}

/* Swap two entries without changing the live count. */
void btlSwapIndexListEntries(BtlIndexList *list, s32 firstIndex, s32 secondIndex) {
    void **entries;
    void *first;
    void *second;

    if (firstIndex == secondIndex) {
        return;
    }
    entries = list->entries;
    first = entries[firstIndex];
    second = entries[secondIndex];
    entries[firstIndex] = second;
    entries[secondIndex] = first;
}

u32 btlFindListIndex(BtlIndexList *list, void *entry) {
    u32 count = btlGetIndexListCount(list);
    u32 index;

    for (index = 0; index < count; index++) {
        if (entry == btlGetIndexListEntry(list, index)) {
            return index;
        }
    }
    return -1;
}

extern void mdlStoreTertiaryVectorVU(MdlCtx *);
extern void mdlSetAmountOnAllContextResources(MdlCtx *, f32);

void btlApplyUnitEffectScale(BtlUnit *unit) {
    BtlEffObjInner *inner;
    if (unit->flags & 2) {
        btlInitializeEffectVectorsFromSourceRecords(unit, unit->resourceKind, unit->species);
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->effectScale, "vmulx.xyzw vf10, vf10, vf2x");
        inner = ((BtlEffObj *)unit->effectObject)->inner;
        inner->flagsC0 |= 1;
        inner->flagsC0 &= ~2;
        VU0_STORE_VF(vf10, inner->vec60);
        mdlStoreTertiaryVectorVU(unit->ext->owner);
        mdlSetAmountOnAllContextResources(unit->ext->owner, unit->effectScale);
        btlSetUnitPosition((u8 *)unit, (u8 *)unit->currentPosition);
    }
}
INCLUDE_ASM(const s32, "game/code_001C8890", func_001DB048);

void btlNormalizeActionCameraKeyScales(s32 action) {
    f32 *key = (f32 *)action;
    u32 flags = *(u32 *)(action + 0xF0);
    u32 i;
    if (flags & 2) {
        for (i = 0; i < 4; i++, key += 12) {
            f32 *pos = key + 12;
            VU0_LOAD_VF(vf10, key + 16);
            VU0_NEGATE_XYZ(vf10);
            VU0_LOAD_VF(vf11, pos);
            VU0_SCALAR_OP(key[20] - 1.0f, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, pos);
            key[20] = 1.0f;
        }
    } else if (flags & 4) {
        f32 *src = key + 12;
        f32 *dst = key + 24;
        for (i = 1; i < 4; i++, dst += 12) {
            f32 delta = dst[8] - src[8];
            dst[0] -= dst[4] * delta;
            dst[1] -= dst[5] * delta;
            dst[2] -= dst[6] * delta;
            dst[8] = src[8];
        }
    }
}

void btlInterpolateVectorStep(f32 *src) {
    f32 vec[4];
    f32 step = -src[8];
    vec[3] = 0.0f;
    vec[0] = src[4] * step + src[0];
    vec[1] = src[5] * step + src[1];
    vec[2] = src[6] * step + src[2];
    VU0_LOAD_VF_MEMORY(vf10, vec);
}

u32 func_001DB358(void) {
    return 1;
}

u32 func_001DB360(void) {
    return 1;
}

u32 func_001DB368(void) {
    return 1;
}

void func_001DB370(CameraPoseTransform *dst, CameraPoseTransform *current, CameraPoseTransform *target, f32 blend) {
    f32 delta;
    f32 value;

    dst->position[0] = current->position[0] + (target->position[0] - current->position[0]) * blend;
    dst->position[1] = current->position[1] + (target->position[1] - current->position[1]) * blend;
    dst->position[2] = current->position[2] + (target->position[2] - current->position[2]) * blend;
    dst->position[3] = 0.0f;
    dst->direction[0] = current->direction[0] + (target->direction[0] - current->direction[0]) * blend;
    dst->direction[1] = current->direction[1] + (target->direction[1] - current->direction[1]) * blend;
    value = current->direction[2];
    dst->direction[2] = value + (target->direction[2] - value) * blend;
    dst->direction[3] = 0.0f;
    delta = target->distance - current->distance;
    dst->distance = current->distance + delta * blend;
    delta = target->fov - current->fov;
    dst->fov = current->fov + delta * blend;
}

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 btlScalarRangeStepQuadratic(u8 *, f32);

extern void func_001DB370(CameraPoseTransform *, CameraPoseTransform *, CameraPoseTransform *, f32);

s32 btlStepPoseBlendHalf(u8 *object) {
    f32 blend;
    if (*(u32 *)(object + 0x110) == 0) {
        *(f32 *)(object + 0x128) = 0.0f;
        btlScalarRangeInitQuadratic(object + 0x13C, (f32)(*(s32 *)(object + 0x12C) * 2));
        btlCopyMotionTransform((u8 *)&((CameraPoseAction *)object)->transform,
                               (u8 *)&((CameraPoseAction *)object)->fromPose);
        return 0;
    }
    blend = btlScalarRangeStepQuadratic(object + 0x13C, 1.0f);
    if (blend > 0.5f) {
        blend = 0.5f;
    }
    func_001DB370(&((CameraPoseAction *)object)->transform, &((CameraPoseAction *)object)->fromPose,
                  &((CameraPoseAction *)object)->savedPose, 2.0f * blend);
    *(f32 *)(object + 0x128) = blend;
    if (blend >= 0.5f) {
        return 1;
    }
    return 0;
}

extern void btlScalarRangeSetStartClearEnd(u8 *, f32);

extern f32 btlScalarRangeStepExponential(u8 *);

extern void btlCopyMotionTransform(u8 *, u8 *);

extern void func_001DB370(CameraPoseTransform *, CameraPoseTransform *, CameraPoseTransform *, f32);

s32 btlStepPoseBlend(u8 *actor) {
    u8 *motion = actor + 0x134;
    CameraPoseTransform *from = &((CameraPoseAction *)actor)->fromPose;
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        btlScalarRangeSetStartClearEnd(motion, *(f32 *)(actor + 0x130));
        btlCopyMotionTransform((u8 *)&((CameraPoseAction *)actor)->transform, (u8 *)from);
    }
    value = btlScalarRangeStepExponential(motion);
    func_001DB370(&((CameraPoseAction *)actor)->transform, from,
                  &((CameraPoseAction *)actor)->savedPose, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 btlScalarRangeStepQuadratic(u8 *, f32);

s32 btlStepPoseBlendFrame(u8 *actor) {
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        *(u32 *)(actor + 0x128) = 0;
        btlScalarRangeInitQuadratic(actor + 0x13C, (f32)*(s32 *)(actor + 0x12C));
        btlCopyMotionTransform((u8 *)&((CameraPoseAction *)actor)->transform,
                               (u8 *)&((CameraPoseAction *)actor)->fromPose);
        return 0;
    }
    value = btlScalarRangeStepQuadratic(actor + 0x13C, 1.0f);
    func_001DB370(&((CameraPoseAction *)actor)->transform, &((CameraPoseAction *)actor)->fromPose,
                  &((CameraPoseAction *)actor)->savedPose, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

s32 btlStepPoseBlendRatio(u8 *actor) {
    f32 ratio = (f32)*(s32 *)(actor + 0x110) / (f32)*(s32 *)(actor + 0x12C);
    if (ratio <= 1.0f) {
        func_001DB370(&((CameraPoseAction *)actor)->transform, &((CameraPoseAction *)actor)->fromPose,
                      &((CameraPoseAction *)actor)->savedPose, ratio);
        return 0;
    }
    btlCopyMotionTransform((u8 *)&((CameraPoseAction *)actor)->transform,
                           (u8 *)&((CameraPoseAction *)actor)->savedPose);
    return 0;
}

s32 func_001DB698(CameraPoseTransform *state) {
    f32 vector[4];
    f32 direction[4];
    f32 length = state->distance;
    f32 y;
    f32 scale;
    f32 delta;
    s32 changed = 0;

    if (!(length <= 1.0f)) {
        scale = -length;
        vector[0] = state->direction[0] * scale + state->position[0];
        y = state->position[1];
        vector[1] = state->direction[1] * scale + y;
        vector[2] = state->direction[2] * scale + state->position[2];
        vector[3] = 0.0f;
        if (-20.0f < vector[1]) {
            VU0_LOAD_VF(vf10, state->position);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, vector);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            delta = y - (-20.0f);
            scale = fsqrtf(delta * delta + length * length);
            vector[0] = -direction[0] * scale;
            vector[1] = delta;
            vector[2] = -direction[2] * scale;
            VU0_LOAD_VF(vf10, vector);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, state->direction);
            changed = 1;
        }
    }
    return changed;
}

/* VU0 math: constrain the pose direction using a horizontal height plane. */
s32 func_001DB7D0(CameraPoseTransform *state, f32 height) {
    f32 vector[4];
    f32 direction[4];
    f32 length = state->distance;
    f32 y;
    f32 scale;
    f32 delta;
    s32 changed = 0;

    if (!(length <= 1.0f)) {
        scale = -length;
        vector[0] = state->direction[0] * scale + state->position[0];
        y = state->position[1];
        vector[1] = state->direction[1] * scale + y;
        vector[2] = state->direction[2] * scale + state->position[2];
        vector[3] = 0.0f;
        if (height < vector[1]) {
            VU0_LOAD_VF(vf10, state->position);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_MOVE_VF(vf11, vf10);
            VU0_LOAD_VF(vf10, vector);
            VU0_SET_VF10_COMPONENT(y, 0.0f);
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            delta = y - height;
            scale = fsqrtf(delta * delta + length * length);
            vector[0] = -direction[0] * scale;
            vector[1] = delta;
            vector[2] = -direction[2] * scale;
            VU0_LOAD_VF(vf10, vector);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, state->direction);
            changed = 1;
        }
    }
    return changed;
}

u32 btlExecuteCommandSoundTask(u32 *arg0) {
    func_001DB048(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

void *btlCreateCommandSoundTask(s32 owner, s32 variant) {
    SoundTask *task = (SoundTask *)btlAllocTask(20);
    u32 *arguments;
    task->startCondition.kind = 1;
    task->taskId = 0x27;
    task->endCondition.kind = 0;
    if (owner != 0 && *(s32 *)(owner + 0x18) != 0) {
        task->owner = ((BtlTask *)owner)->unit->identity;
    }
    task->callback.commandSound = btlExecuteCommandSoundTask;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = owner;
    arguments[3] = variant;
    arguments[1] = 0;
    arguments[2] = 0;
    arguments[4] = 0;
    return task;
}

void *btlCreateTargetedCommandSoundTask(s32 owner, s32 variant, u32 target) {
    void *task = btlCreateCommandSoundTask(owner, variant);
    u32 *arguments = btlGetTaskArguments(task);
    arguments[4] = target;
    return task;
}

void *btlCreateCommandSoundWithArguments(s32 owner, s32 variant, u32 first, u32 second, u32 third) {
    void *task = btlCreateCommandSoundTask(owner, second);
    u32 *arguments = btlGetTaskArguments(task);
    arguments[4] = third;
    arguments[1] = variant;
    arguments[2] = first;
    return task;
}

extern void btlInitMotionTransformFromComponents(u8 *, f32, f32, f32, f32, f32, f32, f32, f32);

u32 btlInitializeMotionTransformFromTaskArguments(u8 *arguments) {
    u8 *context = (u8 *)btlGetRuntime();
    func_001DB048(1, *(u32 *)arguments, 0, 0, 0);
    btlInitMotionTransformFromComponents(context + 0x70, *(f32 *)(arguments + 4), *(f32 *)(arguments + 8),
                    *(f32 *)(arguments + 0xC), *(f32 *)(arguments + 0x10),
                    *(f32 *)(arguments + 0x14), *(f32 *)(arguments + 0x18),
                    *(f32 *)(arguments + 0x1C), *(f32 *)(arguments + 0x20));
    return 1;
}

u8 *btlCreateFloatTask28(u8 *actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
    u8 *task = btlAllocTask(0x24);
    f32 *args;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x28;
    task[0x10] = 0;
    if (actor != 0 && *(u8 **)(actor + 0x18) != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(void **)(task + 0x4C) = btlInitializeMotionTransformFromTaskArguments;
    *(u32 *)(task + 0x48) = 0;
    args = btlGetTaskArguments(task);
    *(u8 **)args = actor;
    args[1] = a;
    args[2] = b;
    args[3] = c;
    args[4] = d;
    args[5] = e;
    args[6] = f;
    args[7] = g;
    args[8] = h;
    return task;
}

extern void func_001DB048(s32, s32, s32, s32, s32);

extern void btlSetEffectCameraKeys(u8 *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 btlApplyEffectCameraKeyframes(f32 *args) {
    s32 context = btlGetRuntime();
    func_001DB048(1, *(s32 *)args, 0, 0, 0);
    btlSetEffectCameraKeys((u8 *)(context + 0x70), args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                  args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]);
    return 1;
}

u8 *btlCreateFloatTask29(u8 *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    u8 *task = btlAllocTask(0x44);
    f32 *args;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x29;
    task[0x10] = 0;
    if (actor != 0 && *(u8 **)(actor + 0x18) != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(void **)(task + 0x4C) = btlApplyEffectCameraKeyframes;
    *(u32 *)(task + 0x48) = 0;
    args = btlGetTaskArguments(task);
    *(u8 **)args = actor;
    args[1] = a1;
    args[2] = a2;
    args[3] = a3;
    args[4] = a4;
    args[5] = a5;
    args[6] = a6;
    args[7] = a7;
    args[8] = a8;
    args[9] = a9;
    args[10] = a10;
    args[11] = a11;
    args[12] = a12;
    args[13] = a13;
    args[14] = a14;
    args[15] = a15;
    args[16] = a16;
    return task;
}

u32 btlRunCameraMotionResetTask(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    btlResetCameraMotion(temp_v0 + 0x70);
    return 1;
}

SoundTask *btlScheduleContextReset(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback.process = btlRunCameraMotionResetTask;
    task->taskId = 0x2A;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DBE68);

extern f32 D_00359EA0[4], D_00359EB0[4];
extern f32 D_00359E80[4], D_00359E90[4];
extern u32 D_003BB660;
extern EffWorldNode *dds3GetWorldCameraObject(EffWorldNode *);
extern EffWorldNode *dds3SetWorldCameraObject(EffWorldNode *, EffWorldNode *);
extern EffWorldNode *dds3CreateCameraObject(s32, void *, void *);
extern void dds3SetCameraFieldOfView(EffWorldNode *, f32);
extern void effObjSetInnerFloat(EffWorldNode *, f32);
extern void dds3EnsureSlotData(void *);
extern void func_001127A0(EffWorldNode *, s32);

/* vu0 routine: add the battle origin to the default camera position. */
void func_001DC0E8(void) {
    f32 position[4];
    EffWorldNode *camera;
    CameraData *data;
    BtlState *battle = (BtlState *)btlGetRuntime();

    battle->cameraCommand.camera.fov = 0.6981317f;
    VU0_LOAD_VF(vf10, D_00359EA0);
    VU0_LOAD_VF(vf11, battle->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, position);
    camera = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (camera != NULL) {
        effObjSetInnerFirstVec(camera, position);
        effObjSetInnerSecondVec(camera, D_00359EB0);
        data = camera->data;
        dds3SetCameraFieldOfView(camera, 0.6981317f);
        data->fovUpdatePending |= 1;
    }
    camera = dds3CreateCameraObject(dds3AdvanceWorldCounter(), D_00359E80, D_00359E90);
    camera->value = D_003BB660;
    effObjSetInnerFloat(camera, 10.0f);
    dds3EnsureSlotData(camera);
    func_001127A0(camera, 0);
    dds3SetCameraFieldOfView(camera, 0.6981317f);
    dds3SetWorldCameraObject(dds3GetWorldObject(), camera);
    battle->cameraObject = camera;
    battle->cameraCommand.targetList = btlAllocateIndexList(13);
    battle->battleFlags |= 0x10;
}


void btlClearPendingSoundList(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    BtlIndexList *list = work->pendingSoundList;
    if (list != 0) {
        btlFreeIndexList(list);
        work->pendingSoundList = 0;
    }
    work->flags &= ~0x10;
}

void btlCopyMotionTransform(u8 *dst, u8 *src) {
    PCP_COPY_VECTOR(dst, src);
    PCP_COPY_VECTOR(dst + 0x10, src + 0x10);
    *(f32 *)(dst + 0x20) = *(f32 *)(src + 0x20);
    *(f32 *)(dst + 0x24) = *(f32 *)(src + 0x24);
}

void func_001DC2A0(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0x24) = arg1;
}

extern void effMiscQuaternionToMatrixVU(void);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];

void btlInitMotionTransformFromVectors(u8 *object, f32 *origin, f32 *direction) {
    VU0_LOAD_VF(vf10, direction);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_0037E110);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, object + 0x10);
    VU0_SET_VF2X(1.0f);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, origin);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, object);
    *(f32 *)(object + 0x20) = 1.0f;
    *(f32 *)(object + 0x24) = 0.6981317f;
    btlClearRuntimeFlag2000();
}

extern void btlInitMotionTransformFromVectors(u8 *, f32 *, f32 *);

void btlInitMotionTransformFromComponents(u8 *object, f32 x, f32 y, f32 z, f32 vx, f32 vy,
                    f32 vz, f32 vw, f32 scale) {
    f32 origin[4];
    f32 direction[4];
    origin[0] = x;
    origin[1] = y;
    origin[2] = z;
    direction[0] = vx;
    direction[1] = vy;
    direction[2] = vz;
    direction[3] = vw;
    origin[3] = 0.0f;
    btlInitMotionTransformFromVectors(object, origin, direction);
    *(f32 *)(object + 0x24) = scale * 0.017453293f;
}

void btlSetEffectCameraKeys(u8 *fx, f32 x0, f32 y0, f32 z0, f32 vx0, f32 vy0, f32 vz0, f32 vw0,
                            f32 x1, f32 y1, f32 z1, f32 vx1, f32 vy1, f32 vz1, f32 vw1,
                            f32 scale, f32 f154) {
    btlInitMotionTransformFromComponents(fx + 0x30, x0, y0, z0, vx0, vy0, vz0, vw0, scale);
    btlInitMotionTransformFromComponents(fx + 0xC0, x1, y1, z1, vx1, vy1, vz1, vw1, scale);
    *(f32 *)(fx + 0x130) = f154;
    *(u32 *)(fx + 0xF0) |= 0x41;
}

u32 btlGetActiveUnitId(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(temp_v0 + 0x174);
}

f32 btlGetPoseBlendProgress(s32 arg0) {
    return *(f32 *)(arg0 + 0x128);
}

s32 btlIsUnitInActiveList(void *unit) {
    BattleController *work = (BattleController *)btlGetRuntime();
    u8 *slot = work->activeSlot;
    u32 count;
    u32 i;
    if (slot != 0 && *(void **)(slot + 0x18) == unit) {
        return 1;
    }
    count = btlGetIndexListCount(work->pendingSoundList);
    for (i = 0; i < count; i++) {
        if (btlGetIndexListEntry(work->pendingSoundList, i) == unit) {
            return 1;
        }
    }
    return 0;
}

void btlResetActiveUnitList(void) {
    BattleController *work;

    work = (BattleController *)btlGetRuntime();
    work->activeSlot = 0;
    work->runtimeFlags = work->runtimeFlags | 0x400;
    btlClearIndexList(work->pendingSoundList);
}

void btlClearRuntimeFlag2000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) & 0xffffdfff;
}

void btlSetRuntimeFlag2000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x2000;
}

u32 btlIsRuntimeFlag2000Clear(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return ((*(s32 *)(temp_v0 + 0x160) >> 0xd) ^ 1U) & 1;
}


typedef struct WorldObjectSub {
    u8 pad0[0x34];
    s32 handle;
} WorldObjectSub;

typedef struct WorldObjectHead {
    u8 pad0[8];
    WorldObjectSub *sub;
} WorldObjectHead;

typedef struct WorldObj {
    u8 pad0[0x18];
    WorldObjectHead *head;
} WorldObj;




extern f32 dds3GetCameraFieldOfView(EffWorldNode *);

extern void sdfSetViewFieldOfView(f32);
void btlRefreshWorldCameraHandle(void) {
    WorldObj *object;
    EffWorldNode *handle;
    if (((BattleController *)btlGetRuntime())->flags & 2) {
        object = dds3GetWorldObject();
        if (object != NULL) {
            handle = dds3GetWorldCameraObject((EffWorldNode *)object);
            if (handle != 0) {
                if (((EffWorldNode *)handle)->next != NULL) {
                    handle = handle->next;
                } else {
                    handle = (EffWorldNode *)object->head->sub->handle;
                }
                dds3SetWorldCameraObject((EffWorldNode *)object, handle);
                sdfSetViewFieldOfView(dds3GetCameraFieldOfView(handle));
            }
        }
    }
}

extern s32 D_003BB668;

extern s32 D_003BB664;

s32 btlGetWorldObjectDefault(void) {
    EffWorldNode *camera;
    if (!(((BattleController *)btlGetRuntime())->flags & 2)) {
        return D_003BB668;
    }
    camera = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (camera == NULL) {
        return D_003BB668;
    }
    if (camera->value == 0) {
        return D_003BB664;
    }
    return (s32)camera->value;
}

s32 btlIsWorldMotionIdle(void) {
    s32 context = btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 2) == 0) {
        return 0;
    }
    {
        EffWorldNode *camera = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (camera == NULL) {
            return 0;
        }
        return camera->value == 0;
    }
}

s32 btlGetCameraVectorWork(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return temp_v0 + 0x70;
}

void btlFlagAllUnitDefeatCandidatesTask(void) {
    btlFlagAllUnitsDefeatCandidate();
}

void btlClearAllUnitDefeatCandidatesTask(void) {
    btlClearAllUnitDefeatCandidates();
}

void btlFlagLinkedGroupDefeatCandidatesTask(s32 arg0) {
    btlFlagMatchingUnitsDefeatCandidate(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0xf4) + 0x18) + 0x110) & 0x600);
}

void btlApplyCombinedActorFlags(u8 *resource) {
    u32 flags = 0;
    u32 count = btlGetIndexListCount(*(struct BtlIndexList **)(resource + 0x118));
    u32 index;
    for (index = 0; index < count; index++) {
        u8 *actor = (u8 *)btlGetIndexListEntry(*(struct BtlIndexList **)(resource + 0x118), index);
        flags |= *(u32 *)(actor + 0x110) & 0x600;
    }
    if (flags != 0) {
        btlFlagMatchingUnitsDefeatCandidate(flags);
    }
}

extern f32 D_00359EC0[];
extern char D_003A3DD0[];
extern s32 mdlGetNodeField2C(MdlCtx *, s32);

extern void sdfMotionSampleAtFrame(Motion *, f32);

void btlResetCameraMotion(s32 action) {
    BtlCameraResetWork *work = (BtlCameraResetWork *)btlGetRuntime();
    BtlUnit *unit;
    f32 current;
    f32 limit;

    if (work->activeUnitId == 1 || btlHasSingleLinkedResource(action) != 0) {
        unit = work->actorList;
        if (unit != 0) {
            f32 fallbackScale = 0.7f;
            for (; unit != 0; unit = unit->next) {
                if (unit->flags & 1) {
                    if (unit->flags & 0x200) {
                        if (unit->flags & 2) {
                            if (unit->ext != 0) {
                                s32 node = mdlGetNodeField2C(unit->ext->owner, 0);
                                if (node == 0xD || node == 0x12) {
                                    current = btlGetUnitModelValue1C(unit);
                                    limit = (f32)btlGetUnitModelFrameCount(unit);
                                    if (unit->partyRecord.unitId < 8) {
                                        limit = limit * D_00359EC0[unit->partyRecord.unitId];
                                    } else {
                                        limit = limit * fallbackScale;
                                    }
                                    if (current < limit) {
                                        sdfMotionSampleAtFrame(unit->ext->owner->first, limit);
                                        btlBossDebugPrintf(D_003A3DD0, unit);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
extern char D_003A3DF0[];
extern char D_003A3E08[];
extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

void btlDebugPrintWorldTransform(s32 arg0, u8 *arg1) {
    EffWorldNode *object;

    if (((BattleController *)btlGetRuntime())->flags & 2) {
        object = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (object != 0) {
            btlBossDebugPrintfN(arg0, (s32)arg1, 0, (s32)D_003A3DF0,
                                (double)object->inner->position[0],
                                (double)object->inner->position[1],
                                (double)object->inner->position[2]);
            btlBossDebugPrintfN(arg0, (s32)(arg1 + 0xC), 0, (s32)D_003A3E08,
                                (double)object->inner->rotation[0],
                                (double)object->inner->rotation[1],
                                (double)object->inner->rotation[2],
                                (double)object->inner->rotation[3]);
        }
    }
}

extern void func_001F6E28(BtlIndexList *, s32, s32);

void btlFaceActionParticipantsTowardLinkedTarget(CameraPoseAction *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    BtlUnit *first;
    u32 i;
    u32 count = btlGetIndexListCount(action->actorIndices);
    if (count != 0) {
        target = action->link->unit;
        if (count == 1) {
            first = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
            if (target != 0 && (target->flags & 0x600) == (first->flags & 0x600)) {
                return;
            }
            btlUnitGetMuzzlePosVU(first);
        } else {
            func_001F6E28(action->actorIndices, 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if (target->flags & 0x80000) {
                if (btlAimHorizontalDirectionVU(&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation((u8 *)target, &vec[2]);
                }
            }
        }
        for (i = 0; i < count; i++) {
            btlUnitFaceTarget((u8 *)btlGetIndexListEntry(action->actorIndices, i), (u8 *)target);
        }
    }
}
void btlAimLinkedUnitAtMuzzle(u8 *action) {
    s128 vec[3];
    s128 *pos;
    u8 *target;
    u32 count = btlGetIndexListCount(*(struct BtlIndexList **)(action + 0x118));
    if (count != 0) {
        target = *(u8 **)(*(s32 *)(action + 0xF4) + 0x18);
        if (count == 1) {
            btlUnitGetMuzzlePosVU(btlGetIndexListEntry(*(struct BtlIndexList **)(action + 0x118), 0));
        } else {
            func_001F6E28(*(BtlIndexList **)(action + 0x118), 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if (*(u32 *)(target + 0x110) & 0x80000) {
                if (btlAimHorizontalDirectionVU(&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation(target, &vec[2]);
                }
            }
        }
    }
}
INCLUDE_ASM(const s32, "game/code_001C8890", btlMatchFirstLinkedActorFlags);

/* Check actor status, linked-group marks, and owner restrictions before use. */
s32 btlCanUseLinkedActor(s32 actor) {
    u32 status = *(u32 *)(actor + 0x104);
    BattleActionLinkState *linked;
    s32 category;
    u32 count;
    BtlOperandGroup *entry;
    u32 i;

    switch (status) {
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    default:
        return 1;
    }
    linked = ((CameraPoseAction *)actor)->link;
    if (linked == 0) {
        return 1;
    }
    count = btlGetIndexListCount(linked->actorIndices);
    entry = linked->groups;
    for (i = 0; i < count; i++, entry++) {
        if (entry->unk14 != 0) {
            return 0;
        }
    }
    if (((BtlUnit *)linked->unit)->partyRecord.status & 0x480) {
        return 0;
    }
    category = *(s32 *)(actor + 0x114);
    if (category != 0 && (*(u16 *)(datActionAnimationRecords + category * 32 + 0x1C) & 1)) {
        return 0;
    }
    return 1;
}

/* Return whether a live linked group has its byte at 0x10 marked. */
u32 btlHasMarkedEntry10(u8 *object) {
    BattleActionLinkState *resource = ((CameraPoseAction *)object)->link;
    u32 count;
    u32 index;
    BtlOperandGroup *entry;
    if (resource == 0) {
        return 0;
    }
    count = btlGetIndexListCount(resource->actorIndices);
    entry = resource->groups;
    for (index = 0; index < count; index++, entry++) {
        if (entry->skipped != 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlGetRuntime(void);

extern f32 btlUnitGetTopY(BtlUnit *);

s32 btlCheckActorDistanceLimit(void) {
    s32 actor = *(s32 *)(btlGetRuntime() + 0x228);

    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY((BtlUnit *)actor) > 400.0f) {
                    return 0;
                }
            }
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 1;
}

extern f32 func_001F66D8(s32, f32 *, s32);

s32 btlIsEntryHeightWithinLimit(void) {
    if (func_001F66D8(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

/* Find an unmarked linked kind-two slot whose unit is not disabled. */
s32 btlHasIdleLinkedSlotKindTwo(u8 *actor) {
    BattleActionLinkState *linked = ((CameraPoseAction *)actor)->link;
    u32 count;
    u32 i;
    BtlOperandGroup *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(linked->actorIndices);
    entry = linked->groups;
    for (; i < count; i++, entry++) {
        if (entry->skipped == 0 && entry->kind == 1 && entry->unk0C == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(linked->actorIndices, i))->flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

/* Find a type-two linked group whose associated unit is not disabled. */
s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor) {
    BattleActionLinkState *linked = ((CameraPoseAction *)actor)->link;
    u32 count;
    u32 i;
    BtlOperandGroup *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(linked->actorIndices);
    entry = linked->groups;
    for (; i < count; i++, entry++) {
        if (entry->kind == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(linked->actorIndices, i))->flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasLinkedEffectNodeTrigger(u8 *fx) {
    u8 *task;
    u8 *owner;
    s32 index;
    u8 *table;
    if (*(s32 *)(fx + 0xF4) == 0) {
        return 0;
    }
    if (btlHasSingleLinkedResource() == 0) {
        return 0;
    }
    task = *(u8 **)(fx + 0xF4);
    owner = *(u8 **)(task + 0x18);
    index = *(s32 *)(task + 0x44);
    table = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(owner + 0xC4), *(s32 *)(owner + 0xC8));
    return *(s16 *)(table + index * 0x14 + 0x2C) == 2;
}

s32 btlHasActorCategoryFlag100(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 0x100) == 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorCategoryTypeTwo(s32 arg0) {
    s32 temp_v1;

    temp_v1 = *(s32 *)(arg0 + 0x114);
    if (temp_v1 == 0) {
        return 0;
    }
    return ((*(s32 *)(datCommandRecords + temp_v1 * 56 + 0x30) ^ 2) < 1U);
}

u32 btlCanUseActorCategoryFlag2(s32 actor) {
    s32 index;
    if (btlIsActorCategoryMarked(actor) != 0) {
        return 1;
    }
    if (btlCanUseLinkedActor(actor) == 0) {
        return 0;
    }
    index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 2) != 0) {
        return 1;
    }
    return 0;
}

s32 btlHasSingleLinkedResource(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index != 0 && *(u8 *)(datCommandRecords + index * 56 + 8) != 0) {
        return 0;
    }
    return btlGetIndexListCount(*(struct BtlIndexList **)(actor + 0x118)) == 1;
}

u32 func_001DD2C0(s32 actor) {
    s32 index;
    if (btlCanUseLinkedActor(actor) == 0) {
        return 0;
    }
    index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 4) != 0) {
        return 1;
    }
    return 0;
}

s32 btlIsActorCategoryMarked(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    return *(s32 *)(datCommandRecords + index * 56 + 0x30) == 1;
}

s32 btlHasActorCategoryFlag40(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 0x40) == 0) {
        return 0;
    }
    return 1;
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    u8 *entry;

    switch (*(u32 *)(actor + 0x104)) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = *(s32 *)(actor + 0xF4);
    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(*(struct BtlIndexList **)(linked + 0x60)) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(*(struct BtlIndexList **)(linked + 0x60), 0);
    return ((*(u32 *)(*(s32 *)(linked + 0x18) + 0x110) ^ *(u32 *)(entry + 0x110)) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(u8 *node) {
    u8 *resource = *(u8 **)(node + 0xF4);
    u8 *actor;
    u32 id;
    if (resource == 0) return 0;
    if (btlGetIndexListCount(*(struct BtlIndexList **)(resource + 0x60)) >= 2) return 0;
    actor = (u8 *)btlGetIndexListEntry(*(struct BtlIndexList **)(resource + 0x60), 0);
    if ((*(u32 *)(actor + 0x110) & 0x400) == 0) return 0;
    id = *(u32 *)(actor + 0xC8);
    if (id >= 0x180) return 0;
    if (*(u32 *)(datEnemyRecords + id * 76) & 0x1000) return 1;
    return 0;
}

u8 func_001DD488(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x5f;
}

u8 func_001DD498(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x1a0;
}

void func_001DD4A8(void) {
}

void func_001DD4B0(void) {
}

extern void btlPrepareActionCameraPoseWithActorClearance(void *unit, f32 *pose, u8 *out);
extern void func_001E4708(void *unit, f32 *pose, u8 *out);
extern void func_001E4720(CameraPoseTransform *, CameraPoseTransform *, CameraPoseTransform *);

/* Choose the action's camera pose from active ally and enemy height maxima. */
void btlChooseCameraPoseByActorHeights(CameraPoseAction *action) {
    BtlUnit *unit;
    s32 enemyCount = 0;
    f32 enemyHeight = 0.0f;
    f32 allyHeight = 0.0f;
    f32 height;

    unit = ((BtlActionPoseRuntime *)btlGetRuntime())->actorHead;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            height = btlUnitGetTopY(unit);
            if (unit->flags & 0x200) {
                if (allyHeight < height) {
                    allyHeight = height;
                }
            } else if (unit->flags & 0x400) {
                if (enemyHeight < height) {
                    enemyHeight = height;
                }
                enemyCount++;
            }
        }
    }
    if (enemyCount == 1 && allyHeight + 100.0f < enemyHeight) {
        switch (effMiscRandMod(0, 4)) {
        case 0:
        case 1:
            if (enemyHeight <= 500.0f) {
                func_001E4720(&action->transform, &action->fromPose, &action->savedPose);
            } else {
                btlPrepareActionCameraPoseWithActorClearance(action, (f32 *)&action->fromPose, (u8 *)&action->savedPose);
            }
            break;
        case 2:
            btlPrepareActionCameraPoseWithActorClearance(action, (f32 *)&action->fromPose, (u8 *)&action->savedPose);
            break;
        case 3:
            func_001E4708(action, (f32 *)&action->fromPose, (u8 *)&action->savedPose);
            break;
        }
    } else {
        switch (effMiscRandMod(0, 2)) {
        case 0:
            btlPrepareActionCameraPoseWithActorClearance(action, (f32 *)&action->fromPose, (u8 *)&action->savedPose);
            break;
        case 1:
            func_001E4708(action, (f32 *)&action->fromPose, (u8 *)&action->savedPose);
            break;
        }
    }
    action->unk130 = 100.0f;
    action->flags |= 0x41;
}

void func_001DD678(void) {
}

void func_001DD680(u32 arg0) {
    func_001DF358(arg0, arg0);
}

void func_001DD698(void) {
}

extern void btlFlagUserAndTargetDefeat(u8 *, u8 *);
extern void btlInitTargetCursorAndFacing(CameraPoseAction *, void *);
extern void func_001EEAE0(CameraPoseAction *, CameraPoseAction *);
extern void func_001E2FF8(CameraPoseAction *);
extern void btlSetupActionCameraPair(CameraPoseAction *);

void btlInitializeLinkedActionCamera(CameraPoseAction *action) {
    s32 (*hook)(CameraPoseAction *) = ((BattleController *)btlGetRuntime())->actionCameraInitHook;
    BattleActionLinkState *link;

    action->cameraKind = 0;
    link = action->link;
    if (hook != NULL && hook(action) != 0) {
        return;
    }
    if (link->unit->flags & 0x200) {
        if (link->unit->flags & 0x1000) {
            if (btlHasSingleLinkedResource((s32)action)) {
                action->cameraKind = 9;
                btlFlagUserAndTargetDefeat((u8 *)action, (u8 *)action);
            } else {
                btlInitTargetCursorAndFacing(action, action);
            }
        } else {
            action->cameraKind = 11;
            func_001EEAE0(action, action);
        }
    } else {
        if (btlMatchLinkedActorFlags((s32)action)) {
            func_001E2FF8(action);
        } else if (btlHasSingleLinkedResource((s32)action)) {
            action->cameraKind = 10;
            btlSetupActionCameraPair(action);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, (f32 *)&action->fromPose,
                                              (u8 *)&action->savedPose);
            btlAimLinkedUnitAtMuzzle((u8 *)action);
            action->unk130 = 200.0f;
            action->flags |= 0x41;
        }
        btlResetCameraMotion((s32)action);
    }
}

extern void btlBuildApproachCamera(CameraPoseAction *, CameraPoseTransform *);
extern void btlUpdateActionTargetCameraPose(CameraPoseAction *);
extern void btlAdvanceCursorForUnmarkedUnit(s32, s32);

void func_001DD7E8(s32 actor) {
    s32 (*callback)(s32) = *(s32 (**)(s32))(btlGetRuntime() + 0x614);

    if (callback != 0 && callback(actor) != 0) {
        return;
    }

    switch (*(u16 *)(actor + 0x10C)) {
    case 9:
        btlBuildApproachCamera((CameraPoseAction *)actor, &((CameraPoseAction *)actor)->transform);
        break;
    case 10:
        btlUpdateActionTargetCameraPose((CameraPoseAction *)actor);
        break;
    case 11:
        btlAdvanceCursorForUnmarkedUnit(actor, actor);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DD890);

extern void btlRefreshActionPoseBlendSnapshot();
extern void btlAimEffectPoseAtUnit();
extern void func_001E0100();
extern void func_001E5718(CameraPoseAction *, CameraPoseTransform *);
extern void btlBuildHeightClampedApproachCamera(CameraPoseAction *, CameraPoseTransform *);

/* Let the runtime hook handle the actor before dispatching its camera step. */
INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D38);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D50);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D60);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D70);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D80);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D90);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3DD0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3DF0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3E08);

void btlDispatchActionCameraStep(u8 *actor) {
    BattleController *runtime = (BattleController *)btlGetRuntime();
    s32 (*callback)(u8 *) = runtime->actionCameraStepHook;

    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    switch (*(u16 *)(actor + 0x10C)) {
    case 2:
        btlRefreshActionPoseBlendSnapshot(actor, actor);
        break;
    case 9:
        btlBuildApproachCamera((CameraPoseAction *)actor, &((CameraPoseAction *)actor)->transform);
        break;
    case 4:
        btlAimEffectPoseAtUnit(actor, actor);
        break;
    case 5:
        func_001E0100(actor, actor);
        break;
    case 7:
        func_001E5718((CameraPoseAction *)actor, &((CameraPoseAction *)actor)->transform);
        break;
    case 8:
        btlBuildHeightClampedApproachCamera((CameraPoseAction *)actor, &((CameraPoseAction *)actor)->transform);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DDF20);

void btlAdvanceTargetCursorUnlessHookHandles(u8 *actor) {
    s32 (*callback)(u8 *) = *(s32 (**)(u8 *))(btlGetRuntime() + 0x634);
    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    if (*(u16 *)(actor + 0x10C) == 12) {
        btlAdvanceTargetCursorAnimation(actor, actor);
    }
}

extern void func_001E3E58(CameraPoseAction *, u8 *, BtlUnit *, s32);

extern void btlPrepareUnitPoseWithTiltRotation();

void btlUpdateActionPoseForLinkedTarget(CameraPoseAction *action) {
    BtlUnit *target;

    if (((BtlActionPoseRuntime *)btlGetRuntime())->flags & 1) {
        target = action->link->unit;
        if (target->flags & 0x400) {
            func_001DF358((s32)action, (s32)action);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform((u8 *)&action->fromPose, (u8 *)&action->transform);
        func_001E3E58(action, (u8 *)&action->savedPose, action->link->unit, 0);
        ((BattlePoseBlendState *)action)->duration = 7.0f;
        action->flags = (action->flags | 0x1041) & 0xFFFBFFFF;
    } else {
        func_001E3E58(action, (u8 *)action, action->link->unit, 0);
    }
}
void func_001DE5F0(void) {
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DE5F8);

void func_001DE958(void) {
}

extern void func_001E0DA0(CameraPoseAction *, f32 *, s32, f32, f32);

void func_001DE960(CameraPoseAction *action) {
    BtlUnit *target;
    s32 kind;
    f32 pos[4];
    if (btlGetIndexListCount(action->actorIndices) != 1) {
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
    if (action->link->unit->flags & 0x200) {
        func_001E3E58(action, action, target, 0);
        return;
    }
    if (btlHasActorCategoryFlag100((s32)action) != 0) {
        return;
    }
    btlUnitGetBodyPosVU(target);
    VU0_STORE_VF(vf10, pos);
    if (pos[0] > 0.0f) {
        kind = 2;
    } else {
        kind = 3;
    }
    func_001E0DA0(action, (f32 *)&action->fromPose, kind, 45.0f, 0.25f);
    func_001E0DA0(action, (f32 *)&action->savedPose, kind, 1.0f, 0.5f);
    ((BattlePoseBlendState *)action)->duration = 30.0f;
    action->flags |= 0x41;
}

void func_001DEA68(void) {
}

void btlStartLinkedActionPoseBlendIfEligible(CameraPoseAction *action) {
    BtlWorkPoseBlendHook *work = (BtlWorkPoseBlendHook *)btlGetRuntime();
    if (action->link->unit->flags & 0x400) {
        if (work->hook628 != 0) {
            s32 hasFlag200 = 0;
            s32 hasFlag400 = 0;
            u32 i;
            u32 count = btlGetIndexListCount(action->link->actorIndices);
            for (i = 0; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(action->link->actorIndices, i);
                if (entry->flags & 0x200) {
                    hasFlag200 = 1;
                }
                if (entry->flags & 0x400) {
                    hasFlag400 = 1;
                }
            }
            if (work->hook628(action, hasFlag200, hasFlag400) != 0) {
                action->flags |= 0x10000;
                return;
            }
        }
        btlPrepareUnitPoseWithTiltRotation(action, (f32 *)&action->fromPose, (u8 *)&action->savedPose);
        ((BattlePoseBlendState *)action)->duration = 200.0f;
        action->flags |= 0x10041;
    } else {
        func_001E5800(action, action);
    }
}
void btlAdvanceUnblockedPlayerCursorAnimation(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    btlAdvancePlayerCursorAnimation(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DEBE0);

void func_001DEDB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    func_001E6260(arg0, temp_v0);
}

void func_001DEDE8(u32 arg0) {
    func_001E6368(arg0, arg0);
}

void btlAdvanceCommandCursorTask(u32 arg0) {
    btlAdvanceCommandCursor(arg0, arg0);
}

void func_001DEE18(u32 arg0) {
    func_001E4AC0(arg0, (s32)&((CameraPoseAction *)arg0)->fromPose,
                 (s32)&((CameraPoseAction *)arg0)->savedPose);
}

void func_001DEE38(void) {
    func_001E4E50();
}

void btlStartLinkedDefeatCandidateAction(u8 *actor) {
    u8 *resource = *(u8 **)(actor + 0xF4);
    btlAppendIndexListEntry(*(struct BtlIndexList **)(actor + 0x118), *(void **)(resource + 0x18));
    func_001E5198(actor, (u8 *)&((CameraPoseAction *)actor)->fromPose,
                 (u8 *)&((CameraPoseAction *)actor)->savedPose);
    btlClearAllUnitDefeatCandidates();
    resource = *(u8 **)(actor + 0xF4);
    btlFlagUnitDefeatCandidate(*(BtlUnit **)(resource + 0x18));
    *(f32 *)(actor + 0x130) = 50.0f;
    *(u32 *)(actor + 0xF0) |= 0x41;
}

void func_001DEEC0(void) {
}

extern void btlBuildGroupFramingCameraPose(CameraPoseTransform *, CameraPoseTransform *);

void btlUpdateLinkedActionEffectVectorByTarget(CameraPoseAction *action) {
    if ((action->link->unit->flags & 0x200) != 0) {
        btlBuildGroupFramingCameraPose(&action->transform, &action->transform);
        return;
    }
    if (action->actionKind != 0x10) {
        btlResetUnitEffectVector((s32)&action->transform, (s32)&action->transform);
        return;
    }
}

void func_001DEF20(void) {
}

void btlInitializeCursorForLinkedAction(s32 arg0) {
    if (*(s32 *)(arg0 + 0xf4) != 0) {
        btlInitLinkedUnitActionCursor(*(s32 *)(arg0 + 0xf4));
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DEFE0);

void func_001DF358(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 27.5f);
}

extern void func_002DD688(f32 angle);

void btlPrepareUnitPoseWithTiltRotation(unit, pose, out)
void *unit;
f32 *pose;
u8 *out;
{
    func_001DEFE0((s32)unit, (s32)pose, 20.0f);
    btlCopyMotionTransform(out, (u8 *)pose);
    if (pose[4] > 0.0f) {
        func_002DD688(-(20.0f * 0.017453293f));
    } else {
        func_002DD688(20.0f * 0.017453293f);
    }
    VU0_STORE_VF(vf10, pose + 4);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, out + 0x10);
}
INCLUDE_ASM(const s32, "game/code_001C8890", func_001DF410);

extern f32 func_002FA148(f32);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3E40);

void btlPrepareRandomizedActionCameraPose(CameraPoseAction *action, CameraPoseTransform *from,
                                           CameraPoseTransform *to) {
    f32 quat[4];
    /* Quaternion rows, distance multiplier, camera parameter, and padding. */
    f32 poses[4][12] = {
        {0.0f, -0.94f, 0.02f, 0x1.333332p-2f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
        {0.06f, -0.94f, -0x1.70a3d6p-3f, 0.25f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
        {0.0f, -0.94f, 0.02f, -0x1.333332p-2f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
        {-0.06f, -0.94f, -0x1.70a3d6p-3f, -0.25f, 0.0f, -1.0f, 0.0f, 0.0f, 1.5f, 35.0f, 0.0f, 0.0f},
    };
    BtlUnit *unit = action->link->unit;
    u32 flags = unit->flags;
    s32 pose;
    f32 fov;
    f32 half;
    f32 span;
    f32 distance;

    if (flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(flags & 0x600);
        btlCopyUnitRotationQuaternion((u8 *)unit, quat);
        pose = effMiscRandMod(0, 4);
        fov = action->transform.fov;
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
        func_001DB698(from);
        func_001DB698(to);
        action->unk130 = poses[pose][9];
        action->flags |= 0x41;
    }
}

void btlAimEffectPoseAtUnit(CameraPoseAction *actor) {
    BtlUnit *object = actor->link->unit;
    if ((object->flags & 2) != 0) {
        if (btlSetActorEffectParameter((u8 *)object, 1) == 0) {
            btlUnitGetMuzzlePosVU(object);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, actor->savedPose.position);
        func_001DB698(&actor->savedPose);
    }
}

extern void func_001E1288(CameraPoseAction *, CameraPoseTransform *, u8);
extern void func_001E16C0(CameraPoseAction *, CameraPoseTransform *);
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
void func_001DFAE0(CameraPoseAction *action, CameraPoseTransform *to, CameraPoseTransform *from) {
    f32 muzzle[4];
    f32 quat[4];
    CameraAimPose poses[2] = {
        {{0x1.70a3d6p-3f, -0x1.999998p-2f, -0x1.70a3d6p-4f, 0.89f}, 3.0f, 0x1.999998p-1f, 30.0f, 0.0f},
        {{0x1.70a3d6p-3f, 0x1.999998p-2f, 0x1.70a3d6p-4f, 0.89f}, 3.0f, 0x1.999998p-1f, 30.0f, 0.0f},
    };
    BtlUnit *unit = action->link->unit;
    s32 pose;
    f32 distance;
    f32 fov;

    if (unit->flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(unit->flags & 0x600);
        if (btlHasSingleLinkedResource((s32)action) == 0) {
            func_001E16C0(action, from);
        } else {
            func_001E1288(action, from, 1);
        }
        btlCopyUnitRotationQuaternion((u8 *)unit, quat);
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
        fov = action->transform.fov;
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
        func_001DB698(to);
        action->unk12C = func_001D6050(unit, unit->unkEC);
        action->flags |= 0x815;
        action->unk11C = 0;
    }
}

void btlRefreshActionPoseBlendSnapshot(CameraPoseAction *action) {
    CameraPoseTransform *saved;
    if (!(action->flags & 1) && action->unk11C == 0) {
        saved = &action->savedPose;
        btlCopyMotionTransform((u8 *)&action->fromPose, (u8 *)&action->transform);
        btlCopyMotionTransform((u8 *)saved, (u8 *)&action->transform);
        action->savedPose.distance += 125.0f;
        action->flags = (action->flags & ~0x14) | 0x41;
        action->state = 0;
        action->unk11C = 1;
        action->unk130 = 40.0f;
        func_001DB698(saved);
    }
}
INCLUDE_ASM(const s32, "game/code_001C8890", func_001DFE28);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E0100);

extern void btlClearAllUnitDefeatCandidates(void);
extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlSetupCameraPoseAimUnit(CameraPoseAction *action, CameraPoseTransform *from, CameraPoseTransform *to) {
    f32 quat[4];
    BtlUnit *unit = action->link->unit;
    f32 fov;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->flags & 0x600);
    btlCopyUnitRotationQuaternion((u8 *)unit, quat);
    fov = action->transform.fov;
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
    btlCopyMotionTransform((u8 *)to, (u8 *)from);
    to->distance += 550.0f;
    action->flags = (action->flags & ~0x14) | 0x41;
    action->unk130 = 25.0f;
    func_001DB698(from);
    func_001DB698(to);
}
INCLUDE_ASM(const s32, "game/code_001C8890", func_001E0398);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E0718);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E0B68);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E0DA0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E1288);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E16C0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E1CF8);

extern void btlPrepareUnitPoseWithTiltRotation();

void btlActionAimUserAtTargets(u8 *action, f32 *pose, u8 *out) {
    typedef struct BtlActionAimLink {
        u8 pad00[0x18];
        BtlUnit *unit;
    } BtlActionAimLink;
    typedef struct BtlActionAim {
        u8 pad00[0xF4];
        BtlActionAimLink *link;
        u8 padF8[0x20];
        struct BtlIndexList *actorIndices;
    } BtlActionAim;
    BtlActionAim *command = (BtlActionAim *)action;
    s128 vec[3];
    BtlUnit *unit = command->link->unit;
    u32 mask = 0;
    u32 i;
    u32 count;
    btlPrepareUnitPoseWithTiltRotation(command, pose, out);
    count = btlGetIndexListCount(command->actorIndices);
    for (i = 0; i < count; i++) {
        mask |= ((BtlUnit *)btlGetIndexListEntry(command->actorIndices, i))->flags & 0x600;
    }
    if (unit->flags & 0x80000) {
        func_001F66D8(mask, 0, 0);
        VU0_STORE_VF(vf10, &vec[0]);
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU(&vec[1], &vec[0]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation((u8 *)unit, &vec[2]);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E20C0);

extern void func_001E20C0(CameraPoseAction *, CameraPoseTransform *, s32, f32 *, f32, f32);
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
void func_001E2578(CameraPoseAction *action, CameraPoseTransform *from, CameraPoseTransform *to) {
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
    if ((u32)(action->actionId - 1) < 0x1FF) {
        animationFlags = ((BtlActionAnimationRecord *)datActionAnimationRecords)[action->actionId].flags;
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
    count = btlGetIndexListCount(action->actorIndices);
    for (i = 0; i < count; i++) {
        groups |= ((BtlUnit *)btlGetIndexListEntry(action->actorIndices, i))->flags & 0x600;
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
    action->unk130 = poses[pose].parameter;
}

void btlFlagUserAndTargetDefeat(u8 *command, u8 *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = *(BtlUnit **)(*(s32 *)(command + 0xF4) + 0x18);
    target = (BtlUnit *)btlGetIndexListEntry(*(struct BtlIndexList **)(command + 0x118), 0);
    if (!(user->flags & target->flags & 0x600)) {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagMatchingUnitsDefeatCandidate(target->flags & 0x600);
    } else {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate(user);
        btlFlagUnitDefeatCandidate(target);
    }
    btlUnitGetMuzzlePosVU(user);
    VU0_STORE_VF_UNCLOBBERED(vf10, userPos);
    btlUnitGetMuzzlePosVU(target);
    VU0_STORE_VF_UNCLOBBERED(vf10, targetPos);
    btlUnitFaceTarget((u8 *)target, (u8 *)user);
    if (userPos[0] < targetPos[0]) {
        *(u32 *)(command + 0xF0) |= 0x200;
    } else {
        *(u32 *)(command + 0xF0) &= ~0x200;
    }
}

extern f32 func_001A47F0(BattleActionLinkState *);
extern f32 func_002F9F60(f32);
extern f32 func_002FA060(f32);

#define BTL_APPROACH_DIST_START 0.55f
#define BTL_APPROACH_DIST_END 0.6f
#define BTL_APPROACH_PITCH_START 0.6108652f /* 35 degrees */
#define BTL_APPROACH_PITCH_END 0.6108652f

/* vu0 routine: */
void btlBuildApproachCamera(CameraPoseAction *action, CameraPoseTransform *out) {
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
    target = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
    extent = user->reach * user->scale;
    span = func_001D6050(user, user->unkEC);
    span /= func_001A47F0(action->link);
    ratio = (f32)action->state / span;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    out->fov = action->transform.fov;
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
    func_001DB7D0(out, -10.0f);
}

void btlSetupActionCameraPair(CameraPoseAction *command) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 lookPos[4];

    user = command->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(command->actorIndices, 0);
    if (!(user->flags & target->flags & 0x600)) {
        btlFlagAllUnitsDefeatCandidate();
    } else {
        func_001DF358((s32)command, (s32)command);
        return;
    }
    func_001E1CF8(command, (f32 *)&command->fromPose);
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
    command->unk11C = 0;
    command->unk130 = 15.0f;
    btlInterpolateVectorStep(command->fromPose.position);
    VU0_STORE_VF_UNCLOBBERED(vf10, lookPos);
    btlUnitGetMuzzlePosVU(user);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, command->fromPose.position);
    VU0_LERP_VF10(0.25f);
    VU0_STORE_VF(vf10, command->fromPose.position);
    VU0_LOAD_VF(vf11, lookPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, command->fromPose.direction);
    btlUnitFaceTarget((u8 *)user, (u8 *)target);
}
/* vu0 routine: update the action camera's saved target pose. */
void btlUpdateActionTargetCameraPose(CameraPoseAction *action) {
    BtlUnit *user;
    BtlUnit *target;
    CameraPoseTransform *out;
    f32 targetPos[4];
    f32 userPos[4];
    f32 dir[4];
    f32 length;
    f32 extent;
    f32 angle;
    s32 frames;
    s32 idle;
    s32 eligible;

    out = &action->savedPose;
    user = action->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
    if (target->flags & user->flags & 0x600) {
        return;
    }
    if (action->unk11C != 0) {
        return;
    }
    frames = func_001D6050(user, user->unkEC);
    frames = (s32)((f32)frames / func_001A47F0(action->link));
    if (action->state == frames && (target->flags & 0x200)) {
        idle = btlHasIdleLinkedSlotKindTwo((u8 *)action);
        eligible = btlHasEligibleLinkedEntryTypeTwo((u8 *)action);
        if (idle == 0 && eligible == 0) {
            return;
        }
        btlCopyMotionTransform((u8 *)&action->fromPose, (u8 *)&action->transform);
        btlCopyMotionTransform((u8 *)out, (u8 *)&action->transform);
        action->savedPose.distance += idle != 0 ? 500.0f : 300.0f;
        action->flags = (action->flags & ~0x14) | 0x41;
        action->unk11C = 1;
        action->unk130 = 10.0f;
        action->state = 0;
        func_001DB698(out);
    } else {
        extent = target->reach * target->scale * 2.25f;
        out->fov = action->transform.fov;
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
        func_001DB698(out);
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E2FF8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E3310);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E37B0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E3920);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E3E58);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4180);

/* vu0 routine: measure camera clearance from the actor's adjusted muzzle position. */
void btlPrepareActionCameraPoseWithActorClearance(void *unit, f32 *pose, u8 *out) {
    BtlUnit *actor = ((BtlActorWork *)btlGetRuntime())->actorList;
    f32 *target = (f32 *)out;
    f32 span;
    f32 distance;

    for (; actor != 0; actor = actor->next) {
        s32 flags = actor->flags;
        if (!(flags & 1)) {
            continue;
        }
        if (flags & 0x200) {
            break;
        }
    }
    func_001DF358((s32)unit, (s32)out);
    btlCopyMotionTransform((u8 *)pose, out);
    span = func_001F66D8(0x200, 0, 0) * 0.5f;
    pose[0] -= span;
    target[0] += span;
    target[8] *= 0.8f;
    btlUnitGetMuzzlePosVU(actor);
    VU0_SET_VF10_COMPONENT(y, -btlUnitGetTopY(actor));
    VU0_LOAD_VF(vf11, out);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    distance += (actor->cameraRadius * actor->scale * 2.0f) /
                func_002FA148(target[9] * 0.5f);
    if (target[8] < distance) {
        target[8] = distance;
    }
}

extern void btlPrepareUnitPoseWithTiltRotation();

void func_001E4708(void *unit, f32 *pose, u8 *out) {
    btlPrepareUnitPoseWithTiltRotation(unit, pose, out);
}

/* vu0 routine: frame the leftmost marked unit in two camera poses. */
void func_001E4720(CameraPoseTransform *source, CameraPoseTransform *from,
                   CameraPoseTransform *to) {
    f32 point[4];
    f32 center[4];
    f32 height;
    f32 fov;
    f32 minX;
    f32 length;
    BattleController *scene;
    BtlUnit *unit;
    BtlUnit *selected;
    s32 first;
    u32 flags;

    scene = (BattleController *)btlGetRuntime();
    fov = source->fov;
    from->fov = fov;
    to->fov = fov;
    func_001F66D8(0x400, &height, 0);
    VU0_STORE_VF(vf10, center);
    center[1] = -height;
    first = 1;
    selected = NULL;
    minX = 0.0f;
    for (unit = (BtlUnit *)scene->actors; unit != NULL; unit = unit->next) {
        flags = unit->flags;
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
    btlCopyMotionTransform((u8 *)to, (u8 *)from);
    func_002DD688(-(45.0f * 0.017453293f));
    VU0_LOAD_VF(vf10, to->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);
    to->distance = length + selected->cameraRadius * selected->scale * 3.0f /
                            func_002FA148(fov * 0.5f);
    func_001DB698(from);
    func_001DB698(to);
}

/* vu0 routine: frame the two unit groups using their bounding extents. */
void btlBuildGroupFramingCameraPose(CameraPoseTransform *source, CameraPoseTransform *out) {
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
    func_001DB698(out);
}

void btlResetUnitEffectVector(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4AC0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4E50);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E5198);

void func_001E5460(u32 arg0) {
    btlFlagUserAndTargetDefeat(arg0, arg0);
}


/* vu0 routine: interpolate pull-back and pitch for actor/target framing. */
void btlBuildHeightClampedApproachCamera(CameraPoseAction *action, CameraPoseTransform *out) {
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
    target = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
    extent = user->reach * user->scale;
    span = func_001D6050(user, user->unkEC);
    span /= func_001A47F0(action->link);
    ratio = (f32)action->state / span;
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    out->fov = action->transform.fov;
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
    func_001DB698(out);
}

void func_001E5700(u32 arg0) {
    func_001E5460(arg0);
}

void func_001E5718(CameraPoseAction *action, CameraPoseTransform *out) {
    btlBuildHeightClampedApproachCamera(action, out);
}

extern void func_001E0718(u8 *action, u8 *pose, u8 *out);

void btlChooseActionPoseBlendFromActorCount(u8 *action) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    u32 count;
    u32 mask;
    BtlUnit *unit;

    mask = ((BtlUnit *)btlGetIndexListEntry(*(struct BtlIndexList **)(action + 0x118), 0))->flags & 0x600;
    count = 0;
    for (unit = work->actorList; unit != 0; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & mask) {
                count++;
            }
        }
    }
    if (count >= 2) {
        func_001E0718(action, (u8 *)&((CameraPoseAction *)action)->fromPose,
                     (u8 *)&((CameraPoseAction *)action)->savedPose);
        return;
    }
    btlPrepareUnitPoseWithTiltRotation(action, (f32 *)&((CameraPoseAction *)action)->fromPose,
                                     (u8 *)&((CameraPoseAction *)action)->savedPose);
    *(f32 *)(action + 0x130) = 200.0f;
    *(u32 *)(action + 0xF0) |= 0x41;
}
void func_001E57F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4000);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4180);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4190);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4310);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4320);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A43E0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A43F0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4400);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4408);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4468);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E5800);

extern s32 D_0035DA28[];
extern s32 D_0035DAD0[];

void btlAdvancePlayerCursorAnimation(s32 action, s32 state) {
    if (!(*(u32 *)(*(s32 *)(*(s32 *)(action + 0xF4) + 0x18) + 0x110) & 0x400)) {
        func_001E9DE0(action, state, D_0035DA28[CURSOR->unk_0A]);
        func_001EB368(action, state);
        func_001EB1B0(action, (CameraPoseTransform *)state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}
void func_001E6180(CameraPoseAction *action) {
    CURSOR->frame = 0;
    if (!(action->link->unit->flags & 0x400)) {
        return;
    }

    memset(CURSOR, 0, 0x130);
    switch (action->link->cursorKindLow) {
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

extern s32 D_0035DA40[];

void func_001E6260(CameraPoseAction *action, s32 state) {
    if (!(action->link->unit->flags & 0x400)) {
        func_001E9DE0((s32)action, state, D_0035DA40[CURSOR->unk_0C]);
        func_001EB368((s32)action, state);
        func_001EB1B0((s32)action, (CameraPoseTransform *)state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 :
            CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        switch (action->link->cursorKind) {
        case 0x1B3:
        case 0x1B7:
        case 0x1BB:
        case 0x1BF:
        case 0x1C3:
            func_001E9DE0((s32)action, state, D_0035DA40[CURSOR->unk_0C]);
            func_001EB368((s32)action, state);
            func_001EB1B0((s32)action, (CameraPoseTransform *)state, 0, 0);
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
extern void func_001E6BB0(s32, s32, s32, s32);
extern void func_001E6668(s32, s32, s32, s32);
extern s16 D_0035D810[];
extern s16 D_0035D7F0[];

typedef struct BtlCursorChoices {
    u16 values[3][3][8];
} BtlCursorChoices;

extern const BtlCursorChoices D_003A4668;

void func_001E6368(CameraPoseAction *action, s32 state) {
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

    CURSOR->category = action->link->unit->lookupId;
    random = btlNextScaledRandom(8);
    CURSOR->index = choices.values[markedCount][CURSOR->category - 3][random];
    func_001E6BB0((s32)action, state, 0, D_0035D810[CURSOR->index]);
    func_001E6668((s32)action, state, 0, D_0035D7F0[CURSOR->index]);
}


void btlAdvanceCommandCursor(s32 arg0, s32 arg1) {
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E6668);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E6AC8);

void btlUnitGetPosVU(u32 unit, u8 mode) {
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

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4668);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A46F8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E6BB0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E9DE0);

/* vu0 routine: constrain a camera pose endpoint to the enabled height planes. */
s32 func_001EB1B0(s32 action, CameraPoseTransform *pose, s8 bypassUpper, s8 bypassLower) {
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EB368);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EBE88);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001ECCA8);

s32 btlFindActiveActorById(s32 id) {
    s32 node;

    for (node = *(s32 *)(btlGetRuntime() + 0x228); node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);

        if (flags & 1) {
            if ((flags & 0xC0) == 0) {
                if (flags & 0x200) {
                    if (*(u8 *)(node + 0x11C) == id) {
                        return node;
                    }
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001ED5C8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EDB20);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EE160);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EE658);

u32 btlCountUnitsByFlags(u32 mask) {
    s32 context = btlGetRuntime();
    u8 *actor = *(u8 **)(context + 0x228);
    u32 count = 0;
    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if ((flags & 1) != 0 && (flags & mask) != 0 && (flags & 0x20) == 0) {
            count++;
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EEAE0);

extern s32 D_0035DAE0[];

void btlAdvanceCursorForUnmarkedUnit(s32 action, s32 state) {
    BtlCameraCursor *cursor = &D_0035F100;

    if (cursor->unk_00 == 1) {
        if (!(((BtlAction *)action)->link->unit->flags & 0x400)) {
            func_001E9DE0(action, state, D_0035DAE0[cursor->unk_0C]);
            func_001EB368(action, state);
            func_001EB1B0(action, (CameraPoseTransform *)state, 0, 1);
            cursor->frame++;
            cursor->frame = cursor->frame <= 0 ? 0 :
                cursor->frame >= 0x7FFF ? 0x7FFE : cursor->frame;
        }
    }
}

void btlClearCommandCursorAndRunAction(s32 actor) {
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    btlFlagUserAndTargetDefeat(actor, actor);
}

extern s32 D_0035DAF0[];
extern s32 D_0035DAF8[];

void btlAdvanceCommandCursorOrAction(s32 action, s32 state) {
    if (CURSOR->unk_00 == 1) {
        if (((BtlCursorAction *)action)->link->unit->flags & 0x400) {
            return;
        }
        func_001E9DE0(action, state, D_0035DAF0[CURSOR->unk_0C]);
        func_001EB368(action, state);
        func_001EB1B0(action, (CameraPoseTransform *)state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera((CameraPoseAction *)action, &((CameraPoseAction *)action)->transform);
    }
}
void btlInitCommandCursorForCategory(s32 action, s32 state) {
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    switch (((CameraPoseAction *)action)->link->unit->partyRecord.unitId) {
    case 1:
        func_001E6BB0(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 2:
        return;
    case 3:
        func_001E6BB0(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 4:
        func_001E6BB0(action, state, 4, 0);
        CURSOR->unk_0C = 0;
        func_001E6668(action, state, 4, 1);
        CURSOR->unk_00 = 1;
        break;
    case 5:
        func_001E6BB0(action, state, 4, 0);
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
void func_001EEED8(s32 action, s32 state) {
    if (CURSOR->unk_00 == 1) {
        if (((BtlCursorAction *)action)->link->unit->flags & 0x400) {
            return;
        }
        func_001E9DE0(action, state, D_0035DAF8[CURSOR->unk_0C]);
        func_001EB368(action, state);
        func_001EB1B0(action, (CameraPoseTransform *)state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera((CameraPoseAction *)action, &((CameraPoseAction *)action)->transform);
    }
}

void btlInitCommandCursorForFirstActor(s32 arg0, s32 arg1) {
    BattleController *work = (BattleController *)btlGetRuntime();
    u8 *first = btlGetIndexListEntry(*(struct BtlIndexList **)(arg0 + 0x118), 0);
    memset(CURSOR, 0, 0x130);
    btlRefreshUnitEffectMotionAndEntry(first);
    if (btlHasFirstLinkedCategoryFlag1000((u8 *)arg0) != 0) {
        func_001E6BB0(arg0, arg1, 5, 1);
    } else {
        func_001E6BB0(arg0, arg1, 5, 0);
    }
    if (work->mode == 0x10E) {
        CURSOR->unk_0C = 3;
    } else {
        CURSOR->unk_0C = 0;
    }
    func_001E6668(arg0, arg1, 0, 0x11);
}

void btlAdvanceTargetCursorAnimation(s32 action, s32 state) {
    if (!(*(u32 *)(*(s32 *)(*(s32 *)(action + 0xF4) + 0x18) + 0x110) & 0x400)) {
        func_001E9DE0(action, state, D_0035DAD0[CURSOR->unk_0C]);
        func_001EB368(action, state);
        func_001EB1B0(action, (CameraPoseTransform *)state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}
extern void btlRefreshUnitEffectMotionAndEntry(u8 *);

extern void func_001E6BB0(s32, s32, s32, s32);

extern void func_001E6668(s32, s32, s32, s32);

void btlInitLinkedUnitActionCursor(u8 *arg0) {
    s32 context = btlGetRuntime() + 0x70;
    *(u8 **)(context + 0xF4) = arg0;
    memset(CURSOR, 0, 0x130);
    CURSOR->unk_0A = 0;
    CURSOR->unk_0E = 0;
    btlRefreshUnitEffectMotionAndEntry(*(s32 *)(arg0 + 0x18));
    func_001E6BB0(context, context, 6, 0);
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(*(BtlUnit **)(arg0 + 0x18));
    CURSOR->unk_0C = 0;
    func_001E6668(context, context, 0, 0);
}

/* Initialize the target cursor and orient the linked unit toward its target. */
void btlInitTargetCursorAndFacing(CameraPoseAction *action, void *state) {
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
    func_001E6BB0((s32)action, (s32)state, 2, 3);
    func_001E6668((s32)action, (s32)state, 0, 0);
    func_001EB1B0((s32)action, (CameraPoseTransform *)state, 0, 1);
    btlFlagMatchingUnitsDefeatCandidate(0x600);
    unit = action->link->unit;
    if (unit->flags & 0x80000) {
        btlUnitGetPosVU((u32)unit, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlCopyUnitRotationQuaternion((u8 *)unit, (s128 *)quaternion);
        VU0_LOAD_VF(vf10, quaternion);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, offset);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_SCALAR_OP(1.0f, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, position);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, aimPosition);
        target = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
        if (target->flags & 0x400) {
            func_001F66D8(0x400, 0, 0);
        } else {
            func_001F66D8(0x200, 0, 0);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlAimHorizontalDirectionVU((s128 *)aimPosition, (s128 *)position);
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation((u8 *)unit, (s128 *)rotation);
    }
}

extern void func_001E6BB0(s32, s32, s32, s32);

extern void func_001E6668(s32, s32, s32, s32);

extern s32 func_001EB1B0(s32, CameraPoseTransform *, s8, s8);

void btlInitCursorAndApplyAction(s32 actor, s32 target) {
    memset(&D_0035F100, 0, sizeof(D_0035F100));
    func_001E6BB0(actor, target, 2, 6);
    func_001E6668(actor, target, 0, 0);
    func_001EB1B0(actor, (CameraPoseTransform *)target, 0, 1);
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
        sdfReleaseChipOrRetainedResource(pointer);
        *(u32 *)(context + 0x580) = 0;
    }
    if (*(u32 *)(context + 0x57C) != 0) {
        sdfReleaseChipOrRetainedResource(*(u32 *)(context + 0x57C));
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

extern void sdfQueueNonzeroResourceId(s32);


void btlFreeFieldBlocks(void) {
    BattleController *context = (BattleController *)btlGetRuntime();
    btlWaitForPendingWorkAndReleaseBuffers();
    if (context->fieldF3 != 0) {
        sdfQueueNonzeroResourceId(context->fieldF3);
        context->fieldF3 = 0;
        btlBossDebugPrintf(D_003A4AD8);
    }
    if (context->fieldF2 != 0) {
        sdfQueueNonzeroResourceId(context->fieldF2);
        context->fieldF2 = 0;
        btlBossDebugPrintf(D_003A4AF0);
    }
    if (context->fieldF1 != 0) {
        sdfQueueNonzeroResourceId(context->fieldF1);
        context->fieldF1 = 0;
        btlBossDebugPrintf(D_003A4B08);
    }
    context = (BattleController *)btlGetRuntime();
    context->flags &= ~2;
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

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4AD8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4AF0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B08);

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
extern s32 fileQueuePlainDispatchRequest(const char *);
extern u32 fileRequestIsReady(u32);
extern void func_00288788(s32);
extern s32 evtCreateWorldObjectFromResource(s32, s32, s32, s32, s32, s32);

extern char D_003A4B50[]; /* "%sf%03d_%03d.LB" */
extern char D_003A4B60[]; /* "btl:field load[%s]\n" */
extern char D_003A4B78[]; /* "btl:field load end[f%03d_%03d]\n" */

u32 btlPollFieldArchiveLoad(BtlFieldLoadArgs *args) {
    BattleController *blocks = (BattleController *)btlGetRuntime();
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
        if (args->request != NULL && fileRequestIsReady((u32)args->request)) {
            BtlFieldArchiveRequest *request = args->request;
            node = request->resources;
            i = 0;
            while (node != NULL) {
                switch (i) {
                case 0:
                    blocks->fieldF3 = node->handle;
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
            func_00288788((s32)request);
            args->request = NULL;
        }
        if (args->fieldF1 != NULL && args->fieldF2 != NULL && args->fieldTB != NULL) {
            evtCreateWorldObjectFromResource(args->stage, args->variant,
                                             (s32)args->fieldF1, (s32)args->fieldF2,
                                             (s32)args->fieldTB, 0);
            btlInitializeSceneLightingAndTint();
            if (blocks->fieldF3 != 0) {
                sdfQueueNonzeroResourceId(blocks->fieldF3);
                blocks->fieldF3 = 0;
                btlBossDebugPrintf(D_003A4AD8);
            }
            blocks->flags |= 2;
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

    task[0] = 1;
    *(u16 *)(task + 0x24) &= ~1;
    *(u16 *)(task + 0x20) = 1;
    *(void **)(task + 0x4C) = btlPollFieldArchiveLoad;
    task[0x10] = 0;
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
        args->frontHandle = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A4BB8, path);
        func_003014F0(path, D_003A4BC8, stage, stage, variant);
        args->sideHandle = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A4BE8, path);
    } else {
        if (args->frontHandle != 0) {
            if (fileIsRequestReadyInCurrentMode(args->frontHandle) != 0) {
                work->primaryBuffer =
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
                work->secondaryBuffer =
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

u8 *btlCreateFloorLoadTask(u32 soundId, u32 variant) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x24) &= ~1;
    *(u16 *)(task + 0x20) = 2;
    *(void **)(task + 0x4C) = btlPollFloorLoadTask;
    task[0x10] = 0;
    arguments = btlGetTaskArguments(task);
    arguments[2] = soundId;
    arguments[3] = variant;
    arguments[0] = 0;
    arguments[1] = 0;
    arguments[4] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F0430);

extern u32 func_001F0430(u32 *);

void *btlCreateEffectTaskWithSourceParams(u8 *source, u32 value) {
    u8 *task = btlAllocTask(0x34);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 3;
    *(u16 *)(task + 0x24) |= 2;
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

s32 func_001F06E0(SceneLightRestoreArgs *args) {
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
            if (!(unit->flags & 2)) {
                continue;
            }
            if (!(unit->stateFlags & 0x10)) {
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
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 4;
    task->flags |= 2;
    task->callback = func_001F06E0;
    task->onStart = 0;
    arguments = btlGetTaskArguments(task);
    arguments->value = value;
    return task;
}

s64 func_001F0998(SceneLightRestoreArgs *arguments) {
    D_003BB694 = 1;
    return func_001F06E0(arguments);
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
    task->startCondition.kind = 1;
    task->taskId = 5;
    task->callback.acquireSound = btlQueueTintTransitionWhenEnabled;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->taskId = 6;
    task->callback.releaseSound = sndTickFadeCounter;
    task->endCondition.kind = 0;
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

void btlExtendTaskFrameLimit(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0xc) < arg1) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

u32 sndGetResourceStatus(u32 *sound) {
    u32 flags;
    if (!sound[1]) {
        return 0;
    }
    flags = sound[0];
    if (flags & 1) {
        return 0xFFFFFFF;
    }
    if (flags & 2) {
        return sound[3];
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F0CA0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B40);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B50);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B60);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B78);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B98);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BB8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BC8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BE8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BF8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4C10);

void sndCreateSystemEffect(SoundEffectNode *effect) {
    SoundMixer *handle;
    if (!(effect->flags & 8) || effect->handle || effect->referenceCount) {
        return;
    }
    handle = sndMixerClone(effect->source);
    effect->handle = handle;
    btlBossDebugPrintf("btl:system effect create[%p]\n", handle);
}

void sndDeleteSystemEffect(SoundEffectNode *effect) {
    if ((effect->flags & 8) && effect->handle && !effect->referenceCount) {
        btlBossDebugPrintf("btl:system effect delete[%p]\n", effect->handle);
        sndReleaseAllVoices(effect->handle);
        effect->handle = 0;
    }
}

void sndAddEffectReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    u32 *target;

    task[2] = 0;
    sndCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    target = (u32 *)task[6];
    source = (u32 *)task[3];
    ++effect[1];
    ++source[0x314 / 4];
    ++target[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F1110);

extern u32 func_001F1110(u32 *);

void sndReleaseEffectReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    u32 *target;

    if (task[2]) {
        effReleaseBattleVoiceOwner(task[2]);
    }
    effect = (u32 *)task[0];
    target = (u32 *)task[6];
    source = (u32 *)task[3];
    --effect[1];
    --source[0x314 / 4];
    --target[0x314 / 4];
    sndDeleteSystemEffect(effect);
}

s32 func_001F12E8(u32 effect, u32 soundId, u8 *owner, u16 variant) {
    u8 *task = btlAllocTask(32);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2B;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = sndAddEffectReferences;
    *(void **)(task + 0x4C) = func_001F1110;
    *(void **)(task + 0x50) = sndReleaseEffectReferences;
    arguments = btlGetTaskArguments(task);
    *(u32 *)arguments = effect;
    *(u32 *)(arguments + 0xC) = soundId;
    *(u32 *)(arguments + 0x10) = soundId;
    *(u32 *)(arguments + 0x14) = soundId;
    *(u8 **)(arguments + 0x18) = owner;
    *(u16 *)(arguments + 4) = variant;
    *(u32 *)(arguments + 8) = 0;
    *(u32 *)(arguments + 0x1C) = 0;
    return (s32)task;
}

SoundTask *sndCreateEffectWithTargets(s32 sound, s32 flags, u32 *source, u32 *target, s32 mode, u16 variant) {
    SoundTask *task = (SoundTask *)func_001F12E8(sound, flags, mode, variant);
    u32 *data = btlGetTaskArguments(task);
    data[4] = (u32)source;
    data[5] = (u32)target;
    return task;
}

/* Retail 001F1590/001F15BC dereference the unit; 001F1588 passes the same
 * encoded key to the integer selector provider (00161838), which stores it. */
typedef union ActorEffectOwner {
    BtlUnit *unit;
    s32 selectorKey;
} ActorEffectOwner;

typedef struct ActorEffectTaskArgs {
    SoundResourceNode *source;
    SoundVoice *effect;
    ActorEffectOwner owner;
    u32 duration;
    s32 counter;
} ActorEffectTaskArgs;

void sndStartEffectTask(ActorEffectTaskArgs *args) {
    SoundResourceNode *source;
    BtlUnit *unit;

    args->effect = 0;
    sndCreateSystemEffect((u32 *)args->source);
    source = args->source;
    unit = args->owner.unit;
    source->unk_04++;
    unit->unk314++;
}

extern u32 effBattleGetCurrentFrame(SoundVoice *effect);
extern void effBTLFieldColorSetSelectors(s32, u32, s32, s32);

s32 func_001F1470(ActorEffectTaskArgs *args) {
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
    if (unit->flags & 4) {
        args->effect->flags |= 8;
    } else {
        args->effect->flags &= ~8;
    }
    if (unit->flags & 2) {
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
    source->unk_04--;
    unit->unk314--;
    sndDeleteSystemEffect((u32 *)source);
}

BtlRuntimeTask *sndCreateActorEffectTask(SoundResourceNode *source, BtlUnit *owner, u32 duration) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(ActorEffectTaskArgs));
    ActorEffectTaskArgs *args;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x2C;
    task->flags |= 2;
    task->ownerId = owner->identity;
    task->onStart = sndStartEffectTask;
    task->callback = func_001F1470;
    task->onFinish = sndFinishActorEffectTask;
    args = btlGetTaskArguments(task);
    args->source = source;
    args->owner.unit = owner;
    args->duration = duration;
    args->effect = 0;
    args->counter = 0;
    return task;
}

void sndIncrementEffectActiveCount(s32 *arg0) {
    *(s32 *)(*arg0 + 8) = *(s32 *)(*arg0 + 8) + 1;
}

extern s32 sndGetEffectNodeParameter(s32, u16);

u32 sndWaitEffectFramesAndApplyUnitParameter(u32 *args) {
    u32 *effect = (u32 *)args[0];
    s32 frames;

    if ((effect[0] & 2) == 0) {
        return 0;
    }
    frames = sndGetEffectNodeParameter((s32)effect, (u16)args[1]);
    if ((s32)args[5] >= frames) {
        *(s32 *)(args[0] + 8) -= 1;
        if ((s32)args[3] >= 0) {
            btlApplyScaledUnitEffectParameter((u8 *)args[2], args[3], args[4], 1.0f);
        }
        return 1;
    }
    args[5]++;
    return 0;
}

void *sndCreateTimedUnitEffectTask(u32 effect, u8 *actor, u16 frames, u32 channel, u32 volume) {
    u8 *task = btlAllocTask(24);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2E;
    *(u64 *)(task + 0x40) = *(u64 *)(actor + 0x108);
    *(void **)(task + 0x48) = sndIncrementEffectActiveCount;
    *(void **)(task + 0x4C) = sndWaitEffectFramesAndApplyUnitParameter;
    *(u32 *)(task + 0x50) = 0;
    arguments = btlGetTaskArguments(task);
    arguments[0] = effect;
    arguments[2] = (u32)actor;
    *(u16 *)(arguments + 1) = frames;
    arguments[3] = channel;
    arguments[4] = volume;
    arguments[5] = 0;
    return task;
}

void sndBeginEffectLoad(EffectLoadArgs *args) {
    SoundEffectNode *effect = args->effect;
    if (effect->flags & 2) {
        if (effect->handle != 0) {
            sndReleaseAllVoices(effect->handle);
            effect->handle = 0;
        }
        effect->flags &= ~2;
    }
    args->loadHandle = fileQueueDefaultCallbackRequest(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

extern char D_003A4C88[];

u32 sndPollEffectLoad(u32 *args) {
    SoundEffectNode *effect = (SoundEffectNode *)args[0];
    s32 resource;

    if (effect->flags & 2) {
        return 1;
    }
    if (!fileIsRequestReadyInCurrentMode(args[1])) {
        return 0;
    }
    btlBossDebugPrintf(D_003A4C88, args[2]);
    resource = fileGetResourceHandle(args[1]);
    effect->handle =
        sndMixerClone(sdfResourceRetainAddress(resource));
    sdfReleaseResourceAllocation(resource);
    filePollEntryCleanup(args[1]);
    effect->flags = (effect->flags & ~1) | 2;
    return 0;
}

extern void sndBeginEffectLoad(EffectLoadArgs *);

extern u32 sndPollEffectLoad(u32 *);

u8 *sndCreateEffectLoadTask(u32 soundId, const char *filename) {
    u8 *task = btlAllocTask(strlen(filename) + 12);
    u8 *arguments;
    char *name;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x2F;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x48) = sndBeginEffectLoad;
    *(void **)(task + 0x4C) = sndPollEffectLoad;
    task[0x10] = 0;
    arguments = btlGetTaskArguments(task);
    name = (char *)(arguments + 12);
    *(u32 *)arguments = soundId;
    *(char **)(arguments + 8) = name;
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
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x30;
    *(void **)(task + 0x4C) = btlWaitUnitListIdle;
    task[0x10] = 0;
    *(u32 *)btlGetTaskArguments(task) = sound;
    return task;
}

u32 sndApplyToActiveActors(u32 *soundId) {
    u8 *object = *(u8 **)(btlGetRuntime() + 0x228);
    while (object != 0) {
        u32 flags = *(u32 *)(object + 0x110);
        if (flags & 1) {
            if (flags & 2) {
                if (*(u32 *)(object + 0x320) != 0 &&
                    (flags & 0xE0) == 0) {
                    btlBlendUnitColor(object, *(u32 *)(object + 0x54), *soundId);
                }
            }
        }
        object = *(u8 **)(object + 0x344);
    }
    return 1;
}

void *btlCreateApplyToActiveActorsTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x31;
    *(void **)(task + 0x4C) = sndApplyToActiveActors;
    task[0x10] = 0;
    *(u32 *)btlGetTaskArguments(task) = sound;
    return task;
}

u32 btlCancelTimedFadeTask(void) {
    kwlnCancelConfiguredFadeFrames();
    return 1;
}

SoundTask *btlCreateFadeStateResetTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback.process = btlCancelTimedFadeTask;
    task->taskId = 0x32;
    task->endCondition.kind = 0;
    return task;
}

void sndAddSourceReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    task[1] = 0;
    sndCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    source = (u32 *)task[2];
    ++effect[1];
    ++source[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F1CC8);

extern u32 func_001F1CC8(u32 *);

void sndFinishEffectSourceTask(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        effReleaseBattleVoiceOwner(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x314) = *(s32 *)(temp_v1 + 0x314) - 1;
    sndDeleteSystemEffect(temp_v0);
}

u8 *sndCreateEffectSourceTask(u32 effect, u8 *owner, u64 resource) {
    u8 *task = btlAllocTask(32);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2D;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = sndAddSourceReferences;
    *(void **)(task + 0x4C) = func_001F1CC8;
    *(void **)(task + 0x50) = sndFinishEffectSourceTask;
    arguments = btlGetTaskArguments(task);
    *(u32 *)arguments = effect;
    *(u8 **)(arguments + 8) = owner;
    *(u64 *)(arguments + 0x10) = resource;
    *(u32 *)(arguments + 4) = 0;
    *(u32 *)(arguments + 0x18) = 0;
    *(u32 *)(arguments + 0x1C) = 0;
    return task;
}

u32 btlTaskStartFadeIn(u32 *arg0) {
    kwlnFadeStartIn(*arg0);
    return 1;
}

void *btlCreateFadeInTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x35;
    *(void **)(task + 0x4C) = btlTaskStartFadeIn;
    task[0x10] = 0;
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
    task->startCondition.kind = 1;
    task->taskId = 0x36;
    task->callback.playCustomSound = btlTaskStartCustomFadeIn;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->callback.process = btlTaskSetBattleFlag40000;
    task->taskId = 0x37;
    task->onStart = 0;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->callback.process = btlTaskClearBattleFlag40000;
    task->taskId = 0x38;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4C88);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F2218);

void sndSetEffectNodeParameter(s32 arg0, u16 arg1) {
    sndReadSelectedMixerBankValue(*(u32 *)(arg0 + 0x10), arg1);
}

s32 sndGetEffectNodeParameter(s32 arg0, u16 arg1) {
    return func_00160858(*(u32 *)(arg0 + 0x10), arg1);
}

s32 sndIsResourceNodeReferencedOrActive(s32 arg0) {
    if (*(s32 *)(arg0 + 4) != 0) {
        return 1;
    }
    return *(u32 *)(arg0 + 8) != 0;
}

s32 sndHasActiveActor(void) {
    s32 actor = *(s32 *)(btlGetRuntime() + 0x228);
    while (actor != 0) {
        s32 sound = *(s32 *)(actor + 0x2F8);
        if (sound != 0 && sndIsResourceNodeReferencedOrActive(sound) != 0) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

/* Allocate a cleared resource node and prepend it to the runtime's resource list. */
SoundResourceNode *sndAllocResourceNode(void) {
    SoundResourceNode *node = sdfAllocAndClearQuadwords(sizeof(SoundResourceNode));
    BtlActorWork *state;
    SoundResourceNode *first;
    node->unk_04 = 0;
    node->unk_08 = 0;
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
        if (node->unk_04 == 0) {
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

SoundResourceLink *sndAllocResourceLink(void *owner) {
    SoundResourceLink *node = sdfAllocAndClearQuadwords(sizeof(SoundResourceLink));
    node->owner = owner;
    node->sound = 0;
    node->variant = 0;
    node->task = 0;
    return node;
}

void sndFreeResourceLink(SoundResourceLink *node) {
    if (node->sound) {
        effReleaseBattleVoiceOwner(node->sound);
        --node->task[1];
        sndDeleteSystemEffect(node->task);
    }
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F2818);

void btlMarkTaskReady(SoundResourceLink *link) {
    link->refreshRequested = 1;
}

SoundLink *sndAllocLink(void *owner) {
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
        node->effect[1]--;
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
        effectId = ((BtlCategoryTableEntry *)datCommandRecords)[actor->selectedEntryIndex].unk2E;
    } else {
        effectId = 0;
    }
    if (effectId != 0 && !(actor->partyRecord.status & 0x4000)) {
        if (link->effectHandle == 0) {
            link->effect = (u32 *)work->soundResourceSlots[BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT];
            sndCreateSystemEffect(link->effect);
            link->effectHandle = func_00160958(link->effect[4], 2, actor, 0);
            link->effect[1]++;
            link->effectHandle->flags = (link->effectHandle->flags | 1) & ~6;
        }
        link->variant = effectId;
    } else if (link->effectHandle != 0) {
        effReleaseBattleVoiceOwner(link->effectHandle);
        link->effect[1]--;
        sndDeleteSystemEffect(link->effect);
        link->effectHandle = 0;
        link->effect = 0;
    }
    if (link->effectHandle != 0 && (actor->flags & 4) && (work->fadeColor & 0xFF000000)) {
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
    task->startCondition.kind = 1;
    task->callback.process = btlDisableBattleFade;
    task->taskId = 0x33;
    task->onStart = 0;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->callback.process = btlEnableBattleFade;
    task->taskId = 0x34;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

extern s32 fileQueuePlainDispatchRequest(const char *path);

extern void func_00288C50(s32 archive);

extern void func_00288788(s32 archive);

extern char D_003A5008[];
extern char D_003A5020[];
/* Consume archive records only for enabled SYSEFF rows; clear unavailable entries. */
void sndLoadSysEffLb(void) {
    const char *path = D_003A5008;
    s32 archive = fileQueuePlainDispatchRequest(path);
    s32 node;
    u32 i;

    func_00288C50(archive);
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
    func_00288788(archive);
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
    u32 handle;
    void *actor;
} SoundHandleNode;

struct FileQueue;
extern struct FileQueue *fileCloneQueueEntries(struct FileQueue *);

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(8);
    SoundBankEntry *entry = &D_0035F748[index];
    node->actor = actor;
    node->handle = (u32)fileCloneQueueEntries((struct FileQueue *)entry->resource);
    return node;
}

void btlUpdateJobPositionFromModel(s32 *args) {
    f32 pos[4];

    if (sdfLoadMapRecordPositionVector((SdfTextParam *)((MdlCtx *)args[1])->inner, 1) == 0) {
        mdlLoadPrimaryVectorVU((MdlCtx *)args[1]);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        pos[1] -= 150.0f;
    } else {
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
    }
    fileQueueSetPosition(args[0], pos);
    fileQueueUpdate(args[0]);
}

void sndDestroyFileQueueWrapper(u32 arg0) {
    fileQueueDestroy(*(u32 *)arg0);
    sdfReleaseChipBlock(arg0);
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

void func_001F3278(s32 adjustmentIndex, s32 sceneIndex) {
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

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4CD8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4CF0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4D08);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4D20);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4D38);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4D50);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4D68);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4D80);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4D98);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4DB0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4DC8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4DE0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4DF8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4E10);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4E28);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4E40);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4E58);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4E70);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4E88);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4EA0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4EC0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4EE0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4F00);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4F18);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4F30);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4F48);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4F60);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4F78);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4F90);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4FA8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4FC0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4FD8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4FF0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5008);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5020);

s32 sndPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (sndLoadAndPlayStationedSe(soundId)) {
        btlBossDebugPrintf("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

SoundTask *sndCreateStationedSeTask(u32 soundId) {
    SoundTask *task = (SoundTask *)btlAllocTask(4);
    task->startCondition.kind = 1;
    task->taskId = 0x55;
    task->endCondition.kind = 0;
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

    task->startCondition.kind = 1;
    task->taskId = 0x52;
    task->endCondition.kind = 0;
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
        if (fileIsRequestReadyInCurrentMode(request->handle)) {
            s32 size;
            s32 data;
            btlBossDebugPrintf(D_003A50F0, request->name);
            request->resourceHandle = fileGetResourceHandle(request->handle);
            size = fileGetResourceSize(request->handle);
            data = sdfResourceRetainAddress(request->resourceHandle);
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
        sdfReleaseResourceAllocation(request->resourceHandle);
        filePollEntryCleanup(request->handle);
        node->flags = (node->flags & ~8) | 0x10;
        return 1;
    }
    return 0;
}

u8 *sndCreateFileLoadTask(SoundLoadNode *node, u32 variant, const char *filename) {
    u8 *task = btlAllocTask(strlen(filename) + 20);
    SoundFileRequest *request;
    char *name;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x53;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x48) = sndStartFileLoad;
    *(void **)(task + 0x4C) = sndPollMotSeFileAndSpu;
    task[0x10] = 0;
    request = btlGetTaskArguments(task);
    name = (char *)(request + 1);
    request->node = node;
    request->blockIndex = variant;
    request->name = name;
    strcpy(name, filename);
    return task;
}

s32 sndLoadDataFile(s32 *data) {
    char filename[0x70];
    if (sndIsCommandBusySigned()) {
        return 1;
    }
    sndFormatResourceNameFromUnitMode(data[0], (s32)filename);
    sdfSoundSendNamedCommand(filename, 0x34);
    return 1;
}

void *sndCreateDataFileLoadTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = sndLoadDataFile;
    *(u16 *)(task + 0x20) = 0x56;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments(task) = (u32)owner;
    return task;
}

s32 sndIsCommandBusySigned(void) {
    return (s8)sdfSoundIsCommandBusy();
}

s32 sndHasResourceFlagsOneOrEight(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    if ((temp_v0 & 1) != 0) {
        return 1;
    }
    return (temp_v0 & 8) > 0;
}

void sndFormatResourceNameFromIndex(s32 arg0, s32 arg1) {
    func_003014F0(arg1, D_003A5158, D_003BB6B0, (arg0 + 0x200) & 0xffff);
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A50D8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A50F0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5110);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5138);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5158);

void sndFormatResourceNameFromUnitMode(s32 arg0, s32 arg1) {
    func_003014F0(arg1, "MDD_%03X.ADB", *(u16 *)(arg0 + 0x124));
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

/* Retain and per-slot loading state, embedded after the category/id key. */
typedef struct SoundSlotWork {
    u32 refCount; /* Shared retain count; release frees only on the zero transition. */
    s32 pendingSoundId; /* Packed-track key consumed by the load-status poll. */
    s32 pendingSlot;    /* Index into resourceHandles for the pending track. */
    s32 fileRequests[0x1D];
    s32 resourceHandles[0x1D];
} SoundSlotWork;

/* Shared motion-SE owner: queued files become resource handles before playback. */
typedef struct SoundSlotOwner {
    u32 flags; /* 1 files queued, 2 files ready; 4 track pending, 8 loading, 0x10 ready. */
    s32 category;
    s32 id;
    SoundSlotWork work;
    struct SoundSlotOwner *prev;
    struct SoundSlotOwner *next;
} SoundSlotOwner;

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
            *(u32 *)(handleTable + offset) = fileQueueDefaultCallbackRequest(filename);
            btlBossDebugPrintf(D_003A51A8, slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound[0] |= 1;
}

/* Find the shared category/id owner, returning its address or zero. */
s32 sndFindListNodeForChannel(s32 category, s32 id) {
    s32 context = btlGetRuntime();
    SoundSlotOwner *node = ((BtlActorWork *)context)->soundSlotOwners;
    while (node != 0) {
        if (node->category == category && node->id == id) {
            return (s32)node;
        }
        node = node->next;
    }
    return 0;
}

extern char D_003A51D0[];

/* Retain or register an owner; model flag 0xC0F suppresses initial file queuing. */
SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id) {
    SoundSlotOwner *node = (SoundSlotOwner *)sndFindListNodeForChannel(category, id);
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
void sndReleaseSlotOwner(u8 *ownerAddress) {
    SoundSlotOwner *node = (SoundSlotOwner *)ownerAddress;
    u32 count = node->work.refCount - 1;
    node->work.refCount = count;
    if (count == 0) {
        u32 i = 0;
        u32 *resources = (u32 *)node->work.resourceHandles;
        u32 *requests = (u32 *)node->work.fileRequests;
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

        sndReleaseSlotOwner((u8 *)node);
        node = next;
    }
}

void btlStartMoveOtherUnitsTask(void) {
    s32 task = btlCreateMoveOtherUnitsTask();
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

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5178);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5188);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5198);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A51A8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A51D0);

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
            data = sdfMemoryGetBlockAddress(soundWork->resourceHandles[args->slot]);
            size = sdfMemoryGetBlockSize(soundWork->resourceHandles[args->slot]);
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

s32 btlCreateMoveOtherUnitsTask(u8 *owner, u32 soundId) {
    u8 *task = btlAllocTask(16);
    SoundTaskArgs *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x54;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlQueueUnitSoundSlotFileLoad;
    *(void **)(task + 0x4C) = sndPollMotionSePlayback;
    arguments = btlGetTaskArguments(task);
    arguments->actor = (BtlUnit *)owner;
    arguments->slot = soundId;
    arguments->option = 0;
    arguments->waitFrames = 0;
    return (s32)task;
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F44C0);

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
    task->startCondition.kind = 1;
    task->callback.process = sndFinishEarringPlayback;
    task->taskId = 0x57;
    task->endCondition.kind = 0;
    return task;
}

typedef struct BattleVoiceLoad {
    u32 request;
    s32 state;
    s32 index;
} BattleVoiceLoad;


extern u8 D_00377654[];

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
        args->request = fileQueueDefaultCallbackRequest(path);
        btlBossDebugPrintf(D_003A5340, path);
    } else if (fileIsRequestReadyInCurrentMode(args->request) != 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        resource = fileGetResourceHandle(args->request);
        data = (u32)sdfResourceRetainAddress(resource);
        size = fileGetResourceSize(args->request);
        filePollEntryCleanup(args->request);
        func_0026ABA8(data, size, D_00377650[args->index].volume);
        sdfReleaseResourceAllocation(resource);
        btlBossDebugPrintf(D_003A5358);
        return 1;
    }
    args->state++;
    return 0;
}

void *sndCreateAtracEffectLoadTask(u32 owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x58;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x4C) = sndPollAtrac3SELoadTask;
    task[0x10] = 0;
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
        if (unit->flags & 0x200) {
            id = unit->partyRecord.unitId;
            if (unit->flags & 0x1000) {
                id += 0x10;
            }
            func_003014F0(path, D_003A5178, D_003A5188, id);
        } else {
            func_003014F0(path, D_003A5198, D_003A5188, unit->partyRecord.unitId);
        }
        args->request = fileQueueDefaultCallbackRequest(path);
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
        if (fileIsRequestReadyInCurrentMode(args[1]) != 0) {
            resource = fileGetResourceHandle(args[1]);
            args[2] = resource;
            data = sdfResourceRetainAddress(resource);
            size = fileGetResourceSize(args[1]);
            filePollEntryCleanup(args[1]);
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
        sdfReleaseResourceAllocation(sound[2]);
    }
    --*(u16 *)(state + 0x260);
}

void *sndCreateEarringPlaybackTask(u8 *owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x59;
    *(u16 *)(task + 0x24) &= ~1;
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
    task->startCondition.kind = 1;
    task->callback.process = btlPlayStationedSe1C;
    task->taskId = 0x5A;
    task->endCondition.kind = 0;
    return task;
}

u32 btlAdvanceTitleStateAfterSound(void) {
    btlAdvanceTitleState();
    return 1;
}

SoundTask *btlCreateAdvanceTitleStateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback.process = btlAdvanceTitleStateAfterSound;
    task->taskId = 0x5B;
    task->endCondition.kind = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F4D50);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F5028);

extern void func_001F4D50(void *);

void btlRepositionPartyAroundBattleCenter(void) {
    f32 vector[4];
    u8 *context = (u8 *)btlGetRuntime();
    PCP_COPY_VECTOR(vector, context);
    func_001F4D50(vector);
}

s64 func_001F53F0(void) {
    return func_001F5028(0x400);
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern f32 sdfSinPoly(f32);

/* Place the three actor slots around the common battle center supplied in vf10. */
void btlPlaceTripleFormationAroundCenter(BattleActionLinkState *link, BtlUnit *first, BtlUnit *second) {
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
    btlSetUnitPosition((u8 *)slot[1], pos);
    if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation((u8 *)slot[1], (s128 *)rotation);
    }
    angle = 30.0f * 0.017453293f;
    pos[0] = center[0] - sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition((u8 *)slot[0], pos);
    if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation((u8 *)slot[0], (s128 *)rotation);
    }
    pos[0] = center[0] + sdfEvaluateCosineViaSinePhaseShift(angle) * radius;
    pos[1] = 0.0f;
    pos[2] = center[2] - sdfSinPoly(angle) * radius;
    btlSetUnitPosition((u8 *)slot[2], pos);
    if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)center) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation((u8 *)slot[2], (s128 *)rotation);
    }
}

extern void btlFlagAllUnitsDefeatCandidate(void);
extern void btlClearMatchingUnitDefeatCandidates(s32);
extern u8 D_0037E100[];

void btlPlaceTripleFormationAroundTarget(BattleActionLinkState *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 radius;
    BtlUnit *target;

    if (btlGetIndexListCount(link->actorIndices) == 1) {
        target = (BtlUnit *)btlGetIndexListEntry(link->actorIndices, 0);
        btlFlagAllUnitsDefeatCandidate();
        btlClearMatchingUnitDefeatCandidates(target->flags & 0x600);
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
        btlSetUnitPosition((u8 *)slot[1], pos);
        btlUnitFaceTarget((u8 *)slot[1], (u8 *)target);
        VU0_LOAD_VF(vf10, D_0037E100);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition((u8 *)slot[0], pos);
        btlUnitFaceTarget((u8 *)slot[0], (u8 *)target);
        VU0_LOAD_VF(vf10, D_0037E100);
        VU0_NEGATE_XYZ(vf10);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_SCALAR_OP(radius, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_LOAD_VF(vf11, center);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        btlSetUnitPosition((u8 *)slot[2], pos);
        btlUnitFaceTarget((u8 *)slot[2], (u8 *)target);
    }
}
/* vu0 routine: Place three indexed actors around the middle actor's facing and muzzle. */
void btlPlaceTripleFormationAroundMiddleActor(BattleActionLinkState *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 dir[4];
    f32 rotation[4];
    f32 radius;
    u32 i;

    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(link->unit->flags & 0x600);
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
        btlSetUnitPosition((u8 *)slot[i], pos);
        if (btlAimHorizontalDirectionVU(pos, center) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
            btlSetUnitRotation((u8 *)slot[i], rotation);
        }
    }
}

void btlOrientFrontAndBackUnitsTowardTargets(BattleActionLinkState *link, BtlUnit *a, BtlUnit *b) {
    BtlUnit *front = 0;
    BtlUnit *back = 0;
    BtlUnit *target;
    s128 vec[3];
    u32 count;

    if (link->unit->flags & 0x1000) {
        back = link->unit;
    } else {
        front = link->unit;
    }
    if (a != 0) {
        if (a->flags & 0x1000) {
            back = a;
        } else {
            front = a;
        }
    }
    if (b != 0) {
        if (b->flags & 0x1000) {
            back = b;
        } else {
            front = b;
        }
    }
    count = btlGetIndexListCount(link->actorIndices);
    target = (BtlUnit *)btlGetIndexListEntry(link->actorIndices, 0);
    if (count == 1) {
        btlUnitFaceTarget((u8 *)front, (u8 *)target);
    } else {
        btlUnitGetMuzzlePosVU(front);
        VU0_STORE_VF(vf10, &vec[0]);
        func_001F66D8(target->flags & 0x600, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU(&vec[0], &vec[1]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation((u8 *)front, &vec[2]);
        }
    }
    btlUnitFaceTarget((u8 *)back, (u8 *)front);
}
/* vu0 routine: measure the center actor's displacement from its sole target. */
void btlAlignTripleFormationWithTarget(BattleActionLinkState *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 position[4];
    f32 targetPosition[4];
    u8 *runtime;
    BtlUnit *target;
    f32 offsetX;
    f32 offsetZ;
    u32 i;

    runtime = (u8 *)btlGetRuntime();
    if (btlGetIndexListCount(link->actorIndices) == 1) {
        target = (BtlUnit *)btlGetIndexListEntry(link->actorIndices, 0);
        btlFlagAllUnitsDefeatCandidate();
        btlClearMatchingUnitDefeatCandidates(target->flags & 0x600);
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
            btlSetUnitPosition((u8 *)slot[i], position);
            btlUnitFaceTarget((u8 *)slot[i], (u8 *)target);
        }
    }
}

void func_001F5D00(BattleActionLinkState *link, BtlUnit *first, BtlUnit *second) {
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F5D08);

typedef struct BattleFormationActionArgs {
    BattleActionLinkState *link;
    BtlUnit *first;
    BtlUnit *second;
    u32 actionId;
    u32 unk10;
} BattleFormationActionArgs;

extern BattleFormationActionTable *D_003BAA64;

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5328);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5340);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5358);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5370);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5390);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A53B0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A53D0);

u32 func_001F5ED8(BattleFormationActionArgs *args) {
    BattleActionLinkState *link;
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
    if (link->unit->flags & 0x400) {
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

    task[0] = 1;
    task[0x10] = 0;
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

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5410);

