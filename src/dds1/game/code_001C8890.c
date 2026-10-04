#include "common.h"
#include "btl_task.h"
#include "dds3obj.h"
#include "pcp_vu0.h"
#include "fpu.h"

#define VU_LOAD10(p) __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p))

#define VU_STORE10(p) __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p))

typedef struct ActorEntrySlot {
    s16 code;
    s16 unk02;
    s16 countdown;
} ActorEntrySlot;

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u32 actionFlags;
    u8 unk_118[8];
    u16 entryMask;
    u8 entryDataTail[2];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
    u8 unk_130[6];
    u8 unk_136[0x18E];
    u8 kind;
    u8 pad_2C5;
    ActorEntrySlot entrySlots[7];
    s32 selectedEntryIndex;
    u32 marker;
    u8 pad_2F8[0x4C];
    struct UiObject *next;
} UiObject;

typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;

/* Actor-task prefix; the separate unit-data list uses UiObject. */
typedef struct SceneTask {
    s32 state;
    u8 pad04[4];
    u32 flags;
    u8 pad0C[0xC];
    UiObject *actor;
} SceneTask;

typedef struct BattleController {
    u8 pad_000[0x1F4];
    u32 flags;
    u8 pad_1F8[0x30];
    UiObject *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad_254[0x28];
    s32 mode;
    u8 pad_280[0x1C];
    s32 taskParent;
    u8 pad_2A0[0xC];
    s32 spriteObject;
    u8 pad_2B0[0x24];
    SceneSlot slots[8];
    SceneTask *groupPrimary[20];
    SceneTask *groupSecondary[45];
    SceneTask *groupTertiary[15];
    u8 pad_42C[0x184];
    s32 (*sceneCallback)();
} BattleController;

extern u8 D_0035F100[];

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

typedef struct SoundEffectNode {
    u32 flags;
    u8 pad4[0xC];
    u32 handle;
} SoundEffectNode;

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

typedef struct BtlUnit {
    u8 pad_00[0x50];
    f32 effectScale;
    u8 pad_54[0x1C];
    s128 orientation;
    f32 scale;
    u32 overlayColor;
    f32 positionZOffset;
    u8 pad_8C[4];
    s128 bodyOffset;
    u8 pad_A0[0x14];
    f32 reach;
    u8 pad_B8[4];
    f32 unkBC;
    f32 cameraRadius;
    s32 resourceKind;
    s32 resourceIndex;
    u8 unkCC;
    u8 pad_CD[0x1F];
    s32 unkEC;
    u8 pad_F0[0x18];
    s64 owner;
    u32 flags;
    u32 stateFlags;
    u32 gunResourceFlags;
    u8 lookupId;
    u8 pad_11D[3];
    u32 statBits;
    u8 pad_124[0x1A0];
    s8 unk2C4;
    u8 pad_2C5[0x2B];
    s32 unk2F0;
    u8 pad_2F4[4];
    s32 resourceNode;
    s32 resourceLink;
    s32 link;
    s32 listNode;
    u8 pad_308[4];
    void *gunResource;
    u8 pad_310[4];
    s32 unk314;
    u8 pad_318[4];
    s32 effectObject;
    s32 ext;
    s32 transparencyModel;
    u8 pad_328[4];
    s32 unk32C;
    s32 unk330;
    u8 pad_334[8];
    u32 handle;
    struct BtlUnit *previousActor;
    struct BtlUnit *nextActor;
} BtlUnit;

