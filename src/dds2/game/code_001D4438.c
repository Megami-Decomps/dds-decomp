#include "common.h"
#include "btl_command.h"
#include "btl_state.h"
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

typedef struct SceneDescriptor {
    s8 unk00;
    u8 pad01[0x1F];
    u16 flags;
    u8 pad22[6];
} SceneDescriptor;

typedef struct BattleEffectParams {
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
} BattleEffectParams;

typedef struct SceneObject {
    s32 state;
} SceneObject;

extern SceneObject *fldGetSceneObjectTaskUserData(void);

typedef struct SceneTask {
    s32 state;
    u16 actionNumber;         /* 0x04 */
    u8 pad06[2];
    u32 flags;
    u32 options;              /* 0x0C */
    u8 pad10[8];
    BtlUnit *actor;
    u8 pad1C[4];
    s32 command;
    s32 commandValue;         /* 0x24 */
    s32 commandReference;      /* 0x28: resolved for command 4 */
    u8 pad2C[0x14];
    s64 linkedOwnerId;        /* 0x40: owner linked to scene-group/effect tasks */
    void (*onUpdate)(void);   /* 0x48 */
    void (*onComplete)(void); /* 0x4C */
    u16 actionStage;          /* 0x50 */
    u8 pad52[2];
    s32 effect;               /* 0x54 */
    u8 pad58[8];
    struct BtlIndexList *targetList; /* 0x60 */
    u8 pad64[4];
    s64 ownerId;
} SceneTask;



typedef struct SceneScriptState {
    u32 state;
    u8 pad04[0xC];
    u32 value10;
} SceneScriptState;



typedef struct BattleSceneWork {
    u8 pad00[0x218];
    u32 flags;
    u32 subFlags;
    u32 refreshFlags;
    u8 pad224[8];
    s32 currentScene;
    s32 queuedScene;
    s32 frame;
    s32 sceneState;
    u8 pad23C[4];
    s32 scriptState;          /* 0x240 */
    s32 scriptArg;            /* 0x244 */
    ActionStateLink *linkedNodes;
    BtlUnit *actors;
    u8 pad250[0x1E];
    u8 phaseFlag;
    u8 pad26F;
    u16 variant;
    u8 pad272[2];
    s32 step;
    u8 pad278[4];
    u8 unk27C;
    u8 pad27D[0x17];
    s32 effectLayer;
    u8 pad298[8];
    s32 mode;
    u8 pad2A4[8];
    s16 tileX;
    s16 tileY;
    s32 loadStep;             /* 0x2B0 */
    u8 pad2B4[0x10];
    u32 taskParent;
    u8 pad2C8[4];
    s32 pendingTask;          /* 0x2CC */
    u32 sceneObject;
    u32 spriteObject;
    u32 sceneStatus;
    u8 pad2DC[0x1C];
    s32 unk2F8;
    u8 pad2FC[2];
    BtlSceneSlot slots[8];
    u8 pad316[2];
    SceneTask *groupPrimary[20];    /* 0x318 */
    SceneTask *groupSecondary[45];  /* 0x368 */
    SceneTask *groupTertiary[15];   /* 0x41C */
    SceneTask *groupHandles[8];     /* 0x458 */
    u16 groupHandleCount;
    u8 pad47A[2];
    s32 activeGroupCount;
    BtlSceneFadingRecord fading[8];
    SceneTask *currentTask;
    u8 pad4C4[0x10];
    s32 scriptTarget;         /* 0x4D4 */
    u8 pad4D8[0xC];
    u32 values[64];
    s32 (*sceneCallback)();
} BattleSceneWork;
extern SceneControl *btlCommandPanelWork;
extern s16 *btlLinkedSelectionTaskBuffer;
extern SceneDescriptor *datBattleSceneRecords;
extern f32 D_00433724;
extern f32 *D_0037F770[];
extern u32 func_001C82D8(s32, s8);
extern s32 btlCreateEffectTaskWithSourceParams(BattleEffectParams *, s32);
extern s32 btlCountTasksForOwner(s64);

extern SceneInitializer D_003B6938[];

extern u32 btlCountFlaggedSceneActors(void);
extern void btlRemoveTaskFromSceneGroup(SceneTask *);

extern s32 func_001AC750(s32, void *);

extern s32 D_004367C0;

extern char D_003B5D10[];

extern char D_003B5B10[];

extern s32 btlGetEntryFlagsUnlessDisabled(void *);

extern void fldInitializeSceneGroups(void);

extern u32 D_00435E64;

extern u32 D_00435E5C;

extern s32 D_00438F54;
typedef struct SceneEntry {
    u16 flags;                /* 0x000 */
    u8 pad02[6];
    u16 weight;               /* 0x008 */
    u8 pad0A[4];
    u16 mask;                 /* 0x00E */
    u8 pad10[0x1A8];
    s32 link;                 /* 0x1B8 */
    u8 pad1BC[8];
} SceneEntry;

