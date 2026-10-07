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
typedef struct BattleLinkedEffectState {
    u32 actor;
    struct BtlUnit *linkedUnit;
    u32 value;
    u16 timer;
    u8 active, phase;
    u32 effect;
    f32 speed;
} BattleLinkedEffectState;
#endif

/* Full battle-work layout for state users; unit/task-only users include btl.h. */
#ifdef VERSION_DDS1

/* Observed fields of the singleton battle work returned by func_001A17F0.
 * This is one object, not separate script/event/actor-update contexts. Holes
 * remain opaque. DDS2 has a different layout, not a uniform offset shift. */
typedef struct BtlState {
    u8 pad000[0x50];
    f32 lightColor[4]; /* 0x50: scene light color used by battle light transitions. */
    f32 ambientColor[4]; /* 0x60: default ambient color used by battle light transitions. */
    u8 pad070[0x30];
    BtlCamState debugStartCamera; /* 0xA0: captured debug camera's initial pose. */
    u8 pad0C8[0x68];
    BtlCamState debugEndCamera; /* 0x130: captured debug camera's final pose. */
    u8 pad158[8];
    u32 runtimeFlags; /* 0x160 */
    u8 pad164[0x1C];
    s32 debugCameraProgress; /* 0x180 */
    u8 pad184[0x1C];
    f32 debugCameraParameter; /* 0x1A0 */
    u8 pad1A4[0x1C];
    s16 eventTaskId; /* 0x1C0: -1 when no event task is available */
    u8 pad1C2[2];
    u32 scriptFlags; /* 0x1C4 */
    u32 eventFlags; /* 0x1C8 */
    s16 eventActive; /* 0x1CC */
    u8 pad1CE[2];
    s32 eventAction; /* 0x1D0 */
    s32 eventResult; /* 0x1D4 */
    void *eventRequest; /* 0x1D8 */
    void *eventData; /* 0x1DC */
    BtlUnit *eventUnit; /* 0x1E0 */
    s32 sequenceHandle; /* 0x1E4 */
    s32 scriptHandle; /* 0x1E8 */
    void *eventAssets; /* 0x1EC */
    u8 pad1F0[4];
    u32 battleFlags; /* 0x1F4 */
    u32 commandRestrictFlags; /* 0x1F8: bit 0x10 blocks commands with the +0x30 restriction */
    u32 unk_1FC; /* Bit 0x800 bypasses command-block-reason checks. */
    u8 pad200[0x24];
    BtlTask *tasks; /* 0x224 */
    BtlUnit *units; /* 0x228 */
    u8 pad22C[0x14];
    struct BattleModelEntry *modelEntries; /* 0x240 */
    u16 cameraPresetMode; /* 0x244: selects the marked actor's camera preset. */
    u8 pad246[2];
    u16 turnPhase; /* 0x248 */
    u8 pad24A[2];
    u16 mode; /* 0x24C */
    u8 pad24E[2];
    s32 turnCount; /* 0x250 */
    u8 pad254[4];
    u8 eventReady; /* 0x258 */
    u8 pad259[3];
    u16 phase; /* 0x25C */
    u8 requestMode; /* 0x25E: script sets this to 4 with requestArgument */
    u8 pad25F[0x11];
    s32 encounterPack; /* 0x270: ENC PACK test selection (func_00215FF8) */
    s32 adjustmentGroupIndex; /* 0x274: encounter reward lookup in code_001A1960 */
    s32 adjustmentEntryIndex; /* 0x278: entry within that encounter group */
    s32 battleMode; /* 0x27C */
    s32 requestArgument; /* 0x280: sign-extended script halfword */
    u8 pad284[4];
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
    u8 pad48C[0x18];
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
    u8 pad598[0x20];
    s32 unk_5B8;
    u8 pad5BC[0x14];
    void (*cleanup)(void); /* 0x5D0 */
    u8 pad5D4[0x1C];
    void (*updateCallback)(void); /* 0x5F0 */
    u8 pad5F4[0x38];
    s32 (*actionCameraStepHook)(u8 *); /* 0x62C: nonzero handles the camera step. */
    u8 pad630[0x24];
    s32 (*hook654)(BtlUnit *);
    s32 (*hook658)(BtlUnit *);
    u8 pad65C[0x38];
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
#endif /* VERSION_DDS1 */

#ifdef VERSION_DDS2
struct ActionStateLink;
struct SceneTask;
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
    u8 pad000[0x50];
    f32 lightColor[4]; /* 0x50: scene light color used by battle light transitions. */
    f32 ambientColor[4]; /* 0x60: default ambient color used by battle light transitions. */
    u8 pad070[0x30];
    BtlCamState debugStartCamera; /* 0xA0: captured debug camera's initial pose. */
    u8 pad0C8[0x68];
    BtlCamState debugEndCamera; /* 0x130: captured debug camera's final pose. */
    u8 pad158[0x28];
    u32 runtimeFlags;
    void *activeSlot;
    u8 pad188[0xC];
    u32 activeUnitId;
    u8 pad198[8];
    s32 debugCameraProgress; /* 0x1A0 */
    u8 pad1A4[4];
    BtlIndexList *pendingSoundList;
    u8 pad1AC[0x18];
    f32 debugCameraParameter; /* 0x1C4 */
    u8 pad1C8[0x1C];
    s16 eventTaskId; /* 0x1E4 */
    u8 pad1E6[2];
    u32 scriptFlags; /* 0x1E8 */
    u32 eventFlags; /* 0x1EC */
    s16 eventActive; /* 0x1F0 */
    u8 pad1F2[2];
    s32 eventAction; /* 0x1F4 */
    s32 eventResult; /* 0x1F8 */
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
    u8 pad224[4];
    s32 unk228;
    s32 currentScene; /* 0x22C */
    s32 queuedScene;
    s32 frame;
    s32 sceneState;
    u16 unk23C;
    u16 unk23E;
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
    u16 unk268;
    u8 pad26A[4];
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
    u8 pad2A8[4];
    BtlBackgroundId background; /* 0x2AC */
    s32 loadStep;
    u8 pad2B4[0x10];
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
    struct SceneTask *groupPrimary[20]; /* 0x318 */
    struct SceneTask *groupSecondary[45]; /* 0x368 */
    struct SceneTask *groupTertiary[15]; /* 0x41C */
    struct SceneTask *groupHandles[8]; /* 0x458 */
    u16 groupHandleCount;
    u8 pad47A[2];
    s32 activeGroupCount;
    BtlSceneFadingRecord fading[8]; /* 0x480 */
    struct SceneTask *currentTask;
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
    s32 (*hook5D4)(BtlUnit *, s32, s32);
    s32 (*hook5D8)(BtlUnit *);
    s32 (*hook5DC)(BtlUnit *, s32);
    void (*actorParameterDeltaCallback)(BtlUnit *, s32 *); /* 0x5E0 */
    s32 (*sceneCallback)(); /* 0x5E4 */
    u8 pad5E8[8];
    s32 (*hook5F0)(BtlUnit *, s32);
    u8 pad5F4[4];
    BtlUnit *(*findModelActor)(s32, s32); /* 0x5F8 */
    u8 pad5FC[0x10];
    void (*beforeActorModelReady)(BtlUnit *); /* 0x60C */
    u8 pad610[4];
    void (*afterActorModelReady)(BtlUnit *); /* 0x614 */
    s32 (*hook618)(BtlUnit *);
    s32 (*hook61C)(BtlUnit *);
    u8 pad620[4];
    void (*afterUnitUpdate)(void); /* 0x624 */
    s32 (*selectScriptState)(void); /* 0x628 */
    void (*completionHook)(); /* 0x62C */
    s32 (*actionStateSelectionHook)(struct ActionStateLink *);
    u8 pad634[4];
    void (*commandTurnEndHook)(struct ActionStateLink *);
    s32 (*commandHook)(s32, s32);
    u8 pad640[8];
    s32 (*hook648)(BtlUnit *);
    s32 (*hook64C)(BtlUnit *);
    s32 (*hook650)(BtlUnit *);
    s32 (*hook654)(BtlUnit *);
    s32 (*hook658)(BtlUnit *);
    u8 pad65C[4];
    s32 (*hook660)(BtlUnit *, s32, s32);
    s32 (*actionCameraStepHook)(struct BtlLinkedCommand *); /* 0x664: nonzero handles the camera step. */
    s32 (*hook668)(BtlUnit *);
    s32 (*hook66C)(BtlUnit *);
    s32 (*hook670)(struct BtlLinkedCommand *);
    u8 pad674[0x10];
    s32 (*hook684)(s32, s32);
    s32 (*hook688)(s32, s32);
    void (*preActionHook)(struct ActionStateLink *, s32, u64, u64, u64);
    void (*postActionHook)(struct ActionStateLink *, s32, BtlUnit *, u64, u64, s32);
    u8 pad694[8];
    s32 (*hook69C)(BtlUnit *);
    s32 (*hook6A0)(BtlUnit *);
    u8 pad6A4[4];
    void (*linkedActionHook)(struct ActionStateLink *);
    u8 pad6AC[0x24];
    void (*actionResourceNameHook)(struct ActionStateLink *, s32, char *);
    u8 pad6D4[8];
    s32 (*commandRangeOverride)(BtlUnit *, s32); /* 0x6DC: func_001B0B30 calls the range override. */
    s32 (*hook6E0)(BtlUnit *);
    s32 (*hook6E4)(BtlUnit *);
    s32 (*hook6E8)(BtlUnit *);
    s32 (*scriptReturnHook)(); /* Optional script-return hook; preserve its unspecified retail prototype. */
    void (*hook6F0)(BtlUnit *, s32, s32, s32, s32, f32);
    void (*hook6F4)(BtlUnit *, s32, f32);
    void (*hook6F8)(BtlUnit *, s32, s32);
    s32 (*hook6FC)(BtlUnit *, s32, s32);
    void (*unitReturnHook)(struct SceneTask *); /* 0x700: custom return-to-group handling */
    u8 pad704[0xC];
    s32 (*hook710)(BtlUnit *, s32);
    void (*modelChangeSoundHook)(struct ActionStateLink *, u64, s32); /* 0x714: prerequisite handle, delay */
    struct BattleLinkedEffectState *effect; /* 0x718 */
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
#endif /* VERSION_DDS2 */

#endif /* BTL_STATE_H */