/* SYSEFF metadata and runtime registrations share these indices. */
enum {
    BTL_SOUND_ENTRY_COUNT = 0x31,
    BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26
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
    u8 pad240[0x64];
    s32 unk2A4;
    u8 pad2A8[0x210];
    struct SoundResourceNode *soundResourceSlots[BTL_SOUND_ENTRY_COUNT];
    u8 pad57C[8];
    u8 fadeEnabled; /* 0 raises the tint, 1 lowers it; refreshed by the frame updater. */
    u8 pad585[3];
    u32 fadeColor; /* Packed tint; retain the original whole-word arithmetic. */
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

typedef struct SoundResourceNode {
    u32 flags;
    u32 unk_04;
    u32 unk_08;
    s32 fadeCountdown;
    u32 resourceHandle; /* Owned clone; destruction releases its voices. */
    u32 sourceHandle;   /* Indexed nodes borrow this archive resource. */
    struct SoundResourceNode *previous;
    struct SoundResourceNode *next;
} SoundResourceNode;

typedef struct SoundLink {
    void *owner;
    void *sound;
    void *task;
    u16 variant;
    u16 unk_0E;
} SoundLink;

typedef struct SoundResourceLink {
    void *owner;
    void *sound;
    void *task;
    u32 variant;
    u32 unk_10;
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

extern s64 func_001F0998(void);

extern s32 func_001F06E0();

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

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 D_003BB694;

extern u32 btlTintTransitionHoldCount;

extern u8 D_0035F5D0[];

extern char D_003BB6B0[];

extern u64 dds3AdvanceWorldCounter(void);

extern u32 evtSpawnActionObj9(u64);

extern s32 btlSetActorEffectParameter();

extern s32 mdlFlagTest(u32);

extern SoundTask *btlDeferredTaskTail;

extern SoundTask *btlDeferredTaskHead;

extern s32 func_00214868(void);

u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

extern s32 datEnemyRecords;

extern void func_001DEFE0(s32, s32, f32);

extern void func_001B83D8(s32, s32, s32);

extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);

extern s32 btlGetRuntime(void);

extern s32 datItemSkillRecords;

extern s32 datCommandSelectors;

extern s32 datCommandRecords;

extern s32 datActionAnimationRecords;

extern s32 func_001F5028(s32 arg0);

extern s8 effSharedRandomState[];

extern s32 sdfAllocGeneralBlock(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern void sndResetTransition(void);

extern void func_001F4430(void);

extern void btlResetDeferredTaskQueue(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

void btlAdjustUnitHp(u8 *object, s32 value);

void btlAdjustUnitMp(u8 *object, s32 value);

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(s32 object);

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(s32 object);

void func_001A1948();

extern s8 effSharedRandomState[];

extern SndPad D_00324510;

extern void func_001B83D8(s32, s32, s32);

extern void sndSetStationedSeVolume(u32);

extern s32 sdfAllocGeneralBlock(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern void btlResetTitleStreamOnBattleFlag(void);

extern void func_001DC0E8(void);

extern void btlAdvanceWorldCounterAndSpawnActionObject(void);

extern void btlCreateRainEffect(u32, u32);

extern void btlRepositionPartyAroundBattleCenter(void);

extern s32 func_001A3638(void);

extern void kwlnFadeStartIn(s32);

extern s32 btlAreWorkBuffersReady(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

s32 btlBothSidesActive(UiObject *unit);

/* No selected entry is represented by -1. */ void btlClearActorSelectedEntryIndex(s32 actor);

void btlClearAllActorEntrySlots(u32 arg0);

void btlClearSceneTaskActiveFlag(s32 arg0);

s32 btlGetActorBedAssetIdFromIndex(s32 arg0);

s32 btlGetEntryFlagsUnlessDisabled(s32 entry);

struct EvtUnit;
extern void evtSetUnitRgbTransition(struct EvtUnit *, s32, u32);
extern void evtSetUnitAlphaTransition(struct EvtUnit *, s32, u32);
/* Retail passes the full motion index; only the stored field is a halfword. */
extern void evtUnitSetStoredParameter(struct EvtUnit *, s32);
extern void evtSetTransitionMotionScale(struct EvtUnit *, f32);
extern s32 btlIsCurrentValueBelowQuarterThreshold(void *);
extern s32 btlTestActorStatusPredicate(s32);
extern void btlApplyUnitModelScaledValue(u8 *);
extern s32 btlIsActorModeAcceptedByBattleHook(u8 *);
extern void func_001D5578(u8 *, s32, s32, f32);
extern void func_001D5990(u8 *);

s32 btlGetLoggedIndexedCommandItem(s32 index);

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

s32 btlIsUnitDefeatTriggeredByValueDelta(u8 *actor, s32 delta);

/* Set the actor's selected entry index. */ void btlSetActorSelectedEntryIndex(s32 actor, u32 index);

void btlSetTrackedTaskDisplayMode(s32 mode);

void btlSyncPlayerWork(UiObject *actor);

void fldAppendTaskToGroup(SceneTask *task);

void fldCreateSceneSpriteTask(s32 arg0);

void func_001A1960();

typedef struct BattleActionLinkState {
    u8 pad00[0x18];
    BtlUnit *unit;
    u8 pad1C[0x44];
    s32 actorIndices;
    u8 pad64[0x1C];
    struct BtlOperandGroup *groups; /* 0x80 */
} BattleActionLinkState;

typedef struct ActionUnit {
    u8 pad00[0x30];
    f32 pos30[4];
    f32 dir40[4];
    u8 pad50[0xA0];
    u32 flags;
    BattleActionLinkState *link;
    u8 padF8[0xC];
    u32 status;
    s32 actionKind;
    u8 pad10C[0xC];
    s32 actorIndices;
    s32 unk11C;
    u8 pad120[0x10];
    f32 unk130;
} ActionUnit;

typedef struct BtlActionPoseRuntime {
    u8 pad00[0x1FC];
    u32 flags;
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
    u8 pad04[0x2C];
    s32 categoryType;
    u8 pad34[4];
} BtlCategoryTableEntry;

typedef struct BtlStatArgs {
    BtlUnit *unit;
    s32 amount;
    s32 category;
} BtlStatArgs;

typedef struct BtlFx {
    u8 pad0[0x50];
    f32 f50;
    u8 pad54[4];
    f32 f58;
    u8 pad5C[0x24];
    f32 f80;
    u8 pad84[4];
    f32 f88;
    u8 pad8C[4];
    union {
        s128 vec90;
        struct {
            f32 f90;
            f32 f94;
            f32 f98;
            f32 f9C;
        };
    };
    s128 vecA0;
    f32 fB0;
    f32 fB4;
    f32 fB8;
    f32 fBC;
    f32 fC0;
} BtlFx;

typedef struct BtlFxSrcA {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
} BtlFxSrcA;

typedef struct BtlFxSrcB {
    s128 vec0;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    f32 f20;
} BtlFxSrcB;

typedef struct BtlTransparencyModel {
    u8 pad_00[0x80];
    s32 unk80;
} BtlTransparencyModel;

typedef struct BtlTransparencyInfo {
    u8 pad_00[0x18];
    BtlTransparencyModel *model;
} BtlTransparencyInfo;

typedef struct BtlTransparencyExt {
    u8 pad_00[0x68];
    s32 modelValue;
    u8 pad_6C[0x20];
    BtlTransparencyInfo *info;
} BtlTransparencyExt;

typedef struct BtlApproachRecord {
    u8 pad00[0xC];
    f32 reachOffset;
    u8 pad10[4];
} BtlApproachRecord;

typedef struct BtlApproachTable {
    u8 pad00[0x2C];
    BtlApproachRecord records[1];
} BtlApproachTable;

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

typedef struct BtlCameraResetUnit {
    u8 pad_000[0x110];
    u32 flags;
    u8 pad_114[0x10];
    u16 mode;
} BtlCameraResetUnit;

typedef struct BtlCameraResetResource {
    u8 pad_00[0x8C];
    void *model;
} BtlCameraResetResource;

typedef struct BtlCameraResetModel {
    u8 pad_00[0x1C];
    void *motion;
} BtlCameraResetModel;

typedef struct BtlCameraPose {
    s128 position;
    s128 direction;
    f32 distance;
    f32 fov;
} BtlCameraPose;

typedef struct BtlActionUnit {
    u8 pad_00[0x110];
    u32 flags;
} BtlActionUnit;

typedef struct BtlActionLink {
    u8 pad_00[0x18];
    BtlActionUnit *unit;
} BtlActionLink;

typedef struct BtlAction {
    u8 pad_00[0xF4];
    BtlActionLink *link;
} BtlAction;

typedef struct BtlAdvanceCursor {
    u16 unk_00;
    s16 frame;
    u8 pad_04[8];
    s16 index;
} BtlAdvanceCursor;

typedef struct BtlCursorActionLink {
    u8 pad_00[0x18];
    BtlUnit *unit;
} BtlCursorActionLink;

typedef struct BtlCursorAction {
    u8 pad_00[0xF4];
    BtlCursorActionLink *link;
} BtlCursorAction;

typedef struct BtlRainWork {
    u8 pad00[0x58C];
    u32 soundTransitionTask;
} BtlRainWork;

typedef struct BattleVoiceEntry {
    u8 volume;
    u8 pad01[3];
    char fileName[0xC];
} BattleVoiceEntry;

typedef struct SoundCursor {
    u16 unk_00;
    s16 frame;
    s16 mode;
    s16 index;
    u16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} SoundCursor;

#define CURSOR ((SoundCursor *)D_0035F100)

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

    for (actor = scene->actorList; actor != 0; actor = actor->nextActor) {
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
    for (actor = scene->actorList; actor != 0; actor = actor->nextActor) {
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

typedef struct SceneActor {
    u8 pad_00[0x108];
    s64 ownerId;
    u8 pad_110[0x1E8];
    s32 resourceNode;
    u8 pad_2FC[8];
    s32 listNode;
    u8 pad_308[0xC];
    s32 pendingResource;
} SceneActor;

s32 fldReleaseIdleSceneActorResources(SceneActor *actor) {
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
    if (actor->pendingResource != 0) {
        return 0;
    }
    if (btlIsUnitInActiveList((s32)actor) != 0) {
        btlResetActiveUnitList();
        return 0;
    }
    if (btlCountTasksForOwner(actor->ownerId) != 0) {
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
            func_001A1948(unit + 0x120, 0x1000);
            flags = *(u32 *)(unit + 0x110);
        }
        masked = flags & ~0x20;
        masked &= ~0x08000000;
        *(u32 *)(unit + 0x110) = masked;
        btlFlagUnitDefeatCandidate(unit);
        func_001D5990(unit);
        btlStartTask(btlAllocateIndexedUnitEffectTask(unit, 0xE, 0, 1.0f));
        fldAppendTaskToGroup((SceneTask *)task);
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
    UiObject *actor;
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
            if (btlBothSidesActive((UiObject *)unit) == 0) {
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001C9098);

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
        if (btlIsSupportedCommandKind(&task->result) != 0) {
            btlDispatchStateHandler((s32)task, 7);
        } else {
            fldSetSceneObjectAndGroupStates();
            if (btlAiCheckStatusRollEligibility(task) != 0) {
                btlDispatchStateHandler((s32)task, 0xB);
            } else {
                btlDispatchStateHandler((s32)task, 0xC);
            }
        }
    } else if (scene->flags & 0x8000) {
        fldSetSceneObjectAndGroupStates();
        btlDispatchStateHandler((s32)task, 9);
    }
}

void btlCommandResultEffectSelect(u8 *task) {
    s32 sel;
    s32 reason;
    s32 state;

    *(u32 *)(task + 8) &= ~4;
    state = *(s32 *)(task + 0x20);
    if (state <= 0) {
        goto spawn;
    }
    if (state >= 5) {
        if (state > 8) {
            goto spawn;
        }
        if (state < 7) {
            goto spawn;
        }
    }
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
spawn:
    fldCreateSceneSpriteTask((s32)task);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001C9960);

void func_001C9C20(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

INCLUDE_ASM(const s32, "game/code_001C8890", btlAiTaskUpdate);

void btlMarkSceneTaskAfterReset(s32 arg0) {
    func_001B83D8(arg0, 0, 0);
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 0x20;
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
    model = (u8 *)btlGetIndexListEntry(*(u32 *)(actor + 0x60), 0);
    if (*(u32 *)(actor + 0x20) == 1 &&
        (btlIsActiveActor((s32)model) == 0 ||
         (*(u64 *)(model + 0x110) & 0xE1) != 1)) {
        btlDispatchStateHandler(actor, 26);
    } else {
        btlDispatchStateHandler(actor, 12);
    }
}

void btlResetCommandIndexWork(s32 arg0) {
    func_001ACC20();
    btlResetIndexWork(arg0 + 0x20);
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x2f4) = 0xffffffff;
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
    s32 *commandPtr;
    s32 command;
    btlBossDebugPrintf(D_003A3648, *(s32 *)(task + 0x20));
    commandPtr = (s32 *)(task + 0x20);
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
    if (btlGetIndexListCount(*(s32 *)(task + 0x60)) == 1) {
        *(s64 *)(task + 0x68) = *(s64 *)((u8 *)btlGetIndexListEntry(*(s32 *)(task + 0x60), 0) + 0x108);
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CE0D8);

void func_001CE5D8(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001CE5F0);

void func_001CEA58(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

extern u8 *btlCreateModelChangeTask(u8 *, s32, s32, s32, s32, u8);

void func_001CEA70(u8 *task) {
    u8 *actor;
    u8 *effectTask;
    u8 *modelTask;
    s32 *statusTable;
    s32 model;

    if (btlCountTasksByKind(0x1A) != 0 ||
        btlCountTasksByKind(0x18) != 0 ||
        btlCountTasksByKind(0x23) != 0) {
        return;
    }

    btlGetRuntime();
    actor = *(u8 **)(task + 0x18);
    statusTable = (s32 *)btlGetSideIndexedActorStatusTable(
        *(s32 *)(actor + 0xC4), *(s32 *)(actor + 0xC8));
    model = *(u16 *)((u8 *)statusTable + 0x2A);
    effectTask = (u8 *)btlCreateEffObjB(actor, 0x7E);
    btlStartTask((s32)effectTask);
    modelTask = btlCreateModelChangeTask(actor, 0, 0x1F, model, 0x12, 1);
    btlStartTask((s32)modelTask);

    *(u32 *)(actor + 0x110) = (*(u32 *)(actor + 0x110) | 0x1000) & 0xEFFFFFFF;
    *(u16 *)(actor + 0x120) |= 0x1000;
    if (*(u32 *)(task + 8) & 0x10) {
        btlDispatchStateHandler((s32)task, 0x1B);
    } else {
        btlDispatchStateHandler((s32)task, 0x1A);
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

extern u64 btlStartTask(s32);

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
        btlSyncPlayerWork((UiObject *)actor);
    }
    object = fldCreateSceneGroupAction(task, 0x64, 1);
    *(s32 *)(object + 0x28) = 0xF;
    *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(task + 0x18) + 0x108);
    btlStartTask((s32)object);
    object = func_001D9468(actor, 1);
    *(s32 *)(object + 0x28) = 0xF;
    return btlStartTask((s32)object);
}

extern s32 btlCountTasksByKind(u16 kind);

extern void btlRepositionPartyAroundBattleCenter(void);

extern s32 func_001A3638(void);

extern void fldUpdateSceneGroupTask(s32 task);

extern void btlRemoveTaskFromSceneGroup(SceneTask *task);

extern void btlDispatchStateHandler(s32 object, s32 state);

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
    if (fldReleaseIdleSceneActorResources((SceneActor *)*(s32 *)(task + 0x18)) != 0) {
        if (*(u32 *)(unit + 0x110) & 0x200) {
            func_001A1960(unit + 0x120, 8);
            func_001A2258(unit);
        }
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup((SceneTask *)task);
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

void btlRecordLinkedActorOutcome(s32 object) {
    s32 context = btlGetRuntime();
    s32 target = *(s32 *)(object + 0x18);
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

extern void btlResetIndexWork();

void btlUnitTurnEndCommit(s32 task) {
    void (*hook)(s32) = *(void (**)(s32))(btlGetRuntime() + 0x600);
    s32 owner = *(s32 *)(task + 0x18);

    if (hook != 0) {
        hook(task);
    }
    btlAdvanceHistoryCounter(task);
    btlResetIndexWork((u8 *)task + 0x20);
    *(s32 *)(owner + 0x2F4) = -1;
    *(u32 *)(task + 0xC) &= ~1;
    *(s32 *)(task + 0x14) += 1;
    *(u32 *)(owner + 0x110) &= ~0x4000;
    fldUpdateSceneGroupTask(task);
    if (*(u32 *)(*(s32 *)(task + 0x18) + 0x110) & 0x20) {
        btlUnitTurnEndStateSelect((u8 *)task);
    } else {
        btlDispatchStateHandler(task, 2);
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D0210);

extern void btlResetIndexWork();

void btlRemoveEligibleActorSceneTask(s32 arg0) {
    UiObject *actor = *(UiObject **)(arg0 + 0x18);
    s32 entryFlags;

    if (actor->flags & 0x200) {
        if (fldReleaseIdleSceneActorResources((SceneActor *)actor) == 0) {
            return;
        }
        btlResetIndexWork(arg0 + 0x20);
        actor->marker = -1;
        btlClearAllActorEntrySlots((u32)actor);
        fldUpdateSceneGroupTask(arg0);
        btlRemoveTaskFromSceneGroup((SceneTask *)arg0);
        btlDispatchStateHandler(arg0, 1);
    } else {
        if ((actor->flags & 0x400) == 0) {
            return;
        }
        entryFlags = btlGetEntryFlagsUnlessDisabled((s32)((u8 *)actor + 0x120));
        if ((actor->flags & 0x40) == 0 && !(entryFlags & 0x200)) {
            return;
        }
        fldUpdateSceneGroupTask(arg0);
        btlRemoveTaskFromSceneGroup((SceneTask *)arg0);
        if ((entryFlags & 0x200) && (actor->flags & 0x40) == 0) {
            btlDispatchStateHandler(arg0, 1);
        } else {
            btlDispatchStateHandler(arg0, 0x1E);
        }
    }
}

void func_001D0590(void) {
}

void btlCommandTaskReleaseActor(s32 arg0) {
    if ((*(u32 *)(arg0 + 8) & 8) != 0) {
        if (fldReleaseIdleSceneActorResources((SceneActor *)*(s32 *)(arg0 + 0x18)) == 0) {
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
    void (*start)(s32);
    void (*update)(s32);
    void (*finish)(s32);
} BattleActionState;

extern BattleActionState D_00359B28[];

void btlDispatchStateHandler(s32 object, s32 kind) {
    ((s32 *)object)[0] = kind;
    ((s32 *)object)[4] = 0;
    D_00359B28[kind].start(object);
}
extern char D_003A3788[]; /* "btl:action seq create[%p]\n" */

u8 *btlCreateActionSeq(void) {
    u8 *sequence = sdfAllocAndClearQuadwords(0x170);
    u8 *context;
    u8 *head;

    *(u16 *)(sequence + 4) = 1;
    btlInitBattleIndexWork(sequence + 0x20);
    context = (u8 *)btlGetRuntime();
    *(u8 **)(sequence + 0x168) = 0;
    head = *(u8 **)(context + 0x224);
    if (head != 0) {
        *(u8 **)(head + 0x168) = sequence;
        *(u8 **)(sequence + 0x16C) = *(u8 **)(context + 0x224);
    } else {
        *(u8 **)(sequence + 0x16C) = 0;
    }
    *(u8 **)(context + 0x224) = sequence;
    btlDispatchStateHandler((s32)sequence, 0);
    btlBossDebugPrintf(D_003A3788, sequence);
    return sequence;
}

extern char D_003A37A8[]; /* "btl:action seq delete[%p]\n" */

void btlDestroyActionSeq(s32 object) {
    btlBossDebugPrintf(D_003A37A8, object);
    btlReleaseObjectBuffers(object + 0x20);
    if (*(s32 *)(object + 0x16C) != 0) {
        *(s32 *)(*(s32 *)(object + 0x16C) + 0x168) = *(s32 *)(object + 0x168);
    }
    if (*(s32 *)(object + 0x168) != 0) {
        *(s32 *)(*(s32 *)(object + 0x168) + 0x16C) = *(s32 *)(object + 0x16C);
    } else {
        s32 context = btlGetRuntime();
        *(s32 *)(context + 0x224) = *(s32 *)(object + 0x16C);
    }
    sdfReleaseChipBlock(object);
}

void btlUpdateActionSeqs(void) {
    s32 action = *(s32 *)(btlGetRuntime() + 0x224);
    while (action != 0) {
        s32 flags = *(s32 *)(action + 8);
        s32 next = *(s32 *)(action + 0x16C);
        if (flags & 1) {
            D_00359B28[*(s32 *)action].update(action);
            *(s32 *)(action + 0x10) += 1;
        } else if (flags & 2) {
            btlDestroyActionSeq(action);
        }
        action = next;
    }
}

void btlDestroyAllActionSeqs(void) {
    u8 *node = *(u8 **)(btlGetRuntime() + 0x224);
    while (node != 0) {
        u8 *next = *(u8 **)(node + 0x16C);
        btlDestroyActionSeq((s32)node);
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

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3738);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3748);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3758);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3768);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3778);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3788);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A37A8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D09B8);

void btlDispatchCommandViaHookOrDefault(u8 *command, u8 *argument) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x604);

    if (handler != 0) {
        s32 result = handler((s32)command, (s32)argument);
        if (result != -1) {
            btlDispatchStateHandler((s32)command, result);
            return;
        }
    }
    switch (*(s32 *)argument) {
    case 1:
        btlDispatchStateHandler((s32)command, 0xD);
        break;
    case 4:
        *(s32 *)(argument + 4) = btlGetLoggedIndexedCommandItem(*(s32 *)(argument + 8));
        /* fallthrough */
    case 2:
    case 3:
    case 7:
    case 8:
        if (*(s8 *)(datCommandSelectors + *(s32 *)(argument + 4) * 2 + 1) != 1) {
            btlDispatchStateHandler((s32)command, 0xE);
        } else {
            if (*(u32 *)(*(s32 *)(command + 0x18) + 0x110) & 0x200) {
                scrSetGlobalSeenBit(*(u16 *)(argument + 4));
            }
            btlDispatchStateHandler((s32)command, 0xF);
        }
        break;
    case 5:
        if (*(u32 *)(*(s32 *)(command + 0x18) + 0x110) & 0x1000) {
            btlDispatchStateHandler((s32)command, 0x10);
        } else {
            btlDispatchStateHandler((s32)command, 0x11);
        }
        break;
    case 10:
    case 13:
    case 14:
        btlDispatchStateHandler((s32)command, 0x13);
        break;
    case 9:
        if (*(u32 *)(*(s32 *)(command + 0x34) + 0x110) & 1) {
            btlDispatchStateHandler((s32)command, 0x16);
        } else {
            btlDispatchStateHandler((s32)command, 0x15);
        }
        break;
    case 12:
        btlDispatchStateHandler((s32)command, 0x15);
        break;
    case 6: {
        u32 flags = *(u32 *)(*(s32 *)(command + 0x18) + 0x110);
        if (flags & 0x200) {
            btlDispatchStateHandler((s32)command, 0x17);
        } else if (flags & 0x400) {
            btlDispatchStateHandler((s32)command, 0x14);
        }
        break;
    }
    case 11:
        btlDispatchStateHandler((s32)command, 0x14);
        break;
    case 15:
        btlDispatchStateHandler((s32)command, 0x18);
        break;
    case 16:
        btlDispatchStateHandler((s32)command, 0x19);
        break;
    case 17:
        btlDispatchStateHandler((s32)command, 0x1F);
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

INCLUDE_ASM(const s32, "game/code_001C8890", btlResolveActionOperand);

u32 btlClassifyActionOperand(u8 *actor, u8 *argument) {
    switch (*(s32 *)argument) {
    case 1: {
        u32 count = btlGetIndexListCount(*(s32 *)(argument + 0x40));
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

    btlGetEntryFlagsUnlessDisabled((s32)(arg0 + 0x120));
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

/* Operand payload words remain unknown; only the flag bits are identified. */
typedef struct BtlOperandEntry {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    u8 pad14[4];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[2];
    u16 flags;
} BtlOperandEntry;

/* Retained group: a header followed by 64 operands. Reset leaves the payload intact. */
typedef struct BtlOperandGroup {
    u8 count;
    u8 pad01[7];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 pad11[3];
    u8 unk14;
    u8 pad15[7];
    BtlOperandEntry entries[64];
} BtlOperandGroup;

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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D1218);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D12A0);

typedef struct BtlIndexList {
    s32 capacity;       /* 0x00: allocated entry count */
    s32 count;          /* 0x04: live entry count */
    u32 *entries;       /* 0x08: points just past this header */
} BtlIndexList;


/* Owns an index list and an SDK allocation containing thirteen operand groups. */
typedef struct BattleIndexWork {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24[9];
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u16 unk30;
    u8 pad32[2];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    BtlIndexList *indices;
    u8 pad44[4];
    u64 unk48;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u16 unk5C;
    u8 unk5E;
    u8 pad5F;
    BtlOperandGroup *groups;
    u32 allocationHandle;
} BattleIndexWork;

/* Reset work status and group headers, preserving the retained payload and allocation. */
void btlResetIndexWork(BattleIndexWork *work) {
    u32 i;
    s32 offset;
    BtlOperandGroup *entry;
    BtlOperandGroup *group;
    work->unk00 = -1;
    work->unk04 = -1;
    work->unk08 = -1;
    work->unk0C = 0;
    work->unk10 = 0;
    work->unk14 = 0;
    work->unk18 = -1;
    work->unk1C = 0;
    work->unk20 = 8;
    work->unk30 = 0;
    work->unk34 = 0;
    work->unk38 = -1;
    work->unk3C = 0;
    work->unk2D = 0;
    work->unk2E = 0;
    work->unk50 = 0;
    work->unk54 = 0;
    work->unk58 = 0;
    work->unk5C = 0;
    work->unk5E = 0;
    /* SDK addresses are words; offset-first arithmetic is required for matching. */
    for (i = 0, offset = 0; i < 13; i++) {
        ((BtlOperandGroup *)(offset + (u32)work->groups))->count = 0;
        entry = (BtlOperandGroup *)(offset + (u32)work->groups);
        entry->unk08 = 0;
        entry->unk14 = 0;
        group = (BtlOperandGroup *)(offset + (u32)work->groups);
        offset += sizeof(BtlOperandGroup);
        group->unk10 = 0;
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

extern void btlFreeIndexList();

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

extern u32 func_001D2C78(void *);

extern void *btlAllocTask(s32);

extern u32 btlGetTaskArguments(s32);

void *btlCreateActorParameterDeltaTask(u8 *owner, BattleDeltaSpec *spec) {
    u8 *task = btlAllocTask(0x2C);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x45;
    *(void **)(task + 0x4C) = func_001D2C78;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    func_001D5990(actor);
    btlIsUnitDefeatTriggeredByValueDelta(actor, 0);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    func_001A1948(owner + 0x120, args[1]);
    func_001D5990(owner);
    btlIsUnitDefeatTriggeredByValueDelta(owner, 0);
    return 1;
}

extern void *btlAllocTask(s32);

extern u32 btlGetTaskArguments(s32);

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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
        btlAdjustUnitHp((u8 *)&unit->statBits, -0x7FFF);
        func_001A1948(&unit->statBits, 0x4000);
        unit->flags |= 0x20;
    }
    if (args->amount == 0) {
        return 1;
    }
    switch (((BtlCategoryTableEntry *)datCommandRecords)[args->category].kind03) {
    case 1:
        btlAdjustUnitHp((u8 *)&unit->statBits, -args->amount);
        return 1;
    case 2:
        btlAdjustUnitMp((u8 *)&unit->statBits, -args->amount);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}
u32 btlApplyQueuedActorEntrySelection(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        btlSetActorSelectedEntryIndex(*arg0, arg0[7]);
        func_001D5990(*arg0);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}
u32 btlClearQueuedActorEntrySelection(u32 *arg0) {
    btlClearActorSelectedEntryIndex(*arg0);
    func_001D5990(*arg0);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D3688);

extern void func_001D3688();

u8 *btlCreateActorSoundOptionTask(u8 *arg0, s32 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4C;
    *(void **)(task + 0x4C) = func_001D3688;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 func_001D3A20(s32 arg0) {
    if ((*(u8 *)(*(s32 *)(arg0 + 4) * 8 + datItemSkillRecords + 1) & 4) != 0) {
        func_00119900(*(s32 *)(arg0 + 4), 0xffffffffffffffff);
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
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 btlPlayQueuedBattleVoice(s32 arg0) {
    func_00119900(*(u16 *)(arg0 + 4), 1);
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
    data = (u32 *)btlGetTaskArguments((s32)task);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
                                func_001A1948(actor + 0x120, 1);
                                func_001D5990(actor);
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
u32 btlGetTaskArguments(s32 task) {
    return (u32)((SoundTask *)task)->args;
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
u64 btlStartTask(s32 taskAddress) {
    u64 value = btlAdvanceRuntimeSequenceCounter();
    SoundTask *task = (SoundTask *)taskAddress;
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

extern s32 btlBossDebugPrintf(const char *, ...);

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

void btlInitializeEffectVectorsFromSourceRecords(BtlFx *fx, s32 kind, s32 index) {
    BtlFxSrcA *alt = (BtlFxSrcA *)btlSelectSharedOrIndexedTransformParameters(kind, index);
    BtlFxSrcB *base = (BtlFxSrcB *)btlGetSideIndexedActorStatusTable(kind, index);

    if (alt->fC == 0.0f) {
        PCP_COPY_VECTOR(&fx->vec90, base);
        fx->fB4 = base->f18;
        fx->fB0 = base->f1C;
        fx->fC0 = base->f20;
    } else {
        fx->f90 = alt->f0;
        fx->f94 = alt->f4;
        fx->f98 = alt->f8;
        fx->f9C = 0.0f;
        fx->fB4 = alt->f10;
        fx->fB0 = alt->f14;
    }
    PCP_COPY_VECTOR(&fx->vecA0, base);
    fx->fBC = base->f18;
    fx->fB8 = base->f1C;
    fx->fC0 = base->f20;
    fx->f80 = base->f10;
    fx->f50 = base->f10;
    fx->f88 = base->f14;
    fx->f58 = base->f14;
}
extern s32 mdlGetContextResourceGroup(s32);

extern s32 mdlGetContextResourceId(s32);

s32 btlHasMatchingModel(s32 effect, s32 model) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x228);
    while (node != 0) {
        if ((*(u32 *)(node + 0x110) & 2) != 0 &&
            *(s32 *)(node + 0x320) != 0 &&
            *(s32 *)(node + 0x308) != 0 &&
            mdlGetContextResourceGroup(*(s32 *)(*(s32 *)(node + 0x320) + 0x8C)) == effect &&
            mdlGetContextResourceId(*(s32 *)(*(s32 *)(node + 0x320) + 0x8C)) == model) {
            return 1;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D4E60);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D4E98);

extern const char D_003A3AD0[];

void btlReleaseActorModelResources(u8 *object) {
    s32 sound;
    s32 load;
    s32 model;
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
        model = *(s32 *)(object + 0x31C);
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
void btlFlagUnitDefeatCandidate(u8 *unit) {
    s32 (*hook)(u8 *) = *(s32 (**)(u8 *))((u8 *)btlGetRuntime() + 0x654);
    if (hook == 0 || hook(unit) != 0) {
        *(u32 *)(unit + 0x110) |= 4;
        if (!(*(u32 *)(unit + 0x110) & 0x8000000)) {
            *(u32 *)(unit + 0x110) |= 8;
            if (*(u32 *)(unit + 0x110) & 2) {
                **(u32 **)(*(u8 **)(unit + 0x320) + 0x8C) &= ~1;
            }
        }
    }
}

void btlClearUnitDefeatCandidate(u8 *object) {
    s32 (*callback)(u8 *);
    u32 flags;
    u32 masked;

    callback = *(s32 (**)(u8 *))(btlGetRuntime() + 0x658);
    if (callback != 0 && callback(object) == 0) {
        return;
    }
    flags = *(u32 *)(object + 0x110);
    masked = flags & ~4;
    masked &= ~8;
    *(u32 *)(object + 0x110) = masked;
    if ((flags & 2) != 0) {
        u32 *resource = *(u32 **)(*(u8 **)(object + 0x320) + 0x8C);
        *resource |= 1;
    }
}

u32 btlIsUnitInfoFlagOneEligible(u8 *object) {
    u32 flags = *(u32 *)(object + 0x110);
    if ((flags & 0x8000000) != 0) {
        return 0;
    }
    if ((flags & 1) == 0) {
        return 0;
    }
    if ((flags & 2) == 0) {
        return 0;
    }
    return **(u8 **)(*(u8 **)(object + 0x320) + 0x8C) & 1;
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3AD0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D5578);

void func_001D5990(u8 *unit) {
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
    entryFlags = btlGetEntryFlagsUnlessDisabled((s32)(unit + 0x120));
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
    if (btlTestActorStatusPredicate((s32)unit) != 0) {
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
    if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0) != 0 &&
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
        func_001D5578(unit, index, mode, rate);
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

extern void func_001D5578(u8 *, s32, s32, f32);

void btlApplyScaledUnitEffectParameter(u8 *object, s32 index, s32 argument, f32 scale) {
    u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    f32 value = *(f32 *)(resource + index * 20 + 0x34);
    func_001D5578(object, index, argument, value * scale);
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
            s32 model = *(s32 *)(*(s32 *)(object + 0x320) + 0x8C);
            s32 node = mdlGetNodeField2C(model, 0);
            if (*(s16 *)(resource + node * 20 + 0x30) == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                func_001D5990(object);
                func_001D5578(object, *(s32 *)(object + 0xFC),
                              *(s32 *)(object + 0x100),
                              *(f32 *)(object + 0x104));
            }
        }
        object = *(u8 **)(object + 0x344);
    }
}

void btlApplyUnitModelScaledValue(u8 *object) {
    s32 context;
    u8 *resource;
    f32 volume;
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return;
    }
    context = btlGetRuntime();
    *(u32 *)(object + 0xE8) &= ~1;
    volume = *(f32 *)(object + 0xF4);
    resource = *(u8 **)(object + 0x320);
    *(f32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) =
        volume * (30.0f / (f32)*(s8 *)(context + 0x490));
}

void btlResetUnitModelProgress(u8 *object) {
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        u8 *resource = *(u8 **)(object + 0x320);
        *(u32 *)(object + 0xE8) |= 1;
        *(u32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D6050);

f32 btlGetUnitModelValue1C(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0.0f;
    }
    return *(f32 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x1c);
}

void btlAdvanceUnitModelFrame(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) != 0) {
        sdfMotionSampleAtFrame(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 800) + 0x8c) + 0x1c));
        return;
    }
}

s32 btlGetUnitModelFrameCount(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0;
    }
    return *(u16 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x2e);
}

void btlSeekUnitModelFrameZero(s32 arg0) {
    extern void sdfMotionSampleAtFrame(void *, f32);

    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return;
    }
    sdfMotionSampleAtFrame(*(void **)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c), 0.0f);
}

void btlSeekRandomModelFrame(u8 *object) {
    s32 duration;
    u32 randomFrame;
    f32 frame;
    u8 *resource;
    extern void sdfMotionSampleAtFrame(void *, f32);

    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return;
    }
    duration = btlGetUnitModelFrameCount((s32)object);
    if (duration > 0) {
        randomFrame = (u32)effMiscRandMod(0, duration);
        frame = (f32)randomFrame;
        resource = *(u8 **)(object + 0x320);
        sdfMotionSampleAtFrame(*(void **)(*(u8 **)(resource + 0x8C) + 0x1C),
                       frame);
    }
}

u32 btlIsUnitModelStateFive(s32 object) {
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 1;
    }
    if (*(s32 *)(object + 0xF0) != 2) {
        return 1;
    }
    return *(u8 *)(*(s32 *)(*(s32 *)(*(s32 *)(object + 0x320) + 0x8C) + 0x1C) + 0x30) == 5;
}

void btlSetUnitPosition(u8 *object, void *position) {
    f32 world[4] __attribute__((aligned(16)));
    s32 context;
    if ((*(u32 *)(object + 0x114) & 0x80) != 0) {
        return;
    }
    context = btlGetRuntime();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(position));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x60) : "memory");
    __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder" : : "r"(context));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(world) : "memory");
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
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\tlqc2 vf11, 0(%1)\n\tvadd.xyzw vf10, vf10, vf11\n\tsqc2 vf10, 0(%2)\n\t.set reorder"
                     : : "r"(object + 0x60), "r"(context), "r"(worldPosition) : "memory");
}