typedef struct SceneParty {
    u8 pad00[0xA60];
    SceneEntry entry[5];
} SceneParty;

extern SceneParty *datGameState;
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
extern s32 btlCreateSoundUpdateTask();
extern s32 btlCreateSoundReleaseTask();
extern s32 btlCreateWaitUnitListIdleTask();
extern s32 btlCreateApplyToActiveActorsTask();
extern s32 btlCreateFadeStateResetTask();
extern void btlCreateGuidePanelTask();
extern void func_001B81B0();
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

typedef struct SceneAiOther {
    u8 pad00[0x7BE];
    s16 index;                /* 0x7BE */
} SceneAiOther;

extern char *D_004367CC;
extern void *sdfAllocAndClearQuadwords(s32);

typedef struct SceneCoordinateRecord {
    u8 pad00[0xC];
    s32 scaledX;
    s32 scaledY;
    u8 pad14[0x68];
    s32 sourceX;
    s32 sourceY;
    u8 pad84[0x1C];
} SceneCoordinateRecord;

typedef struct SceneCoordinateWork {
    u8 pad00[0x18];
    SceneCoordinateRecord *records;
} SceneCoordinateWork;

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
    struct BtlIndexList *pendingSoundList; /* 0x1A8 */
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
    s32 (*hook61C)(BtlUnit *);
    u8 pad620[0x18];
    void (*hook638)(BtlUnit *);
    u8 pad63C[0x10];
    s32 (*hook64C)(BtlUnit *);
    s32 (*hook650)(BtlUnit *);
    s32 (*hook654)(BtlUnit *);
    s32 (*hook658)(BtlUnit *);
    u8 pad65C[4];
    s32 (*hook660)(BtlUnit *, s32, s32);
    s32 (*actionCameraStepHook)(BtlUnit *);
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

/* Action-sequence +0x20 is the SDK BattleIndexWork payload. Its parent
 * allocation is 0x180 bytes, distinct from the world-actor BtlUnit. */
extern void btlResetIndexWork();
extern void btlAdvanceHistoryCounter(BtlUnit *);


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
    struct BtlIndexList *actorIndices;   /* 0x40 */
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

extern u32 dds3AdvanceWorldCounter(void);

extern struct ActionObj *evtSpawnActionObj9(s32);

extern s8 btlSetActorEffectParameter(BtlUnit *, s32);

extern s32 func_0022F180(void);

extern s32 btlGetRuntime(void);

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

typedef struct SoundSceneEntry {
    u8 pad00[0x24];
    u16 unk24;
    u8 pad26[2];
} SoundSceneEntry;

extern s32 sndFindPackedTrackLoadStatus(u32);

