#include "common.h"
#include "btl.h"

#include "pcp_vu0.h"

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

typedef struct BtlUnitData {
    u8 pad0[0x1C];
    f32 f1C;
    union {
        s32 unk20;
        f32 f20;
    };
    u8 pad24[10];
    u16 s2E;
    u8 b30;
} BtlUnitData;

typedef struct BtlUnitInfo {
    union {
        u8 b0;
        u32 flags;
    };
    u8 pad1[0x14];
    s32 unk18;
    BtlUnitData *data;
} BtlUnitInfo;

typedef struct BtlLightSource {
    s128 vec0;
    s128 vec10;
    u8 pad20[0x20];
    s128 vec40;
} BtlLightSource;

typedef struct BtlExtModel {
    u8 pad0[0x18];
    BtlLightSource *light;
} BtlExtModel;

typedef struct BtlUnit BtlUnit;

typedef struct BtlWork {
    u8 pad0[0x180];
    u32 runtimeFlags;        /* 0x180 */
    void *activeSlot;        /* 0x184 */
    u8 pad188[0xC];
    u32 activeUnitId;        /* 0x194 */
    u8 pad198[0x10];
    s32 pendingSoundList; /* 0x1A8 */
    u8 pad1AC[0x5C];
    s32 unk208;
    u8 pad20C[0xC];
    u32 battleFlags;
    u32 flags21C;
    u32 flags220;           /* 0x220 */
    u8 pad224[4];
    s32 unk228;
    s32 unk22C;
    u8 pad230[0x18];
    BtlUnit *head;
    BtlUnit *actorList;
    struct SoundTask *taskList250;
    struct SoundTask *taskList254;
    struct SoundResourceNode *resourceList258;
    struct ActiveSoundNode *soundList;
    struct SoundSlotOwner *soundSlotOwners;
    u8 pad264[4];
    u16 unk268;
    u8 pad26A[0xE];
    s32 unk278;
    u8 pad27C[2];
    u16 unk27E;
    u8 pad280[4];
    u16 earringPlaybackCount; /* 0x284 */
    u8 pad286[2];
    s32 unk288;
    u32 unk28C;
    u8 pad290[0x3C];
    s32 unk2CC;
    u8 pad2D0[0x18];
    s32 moneyEarned;
    u8 pad2EC[8];
    s32 experienceEarned;
    u8 pad2F8[0x1CC];
    s8 unk4C4;
    u8 pad4C5[3];
    f32 unk4C8;
    u8 pad4CC[0xB8];
    struct BtlBattleData *battleData; /* 0x584 */
    u8 pad588[0x28];
    void *primaryBuffer;
    void *secondaryBuffer;
    u8 fadeEnabled;
    u8 pad5B9[3];
    u32 fadeColor;
    s32 soundTransitionTask; /* 0x5C0 */
    u8 pad5C4[0x14];
    s32 (*hook5D8)(s32);
    u8 pad5DC[0x14];
    s32 (*hook5F0)(BtlUnit *, s32);
    u8 pad5F4[0x24];
    s32 (*hook618)(BtlUnit *);
    u8 pad61C[0x1C];
    void (*hook638)(BtlUnit *);
    u8 pad63C[0x10];
    s32 (*hook64C)(BtlUnit *);
    s32 (*hook650)(BtlUnit *);
    s32 (*hook654)(BtlUnit *);
    s32 (*hook658)(BtlUnit *);
    u8 pad65C[4];
    s32 (*hook660)(BtlUnit *, s32, s32);
    u8 pad664[4];
    s32 (*hook668)(BtlUnit *);
    s32 (*hook66C)(BtlUnit *);
    u8 pad670[0x14];
    s32 (*hook684)(s32, s32);
    s32 (*hook688)(s32, s32);
    u8 pad68C[0x10];
    s32 (*hook69C)(BtlUnit *);
    s32 (*hook6A0)(BtlUnit *);
    u8 pad6A4[4];
    void (*hook)(BtlUnit *);
    u8 pad6AC[0x34];
    s32 (*hook6E0)(BtlUnit *);
    s32 (*hook6E4)(BtlUnit *);
    s32 (*hook6E8)(BtlUnit *);
    u8 pad6EC[4];
    void (*hook6F0)(BtlUnit *, s32, f32, s32, s32, s32);
    u8 pad6F4[0x1C];
    s32 (*hook710)(BtlUnit *, s32);
    u8 pad714[8];
    u32 tint71C;
    u8 pad720[4];
    s32 unk724;
} BtlWork;

extern void btlResetIndexWork();
extern void btlAdvanceHistoryCounter(BtlUnit *);
extern void fldUpdateSceneGroupTask(BtlUnit *);
extern void btlUnitTurnEndStateSelect(BtlUnit *);

/* Command actor and its linked action/index state; distinct from BtlUnit. */
typedef struct BattleActionLinkState {
    u8 pad00[0x18];
    BtlUnit *unit;          /* 0x18 */
    u8 pad1C[0x28];
    s32 resourceNodeIndex;   /* 0x44: index into BtlEffectResource.nodes */
    u8 pad48[0x18];
    s32 actorIndices;       /* 0x60 */
    u8 pad64[0x24];
    u8 *entries;            /* 0x88 */
} BattleActionLinkState;

typedef struct ActionUnit {
    u8 pad00[0x30];
    f32 pos30[4];
    f32 dir40[4];
    u8 pad50[0x70];
    u8 outputPose[0x20];    /* 0xC0: receives the transformed pose */
    f32 fE0;                /* 0xE0 */
    u8 padE4[0x2C];
    u32 flags;              /* 0x110 */
    BattleActionLinkState *link;  /* 0x114 */
    u8 pad118[8];
    BtlUnit *focus;         /* 0x120 */
    u32 status;             /* 0x124 */
    s32 actionKind;         /* 0x128 */
    u16 stepKind;           /* 0x12C */
    u8 pad12E[6];
    s32 category;           /* 0x134 */
    s32 actorIndices;       /* 0x138 */
    s32 unk13C;
    u8 pad140[4];
    s32 stageCount;         /* 0x144 */
    u8 pad148[0xC];
    f32 unk154;
} ActionUnit;

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

typedef struct BtlIndexList {
    s32 capacity;       /* 0x00: allocated entry count */
    s32 count;          /* 0x04: live entry count */
    u32 *entries;       /* 0x08: points just past this header */
} BtlIndexList;

/* Pose command state: the progress slot is initialized as bits, then used as float. */
typedef struct BattlePoseBlendState {
    u8 pad00[0x130];
    s32 blendMode;           /* 0x130 */
    u8 pad134[8];
    s32 state13C;            /* 0x13C */
    u8 pad140[0xC];
    union {
        s32 progressBits;
        f32 progress;
    };                      /* 0x14C */
    s32 durationFrames;     /* 0x150 */
    f32 duration;           /* 0x154 */
} BattlePoseBlendState;

typedef struct BtlStateHandler {
    void (*start)(void *);
    void (*update)(void *);
    void (*finish)(void *);
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

typedef struct BtlAt3Entry {
    u8 volume;
    u8 pad01[3];
    char fileName[12];
} BtlAt3Entry;

extern BtlAt3Entry D_003E0F60[];

extern s128 D_003B6B80;

extern u8 D_003BD7D0[];

extern void evtSetUnitNormalizedDirection(BtlUnitExt *, s32);

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
    s32 actorIndices;   /* 0x40 */
    u8 pad44[0x24];
    BtlCommandOption *option; /* 0x68 */
} BtlCommandArgument;

typedef struct BtlShapeResource {
    u8 pad00[0x14];
    s32 itemKind;       /* 0x14 */
    s32 itemIndex;      /* 0x18 */
} BtlShapeResource;

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

extern struct SoundTask *btlCreateHookedUnitSoundTask();

extern u32 D_00436AD4;

extern u64 dds3AdvanceWorldCounter(void);

extern u32 evtSpawnActionObj9(u64);

extern s8 btlSetActorEffectParameter(BtlUnit *, s32);

extern s32 mdlFlagTest(u32);

extern s32 func_0022F180(void);

extern u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

extern s32 btlGetRuntime(void);

extern void func_001AA850(void *, s32);

extern void mdlStoreTertiaryVectorVU(s32);

extern void mdlSetAmountOnAllContextResources(f32, s32);

extern f32 func_001F5780(u32, u8, f32, f32);

extern f32 func_001FDD20(f32 *, f32, f32, s32);

extern struct SoundTask *btlDeferredTaskTail;

extern struct SoundTask *btlDeferredTaskHead;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 kwlnDrawControlFlags;

extern s32 mnuPollTitleStreamStateLocked(void);

extern void mnuResetTitleStreamLocked(void);

extern void func_002A2200(s32);

extern s32 D_00435E0C;

extern s32 datBattleSceneRecords;

typedef struct SoundSceneEntry {
    u8 pad00[0x24];
    u16 unk24;
    u8 pad26[2];
} SoundSceneEntry;

extern s32 sndFindPackedTrackLoadStatus(u32);

typedef struct SoundTask {
    u8 enabled;
    u8 unk_01[0xF];
    u8 status;
    u8 unk_11[0xF];
    u16 taskId;
    u16 unk_22;
    u16 flags;
    u8 unk_26[2];
    s32 startDelay;
    s32 endDelay;
    u32 unk_30;
    u32 unk_34;
    u64 unk_38;
    u64 owner;
    void (*onStart)(u32);
    s32 (*callback)();
    void (*onFinish)(u32 *);
    void *args;
    struct SoundTask *next;
    struct SoundTask *nextActive;
    struct SoundTask *deferNext;
    struct SoundTask *deferPrev;
} SoundTask;

extern s64 func_00201520(void);

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

extern void *sdfAllocAndClearQuadwords(s32);

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern s32 btlCountTasksForOwner(s64);

extern void btlRunTask(SoundTask *);

extern s32 btlBossDebugPrintf(const char *, ...);

extern s32 func_00232EE8(s32);

extern s32 func_00232EF8(s32);

extern f32 func_00208000(s32, s32, s32);

extern s32 func_0035C860();

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

extern SoundTask *btlAllocTask(s32);

extern SoundTaskArgs *btlGetTaskArguments(s32);

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

extern void func_001F02E0(s32, s32);
extern void func_001F0690(s32);
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

#define CURSOR ((SoundCursor *)D_003BD7D0)

extern char D_00418C58[];

extern void sdfFreeMemoryFromEitherHeap(void *);

extern s32 sndHasActiveFileLoad(void);

extern s32 fileGetResourceSize(s32);

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

extern s32 sndGetEffectNodeParameter(s32, u16);

extern u32 func_002D4138(u32);

typedef struct BtlCommandTask {
    s32 state;            /* 0x00 */
    u8 pad04[4];
    u32 flags;            /* 0x08 */
    u8 padC[0xC];
    BtlUnit *actor;       /* 0x18 */
    u8 pad1C[4];
    s32 kind;             /* 0x20 */
    u8 pad24[0x10];
    BtlUnit *linked;      /* 0x34 */
} BtlCommandTask;

extern s32 btlDoesEnabledStatusMatchCurrentId(void *, s32);
extern void btlUnitGetMuzzlePosVU(BtlUnit *);
extern s32 btlGetEntryFlagsUnlessDisabled(const void *);
extern void btlClearAllActorEntrySlots(BtlUnit *);
extern void btlReleaseUnitResources(BtlUnit *);
extern void btlInitUnitFxDefaults(BtlFx *);
extern void btlClearSceneTaskActiveFlag(BtlCommandTask *);

extern s32 btlCheckSpecialAbility(s32, s32);
extern void func_001AA868(void *, s32);
extern s32 btlCountTasksByKind(u16 kind);
extern void btlRepositionPartyAroundBattleCenter(void);
extern s32 func_001AC648(void);
extern void func_001F5868(s32, s32, s32, s32);
extern void func_001F5320(s32, s32, s32, s32);
extern s32 effCreateSelectionFlagListFromWork(void *);
extern char D_003BDCC8[];
extern void mnuReleaseSoundBufferLocked(void);
extern void func_0023CA60(u32, s32, u32);
extern void func_002A27A8(s32, s32, u8);
extern void mdlBroadcastMasked(s32, s32);
extern s32 fldReleaseIdleSceneActorResources(BtlUnit *);
extern void func_001AB160(BtlUnit *);
extern void btlRemoveTaskFromSceneGroup(BtlUnit *);
extern s32 func_001DACF8();

extern void func_001DB048(void);

extern void btlCommandTaskStartEffects(BtlCommandTask *task);
extern s32 btlCommandTaskReturnStart();

extern void btlCommandTaskReturnUpdate(BtlUnit *task);

extern void btlStartLinkedActorEffectTask(BtlUnit *unit);
extern s32 func_001DB5E0();

extern void btlStartOwnerEffectTasks(s32 *arguments);
extern s32 func_001DBE70();

extern void btlRecordLinkedActorOutcome(BtlUnit *unit);
extern s32 func_001DC2D8();

extern void func_001DC538(void);
extern s32 func_001DC540();

extern void btlSpawnSceneActionAndSwitchState(void);

extern void func_001DC7F8(u8 *unit);

extern void func_001DC838(void);

extern void btlAdvanceStateWhenLinkedTasksFinish(BtlUnit *unit);

extern void btlAdvanceLinkedUnitWhenOwnerIdle(void);

extern void func_001DC890(BtlUnit *unit);

extern void btlFinalizeLinkedActionAndAdvanceHistory(void);

extern void btlUnitTurnEndCommit(BtlUnit *unit);
extern s32 func_001DC9C0();
extern s32 btlRemoveEligibleActorSceneTask();

extern void func_001DCD80(void);

extern void btlCommandTaskReleaseActor(BtlCommandTask *task);

extern void btlFlagLinkedActorActionInProgress(s32 unit);

extern void btlRunHookAndAdvanceUnitState(BtlUnit *unit);

extern void func_001DCEB0(void);

extern void func_001DCEB8(void);

extern void func_001DCEC0(void);

extern void btlAdvanceUnitWhenActionGateClears(u32 unit);

extern void btlDispatchStateHandler(s32 *obj, s32 kind);

extern BtlUnit *btlCreateActionSeq(void);

extern void btlDestroyActionSeq(BtlUnit *unit);

extern void btlUpdateActionSeqs(void);

extern void btlDestroyAllActionSeqs(void);

