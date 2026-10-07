#include "common.h"
#include "btl_scene_fade.h"
#include "btl_command.h"
#include "btl_action.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "pcp_vu0.h"


extern s32 btlGetRuntime(void);
extern void btlDispatchStateHandler(void *obj, s32 kind);

extern s32 kwlnTaskGetTaskByName(const char *);

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);

extern s32 kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), s32);
extern void func_00101968(s32, s32);
extern s32 kwlnTaskDestroyWithHierarchy(s32, s32);
extern s32 btlGetTrackedTaskHandle(s32);
extern s32 btlIsNamedBattleTaskRegistered(void);
extern s32 btlHasRegisteredGuidePanelTask(void);
extern s32 btlHasRegisteredSkillNamePanelTask(void);
extern s32 btlHasRegisteredAphNamePanelTask(void);
extern s32 btlCreateAiWork(s32);
extern void btlBossDebugPrintf(const char *, ...);
extern u32 fldGetSceneScriptTaskUserData(void);
extern char *D_004367B8;

extern s32 datEnemyRecords;

extern char *btlCommandPanelTaskNameRef;

extern void func_001C7DB8(s32, s32);

extern void func_001C35F0(s32, s32, s32);

extern u32 kwlnTaskGetUserValue();

extern void func_00230960(s32);

extern s32 D_003B6940[];

extern s32 func_00230978(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);
extern s32 func_001B4040();
extern s32 func_001B4210();
extern void func_0022AF90();
extern void func_00229728();
extern void btlSelectSceneAudioTrack();

typedef struct {
    void (*initialize)(s32);
    s32 (*update)(s32);
    s32 flags;
} SceneInitializer;

typedef struct RosterAvailability {
    u8 flags;
    u8 pad[7];
} RosterAvailability;

typedef struct SceneControl {
    u8 mode;
    s8 kind;
} SceneControl;

typedef struct BattleEffectParams {
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
} BattleEffectParams;

typedef struct SceneObject {
    s32 state;
} SceneObject;

extern SceneObject *fldGetSceneObjectTaskUserData(void);




typedef struct SceneScriptState {
    u32 state;
    u8 pad04[0xC];
    u32 value10;
} SceneScriptState;



extern SceneControl *btlCommandPanelWork;
extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;
extern f32 D_00433724;
extern f32 *D_0037F770[];
extern u32 func_001C82D8(s32, s8);
extern s32 btlCreateEffectTaskWithSourceParams(BattleEffectParams *, s32);
extern s32 btlCountTasksForOwner(s64);

extern SceneInitializer D_003B6938[];

extern u32 btlCountFlaggedSceneActors(void);
extern void btlRemoveTaskFromSceneGroup(BtlTask *);

extern s32 func_001AC750(s32, void *);

extern s32 D_004367C0;

extern char D_003B5D10[];

extern char D_003B5B10[];

extern s32 btlGetEntryFlagsUnlessDisabled(void *);

extern void fldInitializeSceneGroups(void);

extern u32 D_00435E64;

extern u32 D_00435E5C;

extern SceneSlotFadeWork *D_00438F54;


extern s32 datItemSkillRecords;

extern s32 func_00206090();
extern void btlRepositionPartyAroundBattleCenter();
extern s32 func_001AC648();
extern void func_001AEEA8();
extern void kwlnFadeBackgroundStartOut();
extern void kwlnDrawSetOverlayTransition();
extern void kwlnDrawEnableD88();
extern void kwlnDrawEnableDc8();
extern void kwlnDrawEnableE08();
extern void kwlnDrawSetupC70B();
extern void kwlnDrawEnableCd0();
extern void kwlnDrawEnableD30();
extern void btlMarkRuntimeUpdatePending();
extern void kwlnFadeStartIn();
extern s32 fldGetEncounterRuntimeResult();
extern void fldSetEncounterPendingValue();
extern void evtSetSolarOverlayFullyVisible();
extern void btlSyncModelFlagFromEventThresholds();
extern void btlResetTitleStreamOnBattleFlag();
extern void func_001E9410();
extern void btlSpawnBattleWorldAction();
extern void btlCreateRainEffect();
extern s32 btlReleaseScriptResourceA();
extern s32 btlReleaseScriptResource();
extern BtlRuntimeTask *btlCreateSoundUpdateTask(u32);
extern s32 btlCreateSoundReleaseTask();
extern s32 btlCreateWaitUnitListIdleTask();
extern s32 btlCreateApplyToActiveActorsTask();
extern s32 btlCreateFadeStateResetTask();
extern void btlCreateGuidePanelTask();
extern void btlRequestGuidePanelClose();
extern void fldEnableSceneGroupAdvancement(void);
extern s8 D_0037F531[];

typedef struct SceneKindTable {
    u8 pad00[0x14];
    u32 value[8];
} SceneKindTable;

extern SceneKindTable *D_00438F4C;
extern void func_001C8518();
extern void fldCollectAvailableRosterEntries(s32, s16 *);
extern char *fldGetCachedSceneActorNameAndId(s32, s16 *);

extern void func_001C92A0(s32, s32, s32, s32);
extern void func_001C9EA0(s32);
extern void func_001C9BE8(s32);
extern void func_001C98E8(s32);
extern void btlDrawRetreatCommandLabel(s32);

typedef struct SceneCheckArgs {
    u16 mode;
    u16 pad2;
    s32 first;
    s32 second;
} SceneCheckArgs;

extern s32 func_001ABDE8();
extern s32 func_001ABA40();

extern void btlReleaseBattleScratchBlocks(void);

typedef struct SceneGlobalState {
    u8 pad00[0x38];
    u32 stage; /* 0x38: state gate */
    u32 flags; /* 0x3C: scene restrictions */
    u8 pad40[8];
    s8 fadeLatched;  /* 0x48: fade kind counts recorded */
    u8 pad49[3];
    s32 fadeKindA;
    s32 fadeKindB;
} SceneGlobalState;

extern u8 *btlTrackedTaskHandles;
extern s32 mdlFlagTest();
extern s32 btlGetTaskState6();

extern s32 D_00416BB8[];

typedef struct BtlPanelInner {
    u8 pad00[0xDCC];
    s32 fDCC;
    u8 padDD0[0x6C];
    s32 fE3C;
} BtlPanelInner;

typedef struct BtlPanelRes {
    u8 pad00[0x18];
    BtlPanelInner *inner;
} BtlPanelRes;

typedef struct BtlPanelBlock {
    u8 pad00[0x18];
    BtlPanelRes *res;
} BtlPanelBlock;

extern BtlPanelBlock *btlResourceBlock;
extern void func_00306C28(s32, s32, s32, u8 *, s32, BtlPanelRes *, s32, s32);

extern s32 btlGetEffectActive();
extern void func_001CC020();
extern void func_001CC438();


extern char *D_004367CC;
extern void *sdfAllocAndClearQuadwords(s32);


extern char *D_004368B0;
extern void btlLoadResourceBlock(void);
extern void btlStartRegisteredChildTask(void);
extern void func_001C1520(void);
extern void func_001C16B0(s32);

extern void itfMesClearFlags();
extern void brsTaskAllowUpdate();
extern void evtBeginSolarOverlayFadeOut();
extern void func_001AA868();
extern s32 func_0029D000();
extern void datAdjustCurrentHp();

extern s32 func_002998D8();
extern s32 fldLoadAreaResource();
extern void btlReleaseEventAssets();
extern void btlReleaseBossData();
extern void fldPollAreaResourceLoad();
extern s32 fldGetResourceReadyFlag();
extern s32 brsTaskPollDone();

extern u32 func_001AB8D8();

extern s32 func_001B2630();


extern s64 mnuGetSoundBufferStateLocked(void);
extern void func_00336538(f32);
extern void func_003364B8(f32);
extern void btlBossDebugPrintfN(s32, s32, s32, const char *, ...);
extern f32 effMiscRandUnitFloat(void *state);
extern void mdlAddEntryPlain(void *, s32, s32);
extern void mdlAddEntryFlagged(void *, s32, s32);
extern u8 effSharedRandomState[];
extern void func_001EC5F0(u32);
extern void func_001EF030(void *, void *);
extern s32 func_001E88A8(u8 *);
extern void mdlProcessContextNodesAndTransforms(void *, void *);
extern void func_001E38F0(void *, void *, s32, u8 *, u32);
extern void dds3ClearObjectFlags(s32, s32);
extern u8 D_00380788[];
extern u8 D_003B6BD0[];

extern void func_001EC868(void *, f32 *, f32);

typedef struct BtlUnit BtlUnit;


/* Action-sequence +0x20 is the SDK BattleIndexWork payload. Its parent
 * allocation is 0x180 bytes, distinct from the world-actor BtlUnit. */
extern void btlAdvanceHistoryCounter(ActionStateLink *);