typedef struct SoundTask {
    u8 enabled;
    u8 unk_01[7];
    u64 conditionHandle;
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

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern void btlRunTask(SoundTask *);

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

typedef struct BattleFieldBlocks {
    u8 unk_00[0x2B8];
    s32 fieldF1;
    s32 fieldF2;
    s32 fieldTB;
} BattleFieldBlocks;



extern f32 *D_0037F770[];

extern u8 kwlnDefaultColorVector[];

extern void fldApplyLightSetCurrent(void);

extern s32 sndGetEffectNodeParameter(s32, u16);


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
extern void btlClearAllActorEntrySlots(BtlUnit *);
extern void btlReleaseUnitResources(BtlUnit *);
extern void btlInitUnitFxDefaults(BtlFx *);

extern s32 btlCheckSpecialAbility(s32, s32);
extern void func_001F5868(s32, s32, s32, s32);
extern void func_001F5320(s32, s32, s32, s32);
extern s32 effCreateSelectionFlagListFromWork(void *);
extern char D_003BDCC8[];
extern void mnuReleaseSoundBufferLocked(void);
extern void evtSetUnitAlphaTransition(u32, s32, u32);
extern void func_002A27A8(s32, s32, u8);
extern void mdlBroadcastMasked(s32, s32);
extern void func_001AB160(BtlUnit *);
extern s32 func_001E6428(s32 owner, s32 option);


extern void btlDebugPrintf(s32 tag, ...);

extern void fldCreateSceneSpriteTask(s32 sourceTask);

extern void fldAppendTaskToGroup(SceneTask *task);

extern void fldUpdateSceneGroupTask(SceneTask *task);

extern u8 *fldCreateSceneGroupAction(u8 *actor, u32 owner, s32 groupIndex);

extern void btlClearSceneTaskActiveFlag(s32 task);

void btlActionSeqStateSelect(u8 *task) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *unit = (u8 *)((SceneTask *)task)->actor;
    s32 (*hook)(u8 *);
    s32 next;
    u32 flags;
    ((SceneTask *)task)->flags &= ~0x20;
    if (((SceneTask *)task)->actionNumber == 0) {
        btlDispatchStateHandler(task, 0x1B);
        btlBossDebugPrintf("btl:actnum 0 [%p]\n", task);
        return;
    }
    hook = *(s32 (**)(u8 *))(work + 0x630);
    if (hook != 0) {
        next = hook(task);
        if (next != -1) {
            btlDispatchStateHandler(task, next);
            return;
        }
    }
    if (((SceneTask *)task)->flags & 0x40) {
        btlDispatchStateHandler(task, 0xA);
    } else {
        flags = ((BtlUnit *)unit)->flags;
        if (flags & 0x200) {
            if (((BattleSceneWork *)work)->flags & 0x8000) {
                btlDispatchStateHandler(task, 9);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        } else if (flags & 0x400) {
            if (!(((BattleSceneWork *)work)->refreshFlags & 1)) {
                btlDispatchStateHandler(task, 8);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        }
    }
}

extern s32 effOffsetIfOwnerFlagClear();

void btlUnitTurnEndStateSelect(u8 *task) {
    u8 *unit = (u8 *)((SceneTask *)task)->actor;
    u32 flags = ((BtlUnit *)unit)->flags;
    if (flags & 0x200) {
        if (flags & 0x1000) {
            if ((((BtlUnit *)unit)->stateFlags & 0x40) && !(((BtlUnit *)unit)->conditionFlags & 0x5800) &&
                !(((SceneTask *)task)->flags & 0x100)) {
                ((SceneTask *)task)->actionStage = 4;
                ((SceneTask *)task)->effect = effOffsetIfOwnerFlagClear(unit, 0xA4);
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
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    BtlUnit *actor;
    for (actor = scene->actors; actor != 0; actor = actor->nextActor) {
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
    for (actor = scene->actors; actor != 0; actor = actor->nextActor) {
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
    ((SceneTask *)task)->flags = ((SceneTask *)task)->flags & 0xfffffdff;
}

extern void func_001AA850();
extern void btlFlagUnitDefeatCandidate();
extern void btlRefreshUnitMotionSelection();
extern s32 btlAllocateIndexedUnitEffectTask(u8 *, s32, s32, f32);

INCLUDE_ASM(const s32, "game/code_001D4438", btlReleaseIdleUnitSoundAndAdvanceTask);

void func_001D4A30(s32 task) {
    ((SceneTask *)task)->flags = (((SceneTask *)task)->flags | 0x10) & ~0x200;
}

void btlUnitStateSelectAfterAction(u8 *task) {
    u8 *unit = (u8 *)((SceneTask *)task)->actor;
    u32 flags;
    if (!(((BtlUnit *)unit)->flags & 0x20)) {
        ((SceneTask *)task)->flags &= ~0x100;
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
    ((SceneTask *)task)->flags = ((SceneTask *)task)->flags & 0xffffffef;
}

extern s32 fldCheckSceneResourcesIdle(s32);
extern s32 btlBothSidesActive(s32);
extern s32 func_0020EB40(u8 *);

void btlActionSeqCheckDispatch(u8 *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    u32 flags = scene->flags;
    s32 unit = (s32)((SceneTask *)task)->actor;
    BtlUnit *actor;
    if (!(flags & 0x20)) {
        for (actor = scene->actors; actor != 0; actor = actor->nextActor) {
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

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D4CA0);

void func_001D4FE0(void) {
}

extern s32 btlCreateEffObjB();
extern s64 btlStartTask(void *);
extern s32 sndHasActiveActor(void);
extern s64 btlAdvanceRuntimeSequenceCounter(void);
extern SoundTask *btlCreateCommandSoundUpdateTask(void);
extern SoundTask *btlCreateSecondaryCommandSoundTask(void);
extern SoundTask *btlCreateCommandSoundTask(s32, s32);
extern void func_00211360();
extern s32 func_0020F200();

typedef struct TaskBlock {
    u32 word[11];
} TaskBlock;

extern u8 *btlCreateActorParameterDeltaTask(BtlUnit *, TaskBlock *);
extern u8 *btlCreateLinkedEffectTask(BtlUnit *, u32, s32);

/* Starts the command sound tasks and the follow-up action for the acting
 * unit, chosen by its selection flags. */
void btlStartCommandAudioAndSelectedAction(u8 *task) {
    BtlUnit *unit;
    s64 ownerId;
    u8 *effectTask;
    u8 *sceneTask;
    u8 *object;
    s32 effect;
    TaskBlock block;
    u32 hp, mp;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x49) == 0) {
        unit = ((SceneTask *)task)->actor;
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
            if (((SceneTask *)task)->actor->flags & 0x200) {
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
            ((SceneTask *)task)->command = 0xD;
            btlDispatchStateHandler(task, 0xC);
            break;
        case 8:
            sceneTask = fldCreateSceneGroupAction(task, 0x64, 1);
            *sceneTask = 7;
            *(s64 *)(sceneTask + 8) = ownerId;
            ((SceneTask *)sceneTask)->linkedOwnerId = unit->owner;
            btlStartTask(sceneTask);
            memset(&block, 0, 0x2C);
            hp = unit->maxHp;
            block.word[0] = hp / 10;
            mp = unit->unk12C;
            block.word[1] = mp / 10;
            effectTask = btlCreateActorParameterDeltaTask(unit, &block);
            *effectTask = 7;
            *(s64 *)(effectTask + 8) = ownerId;
            btlStartTask(effectTask);
            if ((s32)block.word[0] > 0) {
                object = btlCreateLinkedEffectTask(unit, block.word[0], 0);
                *object = 4;
                *(s64 *)(object + 8) = *(s64 *)(effectTask + 0x38);
                btlStartTask(object);
            }
            if ((s32)block.word[1] > 0) {
                object = btlCreateLinkedEffectTask(unit, block.word[1], 1);
                *object = 4;
                *(s64 *)(object + 8) = *(s64 *)(effectTask + 0x38);
                btlStartTask(object);
            }
            btlDispatchStateHandler(task, 0x1B);
            break;
        case 0x800:
            sceneTask = fldCreateSceneGroupAction(task, 0x64, 1);
            *sceneTask = 7;
            *(s64 *)(sceneTask + 8) = ownerId;
            ((SceneTask *)sceneTask)->linkedOwnerId = unit->owner;
            btlStartTask(sceneTask);
            btlDispatchStateHandler(task, 0x1B);
            break;
        }
        effect = func_0020F200(task);
        if (effect > 0) {
            object = (u8 *)btlCreateEffObjB(unit, effect);
            ((SceneTask *)object)->linkedOwnerId = ownerId;
            btlStartTask(object);
        }
        ((SceneTask *)task)->flags |= 0x200;
    }
}

void func_001D5330(u32 task) {
    btlGetRuntime();
    ((SceneTask *)task)->flags = ((SceneTask *)task)->flags & 0xfffffffb;
    func_001CAB60(task);
}

extern s32 fldGetSceneObjectState(void);
extern void fldSetSceneObjectAndGroupStates(void);
extern s32 btlIsSupportedCommandKind(s32 *);
extern s32 btlAiCheckStatusRollEligibility(BtlTask *);

void func_001D5368(SceneTask *task) {
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    s32 sceneObjectState;

    if (work->flags & 0x20) {
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
        if (btlIsSupportedCommandKind(&task->command) != 0) {
            btlDispatchStateHandler(task, 7);
        } else {
            fldSetSceneObjectAndGroupStates();
            if (btlAiCheckStatusRollEligibility((BtlTask *)task) != 0) {
                btlDispatchStateHandler(task, 0xB);
            } else {
                btlDispatchStateHandler(task, 0xC);
            }
        }
    } else if (work->flags & 0x8000) {
        fldSetSceneObjectAndGroupStates();
        btlDispatchStateHandler(task, 9);
    }
}

void btlCommandResultEffectSelect(u8 *task) {
    s32 selection;

    ((SceneTask *)task)->flags &= ~4;
    switch (((SceneTask *)task)->command) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
        if (((SceneTask *)task)->command == 4) {
            selection = btlGetLoggedIndexedCommandItem(((SceneTask *)task)->commandReference);
        } else {
            selection = ((SceneTask *)task)->commandValue;
        }
        switch (btlGetCommandBlockReason(task, selection)) {
        case 2:
            btlStartTask(btlCreateEffObjB(((SceneTask *)task)->actor, 0x82));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 6:
            btlStartTask(btlCreateEffObjB(((SceneTask *)task)->actor, 0xB0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 8:
            btlStartTask(btlCreateEffObjB(((SceneTask *)task)->actor, 0xB2));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        case 10:
            btlStartTask(btlCreateEffObjB(((SceneTask *)task)->actor, 0xD0));
            sndSetStationedSeVolume(0xD);
            btlDispatchStateHandler(task, 6);
            return;
        }
        break;
    }
    fldCreateSceneSpriteTask((s32)task);
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D5668);

void func_001D5938(s32 task) {
    ((SceneTask *)task)->flags = ((SceneTask *)task)->flags & 0xffffff7f;
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
s32 btlAiTaskUpdate(SceneTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    u16 index;
    if (!(scene->flags & 0x20)) {
        if (sndHasActiveActor() == 0) {
            if (fldCheckSceneResourcesIdle((s32)task->actor) != 0) {
                if (!(task->flags & 0x80)) {
                    index = task->actor->mode;
                    scene->pendingTask = 0;
                    if (datEnemyAiRecords[index].kind != 1 && btlAllocAndCheck(task) != 0) {
                        btlAssignTaskResultAndArgument(task);
                    } else if (datEnemyAiRecords[index].slot != 0) {
                        btlBindActorSlot(task, datEnemyAiRecords[index].slot);
                    } else {
                        btlRunRandomWeightedAiTableAction(task);
                    }
                    task->flags |= 0x80;
                    scene->flags &= ~0x100000;
                }
                if (scene->pendingTask == 0) {
                    scene->flags |= 0x100000;
                    if (btlAiCheckStatusRollEligibility(task) != 0) {
                        btlDispatchStateHandler(task, 0xB);
                    } else {
                        btlDispatchStateHandler(task, 0xC);
                    }
                } else if (kwlnTaskIsRegistered(scene->pendingTask) == 0) {
                    if (task->command == -1) {
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
                    scene->pendingTask = 0;
                }
            }
        }
    }
}

void btlMarkSceneTaskAfterReset(s32 task) {
    func_001C35F0(task, 0, 0);
    ((SceneTask *)task)->flags = ((SceneTask *)task)->flags | 0x20;
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

s32 btlCommandStateSelectC(u8 *task) {
    s32 command;
    s32 ready;
    s32 value;
    BtlUnit *actor;
    if ((((SceneTask *)task)->options & 8) || sndHasActiveActor() == 0) {
        actor = (BtlUnit *)btlGetIndexListEntry(((SceneTask *)task)->targetList, 0);
        command = ((SceneTask *)task)->command;
        ready = 0;
        if (command == 1) {
            ready = 1;
        } else if (command == 2) {
            value = ((SceneTask *)task)->commandValue;
            if (value < 0xFA) {
                if (value < 0xF8) {
                    ready = 0;
                } else {
                    ready = 1;
                }
            }
        }
        if (ready == 1 && (btlIsActiveActor(actor) == 0 || (actor->flags64 & 0xE1) != 1)) {
            btlDispatchStateHandler(task, 0x1B);
        } else if (!(((SceneTask *)task)->options & 8)) {
            btlDispatchStateHandler(task, 0xC);
        } else {
            func_001DD390(task, task + 0x20);
        }
    }
}

void btlResetCommandIndexWork(s32 task) {
    func_001B7830();
    btlResetIndexWork(task + 0x20);
    ((SceneTask *)task)->actor->unk314 = 0xffffffff;
}

void btlCommandStartSoundTasks(u8 *task) {
    u8 *unit;
    s64 ownerId;
    s32 effect;
    u8 *object;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x49) == 0) {
        unit = (u8 *)((SceneTask *)task)->actor;
        ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        if (((BtlUnit *)unit)->flags & 0x200) {
            btlStartTask(btlCreateCommandSoundTask(task, 9));
        } else {
            btlStartTask(btlCreateCommandSoundTask(task, 3));
        }
        if ((((BtlUnit *)unit)->conditionFlags & 0x7FFF) == 0x20) {
            if (((SceneTask *)task)->actor->flags & 0x200) {
                func_00211360(task, 2);
            } else {
                func_00211360(task, 3);
            }
            btlDispatchStateHandler(task, 0xC);
        }
        effect = func_0020F200(task);
        if (effect > 0) {
            object = (u8 *)btlCreateEffObjB(unit, effect);
            ((SceneTask *)object)->linkedOwnerId = ownerId;
            btlStartTask(object);
        }
        ((SceneTask *)task)->flags |= 0x200;
    }
}

extern void func_001DDB60(SceneTask *, s32);

void btlCommandPrintAndFetchOwner(SceneTask *task) {
    s32 command;
    btlBossDebugPrintf("btl:command=%d\n", task->command);
    func_001DDB60(task, (s32)task + 0x20);
    command = task->command;
    if (command > 0) {
        if (command >= 4) {
            if (command < 9) {
                if (command >= 7) {
                    if (btlGetIndexListCount(task->targetList) == 1) {
                        task->ownerId = ((BtlUnit *)btlGetIndexListEntry(task->targetList, 0))->owner;
                    }
                }
            }
        } else {
            if (btlGetIndexListCount(task->targetList) == 1) {
                task->ownerId = ((BtlUnit *)btlGetIndexListEntry(task->targetList, 0))->owner;
            }
        }
    }
}

extern void func_00201828();

void btlProcessEligibleCommandTaskEffects(u8 *task) {
    u8 *commandData = task + 0x20;
    u8 *work = (u8 *)btlGetRuntime();
    s32 owner = (s32)((SceneTask *)task)->actor;
    void (*hook)(u8 *);
    if (fldCheckSceneResourcesIdle(owner) != 0) {
        if (((SceneTask *)task)->actionStage == 2) {
            btlStartTask(btlCreateEffObjB(owner, ((SceneTask *)task)->effect));
        }
        func_00201828(task, commandData);
        hook = *(void (**)(u8 *))(work + 0x634);
        if (hook != 0) {
            hook(task);
        }
        func_001DD390(task, commandData);
    }
}

void func_001D5FA8(void) {
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D5FB0);

typedef struct BattleVisualState {
    u8 pad00[0x110];
    u32 flags;
} BattleVisualState;

typedef struct BattleVisualObject {
    u8 pad00[0x18];
    BattleVisualState *visual;
} BattleVisualObject;

void func_001D7228(void) {
}

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417348);

INCLUDE_RODATA(const s32, "game/code_001D4438", D_00417360);

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D7230);

void func_001D8C78(void) {
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001D8C80);

/* Four callbacks mark the linked visual state; bit 0x4000's meaning is unconfirmed. */
void func_001DA1F8(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DA210);

void func_001DA728(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DA740);

void func_001DABC0(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

extern u8 *btlCreateModelChangeTask(u8 *, s32, s32, s32, s32, u8);

void func_001DABD8(u8 *task) {
    u8 *actor;
    u8 *effectTask;
    u8 *modelTask;
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

void func_001DACE0(BattleVisualObject *object) {
    object->visual->flags = object->visual->flags | 0x4000;
}

INCLUDE_ASM(const s32, "game/code_001D4438", func_001DACF8);

void func_001DB048(void) {
}

void btlCommandTaskStartEffects(BtlCommandTask *task) {
    s32 effectId;
    s32 countdown;
    s32 skipEffect = 0;
    u8 *object;

    if ((task->actor->flags & 0x200) != 0 && (task->flags & 0x20) != 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
    }
    switch (task->kind) {
    case 14:
        task->actor->flags |= 0x2000000;
        effectId = 0xF;
        countdown = 0x64;
        break;
    case 18:
        effectId = 0;
        countdown = 0;
        skipEffect = 1;
        break;
    case 10:
        if ((task->actor->flags & 0x400) != 0) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
        }
        effectId = (task->actor->flags & 0x200) ? 0xF : 0x1E;
        countdown = 0x32;
        if (btlDoesEnabledStatusMatchCurrentId(&task->actor->statBits, 0xDF) != 0) {
            countdown = 0;
        }
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
    if (!skipEffect) {
        btlStartTask(btlCreateEffObjA(0, task->kind));
        object = fldCreateSceneGroupAction((u8 *)task, countdown, 1);
        *(s32 *)(object + 0x28) = effectId;
        ((SceneTask *)object)->linkedOwnerId = task->actor->owner;
        btlStartTask(object);
    }
    if ((task->actor->conditionFlags & 0x480) != 0) {
        btlDispatchStateHandler(task, 0x19);
    } else {
        btlDispatchStateHandler(task, 0x1B);
    }
}
void btlCommandTaskReturnStart(BtlCommandTask *task) {
    BtlUnit *actor;
    u8 *object;

    if (task->linked != 0) {
        actor = task->linked;
    } else {
        actor = task->actor;
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
            btlStartTask(btlCreateEffObjA(actor, task->kind));
        } else if ((task->flags & 0x200) == 0) {
            btlStartTask(btlCreateCommandSoundTask((s32)task, 0x10));
            btlStartTask(btlCreateEffObjB(actor, 0xF));
        }
    }
    if (actor->flags & 0x200) {
        func_001AA868(&actor->statBits, 8);
        btlSyncPlayerWork(actor);
    }
    if (!(actor->stateFlags & 0x40000)) {
        if (btlCheckSpecialAbility((s32)task->actor + 0x120, 0x280) == 0) {
            object = fldCreateSceneGroupAction((u8 *)task, 0x64, 1);
        } else {
            object = fldCreateSceneGroupAction((u8 *)task, 0x32, 1);
        }
        *(s32 *)(object + 0x28) = 0xF;
        ((SceneTask *)object)->linkedOwnerId = task->actor->owner;
        btlStartTask(object);
    }
    object = (u8 *)func_001E6428((s32)actor, 1);
    *(s32 *)(object + 0x28) = 0xF;
    btlStartTask(object);
    actor->stateFlags &= ~0x40000;
}


void btlCommandTaskReturnUpdate(BtlUnit *task) {
    BtlUnit *unit = task->link18;
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
    if (fldReleaseIdleSceneActorResources(task->link18) != 0) {
        if (unit->flags & 0x200) {
            func_001AA868(&unit->statBits, 8);
            func_001AB160(unit);
        }
        fldUpdateSceneGroupTask((SceneTask *)task);
        btlRemoveTaskFromSceneGroup((SceneTask *)task);
        btlDispatchStateHandler(task, 0x1F);
    }
}

void btlStartLinkedActorEffectTask(BtlUnit *unit) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *owner = unit->link18;
    u8 *task;
    if (!(owner->flags & 0x200)) {
        if (func_001B3200(owner) == 0) {
            task = (u8 *)btlCreateEffObjB(unit->link18, 0x67);
            task[0] = 0xA;
            *(u16 *)(task + 8) = 0x43;
            btlStartTask(task);
            if (unit->link18->conditionFlags & 0x480) {
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

void btlRecordLinkedActorOutcome(BtlUnit *unit) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    work->unk278 = work->unk278 + 1;
    if (func_001B2AF8(unit->link18) != 0) {
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

void btlAdvanceStateWhenLinkedTasksFinish(BtlUnit *unit) {
    if (btlCountTasksForOwner(unit->link18->owner) == 0) {
        btlDispatchStateHandler(unit, 0x1D);
    }
}

void btlAdvanceLinkedUnitWhenOwnerIdle(void) {
}

void func_001DC890(BtlUnit *unit) {
    BtlUnit *owner = unit->link18;
    if (btlCountTasksForOwner(owner->owner) == 0) {
        owner->flags &= ~0x4000;
        btlDispatchStateHandler(unit, 2);
    }
}

void btlFinalizeLinkedActionAndAdvanceHistory(void) {
}

void btlUnitTurnEndCommit(BtlUnit *unit) {
    void (*hook)(BtlUnit *) = ((BtlWork *)btlGetRuntime())->hook638;
    BtlUnit *owner = unit->link18;
    if (hook != 0) {
        hook(unit);
    }
    btlAdvanceHistoryCounter(unit);
    btlResetIndexWork((u8 *)unit + 0x20);
    owner->unk314 = -1;
    unit->unkC &= ~1;
    unit->unk14 += 1;
    owner->flags &= ~0x4000;
    fldUpdateSceneGroupTask((SceneTask *)unit);
    if (unit->link18->flags & 0x20) {
        btlUnitTurnEndStateSelect((u8 *)unit);
    } else {
        btlDispatchStateHandler(unit, 2);
    }
}

extern s32 btlGetSideIndexedActorStatusTable(s32, s32);
extern void btlAccumulateEnemyDefeatRewards(BtlUnit *);
extern SoundTask *btlCreateActorModelBlendTask(BtlUnit *, u32, u32, u32, f32);
extern SoundTask *btlCreateSelectedEffectUpdateTask(BtlUnit *);
extern SoundTask *sndCreateStationedSeTask(u32);

void btlStartActorDefeatTransition(BtlUnit *command) {
    BtlWork *work = (BtlWork *)btlGetRuntime();
    BtlUnit *actor = command->link18;
    u16 *profile = &actor->statBits;
    SoundTask *soundTask;
    SoundTask *object;
    s64 sequence;
    s32 entryFlags;
    s32 result;

    actor->unk310 = -1;
    func_001AA850(profile, 0x4000);
    btlGetSideIndexedActorStatusTable(actor->resourceKind, actor->resourceIndex);
    if (!(command->seqFlags & 0x100)) {
        soundTask = btlCreateHookedUnitSoundTask(actor, 11);
        btlStartTask(soundTask);
        sequence = soundTask->unk_38;
    } else {
        sequence = btlAdvanceRuntimeSequenceCounter();
    }
    if (actor->flags & 0x200) {
        if (!(command->seqFlags & 0x100)) {
            if (!(actor->flags & 0x8000000) && actor->unkEC != 11) {
                object = btlCreateActorModelBlendTask(actor, 0, 11, 2, 1.0f);
                object->enabled = 4;
                object->conditionHandle = sequence;
                btlStartTask(object);
            }
            btlRefreshUnitMotionSelection(actor);
        }
    } else if (actor->flags & 0x400) {
        btlAccumulateEnemyDefeatRewards(actor);
        if (actor->flags & 0x8000000) {
            object = btlCreateSelectedEffectUpdateTask(actor);
            object->enabled = 4;
            object->conditionHandle = sequence;
            btlStartTask(object);
            object = sndCreateStationedSeTask(0x1000E);
            object->enabled = 4;
            object->conditionHandle = sequence;
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
                        object->enabled = 4;
                        object->conditionHandle = sequence;
                        btlStartTask(object);
                    }
                    btlRefreshUnitMotionSelection(actor);
                }
            } else {
                object = (SoundTask *)func_001E6428((s32)actor, 0);
                object->enabled = 4;
                object->conditionHandle = sequence;
                btlStartTask(object);
                actor->flags &= ~1;
            }
        }
        actor->statBits &= ~2;
    }
}

void btlRemoveEligibleActorSceneTask(BtlUnit *task) {
    BtlUnit *unit = task->link18;
    BtlWork *work;
    s32 hookResult;
    s32 entryFlags;
    if (unit->flags & 0x200) {
        if (fldReleaseIdleSceneActorResources((BtlUnit *)unit) == 0) {
            return;
        }
        btlResetIndexWork((u8 *)task + 0x20);
        unit->unk314 = -1;
        btlClearAllActorEntrySlots(unit);
        fldUpdateSceneGroupTask((SceneTask *)task);
        btlRemoveTaskFromSceneGroup((SceneTask *)task);
        btlDispatchStateHandler(task, 1);
    } else if (unit->flags & 0x400) {
        hookResult = 0;
        work = (BtlWork *)btlGetRuntime();
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
        fldUpdateSceneGroupTask((SceneTask *)task);
        btlRemoveTaskFromSceneGroup((SceneTask *)task);
        if ((entryFlags & 0x200) && !(unit->flags & 0x40) && hookResult == 0) {
            btlDispatchStateHandler(task, 1);
        } else {
            btlDispatchStateHandler(task, 0x1F);
        }
    }
}


void func_001DCD80(void) {
}

void btlCommandTaskReleaseActor(BtlCommandTask *task) {
    if ((task->flags & 8) != 0) {
        if (fldReleaseIdleSceneActorResources(task->actor) == 0) {
            return;
        }
        if (task->actor != 0) {
            btlReleaseUnitResources(task->actor);
            {
                BtlUnit *fx = task->actor;
                fx->baseColor = 0x80808080;
                fx->overlayColor = 0x80808080;
                fx->flags = fx->flags & 0x700;
                btlInitUnitFxDefaults((BtlFx *)fx);
            }
            {
                BtlUnit *model = task->actor;
                PCP_COPY_VECTOR(model->currentPosition, model->position);
                PCP_COPY_VECTOR(model->orientation, model->rotation);
                model->unk310 = -1;
            }
            task->actor = 0;
            task->flags &= ~8;
        }
    }
    btlClearSceneTaskActiveFlag((s32)task);
    task->flags |= 2;
}

void btlFlagLinkedActorActionInProgress(s32 unit) {
    ((BtlUnit *)((BtlUnit *)unit)->link18)->flags =
        ((BtlUnit *)((BtlUnit *)unit)->link18)->flags | 0x4000;
}

void btlRunHookAndAdvanceUnitState(BtlUnit *unit) {
    void (*hook)(BtlUnit *) = ((BtlWork *)btlGetRuntime())->hook;
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

BtlUnit *btlCreateActionSeq(void) {
    BtlUnit *seq = sdfAllocAndClearQuadwords(0x180);
    BtlWork *work;
    seq->unk4 = 1;
    btlInitBattleIndexWork((u8 *)seq + 0x20);
    work = (BtlWork *)btlGetRuntime();
    seq->prev = 0;
    if (work->head != 0) {
        work->head->prev = seq;
        seq->next = work->head;
    } else {
        seq->next = 0;
    }
    work->head = seq;
    btlDispatchStateHandler(seq, 0);
    btlBossDebugPrintf("btl:action seq create[%p]\n", seq);
    return seq;
}

void btlDestroyActionSeq(BtlUnit *unit) {
    btlBossDebugPrintf("btl:action seq delete[%p]\n", unit);
    btlReleaseObjectBuffers((s32 *)((u8 *)unit + 0x20));
    if (unit->next != 0) {
        unit->next->prev = unit->prev;
    }
    if (unit->prev != 0) {
        unit->prev->next = unit->next;
    } else {
        ((BtlWork *)btlGetRuntime())->head = unit->next;
    }
    sdfReleaseChipBlock(unit);
}

void btlUpdateActionSeqs(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlWork *)btlGetRuntime())->head; unit != 0; unit = next) {
        next = unit->next;
        if (unit->seqFlags & 1) {
            D_003B69D8[unit->state].update(unit);
            unit->stateTime++;
        } else if (unit->seqFlags & 2) {
            btlDestroyActionSeq(unit);
        }
    }
}

void btlDestroyAllActionSeqs(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlWork *)btlGetRuntime())->head; unit != 0; unit = next) {
        next = unit->next;
        btlDestroyActionSeq(unit);
    }
}

BtlUnit *btlFindUnitByActor(BtlUnit *actor) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)btlGetRuntime())->head; unit != 0; unit = unit->next) {
        if (unit->link18 == actor) {
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
    BattleSceneWork *controller = (BattleSceneWork *)btlGetRuntime();
    SceneTask **primary;
    SceneTask **secondary;
    SceneTask **tertiary;
    SceneTask *task;
    s32 color;
    u32 i;

    if ((controller->flags & 4) == 0) {
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
        if (task == NULL || task->actor == NULL) {
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