extern BtlUnit *btlFindUnitByActor(BtlUnit *actor);
extern s32 func_001DD1A8();

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DD390);

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
    s32 value;
    switch (argument[0]) {
    case 1:
        if ((unit->flags64 & 0x1200) == 0x200 && (unit->statBits & 0x10) == 0) {
            return btlGetActorBedAssetIdFromIndex(unit->unk172);
        }
        if (argument[1] > 0) {
            return argument[1];
        }
        value = func_001B5688();
        if (value > 0) {
            return value;
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

u32 btlClassifyActionOperand(BtlUnit *unit, u8 *argument) {
    switch (((BtlCommandArgument *)argument)->command) {
    case 1: {
        u32 count = btlGetIndexListCount(((BtlCommandArgument *)argument)->actorIndices);
        if ((unit->flags & 0x200) && ((unit->flags & 0x1000) || (unit->statBits & 0x10)) &&
            (unit->conditionFlags & 0x1000) == 0 && count == 1) {
            BtlCommandOption *option = ((BtlCommandArgument *)argument)->option;
            if (option->kind == 2 && option->inactive == 0) {
                return 0x17;
            }
        }
        return 3;
    }
    case 4:
        return (unit->flags & 0x200) ? 0xC : 4;
    case 2:
    case 3:
    case 7:
    case 8: {
        s32 index = ((BtlCommandArgument *)argument)->index;
        if (index == 0xD6 && (unit->flags64 & 0x1200) == 0x200 && (unit->statBits & 0x10) == 0) {
            return 0xC;
        }
        return ((BtlActionTableEntry *)datActionAnimationRecords)[index].kind;
    }
    default:
        return 0;
    }
}

s32 btlClassifyActionResult(BtlUnit *actor, u32 arg1, s32 arg2, u32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 code;

    btlGetEntryFlagsUnlessDisabled(&actor->statBits);
    if (arg6 >= 0) {
        switch (((BtlCategoryTableEntry *)datCommandRecords)[arg6].categoryType) {
        case 1:
        case 2:
        case 9:
        case 10:
            return -1;
        case 22:
            if (actor->flags & 0x200) {
                return 0x12;
            }
            break;
        }
    }
    if ((((BtlCategoryTableEntry *)datCommandRecords)[arg6].flags24 & 0x400000FF) == 0x40000002) {
        return -1;
    }
    if (arg1 & 0x50004) {
        return -1;
    }
    if (arg3 & 0xE0001) {
        code = -1;
    } else if ((actor->flags & 0x200) != 0 && arg2 == 2 && arg4 == 1 && arg5 == 0) {
        code = 0x12;
    } else {
        code = 1;
    }
    if (arg5 != 0 && (actor->flags64 & 0x4000000200) == 0x200) {
        code = 0xB;
    }
    if ((arg1 & 0x20001) == 0) {
        code = -1;
    }
    return code;
}

typedef struct BtlOperandSlot {
    u8 pad00[8];
    s32 kind;              /* 0x08 */
} BtlOperandSlot;

typedef struct BtlOperandEntry {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0C;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1C;
    s32 f20;
    u8 pad24[4];
    u32 flags;             /* 0x28 */
} BtlOperandEntry;

s32 btlActionEntryIsEmpty(s32 index, BtlOperandSlot *slot, BtlOperandEntry *entry) {
    s32 kind;

    if (index >= 0) {
        switch (index) {
        case 0x109:
        case 0x179:
        case 0x19C:
        case 0x1A5:
            return 0;
        }
        switch (((BtlCategoryTableEntry *)datCommandRecords)[index].categoryType) {
        case 1:
        case 2:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
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
    if (entry->f00 == 0) {
        if (entry->f04 == 0) {
            if (entry->f08 == 0) {
                if (entry->f0C == 0) {
                    if (entry->f1C == 0) {
                        if (entry->f20 == 0) {
                            if (entry->f10 == 0) {
                                if (entry->f18 == 0) {
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

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DDAD0);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DDB60);

typedef struct BattleIndexEntry {
    u8 unk0;
    u8 pad1[7];
    s32 unk8;
    u8 padC[4];
    u8 unk10;
    u8 pad11[3];
    u8 unk14;
    u8 pad15[0x587];
} BattleIndexEntry;

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
    void *indices;
    u32 unk44;
    u64 previous;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u16 unk5C;
    u8 pad5E[2];
    s32 unk60;
    u8 unk64;
    u8 pad65[3];
    u32 device;
    u32 command;
} BattleIndexWork;

void btlResetIndexWork(BattleIndexWork *work) {
    u32 i;
    s32 offset;
    BattleIndexEntry *entry;
    BattleIndexEntry *next;
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
    work->unk64 = 0;
    work->unk60 = 0;
    for (i = 0, offset = 0; i < 13; i++) {
        *(u8 *)(offset + work->device) = 0;
        entry = (BattleIndexEntry *)(offset + work->device);
        entry->unk8 = 0;
        entry->unk14 = 0;
        next = (BattleIndexEntry *)(offset + work->device);
        offset += 0x59C;
        next->unk10 = 0;
    }
    btlClearIndexList(work->indices);
}

extern void *btlAllocateIndexList(s32);

extern u32 sdfAllocGeneralBlock(s32);

extern u32 sdfResourceRetainAddress(u32);

void btlInitBattleIndexWork(BattleIndexWork *work) {
    u32 command;
    work->indices = btlAllocateIndexList(13);
    command = sdfAllocGeneralBlock(0x48EC);
    work->device = sdfResourceRetainAddress(command);
    work->command = command;
    work->previous = 0;
    btlResetIndexWork(work);
}

void btlReleaseObjectBuffers(s32 *object) {
    if (object[0x6C / 4] != 0) {
        sdfReleaseResourceAllocation(object[0x6C / 4]);
        object[0x6C / 4] = 0;
    }
    if (object[0x40 / 4] != 0) {
        btlFreeIndexList(object[0x40 / 4]);
        object[0x40 / 4] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DF860);

extern u32 func_001DF860(s32);

typedef struct TaskBlock {
    u32 word[11];
} TaskBlock;

SoundTask *btlCreateActorParameterDeltaTask(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x49;
    task->owner = unit->owner;
    task->callback = func_001DF860;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 btlApplyDeferredActorStats(u8 *arguments) {
    s32 context = btlGetRuntime();
    u8 *actor = *(u8 **)arguments;
    s32 primary;
    u8 *resource;
    if ((((BtlWork *)context)->battleFlags & 0x80) == 0) {
        return 1;
    }
    primary = ((BtlDeferredStats *)arguments)->primary;
    if (primary == 0 && ((BtlDeferredStats *)arguments)->secondary == 0) {
        return 1;
    }
    if (((BtlUnit *)actor)->flags & 0x60) {
        return 1;
    }
    resource = actor + 0x120;
    btlAdjustUnitHp(resource, primary);
    btlAdjustUnitMp(resource, ((BtlDeferredStats *)arguments)->secondary);
    func_001E2758(actor);
    btlIsUnitDefeatTriggeredByValueDelta(actor, 0);
    return 1;
}

SoundTask *btlCreateDeferredActorStatsTask(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4A;
    task->owner = unit->owner;
    task->callback = btlApplyDeferredActorStats;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 btlApplyDeferredUnitStatus(void *arg) {
    s32 *args = arg;
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit = (BtlUnit *)args[0];
    if (!(work->battleFlags & 0x80)) {
        return 1;
    }
    func_001AA850(&unit->statBits, args[1]);
    func_001E2758(unit);
    btlIsUnitDefeatTriggeredByValueDelta(unit, 0);
    return 1;
}

SoundTask *btlCreateDeferredUnitStatusTask(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4B;
    task->owner = actor->owner;
    task->callback = btlApplyDeferredUnitStatus;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

typedef struct BtlStatArgs {
    BtlUnit *unit;
    s32 amount;
    s32 category;
} BtlStatArgs;

s32 btlApplyCategoryStatDamage(BtlStatArgs *args) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit = args->unit;
    if (!(work->battleFlags & 0x80)) {
        return 1;
    }
    if (((BtlCategoryTableEntry *)datCommandRecords)[args->category].flags00 & 8) {
        btlAdjustUnitHp(&unit->statBits, -0x7FFF);
        func_001AA850(&unit->statBits, 0x4000);
        unit->flags |= 0x20;
    }
    if (args->amount == 0) {
        return 1;
    }
    switch (((BtlCategoryTableEntry *)datCommandRecords)[args->category].kind03) {
    case 1:
        btlAdjustUnitHp(&unit->statBits, -args->amount);
        return 1;
    case 2:
        btlAdjustUnitMp(&unit->statBits, -args->amount);
        return 1;
    default:
        return 1;
    }
}

SoundTask *btlCreateCategoryStatDamageTask(BtlUnit *unit, u32 target, u32 option) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlApplyCategoryStatDamage;
    task->taskId = 0x4C;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->unk_08 = target;
    args->option = option;
    return task;
}

s32 func_001DFFE0(s32 taskArgs) {
    s32 args;

    args = taskArgs;
    func_001ADFE0(*(s32 *)args, *(s32 *)(args + 0x14), *(s16 *)(args + 0x18));
    return 1;
}

SoundTask *btlCreateMaskedActorEntryUpdateTask(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4D;
    task->owner = unit->owner;
    task->callback = func_001DFFE0;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 btlApplyQueuedActorEntrySelection(u32 *taskArgs) {
    if (0 < (s32)taskArgs[7]) {
        btlSetActorSelectedEntryIndex(*taskArgs, taskArgs[7]);
        func_001E2758(*taskArgs);
    }
    return 1;
}

SoundTask *btlCreateQueuedActorEntrySelectionTask(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4E;
    task->owner = unit->owner;
    task->callback = btlApplyQueuedActorEntrySelection;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 btlClearQueuedActorEntrySelection(u32 *taskArgs) {
    btlClearActorSelectedEntryIndex(*taskArgs);
    func_001E2758(*taskArgs);
    return 1;
}

SoundTask *func_001E0238(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlClearQueuedActorEntrySelection;
    task->taskId = 0x4F;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E02A8);

extern u32 func_001E02A8(s32);

SoundTask *btlCreateActorSoundOptionTask(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x50;
    task->owner = actor->owner;
    task->callback = func_001E02A8;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

typedef struct {
    u32 unk0;
    s32 soundIndex;
} BattleVoiceWork;

extern u8 *datItemSkillRecords;

extern void func_0011A118(s32, s32);

extern void btlSyncModelFlagFromEventThresholds(void);

s32 btlPlayPermittedBattleVoice(BattleVoiceWork *work) {
    s32 index = work->soundIndex;
    if (datItemSkillRecords[index * 8 + 1] & 4) {
        func_0011A118(index, -1);
        switch (work->soundIndex) {
        case 0x53:
        case 0x54:
            btlSyncModelFlagFromEventThresholds();
            break;
        }
    }
    return 1;
}

SoundTask *btlCreatePermittedBattleVoiceTask(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x51;
    task->owner = actor->owner;
    task->callback = btlPlayPermittedBattleVoice;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 btlPlayQueuedBattleVoice(s32 taskArgs) {
    func_0011A118(*(u16 *)(taskArgs + 4), 1);
    return 1;
}

SoundTask *btlCreateQueuedBattleVoiceTask(BtlUnit *actor, u16 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x52;
    task->owner = actor->owner;
    task->callback = btlPlayQueuedBattleVoice;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->optionId = option;
    return task;
}

/* Task payload shared by the experience and money reward callbacks. */
typedef struct BattleRewardPacket {
    BtlUnit *actor;
    s32 amount;
} BattleRewardPacket;

u32 btlAddEpFromPacket(s32 packetAddress) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BattleRewardPacket *packet = (BattleRewardPacket *)packetAddress;
    if (packet->amount == 0) {
        return 1;
    }
    if (packet->actor->flags & 0x400) {
        return 1;
    }
    work->experienceEarned += packet->amount;
    btlBossDebugPrintf("btl:epall=%d[%d](packet)\n", work->experienceEarned, packet->amount);
    return 1;
}

extern u32 btlAddEpFromPacket(s32);

SoundTask *btlScheduleEpPacketTask(BtlUnit *actor, s32 amount) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x53;
    task->owner = actor->owner;
    task->callback = btlAddEpFromPacket;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = amount;
    return task;
}

u32 btlAddMoneyFromPacket(s32 packetAddress) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BattleRewardPacket *packet = (BattleRewardPacket *)packetAddress;
    if (packet->amount == 0) {
        return 1;
    }
    if (packet->actor->flags & 0x400) {
        return 1;
    }
    work->moneyEarned += packet->amount;
    btlBossDebugPrintf("btl:money=%d[%d](packet)\n", work->moneyEarned, packet->amount);
    return 1;
}

extern u32 btlAddMoneyFromPacket(s32);

SoundTask *btlScheduleMoneyPacketTask(BtlUnit *actor, s32 amount) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x54;
    task->owner = actor->owner;
    task->callback = btlAddMoneyFromPacket;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = amount;
    return task;
}

extern s32 datEnemyRecords;

u32 btlRefreshEligibleActors(void) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit = work->actorList;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 && (u16)(unit->mode - 1) < 0x17F) {
                    u32 entry = ((BtlResourceTableEntry *)datEnemyRecords)[unit->mode].flags;
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((unit->stateFlags & 8) == 0) {
                                u16 prior = unit->conditionFlags;
                                func_001AA850(&unit->statBits, 1);
                                func_001E2758(unit);
                                if (unit->conditionFlags == 1 && prior != unit->conditionFlags) {
                                    unit->stateFlags |= 4;
                                    work->flags21C |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    return 1;
}

SoundTask *btlCreateRefreshEligibleActorsTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlRefreshEligibleActors;
    task->taskId = 0x55;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 btlApplyQueuedCurrencyReward(s32 taskArgs) {
    datAddCurrencyClamped(*(u32 *)(taskArgs + 4));
    return 1;
}

SoundTask *btlCreateCurrencyRewardTask(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x56;
    task->owner = actor->owner;
    task->callback = btlApplyQueuedCurrencyReward;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

void btlUpdateAutoMusic(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u32 flags = ((BtlWork *)context)->battleFlags;
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if ((s8)D_0037F510[0x22] < 0 || (s8)D_0037F510[0x23] < 0) {
            ((BtlWork *)context)->battleFlags = flags & ~0x8000;
            sndSetSequenceVolumePan(6, 0x7F, 0x3F);
            btlSetTrackedTaskDisplayMode(0);
            btlBossDebugPrintf(D_004178A8);
        }
    } else if ((s8)D_0037F510[0x22] < 0) {
        ((BtlWork *)context)->battleFlags = flags | 0x8000;
        sndSetSequenceVolumePan(5, 0x7F, 0x3F);
        btlSetTrackedTaskDisplayMode(1);
        btlBossDebugPrintf(D_004178B8);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E0CE0);

void btlFindSoundTaskByWorkValue(void) {
}

SoundTask *btlFindTaskByHandle(u64 value) {
    SoundTask *task;
    for (task = ((BtlWork *)btlGetRuntime())->taskList254; task != 0; task = task->next) {
        if (task->unk_38 == value) {
            return task;
        }
    }
    return 0;
}

SoundTask *btlFindTaskByOwner(u64 owner) {
    SoundTask *task;
    for (task = ((BtlWork *)btlGetRuntime())->taskList254; task != 0; task = task->next) {
        if (task->owner == owner) {
            return task;
        }
    }
    return 0;
}

SoundTask *btlFindTaskByKind(u16 kind) {
    SoundTask *task;
    for (task = ((BtlWork *)btlGetRuntime())->taskList254; task != 0; task = task->next) {
        if (task->taskId == kind) {
            return task;
        }
    }
    return 0;
}

s32 btlCountRegisteredTasks(void) {
    s32 task;
    s32 count;

    task = btlGetRuntime();
    count = 0;
    for (task = (s32)((BtlWork *)task)->taskList254; task != 0; task = (s32)((SoundTask *)task)->next) {
        count = count + 1;
    }
    return count;
}

s32 btlCountTasksForOwner(s64 owner) {
    s32 count = 0;
    SoundTask *task;
    for (task = ((BtlWork *)btlGetRuntime())->taskList254; task != 0; task = task->next) {
        if (task->owner == owner) {
            count++;
        }
    }
    return count;
}

s32 btlCountTasksByKind(u16 kind) {
    s32 count = 0;
    SoundTask *task;
    for (task = ((BtlWork *)btlGetRuntime())->taskList254; task != 0; task = task->next) {
        if (task->taskId == kind) {
            count++;
        }
    }
    return count;
}

void btlFlagTasksForUpdate(void) {
    SoundTask *task;
    SoundTask *next;
    for (task = ((BtlWork *)btlGetRuntime())->taskList250; task != 0; task = next) {
        u16 flags = task->flags;
        next = task->nextActive;
        if (flags & 1) {
            task->flags = flags | 4;
        }
    }
}

typedef struct SoundCondition {
    u8 kind;
    u8 pad1[7];
    union {
        s32 count;
        u64 key;
        u16 kindId;
    };
} SoundCondition;

extern SoundTask *btlFindTaskByHandle(u64);
extern SoundTask *btlFindTaskByOwner(u64);
extern SoundTask *btlFindTaskByKind(u16);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004178A8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004178B8);

s32 btlEvalTaskCondition(SoundCondition *cond, s32 value) {
    s32 result = 0;
    SoundTask *task;
    switch (cond->kind) {
    case 0:
        break;
    case 1:
        result = 1;
        break;
    case 2:
        if (!(value < cond->count)) {
            result = 1;
        }
        break;
    case 3:
        if (btlFindTaskByHandle(cond->key) != 0) {
            result = 1;
        }
        break;
    case 4:
        if (btlFindTaskByHandle(cond->key) == 0) {
            result = 1;
        }
        break;
    case 5:
        task = btlFindTaskByHandle(cond->key);
        if (task != 0) {
            if (task->unk_22 == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 6:
        if (btlFindTaskByOwner(cond->key) != 0) {
            result = 1;
        }
        break;
    case 7:
        if (btlFindTaskByOwner(cond->key) == 0) {
            result = 1;
        }
        break;
    case 8:
        task = btlFindTaskByOwner(cond->key);
        if (task != 0) {
            if (task->unk_22 == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 9:
        if (btlFindTaskByKind(cond->kindId) != 0) {
            result = 1;
        }
        break;
    case 10:
        result = btlFindTaskByKind(cond->kindId) == 0;
        break;
    }
    return result;
}

extern void *sdfAllocAndClearQuadwords(s32);

SoundTask *btlAllocTask(s32 size) {
    SoundTask *task = sdfAllocAndClearQuadwords(size + 0x70);
    BtlWork *work;
    if (size > 0) {
        task->args = (u8 *)task + 0x70;
    } else {
        task->args = 0;
    }
    work = (BtlWork *)btlGetRuntime();
    task->next = 0;
    if (work->taskList250 != 0) {
        work->taskList250->next = task;
        task->nextActive = work->taskList250;
    } else {
        work->taskList254 = task;
        task->nextActive = 0;
    }
    work->taskList250 = task;
    task->flags |= 1;
    return task;
}

SoundTaskArgs *btlGetTaskArguments(s32 task) {
    return *(SoundTaskArgs **)(task + 0x54);
}

extern void sdfReleaseChipBlock(void *);

void btlFreeTask(SoundTask *task) {
    BtlWork *work;
    if (task->onFinish != 0) {
        task->onFinish((u32 *)task->args);
    }
    work = (BtlWork *)btlGetRuntime();
    if (task->nextActive != 0) {
        task->nextActive->next = task->next;
    } else {
        work->taskList254 = task->next;
    }
    if (task->next != 0) {
        task->next->nextActive = task->nextActive;
    } else {
        work->taskList250 = task->nextActive;
    }
    sdfReleaseChipBlock(task);
}

u64 btlStartTask(task)
    SoundTask *task;
{
    task->unk_38 = btlAdvanceRuntimeSequenceCounter();
    task->flags |= 8;
    task->unk_30 = 0;
    task->unk_34 = 0;
    task->unk_22 = 0;
    task->deferNext = 0;
    task->deferPrev = 0;
    if (task->onStart != 0) {
        task->onStart((u32)task->args);
    }
    return task->unk_38;
}

void btlResetDeferredTaskQueue(void) {
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

void btlRunTask(SoundTask *task) {
    u32 counter;
    if (!(task->flags & 8)) {
        return;
    }
    if (task->flags & 4) {
        btlFreeTask(task);
        return;
    }
    counter = task->unk_30;
    task->unk_30 = counter + 1;
    switch (task->unk_22) {
    case 0:
        if (btlEvalTaskCondition(task, counter) == 0) {
            break;
        }
        task->unk_22 = 1;
    case 1:
        if (task->startDelay <= 0) {
            task->unk_22 = 2;
        } else {
            task->startDelay = task->startDelay - 1;
            break;
        }
    case 2:
        if (btlEvalTaskCondition((u8 *)task + 0x10, task->unk_34) != 0) {
            task->unk_22 = 3;
        } else if (task->callback(task->args) != 0) {
            task->unk_22 = 3;
        } else {
            task->unk_34 = task->unk_34 + 1;
            break;
        }
    case 3:
        if (task->endDelay <= 0) {
            btlFreeTask(task);
        } else {
            task->endDelay = task->endDelay - 1;
        }
        break;
    }
}

void btlSweepFinishedTasks(void) {
    SoundTask *task;
    SoundTask *next;
    for (task = ((BtlWork *)btlGetRuntime())->taskList254; task != 0; task = next) {
        next = task->next;
        if (!(task->flags & 2)) {
            btlRunTask(task);
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
    }
}

void btlClearDeferredTasks(void) {
    SoundTask *node = btlDeferredTaskHead;
    while (node != 0) {
        SoundTask *next = node->deferNext;
        btlRunTask(node);
        node = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

void btlClearTaskLists(void) {
    SoundTask *task;
    SoundTask *next;
    for (task = ((BtlWork *)btlGetRuntime())->taskList250; task != 0; task = next) {
        next = task->nextActive;
        btlFreeTask(task);
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

u32 func_001E1848(void) {
    return 1;
}

SoundTask *btlCreateImmediateCompletionTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_001E1848;
    task->taskId = 0x6A;
    task->onStart = 0;
    task->status = 0;
    return task;
}

void btlDumpTaskQueue(void) {
    s32 context = btlGetRuntime();
    s32 node = (s32)((BtlWork *)context)->taskList254;
    while (node != 0) {
        btlBossDebugPrintf("btl:packet[%d]\n", ((SoundTask *)node)->taskId);
        node = (s32)((SoundTask *)node)->next;
    }
    btlBossDebugPrintf("btl:packet head[%p]\n", *(void **)(context + 0x250));
    btlBossDebugPrintf("btl:packet tail[%p]\n", *(void **)(context + 0x254));
}

void btlInitUnitFxDefaults(BtlFx *fx) {
    PCP_COPY_VECTOR(&fx->vec90, &D_003B6B80);
    fx->fB0 = 220.0f;
    fx->fB4 = 80.0f;
    fx->fC0 = 75.0f;
}

extern s128 D_003B6B90;

extern s128 D_003B6BA0;

typedef struct BtlFxLight {
    s128 vecA;
    s128 vecB;
    f32 intensity;
    u32 color;
    u32 unk8;
} BtlFxLight;

typedef struct BtlFxLights {
    u8 pad0[0x30];
    BtlFxLight light0;
    BtlFxLight light1;
} BtlFxLights;

void btlInitFxLights(BtlFxLights *fx) {
    PCP_COPY_VECTOR(&fx->light0.vecA, &D_003B6B90);
    PCP_COPY_VECTOR(&fx->light0.vecB, &D_003B6BA0);
    fx->light0.unk8 = 0;
    fx->light0.intensity = 1.0f;
    fx->light0.color = 0x80808080;
    PCP_COPY_VECTOR(&fx->light1.vecA, &D_003B6B90);
    PCP_COPY_VECTOR(&fx->light1.vecB, &D_003B6BA0);
    fx->light1.intensity = 1.0f;
    fx->light1.color = 0x80808080;
    fx->light1.unk8 = 0;
}

extern void *btlSelectSharedOrIndexedTransformParameters(s32, s32);

void btlInitializeEffectVectorsFromSourceRecords(BtlFx *fx, s32 kind, s32 index) {
    BtlFxSrcA *alt = btlSelectSharedOrIndexedTransformParameters(kind, index);
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

s32 btlHasMatchingModel(s32 effect, s32 model) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit = work->actorList;
    while (unit != NULL) {
        if ((unit->flags & 2) != 0 &&
            unit->ext != NULL &&
            unit->unk328 != 0 &&
            func_00232EE8((s32)unit->ext->info) == effect &&
            func_00232EF8((s32)unit->ext->info) == model) {
            return 1;
        }
        unit = unit->nextActor;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E1B80);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E1BB8);

extern void sndReleaseSlotOwner(struct SoundSlotOwner *);
extern void sdfReleaseDevSlot(s32, s32, s32);
extern void dds3RemoveWorldObjectNode(s32);
extern char D_00417940[]; /* "btl:unit transparency delete[%p]\n" */

void btlReleaseActorModelResources(BtlUnit *unit) {
    if (unit->unkCC == 0) {
        if (unit->unk328 != 0) {
            sndReleaseSlotOwner((struct SoundSlotOwner *)unit->unk328);
            unit->unk328 = 0;
        }
        if (unit->unk344 != 0) {
            sdfReleaseDevSlot(unit->unk344, 1, 1);
            unit->unk344 = 0;
            btlBossDebugPrintf(D_00417940, unit);
        }
        if (unit->effectObject != 0) {
            dds3RemoveWorldObjectNode(unit->effectObject);
            unit->effectObject = 0;
            unit->ext = 0;
        }
    } else {
        unit->unk328 = 0;
        unit->unk344 = 0;
        unit->effectObject = 0;
        unit->ext = 0;
    }
    unit->gunResourceFlags &= ~1;
    unit->flags &= ~2;
    unit->gunResourceFlags &= ~2;
}

void btlRequestModelAssetByMode(u32 unused, u32 effect, u32 model) {
    s64 available;

    available = mdlFlagTest(0xc0f);
    if (available != 0) {
        func_0022CD60(effect, model);
        return;
    }
    mdlRequestAsset(effect, model, 0);
}

void btlReleaseModelAssetByMode(u32 unused, u32 effect, u32 model) {
    s64 available;

    available = mdlFlagTest(0xc0f);
    if (available != 0) {
        btlReleaseFoundModelEntry(effect, model);
        return;
    }
}

s32 btlCheckModelAssetByMode(u8 *object, u32 effect, u32 model) {
    if (mdlFlagTest(0xC0F) != 0) {
        if (func_0022CD60(effect, model, 0) != 0) {
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
    s32 (*hook)(BtlUnit *) = ((BtlWork *)btlGetRuntime())->hook69C;
    if (hook == 0 || hook(unit) != 0) {
        unit->flags |= 4;
        if (!(unit->flags & 0x8000000)) {
            unit->flags |= 8;
            if (unit->flags & 2) {
                unit->ext->info->flags &= ~1;
            }
        }
    }
}

void btlClearUnitDefeatCandidate(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlWork *)btlGetRuntime())->hook6A0;
    if (hook == 0 || hook(unit) != 0) {
        unit->flags &= ~4;
        unit->flags &= ~8;
        if (unit->flags & 2) {
            unit->ext->info->flags |= 1;
        }
    }
}

u32 btlIsUnitInfoFlagOneEligible(BtlUnit *unit) {
    if (unit->flags & 0x8000000) {
        return 0;
    }
    if (!(unit->flags & 1)) {
        return 0;
    }
    if (!(unit->flags & 2)) {
        return 0;
    }
    return unit->ext->info->b0 & 1;
}

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417940);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E22D8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E2758);

s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *unit) {
    BtlWork *work;
    if (!(unit->flags & 2)) {
        return 0;
    }
    work = (BtlWork *)btlGetRuntime();
    if (work->hook5D8 != 0 && work->hook5D8(0) == unit->unkEC) {
        return 1;
    }
    switch (unit->unkEC) {
    case 0:
    case 2:
    case 9:
    case 10:
    case 11:
        return 1;
    default:
        return 0;
    }
}

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);

extern void func_001E22D8(u8 *, s32, s32, f32);

/* Per-model effect node records begin at +0x2C, with a 0x14-byte stride. */
typedef struct BtlEffectNode {
    s16 triggerKind;
    u8 pad02[2];
    s16 rateKind;
    u8 pad06[2];
    f32 scale;
    u8 pad0C[8];
} BtlEffectNode;

typedef struct BtlEffectResource {
    u8 pad00[0x2C];
    BtlEffectNode nodes[1];
} BtlEffectResource;

void btlApplyScaledUnitEffectParameter(u8 *unit, s32 index, s32 option, f32 scale) {
    u8 *table = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)unit)->resourceKind, ((BtlUnit *)unit)->resourceIndex);
    func_001E22D8(unit, index, option, ((BtlEffectResource *)table)->nodes[index].scale * scale);
}

s32 btlGetSlotRateKind(u8 *unit, s32 index) {
    u8 *table = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)unit)->resourceKind, ((BtlUnit *)unit)->resourceIndex);
    s32 value = ((BtlEffectResource *)table)->nodes[index].rateKind;
    switch (value) {
    case 0:
        return 0;
    case 1:
    case 2:
    case 3:
        return 2;
    default:
        return 0;
    }
}

void btlUpdateUnitEffects(void) {
    s32 context = btlGetRuntime();
    u8 *object = *(u8 **)(context + 0x24C);

    while (object != 0) {
        if (((BtlUnit *)object)->flags & 2) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)object)->resourceKind,
                                                  ((BtlUnit *)object)->resourceIndex);
            s32 model = (s32)((BtlUnit *)object)->ext->info;
            s32 node = mdlGetNodeField2C(model, 0);
            if (((BtlEffectResource *)resource)->nodes[node].rateKind == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                func_001E2758(object);
                func_001E22D8(object, ((BtlUnit *)object)->effectIndex,
                              ((BtlUnit *)object)->effectParameter,
                              ((BtlUnit *)object)->effectScale);
            }
        }
        object = *(u8 **)(object + 0x364);
    }
}

void btlApplyUnitModelScaledValue(u8 *object) {
    s32 context;
    u8 *resource;
    f32 volume;
    if ((((BtlUnit *)object)->flags & 2) == 0) {
        return;
    }
    context = btlGetRuntime();
    ((BtlUnit *)object)->updateFlags &= ~1;
    volume = ((BtlUnit *)object)->fF4;
    resource = *(u8 **)(object + 0x340);
    *(f32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) =
        volume * (30.0f / (f32)((BtlWork *)context)->unk4C4);
}

void btlResetUnitModelProgress(BtlUnit *unit) {
    if (unit->flags & 2) {
        unit->updateFlags |= 1;
        unit->ext->info->data->unk20 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E2E58);

f32 btlGetUnitModelValue1C(BtlUnit *unit) {
    f32 value = 0.0f;
    if (unit->flags & 2) {
        value = unit->ext->info->data->f1C;
    }
    return value;
}

void btlAdvanceUnitModelFrame(BtlUnit *unit) {
    if ((unit->flags & 2) != 0) {
        sdfMotionSampleAtFrame((u32)unit->ext->info->data);
        return;
    }
}

u16 btlGetUnitModelFrameCount(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return 0;
    }
    return unit->ext->info->data->s2E;
}

/* This unit preserves the retail caller's integer view of the final $v0
 * scratch value. The motion routine itself has no semantic return contract. */
extern s32 sdfMotionSampleAtFrame(BtlUnitData *, f32);

void btlSeekUnitModelFrameZero(BtlUnit *unit) {
    if (unit->flags & 2) {
        sdfMotionSampleAtFrame(unit->ext->info->data, 0.0f);
    }
}

extern u32 effMiscRandMod(s32, s32);

s64 btlSeekRandomModelFrame(BtlUnit *unit) {
    s32 count;
    f32 amount;
    if (unit->flags & 2) {
        count = btlGetUnitModelFrameCount(unit);
        if (count > 0) {
            amount = effMiscRandMod(0, count);
            return sdfMotionSampleAtFrame(unit->ext->info->data, amount);
        }
    }
}

s32 btlIsUnitModelStateFive(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return 1;
    }
    if (unit->effectState != 2) {
        return 1;
    }
    return unit->ext->info->data->b30 == 5;
}

extern void effObjSetInnerFirstVec(s32, f32 *);

void btlSetUnitPosition(BtlUnit *unit, f32 *vec) {
    f32 pos[4];
    if (!(unit->stateFlags & 0x80)) {
        u8 *work = (u8 *)btlGetRuntime();
        VU0_LOAD_VF(vf10, vec);
        VU0_STORE_VF_UNCLOBBERED(vf10, (u8 *)unit + 0x60);
        VU0_LOAD_VF(vf11, work);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        if (unit->flags & 2) {
            pos[2] += unit->positionZOffset;
            effObjSetInnerFirstVec(unit->effectObject, pos);
        }
    }
}

void func_001E3108(u8 *unit, s128 *dst) {
    PCP_COPY_VECTOR(dst, unit + 0x60);
}

void btlGetUnitWorldPos(u8 *unit, s128 *dst) {
    u8 *work = (u8 *)btlGetRuntime();
    VU0_LOAD_VF(vf10, unit + 0x60);
    VU0_LOAD_VF(vf11, work);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, dst);
}

extern s32 sdfLoadMapRecordPositionVector(s32, s32);

extern void btlRefreshUnitFxVectors(BtlUnit *);

s8 btlSetActorEffectParameter(BtlUnit *unit, s32 mode) {
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->flags & 2)) {
        return 0;
    }
    hook = ((BtlWork *)btlGetRuntime())->hook5F0;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlRefreshUnitFxVectors(unit);
    return sdfLoadMapRecordPositionVector(unit->ext->info->unk18, mode);
}

void btlSetActorEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode) {
    if (btlSetActorEffectParameter(unit, mode) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E3230);

extern s32 sdfLoadMapRecordLookAtBasis(s32, s32);

s8 btlSetActorAlternateEffectParameter(unit, mode)
    BtlUnit *unit;
    s32 mode;
{
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->flags & 2)) {
        return 0;
    }
    hook = ((BtlWork *)btlGetRuntime())->hook5F0;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlRefreshUnitFxVectors(unit);
    return sdfLoadMapRecordLookAtBasis(unit->ext->info->unk18, mode);
}

void btlSetAlternateEffectParameterOrMuzzlePosition(void) {
    if (btlSetActorAlternateEffectParameter() == 0) {
        VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
    }
}

s32 btlIsUnitAtStoredPosition(u8 *object) {
    f32 position[3];
    func_001E3108(object, position);
    if (((BtlUnit *)object)->positionX == position[0] &&
        ((BtlUnit *)object)->positionY == position[1] &&
        ((BtlUnit *)object)->positionZ == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_004179E0[];

extern void effMiscQuatMultiplyVU(void);

extern void effObjSetInnerSecondVec(s32, f32 *);

void btlSetUnitRotation(BtlUnit *unit, s128 *quat) {
    f32 result[4];
    if (!(unit->stateFlags & 0x100)) {
        VU0_LOAD_VF(vf10, quat);
        if (unit->flags & 0x10) {
            VU0_LOAD_VF(vf11, D_004179E0);
            effMiscQuatMultiplyVU();
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->orientation);
        VU0_LOAD_VF(vf11, D_004179E0);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF_UNCLOBBERED(vf10, result);
        if (unit->flags & 2) {
            effObjSetInnerSecondVec(unit->effectObject, result);
        }
    }
}

void btlCopyUnitRotationQuaternion(u8 *unit, s128 *dst) {
    PCP_COPY_VECTOR(dst, unit + 0x70);
}

extern void evtSetUnitRgbTransition(BtlUnitExt *, s32, u32);

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    if (unit->flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        unit->baseColor = (unit->baseColor & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition(unit->ext, mode, color);
    }
}

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if (unit->flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        unit->overlayColor = (unit->overlayColor & 0xFF000000) | (color & 0xFFFFFF);
        evtSetUnitRgbTransition(unit->ext, mode, blended);
    }
}

void btlReleaseUnitModelColorResource(BtlUnit *unit, u32 value) {
    mdlReleaseInnerResourceHandle(unit->ext->info, (value & 0xFFFFFF) | 0x80000000);
}

extern void effObjFetchInnerFirstVec(s32);

extern void effObjFetchInnerSecondVecNorm(s32);

extern void mdlStorePrimaryVectorVU(BtlUnitInfo *);

extern void mdlUpdateContextRotationBasisFromQuaternion(BtlUnitInfo *);

extern void sdfModelUpdateCurrentFrameTransforms(s32);

void btlRefreshUnitFxVectors(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return;
    }
    effObjFetchInnerFirstVec(unit->effectObject);
    mdlStorePrimaryVectorVU(unit->ext->info);
    effObjFetchInnerSecondVecNorm(unit->effectObject);
    mdlUpdateContextRotationBasisFromQuaternion(unit->ext->info);
    sdfModelUpdateCurrentFrameTransforms(unit->ext->info->unk18);
}

extern s32 btlAimHorizontalDirectionVU(s128 *, s128 *);

extern void btlUnitGetBodyPosVU(BtlUnit *);

extern void btlSetUnitRotation(BtlUnit *, s128 *);

void btlUnitFaceTarget(BtlUnit *unit, BtlUnit *target) {
    s128 from;
    s128 to;
    s128 hit;
    if (unit->flags & 0x80000) {
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &from);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, &to);
        if (btlAimHorizontalDirectionVU(&from, &to) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &hit);
            btlSetUnitRotation(unit, &hit);
        }
    }
}