extern void btlRefreshUnitFxVectors(u8 *);

s32 btlSetActorEffectParameter(object, value)
u8 *object;
s32 value;
{
    s32 (*callback)(u8 *, s32);
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    callback = *(s32 (**)(u8 *, s32))(btlGetRuntime() + 0x5BC);
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        u8 *resource = *(u8 **)(object + 0x320);
        u8 *effect = *(u8 **)(resource + 0x8C);
        return (s8)sdfLoadMapRecordPositionVector(*(s32 *)(effect + 0x18), value);
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D6428);

extern s32 sdfLoadMapRecordLookAtBasis(s32, s32);

s32 btlSetActorAlternateEffectParameter(object, value)
u8 *object;
s32 value;
{
    s32 (*callback)(u8 *, s32);
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    callback = *(s32 (**)(u8 *, s32))(btlGetRuntime() + 0x5BC);
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        u8 *resource = *(u8 **)(object + 0x320);
        u8 *effect = *(u8 **)(resource + 0x8C);
        return (s8)sdfLoadMapRecordLookAtBasis(*(s32 *)(effect + 0x18), value);
    }
}

void btlSetAlternateEffectParameterOrMuzzlePosition(void) {
    if (btlSetActorAlternateEffectParameter() != 0) {
        return;
    }
    __asm__ volatile(".set noreorder\n\tvsub.xyzw vf28, vf0, vf0\n\tvmr32.xyzw vf30, vf0\n\tvmove.xyzw vf31, vf0\n\tvaddw.x vf28, vf28, vf0w\n\tvmr32.xyzw vf29, vf30\n\t.set reorder");
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

extern void effObjSetInnerSecondVec(s32, void *);

void btlSetUnitRotation(u8 *object, void *rotation) {
    u8 vector[16];
    if ((*(u32 *)(object + 0x114) & 0x100) != 0) {
        return;
    }
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(rotation));
    if ((*(u32 *)(object + 0x110) & 0x10) != 0) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r"(D_003A3B70));
        effMiscQuatMultiplyVU();
    }
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x70));
    __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r"(D_003A3B70));
    effMiscQuatMultiplyVU();
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(vector));
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