/* Metadata records reached through the battle table pointers. */
typedef struct BtlActionTableEntry {
    u8 kind;               /* 0x00 */
    u8 pad01[2];
    u8 resourceType;       /* 0x03 */
    u8 pad04[0x14];
    s32 defaultValue;      /* 0x18 */
    u16 flags;             /* 0x1C */
    u8 pad1E[2];
} BtlActionTableEntry;

typedef struct BtlCategoryTableEntry {
    u8 flags00;            /* 0x00 */
    u8 pad01[2];
    u8 kind03;             /* 0x03 */
    u8 pad04[4];
    u8 restriction;        /* 0x08 */
    u8 flags09;            /* 0x09 */
    u8 pad0A[0x1A];
    u32 flags24;           /* 0x24 */
    u8 pad28[8];
    s32 categoryType;      /* 0x30 */
    u8 pad34[4];
} BtlCategoryTableEntry;

typedef struct BtlResourceTableEntry {
    u32 flags;
    u8 pad04[72];
} BtlResourceTableEntry;


typedef struct BtlStateHandler {
    void (*start)(void *);
    void (*update)(void *);
    const char *name; /* debug label, never called */
} BtlStateHandler;

extern BtlStateHandler D_003B69D8[];

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

typedef struct FxTask {
    u8 pad0[0x10];
    s32 unk10;
    BtlUnit *unit;
} FxTask;

typedef struct SoundLink {
    u32 owner;
    s32 effectHandle;
    u32 *effect;
    u16 flags;
} SoundLink;

typedef struct SoundResourceLink {
    u32 owner;
    s32 effectHandle;
    u32 *effect;
    u32 flags;
} SoundResourceLink;

typedef struct SoundEntry {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} SoundEntry;

extern SoundEntry D_003BDE18[];

extern s128 D_003B6B80;

extern u8 D_003BD7D0[];

extern void evtSetUnitNormalizedDirection(struct EvtUnit *, s32);

typedef struct XformData {
    s128 vec0;
    s128 vec1;
    f32 f20;
    f32 f24;
} XformData;

/* Two camera vectors are written at work+0x50 and work+0x60 by btlInitializeSceneLightingAndTint. */
typedef struct BtlCameraVectors {
    u8 pad00[0x50];
    f32 eye[3];
    u8 pad5C[4];
    f32 target[3];
} BtlCameraVectors;

typedef struct BtlCameraTaskArgs {
    u32 kind;
    f32 component[8]; /* camera origin/direction inputs, offsets 0x04..0x20 */
} BtlCameraTaskArgs;

typedef struct BtlVectorTaskArgs {
    u8 pad00[0x20];
    f32 scale;
    u32 state24;
    u32 state28;
    union {
        u32 unit2C;
        s8 mode2C;
    };
    u32 unit30;
} BtlVectorTaskArgs;

typedef struct BtlCommandOption {
    u8 pad00[0xC];
    s32 kind;           /* 0x0C */
    u8 pad10[4];
    u8 inactive;        /* 0x14 */
} BtlCommandOption;

typedef struct BtlCommandArgument {
    s32 command;        /* 0x00 */
    s32 index;          /* 0x04 */
    u8 pad08[0x38];
    struct BtlIndexList *actorIndices;   /* 0x40 */
    u8 pad44[0x24];
    BtlCommandOption *option; /* 0x68 */
} BtlCommandArgument;

typedef struct BtlActiveSlot {
    u8 pad00[0x18];
    s32 unit;           /* 0x18 */
} BtlActiveSlot;

typedef struct BtlDeferredStats {
    BtlUnit *actor;    /* 0x00 */
    u8 pad04[0x1C];
    s32 primary;       /* 0x20 */
    s32 secondary;     /* 0x24 */
} BtlDeferredStats;

extern BtlRuntimeTask *btlCreateHookedUnitSoundTask();

extern u32 D_00436AD4;

extern u32 dds3AdvanceWorldCounter(void);

extern struct EffWorldNode *evtSpawnActionObj9(s32);

extern s8 btlSetActorEffectParameter(BtlUnit *, s32);

extern s32 func_0022F180(void);

extern s32 btlGetRuntime(void);

extern void mdlStoreTertiaryVectorVU(s32);

extern void mdlSetAmountOnAllContextResources(f32, s32);

extern f32 func_001F5780(u32, u8, f32, f32);

extern f32 func_001FDD20(f32 *, f32, f32, s32);

extern BtlRuntimeTask *btlDeferredTaskTail;

extern BtlRuntimeTask *btlDeferredTaskHead;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 kwlnDrawControlFlags;

extern s32 mnuPollTitleStreamStateLocked(void);

extern void mnuResetTitleStreamLocked(void);

extern void func_002A2200(s32);

extern s32 D_00435E0C;

typedef struct SoundSceneEntry {
    u8 pad00[0x24];
    u16 unk24;
    u8 pad26[2];
} SoundSceneEntry;

extern s32 sndFindPackedTrackLoadStatus(u32);


struct SceneLightRestoreArgs;
extern s64 func_00201520(struct SceneLightRestoreArgs *);

extern s64 func_00201718(void);

extern s32 sndPlaySkillSeTask(u32 *);

typedef struct SoundResourceNode {
    u32 flags;
    u32 unk_04;
    u32 unk_08;
    s32 fadeCountdown;
    u32 resourceHandle;
    u32 unk_14;
    struct SoundResourceNode *previous;
    struct SoundResourceNode *next;
} SoundResourceNode;

extern SoundResourceNode *sndAllocResourceNode(void);

extern void sndFormatResourceNameFromUnitMode(s32, s32);

extern s32 datCommandRecords;

extern s32 datActionAnimationRecords;

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern void btlRunTask(BtlRuntimeTask *);

extern s32 mdlGetContextResourceGroup(s32);

extern s32 mdlGetContextResourceId(s32);

extern f32 func_00208000(s32, s32, s32);

extern void func_0035C860();

extern char D_004192E8[]; /* "MDD_%03X.ADB" */

extern char D_004192D8[];

extern char D_00436AE8[];

extern u32 btlApplyDeferredUnitStatus(void *);

extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);

extern void btlInitMotionTransformFromComponents(u8 *, f32, f32, f32, f32, f32, f32, f32, f32);

extern void btlInitMotionTransformFromVectors(u8 *, f32 *, f32 *);

typedef struct SoundCommand {
    u32 handle;
    u32 resource;
    u16 currentId;
    u16 nextId;
} SoundCommand;

extern SoundCommand D_003BDC90;

extern u8 D_003BDCA0[];

typedef struct SoundTransition {
    u32 currentResource;
    u8 unk_04[0x14];
    u32 previousResource;
    u32 queuedResource;
    u16 soundId;
    u16 queuedId;
} SoundTransition;

extern s32 btlQueueTintTransitionWhenEnabled(u32 *);

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
} SoundTaskArgs;

extern BtlRuntimeTask *btlAllocTask(s32);

extern void *btlGetTaskArguments(s32);

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern u8 D_0037F510[];

extern char D_004178A8[];

extern char D_004178B8[];

extern char D_00417AF0[];

extern char D_00417B10[];

extern void func_001E1BB8(u8 *, u32, u32);

extern char D_00417B30[];

extern s32 btlCheckModelAssetByMode(u8 *, u32, u32);

extern void func_001E8258(s32, s32, s32, s32, s32);