extern s32 func_00209258(s128 *, s128 *, f32);

void btlUnitFaceTargetScaled(BtlUnit *unit, BtlUnit *target, f32 scale) {
    s128 from;
    s128 to;
    s128 hit;
    if (unit->flags & 0x80000) {
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &from);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, &to);
        func_00209258(&from, &to, scale);
        VU0_STORE_VF_UNCLOBBERED(vf10, &hit);
        btlSetUnitRotation(unit, &hit);
    }
}

typedef struct {
    u8 unk00[0x110];
    u32 flags;
    u8 unk114[0x10];
    u16 objectId;
} BattleEntryHeader;

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004179E0);

s32 btlClassifySpecialEntryObject(BattleEntryHeader *entry) {
    if (!(entry->flags & 0x400)) {
        return 0;
    }
    switch (entry->objectId) {
    case 0x109: case 0x10A: case 0x110: case 0x111: case 0x112:
    case 0x119: case 0x11D: case 0x11E: case 0x11F: case 0x120:
    case 0x121: case 0x127: case 0x12E: case 0x12F: case 0x131:
    case 0x132: case 0x133: case 0x134: case 0x135: case 0x136:
        return 2;
    default:
        return (btlGetEntryFlagsUnlessDisabled((u8 *)entry + 0x120) >> 14) & 1;
    }
}

typedef struct BtlUnitStats {
    u32 word[0x71];
} BtlUnitStats;

void btlCopyUnitStats(s32 unit, s32 source) {
    BtlUnitStats *stats = (BtlUnitStats *)(unit + 0x120);
    *stats = *(BtlUnitStats *)source;
    btlRefreshUnitMaximumHpAndClampCurrentHp(stats);
    btlRefreshUnitMaximumMpAndClampCurrentMp(stats);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E38F0);

extern s32 sdfModelCreateWithItems(s32, s32);
extern void dds3SetObjectFlags(s32, s32);

void btlCreateUnitTransparency(BtlUnit *unit) {
    u8 *shape;
    if ((unit->flags & 2) == 0) {
        return;
    }
    if (unit->unk344 != 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    shape = *(u8 **)((u8 *)unit->ext->info + 0xC);
    unit->unk344 = sdfModelCreateWithItems(((BtlShapeResource *)shape)->itemKind, ((BtlShapeResource *)shape)->itemIndex);
    dds3SetObjectFlags(unit->effectObject, 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

typedef struct BtlSdfModelState {
    u8 pad00[0x80];
    s32 unk80;
} BtlSdfModelState;

void btlUpdateUnitTransparency(BtlUnit *unit) {
    u32 flags = unit->flags;
    u32 color;
    BtlUnitInfo *info;
    u32 alpha;
    if (flags & 2) {
        if (unit->unkCC == 0) {
            color = unit->overlayColor;
            info = unit->ext->info;
            alpha = color >> 24;
            if (!(flags & 0x20000)) {
                if (unit->unk344 != 0) {
                    sdfReleaseDevSlot(unit->unk344, 1, 1);
                    unit->unk344 = 0;
                    if (unit->flags & 2) {
                        ((BtlSdfModelState *)info->unk18)->unk80 = unit->ext->unk68;
                    } else {
                        ((BtlSdfModelState *)info->unk18)->unk80 = 0;
                    }
                    mdlBroadcastMasked((s32)info, color);
                    mdlProcessContextNodesAndTransforms(info, D_00380788);
                    dds3ClearObjectFlags(unit->effectObject, 1);
                    btlBossDebugPrintf(D_00417940, unit);
                }
            } else if (alpha == 0) {
                dds3SetObjectFlags(unit->effectObject, 1);
            } else if (unit->unk344 == 0) {
                btlCreateUnitTransparency(unit);
            } else {
                func_001E38F0(unit, info, unit->unk344, D_003B6BD0, color);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E3E20);

INCLUDE_ASM(const s32, "game/code_001DD390", btlFormatUnitBedName);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E40F0);

void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit) {
    BtlWork *work;
    if (!(unit->flags & 2)) {
        return;
    }
    work = (BtlWork *)btlGetRuntime();
    if (unit->unkEC != unit->effectIndex) {
        func_001E2758(unit);
        unit->unkF8 = 0;
        unit->unkFA = 0;
        func_001E22D8((u8 *)unit, unit->effectIndex, unit->effectParameter, unit->effectScale);
    }
    if (unit->flags & 0x2000) {
        return;
    }
    if (work->hook6F0 != 0) {
        if (unit->effectIndex != 0xB) {
            work->hook6F0(unit, unit->effectIndex, 1.0f, 0, 0, 1);
        } else {
            work->hook6F0(unit, unit->effectIndex, 1.0f, 0, 0, 2);
        }
    } else {
        if (unit->effectIndex != 0xB) {
            mdlAddEntryFlagged(unit->ext->info, 0, unit->effectIndex);
        } else {
            mdlAddEntryPlain(unit->ext->info, 0, unit->effectIndex);
        }
        sdfMotionSampleAtFrame(unit->ext->info->data, 0.0f);
    }
}

u32 btlApplyIndexedUnitEffectTask(u8 *arguments) {
    s32 index = ((SoundTaskArgs *)arguments)->option;
    if (index >= 0) {
        btlApplyScaledUnitEffectParameter(*(u8 **)arguments, index, ((SoundTaskArgs *)arguments)->unk_08,
                        ((SoundTaskArgs *)arguments)->scale2);
    }
    return 1;
}

SoundTask *btlAllocateIndexedUnitEffectTask(BtlUnit *unit, s32 index, s32 value, f32 scale) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 9;
    task->callback = btlApplyIndexedUnitEffectTask;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = index;
    args->unk_08 = value;
    args->scale2 = scale;
    return task;
}

u32 btlApplyScaledUnitModelTask(u32 *taskArgs) {
    btlApplyUnitModelScaledValue(*taskArgs);
    return 1;
}

SoundTask *btlCreateScaledUnitModelTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlApplyScaledUnitModelTask;
    task->taskId = 0xA;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

u32 btlPollThresholdTask(s32 *arguments) {
    if ((s32)btlGetUnitModelValue1C((BtlUnit *)arguments[0]) >= arguments[1]) {
        if ((((BtlUnit *)arguments[0])->updateFlags & 1) == 0) {
            btlResetUnitModelProgress((BtlUnit *)arguments[0]);
        }
        return 1;
    }
    return 0;
}

SoundTask *btlScheduleThresholdTask(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0xB;
    task->owner = actor->owner;
    task->callback = btlPollThresholdTask;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

/* Each resource row is 0x14 bytes; the reach offset sits 0x0C into it. */
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

/* vu0 routine: move the unit along the line to the target's muzzle, offset by reach */
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
    VU0_LOAD_VF(vf10, unit->orientation);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, unit->bodyOffset);
    VU0_SCALAR_OP(unit->scale, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_NEGATE_XYZ(vf10);
    VU0_LOAD_VF(vf11, pos);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, pos);
    pos[2] -= unit->positionZOffset;
    btlSetUnitPosition(args->unit, pos);
    btlUnitFaceTarget(unit, target);
    if (dist < 1.0f) {
        return 1;
    }
    args->count++;
    return 0;
}

SoundTask *btlAllocateApproachTargetTask(BtlUnit *unit, s32 index, f32 scale) {
    SoundTask *task = btlAllocTask(24);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlApproachTargetTask;
    task->taskId = 0xE;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = index;
    args->scale2 = scale;
    args->unk_08 = 0;
    args->unk_14 = 0;
    return task;
}

typedef struct BtlPosLerpTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
} BtlPosLerpTaskArgs;

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
        btlSetUnitPosition(unit, (f32 *)&pos);
    } else {
        btlSetUnitPosition(unit, (f32 *)&args->to);
        return 1;
    }
    args->count++;
    return 0;
}

SoundTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, f32 *target, f32 scale) {
    SoundTask *task = btlAllocTask(0x30);
    u8 *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0xC;
    task->owner = unit->owner;
    task->callback = btlUpdateUnitPositionInterpolationTask;
    task->onStart = 0;
    args = (u8 *)btlGetTaskArguments((s32)task);
    ((BtlVectorTaskArgs *)args)->scale = scale;
    ((BtlVectorTaskArgs *)args)->unit2C = (u32)unit;
    ((BtlVectorTaskArgs *)args)->state24 = 0;
    ((BtlVectorTaskArgs *)args)->state28 = 0;
    PCP_COPY_VECTOR(args, (u8 *)unit + 0x60);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

typedef struct BtlSlerpTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    s8 mode;
    BtlUnit *unit;
} BtlSlerpTaskArgs;

s32 btlStepUnitRotationNlerp(BtlSlerpTaskArgs *args) {
    s128 quat;
    f32 t;
    f32 rate;
    BtlUnit *unit = args->unit;
    if (args->mode == 0 && !(unit->flags & 0x80000)) {
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
            btlSetUnitRotation(unit, &quat);
            return 0;
        }
    }
    btlSetUnitRotation(unit, &args->to);
    return 1;
}

SoundTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, f32 *target, s8 mode, f32 scale) {
    SoundTask *task = btlAllocTask(0x34);
    u8 *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0xD;
    task->owner = unit->owner;
    task->callback = btlStepUnitRotationNlerp;
    task->onStart = 0;
    args = (u8 *)btlGetTaskArguments((s32)task);
    ((BtlVectorTaskArgs *)args)->scale = scale;
    ((BtlVectorTaskArgs *)args)->mode2C = mode;
    ((BtlVectorTaskArgs *)args)->unit30 = (u32)unit;
    ((BtlVectorTaskArgs *)args)->state24 = 0;
    ((BtlVectorTaskArgs *)args)->state28 = 0;
    PCP_COPY_VECTOR(args, unit->orientation);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

void btlRequestModelOrReuse(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((((BtlUnit *)object)->flags & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        func_001E1BB8(object, effect, model);
        if (*(char *)(arguments + 3) == 0) {
            btlClearUnitDefeatCandidate(object);
            func_0023CA60((u32)((BtlUnit *)object)->ext, 0, 0);
            ((BtlUnit *)object)->overlayColor = ((BtlUnit *)object)->baseColor & 0xFFFFFF;
        }
        btlBossDebugPrintf(D_00417AF0, effect, model);
    } else {
        btlRequestModelAssetByMode(object, effect, model);
        ((BtlUnit *)object)->gunResourceFlags |= 1;
        btlBossDebugPrintf(D_00417B10, effect, model);
    }
}

u32 btlPollModelLoadCompletion(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((((BtlUnit *)object)->flags & 2) == 0) {
        if (!btlCheckModelAssetByMode(object, effect, model)) {
            return 0;
        }
        func_001E1BB8(object, effect, model);
        btlReleaseModelAssetByMode(object, effect, model);
        btlBossDebugPrintf(D_00417B30, effect, model, object);
    }
    if (*(s8 *)(arguments + 3) == 0) {
        btlClearUnitDefeatCandidate(object);
        func_0023CA60((u32)((BtlUnit *)object)->ext, 0, 0);
        ((BtlUnit *)object)->overlayColor = ((BtlUnit *)object)->baseColor & 0xFFFFFF;
    }
    ((BtlUnit *)object)->gunResourceFlags = (((BtlUnit *)object)->gunResourceFlags & ~1) | 2;
    return 1;
}

SoundTask *btlCreateModelLoadPollTask(BtlUnit *unit, u32 index, u32 value, s8 mode) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x18;
    task->flags &= ~1;
    task->owner = unit->owner;
    task->onStart = (void (*)(u32))btlRequestModelOrReuse;
    task->callback = btlPollModelLoadCompletion;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = index;
    args->unk_08 = value;
    args->mode = mode;
    return task;
}

u32 btlReleaseUnitModelTask(u32 *taskArgs) {
    btlClearUnitDefeatCandidate(*taskArgs);
    btlReleaseActorModelResources(*taskArgs);
    return 1;
}

SoundTask *btlScheduleRefreshTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlReleaseUnitModelTask;
    task->taskId = 0x19;
    task->owner = unit->owner;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417AF0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417B10);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417B30);

s32 btlBeginModelChange(u32 *arguments) {
    s32 owner = arguments[0];
    u32 model = arguments[1];
    u32 variant = arguments[2];
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        btlRequestModelAssetByMode(owner, model, variant);
        ((BtlUnit *)owner)->gunResourceFlags = (((BtlUnit *)owner)->gunResourceFlags | 1) & ~2;
        return btlBossDebugPrintf("btl:model change start[%X,%X]\n", model, variant);
    }
    return status;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E50E0);

extern u32 func_001E50E0(u32 *);