void btlReleaseUnitModelColorResource(s32 arg0, s32 arg1) {
    mdlReleaseInnerResourceHandle(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c), (arg1 & 0xffffff) | 0x80000000);
}

extern void effObjFetchInnerFirstVec(u32);

extern void effObjFetchInnerSecondVecNorm(u32);

extern void mdlStorePrimaryVectorVU(u32);

extern void mdlUpdateContextRotationBasisFromQuaternion(u32);

extern void sdfModelUpdateCurrentFrameTransforms(u32);

void btlRefreshUnitFxVectors(u8 *unit) {
    if (!(*(u32 *)(unit + 0x110) & 2)) {
        return;
    }
    effObjFetchInnerFirstVec(*(u32 *)(unit + 0x31C));
    mdlStorePrimaryVectorVU(*(u32 *)(*(u8 **)(unit + 0x320) + 0x8C));
    effObjFetchInnerSecondVecNorm(*(u32 *)(unit + 0x31C));
    mdlUpdateContextRotationBasisFromQuaternion(*(u32 *)(*(u8 **)(unit + 0x320) + 0x8C));
    sdfModelUpdateCurrentFrameTransforms(*(u32 *)(*(u8 **)(*(u8 **)(unit + 0x320) + 0x8C) + 0x18));
}

extern void btlUnitGetBodyPosVU(u8 *);

extern s32 btlAimHorizontalDirectionVU(void *, void *);

void btlUnitFaceTarget(u8 *object, u8 *target) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if ((*(u32 *)(object + 0x110) & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        VU_STORE10(first);
        btlUnitGetBodyPosVU(target);
        VU_STORE10(second);
        if (btlAimHorizontalDirectionVU(first, second) != 0) {
            VU_STORE10(result);
            btlSetUnitRotation(object, result);
        }
    }
}

extern s32 func_001F7868(void *, void *, f32);

void btlUnitFaceTargetScaled(u8 *object, u8 *target, f32 scale) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if ((*(u32 *)(object + 0x110) & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(first));
        btlUnitGetBodyPosVU(target);
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(second));
        func_001F7868(first, second, scale);
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(result));
        btlSetUnitRotation(object, result);
    }
}

typedef struct BtlUnitStats {
    u32 word[0x69];
} BtlUnitStats;

void btlCopyUnitStats(s32 arg0, s32 arg1) {
    BtlUnitStats *stats = (BtlUnitStats *)(arg0 + 0x120);
    *stats = *(BtlUnitStats *)arg1;
    btlRefreshUnitMaximumHpAndClampCurrentHp((s32)stats);
    btlRefreshUnitMaximumMpAndClampCurrentMp((s32)stats);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D6A80);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3B70);

void btlCreateUnitTransparency(u8 *unit) {
    u8 *shape;
    if ((*(u32 *)(unit + 0x110) & 2) == 0) {
        return;
    }
    if (*(s32 *)(unit + 0x324) != 0) {
        return;
    }
    if (*(u8 *)(unit + 0xCC) != 0) {
        return;
    }
    shape = *(u8 **)(*(u8 **)(*(u8 **)(unit + 0x320) + 0x8C) + 0xC);
    *(s32 *)(unit + 0x324) = sdfModelCreateWithItems(*(s32 *)(shape + 0x14), *(s32 *)(shape + 0x18));
    dds3SetObjectFlags(*(s32 *)(unit + 0x31C), 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

extern void sdfReleaseDevSlot(s32, s32, s32);
extern void mdlBroadcastMasked(s32, s32);
extern void mdlProcessContextNodesAndTransforms(BtlTransparencyInfo *, s32);
extern void dds3ClearObjectFlags(s32, s32);
extern void dds3SetObjectFlags(s32, s32);
extern void func_001D6A80(BtlUnit *, BtlTransparencyInfo *, s32, s32, u32);
extern s32 D_00325788[];
extern s32 D_00359D10[];

void btlUpdateUnitTransparency(BtlUnit *unit) {
    u32 flags = unit->flags;
    u32 color;
    BtlTransparencyInfo *info;
    u32 alpha;

    if (flags & 2) {
        if (unit->unkCC == 0) {
            color = unit->overlayColor;
            info = ((BtlTransparencyExt *)unit->ext)->info;
            alpha = color >> 24;
            if (!(flags & 0x20000)) {
                if (unit->transparencyModel != 0) {
                    sdfReleaseDevSlot(unit->transparencyModel, 1, 1);
                    unit->transparencyModel = 0;
                    if (unit->flags & 2) {
                        info->model->unk80 = ((BtlTransparencyExt *)unit->ext)->modelValue;
                    } else {
                        info->model->unk80 = 0;
                    }
                    mdlBroadcastMasked(info, color);
                    mdlProcessContextNodesAndTransforms(info, (s32)D_00325788);
                    dds3ClearObjectFlags(unit->effectObject, 1);
                    btlBossDebugPrintf(D_003A3AD0, unit);
                }
            } else if (alpha == 0) {
                dds3SetObjectFlags(unit->effectObject, 1);
            } else if (unit->transparencyModel == 0) {
                btlCreateUnitTransparency((u8 *)unit);
            } else {
                func_001D6A80(unit, info, unit->transparencyModel, (s32)D_00359D10, color);
            }
        }
    }
}
INCLUDE_ASM(const s32, "game/code_001C8890", func_001D6FB0);

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
                      btlGetActorBedAssetIdFromIndex(*(u16 *)(actor + 0x172)),
                      *(u16 *)(actor + 0x124));
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3BA8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D7258);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D74B8);

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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)owner;
    return task;
}

u32 btlPollThresholdTask(s32 *arguments) {
    if ((s32)btlGetUnitModelValue1C(arguments[0]) >= arguments[1]) {
        if ((*(u32 *)(arguments[0] + 0xE8) & 1) == 0) {
            btlResetUnitModelProgress((u8 *)arguments[0]);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
        BtlApproachTable *table = (BtlApproachTable *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->resourceIndex);
        args->offset = table->records[unit->unkEC].reachOffset * unit->scale;
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
    pos[2] -= unit->positionZOffset;
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
            PCP_COPY_VECTOR(&args->from, (u8 *)unit + 0x60);
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
    task->owner = unit->owner;
    *(void **)((u8 *)task + 0x4C) = btlUpdateUnitPositionInterpolationTask;
    *(u32 *)((u8 *)task + 0x48) = 0;
    args = (u8 *)btlGetTaskArguments((s32)task);
    *(f32 *)(args + 0x20) = scale;
    *(u32 *)(args + 0x2C) = (u32)unit;
    *(u32 *)(args + 0x24) = 0;
    *(u32 *)(args + 0x28) = 0;
    PCP_COPY_VECTOR(args, (u8 *)unit + 0x60);
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
                PCP_COPY_VECTOR(&args->from, (u8 *)unit + 0x70);
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
    task->owner = unit->owner;
    *(void **)((u8 *)task + 0x4C) = btlStepUnitRotationNlerp;
    *(u32 *)((u8 *)task + 0x48) = 0;
    args = (u8 *)btlGetTaskArguments((s32)task);
    *(f32 *)(args + 0x20) = scale;
    *(u32 *)(args + 0x2C) = (u32)unit;
    *(u32 *)(args + 0x24) = 0;
    *(u32 *)(args + 0x28) = 0;
    PCP_COPY_VECTOR(args, (u8 *)unit + 0x70);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

extern char D_003A3BC8[];

extern char D_003A3BE8[];

extern void func_001D4E98(u8 *, u32, u32);

void btlRequestModelOrReuse(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        func_001D4E98(object, effect, model);
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
        func_001D4E98(object, effect, model);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3BC8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3BE8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3C08);

s32 btlBeginModelChange(u32 *arguments) {
    s32 owner = arguments[0];
    u32 model = arguments[1];
    u32 variant = arguments[2];
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        btlRequestModelAssetByMode(owner, model, variant);
        *(u32 *)(owner + 0x118) = (*(u32 *)(owner + 0x118) | 1) & ~2;
        return btlBossDebugPrintf("btl:model change start[%X,%X]\n", model, variant);
    }
    return status;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D8190);

extern u32 func_001D8190(u32 *);

u8 *btlCreateModelChangeTask(u8 *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    u8 *task = btlAllocTask(0x1C);
    u32 *args;
    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x1A;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlBeginModelChange;
    *(void **)(task + 0x4C) = func_001D8190;
    args = (u32 *)btlGetTaskArguments((s32)task);
    args[0] = (u32)unit;
    args[1] = arg1;
    args[2] = arg2;
    args[3] = arg3;
    args[4] = arg4;
    *((u8 *)args + 0x19) = arg5;
    *((u8 *)args + 0x18) = 0;
    args[5] = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        evtSetUnitStatusFlags(*(u32 *)(*(s32 *)(arg0 + 0xc) + 800));
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(u32 *arg0) {
    if ((*(u64 *)(arg0[3] + 0x110) & 0x1000000002) == 0x1000000002) {
        func_00221D00(*(u32 *)(arg0[3] + 800), arg0[2], *arg0, arg0[1]);
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
    args = (u32 *)btlGetTaskArguments((s32)task);
    args[3] = (u32)unit;
    args[0] = arg1;
    args[1] = arg2;
    args[2] = arg3;
    return task;
}

void btlPrepareUnitStatusFxOnStart(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x110) & 2) != 0) {
        evtSetUnitStatusFlags(*(u32 *)(*(s32 *)(arg0 + 0x14) + 800));
        return;
    }
}

u32 btlApplyUnitVectorFxWhenLoaded(u8 *arguments) {
    u8 *object = *(u8 **)(arguments + 0x14);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(arguments));
        evtSetUnitNormalizedDirection(*(s32 *)(object + 0x320), *(s32 *)(arguments + 0x10));
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
    args = (u32 *)btlGetTaskArguments((s32)task);
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
            btlFlagUnitDefeatCandidate((u8 *)unit);
            mdlBroadcastMasked(*(s32 *)(unit->ext + 0x8C), 0);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
            btlFlagUnitDefeatCandidate((u8 *)unit);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D8E80);

extern u32 func_001D8E80(u32 *);

u8 *func_001D9038(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x13;
    *(void **)(task + 0x4C) = func_001D8E80;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D90C0);

extern u32 func_001D90C0(u32 *);

