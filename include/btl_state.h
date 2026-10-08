#ifndef BTL_STATE_H
#define BTL_STATE_H

#include "btl.h"
#include "btl_command.h"

struct KwlnTask;
struct BtlRuntimeTask;
struct SdfFlagListWork;

/* Three-byte scene scheduling slot; IDs move with their group/countdown. */
typedef struct BtlSceneSlot {
    u8 group;
    u8 remaining;
    u8 id;
} BtlSceneSlot;

typedef struct BtlSceneFadingRecord {
    BtlSceneSlot slot;
    u8 alpha;
    s32 target;
} BtlSceneFadingRecord;

typedef struct BtlItemDrop {
    u16 id;
    u8 count;
    u8 pad03;
} BtlItemDrop;

#ifdef VERSION_DDS2
/* Mode 786 owns the actor pair and vertical-motion state. */
typedef struct BattleLinkedEffectState {
    struct BtlUnit *actor;
    struct BtlUnit *linkedUnit;
    /* The public getter reads the word; boss routing reads signed bytes. */
    union {
        u32 value;
        struct {
            s8 valueByte;
            u8 pad09;
            s8 unk0A;
            u8 pad0B;
        };
    };
    u16 timer;
    u8 active, phase;
    union {
        u32 effect;
        f32 height; /* Actor vertical motion in code_00227288. */
    };
    f32 speed;
} BattleLinkedEffectState;

/* Observed mode-specific payloads allocated into DDS2 battle work +0x718. */
typedef struct BtlSelectCtrl {
    BtlUnit *unit;
    BtlUnit *prevUnit;
    s8 pending;
} BtlSelectCtrl;

typedef struct BattleMarkedCommandState {
    u8 requested;
    u8 current;
    u8 actionFlag;
} BattleMarkedCommandState;

typedef struct BattleEventResourceTriggerState {
    u8 consumed;
    u8 armed;
} BattleEventResourceTriggerState;

typedef union BattleEffectPayload {
    BtlSelectCtrl selection;
    BattleMarkedCommandState markedCommand;
    BattleLinkedEffectState linked;
    BattleEventResourceTriggerState eventTrigger;
    s32 turnCount; /* mode 785 */
    u8 statIndex; /* mode 795: one-byte allocation */
} BattleEffectPayload;

#endif

/* Full battle-work layout for state users; unit/task-only users include btl.h. */
#ifdef VERSION_DDS1

/* Observed fields of the singleton battle work returned by func_001A17F0.
 * This is one object, not separate script/event/actor-update contexts. Holes
 * remain opaque. DDS2 has a different layout, not a uniform offset shift. */