extern void btlSetEffectCameraKeys(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void effMiscQuaternionToMatrixVU(void);

extern void effMiscQuaternionNlerpVU(f32);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_003E9130[];

extern u8 D_003E9120[];

extern s32 D_003BBF70[];

extern s32 D_003BBF88[];

extern s32 D_003BBFA8[];

extern s32 D_003BC090[];

extern s32 D_003BC0A0[];

extern s32 D_003BC0C0[];

extern s32 D_003BC0C8[];

extern void func_001F3E48(s32);
extern void btlAdvanceCursorForUnmarkedUnit(s32, s32);

extern void func_001FA480(s32, s32, s32);

extern void func_001FBAC0(s32, s32);

extern s32 func_001FB908(s32, s32, s32, s32);

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

extern char D_00418C58[];

extern void sdfFreeMemoryFromEitherHeap(void *);

extern s32 sndHasActiveFileLoad(void);

extern s32 fileGetResourceSize(s32);

extern void func_003422F8(s32, s32);




extern f32 *D_0037F770[];

extern u8 kwlnDefaultColorVector[];

extern void fldApplyLightSetCurrent(void);

extern s32 sndGetEffectNodeParameter(s32, u16);



extern s32 btlDoesEnabledStatusMatchCurrentId(void *, s32);
extern void btlUnitGetMuzzlePosVU(BtlUnit *);
extern void btlClearAllActorEntrySlots(BtlUnit *);
extern void btlReleaseUnitResources(BtlUnit *);
extern void btlInitUnitFxDefaults(BtlFx *);

extern s32 btlCheckSpecialAbility(s32, s32);
extern void func_001F5868(s32, s32, s32, s32);
extern void func_001F5320(s32, s32, s32, s32);
extern void mnuReleaseSoundBufferLocked(void);
extern void evtSetUnitAlphaTransition(u32, s32, u32);
extern void func_002A27A8(s32, s32, u8);
extern void mdlBroadcastMasked(s32, s32);
extern void func_001AB160(BtlUnit *);
extern s32 func_001E6428(s32 owner, s32 option);


extern void btlDebugPrintf(s32 tag, ...);

extern void fldCreateSceneSpriteTask(s32 sourceTask);

extern void fldAppendTaskToGroup(BtlTask *task);

extern void fldUpdateSceneGroupTask(BtlTask *task);

extern BtlRuntimeTask *fldCreateSceneGroupAction(u8 *actor, u32 owner, s32 groupIndex);

extern void btlClearSceneTaskActiveFlag(s32 task);

void btlActionSeqStateSelect(ActionStateLink *task) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = task->unit;
    s32 (*hook)(ActionStateLink *);
    s32 next;
    u32 flags;
    task->pendingFlags &= ~0x20;
    if (task->actionNumber == 0) {
        btlDispatchStateHandler(task, 0x1B);
        btlBossDebugPrintf("btl:actnum 0 [%p]\n", task);
        return;
    }
    hook = work->actionStateSelectionHook;
    if (hook != 0) {
        next = hook(task);
        if (next != -1) {
            btlDispatchStateHandler(task, next);
            return;
        }
    }
    if (task->pendingFlags & 0x40) {
        btlDispatchStateHandler(task, 0xA);
    } else {
        flags = unit->flags;
        if (flags & 0x200) {
            if (work->battleFlags & 0x8000) {
                btlDispatchStateHandler(task, 9);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        } else if (flags & 0x400) {
            if (!(work->unk220 & 1)) {
                btlDispatchStateHandler(task, 8);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        }
    }
}

extern s32 effOffsetIfOwnerFlagClear();

void btlUnitTurnEndStateSelect(u8 *task) {
    u8 *unit = (u8 *)((BtlTask *)task)->unit;
    u32 flags = ((BtlUnit *)unit)->flags;
    if (flags & 0x200) {
        if (flags & 0x1000) {
            if ((((BtlUnit *)unit)->stateFlags & 0x40) && !(((BtlUnit *)unit)->conditionFlags & 0x5800) &&
                !(((BtlTask *)task)->flags & 0x100)) {
                ((BtlTask *)task)->actionStage = 4;
                ((BtlTask *)task)->effect = effOffsetIfOwnerFlagClear(unit, 0xA4);
                ((BtlUnit *)unit)->flags = (((BtlUnit *)unit)->flags & ~0x20) | 0x400000;
                ((BtlUnit *)unit)->statBits |= 0x4000;
                ((BtlUnit *)unit)->stateFlags |= 0x2000;
                btlDispatchStateHandler(task, 0x10);
            } else {
                btlDispatchStateHandler(task, 0x1E);
            }
            ((BtlUnit *)unit)->stateFlags &= ~0x40;
        } else {
            btlDispatchStateHandler(task, 0x1E);
        }
    } else {
        btlDispatchStateHandler(task, 0x1E);
    }
}

extern s32 sndIsResourceNodeReferencedOrActive(s32);
extern s32 sndHasResourceFlagsOneOrEight(s32);
extern void sndFreeResourceNode(SoundResourceNode *);
extern void sndFreeListNode(ActiveSoundNode *);
extern s32 btlIsUnitInActiveList(u8 *);
extern void btlResetActiveUnitList(void);
extern s32 btlCountTasksByKind(s32);
extern u32 sndGetResourceStatus(u32 *);

/* Returns nonzero when the scene is idle: frees every actor's unreferenced
 * resource node, then the caller's list node, and requires no waiting actor
 * or scene task of the listed kinds. */
s32 fldCheckSceneResourcesIdle(s32 self) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    BtlUnit *actor;
    for (actor = scene->units; actor != 0; actor = actor->nextActor) {
        if (actor->node318 != 0) {
            if (sndIsResourceNodeReferencedOrActive((s32)actor->node318) != 0) {
                if ((s32)actor == self) {
                    return 0;
                }
                sndGetResourceStatus(&actor->node318->flags);
                return 0;
            }
            sndFreeResourceNode(actor->node318);
            actor->node318 = 0;
        }
    }
    if (((BtlUnit *)self)->node324 != 0) {
        if (sndHasResourceFlagsOneOrEight((s32)((BtlUnit *)self)->node324) != 0) {
            return 0;
        }
        sndFreeListNode(((BtlUnit *)self)->node324);
        ((BtlUnit *)self)->node324 = 0;
    }
    for (actor = scene->units; actor != 0; actor = actor->nextActor) {
        if (actor->flags & 0x200) {
            if (actor->flags & 2) {
                if ((actor->gunResourceFlags & 8) == 0) {
                    return 0;
                }
            }
        }
    }
    if (btlCountTasksByKind(0x23) != 0) return 0;
    if (btlCountTasksByKind(0x40) != 0) return 0;
    if (btlCountTasksByKind(0x41) != 0) return 0;
    if (btlCountTasksByKind(0x3F) != 0) return 0;
    if (btlCountTasksByKind(0x36) != 0) return 0;
    if (btlCountTasksByKind(0x37) != 0) return 0;
    if (btlCountTasksByKind(0x24) != 0) return 0;
    return btlCountTasksByKind(0x2E) == 0;
}

s32 fldReleaseIdleSceneActorResources(BtlUnit *actor) {
    if (actor->node318 != 0) {
        if (sndIsResourceNodeReferencedOrActive((s32)actor->node318) != 0) {
            return 0;
        }
        sndFreeResourceNode(actor->node318);
        actor->node318 = 0;
    }
    if (actor->node324 != 0) {
        if (sndHasResourceFlagsOneOrEight((s32)actor->node324) != 0) {
            return 0;
        }
        sndFreeListNode(actor->node324);
        actor->node324 = 0;
    }
    if (actor->unk334 != 0) {
        return 0;
    }
    if (btlIsUnitInActiveList((u8 *)actor) != 0) {
        btlResetActiveUnitList();
        return 0;
    }
    if (btlCountTasksForOwner(actor->owner) != 0) {
        return 0;
    }
    return btlCountTasksByKind(0x2E) == 0;
}

void func_001D48E0(void) {
}

void func_001D48E8(void) {
}

void func_001D48F0(s32 task) {
    ((BtlTask *)task)->flags = ((BtlTask *)task)->flags & 0xfffffdff;
}

extern void func_001AA850();
extern void btlFlagUnitDefeatCandidate();
extern void btlRefreshUnitMotionSelection();
extern BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(u8 *, s32, s32, f32);

INCLUDE_ASM(const s32, "game/code_001D4438", btlReleaseIdleUnitSoundAndAdvanceTask);

void func_001D4A30(s32 task) {
    ((BtlTask *)task)->flags = (((BtlTask *)task)->flags | 0x10) & ~0x200;
}

void btlUnitStateSelectAfterAction(u8 *task) {
    u8 *unit = (u8 *)((BtlTask *)task)->unit;
    u32 flags;
    if (!(((BtlUnit *)unit)->flags & 0x20)) {
        ((BtlTask *)task)->flags &= ~0x100;
    }
    if (((BtlUnit *)unit)->node318 != 0 && sndIsResourceNodeReferencedOrActive((s32)((BtlUnit *)unit)->node318) == 0) {
        sndFreeResourceNode(((BtlUnit *)unit)->node318);
        ((BtlUnit *)unit)->node318 = 0;
    }
    flags = ((BtlUnit *)unit)->flags;
    if (flags & 0x20000000) {
        btlDispatchStateHandler(task, 0x11);
    } else if (flags & 0x400000) {
        btlDispatchStateHandler(task, 0x10);
    } else if (flags & 0x10000000) {
        btlDispatchStateHandler(task, 0x12);
    } else if (flags & 0x20) {
        btlUnitTurnEndStateSelect(task);
    } else if (((BtlUnit *)unit)->stateFlags & 0x40000) {
        btlDispatchStateHandler(task, 0x15);
    }
}

void func_001D4B90(s32 task) {
    ((BtlTask *)task)->flags = ((BtlTask *)task)->flags & 0xffffffef;
}

extern s32 fldCheckSceneResourcesIdle(s32);
extern s32 btlBothSidesActive(s32);
extern s32 func_0020EB40(u8 *);

void btlActionSeqCheckDispatch(u8 *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u32 flags = scene->battleFlags;
    s32 unit = (s32)((BtlTask *)task)->unit;
    BtlUnit *actor;
    if (!(flags & 0x20)) {
        for (actor = scene->units; actor != 0; actor = actor->nextActor) {
            u32 actorFlags = actor->flags;
            if (actorFlags & 0x4000) {
                return;
            }
            if (actorFlags & 0x30400000) {
                return;
            }
        }
        if (!(flags & 0x8000) || fldCheckSceneResourcesIdle(unit) != 0) {
            if (btlBothSidesActive(unit) == 0) {
                btlDispatchStateHandler(task, 0x1D);
                return;
            }
            if (func_0020EB40(task) != 0) {
                btlDispatchStateHandler(task, 5);
            } else {
                btlActionSeqStateSelect(task);
            }
        }
    }
}

void func_001D4C98(void) {
}

extern BtlRuntimeTask *btlCreateEffObjB(BtlUnit *, s32);
extern u64 btlStartTask(void *);
extern s32 sndHasActiveActor(void);
extern s64 btlAdvanceRuntimeSequenceCounter(void);
extern BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void);
extern BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void);
extern BtlRuntimeTask *btlCreateCommandSoundTask(s32, s32);
extern void func_00211360();
extern s32 func_0020F200();