u8 *func_001D91E0(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x14;
    *(void **)(task + 0x4C) = func_001D90C0;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D9268);

extern u32 func_001D9268(u32 *);

u8 *func_001D9468(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x15;
    *(void **)(task + 0x4C) = func_001D9268;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

extern s32 sndMixerClone(s32);

extern s32 func_00160958(s32, s32, u8 *, s32);

extern void effBattleUpdateSelectedValue(u8 *, s32);

extern void func_00160D88(u8 *);

/* Start from the selected-unit SYSEFF source, then update through its duration.
 * Return one for an ineligible unit or expiry, zero while updating. */
u32 btlUpdateSelectedUnitEffect(u32 *arguments) {
    u8 *unit = (u8 *)arguments[0];
    u8 *context;
    SoundResourceNode *work;
    s32 handle;

    if ((*(u32 *)(unit + 0x110) & 2) == 0) {
        return 1;
    }
    context = (u8 *)btlGetRuntime();
    if (arguments[2] == 0) {
        work = ((BtlActorWork *)context)->soundResourceSlots[BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT];
        handle = work->sourceHandle;
        *(u32 *)(unit + 0x110) |= 0x80;
        arguments[1] = sndMixerClone(handle);
        arguments[2] = func_00160958(arguments[1], 2, unit, 0);
        arguments[3] = 0xE;
        *(u16 *)(arguments[2] + 0xC) &= 0xFFF9;
        effBattleUpdateSelectedValue((u8 *)arguments[2], 0xE);
        *(u32 *)(unit + 0x110) &= ~8;
        if (*(u32 *)(unit + 0x110) & 2) {
            u8 *ext = *(u8 **)(unit + 0x320);
            *(u32 *)(*(u8 **)(ext + 0x8C)) |= 1;
        }
    }
    arguments[4] = arguments[4] + 1;
    if ((s32)arguments[4] >= (s32)arguments[3]) {
        *(u32 *)(unit + 0x110) = (*(u32 *)(unit + 0x110) & ~0x80) | 0x40;
        return 1;
    }
    func_00160D88((u8 *)arguments[2]);
    return 0;
}

extern u32 btlUpdateSelectedUnitEffect(u32 *);

void btlFinishSelectedUnitEffect(u32 *arguments) {
    u32 value = arguments[2];
    if (value != 0) {
        effReleaseBattleVoiceOwner(value);
    }
    if (arguments[1] != 0) {
        sndReleaseAllVoices(arguments[1]);
    }
    btlClearUnitDefeatCandidate(arguments[0]);
    *(u32 *)(arguments[0] + 0x110) |= 0x40;
}

u8 *btlCreateSelectedEffectUpdateTask(u8 *owner) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x16;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x4C) = btlUpdateSelectedUnitEffect;
    *(void **)(task + 0x50) = btlFinishSelectedUnitEffect;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = 0;
    arguments[4] = 0;
    arguments[3] = 0;
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
    *(u32 *)((u8 *)task + 0x48) = 0;
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
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->endCondition.kind = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", btlUnitBaseLightTask);

extern u32 btlUnitBaseLightTask(u32 *);

void *btlCreateUnitBaseLightTask(u8 *owner) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlUnitBaseLightTask;
    *(u16 *)(task + 0x20) = 0x21;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    return task;
}

extern char D_003A3CA0[];

extern f32 effMiscRandUnitFloat(void *);

u32 btlStiffenDamageShakeStep(u32 *task) {
    f32 pos[4] __attribute__((aligned(16)));
    f32 scale;
    s32 node;

    if (!(*(u32 *)((u8 *)*task + 0x110) & 2)) {
        return 1;
    }
    if (*(s32 *)(task + 2) == 0) {
        node = mdlGetNodeField2C(*(s32 *)(*(s32 *)((u8 *)*task + 0x320) + 0x8C), 0);
        if (node < 0x1D) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)((u8 *)*task + 0xC4),
                                                 *(s32 *)((u8 *)*task + 0xC8));
            if (*(s16 *)(resource + node * 20 + 0x30) == 2) {
                func_001D74B8(*(s32 *)task);
                btlBossDebugPrintf(D_003A3CA0);
            }
        }
    }
    if (0.5f < *(f32 *)(task + 1)) {
        scale = *(f32 *)(task + 1) * (effMiscRandUnitFloat(effSharedRandomState) * 0.5f + 0.5f);
        if (*(s32 *)(task + 2) & 1) {
            scale = -scale;
        }
        if (*(u64 *)((u8 *)*task + 0x110) & 0x808000000000) {
            effObjFetchInnerFirstVec(*(u32 *)((u8 *)*task + 0x31C));
            VU_STORE10(pos);
            pos[0] += scale;
        } else {
            func_001D6300((u8 *)*task, pos);
            pos[0] += scale;
            pos[2] += *(f32 *)((u8 *)*task + 0x88);
        }
        effObjSetInnerFirstVec(*(s32 *)((u8 *)*task + 0x31C), pos);
        *(f32 *)(task + 1) *= 0.85f;
    } else {
        if (*(u64 *)((u8 *)*task + 0x110) & 0x808000000000) {
            effObjFetchInnerFirstVec(*(u32 *)((u8 *)*task + 0x31C));
            VU_STORE10(pos);
        } else {
            func_001D6300((u8 *)*task, pos);
            pos[2] += *(f32 *)((u8 *)*task + 0x88);
        }
        effObjSetInnerFirstVec(*(s32 *)((u8 *)*task + 0x31C), pos);
        return 1;
    }
    *(s32 *)(task + 2) += 1;
    return 0;
}

extern u32 btlStiffenDamageShakeStep(u32 *);

u8 *btlCreateStiffenDamageShakeTask(u8 *owner, f32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x1D;
    *(void **)(task + 0x4C) = btlStiffenDamageShakeStep;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    *(f32 *)(arguments + 1) = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001D9C28);

extern void func_001D9C28();

u8 *func_001D9E48(u8 *arg0) {
    u8 *task = btlAllocTask(0x10);
    u32 *data;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9C28;
    *(u16 *)(task + 0x20) = 0x1E;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)owner;
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

typedef struct GunLoadUnit {
    u8 pad_00[0x118];
    u32 resourceFlags;
    u8 pad_11C[0x1F0];
    void *gunResource;
} GunLoadUnit;

typedef struct GunLoadArgs {
    GunLoadUnit *unit;
    s32 handle;
} GunLoadArgs;

u32 btlPollGunLoad(u32 *arg) {
    GunLoadArgs *args = (GunLoadArgs *)arg;
    GunLoadUnit *unit = args->unit;
    if (args->handle == 0) {
        return 1;
    }
    if (fileIsRequestReadyInCurrentMode(args->handle) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:gun & finish load end[%p]\n", args->handle);
    unit->gunResource = sdfResourceRetainAddress(fileGetResourceHandle(args->handle));
    filePollEntryCleanup(args->handle);
    unit->resourceFlags = (unit->resourceFlags & ~4) | 8;
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->endCondition.kind = 0;
    return task;
}

extern void btlCreateUnitTransparency(u8 *);

extern s32 btlCreateActorTransparency(u32 *);

s32 btlCreateActorTransparency(u32 *arguments) {
    u8 *actor = (u8 *)arguments[0];
    if ((*(u32 *)(actor + 0x110) & 2) == 0) {
        return 0;
    }
    btlCreateUnitTransparency(actor);
    *(u32 *)((u8 *)arguments[0] + 0x110) |= 0x20000;
    return 1;
}

void *btlCreateActorTransparencyTask(u32 actor) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x25;
    *(void **)(task + 0x4C) = btlCreateActorTransparency;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = actor;
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)actor;
    arguments[1] = target;
    arguments[2] = index;
    arguments[4] = value;
    *(f32 *)(arguments + 5) = scale;
    arguments[3] = -1;
    arguments[6] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DA468);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DA780);

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
    unit->owner = btlAdvanceRuntimeSequenceCounter();
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->lookupId = unit->unk2F0 = -1;
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
        unit->nextActor = work->actorList;
    } else {
        unit->nextActor = 0;
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
        next = unit->nextActor;
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

/* Allocate a 12-byte native header followed by capacity words. */
void *btlAllocateIndexList(s32 capacity) {
    u8 *list = sdfAllocAndClearQuadwords(capacity * 4 + 12);
    ((BtlIndexList *)list)->capacity = capacity;
    ((BtlIndexList *)list)->entries = (u32 *)(list + 12);
    ((BtlIndexList *)list)->count = 0;
    return list;
}

void btlFreeIndexList(u32 ptr) {
    sdfReleaseChipBlock(ptr);
}

/* Append one word; the caller must keep the live count within the allocated capacity. */
void btlAppendIndexListEntry(s32 list, u32 entry) {
    s32 index;

    index = ((BtlIndexList *)list)->count;
    ((BtlIndexList *)list)->count = index + 1;
    ((BtlIndexList *)list)->entries[index] = entry;
}

/* Clear the live count without changing storage or its existing entries. */
void btlClearIndexList(s32 list) {
    ((BtlIndexList *)list)->count = 0;
}

/* Return the number of live entries, not the allocated capacity. */
u32 btlGetIndexListCount(s32 list) {
    return ((BtlIndexList *)list)->count;
}

/* Return one stored word; the caller supplies an in-range index. */
u32 btlGetIndexListEntry(s32 list, s32 index) {
    return ((BtlIndexList *)list)->entries[index];
}

void btlCopyIndexList(s32 destination, s32 source) {
    u32 count;
    u32 index;

    btlClearIndexList(destination);
    count = btlGetIndexListCount(source);
    for (index = 0; index < count; index++) {
        btlAppendIndexListEntry(destination, btlGetIndexListEntry(source, index));
    }
}

/* Swap two stored words without changing the live count. */
void btlSwapIndexListEntries(s32 list, s32 firstIndex, s32 secondIndex) {
    s32 *entries;
    s32 first;
    s32 second;

    if (firstIndex == secondIndex) {
        return;
    }
    entries = (s32 *)((BtlIndexList *)list)->entries;
    first = entries[firstIndex];
    second = entries[secondIndex];
    entries[firstIndex] = second;
    entries[secondIndex] = first;
}

u32 btlFindListIndex(s32 arg0, s32 arg1) {
    u32 count = btlGetIndexListCount(arg0);
    u32 index;

    for (index = 0; index < count; index++) {
        if (arg1 == btlGetIndexListEntry(arg0, index)) {
            return index;
        }
    }
    return -1;
}

extern void mdlStoreTertiaryVectorVU(s32);
extern void mdlSetAmountOnAllContextResources(f32, s32);

void btlApplyUnitEffectScale(BtlUnit *unit) {
    BtlEffObjInner *inner;
    if (unit->flags & 2) {
        btlInitializeEffectVectorsFromSourceRecords((BtlFx *)unit, unit->resourceKind, unit->resourceIndex);
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->effectScale, "vmulx.xyzw vf10, vf10, vf2x");
        inner = ((BtlEffObj *)unit->effectObject)->inner;
        inner->flagsC0 |= 1;
        inner->flagsC0 &= ~2;
        VU0_STORE_VF(vf10, inner->vec60);
        mdlStoreTertiaryVectorVU(*(s32 *)(unit->ext + 0x8C));
        mdlSetAmountOnAllContextResources(unit->effectScale, *(s32 *)(unit->ext + 0x8C));
        btlSetUnitPosition((u8 *)unit, (u8 *)unit + 0x60);
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
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(vec) : "memory");
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DB370);

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 func_001F7D80(u8 *, f32);

extern void func_001DB370(u8 *, u8 *, u8 *, f32);

s32 btlStepPoseBlendHalf(u8 *object) {
    f32 blend;
    if (*(u32 *)(object + 0x110) == 0) {
        *(f32 *)(object + 0x128) = 0.0f;
        btlScalarRangeInitQuadratic(object + 0x13C, (f32)(*(s32 *)(object + 0x12C) * 2));
        btlCopyMotionTransform(object, object + 0x30);
        return 0;
    }
    blend = func_001F7D80(object + 0x13C, 1.0f);
    if (blend > 0.5f) {
        blend = 0.5f;
    }
    func_001DB370(object, object + 0x30, object + 0xC0, 2.0f * blend);
    *(f32 *)(object + 0x128) = blend;
    if (blend >= 0.5f) {
        return 1;
    }
    return 0;
}

extern void btlScalarRangeSetStartClearEnd(u8 *, f32);

extern f32 func_001F7CD8(u8 *);

extern void btlCopyMotionTransform(u8 *, u8 *);

extern void func_001DB370(u8 *, u8 *, u8 *, f32);