typedef struct BtlState {
    f32 position[4]; /* 0x00: battle origin added to the camera offset at 001DC11C. */
    f32 baselineLightDirection[4];
    f32 baselineLightColor[4];
    f32 baselineAmbientColor[4];
    f32 lightDirection[4];
    f32 lightColor[4]; /* 0x50: scene light color used by battle light transitions. */
    f32 ambientColor[4]; /* 0x60: default ambient color used by battle light transitions. */
    /* The same command payload is passed to the action-camera helpers. */
    BtlLinkedCommand cameraCommand; /* 0x70..0x1AB */
    u8 pad1AC[0x14];
    s16 eventTaskId; /* 0x1C0: -1 when no event task is available */
    u8 pad1C2[2];
    u32 scriptFlags; /* 0x1C4 */
    u32 eventFlags; /* 0x1C8 */
    s16 eventActive; /* 0x1CC */
    u8 pad1CE[2];
    s32 eventAction; /* 0x1D0 */
    void *eventResult; /* 0x1D4 */
    void *eventRequest; /* 0x1D8 */
    void *eventData; /* 0x1DC */
    BtlUnit *eventUnit; /* 0x1E0 */
    s32 sequenceHandle; /* 0x1E4 */
    s32 scriptHandle; /* 0x1E8 */
    void *eventAssets; /* 0x1EC */
    s32 frame; /* 0x1F0 */
    u32 battleFlags; /* 0x1F4 */
    u32 commandRestrictFlags; /* 0x1F8: bit 0x10 blocks commands with the +0x30 restriction */
    u32 unk_1FC; /* Bit 0x800 bypasses command-block-reason checks. */
    struct EffWorldNode *cameraObject; /* 0x200: world camera stored at 001DC1EC. */
    s32 listener; /* 0x204: stored world-node address. */
    s32 currentScene; /* 0x208 */
    s32 queuedScene; /* 0x20C */
    s32 scenePhaseFrame; /* 0x210: scene-phase counter, distinct from the +0x1F0 model-update clock. */
    s32 sceneState; /* 0x214: scheduled end-phase frame. */
    s16 endDelay; /* 0x218: signed battle-end countdown. */
    u16 endFlags; /* 0x21A: jingle, script-release and stream-reset latches. */
    s32 scriptState; /* 0x21C */
    s32 scriptArg; /* 0x220 */
    BtlTask *tasks; /* 0x224 */
    BtlUnit *units; /* 0x228 */
    u8 pad22C[0x14];
    struct BattleModelEntry *modelEntries; /* 0x240 */
    u16 cameraPresetMode; /* 0x244: selects the marked actor's camera preset. */
    u8 pad246[2];
    u16 turnPhase; /* 0x248 */
    u8 unk24A; /* Nonzero selects stream 2 when the scene has no override. */
    u8 pad24B[1];
    u16 mode; /* 0x24C */
    u8 pad24E[2];
    s32 turnCount; /* 0x250 */
    u8 pad254[4];
    u8 eventReady; /* 0x258 */
    u8 pad259;
    u16 unk25A;
    u16 phase; /* 0x25C */
    u8 requestMode; /* 0x25E: script sets this to 4 with requestArgument */
    u8 pad25F[0xD];
    u16 unk26C;
    u16 unk26E;
    s32 encounterPack; /* 0x270: ENC PACK test selection (func_00215FF8) */
    s32 adjustmentGroupIndex; /* 0x274: encounter reward lookup in code_001A1960 */
    s32 adjustmentEntryIndex; /* 0x278: entry within that encounter group */
    s32 battleMode; /* 0x27C */
    s32 requestArgument; /* 0x280: sign-extended script halfword */
    s32 nextAdjustmentEntryIndex; /* 0x284: entry retained for a follow-up encounter. */
    u16 encounterParamA; /* 0x288: scene record +0x1C, else the test-menu default */
    u16 encounterParamB; /* 0x28A: scene record +0x1E, else the test-menu default */
    u8 pad28C[0x10];
    struct KwlnTask *scriptOwner; /* 0x29C: parent task; script tasks use its priority minus one */
    s32 scriptTask; /* 0x2A0: scheduler task handle, not another list pointer */
    s32 boundTask; /* 0x2A4: actor-slot binding task */
    s32 sceneObject; /* 0x2A8: fldDestroySceneTasksAndBuffers destroys this task handle. */
    s32 spriteObject; /* 0x2AC: same cleanup destroys the sprite task. */
    s32 cleanupTask; /* 0x2B0: fldCreateSceneCleanupTask stores its task handle. */
    BtlItemDrop itemDrops[3]; /* 0x2B4: battle defeat item aggregation in code_001A1960 */
    s32 moneyEarned;
    u8 pad2C4[4];
    s32 experienceEarned; /* 0x2C8: defeat experience accumulator */
    s32 epEarned; /* 0x2CC: distinct defeat EP accumulator */
    u8 pad2D0[4];
    BtlSceneSlot slots[8]; /* 0x2D4: fldClearSceneSlotsAndGroups resets all eight. */
    BtlTask *groupPrimary[20]; /* 0x2EC: scene-group lists in code_001C48A8 */
    BtlTask *groupSecondary[45]; /* 0x33C */
    BtlTask *groupTertiary[15]; /* 0x3F0 */
    u8 pad42C[0x20];
    BtlSceneFadingRecord fading[8]; /* 0x44C: fldInitSceneFadeRecords initializes these. */
    u8 pad48C[8];
    f32 modelFrameScale; /* 0x494 */
    u8 pad498[0xC];
    struct EffectSlotSet *resA; /* 0x4A4: btlLoadResourceBlock stores resA at retail 0x001AC740. */
    struct EffectSlotSet *resB; /* 0x4A8: resource slots used for battle-number glyphs */
    struct EffectSlotSet *resC; /* 0x4AC: btlReleaseResourceBlock clears this at retail 0x001AC7B4. */
    u8 pad4B0[4];
    u32 buttonTextureHandle; /* 0x4B4 */
    struct SoundResourceNode *resources[0x31]; /* 0x4B8: SYSEFF resource slots, indexed like DDS2's */
    u8 pad57C[0x10];
    struct SdfFlagListWork *soundTransitionTask; /* 0x58C */
    u8 pad590[4];
    void (*bossCleanup)(void); /* 0x594 */
    u8 pad598[8];
    s32 (*chooseMotion)(BtlUnit *, s32, s32); /* 0x5A0 */
    u8 pad5A4[8];
    void (*actorParameterDeltaCallback)(BtlUnit *, s32 *); /* 0x5AC */
    u8 pad5B0[8];
    s32 unk_5B8;
    s32 (*effectParameterCallback)(BtlUnit *, s32); /* 0x5BC: actor record-index override, DDS1 001D645C. */
    u8 pad5C0[4];
    BtlUnit *(*findReusableUnit)(u32, u32); /* 0x5C4 */
    u64 (*callback5C8)(u64); /* 0x5C8 */
    s32 (*callback5CC)(BtlUnit *); /* 0x5CC */
    void (*cleanup)(void); /* 0x5D0 */
    u64 (*callback5D4)(u64); /* 0x5D4 */
    void (*prepareModelUnit)(BtlUnit *); /* 0x5D8 */
    void (*beforeMotionUpdate)(void); /* 0x5DC */
    void (*finishModelUnit)(BtlUnit *); /* 0x5E0 */
    u8 pad5E4[8];
    s32 (*serialOverride)(void); /* 0x5EC: -1 cancels a scripted follow-up encounter. */
    void (*updateCallback)(void); /* 0x5F0 */
    u8 pad5F4[0x1C];
    s32 (*cameraStateChangePredicate)(BtlLinkedCommand *); /* 0x610 */
    u8 pad614[0x14];
    s32 (*cameraPoseBlendHook)(BtlLinkedCommand *, s32, s32); /* 0x628 */
    s32 (*actionCameraStepHook)(u8 *); /* 0x62C: nonzero handles the camera step. */
    s32 (*handleActorCategoryCamera)(struct BtlLinkedCommand *, s32, s32); /* 0x630: linked-list flags 0x200 / 0x400. */
    u8 pad634[0x20];
    s32 (*allowDefeatCandidate)(BtlUnit *); /* 0x654 */
    s32 (*unk658)(BtlUnit *);
    u8 pad65C[0x10];
    s32 (*allowPositionEffect)(BtlUnit *); /* 0x66C */
    u8 pad670[0x24];
    BattleEffectState *effect; /* 0x694 */
    u8 pad698[0xC];
    s32 unk_6A4;
    s32 unk_6A8;
    u8 pad6AC[8];
    s32 unk_6B4;
    s32 unk_6B8;
    u8 pad6BC[8];
    s32 unk_6C4;
    u8 pad6C8[0x10];
    s32 unk_6D8;
    u8 pad6DC[8];
    s32 unk_6E4;
    s32 unk_6E8;
    u8 pad6EC[8];
    s32 unk_6F4;
    u8 pad6F8[0x10];
    s32 unk_708;
    s32 table0[0x20]; /* 0x70C */
    s32 table1[0x180]; /* 0x78C */
    s32 table2[0x20]; /* 0xD8C */
    s8 unk_E0C;
    u8 unk_E0D;
    s16 unk_E0E;
} BtlState;
typedef char BtlSceneLightDds1Offset0[((unsigned int)&((BtlState *)0)->baselineLightDirection == 0x10) ? 1 : -1];
typedef char BtlSceneLightDds1Offset1[((unsigned int)&((BtlState *)0)->baselineLightColor == 0x20) ? 1 : -1];
typedef char BtlSceneLightDds1Offset2[((unsigned int)&((BtlState *)0)->baselineAmbientColor == 0x30) ? 1 : -1];
typedef char BtlSceneLightDds1Offset3[((unsigned int)&((BtlState *)0)->lightDirection == 0x40) ? 1 : -1];
typedef char BtlSceneLightDds1Offset4[((unsigned int)&((BtlState *)0)->lightColor == 0x50) ? 1 : -1];
typedef char BtlSceneLightDds1Offset5[((unsigned int)&((BtlState *)0)->ambientColor == 0x60) ? 1 : -1];
typedef char BtlSceneLightDds1Offset6[((unsigned int)&((BtlState *)0)->listener == 0x204) ? 1 : -1];
typedef char BtlSceneLightDds1Extent[(sizeof(BtlState) == 0xE10) ? 1 : -1];
typedef char BtlSceneLightDds1Alignment[(__alignof__(BtlState) == 4) ? 1 : -1];
#endif /* VERSION_DDS1 */