SoundTask *btlCreateModelChangeTask(BtlUnit *unit, s32 option, s32 value08, s32 value0C, s32 value10, u8 flag19) {
    SoundTask *task = btlAllocTask(0x1C);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x1A;
    task->flags &= ~1;
    task->owner = unit->owner;
    task->onStart = (void (*)(u32))btlBeginModelChange;
    task->callback = func_001E50E0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = option;
    args->unk_08 = value08;
    args->unk_0C = value0C;
    args->unk_10 = value10;
    args->flag19 = flag19;
    args->flag18 = 0;
    args->unk_14 = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(s32 taskArgs) {
    if ((((BtlUnit *)((SoundTaskArgs *)taskArgs)->unk_0C)->flags & 2) != 0) {
        evtSetUnitStatusFlags((u32)((BtlUnit *)((SoundTaskArgs *)taskArgs)->unk_0C)->ext);
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(u32 *taskArgs) {
    if ((((BtlUnit *)taskArgs[3])->flags64 & 0x1000000002) == 0x1000000002) {
        func_0023C870((u32)((BtlUnit *)taskArgs[3])->ext, taskArgs[2], *taskArgs, taskArgs[1]);
    }
    return 1;
}

SoundTask *btlCreateUnitTask0F(BtlUnit *unit, s32 value, s32 option, s32 value08) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0xF;
    task->owner = unit->owner;
    task->onStart = btlApplyLinkedUnitStatusWhenActorActive;
    task->callback = btlApplyUnitFxWhenLoaded;
    args = btlGetTaskArguments((s32)task);
    args->unk_0C = (u32)unit;
    args->value = value;
    args->option = option;
    args->unk_08 = value08;
    return task;
}

void btlPrepareUnitStatusFxOnStart(s32 taskArgs) {
    if ((((FxTask *)taskArgs)->unit->flags & 2) != 0) {
        evtSetUnitStatusFlags((u32)((FxTask *)taskArgs)->unit->ext);
        return;
    }
}

s32 btlApplyUnitVectorFxWhenLoaded(FxTask *task) {
    BtlUnit *unit = task->unit;
    if (unit->flags & 2) {
        VU0_LOAD_VF_MEMORY(vf10, task);
        evtSetUnitNormalizedDirection(unit->ext, task->unk10);
    }
    return 1;
}

SoundTask *btlCreateUnitTask10(BtlUnit *unit, f32 *vec, s32 option) {
    SoundTask *task = btlAllocTask(0x18);
    FxTask *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x10;
    task->owner = unit->owner;
    task->onStart = (void (*)(u32))btlPrepareUnitStatusFxOnStart;
    task->callback = btlApplyUnitVectorFxWhenLoaded;
    args = (FxTask *)btlGetTaskArguments((s32)task);
    args->unit = unit;
    args->unk10 = option;
    PCP_COPY_VECTOR(args, vec);
    return task;
}

typedef struct BtlFadeArgs {
    BtlUnit *unit;
    s32 fadeIn;
    s32 fadeOut;
    u32 count;
    u32 color;
} BtlFadeArgs;

s32 btlUnitFadeInTask(BtlFadeArgs *args) {
    u32 total = args->fadeIn + args->fadeOut;
    BtlUnit *unit = args->unit;
    if (total != 0) {
        if (args->count == 0) {
            args->color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
            btlFlagUnitDefeatCandidate(unit);
            mdlBroadcastMasked((s32)unit->ext->info, 0);
            evtSetUnitRgbTransition(unit->ext, 0, 0);
            func_0023CA60((u32)unit->ext, 0, 0);
            func_0023CA60((u32)unit->ext, args->fadeIn, args->color);
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
            func_0023CA60((u32)unit->ext, 0, args->color);
            return 1;
        }
    }
    args->count++;
    return 0;
}

SoundTask *btlCreateUnitFadeInTask(BtlUnit *unit, u32 value, u32 variant) {
    SoundTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlUnitFadeInTask;
    task->taskId = 0x11;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->unk_10 = 0x80808080;
    args->option = value;
    args->unk_08 = variant;
    args->unk_0C = 0;
    return task;
}

s32 btlUnitFadeOutTask(BtlFadeArgs *args) {
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
            func_0023CA60((u32)unit->ext, args->fadeIn, 0);
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

SoundTask *btlCreateUnitFadeOutTask(BtlUnit *unit, u32 value, u32 variant) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlUnitFadeOutTask;
    task->taskId = 0x12;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = value;
    args->unk_08 = variant;
    args->unk_0C = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E5E40);

extern u32 func_001E5E40(s32);

SoundTask *func_001E5FF8(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x13;
    task->owner = actor->owner;
    task->callback = func_001E5E40;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E6080);

extern u32 func_001E6080(s32);

SoundTask *func_001E61A0(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x14;
    task->owner = actor->owner;
    task->callback = func_001E6080;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E6228);

extern u32 func_001E6228(s32);

SoundTask *func_001E6428(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x15;
    task->owner = actor->owner;
    task->callback = func_001E6228;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

typedef struct BtlBattleData {
    u8 pad0[0x14];
    s32 unk14;
} BtlBattleData;

typedef struct BtlEffectHandle {
    u8 pad0[0xC];
    u16 flags;
} BtlEffectHandle;

typedef struct UnitEffectTaskArgs {
    BtlUnit *unit;
    s32 effectId;
    BtlEffectHandle *effect;
    s32 duration;
    s32 counter;
} UnitEffectTaskArgs;

extern s32 sndMixerClone(s32);
extern BtlEffectHandle *func_00168548(s32, s32, BtlUnit *, s32);
extern void effBattleUpdateSelectedValue(BtlEffectHandle *, s32);
extern void func_00168978(BtlEffectHandle *);

s32 btlUpdateSelectedUnitEffect(UnitEffectTaskArgs *args) {
    BtlUnit *unit = args->unit;
    BtlWork *work;
    if (!(unit->flags & 2)) {
        return 1;
    }
    work = (BtlWork *)btlGetRuntime();
    if (args->effect == 0) {
        s32 handle = work->battleData->unk14;
        unit->flags |= 0x80;
        args->effectId = sndMixerClone(handle);
        args->effect = func_00168548(args->effectId, 2, unit, 0);
        args->duration = 0xE;
        args->effect->flags &= 0xFFF9;
        effBattleUpdateSelectedValue(args->effect, 0xE);
        unit->flags &= ~8;
        if (unit->flags & 2) {
            unit->ext->info->flags |= 1;
        }
    }
    args->counter = args->counter + 1;
    if (args->counter >= args->duration) {
        unit->flags = (unit->flags & ~0x80) | 0x40;
        return 1;
    }
    func_00168978(args->effect);
    return 0;
}

void btlFinishSelectedUnitEffect(u32 *arguments) {
    u32 value = arguments[2];
    if (value != 0) {
        effReleaseBattleVoiceOwner(value);
    }
    if (arguments[1] != 0) {
        sndReleaseAllVoices(arguments[1]);
    }
    btlClearUnitDefeatCandidate(arguments[0]);
    ((BtlUnit *)arguments[0])->flags |= 0x40;
}

SoundTask *btlCreateSelectedEffectUpdateTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x16;
    task->flags |= 2;
    task->owner = unit->owner;
    task->callback = btlUpdateSelectedUnitEffect;
    task->onFinish = btlFinishSelectedUnitEffect;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->unk_08 = 0;
    args->unk_10 = 0;
    args->unk_0C = 0;
    return task;
}

u32 btlUpdateCommandSoundTask(void) {
    btlUpdateUnitActors();
    return 1;
}

SoundTask *btlCreateCommandSoundUpdateTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlUpdateCommandSoundTask;
    task->taskId = 0x1B;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 btlUpdateCommandSoundTaskSecondary(void) {
    func_00209078();
    return 1;
}

SoundTask *btlCreateSecondaryCommandSoundTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->taskId = 0x1C;
    task->flags |= 2;
    task->status = 0;
    task->onStart = 0;
    task->callback = btlUpdateCommandSoundTaskSecondary;
    return task;
}

u32 func_001E6790(void) {
    return 1;
}

SoundTask *func_001E6798(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_001E6790;
    task->taskId = 0x20;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", btlUnitBaseLightTask);

extern u32 btlUnitBaseLightTask(u32 *);

SoundTask *btlCreateUnitBaseLightTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlUnitBaseLightTask;
    task->taskId = 0x21;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = 0;
    return task;
}

typedef struct BtlStiffenTaskArgs {
    BtlUnit *unit;
    f32 scale;
    s32 count;
} BtlStiffenTaskArgs;

u32 btlStiffenDamageShakeStep(BtlStiffenTaskArgs *args) {
    f32 pos[4];
    f32 scale;
    s32 node;

    if (!(args->unit->flags & 2)) {
        return 1;
    }
    if (args->unit->stateFlags & 0x200000) {
        return 1;
    }
    if (args->count == 0) {
        node = mdlGetNodeField2C((s32)args->unit->ext->info, 0);
        if (node < 0x1D) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(args->unit->resourceKind, args->unit->resourceIndex);
            if (((BtlEffectResource *)resource)->nodes[node].rateKind == 2) {
                btlRefreshUnitEffectMotionAndEntry(args->unit);
                btlBossDebugPrintf("btl:stiffen damage motion wait\n");
            }
        }
    }
    if (0.5f < args->scale) {
        scale = args->scale * (effMiscRandUnitFloat(effSharedRandomState) * 0.5f + 0.5f);
        if (args->count & 1) {
            scale = -scale;
        }
        if (args->unit->flags64 & 0x808000000000) {
            effObjFetchInnerFirstVec(args->unit->effectObject);
            VU0_STORE_VF(vf10, pos);
            pos[0] += scale;
        } else {
            func_001E3108((u8 *)args->unit, (s128 *)pos);
            pos[0] += scale;
            pos[2] += args->unit->positionZOffset;
        }
        effObjSetInnerFirstVec(args->unit->effectObject, pos);
        args->scale *= 0.85f;
    } else {
        if (args->unit->flags64 & 0x808000000000) {
            effObjFetchInnerFirstVec(args->unit->effectObject);
            VU0_STORE_VF(vf10, pos);
        } else {
            func_001E3108((u8 *)args->unit, (s128 *)pos);
            pos[2] += args->unit->positionZOffset;
        }
        effObjSetInnerFirstVec(args->unit->effectObject, pos);
        return 1;
    }
    args->count += 1;
    return 0;
}

SoundTask *btlCreateStiffenDamageShakeTask(BtlUnit *unit, f32 value) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x1D;
    task->callback = btlStiffenDamageShakeStep;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->scale = value;
    args->unk_08 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E6BF8);

extern u32 func_001E6BF8(u32 *);

SoundTask *func_001E6E18(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E6BF8;
    task->taskId = 0x1E;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = 0;
    return task;
}

u32 func_001E6E90(s32 *taskArgs) {
    s32 unit;

    unit = *taskArgs;
    ((BtlUnit *)unit)->flags = ((BtlUnit *)unit)->flags & 0xffffffef;
    btlSetUnitRotation(unit, unit + 0x40);
    return 1;
}

SoundTask *btlScheduleActorUpdate(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E6E90;
    task->taskId = 0x1F;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

u32 btlRefreshUnitFxVectorTask(u32 *taskArgs) {
    btlRefreshUnitFxVectors(*taskArgs);
    return 1;
}

SoundTask *btlCreateUnitFxVectorRefreshTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlRefreshUnitFxVectorTask;
    task->taskId = 0x22;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

extern s32 btlFormatUnitBedName(BtlUnit *, char *);
extern s32 func_002C80E8(char *);

void btlStartGunFinishLoad(s32 *task) {
    char filename[0x70];
    BtlUnit *unit = *(BtlUnit **)task;
    if (unit->flags & 0x400) {
        return;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
    }
    if (btlFormatUnitBedName(unit, filename)) {
        s32 handle = func_002C80E8(filename);
        task[1] = handle;
        btlBossDebugPrintf("btl:gun & finish load start[%s][%p]\n", filename, handle);
    }
    unit->gunResourceFlags = (unit->gunResourceFlags | 4) & ~8;
}

extern s32 fileIsRequestReadyInCurrentMode(s32);

extern s32 fileGetResourceHandle(s32);

extern s32 filePollEntryCleanup(s32);

typedef struct GunLoadArgs {
    BtlUnit *unit;
    s32 handle;
} GunLoadArgs;

u32 btlPollGunLoad(s32 arg) {
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

SoundTask *btlCreateGunLoadPollTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x23;
    task->flags &= ~1;
    task->owner = unit->owner;
    task->onStart = btlStartGunFinishLoad;
    task->callback = btlPollGunLoad;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = 0;
    return task;
}

u32 btlUpdateUnitEffectsTask(void) {
    btlUpdateUnitEffects();
    return 1;
}

SoundTask *btlCreateUpdateUnitEffectsTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlUpdateUnitEffectsTask;
    task->taskId = 0x24;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 btlCreateActorTransparency(u32 *task) {
    BtlUnit *unit = *(BtlUnit **)task;
    if (!(unit->flags & 2)) {
        return 0;
    }
    btlCreateUnitTransparency(unit);
    (*(BtlUnit **)task)->flags |= 0x20000;
    return 1;
}

extern u32 btlCreateActorTransparency(u32 *);

SoundTask *btlCreateActorTransparencyTask(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x25;
    task->callback = btlCreateActorTransparency;
    task->status = 0;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E72B0);

extern u32 func_001E72B0(u32 *);

SoundTask *btlCreateActorModelBlendTask(BtlUnit *unit, u32 target, u32 index, u32 value, f32 scale) {
    SoundTask *task = btlAllocTask(0x1C);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E72B0;
    task->taskId = 0x26;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = target;
    args->unk_08 = index;
    args->unk_10 = value;
    args->scale14 = scale;
    args->unk_0C = -1;
    args->unk_18 = 0;
    return task;
}

u32 btlRotateUnitTowardOtherBody(s32 taskArgs) {
    s128 hit[1];
    s128 from;
    s128 to;
    btlUnitGetBodyPosVU(*(BtlUnit **)taskArgs);
    VU0_STORE_VF_UNCLOBBERED(vf10, &from);
    btlUnitGetBodyPosVU(*(BtlUnit **)(taskArgs + 4));
    VU0_STORE_VF_UNCLOBBERED(vf10, &to);
    if (btlAimHorizontalDirectionVU(&from, &to) != 0) {
        VU0_STORE_VF_UNCLOBBERED(vf10, hit);
        btlSetUnitRotation(*(BtlUnit **)taskArgs, hit);
    }
    return 1;
}

SoundTask *btlCreateUnitFaceBodyTask(BtlUnit *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x27;
    task->owner = actor->owner;
    task->callback = btlRotateUnitTowardOtherBody;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 btlFlagDefeatCandidateTask(u32 *taskArgs) {
    btlFlagUnitDefeatCandidate(*taskArgs);
    return 1;
}

SoundTask *btlCreateDefeatCandidateTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlFlagDefeatCandidateTask;
    task->taskId = 0x28;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

u32 btlClearDefeatCandidateTask(u32 *taskArgs) {
    btlClearUnitDefeatCandidate(*taskArgs);
    return 1;
}

SoundTask *btlCreateDefeatCandidateClearTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlClearDefeatCandidateTask;
    task->taskId = 0x29;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E7648);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E7960);

void btlResetUnitLinks(BtlUnit *unit) {
    unit->unk310 = -1;
    unit->unk314 = -1;
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->gunResourceFlags = 0;
    unit->unk330 = 0;
    btlClearAllActorEntrySlots(unit);
    unit->link31C = sndAllocResourceLink(unit);
    unit->link320 = sndAllocLink(unit);
}

extern s64 btlAdvanceRuntimeSequenceCounter(void);

extern void *memset(void *, s32, u32);

BtlUnit *btlCreateUnit(void) {
    u32 handle = sdfAllocGeneralBlock(0x368);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(handle);
    BtlWork *work;
    memset(unit, 0, 0x368);
    unit->handle35C = handle;
    unit->owner = btlAdvanceRuntimeSequenceCounter();
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->lookupId = unit->unk310 = -1;
    unit->unk2E4 = 6;
    unit->gunResourceFlags = 0;
    unit->unk334 = 0;
    unit->node318 = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults((BtlFx *)unit);
    btlInitFxLights((BtlFxLights *)unit);
    btlResetUnitLinks(unit);
    work = (BtlWork *)btlGetRuntime();
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

extern void sndFreeResourceNode(struct SoundResourceNode *);

extern void sndFreeResourceLink(struct SoundResourceLink *);

extern void sndFreeLink(struct SoundLink *);

extern void sdfFreeMemoryFromEitherHeap(void *);

extern void sdfQueueNonzeroResourceId(s32);

extern void btlReleaseActorModelResources(BtlUnit *);

extern void sndFreeListNode(struct ActiveSoundNode *);

void btlReleaseUnitResources(BtlUnit *unit) {
    btlBossDebugPrintf("btl:unit data free[%p]\n", unit);
    if (unit->node318 != 0) {
        sndFreeResourceNode(unit->node318);
        unit->node318 = 0;
    }
    if (unit->link31C != 0) {
        sndFreeResourceLink(unit->link31C);
        unit->link31C = 0;
    }
    if (unit->link320 != 0) {
        sndFreeLink(unit->link320);
        unit->link320 = 0;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
        unit->gunResourceFlags &= ~4;
        unit->gunResourceFlags &= ~8;
    }
    if (unit->node324 != 0) {
        sndFreeListNode(unit->node324);
        unit->node324 = 0;
    }
    btlReleaseActorModelResources(unit);
    if (unit->unk350 != 0) {
        sdfQueueNonzeroResourceId(unit->unk350);
        unit->unk350 = 0;
        unit->unk34C = 0;
    }
}

extern void sdfReleaseResourceAllocation(u32);

void btlDestroyUnit(BtlUnit *unit) {
    btlBossDebugPrintf("btl:unit delete[%p]\n", unit);
    btlReleaseUnitResources(unit);
    if (unit->nextActor != 0) {
        unit->nextActor->previousActor = unit->previousActor;
    }
    if (unit->previousActor != 0) {
        unit->previousActor->nextActor = unit->nextActor;
    } else {
        ((BtlWork *)btlGetRuntime())->actorList = unit->nextActor;
    }
    sdfReleaseResourceAllocation(unit->handle35C);
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = next) {
        next = unit->nextActor;
        btlDestroyUnit(unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    s32 actor = (s32)((BtlWork *)btlGetRuntime())->actorList;
    s32 next;
    while (actor != 0) {
        next = (s32)((BtlUnit *)actor)->nextActor;
        if (((BtlUnit *)actor)->flags & mask) {
            btlDestroyUnit(actor);
        }
        actor = next;
    }
}

BtlUnit *btlFindActorForOwner(u64 owner) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->owner == owner) {
            return unit;
        }
    }
    return 0;
}

s32 btlIsActiveActor(BtlUnit *actor) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit == actor) {
            return 1;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeClear(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (!(unit->statBits & 0x20) && unit->mode == mode) {
            return unit;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeFlagged(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if ((unit->statBits & 0x20) && unit->mode == mode) {
            return unit;
        }
    }
    return 0;
}

void *btlAllocateIndexList(s32 capacity) {
    u8 *list = sdfAllocAndClearQuadwords(capacity * 4 + 12);
    ((BtlIndexList *)list)->capacity = capacity;
    ((BtlIndexList *)list)->entries = (u32 *)(list + 12);
    ((BtlIndexList *)list)->count = 0;
    return list;
}

void btlFreeIndexList(s32 list) {
    sdfReleaseChipBlock(list);
}

void btlAppendIndexListEntry(s32 list, u32 entry) {
    s32 index;

    index = ((BtlIndexList *)list)->count;
    ((BtlIndexList *)list)->count = index + 1;
    ((BtlIndexList *)list)->entries[index] = entry;
}

void btlClearIndexList(s32 list) {
    ((BtlIndexList *)list)->count = 0;
}

u32 btlGetIndexListCount(s32 list) {
    return ((BtlIndexList *)list)->count;
}

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

u32 btlFindListIndex(s32 list, s32 value) {
    u32 count = btlGetIndexListCount(list);
    u32 i;
    for (i = 0; i < count; i++) {
        if (value == btlGetIndexListEntry(list, i)) {
            return i;
        }
    }
    return -1;
}

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

void btlApplyUnitEffectScale(BtlUnit *unit) {
    BtlEffObjInner *inner;
    if (unit->flags & 2) {
        btlInitializeEffectVectorsFromSourceRecords((BtlFx *)unit, unit->resourceKind, unit->resourceIndex);
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->unk50, "vmulx.xyzw vf10, vf10, vf2x");
        inner = ((BtlEffObj *)unit->effectObject)->inner;
        inner->flagsC0 |= 1;
        inner->flagsC0 &= ~2;
        VU0_STORE_VF(vf10, inner->vec60);
        mdlStoreTertiaryVectorVU((s32)unit->ext->info);
        mdlSetAmountOnAllContextResources(unit->unk50, (s32)unit->ext->info);
        btlSetUnitPosition(unit, (f32 *)((u8 *)unit + 0x60));
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E8258);

void btlNormalizeActionCameraKeyScales(ActionUnit *action) {
    f32 *key = (f32 *)action;
    u32 flags = action->flags;
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

u32 func_001E8568(void) {
    return 1;
}

u32 func_001E8570(void) {
    return 1;
}

u32 func_001E8578(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E8580);

extern void func_001E8580(XformData *, XformData *, XformData *, f32);

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 func_00209770(f32);

s32 btlStepPoseBlendHalf(u8 *fx) {
    f32 t;
    if (((BattlePoseBlendState *)fx)->blendMode == 0) {
        ((BattlePoseBlendState *)fx)->progressBits = 0;
        btlScalarRangeInitQuadratic(fx + 0x160, (f32)(((BattlePoseBlendState *)fx)->durationFrames * 2));
        btlCopyMotionTransform((XformData *)fx, (XformData *)(fx + 0x30));
        return 0;
    }
    t = func_00209770(1.0f);
    if (t > 0.5f) {
        t = 0.5f;
    }
    func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), t + t);
    ((BattlePoseBlendState *)fx)->progress = t;
    if (0.5f <= t) {
        return 1;
    }
    return 0;
}

extern void btlScalarRangeSetStartClearEnd(u8 *, f32);

extern f32 func_002096C8(u8 *);

extern void func_001E8580(XformData *, XformData *, XformData *, f32);

s32 btlStepPoseBlend(u8 *fx) {
    u8 *timer = fx + 0x158;
    u8 *pose = fx + 0x30;
    f32 t;
    if (((BattlePoseBlendState *)fx)->blendMode == 0) {
        btlScalarRangeSetStartClearEnd(timer, ((BattlePoseBlendState *)fx)->duration);
        btlCopyMotionTransform((XformData *)fx, (XformData *)pose);
    }
    t = func_002096C8(timer);
    func_001E8580((XformData *)fx, (XformData *)pose, (XformData *)(fx + 0xC0), t);
    ((BattlePoseBlendState *)fx)->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

extern void func_001E8580(XformData *, XformData *, XformData *, f32);

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 func_00209770(f32);

s32 btlStepPoseBlendFrame(u8 *fx) {
    f32 t;
    if (((BattlePoseBlendState *)fx)->blendMode == 0) {
        ((BattlePoseBlendState *)fx)->progressBits = 0;
        btlScalarRangeInitQuadratic(fx + 0x160, (f32)((BattlePoseBlendState *)fx)->durationFrames);
        btlCopyMotionTransform((XformData *)fx, (XformData *)(fx + 0x30));
        return 0;
    }
    t = func_00209770(1.0f);
    func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), t);
    ((BattlePoseBlendState *)fx)->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

s32 btlStepPoseBlendRatio(u8 *fx) {
    f32 ratio = (f32)((BattlePoseBlendState *)fx)->blendMode / (f32)((BattlePoseBlendState *)fx)->durationFrames;
    if (ratio <= 1.0f) {
        func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), ratio);
        return 0;
    }
    btlCopyMotionTransform((XformData *)fx, (XformData *)(fx + 0xC0));
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E88A8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E89E0);

u32 btlExecuteCommandSoundTask(u32 *taskArgs) {
    func_001E8258(taskArgs[3], *taskArgs, taskArgs[1], taskArgs[2], taskArgs[4]);
    return 1;
}

SoundTask *btlCreateCommandSoundTask(s32 actor, s32 mode) {
    SoundTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;

    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x2A;
    if (actor != 0 && ((BtlUnit *)actor)->link18 != 0) {
        task->owner = ((BtlUnit *)actor)->link18->owner;
    }
    task->callback = btlExecuteCommandSoundTask;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = (void *)actor;
    args->unk_0C = mode;
    args->option = 0;
    args->unk_08 = 0;
    args->unk_10 = 0;
    return task;
}

SoundTask *btlCreateTargetedCommandSoundTask(s32 actor, s32 mode, u32 command) {
    SoundTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = btlGetTaskArguments((s32)task);

    args->unk_10 = command;
    return task;
}

SoundTask *btlCreateCommandSoundWithArguments(s32 actor, s32 option, s32 flag, s32 mode, s32 command) {
    SoundTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = btlGetTaskArguments((s32)task);

    args->unk_10 = command;
    args->option = option;
    args->unk_08 = flag;
    return task;
}

u32 btlInitializeMotionTransformFromTaskArguments(u8 *arguments) {
    u8 *context = (u8 *)btlGetRuntime();
    func_001E8258(1, *(u32 *)arguments, 0, 0, 0);
    btlInitMotionTransformFromComponents(context + 0x70, ((BtlCameraTaskArgs *)arguments)->component[0], ((BtlCameraTaskArgs *)arguments)->component[1],
                    ((BtlCameraTaskArgs *)arguments)->component[2], ((BtlCameraTaskArgs *)arguments)->component[3],
                    ((BtlCameraTaskArgs *)arguments)->component[4], ((BtlCameraTaskArgs *)arguments)->component[5],
                    ((BtlCameraTaskArgs *)arguments)->component[6], ((BtlCameraTaskArgs *)arguments)->component[7]);
    return 1;
}