s32 btlStepPoseBlend(u8 *actor) {
    u8 *motion = actor + 0x134;
    u8 *position = actor + 0x30;
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        btlScalarRangeSetStartClearEnd(motion, *(f32 *)(actor + 0x130));
        btlCopyMotionTransform(actor, position);
    }
    value = func_001F7CD8(motion);
    func_001DB370(actor, position, actor + 0xC0, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 func_001F7D80(u8 *, f32);

s32 btlStepPoseBlendFrame(u8 *actor) {
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        *(u32 *)(actor + 0x128) = 0;
        btlScalarRangeInitQuadratic(actor + 0x13C, (f32)*(s32 *)(actor + 0x12C));
        btlCopyMotionTransform(actor, actor + 0x30);
        return 0;
    }
    value = func_001F7D80(actor + 0x13C, 1.0f);
    func_001DB370(actor, actor + 0x30, actor + 0xC0, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

s32 btlStepPoseBlendRatio(u8 *actor) {
    f32 ratio = (f32)*(s32 *)(actor + 0x110) / (f32)*(s32 *)(actor + 0x12C);
    if (ratio <= 1.0f) {
        func_001DB370(actor, actor + 0x30, actor + 0xC0, ratio);
        return 0;
    }
    btlCopyMotionTransform(actor, actor + 0xC0);
    return 0;
}

typedef struct CameraPoseTransform {
    f32 vec0[4];
    f32 vec1[4];
    f32 distance;
    f32 fov;
} CameraPoseTransform;

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
        vector[0] = state->vec1[0] * scale + state->vec0[0];
        y = state->vec0[1];
        vector[1] = state->vec1[1] * scale + y;
        vector[2] = state->vec1[2] * scale + state->vec0[2];
        vector[3] = 0.0f;
        if (-20.0f < vector[1]) {
            VU0_LOAD_VF(vf10, state->vec0);
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
            VU0_STORE_VF(vf10, state->vec1);
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
        vector[0] = state->vec1[0] * scale + state->vec0[0];
        y = state->vec0[1];
        vector[1] = state->vec1[1] * scale + y;
        vector[2] = state->vec1[2] * scale + state->vec0[2];
        vector[3] = 0.0f;
        if (height < vector[1]) {
            VU0_LOAD_VF(vf10, state->vec0);
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
            VU0_STORE_VF(vf10, state->vec1);
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
        *(u64 *)((u8 *)task + 0x40) = *(u64 *)(*(s32 *)(owner + 0x18) + 0x108);
    }
    task->callback.update = (void (*)(void))btlExecuteCommandSoundTask;
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = owner;
    arguments[3] = variant;
    arguments[1] = 0;
    arguments[2] = 0;
    arguments[4] = 0;
    return task;
}

void *btlCreateTargetedCommandSoundTask(s32 owner, s32 variant, u32 target) {
    void *task = btlCreateCommandSoundTask(owner, variant);
    u32 *arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[4] = target;
    return task;
}

void *btlCreateCommandSoundWithArguments(s32 owner, s32 variant, u32 first, u32 second, u32 third) {
    void *task = btlCreateCommandSoundTask(owner, second);
    u32 *arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    args = (f32 *)btlGetTaskArguments((s32)task);
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
    args = (f32 *)btlGetTaskArguments((s32)task);
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
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->endCondition.kind = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DBE68);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DC0E8);

void btlClearPendingSoundList(void) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x188);
    if (actor != 0) {
        btlFreeIndexList(actor);
        *(s32 *)(context + 0x188) = 0;
    }
    *(u32 *)(context + 0x1F4) &= ~0x10;
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
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(direction));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_0037E110));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddz.xyzw vf10, vf30, vf10z\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x10) : "memory");
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(1.0f));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(origin));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object) : "memory");
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

s32 btlIsUnitInActiveList(s32 unit) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *slot = *(u8 **)(work + 0x164);
    u32 count;
    u32 i;
    if (slot != 0 && *(s32 *)(slot + 0x18) == unit) {
        return 1;
    }
    count = btlGetIndexListCount(*(s32 *)(work + 0x188));
    for (i = 0; i < count; i++) {
        if (btlGetIndexListEntry(*(s32 *)(work + 0x188), i) == unit) {
            return 1;
        }
    }
    return 0;
}

void btlResetActiveUnitList(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x164) = 0;
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x400;
    btlClearIndexList(*(u32 *)(temp_v0 + 0x188));
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


typedef struct WorldMotionData {
    u8 pad0[0x20];
    s32 unk20;
} WorldMotionData;

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

extern void *dds3GetWorldObject(void);

extern WorldMotionData *dds3GetWorldCameraObject(void *);

extern void dds3SetWorldCameraObject(void *, s32);

extern f32 dds3GetCameraFieldOfView(s32);

extern void func_00106488(f32);
void btlRefreshWorldCameraHandle(void) {
    WorldObj *object;
    s32 handle;
    if (((BattleController *)btlGetRuntime())->flags & 2) {
        object = dds3GetWorldObject();
        if (object != NULL) {
            handle = (s32)dds3GetWorldCameraObject(object);
            if (handle != 0) {
                if (((WorldMotionData *)handle)->unk20 != 0) {
                    handle = ((WorldMotionData *)handle)->unk20;
                } else {
                    handle = object->head->sub->handle;
                }
                dds3SetWorldCameraObject(object, handle);
                func_00106488(dds3GetCameraFieldOfView(handle));
            }
        }
    }
}

extern s32 D_003BB668;

extern s32 D_003BB664;

s32 btlGetWorldObjectDefault(void) {
    WorldMotionData *data;
    s32 *value;
    if (!(((BattleController *)btlGetRuntime())->flags & 2)) {
        return D_003BB668;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return D_003BB668;
    }
    value = (s32 *)((u8 *)data + 8);
    if (*value == 0) {
        return D_003BB664;
    }
    return *value;
}

s32 btlIsWorldMotionIdle(void) {
    s32 context = btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 2) == 0) {
        return 0;
    }
    {
        s32 state = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (state == 0) {
            return 0;
        }
        return *(s32 *)(state + 8) == 0;
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
    u32 count = btlGetIndexListCount(*(s32 *)(resource + 0x118));
    u32 index;
    for (index = 0; index < count; index++) {
        u8 *actor = (u8 *)btlGetIndexListEntry(*(s32 *)(resource + 0x118), index);
        flags |= *(u32 *)(actor + 0x110) & 0x600;
    }
    if (flags != 0) {
        btlFlagMatchingUnitsDefeatCandidate(flags);
    }
}

extern f32 D_00359EC0[];
extern char D_003A3DD0[];
extern s32 mdlGetNodeField2C(s32, s32);

extern void sdfMotionSampleAtFrame(void *, f32);