typedef struct TaskBlock {
    u32 word[11];
} TaskBlock;

extern BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *, TaskBlock *);
extern BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *, s32, u8);

extern s32 evtRunContext(s32, s32, s32, s32, u16);
extern s32 btlRollAiBucket(void);

/* Try to clear the unit's condition: 2 and 4 always clear, 1 needs battle mode 2, and the
 * others roll a script-supplied chance (capped at 70, scaled by ability 0x252). */
void func_001D4CA0(BtlTask *task) {
    TaskBlock block;
    BtlUnit *unit;
    s32 chance;
    f32 scale;
    BtlRuntimeTask *effect;

    if (btlCountTasksByKind(0x49) != 0) {
        return;
    }
    unit = task->unit;
    unit->flags |= 0x4000;
    switch (unit->conditionFlags & 0x7FFF) {
    case 8:
    case 0x20:
    case 0x200:
    case 0x1000:
    case 0x2000:
        if (unit->stateFlags & 4) {
            unit->stateFlags &= ~4;
            break;
        }
        /* fallthrough */
    case 1:
        unit->stateFlags &= ~4;
        switch (unit->conditionFlags & 0x7FFF) {
        case 0x1000:
            chance = evtRunContext(0xE, (s32)&unit->statBits, 0, 0, 0);
            break;
        case 0x200:
            chance = evtRunContext(0xF, (s32)&unit->statBits, 0, 0, 0);
            break;
        case 0x20:
            chance = evtRunContext(0x10, (s32)&unit->statBits, 0, 0, 0);
            break;
        case 8:
            chance = evtRunContext(0x11, (s32)&unit->statBits, 0, 0, 0);
            break;
        case 1:
            chance = ((BtlState *)btlGetRuntime())->mode == 2 ? 100 : 0;
            break;
        case 0x2000:
            chance = evtRunContext(0x12, (s32)&unit->statBits, 0, 0, 0);
            break;
        default:
            chance = 0;
            break;
        }
        scale = 1.0f;
        if (btlCheckSpecialAbility((s32)&unit->statBits, 0x252)) {
            scale = datAbilityParameters[0x252 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        chance = chance * scale;
        if ((unit->conditionFlags & 0x7FFF) != 1 && chance > 70) {
            chance = 70;
        }
        btlBossDebugPrintf("btl:bad recovery=%d%%[ratio=%.2f]\n", chance, scale);
        if (btlRollAiBucket() >= chance) {
            break;
        }
        /* fallthrough */
    case 2:
    case 4:
        memset(&block, 0, sizeof(block));
        block.word[3] = 0x322F;
        btlStartTask(btlCreateActorParameterDeltaTask(unit, &block));
        if (unit->conditionFlags & 0x1000) {
            unit->flags |= 0x20000000;
            effect = btlCreateEffObjB(task->unit, 0xCA);
            effect->ownerId = btlAdvanceRuntimeSequenceCounter();
            btlStartTask(effect);
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
        }
        break;
    }
    btlDispatchStateHandler(task, 0x1C);
}

void func_001D4FE0(void) {
}

/* Starts the command sound tasks and the follow-up action for the acting
 * unit, chosen by its selection flags. */
void btlStartCommandAudioAndSelectedAction(u8 *task) {
    BtlUnit *unit;
    s64 ownerId;
    BtlRuntimeTask *effectTask;
    BtlRuntimeTask *sceneTask;
    BtlRuntimeTask *object;
    s32 effect;
    TaskBlock block;
    u32 hp, mp;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x49) == 0) {
        unit = ((BtlTask *)task)->unit;
        ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        if (unit->flags & 0x200) {
            btlStartTask(btlCreateCommandSoundTask(task, 9));
        } else {
            btlStartTask(btlCreateCommandSoundTask(task, 3));
        }
        switch (unit->conditionFlags & 0x7FFF) {
        case 0x200:
            func_00211360(task, 0);
            btlDispatchStateHandler(task, 0xC);
            break;
        case 0x20:
            if (((BtlTask *)task)->unit->flags & 0x200) {
                func_00211360(task, 2);
            } else {
                func_00211360(task, 3);
            }
            btlDispatchStateHandler(task, 0xC);
            break;
        case 0x40:
            func_00211360(task, 4);
            btlDispatchStateHandler(task, 0xC);
            break;
        case 1:
        case 0x2000:
            ((BtlTask *)task)->result = 0xD;
            btlDispatchStateHandler(task, 0xC);
            break;
        case 8:
            sceneTask = fldCreateSceneGroupAction(task, 0x64, 1);
            sceneTask->startCondition.kind = 7;
            sceneTask->startCondition.value.owner = ownerId;
            sceneTask->ownerId = unit->owner;
            btlStartTask(sceneTask);
            memset(&block, 0, 0x2C);
            hp = unit->maxHp;
            block.word[0] = hp / 10;
            mp = unit->unk12C;
            block.word[1] = mp / 10;
            effectTask = btlCreateActorParameterDeltaTask(unit, &block);
            effectTask->startCondition.kind = 7;
            effectTask->startCondition.value.owner = ownerId;
            btlStartTask(effectTask);
            if ((s32)block.word[0] > 0) {
                object = btlCreateLinkedEffectTask(unit, block.word[0], 0);
                object->startCondition.kind = 4;
                object->startCondition.value.handle = effectTask->handle;
                btlStartTask(object);
            }
            if ((s32)block.word[1] > 0) {
                object = btlCreateLinkedEffectTask(unit, block.word[1], 1);
                object->startCondition.kind = 4;
                object->startCondition.value.handle = effectTask->handle;
                btlStartTask(object);
            }
            btlDispatchStateHandler(task, 0x1B);
            break;
        case 0x800:
            sceneTask = fldCreateSceneGroupAction(task, 0x64, 1);
            sceneTask->startCondition.kind = 7;
            sceneTask->startCondition.value.owner = ownerId;
            sceneTask->ownerId = unit->owner;
            btlStartTask(sceneTask);
            btlDispatchStateHandler(task, 0x1B);
            break;
        }
        effect = func_0020F200(task);
        if (effect > 0) {
            object = btlCreateEffObjB(unit, effect);
            object->ownerId = ownerId;
            btlStartTask(object);
        }
        ((BtlTask *)task)->flags |= 0x200;
    }
}

void func_001D5330(ActionStateLink *task) {
    btlGetRuntime();
    task->pendingFlags = task->pendingFlags & 0xfffffffb;
    func_001CAB60(task);
}

extern s32 fldGetSceneObjectState(void);
extern void fldSetSceneObjectAndGroupStates(void);
extern s32 btlIsSupportedCommandKind(s32 *);
extern s32 btlAiCheckStatusRollEligibility(BtlTask *);

void func_001D5368(BtlTask *task) {
    BtlState *work = (BtlState *)btlGetRuntime();
    s32 sceneObjectState;

    if (work->battleFlags & 0x20) {
        return;
    }
    if ((task->flags & 4) == 0 && sndHasActiveActor() == 0 && btlCountTasksByKind(0x2E) == 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
        task->flags |= 4;
    }

    sceneObjectState = fldGetSceneObjectState();
    if (sceneObjectState == 3 || sceneObjectState == 8) {
        if (btlIsSupportedCommandKind(&task->result) != 0) {
            btlDispatchStateHandler(task, 7);
        } else {
            fldSetSceneObjectAndGroupStates();
            if (btlAiCheckStatusRollEligibility(task) != 0) {
                btlDispatchStateHandler(task, 0xB);
            } else {
                btlDispatchStateHandler(task, 0xC);
            }
        }
    } else if (work->battleFlags & 0x8000) {
        fldSetSceneObjectAndGroupStates();
        btlDispatchStateHandler(task, 9);
    }
}