#ifdef VERSION_DDS2
struct ActionStateLink;
struct BtlLinkedCommand;

/* DDS2 0x1A9F30 loads the whole +0x2AC word; 0x1D0020 loads its two
 * signed halfword IDs separately for the field/background resource tasks. */
typedef union BtlBackgroundId {
    u32 packed;
    struct {
        s16 major;
        s16 minor;
    } ids;
} BtlBackgroundId;


/* The 0xFD4-byte singleton allocated by DDS2 0x1A9B80 and returned by
 * 0x1AA6F8. Its script-owner/task pair is +0x2C4/+0x2C8, not DDS1's
 * offsets plus 0x24. Scene groups, actor lists and SYSEFF slots belong here. */
typedef struct BtlState {
    f32 position[4]; /* 0x00: battle origin added to the camera offset at 001E9444. */
    f32 baselineLightDirection[4]; /* 0x10: initializer source for current direction. */
    f32 baselineLightColor[4]; /* 0x20: initializer source for current light color. */
    f32 baselineAmbientColor[4]; /* 0x30: initializer source for current ambient color. */
    f32 lightDirection[4]; /* 0x40: current scene light direction. */
    f32 lightColor[4]; /* 0x50: scene light color used by battle light transitions. */
    f32 ambientColor[4]; /* 0x60: default ambient color used by battle light transitions. */
    /* The same command payload is passed to the action-camera helpers. */
    BtlLinkedCommand cameraCommand; /* 0x70..0x1CF */
    u8 pad1D0[0x14];
    s16 eventTaskId; /* 0x1E4 */
    u8 pad1E6[2];
    u32 scriptFlags; /* 0x1E8 */
    u32 eventFlags; /* 0x1EC */
    s16 eventActive; /* 0x1F0 */
    u8 pad1F2[2];
    s32 eventAction; /* 0x1F4 */
    void *eventResult; /* 0x1F8 */
    void *eventRequest; /* 0x1FC */
    void *eventData; /* 0x200 */
    BtlUnit *eventUnit; /* 0x204 */
    s32 sequenceHandle; /* 0x208 */
    s32 scriptHandle; /* 0x20C */
    void *eventAssets; /* 0x210 */
    u8 pad214[4];
    u32 battleFlags; /* 0x218 */
    u32 commandRestrictFlags; /* 0x21C: bit 0x10 blocks commands with the +0x30 restriction */
    u32 unk220;
    struct EffWorldNode *cameraObject; /* 0x224: world camera stored at 001E9514. */
    s32 unk228;
    s32 currentScene; /* 0x22C */
    s32 queuedScene;
    s32 frame;
    s32 sceneState;
    s16 endDelay; /* 0x23C: signed battle-end countdown. */
    u16 endFlags; /* 0x23E: same end-phase latches as DDS1. */
    s32 scriptState; /* 0x240 */
    s32 scriptArg;
    struct ActionStateLink *tasks; /* 0x248: 0x180-byte sequence list, next at +0x178 */
    BtlUnit *units; /* 0x24C */
    struct BtlRuntimeTask *taskTail; /* 0x250: newest scheduler registration */
    struct BtlRuntimeTask *taskHead; /* 0x254: oldest scheduler registration */
    struct SoundResourceNode *soundResourceHead;
    struct ActiveSoundNode *soundList;
    struct SoundSlotOwner *soundSlotOwners;
    u8 pad264[4];
    union {
        struct {
            u16 unk268;
            u16 cameraActorHighWater;
        };
        u32 cameraActorConfiguration; /* 0x268: mode and persistent actor count. */
    };
    u16 unk26C;
    u8 encounterKind; /* 0x26E: scene setup selects 0, 2 or 3. */
    u8 pad26F;
    u16 mode; /* 0x270 */
    u8 pad272[2];
    s32 turnCount; /* 0x274 */
    s32 unk278;
    u8 eventReady; /* 0x27C */
    u8 pad27D;
    u16 unk27E;
    u16 phase; /* 0x280 */
    u8 requestMode; /* 0x282 */
    u8 pad283;
    u16 earringPlaybackCount;
    u8 pad286[2];
    u32 unk288;
    u32 unk28C;
    u16 unk290;
    u16 unk292;
    s32 effectLayer; /* 0x294 */
    s32 adjustmentGroupIndex; /* 0x298: encounter pack's group selector */
    s32 adjustmentEntryIndex; /* 0x29C: entry selector within the group */
    s32 battleMode; /* 0x2A0 */
    s32 requestArgument; /* 0x2A4 */
    s32 nextAdjustmentEntryIndex; /* 0x2A8: entry retained for a follow-up encounter. */
    BtlBackgroundId background; /* 0x2AC */
    s32 loadStep;
    u8 specialEncounterBlocked; /* 0x2B4: scene setup latch blocks special encounter rolls. */
    u8 pad2B5[0xF];
    struct KwlnTask *scriptOwner; /* 0x2C4: parent task; script tasks use its priority minus one */
    s32 scriptTask; /* 0x2C8: also supplies the task passed to scrSetCurrentActor */
    s32 boundTask; /* 0x2CC */
    u32 sceneObject; /* 0x2D0 */
    u32 spriteObject;
    u32 sceneStatus;
    BtlItemDrop itemDrops[3]; /* 0x2DC */
    s32 moneyEarned;
    s32 moneyTotal;
    s32 experienceEarned; /* 0x2F0 */
    s32 epEarned; /* 0x2F4 */
    s32 unk2F8;
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103 */
    BtlSceneSlot slots[8]; /* 0x2FE */
    u8 pad316[2];
    struct ActionStateLink *groupPrimary[20]; /* 0x318 */
    struct ActionStateLink *groupSecondary[45]; /* 0x368 */
    struct ActionStateLink *groupTertiary[15]; /* 0x41C */
    struct ActionStateLink *groupHandles[8]; /* 0x458 */
    u16 groupHandleCount;
    u8 pad47A[2];
    s32 activeGroupCount;
    BtlSceneFadingRecord fading[8]; /* 0x480 */
    struct ActionStateLink *currentTask;
    s8 unk4C4;
    u8 pad4C5[3];
    f32 unk4C8;
    s32 messageWindows[2];
    s32 scriptTarget;
    u8 pad4D8[4];
    struct EffectSlotSet *resB; /* 0x4DC: resource slots used for battle-number glyphs */
    u8 pad4E0[4];
    u32 unk4E4; /* First word returned by the indexed scene-value API. */
    u32 buttonTextureHandle; /* 0x4E8 */
    struct SoundResourceNode *resources[0x31]; /* 0x4EC: SYSEFF resource slots */
    void *primaryBuffer;
    void *secondaryBuffer;
    u8 fadeEnabled;
    u8 pad5B9[3];
    u32 fadeColor;
    struct SdfFlagListWork *soundTransitionTask; /* 0x5C0 */
    u8 pad5C4[4];
    void (*bossCleanup)(void); /* 0x5C8 */
    s32 (*selectScriptArg)(void); /* 0x5CC */
    u8 pad5D0[4];
    s32 (*chooseMotion)(BtlUnit *, s32, s32); /* 0x5D4 */
    s32 (*unk5D8)(BtlUnit *);
    s32 (*unk5DC)(BtlUnit *, s32);
    void (*actorParameterDeltaCallback)(BtlUnit *, s32 *); /* 0x5E0 */
    s32 (*sceneCallback)(); /* 0x5E4 */
    u8 pad5E8[8];
    s32 (*effectParameterCallback)(BtlUnit *, s32); /* 0x5F0: same override, DDS2 001E3264. */
    u8 pad5F4[4];
    BtlUnit *(*findModelActor)(s32, s32); /* 0x5F8 */
    u64 (*beginBattleEntryTasks)(s32); /* 0x5FC: supplies the model-load dependency. */
    s32 (*selectEntryModelVariant)(BtlUnit *); /* 0x600 */
    u8 pad604[4];
    void (*finishEnemyEntryTasks)(u64); /* 0x608: receives the completed enemy chain. */
    void (*beforeActorModelReady)(BtlUnit *); /* 0x60C */
    u8 pad610[4];
    void (*afterActorModelReady)(BtlUnit *); /* 0x614 */
    s32 (*unk618)(BtlUnit *);
    s32 (*unk61C)(BtlUnit *);
    s32 (*serialOverride)(void); /* 0x620: -1 cancels a scripted follow-up encounter. */
    void (*afterUnitUpdate)(void); /* 0x624 */
    s32 (*selectScriptState)(void); /* 0x628 */
    void (*completionHook)(); /* 0x62C */
    s32 (*actionStateSelectionHook)(struct ActionStateLink *);
    u8 pad634[4];
    void (*commandTurnEndHook)(struct ActionStateLink *);
    s32 (*commandHook)(s32, s32);
    s32 (*cameraArrangementHook)(BtlLinkedCommand *, BtlCamState *, s32); /* 0x640: mode-specific pose override. */
    u8 pad644[4];
    s32 (*unk648)(BtlUnit *);
    s32 (*unk64C)(BtlUnit *);
    s32 (*unk650)(BtlUnit *);
    s32 (*unk654)(BtlUnit *);
    s32 (*unk658)(BtlUnit *);
    u8 pad65C[4];
    s32 (*unk660)(BtlUnit *, s32, s32);
    s32 (*actionCameraStepHook)(struct BtlLinkedCommand *); /* 0x664: nonzero handles the camera step. */
    s32 (*handleActorCategoryCamera)(struct BtlLinkedCommand *, s32, s32); /* 0x668: same category camera override. */
    s32 (*unk66C)(BtlUnit *);
    s32 (*unk670)(struct BtlLinkedCommand *);
    u8 pad674[0x10];
    s32 (*unk684)(s32, s32);
    s32 (*unk688)(s32, s32);
    void (*preActionHook)(struct ActionStateLink *, s32, u64, u64, u64);
    void (*postActionHook)(struct ActionStateLink *, s32, BtlUnit *, u64, u64, s32);
    u8 pad694[8];
    s32 (*unk69C)(BtlUnit *);
    s32 (*unk6A0)(BtlUnit *);
    s32 (*actorEligibilityOverride)(BtlUnit *); /* 0x6A4: optional actor eligibility check. */
    void (*linkedActionHook)(struct ActionStateLink *);
    s32 (*cameraStateChangePredicate)(BtlLinkedCommand *); /* 0x6AC */
    s32 (*cameraUpdatePredicate)(BtlLinkedCommand *); /* 0x6B0: gates the active camera handler. */
    s32 (*unitLiftPredicate)(BtlUnit *); /* 0x6B4 */
    u8 pad6B8[0x18];
    void (*actionResourceNameHook)(struct ActionStateLink *, s32, char *);
    u8 pad6D4[8];
    s32 (*commandRangeOverride)(BtlUnit *, s32); /* 0x6DC: func_001B0B30 calls the range override. */
    BtlUnit *(*selectSoundEffectTarget)(BtlUnit *); /* 0x6E0: DDS2 00201FD8 consumes the returned actor. */
    s32 (*unk6E4)(BtlUnit *);
    s32 (*unk6E8)(BtlUnit *);
    s32 (*scriptReturnHook)(); /* Optional script-return hook; preserve its unspecified retail prototype. */
    void (*unk6F0)(BtlUnit *, s32, s32, s32, s32, f32);
    void (*unk6F4)(BtlUnit *, s32, f32);
    void (*unk6F8)(BtlUnit *, s32, s32);
    s32 (*unk6FC)(BtlUnit *, s32, s32);
    void (*unitReturnHook)(struct ActionStateLink *); /* 0x700: custom return-to-group handling */
    u8 pad704[0xC];
    s32 (*unk710)(BtlUnit *, s32);
    void (*modelChangeSoundHook)(struct ActionStateLink *, u64, s32); /* 0x714: prerequisite handle, delay */
    BattleEffectPayload *effect; /* 0x718: allocation depends on battle mode. */
    u32 tint71C;
    u8 pad720[4];
    s32 unk724;
    s32 unk_728;
    s32 unk_72C;
    u8 pad730[8];
    s32 unk_738;
    s32 unk_73C;
    u8 pad740[8];
    s32 unk_748;
    u8 pad74C[0x10];
    s32 unk_75C;
    u8 pad760[8];
    s32 unk_768;
    s32 unk_76C;
    u8 pad770[8];
    s32 unk_778;
    u8 pad77C[0x10];
    s32 unk_78C;
    s32 table0[0x30]; /* 0x790 */
    s32 table1[0x180]; /* 0x850 */
    s32 table2[0x60]; /* 0xE50 */
    s8 unk_E0C; /* 0xFD0: legacy opaque name, not a DDS2 offset */
    u8 unk_E0D; /* 0xFD1 */
    s16 unk_E0E; /* 0xFD2 */
} BtlState;
typedef char BtlSceneLightOffset0[((unsigned int)&((BtlState *)0)->baselineLightDirection == 0x10) ? 1 : -1];
typedef char BtlSceneLightOffset1[((unsigned int)&((BtlState *)0)->baselineLightColor == 0x20) ? 1 : -1];
typedef char BtlSceneLightOffset2[((unsigned int)&((BtlState *)0)->baselineAmbientColor == 0x30) ? 1 : -1];
typedef char BtlSceneLightOffset3[((unsigned int)&((BtlState *)0)->lightDirection == 0x40) ? 1 : -1];
typedef char BtlSceneLightOffset4[((unsigned int)&((BtlState *)0)->lightColor == 0x50) ? 1 : -1];
typedef char BtlSceneLightOffset5[((unsigned int)&((BtlState *)0)->ambientColor == 0x60) ? 1 : -1];
typedef char BtlSceneLightExtent[(sizeof(BtlState) == 0xFD4) ? 1 : -1];
typedef char BtlSceneLightAlignment[(__alignof__(BtlState) == 4) ? 1 : -1];
#endif /* VERSION_DDS2 */

#endif /* BTL_STATE_H */