void btlResetCameraMotion(s32 action) {
    BtlCameraResetWork *work = (BtlCameraResetWork *)btlGetRuntime();
    BtlUnit *unit;
    f32 current;
    f32 limit;

    if (work->activeUnitId == 1 || btlHasSingleLinkedResource(action) != 0) {
        unit = work->actorList;
        if (unit != 0) {
            f32 fallbackScale = 0.7f;
            for (; unit != 0; unit = unit->nextActor) {
                if (unit->flags & 1) {
                    if (unit->flags & 0x200) {
                        if (unit->flags & 2) {
                            if (unit->ext != 0) {
                                s32 node = mdlGetNodeField2C(
                                    (s32)((BtlCameraResetResource *)unit->ext)->model, 0);
                                if (node == 0xD || node == 0x12) {
                                    current = btlGetUnitModelValue1C((s32)unit);
                                    limit = (f32)btlGetUnitModelFrameCount((s32)unit);
                                    if (((BtlCameraResetUnit *)unit)->mode < 8) {
                                        limit = limit * D_00359EC0[((BtlCameraResetUnit *)unit)->mode];
                                    } else {
                                        limit = limit * fallbackScale;
                                    }
                                    if (current < limit) {
                                        sdfMotionSampleAtFrame(
                                            ((BtlCameraResetModel *)
                                             ((BtlCameraResetResource *)unit->ext)->model)->motion,
                                            limit);
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
typedef struct BtlDebugWorldTransform {
    u8 pad00[0x40];
    f32 position[3];
    u8 pad4C[4];
    f32 rotation[4];
} BtlDebugWorldTransform;

typedef struct BtlDebugWorldObject {
    u8 pad00[0x1C];
    BtlDebugWorldTransform *transform;
} BtlDebugWorldObject;

extern char D_003A3DF0[];
extern char D_003A3E08[];
extern void btlBossDebugPrintfN(s32, s32, s32, s32, ...);

void btlDebugPrintWorldTransform(s32 arg0, u8 *arg1) {
    BtlDebugWorldObject *object;

    if (((BattleController *)btlGetRuntime())->flags & 2) {
        object = (BtlDebugWorldObject *)dds3GetWorldCameraObject(dds3GetWorldObject());
        if (object != 0) {
            btlBossDebugPrintfN(arg0, (s32)arg1, 0, (s32)D_003A3DF0,
                                (double)object->transform->position[0],
                                (double)object->transform->position[1],
                                (double)object->transform->position[2]);
            btlBossDebugPrintfN(arg0, (s32)(arg1 + 0xC), 0, (s32)D_003A3E08,
                                (double)object->transform->rotation[0],
                                (double)object->transform->rotation[1],
                                (double)object->transform->rotation[2],
                                (double)object->transform->rotation[3]);
        }
    }
}

extern void func_001F6E28(s32, s32, s32);

void btlFaceActionParticipantsTowardLinkedTarget(ActionUnit *action) {
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
    u32 count = btlGetIndexListCount(*(s32 *)(action + 0x118));
    if (count != 0) {
        target = *(u8 **)(*(s32 *)(action + 0xF4) + 0x18);
        if (count == 1) {
            btlUnitGetMuzzlePosVU((void *)btlGetIndexListEntry(*(s32 *)(action + 0x118), 0));
        } else {
            func_001F6E28(*(s32 *)(action + 0x118), 0, 0);
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
    linked = ((ActionUnit *)actor)->link;
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
    if (*(u16 *)((u8 *)linked->unit + 0x12E) & 0x480) {
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
    BattleActionLinkState *resource = ((ActionUnit *)object)->link;
    u32 count;
    u32 index;
    BtlOperandGroup *entry;
    if (resource == 0) {
        return 0;
    }
    count = btlGetIndexListCount(resource->actorIndices);
    entry = resource->groups;
    for (index = 0; index < count; index++, entry++) {
        if (entry->unk10 != 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlGetRuntime(void);

extern f32 btlUnitGetTopY(s32);

s32 btlCheckActorDistanceLimit(void) {
    s32 actor = *(s32 *)(btlGetRuntime() + 0x228);

    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY(actor) > 400.0f) {
                    return 0;
                }
            }
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 1;
}

extern f32 func_001F66D8(s32, s32, s32);

s32 btlIsEntryHeightWithinLimit(void) {
    if (func_001F66D8(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

/* Find an unmarked linked kind-two slot whose unit is not disabled. */
s32 btlHasIdleLinkedSlotKindTwo(u8 *actor) {
    BattleActionLinkState *linked = ((ActionUnit *)actor)->link;
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
        if (entry->unk10 == 0 && entry->unk08 == 1 && entry->unk0C == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(linked->actorIndices, i))->flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

/* Find a type-two linked group whose associated unit is not disabled. */
s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor) {
    BattleActionLinkState *linked = ((ActionUnit *)actor)->link;
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
        if (entry->unk08 == 2 &&
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
    return btlGetIndexListCount(*(s32 *)(actor + 0x118)) == 1;
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
    s32 entry;

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
    if (btlGetIndexListCount(*(s32 *)(linked + 0x60)) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(*(s32 *)(linked + 0x60), 0);
    return ((*(u32 *)(*(s32 *)(linked + 0x18) + 0x110) ^ *(u32 *)(entry + 0x110)) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(u8 *node) {
    u8 *resource = *(u8 **)(node + 0xF4);
    u8 *actor;
    u32 id;
    if (resource == 0) return 0;
    if (btlGetIndexListCount(*(s32 *)(resource + 0x60)) >= 2) return 0;
    actor = (u8 *)btlGetIndexListEntry(*(s32 *)(resource + 0x60), 0);
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DD4B8);

void func_001DD678(void) {
}

void func_001DD680(u32 arg0) {
    func_001DF358(arg0, arg0);
}

void func_001DD698(void) {
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DD6A0);

extern s32 func_001E2970(s32, s32);
extern s32 func_001E2D20(s32);
extern void btlAdvanceCursorForUnmarkedUnit(s32, s32);

void func_001DD7E8(s32 actor) {
    s32 (*callback)(s32) = *(s32 (**)(s32))(btlGetRuntime() + 0x614);

    if (callback != 0 && callback(actor) != 0) {
        return;
    }

    switch (*(u16 *)(actor + 0x10C)) {
    case 9:
        func_001E2970(actor, actor);
        break;
    case 10:
        func_001E2D20(actor);
        break;
    case 11:
        btlAdvanceCursorForUnmarkedUnit(actor, actor);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DD890);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D38);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D50);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D60);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D70);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D80);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3D90);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3DD0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3DF0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3E08);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DDE28);

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

extern void func_001E3E58(ActionUnit *, u8 *, BtlUnit *, s32);

extern void btlPrepareUnitPoseWithTiltRotation();

void btlUpdateActionPoseForLinkedTarget(ActionUnit *action) {
    BtlUnit *target;

    if (((BtlActionPoseRuntime *)btlGetRuntime())->flags & 1) {
        target = action->link->unit;
        if (target->flags & 0x400) {
            func_001DF358((s32)action, (s32)action);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform((u8 *)action + 0x30, (u8 *)action);
        func_001E3E58(action, (u8 *)action + 0xC0, action->link->unit, 0);
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

extern void func_001E0DA0(ActionUnit *, f32 *, s32, f32, f32);

void func_001DE960(ActionUnit *action) {
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
    func_001E0DA0(action, (f32 *)action + 12, kind, 45.0f, 0.25f);
    func_001E0DA0(action, (f32 *)((u8 *)action + 0xC0), kind, 1.0f, 0.5f);
    ((BattlePoseBlendState *)action)->duration = 30.0f;
    action->flags |= 0x41;
}

void func_001DEA68(void) {
}

void btlStartLinkedActionPoseBlendIfEligible(ActionUnit *action) {
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
        btlPrepareUnitPoseWithTiltRotation(action, (f32 *)((u8 *)action + 0x30), (u8 *)action + 0xC0);
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
    func_001E4AC0(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

void func_001DEE38(void) {
    func_001E4E50();
}

void btlStartLinkedDefeatCandidateAction(u8 *actor) {
    u8 *resource = *(u8 **)(actor + 0xF4);
    btlAppendIndexListEntry(*(s32 *)(actor + 0x118), *(s32 *)(resource + 0x18));
    func_001E5198(actor, actor + 0x30, actor + 0xC0);
    btlClearAllUnitDefeatCandidates();
    resource = *(u8 **)(actor + 0xF4);
    btlFlagUnitDefeatCandidate(*(s32 *)(resource + 0x18));
    *(f32 *)(actor + 0x130) = 50.0f;
    *(u32 *)(actor + 0xF0) |= 0x41;
}

void func_001DEEC0(void) {
}

void btlUpdateLinkedActionEffectVectorByTarget(u32 arg0) {
    if ((*(u32 *)(*(s32 *)(*(s32 *)((s32)arg0 + 0xf4) + 0x18) + 0x110) & 0x200) != 0) {
        func_001E4960(arg0, arg0);
        return;
    }
    if (*(s32 *)((s32)arg0 + 0x108) != 0x10) {
        btlResetUnitEffectVector(arg0, arg0);
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

typedef struct CameraPoseLink {
    u8 pad_00[0x18];
    BtlUnit *unit;
} CameraPoseLink;

typedef struct CameraPoseAction {
    CameraPoseTransform transform;
    u8 pad_028[0xC8];
    u32 flags;
    CameraPoseLink *link;
    u8 pad_0F8[0x38];
    f32 cameraPreset;
} CameraPoseAction;

extern s32 func_001D6428(u8 *, s32);
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
        VU0_STORE_VF(vf10, from->vec0);
        if (func_001D6428((u8 *)unit, 1) == 0) {
            btlUnitGetMuzzlePosVU((u8 *)unit);
        }
        VU0_STORE_VF(vf10, to->vec0);
        VU0_LOAD_VF(vf11, from->vec0);
        VU0_LERP_VF10(0.5f);
        VU0_STORE_VF(vf10, from->vec0);
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
        VU0_STORE_VF(vf10, from->vec1);
        VU0_LOAD_VF(vf10, &poses[pose][4]);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_0037E110);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, to->vec1);
        func_001DB698(from);
        func_001DB698(to);
        action->cameraPreset = poses[pose][9];
        action->flags |= 0x41;
    }
}

void btlAimEffectPoseAtUnit(u8 *actor) {
    u8 *object = *(u8 **)(*(u8 **)(actor + 0xF4) + 0x18);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        if (btlSetActorEffectParameter(object, 1) == 0) {
            btlUnitGetMuzzlePosVU(object);
        }
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(actor + 0xC0));
        func_001DB698((CameraPoseTransform *)(actor + 0xC0));
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001DFAE0);

void btlRefreshActionPoseBlendSnapshot(u8 *action) {
    u8 *saved;
    if (!(*(u32 *)(action + 0xF0) & 1) && *(s32 *)(action + 0x11C) == 0) {
        saved = action + 0xC0;
        btlCopyMotionTransform(action + 0x30, action);
        btlCopyMotionTransform(saved, action);
        *(f32 *)(action + 0xE0) += 125.0f;
        *(u32 *)(action + 0xF0) = (*(u32 *)(action + 0xF0) & ~0x14) | 0x41;
        *(s32 *)(action + 0x110) = 0;
        *(s32 *)(action + 0x11C) = 1;
        *(f32 *)(action + 0x130) = 40.0f;
        func_001DB698((CameraPoseTransform *)saved);
    }
}
INCLUDE_ASM(const s32, "game/code_001C8890", func_001DFE28);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E0100);

extern void btlClearAllUnitDefeatCandidates(void);
extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlSetupCameraPoseAimUnit(ActionUnit *action, BtlCameraPose *from, BtlCameraPose *to) {
    f32 quat[4];
    BtlUnit *unit = action->link->unit;
    f32 fov;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->flags & 0x600);
    btlCopyUnitRotationQuaternion((u8 *)unit, quat);
    fov = ((BtlCameraPose *)action)->fov;
    from->fov = fov;
    if (func_001D6428((u8 *)unit, 1) == 0) {
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
        s32 actorIndices;
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E2578);

void btlFlagUserAndTargetDefeat(u8 *command, u8 *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = *(BtlUnit **)(*(s32 *)(command + 0xF4) + 0x18);
    target = (BtlUnit *)btlGetIndexListEntry(*(u32 *)(command + 0x118), 0);
    if (!(user->flags & target->flags & 0x600)) {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate((u8 *)user);
        btlFlagMatchingUnitsDefeatCandidate(target->flags & 0x600);
    } else {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate((u8 *)user);
        btlFlagUnitDefeatCandidate((u8 *)target);
    }
    btlUnitGetMuzzlePosVU(user);
    VU_STORE10(userPos);
    btlUnitGetMuzzlePosVU(target);
    VU_STORE10(targetPos);
    btlUnitFaceTarget((u8 *)target, (u8 *)user);
    if (userPos[0] < targetPos[0]) {
        *(u32 *)(command + 0xF0) |= 0x200;
    } else {
        *(u32 *)(command + 0xF0) &= ~0x200;
    }
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E2970);

void btlSetupActionCameraPair(ActionUnit *command) {
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
    func_001E1CF8(command, command->pos30);
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
    btlInterpolateVectorStep(command->pos30);
    VU0_STORE_VF_UNCLOBBERED(vf10, lookPos);
    btlUnitGetMuzzlePosVU(user);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, command->pos30);
    VU0_LERP_VF10(0.25f);
    VU0_STORE_VF(vf10, command->pos30);
    VU0_LOAD_VF(vf11, lookPos);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, command->dir40);
    btlUnitFaceTarget((u8 *)user, (u8 *)target);
}
INCLUDE_ASM(const s32, "game/code_001C8890", func_001E2D20);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E2FF8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E3310);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E37B0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E3920);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E3E58);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4180);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4578);

extern void btlPrepareUnitPoseWithTiltRotation();

void func_001E4708(void) {
    btlPrepareUnitPoseWithTiltRotation();
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4720);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4960);

void btlResetUnitEffectVector(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4AC0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E4E50);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E5198);

void func_001E5460(u32 arg0) {
    btlFlagUserAndTargetDefeat(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E5478);

void func_001E5700(u32 arg0) {
    func_001E5460(arg0);
}

void func_001E5718(void) {
    func_001E5478();
}

extern void func_001E0718(u8 *action, u8 *pose, u8 *out);

void btlChooseActionPoseBlendFromActorCount(u8 *action) {
    BtlActorWork *work = (BtlActorWork *)btlGetRuntime();
    u32 count;
    u32 mask;
    BtlUnit *unit;

    mask = ((BtlUnit *)btlGetIndexListEntry(*(s32 *)(action + 0x118), 0))->flags & 0x600;
    count = 0;
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & mask) {
                count++;
            }
        }
    }
    if (count >= 2) {
        func_001E0718(action, action + 0x30, action + 0xC0);
        return;
    }
    btlPrepareUnitPoseWithTiltRotation(action, (f32 *)(action + 0x30), action + 0xC0);
    *(f32 *)(action + 0x130) = 200.0f;
    *(u32 *)(action + 0xF0) |= 0x41;
}
void func_001E57F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A3FC0);

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

INCLUDE_ASM(const s32, "game/code_001C8890", btlAdvancePlayerCursorAnimation);
INCLUDE_ASM(const s32, "game/code_001C8890", func_001E6180);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E6260);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E6368);


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
        VU_STORE10(pos);
        break;
    case 0:
    default:
        btlUnitGetMuzzlePosVU(unit);
        VU_STORE10(pos);
        break;
    }
    VU_LOAD10(pos);
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4668);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A46F8);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E6BB0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001E9DE0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EB1B0);

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
    BtlAdvanceCursor *cursor = (BtlAdvanceCursor *)D_0035F100;

    if (cursor->unk_00 == 1) {
        if (!(((BtlAction *)action)->link->unit->flags & 0x400)) {
            func_001E9DE0(action, state, D_0035DAE0[cursor->index]);
            func_001EB368(action, state);
            func_001EB1B0(action, state, 0, 1);
            cursor->frame++;
            cursor->frame = cursor->frame <= 0 ? 0 :
                cursor->frame >= 0x7FFF ? 0x7FFE : cursor->frame;
        }
    }
}
extern u8 D_0035F100[];

void btlClearCommandCursorAndRunAction(s32 actor) {
    memset(D_0035F100, 0, 0x130);
    btlFlagUserAndTargetDefeat(actor, actor);
}

extern s32 D_0035DAF0[];

INCLUDE_ASM(const s32, "game/code_001C8890", btlAdvanceCommandCursorOrAction);
INCLUDE_ASM(const s32, "game/code_001C8890", btlInitCommandCursorForCategory);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EEED8);

void btlInitCommandCursorForFirstActor(s32 arg0, s32 arg1) {
    BattleController *work = (BattleController *)btlGetRuntime();
    s32 first = btlGetIndexListEntry(*(s32 *)(arg0 + 0x118), 0);
    memset(CURSOR, 0, 0x130);
    func_001D74B8(first);
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
        func_001EB1B0(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}
extern void func_001D74B8(s32);

extern void func_001E6BB0(s32, s32, s32, s32);

extern void func_001E6668(s32, s32, s32, s32);

void btlInitLinkedUnitActionCursor(u8 *arg0) {
    s32 context = btlGetRuntime() + 0x70;
    *(u8 **)(context + 0xF4) = arg0;
    memset(CURSOR, 0, 0x130);
    CURSOR->unk_0A = 0;
    CURSOR->unk_0E = 0;
    func_001D74B8(*(s32 *)(arg0 + 0x18));
    func_001E6BB0(context, context, 6, 0);
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(*(s32 *)(arg0 + 0x18));
    CURSOR->unk_0C = 0;
    func_001E6668(context, context, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EF1F8);

extern void func_001E6BB0(s32, s32, s32, s32);

extern void func_001E6668(s32, s32, s32, s32);

extern s32 func_001EB1B0(s32, s32, s32, s32);

void btlInitCursorAndApplyAction(s32 actor, s32 target) {
    memset(D_0035F100, 0, 0x130);
    func_001E6BB0(actor, target, 2, 6);
    func_001E6668(actor, target, 0, 0);
    func_001EB1B0(actor, target, 0, 1);
    *(u16 *)(D_0035F100 + 0xC) = 2;
}

void btlAdvanceWorldCounterAndSpawnActionObject(void) {
    u64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v1 = btlGetRuntime();
    temp_v0 = dds3AdvanceWorldCounter();
    temp_v2 = evtSpawnActionObj9(temp_v0);
    *(u32 *)(temp_v1 + 0x204) = temp_v2;
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

typedef struct BattleFieldBlocks {
    u8 unk_00[0x290];
    s32 fieldF1;
    s32 fieldF2;
    s32 fieldF3;
} BattleFieldBlocks;

void btlFreeFieldBlocks(void) {
    BattleFieldBlocks *context = (BattleFieldBlocks *)btlGetRuntime();
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
    context = (BattleFieldBlocks *)btlGetRuntime();
    *(u32 *)((u8 *)context + 0x1F4) &= ~2;
}

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

extern void fldApplyLightSetCurrent(void);

void btlInitializeSceneLightingAndTint(void) {
    u8 *context = (u8 *)btlGetRuntime();
    fldApplyLightSetCurrent();
    btlInitTintTransitionResource(0x80, 0);
    VU_LOAD10((u8 *)D_00324770[0] + 0x10);
    VU_STORE10(context + 0x10);
    VU_STORE10(context + 0x40);
    VU_LOAD10(D_00324770[0]);
    VU_STORE10(context + 0x20);
    VU_STORE10(context + 0x50);
    VU_LOAD10(kwlnDefaultColorVector);
    VU_STORE10(context + 0x30);
    VU_STORE10(context + 0x60);
    *(u32 *)(context + 0x698) = 0x807E5C5E;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EF6B8);

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
    func_00113E40(1);
}

extern f32 *D_00324770[];

void btlUpdateTintAndWorldLight(void) {
    u8 *context = (u8 *)btlGetRuntime();
    f32 *position = D_00324770[0];

    if (position[0] == 0.0f && position[1] == 0.0f &&
        position[2] == 0.0f) {
        func_00113E40(0);
    } else {
        func_00113E40(1);
    }
    if (*(u32 *)(context + 0x1F8) & 0x20) {
        func_00113E40(0);
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

typedef struct BattleWorldTransitionWork {
    u8 pad00[0x204];
    s32 listener;
} BattleWorldTransitionWork;

struct WorldTransformOwner;
struct WorldUnitOwner;
extern WorldTransformSetup D_003D74A0;
extern void dds3LoadWorldTransformSetup(struct WorldTransformOwner *, WorldTransformSetup *);
extern void evtBeginUnitValueColorTransition(struct WorldUnitOwner *, s32);

void func_001EFD58(f32 *position, f32 *scale, s32 value) {
    BattleWorldTransitionWork *work = (BattleWorldTransitionWork *)btlGetRuntime();
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
    dds3LoadWorldTransformSetup((struct WorldTransformOwner *)listener, &D_003D74A0);
    evtBeginUnitValueColorTransition((struct WorldUnitOwner *)work->listener, value);
}

extern s32 effCreateSelectionFlagListFromWork(void *work);
extern u8 D_0035F5F8[];

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4AD8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4AF0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B08);

void btlCreateRainEffect(u32 kind, u32 arg) {
    BtlRainWork *work = (BtlRainWork *)btlGetRuntime();

    switch (kind) {
    case 0xDF:
        switch (arg) {
        case 0:
            break;
        case 1:
        case 3:
        case 4:
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(D_0035F5F8);
            break;
        }
        break;
    case 0xE0:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(D_0035F5F8);
        }
        break;
    case 0xE1:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(D_0035F5F8);
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
        s32 context = btlGetRuntime();
        if (*(u32 *)(context + 0x1F8) & 0x20) {
            return;
        }
        if (*(u32 *)(context + 0x58C) != 0) {
            effUpdateAndDrawSelectionEntries(*(u32 *)(context + 0x58C));
        }
    }
}

extern char D_003A4B40[]; /* "btl:rain exit\n" */

void btlStopRainSoundTransition(void) {
    s32 context = btlGetRuntime();
    if (*(u32 *)(context + 0x58C) != 0) {
        btlBossDebugPrintf(D_003A4B40);
        effReleaseSelectionFlagList(*(u32 *)(context + 0x58C));
        *(u32 *)(context + 0x58C) = 0;
    }
}

extern char D_003A4B98[]; /* "/fld/b/f%03d/f%03d_%03df.tmx" */
extern char D_003A4BB8[]; /* "btl:load 0[%s]\n" */
extern char D_003A4BC8[]; /* "/fld/b/f%03d/f%03d_%03ds.tmx" */
extern char D_003A4BE8[]; /* "btl:load 1[%s]\n" */
extern char D_003A4BF8[]; /* "btl:floor load end 0\n" */
extern char D_003A4C10[]; /* "btl:floor load end 1\n" */

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B40);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001EFFA8);