void btlCommandResultEffectSelect(u8 *task) {
    s32 selection;

    ((BtlTask *)task)->flags &= ~4;
    switch (((BtlTask *)task)->result) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
        if (((BtlTask *)task)->result == 4) {
            selection = btlGetLoggedIndexedCommandItem(((BtlTask *)task)->commandReference);
        } else {
            selection = ((BtlTask *)task)->arg;
        }
        switch (btlGetCommandBlockReason(task, selection)) {
        case 2:
            btlStartTask(btlCreateEffObjB(((BtlTask *)task)->unit, 0x82));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 6:
            btlStartTask(btlCreateEffObjB(((BtlTask *)task)->unit, 0xB0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 8:
            btlStartTask(btlCreateEffObjB(((BtlTask *)task)->unit, 0xB2));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 10:
            btlStartTask(btlCreateEffObjB(((BtlTask *)task)->unit, 0xD0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        }
        break;
    }
    fldCreateSceneSpriteTask((s32)task);
}

extern u32 fldGetSceneScriptState(void);
extern u32 fldGetSceneScriptValue(void);
extern s32 btlGetCommandTargetEligibility(BtlIndexList *, s32);
extern void fldMarkActiveSceneScriptState(void);
extern s32 btlSetTaskPhase2(void);

void func_001D5668(BtlTask *task) {
    BtlState *work = (BtlState *)btlGetRuntime();
    u32 state;
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
        /* Script values carry the selected-list pointer as an opaque word. */
        selected = (BtlIndexList *)fldGetSceneScriptValue();
        switch (task->result) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
            if (task->result == 4) {
                selection = btlGetLoggedIndexedCommandItem(task->commandReference);
            } else {
                selection = task->arg;
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
            case 7:
                btlStartTask(btlCreateEffObjB(task->unit, reason == 5 ? 0xA6 : 0xE2));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            case 9:
                btlStartTask(btlCreateEffObjB(task->unit, 0xCC));
                sndSetStationedSeVolume(0xD);
                btlSetTaskPhase2();
                return;
            }
            break;
        }
        if (task->result != 9) {
            sndSetStationedSeVolume(8);
        }
        btlCopyIndexList(task->targetList, selected);
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

void func_001D5938(s32 task) {
    ((BtlTask *)task)->flags = ((BtlTask *)task)->flags & 0xffffff7f;
}

typedef struct SceneAiEntry {
    u8 kind;                  /* 0x000 */
    u8 pad01;
    u16 slot;                 /* 0x002 */
    u8 pad04[0x158];
} SceneAiEntry;

extern SceneAiEntry *datEnemyAiRecords;
extern s32 btlAllocAndCheck();
extern void btlAssignTaskResultAndArgument();
extern void btlBindActorSlot();
extern void btlRunRandomWeightedAiTableAction();
extern s32 btlAiCheckStatusRollEligibility();
extern s32 kwlnTaskIsRegistered();

/* AI task: binds the acting unit's slot on first run, then waits for the
 * pending AI task and dispatches state 0xB or 0xC. */
s32 btlAiTaskUpdate(BtlTask *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    u16 index;
    if (!(scene->battleFlags & 0x20)) {
        if (sndHasActiveActor() == 0) {
            if (fldCheckSceneResourcesIdle((s32)task->unit) != 0) {
                if (!(task->flags & 0x80)) {
                    index = task->unit->mode;
                    scene->boundTask = 0;
                    if (datEnemyAiRecords[index].kind != 1 && btlAllocAndCheck(task) != 0) {
                        btlAssignTaskResultAndArgument(task);
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
                } else if (kwlnTaskIsRegistered(scene->boundTask) == 0) {
                    if (task->result == -1) {
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

void btlMarkSceneTaskAfterReset(s32 task) {
    func_001C35F0(task, 0, 0);
    ((BtlTask *)task)->flags = ((BtlTask *)task)->flags | 0x20;
}

extern void func_001E0CE0(s32, s32);
extern s32 btlAiCheckStatusRollEligibility();

s32 btlCommandStateSelectB(s32 task) {
    if (sndHasActiveActor() == 0) {
        func_001E0CE0(task, task + 0x20);
        if (btlAiCheckStatusRollEligibility(task) != 0) {
            btlDispatchStateHandler(task, 0xB);
        } else {
            btlDispatchStateHandler(task, 0xC);
        }
    }
}

void func_001D5BD8(void) {
}

extern void func_001DD390(u8 *command, u8 *argument);
extern s32 btlIsActiveActor();

s32 btlCommandStateSelectC(ActionStateLink *task) {
    s32 command;
    s32 ready;
    s32 value;
    BtlUnit *actor;
    if ((task->flags & 8) || sndHasActiveActor() == 0) {
        actor = (BtlUnit *)btlGetIndexListEntry(task->indexWork.indices, 0);
        command = task->indexWork.phase;
        ready = 0;
        if (command == 1) {
            ready = 1;
        } else if (command == 2) {
            value = task->indexWork.skillId;
            if (value < 0xFA) {
                if (value < 0xF8) {
                    ready = 0;
                } else {
                    ready = 1;
                }
            }
        }
        if (ready == 1 && (btlIsActiveActor(actor) == 0 || (btlUnitStatusPair(actor) & 0xE1) != 1)) {
            btlDispatchStateHandler(task, 0x1B);
        } else if (!(task->flags & 8)) {
            btlDispatchStateHandler(task, 0xC);
        } else {
            func_001DD390((u8 *)task, (u8 *)&task->indexWork);
        }
    }
}

void btlResetCommandIndexWork(ActionStateLink *task) {
    func_001B7830();
    btlResetIndexWork(&task->indexWork);
    task->unit->unk314 = 0xffffffff;
}

void btlCommandStartSoundTasks(u8 *task) {
    u8 *unit;
    s64 ownerId;
    s32 effect;
    BtlRuntimeTask *object;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x49) == 0) {
        unit = (u8 *)((BtlTask *)task)->unit;
        ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        if (((BtlUnit *)unit)->flags & 0x200) {
            btlStartTask(btlCreateCommandSoundTask(task, 9));
        } else {
            btlStartTask(btlCreateCommandSoundTask(task, 3));
        }
        if ((((BtlUnit *)unit)->conditionFlags & 0x7FFF) == 0x20) {
            if (((BtlTask *)task)->unit->flags & 0x200) {
                func_00211360(task, 2);
            } else {
                func_00211360(task, 3);
            }
            btlDispatchStateHandler(task, 0xC);
        }
        effect = func_0020F200(task);
        if (effect > 0) {
            object = btlCreateEffObjB((BtlUnit *)unit, effect);
            object->ownerId = ownerId;
            btlStartTask(object);
        }
        ((BtlTask *)task)->flags |= 0x200;
    }
}

extern void func_001DDB60(ActionStateLink *, s32);

void btlCommandPrintAndFetchOwner(ActionStateLink *task) {
    s32 command;
    btlBossDebugPrintf("btl:command=%d\n", task->indexWork.phase);
    func_001DDB60(task, (s32)&task->indexWork);
    command = task->indexWork.phase;
    if (command > 0) {
        if (command >= 4) {
            if (command < 9) {
                if (command >= 7) {
                    if (btlGetIndexListCount(task->indexWork.indices) == 1) {
                        task->indexWork.unk48 = ((BtlUnit *)btlGetIndexListEntry(task->indexWork.indices, 0))->owner;
                    }
                }
            }
        } else {
            if (btlGetIndexListCount(task->indexWork.indices) == 1) {
                task->indexWork.unk48 = ((BtlUnit *)btlGetIndexListEntry(task->indexWork.indices, 0))->owner;
            }
        }
    }
}

extern void func_00201828();

void btlProcessEligibleCommandTaskEffects(ActionStateLink *task) {
    BattleIndexWork *commandData = &task->indexWork;
    u8 *work = (u8 *)btlGetRuntime();
    BtlUnit *owner = task->unit;
    void (*hook)(u8 *);
    if (fldCheckSceneResourcesIdle((s32)owner) != 0) {
        if (task->indexWork.stage == 2) {
            btlStartTask(btlCreateEffObjB(owner, task->indexWork.parameter));
        }
        func_00201828(task, commandData);
        hook = *(void (**)(u8 *))(work + 0x634);
        if (hook != 0) {
            hook((u8 *)task);
        }
        func_001DD390((u8 *)task, (u8 *)commandData);
    }
}

void func_001D5FA8(void) {
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D5FB0);


void func_001D7228(void) {
}

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417348);

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417360);

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D7230);

void func_001D8C78(void) {
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D8C80);

/* Command-state callbacks mark the same world unit as the return/end handlers. */
void func_001DA1F8(ActionStateLink *task) {
    task->unit->flags |= 0x4000;
}

extern u16 *btlGetSideIndexedActorStatusTable(s32 kind, s32 index);
extern s32 func_001E2E58(BtlUnit *, s32);
extern BtlRuntimeTask *btlScheduleRefreshTask(BtlUnit *unit);
extern BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 model, s32 variant, s32 motion, s32 frames, u8 mode);
extern BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *unit, u32 index, u32 value, s8 mode);
extern BtlRuntimeTask *btlCreateUnitFadeInTask(BtlUnit *unit, u32 value, u32 variant);
extern BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *unit);
extern BtlRuntimeTask *sndCreateEffectSourceTask(struct SoundResourceNode *resource, BtlUnit *unit, u64 wait);
extern BtlRuntimeTask *btlCreateEffObjA(BtlUnit *unit, s32 effect);
extern BtlRuntimeTask *btlCreateEffObjD(BtlUnit *unit, s32 ability);

/* State-handler table entry (0x3B6A9C); the table holds s32 handlers (btlCommandStateSelectB) and this one
 * returns without a value, as retail's missing sibling calls show.
 * Gun-change command (slot 0x11): once no blocking tasks remain, swap the unit to its gun model with the hooked
 * sound, refresh linked allies and reload their models, start the gun effects, and continue to state 0x19/0x1B;
 * a unit already holding the gun only restores its motion, swaps back and continues to 0x1C/0x1B. */
s32 btlCommandGunChangeStart(BtlTask *task) {
    BtlUnit *unit;
    BtlState *state;
    BtlUnit *other;
    BtlRuntimeTask *sound;
    BtlRuntimeTask *change;
    BtlRuntimeTask *spawned;
    BtlRuntimeTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->resourceIndex);
    if (!(unit->flags & 0x20)) {
        motion = func_001E2E58(unit, 0x11) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->flags & 0x400020)) {
        unit->flags &= ~0x1000;
        unit->statBits &= ~0x1000;
        sound = btlCreateHookedUnitSoundTask(unit, 0x11);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->nextActor) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = btlScheduleRefreshTask(other);
                spawned->startCondition.kind = 4;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->ownerId = unit->owner;
                btlStartTask(spawned);
            }
        }
        change = btlCreateModelChangeTask(unit, unit->modelId, unit->modelVariant, motion, 0x18, 0);
        change->startCondition.kind = 4;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->nextActor) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = btlCreateModelLoadPollTask(other, other->resourceKind, other->resourceIndex, 0);
                load->startCondition.kind = 4;
                load->startCondition.value.handle = change->handle;
                load->ownerId = unit->owner;
                btlStartTask(load);
                spawned = btlCreateUnitFadeInTask(other, 0, 0);
                spawned->startCondition.kind = 4;
                spawned->startCondition.value.handle = load->handle;
                spawned->ownerId = unit->owner;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[45], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = btlCreateEffObjA(unit, task->result);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundTask((s32)task, 0xE);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlAllocateIndexedUnitEffectTask((u8 *)unit, 0x11,
                                                                btlGetSlotRateKind((u8 *)unit, 0x11), 1.0f);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        if (!btlDoesEnabledStatusMatchCurrentId(&task->unit->statBits, 0xE0)) {
            spawned = fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
            spawned->startCondition.kind = 4;
            spawned->startCondition.value.handle = sound->handle;
            spawned->ownerId = unit->owner;
            btlStartTask(spawned);
        }
        if (task->unit->conditionFlags & 0x480) {
            btlDispatchStateHandler(task, 0x19);
        } else {
            btlDispatchStateHandler(task, 0x1B);
        }
    } else {
        if ((unit->conditionFlags & 0x7FFF) != 0x4000) {
            unit->flags &= ~0x20;
            if (unit->hp == 0) {
                unit->hp = 1;
                btlRefreshUnitMotionSelection(unit);
            }
        }
        if (task->actionStage == 4) {
            if (!btlCheckSpecialAbility((s32)&unit->statBits, 0x251)) {
                btlStartTask(btlCreateEffObjB(unit, task->effect));
                task->actionStage = 0;
            } else {
                btlStartTask(btlCreateEffObjD(unit, 0x251));
            }
        }
        unit->flags &= ~0x1000;
        unit->statBits &= ~0x1000;
        change = btlCreateModelChangeTask(unit, unit->modelId, unit->modelVariant, motion, 0x12, 3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[47], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->flags &= ~0x400000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1C);
        } else {
            btlDispatchStateHandler(task, 0x1B);
        }
    }
}