SoundTask *btlCreateFloatTask28(BtlUnit *actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
    SoundTask *task = btlAllocTask(0x24);
    f32 *args;
    task->enabled = 1;
    task->taskId = 0x2B;
    task->status = 0;
    if (actor != 0 && actor->link18 != 0) {
        task->owner = actor->link18->owner;
    }
    task->callback = btlInitializeMotionTransformFromTaskArguments;
    task->onStart = 0;
    args = (f32 *)btlGetTaskArguments((s32)task);
    *(BtlUnit **)args = actor;
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

s32 btlApplyEffectCameraKeyframes(f32 *args) {
    s32 context = btlGetRuntime();
    func_001E8258(1, *(s32 *)args, 0, 0, 0);
    btlSetEffectCameraKeys(context + 0x70, args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                  args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]);
    return 1;
}

SoundTask *btlCreateFloatTask29(BtlUnit *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    SoundTask *task = btlAllocTask(0x44);
    f32 *args;
    task->enabled = 1;
    task->taskId = 0x2C;
    task->status = 0;
    if (actor != 0 && actor->link18 != 0) {
        task->owner = actor->link18->owner;
    }
    task->callback = btlApplyEffectCameraKeyframes;
    task->onStart = 0;
    args = (f32 *)btlGetTaskArguments((s32)task);
    *(BtlUnit **)args = actor;
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

u32 btlApplyCameraKeysAndMarkRuntimeChange(u32 taskArgs) {
    s32 work;

    work = btlGetRuntime();
    btlApplyEffectCameraKeyframes(taskArgs);
    ((BtlWork *)work)->runtimeFlags |= 0x80000;
    return 1;
}

SoundTask *btlCreateNotifyingCameraKeyframeTask(BtlUnit *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    SoundTask *task = btlCreateFloatTask29(actor, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
    task->callback = btlApplyCameraKeysAndMarkRuntimeChange;
    return task;
}

u32 btlRunCameraMotionResetTask(void) {
    s32 work;

    work = btlGetRuntime();
    btlResetCameraMotion(work + 0x70);
    return 1;
}

SoundTask *btlScheduleContextReset(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlRunCameraMotionResetTask;
    task->taskId = 0x2D;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E9130);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E9410);

void btlClearPendingSoundList(void) {
    s32 context = btlGetRuntime();
    s32 list = ((BtlWork *)context)->pendingSoundList;

    if (list != 0) {
        btlFreeIndexList(list);
        ((BtlWork *)context)->pendingSoundList = 0;
    }
    ((BtlWork *)context)->battleFlags &= ~0x10;
}

void btlCopyMotionTransform(XformData *dst, XformData *src) {
    PCP_COPY_VECTOR(&dst->vec0, &src->vec0);
    PCP_COPY_VECTOR(&dst->vec1, &src->vec1);
    dst->f20 = src->f20;
    dst->f24 = src->f24;
}

void func_001E95C8(s32 transform, f32 value) {
    ((XformData *)transform)->f24 = value;
}

void btlInitMotionTransformFromVectors(u8 *object, f32 *origin, f32 *direction) {
    VU0_LOAD_VF(vf10, direction);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9130);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, object + 0x10);
    VU0_SET_VF2X(1.0f);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, origin);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, object);
    ((XformData *)object)->f20 = 1.0f;
    ((XformData *)object)->f24 = 0.6981317f;
    btlClearRuntimeFlag2000();
}

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
    ((XformData *)object)->f24 = scale * 0.017453293f;
}

void btlSetEffectCameraKeys(s32 fx, f32 x0, f32 y0, f32 z0, f32 vx0, f32 vy0, f32 vz0, f32 vw0, f32 x1, f32 y1, f32 z1,
                   f32 vx1, f32 vy1, f32 vz1, f32 vw1, f32 scale, f32 f154) {
    btlInitMotionTransformFromComponents((u8 *)fx + 0x30, x0, y0, z0, vx0, vy0, vz0, vw0, scale);
    btlInitMotionTransformFromComponents((u8 *)fx + 0xC0, x1, y1, z1, vx1, vy1, vz1, vw1, scale);
    ((ActionUnit *)fx)->unk154 = f154;
    ((ActionUnit *)fx)->flags |= 0x41;
}

u32 btlGetActiveUnitId(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    return ((BtlWork *)workAddress)->activeUnitId;
}

f32 btlGetPoseBlendProgress(u8 *unit) {
    return ((BattlePoseBlendState *)unit)->progress;
}

s32 btlIsUnitInActiveList(s32 unit) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *slot = (u8 *)((BtlWork *)work)->activeSlot;
    u32 count;
    u32 i;
    if (slot != 0 && ((BtlActiveSlot *)slot)->unit == unit) {
        return 1;
    }
    count = btlGetIndexListCount(((BtlWork *)work)->pendingSoundList);
    for (i = 0; i < count; i++) {
        if (btlGetIndexListEntry(((BtlWork *)work)->pendingSoundList, i) == unit) {
            return 1;
        }
    }
    return 0;
}

void btlResetActiveUnitList(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlWork *)workAddress)->activeSlot = 0;
    ((BtlWork *)workAddress)->runtimeFlags = ((BtlWork *)workAddress)->runtimeFlags | 0x400;
    btlClearIndexList(((BtlWork *)workAddress)->pendingSoundList);
}

void btlClearRuntimeFlag2000(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlWork *)workAddress)->runtimeFlags = ((BtlWork *)workAddress)->runtimeFlags & 0xffffdfff;
}

void btlSetRuntimeFlag2000(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlWork *)workAddress)->runtimeFlags = ((BtlWork *)workAddress)->runtimeFlags | 0x2000;
}

u32 btlIsRuntimeFlag2000Clear(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    return ((((s32)((BtlWork *)workAddress)->runtimeFlags >> 0xd)) ^ 1U) & 1;
}

typedef struct WorldMotionData {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x14];
    s32 unk20;
} WorldMotionData;

extern void *dds3GetWorldObject(void);

extern WorldMotionData *dds3GetWorldCameraObject(void *);

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

extern void dds3SetWorldCameraObject(void *, s32);
extern f32 dds3GetCameraFieldOfView(s32);
extern void func_001063A8(f32);

void btlRefreshWorldCameraHandle(void) {
    WorldObj *object;
    s32 handle;
    if (((BtlWork *)btlGetRuntime())->battleFlags & 2) {
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
                func_001063A8(dds3GetCameraFieldOfView(handle));
            }
        }
    }
}

extern s32 D_00436A9C;

extern s32 D_00436AA0;

s32 btlGetWorldObjectDefault(void) {
    WorldMotionData *data;
    if (!(((BtlWork *)btlGetRuntime())->battleFlags & 2)) {
        return D_00436AA0;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return D_00436AA0;
    }
    if (data->unk8 == 0) {
        return D_00436A9C;
    }
    return data->unk8;
}

s32 btlIsWorldMotionIdle(void) {
    WorldMotionData *data;
    if (!(((BtlWork *)btlGetRuntime())->battleFlags & 2)) {
        return 0;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return 0;
    }
    return data->unk8 == 0;
}

s32 btlGetCameraVectorWork(void) {
    s32 work;

    work = btlGetRuntime();
    return work + 0x70;
}

void btlFlagAllUnitDefeatCandidatesTask(void) {
    btlFlagAllUnitsDefeatCandidate();
}

void btlClearAllUnitDefeatCandidatesTask(void) {
    btlClearAllUnitDefeatCandidates();
}

void btlFlagLinkedGroupDefeatCandidatesTask(s32 action) {
    btlFlagMatchingUnitsDefeatCandidate(((ActionUnit *)action)->link->unit->flags & 0x600);
}

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlApplyCombinedActorFlags(u8 *fx) {
    u32 i = 0;
    s32 bits = 0;
    u32 count = btlGetIndexListCount(((ActionUnit *)fx)->actorIndices);
    if (count != 0) {
        do {
            bits |= ((BtlUnit *)btlGetIndexListEntry(((ActionUnit *)fx)->actorIndices, i++))->flags & 0x600;
        } while (i < count);
    }
    if (bits != 0) {
        btlFlagMatchingUnitsDefeatCandidate(bits);
    }
}

extern f32 D_003B6D90[];

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417C78);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417C88);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417C98);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417CA8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417CB8);

void btlResetCameraMotion(s32 action) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit;
    f32 current;
    f32 limit;
    if (work->activeUnitId == 1 || btlHasSingleLinkedResource(action) != 0) {
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            if (unit->flags & 1) {
                if (unit->flags & 0x200) {
                    if (unit->flags & 2) {
                        if (unit->ext != 0) {
                            s32 node = mdlGetNodeField2C(unit->ext->info, 0);
                            if (node == 0xD || node == 0x12) {
                                current = btlGetUnitModelValue1C(unit);
                                limit = (f32)btlGetUnitModelFrameCount(unit);
                                if (unit->mode < 0xA) {
                                    limit = limit * D_003B6D90[unit->mode];
                                } else {
                                    limit = limit * 0.7f;
                                }
                                if (current < limit) {
                                    sdfMotionSampleAtFrame(unit->ext->info->data, limit);
                                    btlBossDebugPrintf("btl:camera mot reset[%p]\n", unit);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

s32 btlPositionActorIndexUnits(ActionUnit *action) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit;
    u32 count;
    f32 pos[4];
    if (work->unk268 != 3) {
        return 0;
    }
    count = btlGetIndexListCount(action->actorIndices);
    if (count != 1) {
        return 0;
    }
    unit = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
    if (!(unit->flags & 0x200)) {
        return 0;
    }
    if (unit->lookupId != count) {
        return 0;
    }
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
        if ((unit->flags & 0x100) && unit->lookupId != 1) {
            func_001E3108((u8 *)unit, (s128 *)pos);
            pos[2] = unit->positionZ - 90.0f;
            btlSetUnitPosition(unit, pos);
        }
    }
    return 1;
}

typedef struct BtlWorldTransform {
    u8 pad00[0x40];
    f32 pos[3];
    u8 pad4C[4];
    f32 rot[4];
} BtlWorldTransform;

typedef struct BtlWorldObject {
    u8 pad00[0x1C];
    BtlWorldTransform *transform;
} BtlWorldObject;

void btlDebugPrintWorldTransform(s32 arg0, u8 *arg1) {
    BtlWorldObject *object;
    if (((BtlWork *)btlGetRuntime())->battleFlags & 2) {
        object = (BtlWorldObject *)dds3GetWorldCameraObject(dds3GetWorldObject());
        if (object != 0) {
            btlBossDebugPrintfN(arg0, (s32)arg1, 0, "P:%.1f %.1f %.1f", (double)object->transform->pos[0],
                                (double)object->transform->pos[1], (double)object->transform->pos[2]);
            btlBossDebugPrintfN(arg0, (s32)(arg1 + 0xC), 0, "R:%.3f %.3f %.3f %.3f",
                                (double)object->transform->rot[0], (double)object->transform->rot[1],
                                (double)object->transform->rot[2], (double)object->transform->rot[3]);
        }
    }
}

extern void func_00208750(s32, s32, s32);

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
            func_00208750(action->actorIndices, 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if (target->flags & 0x80000) {
                if (btlAimHorizontalDirectionVU(&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation(target, &vec[2]);
                }
            }
        }
        for (i = 0; i < count; i++) {
            btlUnitFaceTarget((BtlUnit *)btlGetIndexListEntry(action->actorIndices, i), target);
        }
    }
}

void btlAimLinkedUnitAtMuzzle(ActionUnit *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    u32 count = btlGetIndexListCount(action->actorIndices);
    if (count != 0) {
        target = action->link->unit;
        if (count == 1) {
            btlUnitGetMuzzlePosVU((BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0));
        } else {
            func_00208750(action->actorIndices, 0, 0);
        }
        pos = &vec[1];
        VU0_STORE_VF(vf10, pos);
        if (target != 0) {
            btlUnitGetMuzzlePosVU(target);
            VU0_STORE_VF(vf10, &vec[0]);
            if (target->flags & 0x80000) {
                if (btlAimHorizontalDirectionVU(&vec[0], pos) != 0) {
                    VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
                    btlSetUnitRotation(target, &vec[2]);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", btlMatchFirstLinkedActorFlags);

s32 btlHasMarkedEntry14(s32 actor) {
    s32 linked = (s32)((ActionUnit *)actor)->link;
    u32 count;
    u8 *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = btlGetIndexListCount(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (i = 0; i < count; i++, entry += 0x59C) {
        if (entry[0x14] != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlCanUseLinkedActor(s32 actor) {
    u32 status = ((ActionUnit *)actor)->status;
    s32 linked;
    s32 category;

    switch (status) {
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    default:
        return 1;
    }
    linked = (s32)((ActionUnit *)actor)->link;
    if (linked == 0) {
        return 1;
    }
    if (btlHasMarkedEntry14(actor)) {
        return 0;
    }
    if (((BattleActionLinkState *)linked)->unit->conditionFlags & 0x480) {
        return 0;
    }
    category = ((ActionUnit *)actor)->category;
    if (category != 0 && (((BtlActionTableEntry *)datActionAnimationRecords)[category].flags & 1)) {
        return 0;
    }
    return 1;
}

s32 btlHasMarkedEntry10(s32 actor) {
    s32 linked = (s32)((ActionUnit *)actor)->link;
    u32 count;
    u8 *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = btlGetIndexListCount(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (i = 0; i < count; i++, entry += 0x59C) {
        if (entry[0x10] != 0) {
            return 1;
        }
    }
    return 0;
}

extern f32 btlUnitGetTopY(s32);

s32 btlCheckActorDistanceLimit(void) {
    BtlUnit *unit = ((BtlWork *)btlGetRuntime())->actorList;

    while (unit != NULL) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY((s32)unit) > 400.0f) {
                    return 0;
                }
            }
        }
        unit = unit->nextActor;
    }
    return 1;
}

s32 btlIsEntryHeightWithinLimit(void) {
    if (func_00208000(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

s32 btlHasIdleLinkedSlotKindTwo(u8 *actor) {
    u8 *linked = (u8 *)((ActionUnit *)actor)->link;
    u32 count;
    u32 i;
    u8 *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (; i < count; i++, entry += 0x59C) {
        if (entry[0x10] == 0 && *(s32 *)(entry + 8) == 1 && *(s32 *)(entry + 0xC) == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(((BattleActionLinkState *)linked)->actorIndices, i))->flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor) {
    u8 *linked = (u8 *)((ActionUnit *)actor)->link;
    u32 count;
    u32 i;
    u8 *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (; i < count; i++, entry += 0x59C) {
        if (*(s32 *)(entry + 8) == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(((BattleActionLinkState *)linked)->actorIndices, i))->flags & 0x80002000)) {
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
    if (((ActionUnit *)fx)->link == 0) {
        return 0;
    }
    if (btlHasSingleLinkedResource() == 0) {
        return 0;
    }
    task = (u8 *)((ActionUnit *)fx)->link;
    owner = (u8 *)((BattleActionLinkState *)task)->unit;
    index = ((BattleActionLinkState *)task)->resourceNodeIndex;
    table = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)owner)->resourceKind, ((BtlUnit *)owner)->resourceIndex);
    if (((ActionUnit *)fx)->category == 0x91) {
        return 0;
    }
    return ((BtlEffectResource *)table)->nodes[index].triggerKind == 2;
}

static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

s32 btlHasActorCategoryFlag100(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 0x100);
}

s32 btlIsActorCategoryTypeTwo(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return ((BtlCategoryTableEntry *)datCommandRecords)[category].categoryType == 2;
}

extern s32 btlIsActorCategoryMarked(s32);

s32 btlCanUseActorCategoryFlag2(s32 actor) {
    s32 category;

    if (btlIsActorCategoryMarked(actor)) {
        return 1;
    }
    if (!btlCanUseLinkedActor(actor)) {
        return 0;
    }
    category = ((ActionUnit *)actor)->category;
    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 2);
}

s32 btlHasSingleLinkedResource(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category != 0 && ((BtlCategoryTableEntry *)datCommandRecords)[category].restriction != 0) {
        return 0;
    }
    return btlGetIndexListCount(((ActionUnit *)actor)->actorIndices) == 1;
}

s32 btlCanUseActorCategoryFlag4(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    if ((((BtlCategoryTableEntry *)datCommandRecords)[category].flags09 & 1) == 0) {
        if (!btlCanUseLinkedActor(actor)) {
            return 0;
        }
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[((ActionUnit *)actor)->category].flags, 4);
}

s32 btlIsActorCategoryMarked(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return ((BtlCategoryTableEntry *)datCommandRecords)[category].categoryType == 1;
}

s32 btlHasActorCategoryFlag40(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 0x40);
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    s32 entry;

    switch (((ActionUnit *)actor)->status) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = (s32)((ActionUnit *)actor)->link;
    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(((BattleActionLinkState *)linked)->actorIndices) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(((BattleActionLinkState *)linked)->actorIndices, 0);
    return ((((BattleActionLinkState *)linked)->unit->flags ^ ((BtlUnit *)entry)->flags) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(s32 actor) {
    s32 linked = (s32)((ActionUnit *)actor)->link;
    s32 entry;
    u32 category;

    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(((BattleActionLinkState *)linked)->actorIndices) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(((BattleActionLinkState *)linked)->actorIndices, 0);
    if ((((BtlUnit *)entry)->flags & 0x400) == 0) {
        return 0;
    }
    category = ((BtlUnit *)entry)->resourceIndex;
    if (category >= 0x180) {
        return 0;
    }
    return btlHasFlag(((BtlResourceTableEntry *)datEnemyRecords)[category].flags, 0x1000);
}

u8 func_001EA940(s32 action) {
    return ((ActionUnit *)action)->category == 0x5f;
}

s32 btlMapActorCategory(s32 actor) {
    switch ((u32)((ActionUnit *)actor)->category) {
    case 0x09:
        return 0x29;
    case 0x12:
        return 0x51;
    case 0x1B:
        return 0x2B;
    case 0x24:
        return 0x4D;
    case 0x2D:
        return 0x3C;
    case 0x5B:
        return 0x45;
    case 0x5C:
        return 0x6E;
    case 0x5D:
        return 0xAA;
    default:
        return 0;
    }
}

s32 btlIsSpecialActorCategory(s32 actor) {
    switch ((u32)((ActionUnit *)actor)->category) {
    case 0x5B:
    case 0x5C:
    case 0x5D:
        return 1;
    default:
        return 0;
    }
}

u32 func_001EAA00(void) {
    return 0;
}

u8 func_001EAA08(s32 action) {
    return ((ActionUnit *)action)->category == 0x1a0;
}

void func_001EAA18(void) {
}

void func_001EAA20(void) {
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EAA28);

void func_001EAC08(void) {
}

void func_001EAC10(u32 action) {
    func_001ECBF8(action, action);
}

void func_001EAC28(void) {
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EAC30);

void btlDispatchActionCursorStepByKind(ActionUnit *action) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    if (work->hook64C != 0 && work->hook64C((BtlUnit *)action) != 0) {
        return;
    }
    switch (action->stepKind) {
    case 9:
        func_001F02E0((s32)action, (s32)action);
        break;
    case 0xA:
        func_001F0690((s32)action);
        break;
    case 0xB:
        btlAdvanceCursorForUnmarkedUnit((s32)action, (s32)action);
        break;
    case 0xC:
        func_001F3E48((s32)action);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EAE88);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EB490);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EB5B0);

void btlAdvanceActorStageAndPose(ActionUnit *action) {
    BtlWork *work;
    s32 category;
    u8 *out;
    if (func_001EAA00() != 0) {
        func_001EC5F0((u32)action);
        return;
    }
    work = (BtlWork *)btlGetRuntime();
    if (work->hook66C != 0 && work->hook66C((BtlUnit *)action) != 0) {
        return;
    }
    category = btlMapActorCategory((s32)action);
    if (category > 0 && category == action->stageCount++) {
        btlClearRuntimeFlag2000();
        if (work->hook658 != 0 && work->hook658((BtlUnit *)action) != 0) {
            return;
        }
        if (btlIsSpecialActorCategory((s32)action) != 0) {
            func_001EC868(action, (f32 *)action, 0.0f);
            func_003364B8(-(10.0f * 0.017453293f));
            VU0_STORE_VF_UNCLOBBERED(vf10, (u8 *)action + 0x10);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF(vf10, (u8 *)action + 0x10);
            ((XformData *)action)->f20 += 150.0f;
            out = action->outputPose;
        } else {
            u8 *pose = (u8 *)action->pos30;
            out = action->outputPose;
            func_001EF030(action, pose);
            btlCopyMotionTransform((XformData *)out, (XformData *)pose);
            action->fE0 += 100.0f;
            action->flags |= 0x41;
            ((BattlePoseBlendState *)action)->blendMode = 0;
            ((BattlePoseBlendState *)action)->duration = 30.0f;
        }
        func_001E88A8(out);
        return;
    }
    if (action->stepKind == 0xD) {
        btlAdvanceTargetCursorAnimation((s32)action, (s32)action);
    }
}

void btlUpdateActionPoseForLinkedTarget(ActionUnit *action) {
    BtlUnit *target;
    if (((BtlWork *)btlGetRuntime())->flags220 & 1) {
        target = action->link->unit;
        if (target->flags & 0x400) {
            func_001ECBF8(action, action, target);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform((XformData *)((u8 *)action + 0x30), (XformData *)action);
        func_001F17C8(action, (u8 *)action + 0xC0, action->link->unit, 0);
        ((BattlePoseBlendState *)action)->duration = 7.0f;
        action->flags = (action->flags | 0x1041) & 0xFFFBFFFF;
    } else {
        func_001F17C8(action, action, action->link->unit, 0);
    }
}

void func_001EBE28(void) {
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EBE30);

void func_001EC190(void) {
}

extern void func_001EE690(ActionUnit *, f32 *, s32, f32, f32);

void func_001EC198(ActionUnit *action) {
    BtlUnit *target;
    s32 kind;
    f32 pos[4];
    if (btlGetIndexListCount(action->actorIndices) != 1) {
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0);
    if (action->link->unit->flags & 0x200) {
        func_001F17C8(action, action, target, 0);
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
    func_001EE690(action, (f32 *)action + 12, kind, 45.0f, 0.25f);
    func_001EE690(action, (f32 *)((u8 *)action + 0xC0), kind, 1.0f, 0.5f);
    ((BattlePoseBlendState *)action)->duration = 30.0f;
    action->flags |= 0x41;
}

void func_001EC2A0(void) {
}

void btlStartLinkedActionPoseBlendIfEligible(ActionUnit *action) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    if (action->link->unit->flags & 0x400) {
        if (work->hook660 != 0) {
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
            if (work->hook660((BtlUnit *)action, hasFlag200, hasFlag400) != 0) {
                action->flags |= 0x10000;
                return;
            }
        }
        btlPrepareUnitPoseWithTiltRotation(action, (f32 *)((u8 *)action + 0x30), (u8 *)action + 0xC0);
        ((BattlePoseBlendState *)action)->duration = 200.0f;
        action->flags |= 0x10041;
    } else {
        func_001F41F0(action, action);
    }
}

void btlAdvanceUnblockedPlayerCursorAnimation(u32 unit) {
    if (!(((BtlUnit *)unit)->flags & 0x10000)) {
        btlAdvancePlayerCursorAnimation(unit, unit);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EC418);

void func_001EC5F0(u32 unit) {
    if (!(((BtlUnit *)unit)->flags & 0x10000)) {
        func_001F4F10(unit, unit);
    }
}

void func_001EC620(u32 action) {
    func_001F5018(action, action);
}

void btlAdvanceCommandCursorTask(u32 action) {
    btlAdvanceCommandCursor(action, action);
}

void func_001EC650(u32 action) {
    func_001F2758(action, (s32)action + 0x30, (s32)action + 0xc0);
}

void func_001EC670(void) {
    func_001F2AE8();
}

void btlAppendLinkedUnitToActorIndices(u32 action) {
    s32 actor;

    actor = (s32)action;
    btlAppendIndexListEntry(((ActionUnit *)actor)->actorIndices, (u32)((ActionUnit *)actor)->link->unit);
    func_001F2E30(action, actor + 0x30, actor + 0xc0);
}

void func_001EC6C8(void) {
}

void btlUpdateLinkedActionEffectVectorByTarget(u32 action) {
    if ((((ActionUnit *)action)->link->unit->flags & 0x200) != 0) {
        func_001F25F8(action, action);
        return;
    }
    if (((ActionUnit *)action)->actionKind != 0x10) {
        btlResetUnitEffectVector(action, action);
        return;
    }
}

void func_001EC728(void) {
}

void btlInitializeCursorForLinkedAction(s32 action) {
    if (((ActionUnit *)action)->link != 0) {
        btlInitLinkedUnitActionCursor((s32)((ActionUnit *)action)->link);
        return;
    }
}

void func_001EC760(void) {
}

void func_001EC768(u32 action) {
    func_001F35C8(action, (s32)action + 0x30, (s32)action + 0xc0);
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
    func_001F3888(actor, (u8 *)actor + 0x30, (u8 *)actor + 0xC0);
}

s32 func_001EC7E8(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlWork *)btlGetRuntime())->hook650;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}

s32 func_001EC828(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlWork *)btlGetRuntime())->hook658;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EC868);

void func_001ECBF8(u8 *unit, f32 *vec) {
    func_001EC868(unit, vec, 27.5f);
}

void btlPrepareUnitPoseWithTiltRotation(void *unit, f32 *pose, u8 *out) {
    func_001EC868(unit, pose, 20.0f);
    btlCopyMotionTransform((XformData *)out, (XformData *)pose);
    if (pose[4] > 0.0f) {
        func_00336538(-(20.0f * 0.017453293f));
    } else {
        func_00336538(20.0f * 0.017453293f);
    }
    VU0_STORE_VF(vf10, pose + 4);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, out + 0x10);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001ECCB0);

extern f32 func_00353228(f32);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417D70);

void btlPrepareRandomizedActionCameraPose(ActionUnit *action, XformData *from, XformData *to) {
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
    u32 flags = unit->flags;
    s32 pose;
    f32 fov;
    f32 half;
    f32 span;
    f32 dist;
    if (flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(flags & 0x600);
        btlCopyUnitRotationQuaternion((u8 *)unit, (s128 *)quat);
        pose = effMiscRandMod(0, 4);
        fov = ((XformData *)action)->f24;
        from->f24 = fov;
        to->f24 = fov;
        span = func_00208000(flags & 0x600, 0, 0) * 1.25f;
        VU0_STORE_VF(vf10, &from->vec0);
        if (func_001E3230(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF(vf10, &to->vec0);
        VU0_LOAD_VF(vf11, &from->vec0);
        VU0_LERP_VF10(0.5f);
        VU0_STORE_VF(vf10, &from->vec0);
        half = fov * 0.5f;
        dist = span / func_00353228(half);
        from->f20 = dist;
        dist = unit->unkC0 * unit->scale / func_00353228(half);
        to->f20 = dist * poses[pose][8];
        VU0_LOAD_VF(vf10, poses[pose]);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, &from->vec1);
        VU0_LOAD_VF(vf10, &poses[pose][4]);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, D_003E9130);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, &to->vec1);
        func_001E88A8((u8 *)from);
        func_001E88A8((u8 *)to);
        action->unk154 = poses[pose][9];
        action->flags |= 0x41;
    }
}

void btlAimEffectPoseAtUnit(u8 *fx) {
    BtlUnit *unit = ((ActionUnit *)fx)->link->unit;
    if (unit->flags & 2) {
        if (btlSetActorEffectParameter(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, fx + 0xC0);
        func_001E88A8(fx + 0xC0);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001ED380);

void btlRefreshActionPoseBlendSnapshot(ActionUnit *action) {
    BattlePoseBlendState *pose = (BattlePoseBlendState *)action;
    XformData *saved;
    if (!(action->flags & 1) && pose->state13C == 0) {
        saved = (XformData *)((u8 *)action + 0xC0);
        btlCopyMotionTransform((XformData *)((u8 *)action + 0x30), (XformData *)action);
        btlCopyMotionTransform(saved, (XformData *)action);
        action->fE0 += 125.0f;
        action->flags = (action->flags & ~0x14) | 0x41;
        pose->blendMode = 0;
        pose->state13C = 1;
        pose->duration = 40.0f;
        func_001E88A8((u8 *)saved);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001ED6C8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001ED9A0);

void btlSetupCameraPoseAimUnit(ActionUnit *action, XformData *from, XformData *to) {
    f32 quat[4];
    BtlUnit *unit = action->link->unit;
    f32 fov;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->flags & 0x600);
    btlCopyUnitRotationQuaternion((u8 *)unit, (s128 *)quat);
    fov = ((XformData *)action)->f24;
    from->f24 = fov;
    if (func_001E3230(unit, 1) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_STORE_VF(vf10, &from->vec0);
    from->f20 = unit->unkC0 * unit->scale / func_00353228(fov * 0.5f);
    VU0_LOAD_VF(vf10, quat);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, D_003E9130);
    VU0_NEGATE_XYZ(vf10);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, &from->vec1);
    btlCopyMotionTransform(to, from);
    to->f20 += 550.0f;
    action->flags = (action->flags & ~0x14) | 0x41;
    action->unk154 = 25.0f;
    func_001E88A8((u8 *)from);
    func_001E88A8((u8 *)to);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EDC38);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EDFB8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EE458);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EE690);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EEB78);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EF030);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EF668);