extern u32 func_001EFFA8(u32 *);

s32 fldCreateSceneTileTask(u32 soundId, u32 variant) {
    u8 *task = btlAllocTask(40);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x24) &= ~1;
    *(u16 *)(task + 0x20) = 1;
    *(void **)(task + 0x4C) = func_001EFFA8;
    task[0x10] = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    arguments = (u8 *)btlGetTaskArguments((s32)task);
    *(u32 *)(arguments + 0x30) = value;
    if (source != 0) {
        memcpy(arguments, source, 0x30);
    } else {
        memcpy(arguments, (u8 *)btlGetRuntime() + 0x10, 0x30);
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F06E0);

void *func_001F0920(owner)
    u32 owner;
{
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 4;
    *(u16 *)(task + 0x24) |= 2;
    *(void **)(task + 0x4C) = func_001F06E0;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = owner;
    return task;
}

s64 func_001F0998(void) {
    D_003BB694 = 1;
    return func_001F06E0();
}

SoundTask *btlCreateSoundUpdateTask(void) {
    SoundTask *task = (SoundTask *)func_001F0920();
    task->taskId = 7;
    task->callback.update = func_001F0998;
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
    *(u32 *)((u8 *)task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
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
    *(u32 *)((u8 *)task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)sound;
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

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4B98);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BB8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BC8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BE8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4BF8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A4C10);

void sndCreateSystemEffect(u32 *effect) {
    s32 handle;
    if (!(effect[0] & 8) || effect[4] || effect[1]) {
        return;
    }
    handle = sndMixerClone(effect[5]);
    effect[4] = handle;
    btlBossDebugPrintf("btl:system effect create[%p]\n", handle);
}

void sndDeleteSystemEffect(u32 *effect) {
    if ((effect[0] & 8) && effect[4] && !effect[1]) {
        btlBossDebugPrintf("btl:system effect delete[%p]\n", effect[4]);
        sndReleaseAllVoices(effect[4]);
        effect[4] = 0;
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
    arguments = btlGetTaskArguments((s32)task);
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
    u32 *data = (u32 *)btlGetTaskArguments((s32)task);
    data[4] = (u32)source;
    data[5] = (u32)target;
    return task;
}

void sndStartEffectTask(u32 *task) {
    u32 *effect;
    u32 *source;
    task[1] = 0;
    sndCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    source = (u32 *)task[2];
    ++effect[1];
    ++source[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F1470);

extern u32 func_001F1470(u32 *);

void sndFinishActorEffectTask(s32 *arg0) {
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

u8 *sndCreateActorEffectTask(u32 effect, u8 *owner, u32 channel) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2C;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = sndStartEffectTask;
    *(void **)(task + 0x4C) = func_001F1470;
    *(void **)(task + 0x50) = sndFinishActorEffectTask;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = effect;
    arguments[2] = (u32)owner;
    arguments[3] = channel;
    arguments[1] = 0;
    arguments[4] = 0;
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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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
    args->loadHandle = (void *)fileQueueDefaultCallbackRequest(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

extern char D_003A4C88[];

u32 sndPollEffectLoad(u32 *args) {
    u8 *effect = (u8 *)args[0];
    s32 resource;

    if (*(u32 *)effect & 2) {
        return 1;
    }
    if (!fileIsRequestReadyInCurrentMode(args[1])) {
        return 0;
    }
    btlBossDebugPrintf(D_003A4C88, args[2]);
    resource = fileGetResourceHandle(args[1]);
    *(u32 *)(effect + 0x10) =
        sndMixerClone(sdfResourceRetainAddress(resource));
    sdfReleaseResourceAllocation(resource);
    filePollEntryCleanup(args[1]);
    *(u32 *)effect = (*(u32 *)effect & ~1) | 2;
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
    arguments = btlGetTaskArguments((s32)task);
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
    *(u32 *)btlGetTaskArguments((s32)task) = sound;
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
    *(u32 *)btlGetTaskArguments((s32)task) = sound;
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
    arguments = btlGetTaskArguments((s32)task);
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
    *(u32 *)btlGetTaskArguments((s32)task) = sound;
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
    *(u32 *)((u8 *)task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
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
    *(u32 *)((u8 *)task + 0x48) = 0;
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
    *(u32 *)((u8 *)task + 0x48) = 0;
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

SoundResourceNode *sndCreateResourceNode(u32 soundId) {
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
        --*(s32 *)((u8 *)node->task + 4);
        sndDeleteSystemEffect(node->task);
    }
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F2818);

void btlMarkTaskReady(s32 arg0) {
    *(u8 *)(arg0 + 0x10) = 1;
}

SoundLink *sndAllocLink(void *owner) {
    SoundLink *node = sdfAllocAndClearQuadwords(sizeof(SoundLink));
    node->owner = owner;
    node->sound = 0;
    node->variant = 0;
    node->task = 0;
    return node;
}

void sndFreeLink(SoundLink *node) {
    if (node->sound) {
        effReleaseBattleVoiceOwner(node->sound);
        --*(s32 *)((u8 *)node->task + 4);
        sndDeleteSystemEffect(node->task);
    }
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F2C00);

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
    *(u32 *)((u8 *)task + 0x48) = 0;
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
    *(u32 *)((u8 *)task + 0x48) = 0;
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
            btlCreateIndexedSoundResourceNode(i, D_0035F748[i].resource);
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
void btlCreateIndexedSoundResourceNode(s32 slotIndex, u32 handle) {
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

extern u32 func_002940D0(s32);

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(8);
    SoundBankEntry *entry = &D_0035F748[index];
    node->actor = actor;
    node->handle = func_002940D0(entry->resource);
    return node;
}

void btlUpdateJobPositionFromModel(s32 *args) {
    f32 pos[4];

    if (sdfLoadMapRecordPositionVector(*(s32 *)(args[1] + 0x18), 1) == 0) {
        mdlLoadPrimaryVectorVU(args[1]);
        VU_STORE10(pos);
        pos[1] -= 150.0f;
    } else {
        VU_STORE10(pos);
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F3278);

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
    *(u32 *)btlGetTaskArguments((s32)task) = soundId;
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
    data = (u8 *)btlGetTaskArguments((s32)task);
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

    request->handle = (void *)fileQueueDefaultCallbackRequest(request->name);
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
                btlBossDebugPrintf(D_003A5110, *(u16 *)((u8 *)node + 0xA), size);
            }
            node->flags = (node->flags & ~1) | 2;
        }
    } else if (sndFindPackedTrackLoadStatus(node->position) != 0) {
        btlBossDebugPrintf(D_003A5138, *(u16 *)((u8 *)node + 0xA));
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
    request = (SoundFileRequest *)btlGetTaskArguments((s32)task);
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
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)owner;
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

/* Shared motion-SE owner: queued files become resource handles before playback. */
typedef struct SoundSlotOwner {
    u32 flags; /* 1 files queued, 2 files ready; 4 track pending, 8 loading, 0x10 ready. */
    s32 category;
    s32 id;
    u32 refCount; /* Shared retain count; release frees only on the zero transition. */
    s32 pendingSoundId; /* Packed-track key consumed by the load-status poll. */
    s32 pendingSlot;    /* Index into resourceHandles for the pending track. */
    s32 fileRequests[0x1D];
    s32 resourceHandles[0x1D];
    struct SoundSlotOwner *prev;
    struct SoundSlotOwner *next;
} SoundSlotOwner;

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
u8 *sndAcquireSlotOwner(s32 category, s32 id) {
    SoundSlotOwner *node = (SoundSlotOwner *)sndFindListNodeForChannel(category, id);
    BtlActorWork *context;
    SoundSlotOwner *head;

    if (node != 0) {
        btlBossDebugPrintf(D_003A51D0, node);
        node->refCount++;
        return (u8 *)node;
    }
    node = sdfAllocAndClearQuadwords(0x108);
    node->category = category;
    node->id = id;
    node->refCount = 1;
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
    return (u8 *)node;
}

/* The last reference cleans queued files and resource handles, then unlinks/frees. */
void sndReleaseSlotOwner(u8 *ownerAddress) {
    SoundSlotOwner *node = (SoundSlotOwner *)ownerAddress;
    u32 count = node->refCount - 1;
    node->refCount = count;
    if (count == 0) {
        u32 i = 0;
        u32 *resources = (u32 *)node->resourceHandles;
        u32 *requests = (u32 *)node->fileRequests;
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

INCLUDE_ASM(const s32, "game/code_001C8890", btlQueueUnitSoundSlotFileLoad);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5178);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5188);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5198);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A51A8);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A51D0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F41C0);

extern void btlQueueUnitSoundSlotFileLoad(s32);

extern u32 func_001F41C0(u32 *);

s32 btlCreateMoveOtherUnitsTask(u8 *owner, u32 soundId) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x54;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlQueueUnitSoundSlotFileLoad;
    *(void **)(task + 0x4C) = func_001F41C0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = soundId;
    arguments[1] = 0;
    arguments[3] = 0;
    return (s32)task;
}

void func_001F4430(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(s32 *)(temp_v0 + 0x264) = -1;
    *(s32 *)(temp_v0 + 0x268) = -1;
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
            u32 *requests = (u32 *)node->fileRequests;
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

extern s32 mnuGetSoundBufferStateLocked(void);

extern void mnuPrintTitleDebugBanner(void);

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

INCLUDE_ASM(const s32, "game/code_001C8890", sndPollAtrac3SELoadTask);
extern s32 sndPollAtrac3SELoadTask(BattleVoiceLoad *);

void *sndCreateAtracEffectLoadTask(u32 owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x58;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x4C) = sndPollAtrac3SELoadTask;
    task[0x10] = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = 0;
    arguments[1] = 0;
    arguments[2] = owner;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F49D0);

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

extern u32 func_001F49D0(u32 *);

void *sndCreateEarringPlaybackTask(u8 *owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x59;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = func_001F49D0;
    *(void **)(task + 0x4C) = sndUpdateEarringDeadPlayback;
    *(void **)(task + 0x50) = sndFinishEarringPlaybackTask;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
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

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F5410);

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
        btlFlagUnitDefeatCandidate((u8 *)target);
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
INCLUDE_ASM(const s32, "game/code_001C8890", func_001F57D0);

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
INCLUDE_ASM(const s32, "game/code_001C8890", func_001F5B50);

void func_001F5D00(void) {
}

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F5D08);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5390);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A53B0);

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A53D0);

INCLUDE_ASM(const s32, "game/code_001C8890", func_001F5ED8);

extern u32 func_001F5ED8(u32 *);

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
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = soundId;
    arguments[2] = variant;
    arguments[3] = channel;
    arguments[4] = flags;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001C8890", D_003A5410);