void func_001DA728(ActionStateLink *task) {
    task->unit->flags |= 0x4000;
}

/* Slot-0x10 model-change command: the same flow as btlCommandGunChangeStart, swapping to the unit's
 * unkDC/combatantKind model and back; the return path only clears flag 0x20000000. */
void func_001DA740(BtlTask *task) {
    BtlUnit *unit;
    BtlState *state;
    BtlUnit *other;
    BtlRuntimeTask *sound;
    BtlRuntimeTask *change;
    BtlRuntimeTask *spawned;
    BtlRuntimeTask *load;
    u16 *status;
    s32 motion;

    if (btlCountTasksByKind(0x1A) != 0 || btlCountTasksByKind(0x18) != 0 || btlCountTasksByKind(0x23) != 0) {
        return;
    }
    state = (BtlState *)btlGetRuntime();
    unit = task->unit;
    status = btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->resourceIndex);
    if (!(unit->flags & 0x20)) {
        motion = func_001E2E58(unit, 0x10) + 0x14;
    } else {
        motion = status[0x15];
    }
    if (!(unit->flags & 0x20000020)) {
        unit->flags |= 0x1000;
        unit->statBits |= 0x1000;
        sound = btlCreateHookedUnitSoundTask(unit, 0x10);
        btlStartTask(sound);
        for (other = state->units; other != NULL; other = other->nextActor) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                spawned = btlScheduleRefreshTask(other);
                spawned->startCondition.kind = 4;
                spawned->startDelay = 1;
                spawned->startCondition.value.handle = sound->handle;
                spawned->ownerId = unit->owner;
                btlStartTask(spawned);
            }
        }
        change = btlCreateModelChangeTask(unit, unit->unkDC, unit->combatantKind, motion, 0x18, 0);
        change->startCondition.kind = 4;
        change->startCondition.value.handle = sound->handle;
        btlStartTask(change);
        for (other = state->units; other != NULL; other = other->nextActor) {
            if (other != unit && (btlUnitStatusPair(other) & 0x202) == 0x202) {
                load = btlCreateModelLoadPollTask(other, other->resourceKind, other->resourceIndex, 0);
                load->startCondition.kind = 4;
                load->startCondition.value.handle = change->handle;
                load->ownerId = unit->owner;
                btlStartTask(load);
                spawned = btlCreateUnitFadeInTask(other, 0, 0);
                spawned->startCondition.kind = 4;
                spawned->startCondition.value.handle = load->handle;
                spawned->ownerId = unit->owner;
                btlStartTask(spawned);
            }
        }
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[45], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        spawned = btlCreateEffObjA(unit, task->result);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundUpdateTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateSecondaryCommandSoundTask();
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlCreateCommandSoundTask((s32)task, 0xE);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        spawned = btlAllocateIndexedUnitEffectTask((u8 *)unit, 0x10, btlGetSlotRateKind((u8 *)unit, 0x10), 1.0f);
        spawned->startCondition.kind = 4;
        spawned->startCondition.value.handle = sound->handle;
        btlStartTask(spawned);
        if (!btlDoesEnabledStatusMatchCurrentId(&task->unit->statBits, 0xE0)) {
            spawned = fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
            spawned->startCondition.kind = 4;
            spawned->startCondition.value.handle = sound->handle;
            spawned->ownerId = unit->owner;
            btlStartTask(spawned);
        }
        if (task->unit->conditionFlags & 0x480) {
            btlDispatchStateHandler(task, 0x19);
        } else {
            btlDispatchStateHandler(task, 0x1B);
        }
    } else {
        unit->flags |= 0x1000;
        unit->statBits |= 0x1000;
        change = btlCreateModelChangeTask(unit, unit->unkDC, unit->combatantKind, motion, 0x12, 3);
        btlStartTask(change);
        btlStartTask(btlCreateGunLoadPollTask(unit));
        btlStartTask(sndCreateEffectSourceTask(state->resources[47], unit, change->handle));
        btlStartTask(sndCreateStationedSeTask(0x1000F));
        unit->flags &= ~0x20000000;
        if (task->flags & 0x10) {
            btlDispatchStateHandler(task, 0x1C);
        } else {
            btlDispatchStateHandler(task, 0x1B);
        }
    }
}

void func_001DABC0(ActionStateLink *task) {
    task->unit->flags |= 0x4000;
}