void btlActionAimUserAtTargets(ActionUnit *action, f32 *pose, u8 *out) {
    s128 vec[3];
    BtlUnit *unit = action->link->unit;
    u32 mask = 0;
    u32 i;
    u32 count;
    btlPrepareUnitPoseWithTiltRotation(action, pose, out);
    count = btlGetIndexListCount(action->actorIndices);
    for (i = 0; i < count; i++) {
        mask |= ((BtlUnit *)btlGetIndexListEntry(action->actorIndices, i))->flags & 0x600;
    }
    if (unit->flags & 0x80000) {
        func_00208000(mask, 0, 0);
        VU0_STORE_VF(vf10, &vec[0]);
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU(&vec[1], &vec[0]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation(unit, &vec[2]);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EFA30);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EFEE8);

void btlFlagUserAndTargetDefeat(ActionUnit *command, ActionUnit *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];

    user = command->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(command->actorIndices, 0);
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
    btlUnitFaceTarget(target, user);
    if (userPos[0] < targetPos[0]) {
        command->flags |= 0x200;
    } else {
        command->flags &= ~0x200;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F02E0);

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
        func_001ECBF8(command, command);
        return;
    }
    func_001EF668(command, command->pos30);
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
    command->unk13C = 0;
    command->unk154 = 15.0f;
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
    btlUnitFaceTarget(user, target);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F0690);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F0968);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F0C80);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F1120);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F1290);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F17C8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F1B00);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F1F20);

void func_001F20B0(void *unit, f32 *pose, u8 *out) {
    btlPrepareUnitPoseWithTiltRotation(unit, pose, out);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F20C8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2308);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F25F8);

void btlResetUnitEffectVector(u8 *unit, f32 *vec) {
    func_001EC868(unit, vec, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2758);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2AE8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2E30);

void func_001F3228(u32 action) {
    btlFlagUserAndTargetDefeat(action, action);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F3240);

void func_001F34C8(u32 action) {
    func_001F3228(action);
}

void func_001F34E0(void) {
    func_001F3240();
}

void btlChooseActionPoseBlendFromActorCount(ActionUnit *action) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    u32 count;
    u32 mask;
    BtlUnit *unit;
    mask = ((BtlUnit *)btlGetIndexListEntry(action->actorIndices, 0))->flags & 0x600;
    count = 0;
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & mask) {
                count++;
            }
        }
    }
    if (count >= 2) {
        func_001EDFB8(action, (u8 *)action + 0x30, (u8 *)action + 0xC0);
        return;
    }
    btlPrepareUnitPoseWithTiltRotation(action, (f32 *)((u8 *)action + 0x30), (u8 *)action + 0xC0);
    ((BattlePoseBlendState *)action)->duration = 200.0f;
    action->flags |= 0x41;
}

void func_001F35C0(void) {
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F35C8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F3888);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F3C30);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F3E48);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F41F0);

void btlAdvancePlayerCursorAnimation(s32 action, s32 state) {
    if (!(((ActionUnit *)action)->link->unit->flags & 0x400)) {
        func_001FA480(action, state, D_003BBFA8[CURSOR->unk_0A]);
        func_001FBAC0(action, state);
        func_001FB908(action, state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417EF0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417F30);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004180B0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004180C0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418240);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418250);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418310);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418320);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418330);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418338);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418398);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004183D8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F4E30);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F4F10);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F5018);

void btlAdvanceCommandCursor(s32 action, s32 state) {
    if (CURSOR->mode == 0) {
        func_001FA480(action, state, D_003BBF70[CURSOR->index]);
    } else {
        func_001FA480(action, state, D_003BBF88[CURSOR->index]);
    }
    func_001FBAC0(action, state);
    CURSOR->frame++;
}

typedef struct {
    u8 pad[0x11C];
    u8 category;
} BattleActorLink;

typedef struct {
    u8 pad[0x18];
    BattleActorLink *primary;
} BattleActorLinks;

typedef struct {
    u8 pad[0x114];
    BattleActorLinks *links;
    BattleActorLink *secondary;
    BattleActorLink *tertiary;
} BattleActorLinkOwner;

BattleActorLink *btlFindActorLinkByCategory(BattleActorLinkOwner *actor, s32 category) {
    BattleActorLink *candidate = actor->links->primary;
    if (candidate->category == category) {
        return candidate;
    }
    candidate = actor->secondary;
    if (candidate != NULL && candidate->category == category) {
        return candidate;
    }
    candidate = actor->tertiary;
    if (candidate != NULL && candidate->category == category) {
        return candidate;
    }
    return actor->links->primary;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F5320);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F5780);

void btlUnitGetPosVU(u32 unit, u8 mode) {
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

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004184D8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418568);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F5868);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FA480);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FB908);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FBAC0);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FC5E0);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FD400);

BtlUnit *btlFindActiveActorById(s32 id) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (unit->flags & 0x200) {
                    if (unit->lookupId == id) {
                        return unit;
                    }
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FDD20);

f32 btlGetUnitTargetDistance(u32 unit, u8 mode, s32 target, f32 scale) {
    f32 saved[4];
    f32 pos[4];
    f32 result;
    if (unit == 0 || target == 0) {
        return 0.0f;
    }
    result = func_001F5780(unit, mode, 1.0f, 1.0f);
    btlUnitGetPosVU(unit, mode);
    VU0_STORE_VF_UNCLOBBERED(vf10, pos);
    result = func_001FDD20(pos, result, scale, target);
    VU0_STORE_VF(vf10, saved);
    VU0_LOAD_VF(vf10, saved);
    return result;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FDF18);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FE068);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FE5C0);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FEC00);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FF0F8);

s32 btlCountUnitsByFlags(u32 mask) {
    BtlUnit *unit;
    s32 count = 0;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            if (!(unit->flags & 0x20)) {
                count++;
            }
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FF5D8);

void btlAdvanceCursorForUnmarkedUnit(s32 action, s32 state) {
    if (CURSOR->unk_00 == 1) {
        if (!(((ActionUnit *)action)->link->unit->flags & 0x400)) {
            func_001FA480(action, state, D_003BC0A0[CURSOR->unk_0C]);
            func_001FBAC0(action, state);
            func_001FB908(action, state, 0, 1);
            CURSOR->frame++;
            CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
        }
    }
}

void btlClearCommandCursorAndRunAction(u32 action) {
    memset(D_003BD7D0, 0, 0x130);
    btlFlagUserAndTargetDefeat(action, action);
}

void btlAdvanceCommandCursorOrAction(s32 action, s32 state) {
    if (CURSOR->unk_00 == 1) {
        if (((ActionUnit *)action)->link->unit->flags & 0x400) {
            return;
        }
        func_001FA480(action, state, D_003BC0C0[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        func_001FB908(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        func_001F02E0(action, action);
    }
}

void btlInitCommandCursorForCategory(s32 action, s32 state) {
    memset(D_003BD7D0, 0, 0x130);
    switch (((ActionUnit *)action)->link->unit->mode) {
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

void func_001FFAD8(s32 action, s32 state) {
    if (CURSOR->unk_00 == 1) {
        if (((ActionUnit *)action)->link->unit->flags & 0x400) {
            return;
        }
        func_001FA480(action, state, D_003BC0C8[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        func_001FB908(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        func_001F02E0(action, action);
    }
}

void btlInitCommandCursorForFirstActor(s32 action, s32 state) {
    s32 first;
    btlGetRuntime();
    first = btlGetIndexListEntry(((ActionUnit *)action)->actorIndices, 0);
    memset(CURSOR, 0, 0x130);
    btlRefreshUnitEffectMotionAndEntry((BtlUnit *)first);
    if (btlHasFirstLinkedCategoryFlag1000(action) != 0) {
        func_001F5868(action, state, 5, 1);
    } else {
        func_001F5868(action, state, 5, 0);
    }
    CURSOR->unk_0C = 0;
    func_001F5320(action, state, 0, 0x11);
}

void btlAdvanceTargetCursorAnimation(s32 action, s32 state) {
    if (!(((ActionUnit *)action)->link->unit->flags & 0x400)) {
        func_001FA480(action, state, D_003BC090[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        func_001FB908(action, state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

extern void btlClearAllUnitDefeatCandidatesTask(void);

void btlInitLinkedUnitActionCursor(BattleActionLinkState *linkState) {
    u8 *scene = (u8 *)btlGetRuntime() + 0x70;
    *(BattleActionLinkState **)(scene + 0x114) = linkState;
    memset(D_003BD7D0, 0, 0x130);
    CURSOR->unk_0A = 0;
    CURSOR->unk_0E = 0;
    btlRefreshUnitEffectMotionAndEntry(linkState->unit);
    func_001F5868((s32)scene, (s32)scene, 6, 0);
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(linkState->unit);
    CURSOR->unk_0C = 0;
    func_001F5320((s32)scene, (s32)scene, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FFDD0);

s32 btlInitCursorAndApplyAction(s32 action, s32 state) {
    s32 result;
    memset(D_003BD7D0, 0, 0x130);
    func_001F5868(action, state, 2, 6);
    func_001F5320(action, state, 0, 0);
    result = func_001FB908(action, state, 0, 1);
    CURSOR->unk_0C = 2;
    return result;
}

void btlSpawnBattleWorldAction(void) {
    u64 worldCounter;
    s32 work;
    u32 action;

    work = btlGetRuntime();
    worldCounter = dds3AdvanceWorldCounter();
    action = evtSpawnActionObj9(worldCounter);
    ((BtlWork *)work)->unk228 = action;
    D_00436AD4 = 0;
}

s32 btlAreWorkBuffersReady(void) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    if (work->primaryBuffer != 0) {
        if (work->secondaryBuffer != 0) {
            return 1;
        }
    }
    return 0;
}

void btlReleaseWorkBuffers(void) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
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
    ((BtlWork *)work)->battleFlags = ((BtlWork *)work)->battleFlags & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418C58);

void btlFreeFieldBlocks(void) {
    BattleFieldBlocks *blocks = (BattleFieldBlocks *)btlGetRuntime();
    btlWaitForPendingWorkAndReleaseBuffers();
    if (blocks->fieldTB != 0) {
        sdfQueueNonzeroResourceId(blocks->fieldTB);
        blocks->fieldTB = 0;
        btlBossDebugPrintf(D_00418C58);
    }
    if (blocks->fieldF2 != 0) {
        sdfQueueNonzeroResourceId(blocks->fieldF2);
        blocks->fieldF2 = 0;
        btlBossDebugPrintf("btl:free field F2\n");
    }
    if (blocks->fieldF1 != 0) {
        sdfQueueNonzeroResourceId(blocks->fieldF1);
        blocks->fieldF1 = 0;
        btlBossDebugPrintf("btl:free field F1\n");
    }
    blocks = (BattleFieldBlocks *)btlGetRuntime();
    ((BtlWork *)blocks)->battleFlags &= ~2;
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
    ((BtlWork *)context)->tint71C = 0x807E5C5E;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00200290);

s32 btlGetActionDefaultOrOverride(s32 index) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
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

void btlQueueTintTransition(u32 resource, u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_003BDCA0;
        transition->soundId = 0;
        transition->currentResource = resource;
        transition->queuedResource = resource;
        return;
    }
    transition = (SoundTransition *)D_003BDCA0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = resource;
}

void btlQueueTintTransitionToZero(u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_003BDCA0;
        transition->soundId = 0;
        transition->currentResource = 0;
        transition->queuedResource = 0;
        return;
    }
    transition = (SoundTransition *)D_003BDCA0;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = 0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
}

extern u32 btlBlendColor(u32, u32, f32);

void btlStepBlendColor(void) {
    SoundTransition *transition = (SoundTransition *)D_003BDCA0;
    if (transition->soundId != 0) {
        transition->currentResource = btlBlendColor(transition->queuedResource, transition->previousResource,
                                                    (f32)transition->soundId / (f32)transition->queuedId);
        transition->soundId += 0xFFFF;
    } else {
        transition->currentResource = transition->queuedResource;
    }
    btlStepTintTransition();
}

void btlDrawTintIfVisible(void) {
    SoundTransition *transition = (SoundTransition *)D_003BDCA0;
    if (transition->currentResource & 0xFF000000) {
        func_0018F840(transition);
    }
}

void sndResetTransition(void) {
    SoundTransition *transition = (SoundTransition *)D_003BDCA0;
    D_00436AD4 = 0;
    btlTintTransitionHoldCount = 0;
    transition->soundId = 0;
    transition->currentResource = 0;
}

void btlClearTintAndEnableCamera(void) {
    btlQueueTintTransitionToZero(0);
    func_00114068(1);
}

void btlUpdateTintAndWorldLight(void) {
    u8 *context = (u8 *)btlGetRuntime();
    f32 *position = D_0037F770[0];

    if (position[0] == 0.0f && position[1] == 0.0f &&
        position[2] == 0.0f) {
        func_00114068(0);
    } else {
        func_00114068(1);
    }
    if (((BtlWork *)context)->flags21C & 0x20) {
        func_00114068(0);
    }
    btlStepBlendColor();
}

void btlTickFieldSwayAndTint(void) {
    s32 work;

    work = btlGetRuntime();
    if ((((((BtlWork *)work)->battleFlags & 0x20000) != 0) && ((((BtlWork *)work)->flags21C & 0x20) == 0)) &&
          ((((BtlWork *)work)->flags220 & 0x4000000) == 0)) {
        func_001355D8();
        fldUpdateSwayOffset();
        func_00134A18();
    }
    btlDrawTintIfVisible();
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00200930);

void btlCreateRainEffect(u32 kind, u32 arg) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    switch (kind) {
    case 0xDF:
        switch (arg) {
        case 0:
            break;
        case 1:
        case 3:
        case 4:
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(D_003BDCC8);
            break;
        }
        break;
    case 0xE0:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(D_003BDCC8);
        }
        break;
    case 0xE1:
        if (arg == 5) {
            work->soundTransitionTask = effCreateSelectionFlagListFromWork(D_003BDCC8);
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
    u8 *work = (u8 *)btlGetRuntime();
    if (((BtlWork *)work)->soundTransitionTask != 0) {
        btlBossDebugPrintf("btl:rain exit\n");
        effReleaseSelectionFlagList(((BtlWork *)work)->soundTransitionTask);
        ((BtlWork *)work)->soundTransitionTask = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00200B30);

extern u32 func_00200B30(void);

SoundTask *fldCreateSceneTileTask(s32 value, s32 option) {
    SoundTask *task = btlAllocTask(0x28);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 1;
    task->flags &= ~1;
    task->callback = func_00200B30;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    memset(args, 0, 0x28);
    args->value = value;
    args->option = option;
    *(s32 *)((u8 *)args + 0x24) = 0;
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
    BtlWork *work = (BtlWork *)btlGetRuntime();
    s32 stage = args->stage;
    s32 variant = args->variant;
    char path[0x70];
    if (args->state == 0) {
        func_0035C860(path, "/fld/b/f%03d/f%03d_%03df.tmx", stage, stage, variant);
        result = 0;
        args->frontHandle = (s32)func_002C80C8(path);
        btlBossDebugPrintf("btl:load 0[%s]\n", path);
        func_0035C860(path, "/fld/b/f%03d/f%03d_%03ds.tmx", stage, stage, variant);
        args->sideHandle = (s32)func_002C80C8(path);
        btlBossDebugPrintf("btl:load 1[%s]\n", path);
    } else {
        if (args->frontHandle != 0) {
            if (fileIsRequestReadyInCurrentMode(args->frontHandle) != 0) {
                work->primaryBuffer = (void *)sdfResourceRetainAddress(fileGetResourceHandle(args->frontHandle));
                filePollEntryCleanup(args->frontHandle);
                args->frontHandle = 0;
                btlBossDebugPrintf("btl:floor load end 0\n");
            } else {
                result = 0;
            }
        }
        if (args->sideHandle != 0) {
            if (fileIsRequestReadyInCurrentMode(args->sideHandle) != 0) {
                work->secondaryBuffer = (void *)sdfResourceRetainAddress(fileGetResourceHandle(args->sideHandle));
                filePollEntryCleanup(args->sideHandle);
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

SoundTask *btlCreateFloorLoadTask(s32 first, s32 second) {
    SoundTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 2;
    task->flags &= ~1;
    task->callback = btlPollFloorLoadTask;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    args->unk_08 = first;
    args->unk_0C = second;
    args->value = 0;
    args->option = 0;
    args->unk_10 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00200FB8);

extern u32 func_00200FB8(u32 *);

SoundTask *btlCreateEffectTaskWithSourceParams(u8 *source, u32 value) {
    SoundTask *task = btlAllocTask(0x34);
    u8 *arguments;
    task->enabled = 1;
    task->taskId = 3;
    task->flags |= 2;
    task->callback = func_00200FB8;
    task->status = 0;
    task->onStart = 0;
    arguments = (u8 *)btlGetTaskArguments((s32)task);
    *(u32 *)(arguments + 0x30) = value;
    if (source != 0) {
        memcpy(arguments, source, 0x30);
    } else {
        memcpy(arguments, (u8 *)btlGetRuntime() + 0x10, 0x30);
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00201268);

extern s32 func_00201268();

SoundTask *func_002014A8(value)
    u32 value;
{
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 4;
    task->flags |= 2;
    task->callback = func_00201268;
    task->status = 0;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    return task;
}

s64 func_00201520(void) {
    D_00436AD4 = 1;
    return func_00201268();
}

SoundTask *btlCreateSoundUpdateTask(void) {
    SoundTask *task = (SoundTask *)func_002014A8();
    task->taskId = 7;
    task->callback = func_00201520;
    return task;
}

s32 btlQueueTintTransitionWhenEnabled(u32 *taskArgs) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    if (!(work->battleFlags & 0x20000000)) {
        btlQueueTintTransition(taskArgs[0], *(u16 *)(taskArgs + 1));
    }
    btlTintTransitionHoldCount++;
    return 1;
}

SoundTask *sndCreateAcquireTask(s32 value, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 5;
    task->callback = btlQueueTintTransitionWhenEnabled;
    task->status = 0;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    args->option = option;
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
    if ((((BtlWork *)context)->battleFlags & 0x20000000) != 0) {
        return 1;
    }
    btlQueueTintTransitionToZero(*soundId);
    return 1;
}

extern s32 sndTickFadeCounter();

SoundTask *sndCreateReleaseTask(value)
    u32 value;
{
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 6;
    task->callback = sndTickFadeCounter;
    task->status = 0;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    return task;
}

s64 func_00201718(void) {
    btlTintTransitionHoldCount = 1;
    return sndTickFadeCounter();
}

SoundTask *btlCreateSoundReleaseTask(void) {
    SoundTask *task = (SoundTask *)sndCreateReleaseTask();
    task->taskId = 8;
    task->callback = func_00201718;
    return task;
}

void btlExtendTaskFrameLimit(s32 task, s32 frames) {
    if (*(s32 *)(task + 0xc) < frames) {
        *(s32 *)(task + 0xc) = frames;
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

s32 sndLookupResourceType(s32 actor, s32 resourceIndex) {
    s32 (*hook)(s32, s32) = ((BtlWork *)btlGetRuntime())->hook684;
    s32 type;
    if (hook != 0) {
        type = hook(actor, resourceIndex);
        if (type != -1) {
            return type;
        }
    }
    return ((BtlActionTableEntry *)datActionAnimationRecords)[resourceIndex].resourceType;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00201828);

void sndCreateSystemEffect(u32 *effect) {
    if (effect[0] & 8) {
        if (effect[4] == 0) {
            if (effect[1] == 0) {
                effect[4] = sndMixerClone(effect[5]);
                btlBossDebugPrintf("btl:system effect create[%p]\n", effect[4]);
            }
        }
    }
}

void sndDeleteSystemEffect(u32 *effect) {
    if ((effect[0] & 8) && effect[4] && !effect[1]) {
        btlBossDebugPrintf("btl:system effect delete[%p]\n", effect[4]);
        sndReleaseAllVoices(effect[4]);
        effect[4] = 0;
    }
}

void sndAddEffectReferences(s32 *taskArgs) {
    s32 *effect;
    s32 *target;
    s32 *source;
    taskArgs[2] = 0;
    sndCreateSystemEffect(taskArgs[0]);
    effect = (s32 *)taskArgs[0];
    target = (s32 *)taskArgs[6];
    source = (s32 *)taskArgs[3];
    effect[1] = effect[1] + 1;
    source[0x334 / 4] = source[0x334 / 4] + 1;
    target[0x334 / 4] = target[0x334 / 4] + 1;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00201C98);

void sndReleaseEffectReferences(s32 *taskArgs) {
    s32 *effect;
    s32 *target;
    s32 *source;
    if (taskArgs[2] != 0) {
        effReleaseBattleVoiceOwner(taskArgs[2]);
    }
    effect = (s32 *)taskArgs[0];
    target = (s32 *)taskArgs[6];
    source = (s32 *)taskArgs[3];
    effect[1] = effect[1] - 1;
    source[0x334 / 4] = source[0x334 / 4] - 1;
    target[0x334 / 4] = target[0x334 / 4] - 1;
    sndDeleteSystemEffect((u32 *)effect);
}

extern u32 func_00201C98(u32 *);

SoundTask *btlCreateReferencedSoundEffectTask(u32 effect, s32 soundId, BtlUnit *actor, u16 frames) {
    s32 v1 = 0;
    s32 v2;
    SoundTask *task = btlAllocTask(32);
    SoundTaskArgs *args;
    BtlWork *work;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x2E;
    task->flags |= 2;
    task->owner = actor->owner;
    task->onStart = (void (*)(u32))sndAddEffectReferences;
    task->callback = func_00201C98;
    task->onFinish = (void (*)(u32 *))sndReleaseEffectReferences;
    work = (BtlWork *)btlGetRuntime();
    if (work->hook6E4 != 0) {
        v1 = work->hook6E4(actor);
    }
    if (v1 == 0) {
        v1 = soundId;
    }
    v2 = 0;
    if (work->hook6E8 != 0) {
        v2 = work->hook6E8(actor);
    }
    if (v2 == 0) {
        v2 = soundId;
    }
    if (work->hook6E0 != 0) {
        s32 r = work->hook6E0(actor);
        if (r != 0) {
            actor = (BtlUnit *)r;
        }
    }
    args = btlGetTaskArguments((s32)task);
    args->value = effect;
    args->unk_0C = soundId;
    args->unk_10 = v1;
    args->unk_14 = v2;
    args->optionId = frames;
    args->unk_18 = (u32)actor;
    args->unk_08 = 0;
    args->unk_1C = 0;
    return task;
}

s32 sndCreateEffectWithTargets(s32 effectId, s32 soundId, s32 source, s32 target, s32 actor, s32 frames) {
    s32 effect = btlCreateReferencedSoundEffectTask(effectId, soundId, actor, frames & 0xFFFF);
    SoundTaskArgs *args = btlGetTaskArguments(effect);
    args->unk_10 = source;
    args->unk_14 = target;
    return effect;
}

void sndStartEffectTask(s32 *taskArgs) {
    s32 *effect;
    s32 *unit;
    taskArgs[1] = 0;
    sndCreateSystemEffect(taskArgs[0]);
    effect = (s32 *)taskArgs[0];
    unit = (s32 *)taskArgs[2];
    effect[1] = effect[1] + 1;
    unit[0x334 / 4] = unit[0x334 / 4] + 1;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00202100);

void sndFinishActorEffectTask(s32 *taskArgs) {
    s32 effect;
    s32 unit;

    if (taskArgs[1] != 0) {
        effReleaseBattleVoiceOwner(taskArgs[1]);
    }
    effect = *taskArgs;
    unit = taskArgs[2];
    *(s32 *)(effect + 4) = *(s32 *)(effect + 4) - 1;
    ((BtlUnit *)unit)->unk334 = ((BtlUnit *)unit)->unk334 - 1;
    sndDeleteSystemEffect(effect);
}

extern u32 func_00202100(u32 *);

SoundTask *sndCreateActorEffectTask(u32 effect, BtlUnit *owner, u32 channel) {
    SoundTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x2F;
    task->flags |= 2;
    task->owner = owner->owner;
    task->onStart = (void (*)(u32))sndStartEffectTask;
    task->callback = func_00202100;
    task->onFinish = sndFinishActorEffectTask;
    args = btlGetTaskArguments((s32)task);
    args->value = effect;
    args->unk_08 = (u32)owner;
    args->unk_0C = channel;
    args->option = 0;
    args->unk_10 = 0;
    return task;
}

typedef struct SoundEffectNode {
    u32 flags;
    s32 referenceCount;     /* 0x04 */
    u32 activeCount;        /* 0x08 */
    u8 padC[4];
    u32 handle;
} SoundEffectNode;

void sndIncrementEffectActiveCount(s32 *taskArgs) {
    ((SoundEffectNode *)*taskArgs)->activeCount = ((SoundEffectNode *)*taskArgs)->activeCount + 1;
}

u32 sndWaitEffectFramesAndApplyUnitParameter(u32 *args) {
    u32 *effect = (u32 *)args[0];
    s32 frames;

    if ((effect[0] & 2) == 0) {
        return 0;
    }
    frames = sndGetEffectNodeParameter((s32)effect, (u16)args[1]);
    if ((s32)args[5] >= frames) {
        ((SoundEffectNode *)args[0])->activeCount -= 1;
        if ((s32)args[3] >= 0) {
            btlApplyScaledUnitEffectParameter((u8 *)args[2], args[3], args[4], 1.0f);
        }
        return 1;
    }
    args[5]++;
    return 0;
}

SoundTask *sndCreateTimedUnitEffectTask(u32 effect, BtlUnit *actor, u16 frames, u32 channel, u32 volume) {
    SoundTask *task = btlAllocTask(24);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x31;
    task->owner = actor->owner;
    task->onStart = (void (*)(u32))sndIncrementEffectActiveCount;
    task->callback = sndWaitEffectFramesAndApplyUnitParameter;
    task->onFinish = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = effect;
    args->unk_08 = (u32)actor;
    args->optionId = frames;
    args->unk_0C = channel;
    args->unk_10 = volume;
    args->unk_14 = 0;
    return task;
}

extern void *func_002C80C8(const char *);

typedef struct EffectLoadArgs {
    SoundEffectNode *effect;
    void *loadHandle;
    const char *name;
} EffectLoadArgs;

void sndBeginEffectLoad(EffectLoadArgs *args) {
    SoundEffectNode *effect = args->effect;
    if (effect->flags & 2) {
        if (effect->handle != 0) {
            sndReleaseAllVoices(effect->handle);
            effect->handle = 0;
        }
        effect->flags &= ~2;
    }
    args->loadHandle = func_002C80C8(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

u32 sndPollEffectLoad(s32 arg) {
    EffectLoadArgs *args = (EffectLoadArgs *)arg;
    SoundEffectNode *effect = args->effect;
    s32 resource;
    if (effect->flags & 2) {
        return 1;
    }
    if (fileIsRequestReadyInCurrentMode((s32)args->loadHandle) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:effect load end[%s]\n", args->name);
    resource = fileGetResourceHandle((s32)args->loadHandle);
    effect->handle = sndMixerClone(sdfResourceRetainAddress(resource));
    sdfReleaseResourceAllocation(resource);
    filePollEntryCleanup((s32)args->loadHandle);
    effect->flags = (effect->flags & ~1) | 2;
    return 0;
}

extern u32 sndPollEffectLoad(s32);

SoundTask *sndCreateEffectLoadTask(s32 value, char *name) {
    SoundTask *task = btlAllocTask(strlen(name) + 0xC);
    SoundTaskArgs *args;
    char *copy;
    task->enabled = 1;
    task->taskId = 0x32;
    task->flags &= ~1;
    task->onStart = (void (*)(u32))sndBeginEffectLoad;
    task->callback = sndPollEffectLoad;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    copy = (char *)args + 0xC;
    args->value = value;
    args->unk_08 = (u32)copy;
    strcpy(copy, name);
    return task;
}

u32 btlWaitUnitListIdle(void) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit;
    if (work->battleFlags & 0x40000000) {
        return 1;
    }
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
    }
    return 1;
}

SoundTask *btlCreateWaitUnitListIdleTask(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x33;
    task->callback = btlWaitUnitListIdle;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    return task;
}

u32 sndApplyToActiveActors(taskArgs)
    s32 *taskArgs;
{
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & 2) {
                if (unit->ext != 0) {
                    if (!(unit->flags & 0xE0)) {
                        btlBlendUnitColor(unit, unit->baseColor, *taskArgs);
                    }
                }
            }
        }
    }
    return 1;
}

SoundTask *btlCreateApplyToActiveActorsTask(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x34;
    task->callback = sndApplyToActiveActors;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    return task;
}

u32 btlCancelTimedFadeTask(void) {
    func_001057A8();
    return 1;
}

SoundTask *btlCreateFadeStateResetTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlCancelTimedFadeTask;
    task->taskId = 0x35;
    task->status = 0;
    return task;
}

void sndAddSourceReferences(s32 *taskArgs) {
    s32 *effect;
    s32 *unit;
    taskArgs[1] = 0;
    sndCreateSystemEffect(taskArgs[0]);
    effect = (s32 *)taskArgs[0];
    unit = (s32 *)taskArgs[2];
    effect[1] = effect[1] + 1;
    unit[0x334 / 4] = unit[0x334 / 4] + 1;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00202958);

void sndFinishEffectSourceTask(s32 *taskArgs) {
    s32 effect;
    s32 unit;

    if (taskArgs[1] != 0) {
        effReleaseBattleVoiceOwner(taskArgs[1]);
    }
    effect = *taskArgs;
    unit = taskArgs[2];
    ((SoundEffectNode *)effect)->referenceCount = ((SoundEffectNode *)effect)->referenceCount - 1;
    ((BtlUnit *)unit)->unk334 = ((BtlUnit *)unit)->unk334 - 1;
    sndDeleteSystemEffect(effect);
}

extern u32 func_00202958(u32 *);

SoundTask *sndCreateEffectSourceTask(u32 effect, BtlUnit *owner, u64 resource) {
    SoundTask *task = btlAllocTask(32);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x30;
    task->flags |= 2;
    task->owner = owner->owner;
    task->onStart = (void (*)(u32))sndAddSourceReferences;
    task->callback = func_00202958;
    task->onFinish = sndFinishEffectSourceTask;
    args = btlGetTaskArguments((s32)task);
    args->value = effect;
    args->unk_08 = (u32)owner;
    *(u64 *)&args->unk_10 = resource;
    args->option = 0;
    args->unk_18 = 0;
    args->unk_1C = 0;
    return task;
}

u32 btlTaskStartFadeIn(u32 *taskArgs) {
    kwlnFadeStartIn(*taskArgs);
    return 1;
}

SoundTask *btlCreateFadeInTask(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x38;
    task->callback = btlTaskStartFadeIn;
    task->status = 0;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    return task;
}

u32 btlTaskStartCustomFadeIn(u8 *taskArgs) {
    kwlnFadeInStart(*taskArgs, taskArgs[1], taskArgs[2], *(u32 *)(taskArgs + 4));
    return 1;
}

extern u32 btlTaskStartCustomFadeIn(u8 *);

SoundTask *sndCreateCustomTask(s32 value, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x39;
    task->callback = btlTaskStartCustomFadeIn;
    task->status = 0;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    args->option = option;
    return task;
}

u32 btlTaskSetBattleFlag40000(void) {
    s32 work;

    work = btlGetRuntime();
    ((BtlWork *)work)->battleFlags = ((BtlWork *)work)->battleFlags | 0x40000;
    mdlClearListedObjectFlag();
    return 1;
}

SoundTask *sndCreateSetBattleFlagTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlTaskSetBattleFlag40000;
    task->taskId = 0x3A;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 btlTaskClearBattleFlag40000(void) {
    s32 work;

    work = btlGetRuntime();
    ((BtlWork *)work)->battleFlags = ((BtlWork *)work)->battleFlags & 0xfffbffff;
    mdlSetListedObjectFlag();
    return 1;
}

SoundTask *sndCreateClearBattleFlagTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlTaskClearBattleFlag40000;
    task->taskId = 0x3B;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00202EA8);

void sndSetEffectNodeParameter(s32 effect, u16 option) {
    sndReadSelectedMixerBankValue(((SoundEffectNode *)effect)->handle, option);
}

s32 sndGetEffectNodeParameter(s32 effect, u16 option) {
    return func_00168448(((SoundEffectNode *)effect)->handle, option);
}

s32 sndIsResourceNodeReferencedOrActive(s32 effect) {
    if (((SoundEffectNode *)effect)->referenceCount != 0) {
        return 1;
    }
    return ((SoundEffectNode *)effect)->activeCount != 0;
}

s32 sndHasActiveActor(void) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->node318 != 0 && sndIsResourceNodeReferencedOrActive((s32)unit->node318) != 0) {
            return 1;
        }
    }
    return 0;
}

SoundResourceNode *sndAllocResourceNode(void) {
    SoundResourceNode *node = sdfAllocAndClearQuadwords(0x20);
    BtlWork *work;
    node->unk_04 = 0;
    node->unk_08 = 0;
    node->fadeCountdown = 0;
    node->resourceHandle = 0;
    work = (BtlWork *)btlGetRuntime();
    node->previous = 0;
    if (work->resourceList258 != 0) {
        work->resourceList258->previous = node;
        node->next = work->resourceList258;
    } else {
        node->next = 0;
    }
    work->resourceList258 = node;
    return node;
}

SoundResourceNode *sndCreateResourceNode(u32 soundId) {
    SoundResourceNode *node = (SoundResourceNode *)sndAllocResourceNode();
    node->resourceHandle = sndMixerClone(soundId);
    node->flags |= 2;
    return node;
}

extern void sndReleaseAllVoices(u32);

extern void sdfReleaseChipBlock(void *);

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
        ((BtlWork *)btlGetRuntime())->resourceList258 = node->next;
    }
    sdfReleaseChipBlock(node);
}

void btlUpdateFadeColor(void) {
    BtlWork *context = (BtlWork *)btlGetRuntime();
    SoundResourceNode *node;

    for (node = context->resourceList258; node != 0; node = node->next) {
        if (node->unk_04 == 0) {
            node->fadeCountdown = 0;
        } else if (node->fadeCountdown > 0) {
            node->fadeCountdown = node->fadeCountdown - 1;
        }
    }
    if ((u32)(btlGetActiveUnitId() - 9) < 2 || context->unk2CC != 0 || context->unk22C == 8) {
        context->fadeEnabled = 0;
    } else {
        context->fadeEnabled = 1;
    }
    switch (context->fadeEnabled) {
    case 0: {
        u32 packedColor = context->fadeColor;

        /* The high byte rises by 0x10 per frame, capped at the opaque gray tint. */
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

void sndClearResourceNodes(void) {
    SoundResourceNode *node;
    SoundResourceNode *next;
    for (node = ((BtlWork *)btlGetRuntime())->resourceList258; node != 0; node = next) {
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

SoundResourceLink *sndAllocResourceLink(u32 owner) {
    SoundResourceLink *link = sdfAllocAndClearQuadwords(0x14);
    link->owner = owner;
    link->effectHandle = 0;
    link->flags = 0;
    link->effect = 0;
    return link;
}

void sndFreeResourceLink(SoundResourceLink *link) {
    if (link->effectHandle != 0) {
        effReleaseBattleVoiceOwner(link->effectHandle);
        link->effect[1] = link->effect[1] - 1;
        sndDeleteSystemEffect(link->effect);
    }
    sdfReleaseChipBlock(link);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_002034A8);

void btlMarkTaskReady(s32 link) {
    *(u8 *)(link + 0x10) = 1;
}

SoundLink *sndAllocLink(u32 owner) {
    SoundLink *link = sdfAllocAndClearQuadwords(0x10);
    link->owner = owner;
    link->effectHandle = 0;
    link->flags = 0;
    link->effect = 0;
    return link;
}

void sndFreeLink(SoundLink *link) {
    if (link->effectHandle != 0) {
        effReleaseBattleVoiceOwner(link->effectHandle);
        link->effect[1] = link->effect[1] - 1;
        sndDeleteSystemEffect(link->effect);
    }
    sdfReleaseChipBlock(link);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00203890);

u32 btlDisableBattleFade(void) {
    s32 work;

    work = btlGetRuntime();
    ((BtlWork *)work)->fadeEnabled = 0;
    return 1;
}

SoundTask *sndCreateClearStateTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlDisableBattleFade;
    task->taskId = 0x36;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 btlEnableBattleFade(void) {
    s32 work;

    work = btlGetRuntime();
    ((BtlWork *)work)->fadeEnabled = 1;
    return 1;
}

SoundTask *sndCreateSetStateTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlEnableBattleFade;
    task->taskId = 0x37;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418E58);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418E70);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418E88);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418EA0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418EB8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418ED0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418EE8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418F00);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418F18);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418F30);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418F48);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418F60);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418F78);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418F90);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418FA8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418FC0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418FD8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00418FF0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419008);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419020);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419040);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419060);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419080);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419098);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004190B0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004190C8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004190E0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004190F8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419110);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419128);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419140);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419158);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419170);

void sndLoadSysEffLb(void) {
    const char *path = "/battle/SYSEFF.LB";
    s32 archive = func_002C7FF0(path);
    s32 node;
    u32 i;

    func_002C81D0(archive);
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
    for (; i < 0x31; i++) {
        D_003BDE18[i].unk4 = 0;
        D_003BDE18[i].unk8 = 0;
    }
    func_002C7CE8(archive);
}

void btlRefreshSoundEntries(void) {
    u32 i;
    btlGetRuntime();
    for (i = 0; i < 0x31; i++) {
        if (D_003BDE18[i].unk4 != 0) {
            btlCreateIndexedSoundResourceNode(i, D_003BDE18[i].unk4);
        }
    }
}

void sndFreeBattleSoundEntries(void) {
    SoundResourceNode **slots = (SoundResourceNode **)((u8 *)btlGetRuntime() + 0x4EC);
    u32 i;
    for (i = 0; i < 0x31; i++) {
        if (D_003BDE18[i].unk4 != 0) {
            sndFreeResourceNode(slots[i]);
            slots[i] = 0;
        }
    }
}

void btlCreateIndexedSoundResourceNode(s32 slotIndex, u32 handle) {
    u32 flags;
    s32 work;
    u32 *node;

    work = btlGetRuntime();
    node = (u32 *)sndAllocResourceNode();
    flags = *node;
    node[5] = handle;
    *(u32 **)(slotIndex * 4 + work + 0x4ec) = node;
    *node = flags | 10;
}