void func_001DABD8(u8 *task) {
    u8 *actor;
    u8 *effectTask;
    BtlRuntimeTask *modelTask;
    u8 *statusTable;
    s32 model;

    if (btlCountTasksByKind(0x1A) != 0 ||
        btlCountTasksByKind(0x18) != 0 ||
        btlCountTasksByKind(0x23) != 0) {
        return;
    }

    btlGetRuntime();
    actor = *(u8 **)(task + 0x18);
    statusTable = (u8 *)btlGetSideIndexedActorStatusTable(
        *(s32 *)(actor + 0xC4), *(s32 *)(actor + 0xC8));
    model = *(u16 *)(statusTable + 0x2A);
    effectTask = (u8 *)btlCreateEffObjB(actor, 0x7E);
    btlStartTask(effectTask);
    modelTask = btlCreateModelChangeTask(actor, 0, 0x1F, model, 0x12, 1);
    btlStartTask(modelTask);

    *(u32 *)(actor + 0x110) = (*(u32 *)(actor + 0x110) | 0x1000) & 0xEFFFFFFF;
    *(u16 *)(actor + 0x120) |= 0x1000;
    if (*(u32 *)(task + 8) & 0x10) {
        btlDispatchStateHandler(task, 0x1C);
    } else {
        btlDispatchStateHandler(task, 0x1B);
    }
}

void func_001DACE0(ActionStateLink *task) {
    task->unit->flags |= 0x4000;
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DACF8);

void func_001DB048(void) {
}

void btlCommandTaskStartEffects(ActionStateLink *task) {
    s32 delay;
    s32 countdown;
    s32 skipEffect = 0;
    BtlRuntimeTask *object;

    if ((task->unit->flags & 0x200) != 0 && (task->pendingFlags & 0x20) != 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
    }
    switch (task->indexWork.phase) {
    case 14:
        task->unit->flags |= 0x2000000;
        delay = 0xF;
        countdown = 0x64;
        break;
    case 18:
        delay = 0;
        countdown = 0;
        skipEffect = 1;
        break;
    case 10:
        if ((task->unit->flags & 0x400) != 0) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
        }
        delay = (task->unit->flags & 0x200) ? 0xF : 0x1E;
        countdown = 0x32;
        if (btlDoesEnabledStatusMatchCurrentId(&task->unit->statBits, 0xDF) != 0) {
            countdown = 0;
        }
        break;
    case 13:
        delay = 0xF;
        countdown = 0x64;
        break;
    default:
        delay = 0;
        countdown = 0;
        break;
    }
    if (!skipEffect) {
        btlStartTask(btlCreateEffObjA(0, task->indexWork.phase));
        object = fldCreateSceneGroupAction((u8 *)task, countdown, 1);
        object->startDelay = delay;
        object->ownerId = task->unit->owner;
        btlStartTask(object);
    }
    if ((task->unit->conditionFlags & 0x480) != 0) {
        btlDispatchStateHandler(task, 0x19);
    } else {
        btlDispatchStateHandler(task, 0x1B);
    }
}
void btlCommandTaskReturnStart(ActionStateLink *task) {
    BtlUnit *actor;
    BtlRuntimeTask *object;

    if (task->indexWork.linkedUnit != 0) {
        actor = task->indexWork.linkedUnit;
    } else {
        actor = task->unit;
    }
    if (actor->flags & 0x200) {
        btlBossDebugPrintf("return:player=%X[%X]\n", actor->unk2E4, actor->mode);
    } else {
        btlBossDebugPrintf("return:enemy=%X\n", actor->mode);
    }
    if (actor->flags & 0x400) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
    }
    if (!(actor->stateFlags & 0x40000)) {
        if (actor->flags & 0x200) {
            btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
            btlStartTask(btlCreateEffObjA(actor, task->indexWork.phase));
        } else if ((task->pendingFlags & 0x200) == 0) {
            btlStartTask(btlCreateCommandSoundTask((s32)task, 0x10));
            btlStartTask(btlCreateEffObjB(actor, 0xF));
        }
    }
    if (actor->flags & 0x200) {
        func_001AA868(&actor->statBits, 8);
        btlSyncPlayerWork(actor);
    }
    if (!(actor->stateFlags & 0x40000)) {
        if (btlCheckSpecialAbility(&task->unit->statBits, 0x280) == 0) {
            object = fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        } else {
            object = fldCreateSceneGroupAction((u8 *)task, 0x32, 1);
        }
        object->startDelay = 0xF;
        object->ownerId = task->unit->owner;
        btlStartTask(object);
    }
    object = (BtlRuntimeTask *)func_001E6428((s32)actor, 1);
    object->startDelay = 0xF;
    btlStartTask(object);
    actor->stateFlags &= ~0x40000;
}


void btlCommandTaskReturnUpdate(ActionStateLink *task) {
    BtlUnit *unit = task->unit;
    u32 flags = unit->flags;

    unit->flags = flags & ~1;
    if (flags & 0x200) {
        btlRepositionPartyAroundBattleCenter();
        func_001AC648();
    }
    if (btlCountTasksByKind(0x3F) != 0) {
        return;
    }
    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    if ((unit->flags & 0x40) == 0) {
        return;
    }
    if (fldReleaseIdleSceneActorResources(task->unit) != 0) {
        if (unit->flags & 0x200) {
            func_001AA868(&unit->statBits, 8);
            func_001AB160(unit);
        }
        fldUpdateSceneGroupTask((BtlTask *)task);
        btlRemoveTaskFromSceneGroup((BtlTask *)task);
        btlDispatchStateHandler(task, 0x1F);
    }
}

void btlStartLinkedActorEffectTask(ActionStateLink *unit) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *owner = unit->unit;
    u8 *task;
    if (!(owner->flags & 0x200)) {
        if (func_001B3200(owner) == 0) {
            task = (u8 *)btlCreateEffObjB(unit->unit, 0x67);
            task[0] = 0xA;
            *(u16 *)(task + 8) = 0x43;
            btlStartTask(task);
            if (unit->unit->conditionFlags & 0x480) {
                btlDispatchStateHandler(unit, 0x19);
                return;
            }
            btlDispatchStateHandler(unit, 0x1B);
            return;
        }
        work->unk27E += 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DB5E0);

void btlStartOwnerEffectTasks(s32 *arguments) {
    s32 owner = arguments[0x34 / 4];
    s32 value = btlCreateEffObjA(owner, arguments[0x20 / 4]);
    btlStartTask(value);
    value = func_001E6428(owner, 1);
    *(s32 *)(value + 0x28) = 7;
    btlStartTask(value);
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DBE70);

void btlRecordLinkedActorOutcome(ActionStateLink *unit) {
    BtlState *work = (BtlState *)btlGetRuntime();
    work->unk278 = work->unk278 + 1;
    if (func_001B2AF8(unit->unit) != 0) {
        work->battleFlags |= 0x2000;
    } else {
        work->battleFlags |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DC2D8);

void func_001DC538(void) {
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DC540);

void btlSpawnSceneActionAndSwitchState(void) {
}

void func_001DC7F8(u8 *unit) {
    u8 *task = fldCreateSceneGroupAction(unit, 0x1194, 1);

    btlStartTask(task);
    btlDispatchStateHandler(unit, 0x1b);
}

void func_001DC838(void) {
}

void btlAdvanceStateWhenLinkedTasksFinish(ActionStateLink *unit) {
    if (btlCountTasksForOwner(unit->unit->owner) == 0) {
        btlDispatchStateHandler(unit, 0x1D);
    }
}

void btlAdvanceLinkedUnitWhenOwnerIdle(void) {
}

void func_001DC890(ActionStateLink *unit) {
    BtlUnit *owner = unit->unit;
    if (btlCountTasksForOwner(owner->owner) == 0) {
        owner->flags &= ~0x4000;
        btlDispatchStateHandler(unit, 2);
    }
}

void btlFinalizeLinkedActionAndAdvanceHistory(void) {
}

void btlUnitTurnEndCommit(ActionStateLink *unit) {
    void (*hook)(struct ActionStateLink *) = ((BtlState *)btlGetRuntime())->commandTurnEndHook;
    BtlUnit *owner = unit->unit;
    if (hook != 0) {
        hook(unit);
    }
    btlAdvanceHistoryCounter(unit);
    btlResetIndexWork(&unit->indexWork);
    owner->unk314 = -1;
    unit->flags &= ~1;
    unit->completedTurns += 1;
    owner->flags &= ~0x4000;
    fldUpdateSceneGroupTask((BtlTask *)unit);
    if (unit->unit->flags & 0x20) {
        btlUnitTurnEndStateSelect((u8 *)unit);
    } else {
        btlDispatchStateHandler(unit, 2);
    }
}

extern void btlAccumulateEnemyDefeatRewards(BtlUnit *);
extern BtlRuntimeTask *btlCreateActorModelBlendTask(BtlUnit *, u32, u32, u32, f32);
extern BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *);
extern BtlRuntimeTask *sndCreateStationedSeTask(u32);