typedef struct SoundHandleNode {
    u32 handle;
    void *actor;
} SoundHandleNode;

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(8);
    SoundEntry *entry = &D_003BDE18[index];
    node->actor = actor;
    node->handle = func_002D4138(entry->unk4);
    return node;
}

extern void mdlLoadPrimaryVectorVU(s32);

extern void fileQueueSetPosition(s32, f32 *);

extern void fileQueueUpdate(s32);

void btlUpdateJobPositionFromModel(s32 *args) {
    f32 pos[4];
    if (sdfLoadMapRecordPositionVector(*(s32 *)(args[1] + 0x18), 1) == 0) {
        mdlLoadPrimaryVectorVU(args[1]);
        VU0_STORE_VF(vf10, pos);
        pos[1] -= 150.0f;
    } else {
        VU0_STORE_VF(vf10, pos);
    }
    fileQueueSetPosition(args[0], pos);
    fileQueueUpdate(args[0]);
}

void sndDestroyFileQueueWrapper(u32 queue) {
    fileQueueDestroy(*(u32 *)queue);
    sdfReleaseChipBlock(queue);
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
    SoundSceneEntry *entry;
    if (soundIndex != 0) {
        v = *(u16 *)(D_00435E0C + soundIndex * 0x190 + 4);
    }
    entry = (SoundSceneEntry *)(sceneIndex * 0x28 + datBattleSceneRecords);
    if (entry->unk24 != 0) {
        v = entry->unk24;
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
    if ((((BtlWork *)work)->battleFlags & 0x10000) != 0) {
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

SoundTask *sndCreateStationedSeTask(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x5A;
    task->callback = sndPlayStationedSe;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    args->value = value;
    return task;
}

s32 sndHasFlaggedActiveNode(void) {
    s32 node = (s32)((BtlWork *)btlGetRuntime())->soundList;
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
    BtlWork *work = (BtlWork *)btlGetRuntime();
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

SoundTask *sndCreateSkillSeTask(s32 *taskArgs, u16 optionId) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x57;
    task->callback = sndPlaySkillSeTask;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
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

typedef struct FileLoadArgs {
    SoundFileNode *node;
    void *loadHandle;
    s32 resourceHandle;
    s32 frames;
    const char *name;
} FileLoadArgs;

void sndStartFileLoad(FileLoadArgs *args) {
    SoundFileNode *node = args->node;
    args->loadHandle = func_002C80C8(args->name);
    node->flags |= 1;
    node->position = (args->frames + 0x200) << 16;
    node->mode = 2;
    btlBossDebugPrintf("btl:sound file load start[%s]\n", args->name);
}

u32 sndPollMotSeFileAndSpu(FileLoadArgs *request) {
    SoundFileNode *node = request->node;
    if (sndHasActiveFileLoad()) {
        btlBossDebugPrintf("btl:sound wait[motSE]\n");
        return 0;
    }
    if ((node->flags & 2) == 0) {
        if (fileIsRequestReadyInCurrentMode((s32)request->loadHandle)) {
            s32 size;
            s32 data;
            btlBossDebugPrintf("btl:sound file load end[%s]\n", request->name);
            request->resourceHandle = fileGetResourceHandle((s32)request->loadHandle);
            size = fileGetResourceSize((s32)request->loadHandle);
            data = sdfResourceRetainAddress(request->resourceHandle);
            if (sndFindPackedTrackLoadStatus(node->position) == 0) {
                func_003422F8(data, size);
                node->flags |= 8;
                btlBossDebugPrintf("btl:sound SPU load start[%X][size:%d]\n", *(u16 *)((u8 *)node + 0xA), size);
            }
            node->flags = (node->flags & ~1) | 2;
        }
    } else if (sndFindPackedTrackLoadStatus(node->position) != 0) {
        btlBossDebugPrintf("btl:sound SPU load end[%X]\n", *(u16 *)((u8 *)node + 0xA));
        sdfReleaseResourceAllocation(request->resourceHandle);
        filePollEntryCleanup((s32)request->loadHandle);
        node->flags = (node->flags & ~8) | 0x10;
        return 1;
    }
    return 0;
}

SoundTask *sndCreateFileLoadTask(s32 value, s32 option, char *name) {
    SoundTask *task = btlAllocTask(strlen(name) + 0x14);
    SoundTaskArgs *args;
    char *copy;
    task->enabled = 1;
    task->taskId = 0x58;
    task->flags &= ~1;
    task->onStart = sndStartFileLoad;
    task->callback = sndPollMotSeFileAndSpu;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    copy = (char *)args + 0x14;
    args->value = value;
    args->unk_0C = option;
    args->unk_10 = (u32)copy;
    strcpy(copy, name);
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

SoundTask *sndCreateDataFileLoadTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = sndLoadDataFile;
    task->taskId = 0x5B;
    task->owner = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    return task;
}

s32 sndIsCommandBusySigned(void) {
    return (s8)sdfSoundIsCommandBusy();
}

s32 sndHasResourceFlagsOneOrEight(s32 resource) {
    s32 flags;

    flags = *(s32 *)resource;
    if ((flags & 1) != 0) {
        return 1;
    }
    return (flags & 8) > 0;
}

void sndFormatResourceNameFromIndex(s32 source, s32 output) {
    func_0035C860(output, D_004192D8, D_00436AE8, (u16)(source + 0x200));
}

void sndFormatResourceNameFromUnitMode(s32 unit, s32 output) {
    func_0035C860(output, D_004192E8, ((BtlUnit *)unit)->mode);
}

s32 sndResolveResourceId(s32 category, s32 id) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    s32 type = -1;
    if (work->hook688 != 0) {
        type = work->hook688(category, id);
    }
    if (type == -1) {
        type = sndLookupResourceType(category, id);
    }
    if (type < 26) {
        if (type == 0) {
            return -1;
        }
        if (type >= 11) {
            return type + work->unk208 - 6;
        }
        return type + 0xFFFF;
    }
    return -1;
}

ActiveSoundNode *sndAllocListNode(void) {
    ActiveSoundNode *node = sdfAllocAndClearQuadwords(0x14);
    BtlWork *work = (BtlWork *)btlGetRuntime();
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

void sndFreeListNode(ActiveSoundNode *node) {
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        ((BtlWork *)btlGetRuntime())->soundList = node->next;
    }
    sdfReleaseChipBlock(node);
}

void sndClearList(void) {
    ActiveSoundNode *node;
    ActiveSoundNode *next;
    for (node = ((BtlWork *)btlGetRuntime())->soundList; node != 0; node = next) {
        next = node->next;
        sndFreeListNode(node);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_002048C8);

extern char D_004192F8[];
extern char D_00419308[];
extern char D_00419318[];

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004192D8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004192E8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004192F8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419308);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419318);

void sndLoadMotSeFiles(u32 *sound) {
    char filename[0x70];
    u32 slot = 0;
    s32 offset = 0x10;
    u8 *handleTable = (u8 *)sound + 8;
    do {
        u32 id = func_002048C8(sound, slot);
        if (id != 0) {
            if (slot != 0xB) {
                func_0035C860(filename, D_004192D8, D_00436AE8, id >> 16);
            } else if (sound[1] == 0) {
                func_0035C860(filename, D_004192F8, D_00419308, sound[2]);
            } else {
                func_0035C860(filename, D_00419318, D_00419308, sound[2]);
            }
            *(u32 *)(handleTable + offset) = func_002C80C8(filename);
            btlBossDebugPrintf("btl:motSE file load start[%d][%p][%s]\n", slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound[0] |= 1;
}

void *sndFindListNodeForChannel(s32 category, s32 id) {
    u8 *node = *(u8 **)(btlGetRuntime() + 0x260);
    while (node != 0) {
        if (*(s32 *)(node + 4) == category && *(s32 *)(node + 8) == id) {
            return node;
        }
        node = *(u8 **)(node + 0x104);
    }
    return 0;
}

typedef struct SoundSlotOwner {
    u32 flags;
    s32 category;
    s32 id;
    s32 refCount;
    s32 load[2];
    s32 slot[0x1D];
    s32 handle[0x1D];
    struct SoundSlotOwner *prev;
    struct SoundSlotOwner *next;
} SoundSlotOwner;

SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id) {
    SoundSlotOwner *owner = sndFindListNodeForChannel(category, id);
    BtlWork *work;
    if (owner != 0) {
        btlBossDebugPrintf("btl:motSE search hit[%p]\n", owner);
        owner->refCount++;
        return owner;
    }
    owner = sdfAllocAndClearQuadwords(0x108);
    owner->category = category;
    owner->id = id;
    owner->refCount = 1;
    work = (BtlWork *)btlGetRuntime();
    owner->prev = 0;
    if (work->soundSlotOwners != 0) {
        work->soundSlotOwners->prev = owner;
        owner->next = work->soundSlotOwners;
    } else {
        owner->next = 0;
    }
    work->soundSlotOwners = owner;
    if (mdlFlagTest(0xC0F) == 0) {
        sndLoadMotSeFiles((u32 *)owner);
    }
    return owner;
}

extern s32 filePollEntryCleanup(s32);

extern void sdfReleaseResourceAllocation(u32);

void sndReleaseSlotOwner(SoundSlotOwner *owner) {
    u32 i;
    if (--owner->refCount == 0) {
        for (i = 0; i < 0x1D; i++) {
            if (owner->slot[i] != 0) {
                filePollEntryCleanup(owner->slot[i]);
            }
            if (owner->handle[i] != 0) {
                sdfReleaseResourceAllocation(owner->handle[i]);
            }
        }
        if (owner->next != 0) {
            owner->next->prev = owner->prev;
        }
        if (owner->prev != 0) {
            owner->prev->next = owner->next;
        } else {
            ((BtlWork *)btlGetRuntime())->soundSlotOwners = owner->next;
        }
        sdfReleaseChipBlock(owner);
    }
}

void sndReleaseAllSlotOwners(void) {
    SoundSlotOwner *owner;
    SoundSlotOwner *next;
    for (owner = ((BtlWork *)btlGetRuntime())->soundSlotOwners; owner != 0; owner = next) {
        next = owner->next;
        sndReleaseSlotOwner(owner);
    }
}

void btlStartMoveOtherUnitsTask(void) {
    u64 task;

    task = btlCreateHookedUnitSoundTask();
    btlStartTask(task);
}

s32 sndHasActiveFileLoad(void) {
    u8 *node = (u8 *)((BtlWork *)btlGetRuntime())->soundSlotOwners;
    while (node != 0) {
        if ((*(u32 *)node & 8) != 0) {
            return 1;
        }
        node = *(u8 **)(node + 0x104);
    }
    return 0;
}

typedef struct SoundSlotTableEntry {
    u16 unk0;
    u16 fileId;
} SoundSlotTableEntry;

extern SoundSlotTableEntry *btlSelectSideIndexedActorParameterTable(s32, s32);

void btlQueueUnitSoundSlotFileLoad(SoundTaskArgs *args) {
    SoundSlotOwner *owner = (SoundSlotOwner *)((BtlUnit *)args->actor)->unk328;
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
    if (owner->handle[args->unk_08] == 0) {
        return;
    }
    table = btlSelectSideIndexedActorParameterTable(owner->category, owner->id);
    entry = &table[args->unk_08];
    args->option = entry->fileId;
    if (args->unk_08 != 0xB) {
        owner->load[0] = func_002048C8(owner, args->unk_08);
        owner->load[1] = args->unk_08;
        owner->flags |= 4;
        owner->flags &= ~8;
        owner->flags &= ~0x10;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00204E50);

extern u32 func_00204E50(u32 *);

struct SoundTask *btlCreateHookedUnitSoundTask(unit, option)
    BtlUnit *unit;
    s32 option;
{
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    BtlWork *work;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x59;
    task->owner = unit->owner;
    task->onStart = btlQueueUnitSoundSlotFileLoad;
    task->callback = func_00204E50;
    work = (BtlWork *)btlGetRuntime();
    if (work->hook710 != 0) {
        option = work->hook710(unit, option);
    }
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->unk_08 = option;
    args->option = 0;
    args->unk_0C = 0;
    return task;
}

void func_002050D0(void) {
    s32 context = btlGetRuntime();
    ((BtlWork *)context)->unk288 = -1;
    ((BtlWork *)context)->unk28C = -1;
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

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419408);

INCLUDE_ASM(const s32, "game/code_001DD390", func_00205160);

s32 sndHasOccupiedNodeSlots(void) {
    SoundSlotOwner *owner;
    u32 i;
    for (owner = ((BtlWork *)btlGetRuntime())->soundSlotOwners; owner != 0; owner = owner->next) {
        if (!(owner->flags & 2)) {
            for (i = 0; i < 0x1D; i++) {
                if (owner->slot[i] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

u32 sndWaitForEarringPlayback(void) {
    u32 ready;
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

SoundTask *sndCreateEarringTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = sndWaitForEarringPlayback;
    task->taskId = 0x5C;
    task->status = 0;
    return task;
}

typedef struct BtlAt3LoadArgs {
    s32 loadHandle;
    s32 state;
    s32 index;
} BtlAt3LoadArgs;

s32 sndPollAtrac3SELoadTask(BtlAt3LoadArgs *args) {
    char path[0x80];
    s32 resource;
    s32 data;
    s32 size;
    if (args->state == 0) {
        func_0035C860(path, "/soundat3/%s.at3", D_003E0F60[args->index].fileName);
        args->loadHandle = (s32)func_002C80C8(path);
        btlBossDebugPrintf("btl:atrac3 SE load[%s]\n", path);
    } else if (fileIsRequestReadyInCurrentMode(args->loadHandle) != 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        resource = fileGetResourceHandle(args->loadHandle);
        data = sdfResourceRetainAddress(resource);
        size = fileGetResourceSize(args->loadHandle);
        filePollEntryCleanup(args->loadHandle);
        func_002A27A8(data, size, D_003E0F60[args->index].volume);
        sdfReleaseResourceAllocation(resource);
        btlBossDebugPrintf("btl:atrac3 SE load end\n");
        return 1;
    }
    args->state++;
    return 0;
}

SoundTask *sndCreateAtracEffectLoadTask(s32 value) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x5D;
    task->flags &= ~1;
    task->callback = sndPollAtrac3SELoadTask;
    task->status = 0;
    args = btlGetTaskArguments((s32)task);
    args->unk_08 = value;
    args->option = 0;
    args->value = 0;
    return task;
}

typedef struct BtlDeadLoadArgs {
    BtlUnit *unit;
    void *handle;
} BtlDeadLoadArgs;

void sndStartDeadAtracLoad(BtlDeadLoadArgs *args) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *unit;
    s32 id;
    char path[0x70];
    if (work->earringPlaybackCount == 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        unit = args->unit;
        if (unit->flags & 0x200) {
            id = unit->mode;
            if (unit->statBits & 0x10) {
                id += 0x20;
            } else if (unit->flags & 0x1000) {
                id += 0x10;
            }
            func_0035C860(path, D_004192F8, D_00419308, id);
        } else {
            func_0035C860(path, D_00419318, D_00419308, unit->mode);
        }
        args->handle = func_002C80C8(path);
        btlBossDebugPrintf("btl:ATRAC3 dead load start[%s]\n", path);
    }
    work->earringPlaybackCount++;
}

s32 sndDeadAtracPlaybackTask(u32 *args) {
    s32 data;
    s32 size;
    if (args[1] == 0) {
        return 1;
    }
    if (args[2] == 0) {
        if (fileIsRequestReadyInCurrentMode(args[1]) != 0) {
            args[2] = fileGetResourceHandle(args[1]);
            data = sdfResourceRetainAddress(args[2]);
            size = fileGetResourceSize(args[1]);
            filePollEntryCleanup(args[1]);
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

void sndFinishEarringPlaybackTask(s32 *taskArgs) {
    u8 *work = (u8 *)btlGetRuntime();
    if (taskArgs[2] != 0) {
        sdfReleaseResourceAllocation(taskArgs[2]);
    }
    ((BtlWork *)work)->earringPlaybackCount += 0xFFFF;
}

SoundTask *sndCreateEarringPlaybackTask(BtlUnit *owner) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x5E;
    task->flags &= ~1;
    task->owner = owner->owner;
    task->onStart = sndStartDeadAtracLoad;
    task->callback = sndDeadAtracPlaybackTask;
    task->onFinish = (void (*)(u32 *))sndFinishEarringPlaybackTask;
    args = btlGetTaskArguments((s32)task);
    args->actor = owner;
    args->option = 0;
    args->unk_08 = 0;
    return task;
}

u32 btlPlayStationedSe1C(void) {
    sndLoadAndPlayStationedSe(0x1c);
    return 1;
}

SoundTask *btlCreateStationedSe1CTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlPlayStationedSe1C;
    task->taskId = 0x5F;
    task->status = 0;
    return task;
}

u32 btlAdvanceTitleStateAfterSound(void) {
    btlAdvanceTitleState();
    return 1;
}

SoundTask *btlCreateAdvanceTitleStateTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlAdvanceTitleStateAfterSound;
    task->taskId = 0x60;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_002059F0);

INCLUDE_ASM(const s32, "game/code_001DD390", func_00205CC8);

void btlRepositionPartyAroundBattleCenter(void) {
    s128 v;
    PCP_COPY_VECTOR(&v, btlGetRuntime());
    func_002059F0(&v);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00206090);

void btlMoveOtherUnitsAway(BtlUnit *unit) {
    f32 pos[4];
    BtlUnit *other = ((BtlWork *)btlGetRuntime())->actorList;
    u32 mask = unit->link18->flags & 0x600;
    for (; other != NULL; other = other->nextActor) {
        if ((other->flags & 1) && (other->flags & mask) && other != unit->link18) {
            btlClearUnitDefeatCandidate(other);
            if ((other->flags64 & 0x102) == 0x102) {
                func_001E3108((u8 *)other, (s128 *)pos);
                pos[1] += 1000000.0f;
                pos[0] = 0;
                effObjSetInnerFirstVec(other->effectObject, pos);
                btlSetUnitPosition(other, pos);
            }
        }
    }
    func_001E3108((u8 *)unit->link18, (s128 *)pos);
    pos[0] = 0;
    btlSetUnitPosition(unit->link18, pos);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_002061B0);

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

extern void btlClearAllUnitDefeatCandidates(void);

void func_00206570(BattleActionLinkState *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 dir[4];
    f32 rot[4];
    BtlUnit *unit;
    f32 radius;
    u32 i;
    BtlWork *work = (BtlWork *)btlGetRuntime();
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(link->unit->flags & 0x600);
    slot[0] = 0;
    slot[1] = 0;
    slot[2] = 0;
    if (first != 0 && second != 0) {
        slot[link->unit->lookupId] = link->unit;
        slot[first->lookupId] = first;
        slot[second->lookupId] = second;
    } else {
        for (unit = work->actorList; unit != NULL; unit = unit->nextActor) {
            u32 flags = unit->flags;
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
            if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)center) != 0) {
                VU0_STORE_VF_UNCLOBBERED(vf10, rot);
                btlSetUnitRotation(slot[i], (s128 *)rot);
            }
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
        btlUnitFaceTarget(front, target);
    } else {
        btlUnitGetMuzzlePosVU(front);
        VU0_STORE_VF(vf10, &vec[0]);
        func_00208000(target->flags & 0x600, 0, 0);
        VU0_STORE_VF_UNCLOBBERED(vf10, &vec[1]);
        if (btlAimHorizontalDirectionVU(&vec[0], &vec[1]) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, &vec[2]);
            btlSetUnitRotation(front, &vec[2]);
        }
    }
    btlUnitFaceTarget(back, front);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00206970);

void func_00206C10(void) {
}

extern s128 D_003BE0A0;

void func_00206C18(BattleActionLinkState *link, BtlUnit *other) {
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
    for (unit = ((BtlWork *)btlGetRuntime())->actorList; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (unit != slot[0] && unit != slot[1]) {
                    btlClearUnitDefeatCandidate(unit);
                    btlSetUnitPosition(unit, (f32 *)&D_003BE0A0);
                    if ((unit->flags64 & 0x102) == 0x102) {
                        PCP_COPY_VECTOR(pos, &D_003BE0A0);
                        pos[1] += 1000000.0f;
                        effObjSetInnerFirstVec(unit->effectObject, pos);
                    }
                }
            }
        }
    }
    if (btlGetIndexListCount(link->actorIndices) == 1) {
        btlUnitGetMuzzlePosVU((BtlUnit *)btlGetIndexListEntry(link->actorIndices, 0));
    } else {
        func_00208750(link->actorIndices, 0, 0);
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
        if (btlAimHorizontalDirectionVU((s128 *)pos, (s128 *)muzzle) != 0) {
            VU0_STORE_VF_UNCLOBBERED(vf10, rot);
            btlSetUnitRotation(slot[i], (s128 *)rot);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00206EA8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_00207268);

INCLUDE_ASM(const s32, "game/code_001DD390", func_00207438);

u32 btlMoveOtherUnitsForCategory(u32 *command) {
    s32 category = command[1];
    if (category >= 0x1AB) {
        return 1;
    }
    if (category >= 0x5E) {
        return 1;
    }
    if (category >= 0x5B) {
        btlMoveOtherUnitsAway((BtlUnit *)command[0]);
    }
    return 1;
}

SoundTask *btlCreateMoveOtherUnitsTask(BtlUnit *unit, s32 option, s32 target) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = btlMoveOtherUnitsForCategory;
    task->taskId = 0x64;
    task->onStart = 0;
    task->owner = unit->link18->owner;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = option;
    args->unk_08 = target;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_002077C0);

extern u32 func_002077C0(u32 *);

SoundTask *btlCreateSoundPlaybackTask(BtlUnit *unit, u32 soundId, u32 variant, u32 channel, u32 flags) {
    SoundTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_002077C0;
    task->taskId = 0x65;
    task->onStart = 0;
    task->owner = unit->link18->owner;
    args = btlGetTaskArguments((s32)task);
    args->actor = unit;
    args->option = soundId;
    args->unk_08 = variant;
    args->unk_0C = channel;
    args->unk_10 = flags;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419540);

INCLUDE_SDATA(const s32, "game/code_001DD390", btlDeferredTaskHead);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A28);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A30);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A38);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A40);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A48);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A50);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A58);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A60);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A68);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A70);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A78);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A80);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A88);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A90);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A98);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436A9C);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AA0);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AB0);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AB8);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AC0);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AD0);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AD4);

INCLUDE_SDATA(const s32, "game/code_001DD390", btlTintTransitionHoldCount);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AE0);

INCLUDE_SDATA(const s32, "game/code_001DD390", D_00436AE8);