void btlStartActorDefeatTransition(ActionStateLink *command) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *actor = command->unit;
    u16 *profile = &actor->statBits;
    BtlRuntimeTask *soundTask;
    BtlRuntimeTask *object;
    s64 sequence;
    s32 entryFlags;
    s32 result;

    actor->unk310 = -1;
    func_001AA850(profile, 0x4000);
    btlGetSideIndexedActorStatusTable(actor->resourceKind, actor->resourceIndex);
    if (!(command->pendingFlags & 0x100)) {
        soundTask = btlCreateHookedUnitSoundTask(actor, 11);
        btlStartTask(soundTask);
        sequence = soundTask->handle;
    } else {
        sequence = btlAdvanceRuntimeSequenceCounter();
    }
    if (actor->flags & 0x200) {
        if (!(command->pendingFlags & 0x100)) {
            if (!(actor->flags & 0x8000000) && actor->unkEC != 11) {
                object = btlCreateActorModelBlendTask(actor, 0, 11, 2, 1.0f);
                object->startCondition.kind = 4;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
            }
            btlRefreshUnitMotionSelection(actor);
        }
    } else if (actor->flags & 0x400) {
        btlAccumulateEnemyDefeatRewards(actor);
        if (actor->flags & 0x8000000) {
            object = btlCreateSelectedEffectUpdateTask(actor);
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
            if (work->hook618 != NULL) {
                result = work->hook618(actor);
            }
            if ((entryFlags & 0x200) && result == 0) {
                result = 1;
                if (work->hook61C != NULL) {
                    result = work->hook61C(actor);
                }
                if (result != 0) {
                    if (actor->unkEC != 11) {
                        object = btlCreateActorModelBlendTask(actor, 0, 11, 2, 1.0f);
                        object->startCondition.kind = 4;
                        object->startCondition.value.handle = sequence;
                        btlStartTask(object);
                    }
                    btlRefreshUnitMotionSelection(actor);
                }
            } else {
                object = (BtlRuntimeTask *)func_001E6428((s32)actor, 0);
                object->startCondition.kind = 4;
                object->startCondition.value.handle = sequence;
                btlStartTask(object);
                actor->flags &= ~1;
            }
        }
        actor->statBits &= ~2;
    }
}

void btlRemoveEligibleActorSceneTask(ActionStateLink *task) {
    BtlUnit *unit = task->unit;
    BtlState *work;
    s32 hookResult;
    s32 entryFlags;
    if (unit->flags & 0x200) {
        if (fldReleaseIdleSceneActorResources(unit) == 0) {
            return;
        }
        btlResetIndexWork(&task->indexWork);
        unit->unk314 = -1;
        btlClearAllActorEntrySlots(unit);
        fldUpdateSceneGroupTask((BtlTask *)task);
        btlRemoveTaskFromSceneGroup((BtlTask *)task);
        btlDispatchStateHandler(task, 1);
    } else if (unit->flags & 0x400) {
        hookResult = 0;
        work = (BtlState *)btlGetRuntime();
        entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->statBits);
        if (work->hook618 != 0) {
            hookResult = work->hook618(unit);
        }
        if (!(unit->flags & 0x40)) {
            if (!(entryFlags & 0x200)) {
                return;
            }
            if (hookResult != 0) {
                return;
            }
        }
        fldUpdateSceneGroupTask((BtlTask *)task);
        btlRemoveTaskFromSceneGroup((BtlTask *)task);
        if ((entryFlags & 0x200) && !(unit->flags & 0x40) && hookResult == 0) {
            btlDispatchStateHandler(task, 1);
        } else {
            btlDispatchStateHandler(task, 0x1F);
        }
    }
}


void func_001DCD80(void) {
}

void btlCommandTaskReleaseActor(ActionStateLink *task) {
    if ((task->pendingFlags & 8) != 0) {
        if (fldReleaseIdleSceneActorResources(task->unit) == 0) {
            return;
        }
        if (task->unit != 0) {
            btlReleaseUnitResources(task->unit);
            {
                BtlUnit *fx = task->unit;
                fx->baseColor = 0x80808080;
                fx->overlayColor = 0x80808080;
                fx->flags = fx->flags & 0x700;
                btlInitUnitFxDefaults((BtlFx *)fx);
            }
            {
                BtlUnit *model = task->unit;
                PCP_COPY_VECTOR(model->currentPosition, model->position);
                PCP_COPY_VECTOR(model->orientation, model->rotation);
                model->unk310 = -1;
            }
            task->unit = 0;
            task->pendingFlags &= ~8;
        }
    }
    btlClearSceneTaskActiveFlag((s32)task);
    task->pendingFlags |= 2;
}

void btlFlagLinkedActorActionInProgress(ActionStateLink *unit) {
    unit->unit->flags = unit->unit->flags | 0x4000;
}

void btlRunHookAndAdvanceUnitState(ActionStateLink *unit) {
    void (*hook)(struct ActionStateLink *) = ((BtlState *)btlGetRuntime())->linkedActionHook;
    if (hook != 0) {
        hook(unit);
    }
    btlDispatchStateHandler(unit, 0x1B);
}

void func_001DCEB0(void) {
}

void func_001DCEB8(void) {
}

void func_001DCEC0(void) {
    func_0022F068();
}

void btlAdvanceUnitWhenActionGateClears(u32 unit) {
    s64 status;

    status = func_0022F180();
    if (status == 0) {
        btlDispatchStateHandler(unit, 6);
        return;
    }
}

void btlDispatchStateHandler(void *obj, s32 kind) {
    ((s32 *)obj)[0] = kind;
    ((s32 *)obj)[4] = 0;
    D_003B69D8[kind].start(obj);
}

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417508);

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417518);

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417528);

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417538);

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417548);

ActionStateLink *btlCreateActionSeq(void) {
    ActionStateLink *seq = sdfAllocAndClearQuadwords(0x180);
    BtlState *work;
    seq->actionNumber = 1;
    btlInitBattleIndexWork(&seq->indexWork);
    work = (BtlState *)btlGetRuntime();
    seq->prev = 0;
    if (work->tasks != 0) {
        work->tasks->prev = seq;
        seq->next = work->tasks;
    } else {
        seq->next = 0;
    }
    work->tasks = seq;
    btlDispatchStateHandler(seq, 0);
    btlBossDebugPrintf("btl:action seq create[%p]\n", seq);
    return seq;
}

void btlDestroyActionSeq(ActionStateLink *unit) {
    btlBossDebugPrintf("btl:action seq delete[%p]\n", unit);
    btlReleaseObjectBuffers(&unit->indexWork);
    if (unit->next != 0) {
        unit->next->prev = unit->prev;
    }
    if (unit->prev != 0) {
        unit->prev->next = unit->next;
    } else {
        ((BtlState *)btlGetRuntime())->tasks = unit->next;
    }
    sdfReleaseChipBlock(unit);
}

void btlUpdateActionSeqs(void) {
    ActionStateLink *unit;
    ActionStateLink *next;
    for (unit = ((BtlState *)btlGetRuntime())->tasks; unit != 0; unit = next) {
        next = unit->next;
        if (unit->pendingFlags & 1) {
            D_003B69D8[unit->state].update(unit);
            unit->stateTime++;
        } else if (unit->pendingFlags & 2) {
            btlDestroyActionSeq(unit);
        }
    }
}

void btlDestroyAllActionSeqs(void) {
    ActionStateLink *unit;
    ActionStateLink *next;
    for (unit = ((BtlState *)btlGetRuntime())->tasks; unit != 0; unit = next) {
        next = unit->next;
        btlDestroyActionSeq(unit);
    }
}

ActionStateLink *btlFindUnitByActor(BtlUnit *actor) {
    ActionStateLink *unit;
    for (unit = ((BtlState *)btlGetRuntime())->tasks; unit != 0; unit = unit->next) {
        if (unit->unit == actor) {
            return unit;
        }
    }
    return 0;
}

extern char D_00417598[];
extern char D_00436A10[];
extern char D_00436A18[];

/* Draw the debug overlay listing each battle slot's group and state name. */
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
    btlBossDebugPrintfN(x, y, 0, D_00417598);
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
            btlBossDebugPrintfN(x, y + (i + 1) * 12, color, D_00436A10,
                               D_003B69D8[task->state].name);
        } else {
            btlBossDebugPrintfN(x, y + (i + 1) * 12, color, D_00436A18,
                               D_003B69D8[task->state].name);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417598);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436920);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436928);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436930);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436938);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436940);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436948);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436950);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436958);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436960);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436968);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436970);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436978);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436980);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436988);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436990);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436998);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369A0);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369A8);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369B0);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369B8);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369C0);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369C8);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369D0);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369D8);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369E0);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369E8);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369F0);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_004369F8);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436A00);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436A08);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436A10);

INCLUDE_SDATA(const s32, "game/code_001D4438", D_00436A18);

INCLUDE_SDATA(const s32, "game/code_001D4438", btlDeferredTaskTail);

