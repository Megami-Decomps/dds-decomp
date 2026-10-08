#include "common.h"
#include "sdf_model.h"
#include "btl.h"
#include "btl_command.h"
#include "btl_state.h"
#include "btl_task_args.h"
#include "btl_sound.h"
#include "file.h"
#include "sdf.h"
#include "btl_action.h"
#include "dds3obj.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "mdl.h"
#include "sdf.h"

extern s32 mdlGetNodeField2C(MdlCtx *, s32);
extern void effObjSetOpacityPassEnabled(u32 enabled);

#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "dat_state.h"
#include "dat_command.h"
#include "file.h"

extern void btlClearAllUnitDefeatCandidates(void);
extern void func_001F3C30(BtlLinkedCommand *action);

extern void sdfReleaseChipBlock(void *block);
extern s32 btlIsUnitInActiveList(void *unit);

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
extern void mdlProcessContextNodesAndTransforms(MdlCtx *, s32);
extern void func_001E38F0(BtlUnit *, MdlCtx *, SdfModel *, SdfPoolNode **, u32);
extern void dds3ClearObjectFlags(s32, s32);
extern u8 D_00380788[];
extern SdfPoolNode *D_003B6BD0[];



extern void func_001EC868(void *, f32 *, f32);

typedef struct BtlUnit BtlUnit;

/* SYSEFF metadata and runtime registrations share these indices. */
enum {
    BTL_SOUND_ENTRY_COUNT = 0x31,
    BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26,
    BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT = 0x2C
};

/* Motion selection and approach tasks share these 0x14-byte resource nodes. */
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
    u8 pad24[6];
    u16 unk2A;
    BtlEffectNode nodes[1];
} BtlEffectResource;

extern void btlAdvanceHistoryCounter(ActionStateLink *);
extern void fldUpdateSceneGroupTask(BtlUnit *);
extern void btlUnitTurnEndStateSelect(BtlUnit *);

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

typedef struct BtlResourceTableEntry {
    u32 flags;
    u8 pad04[72];
} BtlResourceTableEntry;

typedef struct BtlStateHandler {
    void (*start)(void *);
    void (*update)(void *);
    void (*finish)(void *);
} BtlStateHandler;

extern BtlStateHandler D_003B69D8[];


typedef struct BtlFxSrcA {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
} BtlFxSrcA;


typedef struct FxTask {
    u8 pad0[0x10];
    s32 unk10;
    BtlUnit *unit;
} FxTask;

typedef struct SoundLink {
    BtlUnit *owner;
    struct SoundVoice *effectHandle;
    SoundResourceNode *effect;
    u16 flags;
} SoundLink;

typedef struct SoundResourceLink {
    BtlUnit *owner;
    struct SoundVoice *effectHandle;
    struct SoundResourceNode *effect;
    u32 flags;
    u8 refreshRequested;
    u8 pad11[3];
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

extern void evtSetUnitNormalizedDirection(EvtUnit *, s32);

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
    void *unit;         /* 0x18 */
} BtlActiveSlot;


extern struct BtlRuntimeTask *btlCreateHookedUnitSoundTask();

extern u32 D_00436AD4;

extern u32 dds3AdvanceWorldCounter(void);

extern struct EffWorldNode *evtSpawnActionObj9(s32);

extern s8 btlSetActorEffectParameter(BtlUnit *, s32);

extern s32 mdlFlagTest(u32);

extern s32 func_0022F180(void);

extern BtlRuntimeTask *fldCreateSceneGroupAction(ActionStateLink *, u32, s32);

extern s32 btlGetRuntime(void);

extern void func_001AA850(void *, s32);

extern void mdlStoreTertiaryVectorVU(MdlCtx *);

extern void mdlSetAmountOnAllContextResources(MdlCtx *, f32);

extern f32 func_001F5780(u32, u8, f32, f32);

extern f32 func_001FDD20(f32 *, f32, f32, s32);

extern struct BtlRuntimeTask *btlDeferredTaskTail;

extern struct BtlRuntimeTask *btlDeferredTaskHead;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 kwlnDrawControlFlags;

extern s32 mnuPollTitleStreamStateLocked(void);

extern void mnuResetTitleStreamLocked(void);

extern void func_002A2200(s32);

extern s32 D_00435E0C;


extern s32 sndFindPackedTrackLoadStatus(u32);


typedef struct SceneLightRestoreArgs { u32 value; } SceneLightRestoreArgs;
extern s64 func_00201520(SceneLightRestoreArgs *);
extern void evtSetUnitStatusFlags(EvtUnit *);
extern void func_0023C870(EvtUnit *, s32, u32, u32);

extern s64 func_00201718(void);

extern s32 sndPlaySkillSeTask(u32 *);


extern SoundResourceNode *sndAllocResourceNode(void);

extern void sndFormatResourceNameFromUnitMode(s32, s32);

extern s32 datActionAnimationRecords;

extern void *sdfAllocAndClearQuadwords(s32);

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern s32 btlCountTasksForOwner(s64);

extern void btlRunTask(BtlRuntimeTask *);

extern void btlBossDebugPrintf(const char *format, ...);

extern u16 mdlGetContextResourceGroup(MdlCtx *);

extern u16 mdlGetContextResourceId(MdlCtx *);

extern f32 func_00208000(u32, f32 *, f32 *);
extern s32 func_001E3230(BtlUnit *, s32);

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
    u32 unk_20;
    u32 unk_24;
} SoundTaskArgs;

extern BtlRuntimeTask *btlAllocTask(s32);


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

extern void btlBuildApproachCamera(BtlLinkedCommand *, BtlCamState *);
extern void btlUpdateActionTargetCameraPose(BtlLinkedCommand *);
extern void btlBuildGroupFramingCameraPose(BtlCamState *, BtlCamState *);
extern void func_001F3E48(s32);
extern void btlAdvanceCursorForUnmarkedUnit(s32, s32);

extern void func_001FA480(s32, s32, s32);

extern void func_001FBAC0(s32, s32);

extern s32 func_001FB908(s32, BtlCamState *, s8, s8);

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
    u8 pad15[0x11B];
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


struct FileQueue;
extern struct FileQueue *fileCloneQueueEntries(struct FileQueue *);


extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);
extern void btlUnitGetMuzzlePosVU(BtlUnit *);
extern u32 mdlGetBroadcastValue(MdlCtx *);
extern void sdfQueueNonzeroResourceId(s32);
extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);
extern u16 btlRefreshUnitMaximumHpAndClampCurrentHp(DatPartyRecord *);
extern u16 btlRefreshUnitMaximumMpAndClampCurrentMp(DatPartyRecord *);
extern void evtSetUnitRgbTransition(EvtUnit *, s32, u32);
extern void evtUnitSetStoredParameter(void *, s32);
extern void evtSetTransitionMotionScale(void *, f32);
extern s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *);
extern s32 btlTestActorStatusPredicate(BtlUnit *);
extern s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *, s32);
extern void btlApplyUnitModelScaledValue(u8 *);
extern s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *);
extern s32 btlGetSideIndexedActorStatusTable(s32, s32);
extern void btlApplyUnitMotionSelection(u8 *, u32, s32, f32);
extern s32 btlGetSlotRateKind(u8 *, s32);
extern BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *, f32);
extern void evtPrepareUnitMotionState(EvtUnit *, s32, s32, s32, s32);
extern void evtStoreUnitMotionShortParameters(EvtUnit *, s32, s32);
extern void sdfMotionSampleAtFrame(Motion *, f32);
extern void btlRefreshUnitMotionSelection(BtlUnit *);
extern void btlClearActorSelectedEntryIndex(BtlUnit *);
extern void btlClearAllActorEntrySlots(BtlUnit *);
extern void btlReleaseUnitResources(BtlUnit *);
extern void btlInitUnitFxDefaults(BtlUnit *);
extern void btlClearSceneTaskActiveFlag(ActionStateLink *);

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);
extern void func_001AA868(void *, s32);
extern void func_001ADFE0(BtlUnit *, u32, s16);
extern s32 btlCountTasksByKind(u16 kind);
extern void btlRepositionPartyAroundBattleCenter(void);
extern s32 func_001AC648(void);
extern void func_001F5868(s32, s32, s32, s32);
extern void func_001F5320(s32, s32, s32, s32);
extern SdfFlagListParams D_003BDCC8;
extern void mnuReleaseSoundBufferLocked(void);
extern void evtSetUnitAlphaTransition(EvtUnit *, s32, u32);
extern void func_002A27A8(s32, s32, u8);
extern void mdlBroadcastMasked(MdlCtx *, u32);
extern s32 fldReleaseIdleSceneActorResources(BtlUnit *);
extern void func_001AB160(BtlUnit *);
extern void btlRemoveTaskFromSceneGroup(ActionStateLink *);
extern s32 func_001DACF8();

extern void func_001DB048(void);

extern void btlCommandTaskStartEffects(ActionStateLink *task);
extern void btlCommandTaskReturnStart(ActionStateLink *);

extern void btlCommandTaskReturnUpdate(ActionStateLink *task);

extern void btlStartLinkedActorEffectTask(ActionStateLink *unit);
extern s32 func_001DB5E0();

extern void btlStartOwnerEffectTasks(s32 *arguments);
extern s32 func_001DBE70();

extern void btlRecordLinkedActorOutcome(ActionStateLink *unit);
extern s32 func_001DC2D8();

extern void func_001DC538(void);
extern s32 func_001DC540();

extern void btlSpawnSceneActionAndSwitchState(void);

extern void func_001DC7F8(u8 *unit);

extern void func_001DC838(void);

extern void btlAdvanceStateWhenLinkedTasksFinish(ActionStateLink *unit);

extern void btlAdvanceLinkedUnitWhenOwnerIdle(void);

extern void func_001DC890(ActionStateLink *unit);

extern void btlFinalizeLinkedActionAndAdvanceHistory(void);

extern void btlUnitTurnEndCommit(ActionStateLink *unit);
extern void btlStartActorDefeatTransition(ActionStateLink *);
extern void btlRemoveEligibleActorSceneTask(ActionStateLink *);

extern void func_001DCD80(void);

extern void btlCommandTaskReleaseActor(ActionStateLink *task);

extern void btlFlagLinkedActorActionInProgress(ActionStateLink *unit);

extern void btlRunHookAndAdvanceUnitState(ActionStateLink *unit);

extern void func_001DCEB0(void);

extern void func_001DCEB8(void);

extern void func_001DCEC0(void);

extern void btlAdvanceUnitWhenActionGateClears(u32 unit);

extern void btlDispatchStateHandler(void *obj, s32 kind);



extern void btlUpdateActionSeqs(void);

extern void btlDestroyAllActionSeqs(void);


extern s32 btlGetLoggedIndexedCommandItem(s32);
extern void scrSetGlobalBitFlag(u32);

/* A hook result of -1 leaves dispatch to the command-kind handler. */
void func_001DD390(u8 *command, u8 *argument) {
    s32 (*handler)(s32, s32) = ((BtlState *)btlGetRuntime())->commandHook;

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
        if (datCommandSelectors[*(s32 *)(argument + 4)].kind != 1) {
            btlDispatchStateHandler(command, 0xE);
        } else {
            if (*(u32 *)(*(s32 *)(command + 0x18) + 0x110) & 0x200) {
                scrSetGlobalBitFlag(*(u16 *)(argument + 4));
            }
            btlDispatchStateHandler(command, 0xF);
        }
        break;
    case 5: {
        u32 flags = *(u32 *)(*(s32 *)(command + 0x18) + 0x110);
        if (flags & 0x200) {
            if (flags & 0x1000) {
                btlDispatchStateHandler(command, 0x10);
            } else {
                btlDispatchStateHandler(command, 0x11);
            }
        } else {
            btlDispatchStateHandler(command, 0x13);
        }
        break;
    }
    case 10:
    case 13:
    case 14:
    case 18:
        btlDispatchStateHandler(command, 0x14);
        break;
    case 9:
        if (*(u32 *)(*(s32 *)(command + 0x34) + 0x110) & 1) {
            btlDispatchStateHandler(command, 0x17);
        } else {
            btlDispatchStateHandler(command, 0x16);
        }
        break;
    case 12:
        btlDispatchStateHandler(command, 0x16);
        break;
    case 6: {
        u32 flags = *(u32 *)(*(s32 *)(command + 0x18) + 0x110);
        if (flags & 0x200) {
            btlDispatchStateHandler(command, 0x18);
        } else if (flags & 0x400) {
            btlDispatchStateHandler(command, 0x15);
        }
        break;
    }
    case 11:
        btlDispatchStateHandler(command, 0x15);
        break;
    case 15:
        btlDispatchStateHandler(command, 0x19);
        break;
    case 16:
        btlDispatchStateHandler(command, 0x1A);
        break;
    case 17:
        btlDispatchStateHandler(command, 0x20);
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
    s32 value;
    switch (argument[0]) {
    case 1:
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200 && (unit->partyRecord.flags & 0x10) == 0) {
            return btlGetActorBedAssetIdFromIndex(unit->partyRecord.menuValue);
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
        if ((unit->flags & 0x200) && ((unit->flags & 0x1000) || (unit->partyRecord.flags & 0x10)) &&
            (unit->partyRecord.status & 0x1000) == 0 && count == 1) {
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
        if (index == 0xD6 && (btlUnitStatusPair(unit) & 0x1200) == 0x200 && (unit->partyRecord.flags & 0x10) == 0) {
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

    btlGetEntryFlagsUnlessDisabled(&actor->partyRecord);
    if (arg6 >= 0) {
        switch (datCommandRecords[arg6].unk30) {
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
    if ((datCommandRecords[arg6].attribute.bits & 0x400000FF) == 0x40000002) {
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
    if (arg5 != 0 && (btlUnitStatusPair(actor) & 0x4000000200) == 0x200) {
        code = 0xB;
    }
    if ((arg1 & 0x20001) == 0) {
        code = -1;
    }
    return code;
}


/* Test whether the operand is empty, subject to command-category and slot-kind exclusions. */
s32 btlActionEntryIsEmpty(s32 index, BtlOperandGroup *slot, BtlOperandEntry *entry) {
    s32 kind;

    if (index >= 0) {
        switch (index) {
        case 0x109:
        case 0x179:
        case 0x19C:
        case 0x1A5:
            return 0;
        }
        switch (datCommandRecords[index].unk30) {
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
    if (entry->hpDelta == 0) {
        if (entry->mpDelta == 0) {
            if (entry->addedStatus == 0) {
                if (entry->removedStatus == 0) {
                    if (entry->hpRecovery == 0) {
                        if (entry->mpRecovery == 0) {
                            if (entry->entryChangeMask == 0) {
                                if (entry->entrySelection == 0) {
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
s32 func_001DDAD0(s32 unused, BattleIndexWork *state) {
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

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DDB60);



/* Reset work status and group headers, preserving the retained payload and allocation. */
void btlResetIndexWork(BattleIndexWork *work) {
    u32 i;
    work->phase = -1;
    work->skillId = -1;
    work->reference = -1;
    work->companionA = 0;
    work->companionB = 0;
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
    work->unk64 = 0;
    work->unk60 = 0;
    for (i = 0; i < 13; i++) {
        work->groups[i].count = 0;
        work->groups[i].kind = 0;
        work->groups[i].reflected = 0;
        work->groups[i].inactive = 0;
    }
    btlClearIndexList(work->indices);
}


extern u32 sdfAllocGeneralBlock(s32);

extern void *sdfResourceRetainAddress(u32);

/* Allocate the index list and retained groups, then initialize their headers. */
void btlInitBattleIndexWork(BattleIndexWork *work) {
    u32 handle;
    work->indices = btlAllocateIndexList(13);
    handle = sdfAllocGeneralBlock(0x48EC);
    work->groups = (BtlOperandGroup *)sdfResourceRetainAddress(handle);
    work->allocationHandle = handle;
    work->ownerId = 0;
    btlResetIndexWork(work);
}

/* Release each owned buffer once. The cached group address is deliberately not cleared. */
void btlReleaseObjectBuffers(BattleIndexWork *object) {
    if (object->allocationHandle != 0) {
        sdfReleaseResourceAllocation(object->allocationHandle);
        object->allocationHandle = 0;
    }
    if (object->indices != 0) {
        btlFreeIndexList(object->indices);
        object->indices = 0;
    }
}

extern void btlAdjustUnitHp(DatPartyRecord *, s32);
extern void btlAdjustUnitMp(DatPartyRecord *, s32);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001DF860);

extern u32 func_001DF860(BtlOperandTaskArgs *);


BtlRuntimeTask *btlCreateActorParameterDeltaTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x49;
    task->ownerId = unit->owner;
    task->callback = func_001DF860;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlApplyDeferredActorStats(BtlOperandTaskArgs *arguments) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlUnit *actor = arguments->unit;
    s32 primary;
    DatPartyRecord *resource;
    if ((context->battleFlags & 0x80) == 0) {
        return 1;
    }
    primary = arguments->operand.hpRecovery;
    if (primary == 0 && arguments->operand.mpRecovery == 0) {
        return 1;
    }
    if (actor->flags & 0x60) {
        return 1;
    }
    resource = &actor->partyRecord;
    btlAdjustUnitHp(resource, primary);
    btlAdjustUnitMp(resource, arguments->operand.mpRecovery);
    btlRefreshUnitMotionSelection(actor);
    btlIsUnitDefeatTriggeredByValueDelta(actor, 0);
    return 1;
}

BtlRuntimeTask *btlCreateDeferredActorStatsTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x4A;
    task->ownerId = unit->owner;
    task->callback = btlApplyDeferredActorStats;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlApplyDeferredUnitStatus(void *arg) {
    s32 *args = arg;
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = (BtlUnit *)args[0];
    if (!(work->battleFlags & 0x80)) {
        return 1;
    }
    func_001AA850(&unit->partyRecord.flags, args[1]);
    btlRefreshUnitMotionSelection(unit);
    btlIsUnitDefeatTriggeredByValueDelta(unit, 0);
    return 1;
}

BtlRuntimeTask *btlCreateDeferredUnitStatusTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x4B;
    task->ownerId = actor->owner;
    task->callback = btlApplyDeferredUnitStatus;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = args->unit;
    if (!(work->battleFlags & 0x80)) {
        return 1;
    }
    if (datCommandRecords[args->category].flags & 8) {
        btlAdjustUnitHp(&unit->partyRecord, -0x7FFF);
        func_001AA850(&unit->partyRecord.flags, 0x4000);
        unit->flags |= 0x20;
    }
    if (args->amount == 0) {
        return 1;
    }
    switch (datCommandRecords[args->category].costMode) {
    case 1:
        btlAdjustUnitHp(&unit->partyRecord, -args->amount);
        return 1;
    case 2:
        btlAdjustUnitMp(&unit->partyRecord, -args->amount);
        return 1;
    default:
        return 1;
    }
}

BtlRuntimeTask *btlCreateCategoryStatDamageTask(BtlUnit *unit, u32 target, u32 option) {
    BtlRuntimeTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlApplyCategoryStatDamage;
    task->taskId = 0x4C;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->unk_08 = target;
    args->option = option;
    return task;
}

s32 func_001DFFE0(BtlOperandTaskArgs *args) {
    func_001ADFE0(args->unit, args->operand.entryChangeMask, args->operand.entryChange);
    return 1;
}

BtlRuntimeTask *btlCreateMaskedActorEntryUpdateTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x4D;
    task->ownerId = unit->owner;
    task->callback = func_001DFFE0;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlApplyQueuedActorEntrySelection(BtlOperandTaskArgs *taskArgs) {
    if (0 < taskArgs->operand.entrySelection) {
        btlSetActorSelectedEntryIndex(taskArgs->unit, taskArgs->operand.entrySelection);
        btlRefreshUnitMotionSelection(taskArgs->unit);
    }
    return 1;
}

BtlRuntimeTask *btlCreateQueuedActorEntrySelectionTask(BtlUnit *unit, BtlOperandEntry *block) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    BtlOperandTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x4E;
    task->ownerId = unit->owner;
    task->callback = btlApplyQueuedActorEntrySelection;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    memcpy(&args->operand, block, sizeof(*block));
    return task;
}

u32 btlClearQueuedActorEntrySelection(BtlUnit **taskArgs) {
    btlClearActorSelectedEntryIndex(*taskArgs);
    btlRefreshUnitMotionSelection(*taskArgs);
    return 1;
}

BtlRuntimeTask *func_001E0238(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    BtlUnit **args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlClearQueuedActorEntrySelection;
    task->taskId = 0x4F;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *args = unit;
    return task;
}

/* Complete eight-byte argument allocation owned by the hunt-EP task. */
typedef struct BtlHuntExpArgs {
    BtlUnit *actor;
    u32 amount;
} BtlHuntExpArgs;
typedef char BtlHuntExpArgsSizeCheck[sizeof(BtlHuntExpArgs) == 8 ? 1 : -1];
extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);

u32 func_001E02A8(s32 address) {
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
    if (btlCheckSpecialAbility(&args->actor->partyRecord, 0x26C)) {
        count = 0;
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2E4);
        record->huntExp += args->amount;
        head = battle->units;
        for (unit = head; unit != NULL; unit = unit->nextActor) {
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
        for (unit = head; unit != NULL; unit = unit->nextActor) {
            u32 flags = unit->flags;
            if (flags & 0x200) {
                if (flags & 1) {
                    if (args->actor != unit && !(flags & 0x20) && !(unit->partyRecord.status & 0x40)) {
                        record = btlGetIndexedPartyEntryRecord(unit->unk2E4);
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
        record = btlGetIndexedPartyEntryRecord(args->actor->unk2E4);
        record->huntExp += args->amount;
        btlBossDebugPrintf("btl:hunt ep=%d[%p]\n", args->amount, record);
    }
    return 1;
}


extern u32 func_001E02A8(s32);

BtlRuntimeTask *btlCreateActorSoundOptionTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    BtlHuntExpArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x50;
    task->ownerId = actor->owner;
    task->callback = func_001E02A8;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->amount = (u32)option;
    return task;
}

typedef struct {
    u32 unk0;
    s32 soundIndex;
} BattleVoiceWork;

extern u8 *datItemSkillRecords;

extern void ptyAdjustItemQuantity(s32, s32);

extern void btlSyncModelFlagFromEventThresholds(void);

s32 btlPlayPermittedBattleVoice(BattleVoiceWork *work) {
    s32 index = work->soundIndex;
    if (datItemSkillRecords[index * 8 + 1] & 4) {
        ptyAdjustItemQuantity(index, -1);
        switch (work->soundIndex) {
        case 0x53:
        case 0x54:
            btlSyncModelFlagFromEventThresholds();
            break;
        }
    }
    return 1;
}

BtlRuntimeTask *btlCreatePermittedBattleVoiceTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x51;
    task->ownerId = actor->owner;
    task->callback = btlPlayPermittedBattleVoice;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 btlPlayQueuedBattleVoice(s32 taskArgs) {
    ptyAdjustItemQuantity(*(u16 *)(taskArgs + 4), 1);
    return 1;
}

BtlRuntimeTask *btlCreateQueuedBattleVoiceTask(BtlUnit *actor, u16 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x52;
    task->ownerId = actor->owner;
    task->callback = btlPlayQueuedBattleVoice;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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
    BtlState *work = (BtlState *)btlGetRuntime();
    BattleRewardPacket *packet = (BattleRewardPacket *)packetAddress;
    if (packet->amount == 0) {
        return 1;
    }
    if (packet->actor->flags & 0x400) {
        return 1;
    }
    work->epEarned += packet->amount;
    btlBossDebugPrintf("btl:epall=%d[%d](packet)\n", work->epEarned, packet->amount);
    return 1;
}

extern u32 btlAddEpFromPacket(s32);

BtlRuntimeTask *btlScheduleEpPacketTask(BtlUnit *actor, s32 amount) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x53;
    task->ownerId = actor->owner;
    task->callback = btlAddEpFromPacket;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = amount;
    return task;
}

u32 btlAddMoneyFromPacket(s32 packetAddress) {
    BtlState *work = (BtlState *)btlGetRuntime();
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

BtlRuntimeTask *btlScheduleMoneyPacketTask(BtlUnit *actor, s32 amount) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x54;
    task->ownerId = actor->owner;
    task->callback = btlAddMoneyFromPacket;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = amount;
    return task;
}

extern s32 datEnemyRecords;

u32 btlRefreshEligibleActors(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = work->units;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 && (u16)(unit->partyRecord.unitId - 1) < 0x17F) {
                    u32 entry = ((BtlResourceTableEntry *)datEnemyRecords)[unit->partyRecord.unitId].flags;
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((unit->stateFlags & 8) == 0) {
                                u16 prior = unit->partyRecord.status;
                                func_001AA850(&unit->partyRecord.flags, 1);
                                btlRefreshUnitMotionSelection(unit);
                                if (unit->partyRecord.status == 1 && prior != unit->partyRecord.status) {
                                    unit->stateFlags |= 4;
                                    work->commandRestrictFlags |= 0x100;
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

BtlRuntimeTask *btlCreateRefreshEligibleActorsTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = btlRefreshEligibleActors;
    task->taskId = 0x55;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

u32 btlApplyQueuedCurrencyReward(s32 taskArgs) {
    datAddCurrencyClamped(*(u32 *)(taskArgs + 4));
    return 1;
}

BtlRuntimeTask *btlCreateCurrencyRewardTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x56;
    task->ownerId = actor->owner;
    task->callback = btlApplyQueuedCurrencyReward;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

void btlUpdateAutoMusic(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u32 flags = ((BtlState *)context)->battleFlags;
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if ((s8)D_0037F510[0x22] < 0 || (s8)D_0037F510[0x23] < 0) {
            ((BtlState *)context)->battleFlags = flags & ~0x8000;
            sndSetSequenceVolumePan(6, 0x7F, 0x3F);
            btlSetTrackedTaskDisplayMode(0);
            btlBossDebugPrintf(D_004178A8);
        }
    } else if ((s8)D_0037F510[0x22] < 0) {
        ((BtlState *)context)->battleFlags = flags | 0x8000;
        sndSetSequenceVolumePan(5, 0x7F, 0x3F);
        btlSetTrackedTaskDisplayMode(1);
        btlBossDebugPrintf(D_004178B8);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E0CE0);

void btlFindSoundTaskByWorkValue(void) {
}

/* Return the oldest matching handle, or zero; unstarted tasks may have handle 0. */
BtlRuntimeTask *btlFindTaskByHandle(u64 value) {
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->handle == value) {
            return task;
        }
    }
    return 0;
}

/* Return the oldest task with this owner, or zero. */
BtlRuntimeTask *btlFindTaskByOwner(u64 owner) {
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->ownerId == owner) {
            return task;
        }
    }
    return 0;
}

/* Return the oldest registered task of this kind, or zero. */
BtlRuntimeTask *btlFindTaskByKind(u16 kind) {
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->taskId == kind) {
            return task;
        }
    }
    return 0;
}

/* Count all registrations, including tasks awaiting startup or release. */
s32 btlCountRegisteredTasks(void) {
    s32 task;
    s32 count;

    task = btlGetRuntime();
    count = 0;
    for (task = (s32)((BtlState *)task)->taskHead; task != 0; task = (s32)((BtlRuntimeTask *)task)->next) {
        count = count + 1;
    }
    return count;
}

/* Count registrations with this full-width owner key. */
s32 btlCountTasksForOwner(s64 owner) {
    s32 count = 0;
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->ownerId == owner) {
            count++;
        }
    }
    return count;
}

/* Count registrations of the requested task kind. */
s32 btlCountTasksByKind(u16 kind) {
    s32 count = 0;
    BtlRuntimeTask *task;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = task->next) {
        if (task->taskId == kind) {
            count++;
        }
    }
    return count;
}

/* Walk newest first and request release for tasks carrying allocation bit 1. */
void btlFlagTasksForUpdate(void) {
    BtlRuntimeTask *task;
    BtlRuntimeTask *next;
    for (task = ((BtlState *)btlGetRuntime())->taskTail; task != 0; task = next) {
        u16 flags = task->flags;
        next = task->prev;
        if (flags & 1) {
            task->flags = flags | 4;
        }
    }
}


extern BtlRuntimeTask *btlFindTaskByHandle(u64);
extern BtlRuntimeTask *btlFindTaskByOwner(u64);
extern BtlRuntimeTask *btlFindTaskByKind(u16);

/* Return whether the predicate is satisfied by value or registered tasks.
 * Kinds 5/8 accept running (phase 2) or absent, not an existing finishing task. */
INCLUDE_RODATA(const s32, "game/code_001DD390", D_004178A8);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_004178B8);

s32 btlEvalTaskCondition(BtlTaskCondition *condition, s32 value) {
    s32 result = 0;
    BtlRuntimeTask *task;
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
        task = btlFindTaskByHandle(condition->value.handle);
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
        task = btlFindTaskByOwner(condition->value.owner);
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

extern void *sdfAllocAndClearQuadwords(s32);

/* Append a cleared task; positive size exposes argument bytes after the header. */
BtlRuntimeTask *btlAllocTask(s32 size) {
    BtlRuntimeTask *task = sdfAllocAndClearQuadwords(size + 0x70);
    BtlState *work;
    if (size > 0) {
        task->args = task + 1;
    } else {
        task->args = 0;
    }
    work = (BtlState *)btlGetRuntime();
    task->next = 0;
    if (work->taskTail != 0) {
        work->taskTail->next = task;
        task->prev = work->taskTail;
    } else {
        work->taskHead = task;
        task->prev = 0;
    }
    work->taskTail = task;
    task->flags |= 1;
    return task;
}

/* Return the argument address recorded by allocation (zero for no arguments). */
void *btlGetTaskArguments(void *task) {
    return ((BtlRuntimeTask *)task)->args;
}

extern void sdfReleaseChipBlock(void *);

/* Invoke the finish hook before unlinking, then release the task block. */
void btlFreeTask(BtlRuntimeTask *task) {
    BtlState *work;
    if (task->onFinish != 0) {
        task->onFinish((u32 *)task->args);
    }
    work = (BtlState *)btlGetRuntime();
    if (task->prev != 0) {
        task->prev->next = task->next;
    } else {
        work->taskHead = task->next;
    }
    if (task->next != 0) {
        task->next->prev = task->prev;
    } else {
        work->taskTail = task->prev;
    }
    sdfReleaseChipBlock(task);
}

/* Install a fresh handle/reset phase counters, invoke startup, then reread handle. */
u64 btlStartTask(taskObject)
    void *taskObject;
{
    BtlRuntimeTask *task = taskObject;
    task->handle = btlAdvanceRuntimeSequenceCounter();
    task->flags |= 8;
    task->pollCount = 0;
    task->runCount = 0;
    task->state = 0;
    task->deferNext = 0;
    task->deferPrev = 0;
    if (task->onStart != 0) {
        task->onStart((u32)task->args);
    }
    return task->handle;
}

void btlResetDeferredTaskQueue(void) {
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Advance a started task through wait/delay/update/release.
 * Fallthrough is intentional: zero delays permit all phases in one poll. */
void btlRunTask(BtlRuntimeTask *task) {
    u32 counter;
    if (!(task->flags & 8)) {
        return;
    }
    if (task->flags & 4) {
        btlFreeTask(task);
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
        } else if (task->callback(task->args) != 0) {
            task->state = 3;
        } else {
            task->runCount = task->runCount + 1;
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

/* Run ordinary registrations now; queue deferred registrations for the later pass. */
void btlSweepFinishedTasks(void) {
    BtlRuntimeTask *task;
    BtlRuntimeTask *next;
    for (task = ((BtlState *)btlGetRuntime())->taskHead; task != 0; task = next) {
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

/* Run deferred tasks, saving the next link before callbacks may free the task. */
void btlClearDeferredTasks(void) {
    BtlRuntimeTask *node = btlDeferredTaskHead;
    while (node != 0) {
        BtlRuntimeTask *next = node->deferNext;
        btlRunTask(node);
        node = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

/* Release newest first; cache the previous registration before its block is freed. */
void btlClearTaskLists(void) {
    BtlRuntimeTask *task;
    BtlRuntimeTask *next;
    for (task = ((BtlState *)btlGetRuntime())->taskTail; task != 0; task = next) {
        next = task->prev;
        btlFreeTask(task);
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

u32 func_001E1848(void) {
    return 1;
}

BtlRuntimeTask *btlCreateImmediateCompletionTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = func_001E1848;
    task->taskId = 0x6A;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

void btlDumpTaskQueue(void) {
    s32 context = btlGetRuntime();
    s32 node = (s32)((BtlState *)context)->taskHead;
    while (node != 0) {
        btlBossDebugPrintf("btl:packet[%d]\n", ((BtlRuntimeTask *)node)->taskId);
        node = (s32)((BtlRuntimeTask *)node)->next;
    }
    btlBossDebugPrintf("btl:packet head[%p]\n", *(void **)(context + 0x250));
    btlBossDebugPrintf("btl:packet tail[%p]\n", *(void **)(context + 0x254));
}

void btlInitUnitFxDefaults(BtlUnit *unit) {
    PCP_COPY_VECTOR(unit->bodyOffset, &D_003B6B80);
    unit->height = 220.0f;
    unit->reach = 80.0f;
    unit->unkC0 = 75.0f;
}

extern s128 D_003B6B90;

extern s128 D_003B6BA0;


void btlInitFxLights(BtlUnit *unit) {
    PCP_COPY_VECTOR(unit->position, &D_003B6B90);
    PCP_COPY_VECTOR(unit->rotation, &D_003B6BA0);
    unit->unk58 = 0.0f;
    unit->unk50 = 1.0f;
    unit->baseColor = 0x80808080;
    PCP_COPY_VECTOR(unit->currentPosition, &D_003B6B90);
    PCP_COPY_VECTOR(unit->orientation, &D_003B6BA0);
    unit->scale = 1.0f;
    unit->overlayColor = 0x80808080;
    unit->positionZOffset = 0.0f;
}

extern void *btlSelectSharedOrIndexedTransformParameters(s32, s32);

void btlInitializeEffectVectorsFromSourceRecords(BtlUnit *unit, s32 kind, s32 index) {
    BtlFxSrcA *alt = btlSelectSharedOrIndexedTransformParameters(kind, index);
    BtlEffectResource *base = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(kind, index);
    if (alt->fC == 0.0f) {
        PCP_COPY_VECTOR(unit->bodyOffset, base);
        unit->reach = base->f18;
        unit->height = base->f1C;
        unit->unkC0 = base->f20;
    } else {
        unit->bodyOffset[0] = alt->f0;
        unit->bodyOffset[1] = alt->f4;
        unit->bodyOffset[2] = alt->f8;
        unit->bodyOffset[3] = 0.0f;
        unit->reach = alt->f10;
        unit->height = alt->f14;
    }
    PCP_COPY_VECTOR(unit->muzzleOffset, base);
    unit->unkBC = base->f18;
    unit->unkB8 = base->f1C;
    unit->unkC0 = base->f20;
    unit->scale = base->f10;
    unit->unk50 = base->f10;
    unit->positionZOffset = base->f14;
    unit->unk58 = base->f14;
}

s32 btlHasMatchingModel(s32 effect, s32 model) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = work->units;
    while (unit != NULL) {
        if ((unit->flags & 2) != 0 &&
            unit->ext != NULL &&
            unit->unk328 != 0 &&
            mdlGetContextResourceGroup(unit->ext->owner) == effect &&
            mdlGetContextResourceId(unit->ext->owner) == model) {
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
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk69C;
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

void btlClearUnitDefeatCandidate(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk6A0;
    if (hook == 0 || hook(unit) != 0) {
        unit->flags &= ~4;
        unit->flags &= ~8;
        if (unit->flags & 2) {
            unit->ext->owner->flags |= 1;
        }
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

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417940);

void btlApplyUnitMotionSelection(u8 *object, u32 index, s32 mode, f32 rate) {
    BtlUnit *unit = (BtlUnit *)object;
    BtlState *work;
    BtlEffectResource *table;
    BtlRuntimeTask *task;
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
    s32 (*chooseMode)(BtlUnit *, s32, s32);

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
                task->startDelay = 1;
                task->ownerId = 0;
                btlStartTask(task);
                break;
            }
        }
        return;
    }
    work = (BtlState *)btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, color);
        evtSetUnitAlphaTransition(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind,
                                                                unit->resourceIndex);
    if (table->nodes[index].rateKind == 2) {
        unit->updateFlags |= 6;
    }
    chooseMotion = work->chooseMotion;
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
            mode = btlGetSlotRateKind(object, index);
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
            start = unit->unkF8;
            end = unit->unkFA;
            break;
        case 1: case 18:
            start = 0;
            end = 1;
            break;
        case 3: case 4: case 5: case 6: case 7: case 8:
        case 12: case 16: case 17: case 19: case 20: case 21:
        case 22: case 23: case 24:
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
    keepRange = work->unk5DC;
    if (keepRange != 0 && keepRange(unit, index) != 0) {
        start = unit->unkF8;
        end = unit->unkFA;
    }
    if (mode & 0x100) {
        end = 8;
        mode &= ~0x100;
    }
    chooseMode = work->unk6FC;
    if (chooseMode != 0) {
        mode = chooseMode(unit, index, mode);
    }
    unit->fF4 = rate;
    unit->unkEC = index;
    unit->effectState = mode;
    rate = rate * (30.0f / work->unk4C4);
    rate *= work->unk4C8;
    if (work->unk6F0 != 0) {
        work->unk6F0(unit, index, start, end, mode, rate);
    } else {
        evtPrepareUnitMotionState(unit->ext, index, start, end, mode);
        model = unit->ext->owner;
        model->first->frameStep = rate;
        if (end == 0) {
            mdlAddEntryFlagged(model, 0, index);
            sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
        }
    }
    unit->unkF8 = 0;
    frameCount = table->nodes[index].frameCount;
    unit->unkFA = frameCount;
    if (mode != 0 && mode != 3) {
        return;
    }
    if (work->unk6F8 != 0) {
        work->unk6F8(unit, 0, (s16)frameCount);
    } else {
        evtStoreUnitMotionShortParameters(unit->ext, 0, (s16)frameCount);
    }
    unit->unkF8 = 0;
    unit->unkFA = table->nodes[unit->effectIndex].frameCount;
}

void btlRefreshUnitMotionSelection(BtlUnit *unit) {
    s32 entryFlags;
    BtlState *work;
    s32 index;
    s32 selected;
    s32 mode;
    BtlEffectResource *resource;
    f32 rate;
    f32 speed;
    u32 color;
    s32 (*chooseStatus)(BtlUnit *);
    s32 (*chooseMotion)(BtlUnit *, s32, s32);
    void (*setMotion)(BtlUnit *, s32, f32);
    s32 (*chooseMode)(BtlUnit *, s32, s32);

    if ((unit->flags & 2) == 0) {
        return;
    }
    entryFlags = btlGetEntryFlagsUnlessDisabled(&unit->partyRecord);
    work = (BtlState *)btlGetRuntime();
    if (unit->updateFlags & 2) {
        color = (unit->overlayColor & 0xFFFFFF) | 0x80000000;
        evtSetUnitRgbTransition(unit->ext, 0, color);
        evtSetUnitAlphaTransition(unit->ext, 0, color);
        unit->overlayColor = color;
        unit->updateFlags &= ~4;
        unit->updateFlags &= ~2;
    }
    index = 0;
    if (btlIsCurrentValueBelowQuarterThreshold(unit) != 0 &&
        ((unit->flags & 0x200) || (entryFlags & 0x200))) {
        index = 10;
    }
    if (unit->selectedEntryIndex > 0 &&
        ((unit->flags & 0x200) || (entryFlags & 0x200))) {
        index = 9;
    }
    switch (unit->partyRecord.status & 0x7FFF) {
    case 1: case 8: case 0x10: case 0x20: case 0x40:
    case 0x80: case 0x100: case 0x200: case 0x400: case 0x2000:
        index = 2;
        break;
    }
    if (btlTestActorStatusPredicate(unit) != 0) {
        if ((unit->flags & 0x2000) == 0) {
            unit->flags |= 0x80002000;
        }
    } else if (unit->flags & 0x2000) {
        btlApplyUnitModelScaledValue((u8 *)unit);
        unit->flags &= 0x7FFFFFFF;
        unit->flags &= ~0x2000;
    }
    chooseStatus = work->unk5D8;
    if (chooseStatus != 0) {
        selected = chooseStatus(unit);
        if (selected >= 0) {
            index = selected;
        }
    }
    if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0) != 0 &&
        ((unit->flags & 0x200) || (entryFlags & 0x200)) &&
        ((unit->stateFlags & 0x40) == 0)) {
        index = 11;
        btlApplyUnitModelScaledValue((u8 *)unit);
        unit->flags &= 0x7FFFFFFF;
        unit->flags &= ~0x2000;
    }
    resource = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(
        unit->resourceKind, unit->resourceIndex);
    rate = resource->nodes[index].scale;
    chooseMotion = work->chooseMotion;
    if (chooseMotion != 0) {
        selected = chooseMotion(unit, index, 1);
        if (selected == -1) {
            return;
        }
        if (index != selected) {
            index = selected;
            rate = resource->nodes[index].scale;
        }
    }
    unit->effectScale = rate;
    speed = rate * (30.0f / work->unk4C4);
    speed *= work->unk4C8;
    setMotion = work->unk6F4;
    unit->effectIndex = index;
    if (setMotion != 0) {
        setMotion(unit, index, speed);
    } else {
        evtUnitSetStoredParameter(unit->ext, index);
        evtSetTransitionMotionScale(unit->ext, speed);
    }
    chooseMode = work->unk6FC;
    mode = index != 11 ? 1 : 2;
    if (chooseMode != 0) {
        mode = chooseMode(unit, index, mode);
    }
    unit->effectParameter = mode;
    if (unit->unkEC != 11 &&
        (btlIsActorModeAcceptedByBattleHook(unit) != 0 || index == 11) &&
        unit->unkEC != index) {
        btlApplyUnitMotionSelection((u8 *)unit, index, mode, rate);
    }
}

s32 btlIsActorModeAcceptedByBattleHook(BtlUnit *unit) {
    BtlState *work;
    if (!(unit->flags & 2)) {
        return 0;
    }
    work = (BtlState *)btlGetRuntime();
    if (work->unk5D8 != 0 && work->unk5D8(0) == unit->unkEC) {
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

extern void btlApplyUnitMotionSelection(u8 *, u32, s32, f32);


void btlApplyScaledUnitEffectParameter(u8 *unit, s32 index, s32 option, f32 scale) {
    u8 *table = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)unit)->resourceKind, ((BtlUnit *)unit)->resourceIndex);
    btlApplyUnitMotionSelection(unit, index, option, ((BtlEffectResource *)table)->nodes[index].scale * scale);
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
            MdlCtx *model = ((BtlUnit *)object)->ext->owner;
            s32 node = mdlGetNodeField2C(model, 0);
            if (((BtlEffectResource *)resource)->nodes[node].rateKind == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                btlRefreshUnitMotionSelection(object);
                btlApplyUnitMotionSelection(object, ((BtlUnit *)object)->effectIndex,
                              ((BtlUnit *)object)->effectParameter,
                              ((BtlUnit *)object)->effectScale);
            }
        }
        object = *(u8 **)(object + 0x364);
    }
}

void btlApplyUnitModelScaledValue(u8 *object) {
    s32 context;
    f32 volume;
    if ((((BtlUnit *)object)->flags & 2) == 0) {
        return;
    }
    context = btlGetRuntime();
    ((BtlUnit *)object)->updateFlags &= ~1;
    volume = ((BtlUnit *)object)->fF4;
    ((BtlUnit *)object)->ext->owner->first->frameStep =
        volume * (30.0f / (f32)((BtlState *)context)->unk4C4);
}

void btlResetUnitModelProgress(BtlUnit *unit) {
    if (unit->flags & 2) {
        unit->updateFlags |= 1;
        unit->ext->owner->first->frameStep = 0.0f;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E2E58);

f32 btlGetUnitModelValue1C(BtlUnit *unit) {
    f32 value = 0.0f;
    if (unit->flags & 2) {
        value = unit->ext->owner->first->currentFrame;
    }
    return value;
}

void btlAdvanceUnitModelFrame(BtlUnit *unit, f32 frame) {
    if ((unit->flags & 2) != 0) {
        sdfMotionSampleAtFrame(unit->ext->owner->first, frame);
        return;
    }
}

s32 btlGetUnitModelFrameCount(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return 0;
    }
    return unit->ext->owner->first->frameCount;
}


void btlSeekUnitModelFrameZero(BtlUnit *unit) {
    if (unit->flags & 2) {
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
    }
}

extern u32 effMiscRandMod(void *state, u32 modulus);

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

s32 btlIsUnitModelStateFive(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return 1;
    }
    if (unit->effectState != 2) {
        return 1;
    }
    return unit->ext->owner->first->state == 5;
}

extern void effObjSetInnerFirstVec(s32, f32 *);

void btlSetUnitPosition(BtlUnit *unit, f32 *vec) {
    f32 pos[4];
    if (!(unit->stateFlags & 0x80)) {
        u8 *work = (u8 *)btlGetRuntime();
        VU0_LOAD_VF(vf10, vec);
        VU0_STORE_VF_UNCLOBBERED(vf10, unit->currentPosition);
        VU0_LOAD_VF(vf11, work);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        if (unit->flags & 2) {
            pos[2] += unit->positionZOffset;
            effObjSetInnerFirstVec(unit->effectObject, pos);
        }
    }
}

void func_001E3108(void *object, f32 *dst) {
    PCP_COPY_VECTOR(dst, ((BtlUnit *)object)->currentPosition);
}

void btlGetUnitWorldPos(BtlUnit *unit, f32 *dst) {
    u8 *work = (u8 *)btlGetRuntime();
    VU0_LOAD_VF(vf10, unit->currentPosition);
    VU0_LOAD_VF(vf11, work);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, dst);
}

typedef struct SdfTextParam SdfTextParam;
extern s32 sdfLoadMapRecordPositionVector(SdfTextParam *, s32);
extern void mdlLoadPrimaryVectorVU(MdlCtx *);
extern void mdlLoadSecondaryVectorVU(MdlCtx *);
extern void mdlStorePrimaryVectorVU(MdlCtx *);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *);
extern void sdfModelUpdateCurrentFrameTransforms(SdfModel *);

extern void btlRefreshUnitFxVectors(BtlUnit *);

s8 btlSetActorEffectParameter(BtlUnit *unit, s32 mode) {
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->flags & 2)) {
        return 0;
    }
    hook = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlRefreshUnitFxVectors(unit);
    return sdfLoadMapRecordPositionVector((SdfTextParam *)unit->ext->owner->inner, mode);
}

void btlSetActorEffectParameterOrMuzzlePosition(BtlUnit *unit, s32 mode) {
    if (btlSetActorEffectParameter(unit, mode) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    }
}

/* vu0 routine: preserve the actor's primary and secondary vectors while
 * evaluating the requested model record; return the sampled vector in vf10. */
s32 func_001E3230(BtlUnit *unit, s32 value) {
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

s8 btlSetActorAlternateEffectParameter(unit, mode)
    BtlUnit *unit;
    s32 mode;
{
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->flags & 2)) {
        return 0;
    }
    hook = ((BtlState *)btlGetRuntime())->effectParameterCallback;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlRefreshUnitFxVectors(unit);
    return sdfLoadMapRecordLookAtBasis((SdfTextParam *)unit->ext->owner->inner, mode);
}

void btlSetAlternateEffectParameterOrMuzzlePosition(void) {
    if (btlSetActorAlternateEffectParameter() == 0) {
        VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
    }
}

s32 btlIsUnitAtStoredPosition(u8 *object) {
    f32 position[4];
    func_001E3108(object, position);
    if (((BtlUnit *)object)->position[0] == position[0] &&
        ((BtlUnit *)object)->position[1] == position[1] &&
        ((BtlUnit *)object)->position[2] == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_004179E0[];

extern void effMiscQuatMultiplyVU(void);

extern void effObjSetInnerSecondVec(EffWorldNode *, void *);

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

extern void evtSetUnitRgbTransition(EvtUnit *, s32, u32);

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

extern void mdlReleaseInnerResourceHandle(MdlCtx *, s32, f32);

/* Forward the packed model-color word and its scalar to the inner resource list. */
void btlReleaseUnitModelColorResource(BtlUnit *unit, u32 value, f32 scalar) {
    mdlReleaseInnerResourceHandle(unit->ext->owner, (value & 0xFFFFFF) | 0x80000000, scalar);
}

extern void effObjFetchInnerFirstVec(s32);

extern void effObjFetchInnerSecondVecNorm(s32);


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

extern s32 btlAimHorizontalDirectionClampedVU(s128 *, s128 *, f32);

void btlUnitFaceTargetScaled(BtlUnit *unit, BtlUnit *target, f32 scale) {
    s128 from;
    s128 to;
    s128 hit;
    if (unit->flags & 0x80000) {
        btlUnitGetBodyPosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, &from);
        btlUnitGetBodyPosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, &to);
        btlAimHorizontalDirectionClampedVU(&from, &to, scale);
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

s32 btlClassifySpecialEntryObject(BtlUnit *entry) {
    if (!(entry->flags & 0x400)) {
        return 0;
    }
    switch (entry->partyRecord.unitId) {
    case 0x109: case 0x10A: case 0x110: case 0x111: case 0x112:
    case 0x119: case 0x11D: case 0x11E: case 0x11F: case 0x120:
    case 0x121: case 0x127: case 0x12E: case 0x12F: case 0x131:
    case 0x132: case 0x133: case 0x134: case 0x135: case 0x136:
        return 2;
    default:
        return (btlGetEntryFlagsUnlessDisabled(&entry->partyRecord) >> 14) & 1;
    }
}

void btlCopyUnitStats(BtlUnit *unit, DatPartyRecord *source) {
    DatPartyRecord *stats = &unit->partyRecord;
    *stats = *source;
    btlRefreshUnitMaximumHpAndClampCurrentHp(stats);
    btlRefreshUnitMaximumMpAndClampCurrentMp(stats);
}

extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void func_003325F8(SdfModel *, SdfModel *);
extern void func_003320E8(SdfPoolNode **, SdfModel *);
extern void mdlSetAllResourceFrames(MdlCtx *, u32);
extern void mdlDispatchViewerAnchorRecord(MdlCtx *, MdlResourceItem *);
extern u64 D_003B6BB0[4];

void func_001E38F0(BtlUnit *unit, MdlCtx *model, SdfModel *overlay,
                   SdfPoolNode **surfaces, u32 frame) {
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
        sdfAppendPacket(list, (u32)packet);
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
        sdfAppendPacket(list, (u32)packet);
        surfaces[i]->append((SdfListHead *)surfaces[i], list);
    }
    func_003325F8(overlay, model->inner);
    if (unit->flags & 2) {
        overlay->lighting = unit->ext->endpointWork;
    } else {
        overlay->lighting = NULL;
    }
    func_003320E8(surfaces, overlay);
    for (i = 0; i != 4; i++) {
        list = (SdfListHead *)sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        packet = (u64 *)sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = D_003B6BB0[i];
        packet[5] = 0x47;
        sdfAppendPacket(list, (u32)packet);
        surfaces[i]->append((SdfListHead *)surfaces[i], list);
    }
    mdlSetAllResourceFrames(model, frame);
    for (item = model->resourceItems; item != NULL; item = item->next) {
        mdlDispatchViewerAnchorRecord(model, item);
    }
}


extern void dds3SetObjectFlags(s32, s32);

void btlCreateUnitTransparency(BtlUnit *unit) {
    BattleGroupNode *shape;
    if ((unit->flags & 2) == 0) {
        return;
    }
    if (unit->unk344 != 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    shape = unit->ext->owner->sub;
    unit->unk344 = (s32)sdfModelCreateWithItems(shape->resourceList, shape->itemList);
    dds3SetObjectFlags(unit->effectObject, 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

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
                if (unit->unk344 != 0) {
                    sdfReleaseDevSlot(unit->unk344, 1, 1);
                    unit->unk344 = 0;
                    if (unit->flags & 2) {
                        info->inner->lighting = unit->ext->endpointWork;
                    } else {
                        info->inner->lighting = 0;
                    }
                    mdlBroadcastMasked(info, color);
                    mdlProcessContextNodesAndTransforms(info, (s32)D_00380788);
                    dds3ClearObjectFlags(unit->effectObject, 1);
                    btlBossDebugPrintf(D_00417940, unit);
                }
            } else if (alpha == 0) {
                dds3SetObjectFlags(unit->effectObject, 1);
            } else if (unit->unk344 == 0) {
                btlCreateUnitTransparency(unit);
            } else {
                func_001E38F0(unit, info, (SdfModel *)unit->unk344, D_003B6BD0, color);
            }
        }
    }
}

extern SdfGraphObj D_0040B290;
extern SdfPoolNode *D_003B6BE0[];
extern SdfPoolNode *D_003B6BF0[];
extern s32 sdfAllocPacketAligned(s32);
extern s32 sdfAllocatePacketList(s32 (*)(s32));
extern void sdfCreateResourcePacket(SdfListHead *, s32, s32, s32, s32, s32, s32, s32, s32, s32 (*)(s32));
extern void sdfCreateDescriptorPacket(SdfListHead *, s32, s32, s32, s32, s32, s32, s32 (*)(s32));

void func_001E3E20(BtlUnit *unit) {
    MdlCtx *info;
    s32 packet;

    if ((unit->flags & 2) == 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    unit->mirror->unk34C = sdfAllocPacketAligned(0x70000);
    packet = sdfAllocatePacketList(0);
    sdfCreateResourcePacket((SdfListHead *)packet, (s32)D_0040B290.buffers[2], 0, 0, 0x200, 0xE0, unit->mirror->unk34C, 0, 0, 0);
    D_003B6BE0[0]->append((SdfListHead *)D_003B6BE0[0], (SdfListHead *)packet);
    info = unit->ext->owner;
    if (unit->unk344 == 0) {
        unit->unk344 = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->effectObject, 1);
        return;
    }
    func_001E38F0(unit, info, (SdfModel *)unit->unk344, D_003B6BE0, unit->overlayColor);
    info = unit->mirror->ext->owner;
    if (unit->mirror->unk344 == 0) {
        unit->mirror->unk344 = (s32)sdfModelCreateWithItems(info->sub->resourceList, info->sub->itemList);
        dds3SetObjectFlags(unit->mirror->effectObject, 1);
        return;
    }
    packet = sdfAllocatePacketList(0);
    sdfCreateDescriptorPacket((SdfListHead *)packet, (s32)D_0040B290.buffers[2], 0, 0, 0x200, 0xE0, unit->mirror->unk34C, 0);
    D_003B6BF0[0]->append((SdfListHead *)D_003B6BF0[0], (SdfListHead *)packet);
    func_001E38F0(unit->mirror, info, (SdfModel *)unit->mirror->unk344, D_003B6BF0, unit->mirror->overlayColor);
}

extern char D_00436A28[];

s32 btlFormatUnitBedName(BtlUnit *unit, char *name) {
    btlGetRuntime();
    if (unit->flags & 0x200) {
        if (unit->partyRecord.flags & 0x10) {
            func_0035C860(name, "%s%03X_%02X.BED", D_00436A28, 0, unit->partyRecord.unitId + 0x20);
        } else if (unit->flags & 0x1000) {
            func_0035C860(name, "%s%03X_%02X.BED", D_00436A28, 0, unit->partyRecord.unitId);
        } else {
            func_0035C860(name, "%s%03X_%02X.BED", D_00436A28, btlGetActorBedAssetIdFromIndex(unit->partyRecord.menuValue), unit->partyRecord.unitId);
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E40F0);

void btlRefreshUnitEffectMotionAndEntry(BtlUnit *unit) {
    BtlState *work;
    if (!(unit->flags & 2)) {
        return;
    }
    work = (BtlState *)btlGetRuntime();
    if (unit->unkEC != unit->effectIndex) {
        btlRefreshUnitMotionSelection(unit);
        unit->unkF8 = 0;
        unit->unkFA = 0;
        btlApplyUnitMotionSelection((u8 *)unit, unit->effectIndex, unit->effectParameter, unit->effectScale);
    }
    if (unit->flags & 0x2000) {
        return;
    }
    if (work->unk6F0 != 0) {
        if (unit->effectIndex != 0xB) {
            work->unk6F0(unit, unit->effectIndex, 0, 0, 1, 1.0f);
        } else {
            work->unk6F0(unit, unit->effectIndex, 0, 0, 2, 1.0f);
        }
    } else {
        if (unit->effectIndex != 0xB) {
            mdlAddEntryFlagged(unit->ext->owner, 0, unit->effectIndex);
        } else {
            mdlAddEntryPlain(unit->ext->owner, 0, unit->effectIndex);
        }
        sdfMotionSampleAtFrame(unit->ext->owner->first, 0.0f);
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

BtlRuntimeTask *btlAllocateIndexedUnitEffectTask(BtlUnit *unit, s32 index, s32 value, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 9;
    task->callback = btlApplyIndexedUnitEffectTask;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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

BtlRuntimeTask *btlCreateScaledUnitModelTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlApplyScaledUnitModelTask;
    task->taskId = 0xA;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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

BtlRuntimeTask *btlScheduleThresholdTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0xB;
    task->ownerId = actor->owner;
    task->callback = btlPollThresholdTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

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
        BtlEffectResource *table = (BtlEffectResource *)btlGetSideIndexedActorStatusTable(unit->resourceKind, unit->resourceIndex);
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

BtlRuntimeTask *btlAllocateApproachTargetTask(BtlUnit *unit, s32 index, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(24);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlApproachTargetTask;
    task->taskId = 0xE;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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
        btlSetUnitPosition(unit, (f32 *)&pos);
    } else {
        btlSetUnitPosition(unit, (f32 *)&args->to);
        return 1;
    }
    args->count++;
    return 0;
}

BtlRuntimeTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, f32 *target, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(0x30);
    u8 *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0xC;
    task->ownerId = unit->owner;
    task->callback = btlUpdateUnitPositionInterpolationTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    ((BtlVectorTaskArgs *)args)->scale = scale;
    ((BtlVectorTaskArgs *)args)->unit2C = (u32)unit;
    ((BtlVectorTaskArgs *)args)->state24 = 0;
    ((BtlVectorTaskArgs *)args)->state28 = 0;
    PCP_COPY_VECTOR(args, unit->currentPosition);
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

BtlRuntimeTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, f32 *target, s8 mode, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(0x34);
    u8 *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0xD;
    task->ownerId = unit->owner;
    task->callback = btlStepUnitRotationNlerp;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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
            evtSetUnitAlphaTransition(((BtlUnit *)object)->ext, 0, 0);
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
        evtSetUnitAlphaTransition(((BtlUnit *)object)->ext, 0, 0);
        ((BtlUnit *)object)->overlayColor = ((BtlUnit *)object)->baseColor & 0xFFFFFF;
    }
    ((BtlUnit *)object)->gunResourceFlags = (((BtlUnit *)object)->gunResourceFlags & ~1) | 2;
    return 1;
}

BtlRuntimeTask *btlCreateModelLoadPollTask(BtlUnit *unit, u32 index, u32 value, s8 mode) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x18;
    task->flags &= ~1;
    task->ownerId = unit->owner;
    task->onStart = btlRequestModelOrReuse;
    task->callback = btlPollModelLoadCompletion;
    args = btlGetTaskArguments(task);
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

BtlRuntimeTask *btlScheduleRefreshTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlReleaseUnitModelTask;
    task->taskId = 0x19;
    task->ownerId = unit->owner;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

/* Task-start callbacks do not return a status to the scheduler. */
/* Exact 0x1C argument allocation owned by the model-change task creator. */
typedef struct BtlModelChangeArgs {
    BtlUnit *unit;
    u32 resourceKind;
    u32 resourceId;
    s32 delay;
    u32 duration;
    s32 elapsed;
    u8 phase;
    u8 transitionMode;
    u8 pad1A[2];
} BtlModelChangeArgs;
typedef char BtlModelChangeArgsSizeCheck[sizeof(BtlModelChangeArgs) == 0x1C ? 1 : -1];
typedef char BtlModelChangeArgsElapsedOffsetCheck[((u32)&((BtlModelChangeArgs *)0)->elapsed == 0x14) ? 1 : -1];
typedef char BtlModelChangeArgsPhaseOffsetCheck[((u32)&((BtlModelChangeArgs *)0)->phase == 0x18) ? 1 : -1];

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417AF0);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417B10);

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00417B30);

void btlBeginModelChange(u32 argumentsAddress) {
    BtlModelChangeArgs *arguments = (BtlModelChangeArgs *)argumentsAddress;
    BtlUnit *unit = arguments->unit;
    u32 model = arguments->resourceKind;
    u32 variant = arguments->resourceId;
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        btlRequestModelAssetByMode((u32)unit, model, variant);
        unit->gunResourceFlags = (unit->gunResourceFlags | 1) & ~2;
        btlBossDebugPrintf("btl:model change start[%X,%X]\n", model, variant);
    }
}

extern void func_001E1B80(BtlUnit *, BtlUnit *);
extern BtlUnit *btlCreateUnit(void);
extern void btlDestroyUnit(BtlUnit *);

/* Complete model loading, cross-fade the retained actor, and release it. */
u32 func_001E50E0(BtlModelChangeArgs *args) {
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
            func_001E1B80(unit->mirror, unit);
            if (unit->unkCC == 0) {
                unit->mirror->unkCC = 0;
                unit->unkCC = 1;
            }
            btlSetUnitPosition(unit->mirror, unit->currentPosition);
            btlSetUnitRotation(unit->mirror, (s128 *)unit->orientation);
            if (!(unit->stateFlags & 0x100000)) {
                btlSetUnitColor(unit->mirror, unit->baseColor, 0);
            } else {
                color = mdlGetBroadcastValue(unit->mirror->ext->owner);
                btlSetUnitColor(unit->mirror, (color & 0xFFFFFF) | 0x80000000, 0);
            }
            unit->mirror->flags |= 8;
            args->phase = 1;
            args->elapsed = 0;
        } else {
            args->phase = 2;
        }
        btlReleaseActorModelResources(unit);
        btlRefreshUnitMaximumHpAndClampCurrentHp(&unit->partyRecord);
        btlRefreshUnitMaximumMpAndClampCurrentMp(&unit->partyRecord);
        func_001E1BB8((u8 *)unit, resourceKind, resourceId);
        btlReleaseModelAssetByMode((u32)unit, resourceKind, resourceId);
        if (args->duration == 0) {
            kwlnDrawControlFlags |= 0x2000000;
        }
        btlSetUnitPosition(unit, unit->currentPosition);
        btlSetUnitRotation(unit, (s128 *)unit->orientation);
        btlSetUnitColor(unit, unit->baseColor, 0);
        if (unit->stateFlags & 0x10) {
            u32 firstColor;
            u32 secondColor;

            VU0_LOAD_VF(vf10, unit->colorStart);
            EE_MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
            firstColor = packedStart[0];
            VU0_LOAD_VF(vf10, unit->colorEnd);
            EE_MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
            secondColor = packedEnd[0];
            func_0023C870(unit->ext, 0, firstColor, secondColor);
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
                       args->resourceId != 0x1F) {
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
                if (!(unit->stateFlags & 0x100000)) {
                    unit->mirror->overlayColor = ((128 - alpha) << 24) | color;
                } else {
                    unit->mirror->overlayColor = ((128 - alpha) << 24) |
                        (mdlGetBroadcastValue(unit->mirror->ext->owner) & 0xFFFFFF);
                }
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
            btlDestroyUnit(unit->mirror);
            unit->mirror = NULL;
            if (unit->unk350 != 0) {
                sdfQueueNonzeroResourceId(unit->unk350);
                unit->unk350 = 0;
                unit->unk34C = 0;
            }
        }
        btlBossDebugPrintf("btl:model change end[%X,%X]\n", resourceKind, resourceId);
        return 1;
    }
    args->elapsed++;
    return 0;
}

extern u32 func_001E50E0(BtlModelChangeArgs *);

BtlRuntimeTask *btlCreateModelChangeTask(BtlUnit *unit, s32 option, s32 value08, s32 value0C, s32 value10, u8 flag19) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlModelChangeArgs));
    BtlModelChangeArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x1A;
    task->flags &= ~1;
    task->ownerId = unit->owner;
    task->onStart = btlBeginModelChange;
    task->callback = func_001E50E0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->resourceKind = option;
    args->resourceId = value08;
    args->delay = value0C;
    args->duration = value10;
    args->transitionMode = flag19;
    args->phase = 0;
    args->elapsed = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(s32 taskArgs) {
    if ((((BtlUnit *)((SoundTaskArgs *)taskArgs)->unk_0C)->flags & 2) != 0) {
        evtSetUnitStatusFlags(((BtlUnit *)((SoundTaskArgs *)taskArgs)->unk_0C)->ext);
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(u32 *taskArgs) {
    if ((btlUnitStatusPair((BtlUnit *)taskArgs[3]) & 0x1000000002) == 0x1000000002) {
        func_0023C870(((BtlUnit *)taskArgs[3])->ext, taskArgs[2], *taskArgs, taskArgs[1]);
    }
    return 1;
}

BtlRuntimeTask *btlCreateUnitTask0F(BtlUnit *unit, s32 value, s32 option, s32 value08) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0xF;
    task->ownerId = unit->owner;
    task->onStart = btlApplyLinkedUnitStatusWhenActorActive;
    task->callback = btlApplyUnitFxWhenLoaded;
    args = btlGetTaskArguments(task);
    args->unk_0C = (u32)unit;
    args->value = value;
    args->option = option;
    args->unk_08 = value08;
    return task;
}

void btlPrepareUnitStatusFxOnStart(s32 taskArgs) {
    if ((((FxTask *)taskArgs)->unit->flags & 2) != 0) {
        evtSetUnitStatusFlags(((FxTask *)taskArgs)->unit->ext);
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

BtlRuntimeTask *btlCreateUnitTask10(BtlUnit *unit, f32 *vec, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0x18);
    FxTask *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x10;
    task->ownerId = unit->owner;
    task->onStart = btlPrepareUnitStatusFxOnStart;
    task->callback = btlApplyUnitVectorFxWhenLoaded;
    args = btlGetTaskArguments(task);
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

BtlRuntimeTask *btlCreateUnitFadeInTask(BtlUnit *unit, u32 value, u32 variant) {
    BtlRuntimeTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlUnitFadeInTask;
    task->taskId = 0x11;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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

BtlRuntimeTask *btlCreateUnitFadeOutTask(BtlUnit *unit, u32 value, u32 variant) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlUnitFadeOutTask;
    task->taskId = 0x12;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->option = value;
    args->unk_08 = variant;
    args->unk_0C = 0;
    return task;
}

s32 btlStepUnitDefeatFadeIn(u32 *arguments) {
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
        evtSetUnitRgbTransition(unit->ext, 0, unit->overlayColor);
        evtSetUnitAlphaTransition(unit->ext, 0,
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

BtlRuntimeTask *func_001E5FF8(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x13;
    task->ownerId = actor->owner;
    task->callback = btlStepUnitDefeatFadeIn;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

s32 btlStepUnitDefeatFadeOut(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    u32 alpha;

    if (arguments[2] == 0) {
        btlFlagUnitDefeatCandidate(unit);
    }
    if ((s32)arguments[2] >= (s32)arguments[1]) {
        unit->flags &= ~0x20000;
        unit->overlayColor = unit->baseColor & 0xFFFFFF;
        return 1;
    }
    unit->flags |= 0x20000;
    alpha = (u32)((1.0f - (f32)(s32)arguments[2] / (f32)(s32)arguments[1]) * 128.0f);
    alpha <<= 24;
    unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
    arguments[2]++;
    return 0;
}

BtlRuntimeTask *func_001E61A0(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x14;
    task->ownerId = actor->owner;
    task->callback = btlStepUnitDefeatFadeOut;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

u32 func_001E6228(u32 *arguments) {
    BtlUnit *unit = (BtlUnit *)arguments[0];
    s32 finished = 0;
    u32 alpha;

    if (arguments[2] == 0) {
        unit->flags |= 0x80;
        btlFlagUnitDefeatCandidate(unit);
    }

    switch (arguments[1]) {
    case 0:
        if (arguments[2] == 1) {
            btlFlagUnitDefeatCandidate(unit);
            evtSetUnitRgbTransition(unit->ext, 10, 0x80000000);
        } else if (arguments[2] == 10) {
            evtSetUnitRgbTransition(unit->ext, 0, 0x80000000);
            evtSetUnitAlphaTransition(unit->ext, 6, 0);
            unit->flags |= 0x200000;
        }
        if ((unit->flags & 0x200000) == 0 && (s32)arguments[2] >= 16) {
            finished = 1;
        }
        break;

    case 1:
        if ((s32)arguments[2] >= 8) {
            unit->flags &= ~0x20000;
            unit->overlayColor = unit->baseColor & 0xFFFFFF;
            finished = 1;
        } else {
            unit->flags |= 0x20000;
            alpha = (u32)((1.0f - (f32)(s32)arguments[2] * 0.125f) * 128.0f);
            alpha <<= 24;
            unit->overlayColor = alpha | (unit->baseColor & 0xFFFFFF);
        }
        break;
    }

    arguments[2]++;
    if (finished != 0) {
        unit->flags = (unit->flags & ~0x80) | 0x40;
        return 1;
    }
    return 0;
}


BtlRuntimeTask *func_001E6428(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x15;
    task->ownerId = actor->owner;
    task->callback = func_001E6228;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}



typedef struct UnitEffectTaskArgs {
    BtlUnit *unit;
    SoundMixer *mixer;
    SoundVoice *effect;
    s32 duration;
    s32 counter;
} UnitEffectTaskArgs;

extern SoundVoice *func_00168548(SoundMixer *, s32, BtlUnit *, s32);
extern void effBattleUpdateSelectedValue(SoundVoice *, s32);
extern void func_00168978(SoundVoice *);

/* Start from the selected-unit SYSEFF source, then update through its duration.
 * Return one for an ineligible unit or expiry, zero while updating. */
s32 btlUpdateSelectedUnitEffect(UnitEffectTaskArgs *args) {
    BtlUnit *unit = args->unit;
    if (!(unit->flags & 2)) {
        return 1;
    }
    {
        BtlState *battle = (BtlState *)btlGetRuntime();
        if (args->effect == 0) {
            void *handle = battle->resources[BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT]->sourceHandle;
            unit->flags |= 0x80;
            args->mixer = sndMixerClone(handle);
            args->effect = func_00168548(args->mixer, 2, unit, 0);
            args->duration = 0xE;
            args->effect->flags &= 0xFFF9;
            effBattleUpdateSelectedValue(args->effect, 0xE);
            unit->flags &= ~8;
            if (unit->flags & 2) {
                unit->ext->owner->flags |= 1;
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
}

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

BtlRuntimeTask *btlCreateSelectedEffectUpdateTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(0x14);
    UnitEffectTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x16;
    task->flags |= 2;
    task->ownerId = unit->owner;
    task->callback = btlUpdateSelectedUnitEffect;
    task->onFinish = btlFinishSelectedUnitEffect;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->effect = 0;
    args->counter = 0;
    args->duration = 0;
    return task;
}

u32 btlUpdateCommandSoundTask(void) {
    btlUpdateUnitActors();
    return 1;
}

BtlRuntimeTask *btlCreateCommandSoundUpdateTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = btlUpdateCommandSoundTask;
    task->taskId = 0x1B;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

u32 btlUpdateCommandSoundTaskSecondary(void) {
    func_00209078();
    return 1;
}

BtlRuntimeTask *btlCreateSecondaryCommandSoundTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->taskId = 0x1C;
    task->flags |= 2;
    task->endCondition.kind = 0;
    task->onStart = 0;
    task->callback = btlUpdateCommandSoundTaskSecondary;
    return task;
}

u32 func_001E6790(void) {
    return 1;
}

BtlRuntimeTask *func_001E6798(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = func_001E6790;
    task->taskId = 0x20;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

typedef struct BtlUnitBaseLightArgs {
    BtlUnit *unit;
    s32 delay;
} BtlUnitBaseLightArgs;

/* vu0 routine: SDK quadword copies restore source colours and light direction. */
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
                f32 *defaultLight = D_0037F770[0];
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

BtlRuntimeTask *btlCreateUnitBaseLightTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(8);
    BtlUnitBaseLightArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlUnitBaseLightTask;
    task->taskId = 0x21;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->delay = 0;
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
        node = mdlGetNodeField2C(args->unit->ext->owner, 0);
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
        if (btlUnitStatusPair(args->unit) & 0x808000000000) {
            effObjFetchInnerFirstVec(args->unit->effectObject);
            VU0_STORE_VF(vf10, pos);
            pos[0] += scale;
        } else {
            func_001E3108(args->unit, pos);
            pos[0] += scale;
            pos[2] += args->unit->positionZOffset;
        }
        effObjSetInnerFirstVec(args->unit->effectObject, pos);
        args->scale *= 0.85f;
    } else {
        if (btlUnitStatusPair(args->unit) & 0x808000000000) {
            effObjFetchInnerFirstVec(args->unit->effectObject);
            VU0_STORE_VF(vf10, pos);
        } else {
            func_001E3108(args->unit, pos);
            pos[2] += args->unit->positionZOffset;
        }
        effObjSetInnerFirstVec(args->unit->effectObject, pos);
        return 1;
    }
    args->count += 1;
    return 0;
}

BtlRuntimeTask *btlCreateStiffenDamageShakeTask(BtlUnit *unit, f32 value) {
    BtlRuntimeTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x1D;
    task->callback = btlStiffenDamageShakeStep;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->scale = value;
    args->unk_08 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E6BF8);

extern u32 func_001E6BF8(u32 *);

BtlRuntimeTask *func_001E6E18(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = func_001E6BF8;
    task->taskId = 0x1E;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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

BtlRuntimeTask *btlScheduleActorUpdate(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = func_001E6E90;
    task->taskId = 0x1F;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

u32 btlRefreshUnitFxVectorTask(u32 *taskArgs) {
    btlRefreshUnitFxVectors(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateUnitFxVectorRefreshTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlRefreshUnitFxVectorTask;
    task->taskId = 0x22;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

extern s32 btlFormatUnitBedName(BtlUnit *, char *);
extern s32 fileQueueAlternateCallbackRequest(char *);

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
        s32 handle = fileQueueAlternateCallbackRequest(filename);
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

BtlRuntimeTask *btlCreateGunLoadPollTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x23;
    task->flags &= ~1;
    task->ownerId = unit->owner;
    task->onStart = btlStartGunFinishLoad;
    task->callback = btlPollGunLoad;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    args->option = 0;
    return task;
}

u32 btlUpdateUnitEffectsTask(void) {
    btlUpdateUnitEffects();
    return 1;
}

BtlRuntimeTask *btlCreateUpdateUnitEffectsTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = btlUpdateUnitEffectsTask;
    task->taskId = 0x24;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

typedef struct BtlActorTransparencyArgs {
    BtlUnit *unit;
} BtlActorTransparencyArgs;

u32 btlCreateActorTransparency(BtlActorTransparencyArgs *args) {
    BtlUnit *unit = args->unit;
    if (!(unit->flags & 2)) {
        return 0;
    }
    btlCreateUnitTransparency(unit);
    args->unit->flags |= 0x20000;
    return 1;
}


BtlRuntimeTask *btlCreateActorTransparencyTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    BtlActorTransparencyArgs *args;
    task->startCondition.kind = 1;
    task->taskId = 0x25;
    task->callback = btlCreateActorTransparency;
    task->endCondition.kind = 0;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    return task;
}

typedef struct BtlActorModelBlendArgs {
    BtlUnit *unit;
    BtlUnit *target;
    s32 index;
    s32 previousModelValue;
    s32 value;
    f32 scale;
    u32 stage;
} BtlActorModelBlendArgs;
typedef char BtlActorModelBlendArgsSizeCheck[sizeof(BtlActorModelBlendArgs) == 0x1C ? 1 : -1];

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E72B0);

extern u32 func_001E72B0(u32 *);

BtlRuntimeTask *btlCreateActorModelBlendTask(BtlUnit *unit, u32 target, u32 index, u32 value, f32 scale) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(BtlActorModelBlendArgs));
    BtlActorModelBlendArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = func_001E72B0;
    task->taskId = 0x26;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->unit = unit;
    args->target = (BtlUnit *)target;
    args->index = index;
    args->value = value;
    args->scale = scale;
    args->previousModelValue = -1;
    args->stage = 0;
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

BtlRuntimeTask *btlCreateUnitFaceBodyTask(BtlUnit *actor, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
    task->taskId = 0x27;
    task->ownerId = actor->owner;
    task->callback = btlRotateUnitTowardOtherBody;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 btlFlagDefeatCandidateTask(u32 *taskArgs) {
    btlFlagUnitDefeatCandidate(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateDefeatCandidateTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlFlagDefeatCandidateTask;
    task->taskId = 0x28;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

u32 btlClearDefeatCandidateTask(u32 *taskArgs) {
    btlClearUnitDefeatCandidate(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateDefeatCandidateClearTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlClearDefeatCandidateTask;
    task->taskId = 0x29;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
    return task;
}

typedef struct BtlActorMotionSlot {
    u8 pad00[4];
    s16 kind; /* 0x04 */
    s16 alphaStartFrame; /* 0x06 */
    f32 alphaFrameScale; /* 0x08 */
    u8 pad0C[4];
    s16 alphaDuration; /* 0x10 */
    u8 pad12[2];
} BtlActorMotionSlot;

typedef struct BtlActorStatusRecord {
    u8 pad00[0x2A];
    u16 model; /* 0x2A */
    BtlActorMotionSlot motions[29]; /* 0x2C; provider stride 0x270 */
} BtlActorStatusRecord;

/* Update motion completion, alpha transitions, and the selected-unit color pulse. */
void func_001E7648(void) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlActorStatusRecord *status;
    s32 parameter;
    s32 frame;
    s32 index;
    f32 pulse;
    u32 packed[4];
    /* Three boss installers publish no-argument hooks in this opaque slot. */
    void (*beforeMotionUpdate)(void) = *(void (**)(void))runtime->pad610;

    if (beforeMotionUpdate != NULL) {
        beforeMotionUpdate();
    }
    for (unit = runtime->units; unit != NULL; unit = unit->nextActor) {
        if (!(unit->flags & 0x600) || !(unit->flags & 2)) {
            continue;
        }
        status = (BtlActorStatusRecord *)btlGetSideIndexedActorStatusTable(
            unit->resourceKind, unit->resourceIndex);
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
                                (status->motions[index].alphaFrameScale * runtime->unk4C8)),
                            unit->overlayColor & 0xFFFFFF);
                        unit->updateFlags &= ~4;
                    }
                }
            }
            unit->overlayColor = mdlGetBroadcastValue(unit->ext->owner);
        }
        if (unit->flags & 0x8000) {
            /* The pulse uses the signed global counter at +0x214, not scene frame +0x234. */
            pulse = (f32)(*(s32 *)runtime->pad214 % 30) / 15.0f;
            if (pulse > 1.0f) {
                pulse = 2.0f - pulse;
            }
            VU0_SET_ONES_XYZ(vf10);
            VU0_SCALAR_OP(pulse * 1.6f + 0.3f, "vmulx.xyzw vf10, vf10, vf2x");
            EE_MMI_RGBA_PACK(packed[0]);
            btlBlendUnitColor(unit, (packed[0] & 0xFFFFFF) | 0x80000000, 0);
        }
    }
}

extern void func_001E3E20(BtlUnit *);
extern void func_002034A8(struct SoundResourceLink *);
extern void btlUpdateUnitCommandEffect(struct SoundLink *);
extern void func_0020EA18(BtlUnit *);

void btlUpdateActorModelColorAndLinks(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit = work->units;
    s32 color;

    for (; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 2) {
            MdlCtx *model = unit->ext->owner;
            if (!(unit->stateFlags & 0x20000)) {
                unit->unkEC = mdlGetNodeField2C(model, 0);
            }
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
                    func_001E3E20(unit);
                } else {
                    btlUpdateUnitTransparency(unit);
                }
            }
            func_002034A8(unit->link31C);
            btlUpdateUnitCommandEffect(unit->link320);
            if (!(work->commandRestrictFlags & 0x10000)) {
                func_0020EA18(unit);
            }
        }
    }
}

void btlResetUnitLinks(BtlUnit *unit) {
    unit->selectedEntryIndex = -1;
    unit->unk314 = -1;
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->gunResourceFlags = 0;
    unit->effectLink.flags = 0;
    btlClearAllActorEntrySlots(unit);
    unit->link31C = sndAllocResourceLink(unit);
    unit->link320 = sndAllocLink(unit);
}

extern s64 btlAdvanceRuntimeSequenceCounter(void);

extern void *memset(void *, s32, u32);

BtlUnit *btlCreateUnit(void) {
    u32 handle = sdfAllocGeneralBlock(0x368);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(handle);
    BtlState *work;
    memset(unit, 0, 0x368);
    unit->handle35C = handle;
    unit->owner = btlAdvanceRuntimeSequenceCounter();
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->lookupId = unit->selectedEntryIndex = -1;
    unit->unk2E4 = 6;
    unit->gunResourceFlags = 0;
    unit->effectLink.referenceCount = 0;
    unit->node318 = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults(unit);
    btlInitFxLights(unit);
    btlResetUnitLinks(unit);
    work = (BtlState *)btlGetRuntime();
    unit->previousActor = 0;
    if (work->units != 0) {
        work->units->previousActor = unit;
        unit->nextActor = work->units;
    } else {
        unit->nextActor = 0;
    }
    work->units = unit;
    btlBossDebugPrintf("btl:unit create[%p]\n", unit);
    return unit;
}

extern void sndFreeResourceNode(struct SoundResourceNode *);

extern void sndFreeResourceLink(struct SoundResourceLink *);

extern void sndFreeLink(struct SoundLink *);

extern void sdfFreeMemoryFromEitherHeap(void *);


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
        ((BtlState *)btlGetRuntime())->units = unit->nextActor;
    }
    sdfReleaseResourceAllocation(unit->handle35C);
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = next) {
        next = unit->nextActor;
        btlDestroyUnit(unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    s32 actor = (s32)((BtlState *)btlGetRuntime())->units;
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
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (unit->owner == owner) {
            return unit;
        }
    }
    return 0;
}

s32 btlIsActiveActor(BtlUnit *actor) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (unit == actor) {
            return 1;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeClear(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if (!(unit->partyRecord.flags & 0x20) && unit->partyRecord.unitId == mode) {
            return unit;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeFlagged(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if ((unit->partyRecord.flags & 0x20) && unit->partyRecord.unitId == mode) {
            return unit;
        }
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
    u32 i;

    for (i = 0; i < count; i++) {
        if (entry == btlGetIndexListEntry(list, i)) {
            return i;
        }
    }
    return -1;
}


void btlApplyUnitEffectScale(BtlUnit *unit) {
    ObjectTransform *inner;
    if (unit->flags & 2) {
        btlInitializeEffectVectorsFromSourceRecords(unit, unit->resourceKind, unit->resourceIndex);
        VU0_SET_ONES_XYZ(vf10);
        VU0_SCALAR_OP(unit->unk50, "vmulx.xyzw vf10, vf10, vf2x");
        inner = ((EffWorldNode *)unit->effectObject)->inner;
        inner->flags |= 1;
        inner->flags &= ~2;
        VU0_STORE_VF(vf10, inner->scale);
        mdlStoreTertiaryVectorVU(unit->ext->owner);
        mdlSetAmountOnAllContextResources(unit->ext->owner, unit->unk50);
        btlSetUnitPosition(unit, unit->currentPosition);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E8258);

void btlNormalizeActionCameraKeyScales(BtlLinkedCommand *action) {
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

void func_001E8580(BtlCamState *dst, BtlCamState *current, BtlCamState *target, f32 blend) {
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

extern void func_001E8580(BtlCamState *, BtlCamState *, BtlCamState *, f32);

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 btlScalarRangeStepQuadratic(f32);

s32 btlStepPoseBlendHalf(u8 *fx) {
    f32 t;
    if (((BtlLinkedCommand *)fx)->state == 0) {
        ((BtlLinkedCommand *)fx)->progressBits = 0;
        btlScalarRangeInitQuadratic(fx + 0x160, (f32)(((BtlLinkedCommand *)fx)->durationFrames * 2));
        btlCopyMotionTransform(&((BtlLinkedCommand *)fx)->camera, &((BtlLinkedCommand *)fx)->frontCamera);
        return 0;
    }
    t = btlScalarRangeStepQuadratic(1.0f);
    if (t > 0.5f) {
        t = 0.5f;
    }
    func_001E8580(&((BtlLinkedCommand *)fx)->camera, &((BtlLinkedCommand *)fx)->frontCamera, &((BtlLinkedCommand *)fx)->backCamera, t + t);
    ((BtlLinkedCommand *)fx)->progress = t;
    if (0.5f <= t) {
        return 1;
    }
    return 0;
}

extern void btlScalarRangeSetStartClearEnd(u8 *, f32);

extern f32 btlScalarRangeStepExponential(u8 *);

extern void func_001E8580(BtlCamState *, BtlCamState *, BtlCamState *, f32);

s32 btlStepPoseBlend(u8 *fx) {
    u8 *timer = fx + 0x158;
    BtlCamState *pose = &((BtlLinkedCommand *)fx)->frontCamera;
    f32 t;
    if (((BtlLinkedCommand *)fx)->state == 0) {
        btlScalarRangeSetStartClearEnd(timer, ((BtlLinkedCommand *)fx)->motionParameter);
        btlCopyMotionTransform(&((BtlLinkedCommand *)fx)->camera, pose);
    }
    t = btlScalarRangeStepExponential(timer);
    func_001E8580(&((BtlLinkedCommand *)fx)->camera, pose, &((BtlLinkedCommand *)fx)->backCamera, t);
    ((BtlLinkedCommand *)fx)->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

extern void func_001E8580(BtlCamState *, BtlCamState *, BtlCamState *, f32);

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 btlScalarRangeStepQuadratic(f32);

s32 btlStepPoseBlendFrame(u8 *fx) {
    f32 t;
    if (((BtlLinkedCommand *)fx)->state == 0) {
        ((BtlLinkedCommand *)fx)->progressBits = 0;
        btlScalarRangeInitQuadratic(fx + 0x160, (f32)((BtlLinkedCommand *)fx)->durationFrames);
        btlCopyMotionTransform(&((BtlLinkedCommand *)fx)->camera, &((BtlLinkedCommand *)fx)->frontCamera);
        return 0;
    }
    t = btlScalarRangeStepQuadratic(1.0f);
    func_001E8580(&((BtlLinkedCommand *)fx)->camera, &((BtlLinkedCommand *)fx)->frontCamera, &((BtlLinkedCommand *)fx)->backCamera, t);
    ((BtlLinkedCommand *)fx)->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

s32 btlStepPoseBlendRatio(u8 *fx) {
    f32 ratio = (f32)((BtlLinkedCommand *)fx)->state / (f32)((BtlLinkedCommand *)fx)->durationFrames;
    if (ratio <= 1.0f) {
        func_001E8580(&((BtlLinkedCommand *)fx)->camera, &((BtlLinkedCommand *)fx)->frontCamera, &((BtlLinkedCommand *)fx)->backCamera, ratio);
        return 0;
    }
    btlCopyMotionTransform(&((BtlLinkedCommand *)fx)->camera, &((BtlLinkedCommand *)fx)->backCamera);
    return 0;
}

/* VU0 math: constrain the pose direction at the fixed -20 height plane. */
s32 func_001E88A8(BtlCamState *state) {
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
s32 func_001E89E0(BtlCamState *state, f32 height) {
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

u32 btlExecuteCommandSoundTask(u32 *taskArgs) {
    func_001E8258(taskArgs[3], *taskArgs, taskArgs[1], taskArgs[2], taskArgs[4]);
    return 1;
}

BtlRuntimeTask *btlCreateCommandSoundTask(s32 actor, s32 mode) {
    BtlRuntimeTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x2A;
    if (actor != 0 && ((ActionStateLink *)actor)->unit != 0) {
        task->ownerId = ((ActionStateLink *)actor)->unit->owner;
    }
    task->callback = btlExecuteCommandSoundTask;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = (void *)actor;
    args->unk_0C = mode;
    args->option = 0;
    args->unk_08 = 0;
    args->unk_10 = 0;
    return task;
}

BtlRuntimeTask *btlCreateTargetedCommandSoundTask(s32 actor, s32 mode, u32 command) {
    BtlRuntimeTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = btlGetTaskArguments(task);

    args->unk_10 = command;
    return task;
}

BtlRuntimeTask *btlCreateCommandSoundWithArguments(s32 actor, s32 option, s32 flag, s32 mode, s32 command) {
    BtlRuntimeTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = btlGetTaskArguments(task);

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

BtlRuntimeTask *btlCreateFloatTask28(ActionStateLink *actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
    BtlRuntimeTask *task = btlAllocTask(0x24);
    f32 *args;
    task->startCondition.kind = 1;
    task->taskId = 0x2B;
    task->endCondition.kind = 0;
    if (actor != 0 && actor->unit != 0) {
        task->ownerId = actor->unit->owner;
    }
    task->callback = btlInitializeMotionTransformFromTaskArguments;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *(ActionStateLink **)args = actor;
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

BtlRuntimeTask *btlCreateFloatTask29(ActionStateLink *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    BtlRuntimeTask *task = btlAllocTask(0x44);
    f32 *args;
    task->startCondition.kind = 1;
    task->taskId = 0x2C;
    task->endCondition.kind = 0;
    if (actor != 0 && actor->unit != 0) {
        task->ownerId = actor->unit->owner;
    }
    task->callback = btlApplyEffectCameraKeyframes;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    *(ActionStateLink **)args = actor;
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
    ((BtlState *)work)->cameraCommand.flags |= 0x80000;
    return 1;
}

BtlRuntimeTask *btlCreateNotifyingCameraKeyframeTask(ActionStateLink *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    BtlRuntimeTask *task = btlCreateFloatTask29(actor, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
    task->callback = btlApplyCameraKeysAndMarkRuntimeChange;
    return task;
}

u32 btlRunCameraMotionResetTask(void) {
    s32 work;

    work = btlGetRuntime();
    btlResetCameraMotion(work + 0x70);
    return 1;
}

BtlRuntimeTask *btlScheduleContextReset(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = btlRunCameraMotionResetTask;
    task->taskId = 0x2D;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001E9130);
extern f32 D_003B6D70[4], D_003B6D80[4];
extern f32 D_003B6D50[4], D_003B6D60[4];
extern u32 D_00436A98;
extern void *dds3GetWorldObject(void);
extern EffWorldNode *dds3GetWorldCameraObject(EffWorldNode *);
extern EffWorldNode *dds3SetWorldCameraObject(EffWorldNode *, EffWorldNode *);
extern EffWorldNode *dds3CreateCameraObject(s32, void *, void *);
extern void dds3SetCameraFieldOfView(EffWorldNode *, f32);
extern void effObjSetInnerFloat(EffWorldNode *, f32);
extern void dds3EnsureSlotData(void *);
extern void func_001129C8(EffWorldNode *, s32);

/* vu0 routine: add the battle origin to the default camera position. */
void func_001E9410(void) {
    f32 position[4];
    EffWorldNode *camera;
    CameraData *data;
    BtlState *battle = (BtlState *)btlGetRuntime();

    battle->cameraCommand.camera.fov = 0.6981317f;
    VU0_LOAD_VF(vf10, D_003B6D70);
    VU0_LOAD_VF(vf11, battle->position);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, position);
    camera = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (camera != NULL) {
        effObjSetInnerFirstVec(camera, position);
        effObjSetInnerSecondVec(camera, D_003B6D80);
        data = camera->data;
        dds3SetCameraFieldOfView(camera, 0.6981317f);
        data->fovUpdatePending |= 1;
    }
    camera = dds3CreateCameraObject(dds3AdvanceWorldCounter(), D_003B6D50, D_003B6D60);
    camera->value = D_00436A98;
    effObjSetInnerFloat(camera, 10.0f);
    dds3EnsureSlotData(camera);
    func_001129C8(camera, 0);
    dds3SetCameraFieldOfView(camera, 0.6981317f);
    dds3SetWorldCameraObject(dds3GetWorldObject(), camera);
    battle->cameraObject = camera;
    battle->cameraCommand.targetList = btlAllocateIndexList(13);
    battle->battleFlags |= 0x10;
}

void btlClearPendingSoundList(void) {
    s32 context = btlGetRuntime();
    BtlIndexList *list = ((BtlState *)context)->cameraCommand.targetList;

    if (list != 0) {
        btlFreeIndexList(list);
        ((BtlState *)context)->cameraCommand.targetList = 0;
    }
    ((BtlState *)context)->battleFlags &= ~0x10;
}

void btlCopyMotionTransform(BtlCamState *dst, BtlCamState *src) {
    PCP_COPY_VECTOR(dst->position, src->position);
    PCP_COPY_VECTOR(dst->direction, src->direction);
    dst->distance = src->distance;
    dst->fov = src->fov;
}

void func_001E95C8(s32 transform, f32 value) {
    ((BtlCamState *)transform)->fov = value;
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
    ((BtlCamState *)object)->distance = 1.0f;
    ((BtlCamState *)object)->fov = 0.6981317f;
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
    ((BtlCamState *)object)->fov = scale * 0.017453293f;
}

void btlSetEffectCameraKeys(s32 fx, f32 x0, f32 y0, f32 z0, f32 vx0, f32 vy0, f32 vz0, f32 vw0, f32 x1, f32 y1, f32 z1,
                   f32 vx1, f32 vy1, f32 vz1, f32 vw1, f32 scale, f32 f154) {
    btlInitMotionTransformFromComponents(&((BtlLinkedCommand *)fx)->frontCamera, x0, y0, z0, vx0, vy0, vz0, vw0, scale);
    btlInitMotionTransformFromComponents(&((BtlLinkedCommand *)fx)->backCamera, x1, y1, z1, vx1, vy1, vz1, vw1, scale);
    ((BtlLinkedCommand *)fx)->motionParameter = f154;
    ((BtlLinkedCommand *)fx)->flags |= 0x41;
}

u32 btlGetActiveUnitId(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    return ((BtlState *)workAddress)->cameraCommand.status;
}

f32 btlGetPoseBlendProgress(u8 *unit) {
    return ((BtlLinkedCommand *)unit)->progress;
}

s32 btlIsUnitInActiveList(void *unit) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *slot = (u8 *)((BtlState *)work)->cameraCommand.link;
    u32 count;
    u32 i;
    if (slot != 0 && ((BtlActiveSlot *)slot)->unit == unit) {
        return 1;
    }
    count = btlGetIndexListCount(((BtlState *)work)->cameraCommand.targetList);
    for (i = 0; i < count; i++) {
        if (btlGetIndexListEntry(((BtlState *)work)->cameraCommand.targetList, i) == unit) {
            return 1;
        }
    }
    return 0;
}

void btlResetActiveUnitList(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlState *)workAddress)->cameraCommand.link = 0;
    ((BtlState *)workAddress)->cameraCommand.flags = ((BtlState *)workAddress)->cameraCommand.flags | 0x400;
    btlClearIndexList(((BtlState *)workAddress)->cameraCommand.targetList);
}

void btlClearRuntimeFlag2000(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlState *)workAddress)->cameraCommand.flags = ((BtlState *)workAddress)->cameraCommand.flags & 0xffffdfff;
}

void btlSetRuntimeFlag2000(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    ((BtlState *)workAddress)->cameraCommand.flags = ((BtlState *)workAddress)->cameraCommand.flags | 0x2000;
}

u32 btlIsRuntimeFlag2000Clear(void) {
    s32 workAddress;

    workAddress = btlGetRuntime();
    return ((((s32)((BtlState *)workAddress)->cameraCommand.flags >> 0xd)) ^ 1U) & 1;
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
    if (((BtlState *)btlGetRuntime())->battleFlags & 2) {
        object = dds3GetWorldObject();
        if (object != NULL) {
            handle = dds3GetWorldCameraObject((EffWorldNode *)object);
            if (handle != 0) {
                if (handle->next != 0) {
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

extern s32 D_00436A9C;

extern s32 D_00436AA0;

s32 btlGetWorldObjectDefault(void) {
    EffWorldNode *data;
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 2)) {
        return D_00436AA0;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return D_00436AA0;
    }
    if (data->value == 0) {
        return D_00436A9C;
    }
    return data->value;
}

s32 btlIsWorldMotionIdle(void) {
    EffWorldNode *data;
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 2)) {
        return 0;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return 0;
    }
    return data->value == 0;
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
    btlFlagMatchingUnitsDefeatCandidate(((BtlLinkedCommand *)action)->link->unit->flags & 0x600);
}

extern void btlFlagMatchingUnitsDefeatCandidate(s32);

void btlApplyCombinedActorFlags(u8 *fx) {
    u32 i = 0;
    s32 bits = 0;
    u32 count = btlGetIndexListCount(((BtlLinkedCommand *)fx)->targetList);
    if (count != 0) {
        do {
            bits |= ((BtlUnit *)btlGetIndexListEntry(((BtlLinkedCommand *)fx)->targetList, i++))->flags & 0x600;
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
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    f32 current;
    f32 limit;
    if (work->cameraCommand.status == 1 || btlHasSingleLinkedResource(action) != 0) {
        for (unit = work->units; unit != 0; unit = unit->nextActor) {
            if (unit->flags & 1) {
                if (unit->flags & 0x200) {
                    if (unit->flags & 2) {
                        if (unit->ext != 0) {
                            s32 node = mdlGetNodeField2C(unit->ext->owner, 0);
                            if (node == 0xD || node == 0x12) {
                                current = btlGetUnitModelValue1C(unit);
                                limit = (f32)btlGetUnitModelFrameCount(unit);
                                if (unit->partyRecord.unitId < 0xA) {
                                    limit = limit * D_003B6D90[unit->partyRecord.unitId];
                                } else {
                                    limit = limit * 0.7f;
                                }
                                if (current < limit) {
                                    sdfMotionSampleAtFrame(unit->ext->owner->first, limit);
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

s32 btlPositionActorIndexUnits(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    u32 count;
    f32 pos[4];
    if (work->unk268 != 3) {
        return 0;
    }
    count = btlGetIndexListCount(action->targetList);
    if (count != 1) {
        return 0;
    }
    unit = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
    if (!(unit->flags & 0x200)) {
        return 0;
    }
    if (unit->lookupId != count) {
        return 0;
    }
    for (unit = work->units; unit != 0; unit = unit->nextActor) {
        if ((unit->flags & 0x100) && unit->lookupId != 1) {
            func_001E3108(unit, pos);
            pos[2] = unit->position[2] - 90.0f;
            btlSetUnitPosition(unit, pos);
        }
    }
    return 1;
}


void btlDebugPrintWorldTransform(s32 arg0, u8 *arg1) {
    EffWorldNode *object;
    if (((BtlState *)btlGetRuntime())->battleFlags & 2) {
        object = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (object != 0) {
            btlBossDebugPrintfN(arg0, (s32)arg1, 0, "P:%.1f %.1f %.1f", (double)object->inner->position[0],
                                (double)object->inner->position[1], (double)object->inner->position[2]);
            btlBossDebugPrintfN(arg0, (s32)(arg1 + 0xC), 0, "R:%.3f %.3f %.3f %.3f",
                                (double)object->inner->rotation[0], (double)object->inner->rotation[1],
                                (double)object->inner->rotation[2], (double)object->inner->rotation[3]);
        }
    }
}

extern void func_00208750(BtlIndexList *, s32, s32);

void btlFaceActionParticipantsTowardLinkedTarget(BtlLinkedCommand *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    BtlUnit *first;
    u32 i;
    u32 count = btlGetIndexListCount(action->targetList);
    if (count != 0) {
        target = action->link->unit;
        if (count == 1) {
            first = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
            if (target != 0 && (target->flags & 0x600) == (first->flags & 0x600)) {
                return;
            }
            btlUnitGetMuzzlePosVU(first);
        } else {
            func_00208750(action->targetList, 0, 0);
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
            btlUnitFaceTarget((BtlUnit *)btlGetIndexListEntry(action->targetList, i), target);
        }
    }
}

void btlAimLinkedUnitAtMuzzle(BtlLinkedCommand *action) {
    s128 vec[3];
    s128 *pos;
    BtlUnit *target;
    u32 count = btlGetIndexListCount(action->targetList);
    if (count != 0) {
        target = action->link->unit;
        if (count == 1) {
            btlUnitGetMuzzlePosVU((BtlUnit *)btlGetIndexListEntry(action->targetList, 0));
        } else {
            func_00208750(action->targetList, 0, 0);
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

/* Return whether a live linked group has its byte at 0x14 marked. */
s32 btlHasMarkedEntry14(s32 actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    BtlOperandGroup *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (i = 0; i < count; i++, entry++) {
        if (entry->reflected != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlCanUseLinkedActor(s32 actor) {
    u32 status = ((BtlLinkedCommand *)actor)->status;
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
    linked = (s32)((BtlLinkedCommand *)actor)->link;
    if (linked == 0) {
        return 1;
    }
    if (btlHasMarkedEntry14(actor)) {
        return 0;
    }
    if (((ActionStateLink *)linked)->unit->partyRecord.status & 0x480) {
        return 0;
    }
    category = ((BtlLinkedCommand *)actor)->actionCode;
    if (category != 0 && (((BtlActionTableEntry *)datActionAnimationRecords)[category].flags & 1)) {
        return 0;
    }
    return 1;
}

/* Return whether a live linked group has its byte at 0x10 marked. */
s32 btlHasMarkedEntry10(s32 actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    BtlOperandGroup *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (i = 0; i < count; i++, entry++) {
        if (entry->inactive != 0) {
            return 1;
        }
    }
    return 0;
}

extern f32 btlUnitGetTopY(BtlUnit *);

s32 btlCheckActorDistanceLimit(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;

    while (unit != NULL) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY(unit) > 400.0f) {
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

/* Find an unmarked linked kind-two slot whose unit is not disabled. */
s32 btlHasIdleLinkedSlotKindTwo(u8 *actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    u32 i;
    BtlOperandGroup *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (; i < count; i++, entry++) {
        if (entry->inactive == 0 && entry->kind == 1 && entry->parameter == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(linked->indexWork.indices, i))->flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

/* Find a type-two linked group whose associated unit is not disabled. */
s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor) {
    ActionStateLink *linked = ((BtlLinkedCommand *)actor)->link;
    u32 count;
    u32 i;
    BtlOperandGroup *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(linked->indexWork.indices);
    entry = linked->indexWork.groups;
    for (; i < count; i++, entry++) {
        if (entry->kind == 2 &&
            !(((BtlUnit *)btlGetIndexListEntry(linked->indexWork.indices, i))->flags & 0x80002000)) {
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
    if (((BtlLinkedCommand *)fx)->link == 0) {
        return 0;
    }
    if (btlHasSingleLinkedResource() == 0) {
        return 0;
    }
    task = (u8 *)((BtlLinkedCommand *)fx)->link;
    owner = (u8 *)((ActionStateLink *)task)->unit;
    index = ((ActionStateLink *)task)->indexWork.slot;
    table = (u8 *)btlGetSideIndexedActorStatusTable(((BtlUnit *)owner)->resourceKind, ((BtlUnit *)owner)->resourceIndex);
    if (((BtlLinkedCommand *)fx)->actionCode == 0x91) {
        return 0;
    }
    return ((BtlEffectResource *)table)->nodes[index].triggerKind == 2;
}

static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

s32 btlHasActorCategoryFlag100(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 0x100);
}

s32 btlIsActorCategoryTypeTwo(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category == 0) {
        return 0;
    }
    return datCommandRecords[category].unk30 == 2;
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
    category = ((BtlLinkedCommand *)actor)->actionCode;
    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 2);
}

s32 btlHasSingleLinkedResource(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category != 0 && datCommandRecords[category].unk_08 != 0) {
        return 0;
    }
    return btlGetIndexListCount(((BtlLinkedCommand *)actor)->targetList) == 1;
}

s32 btlCanUseActorCategoryFlag4(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category == 0) {
        return 0;
    }
    if ((datCommandRecords[category].options & 1) == 0) {
        if (!btlCanUseLinkedActor(actor)) {
            return 0;
        }
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[((BtlLinkedCommand *)actor)->actionCode].flags, 4);
}

s32 btlIsActorCategoryMarked(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category == 0) {
        return 0;
    }
    return datCommandRecords[category].unk30 == 1;
}

s32 btlHasActorCategoryFlag40(s32 actor) {
    s32 category = ((BtlLinkedCommand *)actor)->actionCode;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)datActionAnimationRecords)[category].flags, 0x40);
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    BtlUnit *entry;

    switch (((BtlLinkedCommand *)actor)->status) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = (s32)((BtlLinkedCommand *)actor)->link;
    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(((ActionStateLink *)linked)->indexWork.indices) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(((ActionStateLink *)linked)->indexWork.indices, 0);
    return ((((ActionStateLink *)linked)->unit->flags ^ entry->flags) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(s32 actor) {
    s32 linked = (s32)((BtlLinkedCommand *)actor)->link;
    BtlUnit *entry;
    u32 category;

    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(((ActionStateLink *)linked)->indexWork.indices) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(((ActionStateLink *)linked)->indexWork.indices, 0);
    if ((entry->flags & 0x400) == 0) {
        return 0;
    }
    category = entry->resourceIndex;
    if (category >= 0x180) {
        return 0;
    }
    return btlHasFlag(((BtlResourceTableEntry *)datEnemyRecords)[category].flags, 0x1000);
}

u8 func_001EA940(s32 action) {
    return ((BtlLinkedCommand *)action)->actionCode == 0x5f;
}

s32 btlMapActorCategory(s32 actor) {
    switch ((u32)((BtlLinkedCommand *)actor)->actionCode) {
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
    switch ((u32)((BtlLinkedCommand *)actor)->actionCode) {
    case 0x5B:
    case 0x5C:
    case 0x5D:
        return 1;
    default:
        return 0;
    }
}

u32 func_001EAA00(BtlLinkedCommand *action) {
    return 0;
}

u8 func_001EAA08(s32 action) {
    return ((BtlLinkedCommand *)action)->actionCode == 0x1a0;
}

void func_001EAA18(void) {
}

void func_001EAA20(void) {
}

extern void btlPrepareActionCameraPoseWithActorClearance(void *unit, f32 *pose, u8 *out);
extern void func_001F20B0(void *unit, f32 *pose, u8 *out);
extern void func_001F20C8(BtlCamState *, BtlCamState *, BtlCamState *);

/* Choose the action's camera pose from active ally and enemy height maxima. */
void btlChooseCameraPoseByActorHeights(BtlLinkedCommand *action) {
    BtlState *work;
    BtlUnit *unit;
    s32 enemyCount;
    f32 enemyHeight;
    f32 allyHeight;
    f32 height;

    work = (BtlState *)btlGetRuntime();
    if (work->unk670 != NULL) {
        if (work->unk670(action)) {
            return;
        }
    }
    enemyCount = 0;
    enemyHeight = 0.0f;
    allyHeight = 0.0f;
    unit = work->units;
    for (; unit != NULL; unit = unit->nextActor) {
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
                func_001F20C8(&action->camera, &action->frontCamera, &action->backCamera);
            } else {
                btlPrepareActionCameraPoseWithActorClearance(action, action->frontCamera.position, (u8 *)&action->backCamera);
            }
            break;
        case 2:
            btlPrepareActionCameraPoseWithActorClearance(action, action->frontCamera.position, (u8 *)&action->backCamera);
            break;
        case 3:
            func_001F20B0(action, action->frontCamera.position, (u8 *)&action->backCamera);
            break;
        }
    } else {
        switch (effMiscRandMod(0, 2)) {
        case 0:
            btlPrepareActionCameraPoseWithActorClearance(action, action->frontCamera.position, (u8 *)&action->backCamera);
            break;
        case 1:
            func_001F20B0(action, action->frontCamera.position, (u8 *)&action->backCamera);
            break;
        }
    }
    action->motionParameter = 100.0f;
    action->flags |= 0x41;
}

void func_001EAC08(void) {
}

void func_001EAC10(u32 action) {
    func_001ECBF8(action, action);
}

void func_001EAC28(void) {
}

extern void btlInitTargetCursorAndFacing(BtlLinkedCommand *, void *);
extern void btlPrepareUnitPoseWithTiltRotation(void *, f32 *, u8 *);
extern void btlFlagUserAndTargetDefeat(BtlLinkedCommand *, BtlLinkedCommand *);
extern void btlSetupActionCameraPair(BtlLinkedCommand *);

/* Select the linked command's initial cursor step and prepare its camera. */
void btlInitializeLinkedCommandCursor(BtlLinkedCommand *action) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk648;
    ActionStateLink *link;
    u32 flags;

    action->stepKind = 0;
    link = action->link;
    if (hook != NULL && hook((BtlUnit *)action) != 0) {
        return;
    }
    flags = link->unit->flags;
    if (flags & 0x200) {
        if (!(flags & 0x1000) && !(link->unit->partyRecord.flags & 0x10)) {
            action->stepKind = 0xB;
            func_001FF5D8(action, action);
        } else if (btlHasSingleLinkedResource((s32)action) != 0) {
            if ((link->unit->partyRecord.flags & 0x10) && link->indexWork.slot == 0x17) {
                action->stepKind = 0xC;
                func_001F3C30(action);
            } else {
                action->stepKind = 9;
                btlFlagUserAndTargetDefeat(action, action);
            }
        } else {
            btlInitTargetCursorAndFacing(action, action);
        }
    } else {
        if (btlMatchLinkedActorFlags((s32)action) != 0) {
            func_001F0968(action);
        } else if (btlHasSingleLinkedResource((s32)action) != 0) {
            btlPositionActorIndexUnits(action);
            action->stepKind = 0xA;
            btlSetupActionCameraPair(action);
        } else {
            btlPrepareUnitPoseWithTiltRotation(action, action->frontCamera.position, (u8 *)&action->backCamera);
            btlAimLinkedUnitAtMuzzle(action);
            action->motionParameter = 200.0f;
            action->flags |= 0x41;
        }
        btlResetCameraMotion((s32)action);
    }
}

void btlDispatchActionCursorStepByKind(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (work->unk64C != 0 && work->unk64C((BtlUnit *)action) != 0) {
        return;
    }
    switch (action->stepKind) {
    case 9:
        btlBuildApproachCamera(action, &action->camera);
        break;
    case 0xA:
        btlUpdateActionTargetCameraPose(action);
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

extern void btlAdvanceUnblockedPlayerCursorAnimation(u32);
extern void btlRefreshActionPoseBlendSnapshot();
extern void btlAimEffectPoseAtUnit();
extern void func_001ED9A0();
extern void func_001F34E0(BtlLinkedCommand *, BtlCamState *);
extern void btlBuildHeightClampedApproachCamera(BtlLinkedCommand *, BtlCamState *);

/* Dispatch camera-step work unless a runtime override handles it. */
void btlDispatchActionCameraStep(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();

    if (func_001EAA00(action) != 0) {
        btlAdvanceUnblockedPlayerCursorAnimation((u32)action);
        return;
    }
    if (work->actionCameraStepHook != 0 && work->actionCameraStepHook(action) != 0) {
        return;
    }
    switch (action->stepKind) {
    case 2:
        btlRefreshActionPoseBlendSnapshot(action, &action->camera);
        break;
    case 9:
        btlBuildApproachCamera(action, &action->camera);
        break;
    case 4:
        btlAimEffectPoseAtUnit((u8 *)action, (u8 *)&action->camera);
        break;
    case 5:
        func_001ED9A0(action, &action->camera);
        break;
    case 7:
        func_001F34E0(action, &action->camera);
        break;
    case 8:
        btlBuildHeightClampedApproachCamera(action, &action->camera);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EB5B0);

void btlAdvanceActorStageAndPose(BtlLinkedCommand *action) {
    BtlState *work;
    s32 category;
    u8 *out;
    if (func_001EAA00(action) != 0) {
        func_001EC5F0((u32)action);
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
            func_001EC868(action, action->camera.position, 0.0f);
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
        func_001E88A8((BtlCamState *)out);
        return;
    }
    if (action->stepKind == 0xD) {
        btlAdvanceTargetCursorAnimation((s32)action, (s32)action);
    }
}

void btlUpdateActionPoseForLinkedTarget(BtlLinkedCommand *action) {
    BtlUnit *target;
    if (((BtlState *)btlGetRuntime())->unk220 & 1) {
        target = action->link->unit;
        if (target->flags & 0x400) {
            func_001ECBF8(action, action, target);
            return;
        }
    }
    if (action->actionKind == action->status || action->actionKind == 0xA || (action->flags & 0x40000)) {
        btlCopyMotionTransform(&action->frontCamera, &action->camera);
        func_001F17C8(action, &action->backCamera, action->link->unit, 0);
        action->motionParameter = 7.0f;
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

extern void func_001EE690(BtlLinkedCommand *, f32 *, s32, f32, f32);

void func_001EC198(BtlLinkedCommand *action) {
    BtlUnit *target;
    s32 kind;
    f32 pos[4];
    if (btlGetIndexListCount(action->targetList) != 1) {
        return;
    }
    target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
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
    func_001EE690(action, action->frontCamera.position, kind, 45.0f, 0.25f);
    func_001EE690(action, action->backCamera.position, kind, 1.0f, 0.5f);
    action->motionParameter = 30.0f;
    action->flags |= 0x41;
}

void func_001EC2A0(void) {
}

void btlStartLinkedActionPoseBlendIfEligible(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (action->link->unit->flags & 0x400) {
        if (work->unk660 != 0) {
            s32 hasFlag200 = 0;
            s32 hasFlag400 = 0;
            u32 i;
            u32 count = btlGetIndexListCount(action->link->indexWork.indices);
            for (i = 0; i < count; i++) {
                BtlUnit *entry = (BtlUnit *)btlGetIndexListEntry(action->link->indexWork.indices, i);
                if (entry->flags & 0x200) {
                    hasFlag200 = 1;
                }
                if (entry->flags & 0x400) {
                    hasFlag400 = 1;
                }
            }
            if (work->unk660((BtlUnit *)action, hasFlag200, hasFlag400) != 0) {
                action->flags |= 0x10000;
                return;
            }
        }
        btlPrepareUnitPoseWithTiltRotation(action, action->frontCamera.position, (u8 *)&action->backCamera);
        action->motionParameter = 200.0f;
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

void func_001EC650(BtlLinkedCommand *action) {
    func_001F2758(action, &action->frontCamera, &action->backCamera);
}

void func_001EC670(void) {
    func_001F2AE8();
}

void btlAppendLinkedUnitToActorIndices(u32 action) {
    s32 actor;

    actor = (s32)action;
    btlAppendIndexListEntry(((BtlLinkedCommand *)actor)->targetList, ((BtlLinkedCommand *)actor)->link->unit);
    func_001F2E30(action, &((BtlLinkedCommand *)actor)->frontCamera, &((BtlLinkedCommand *)actor)->backCamera);
}

void func_001EC6C8(void) {
}

void btlUpdateLinkedActionEffectVectorByTarget(u32 action) {
    if ((((BtlLinkedCommand *)action)->link->unit->flags & 0x200) != 0) {
        btlBuildGroupFramingCameraPose(&((BtlLinkedCommand *)action)->camera, &((BtlLinkedCommand *)action)->camera);
        return;
    }
    if (((BtlLinkedCommand *)action)->actionKind != 0x10) {
        btlResetUnitEffectVector(action, action);
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

void func_001EC768(u32 action) {
    func_001F35C8(action, &((BtlLinkedCommand *)action)->frontCamera, &((BtlLinkedCommand *)action)->backCamera);
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

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EC868);

void func_001ECBF8(u8 *unit, f32 *vec) {
    func_001EC868(unit, vec, 27.5f);
}

void btlPrepareUnitPoseWithTiltRotation(void *unit, f32 *pose, u8 *out) {
    func_001EC868(unit, pose, 20.0f);
    btlCopyMotionTransform((BtlCamState *)out, (BtlCamState *)pose);
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
        func_001E88A8(from);
        func_001E88A8(to);
        action->motionParameter = poses[pose][9];
        action->flags |= 0x41;
    }
}

void btlAimEffectPoseAtUnit(u8 *fx) {
    BtlUnit *unit = ((BtlLinkedCommand *)fx)->link->unit;
    if (unit->flags & 2) {
        if (btlSetActorEffectParameter(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, &((BtlLinkedCommand *)fx)->backCamera);
        func_001E88A8(&((BtlLinkedCommand *)fx)->backCamera);
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

    if (unit->flags & 2) {
        btlClearAllUnitDefeatCandidates();
        btlFlagMatchingUnitsDefeatCandidate(unit->flags & 0x600);
        if (btlHasSingleLinkedResource((s32)action) == 0) {
            func_001EF030(action, from);
        } else {
            func_001EEB78(action, from, 1);
        }
        btlCopyUnitRotationQuaternion((u8 *)unit, (s128 *)quat);
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
        func_001E88A8(to);
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
        func_001E88A8(saved);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001ED6C8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001ED9A0);

void btlSetupCameraPoseAimUnit(BtlLinkedCommand *action, BtlCamState *from, BtlCamState *to) {
    f32 quat[4];
    BtlUnit *unit = action->link->unit;
    f32 fov;
    btlClearAllUnitDefeatCandidates();
    btlFlagMatchingUnitsDefeatCandidate(unit->flags & 0x600);
    btlCopyUnitRotationQuaternion((u8 *)unit, (s128 *)quat);
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
    func_001E88A8(from);
    func_001E88A8(to);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EDC38);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EDFB8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EE458);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EE690);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EEB78);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EF030);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001EF668);

void btlActionAimUserAtTargets(BtlLinkedCommand *action, f32 *pose, u8 *out) {
    s128 vec[3];
    BtlUnit *unit = action->link->unit;
    u32 mask = 0;
    u32 i;
    u32 count;
    btlPrepareUnitPoseWithTiltRotation(action, pose, out);
    count = btlGetIndexListCount(action->targetList);
    for (i = 0; i < count; i++) {
        mask |= ((BtlUnit *)btlGetIndexListEntry(action->targetList, i))->flags & 0x600;
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
        groups |= ((BtlUnit *)btlGetIndexListEntry(action->targetList, i))->flags & 0x600;
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

extern f32 func_001ADBD0(ActionStateLink *);
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
    span /= func_001ADBD0(action->link);
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
    func_001E89E0(out, -10.0f);
}

void btlSetupActionCameraPair(BtlLinkedCommand *command) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    f32 lookPos[4];

    user = command->link->unit;
    target = (BtlUnit *)btlGetIndexListEntry(command->targetList, 0);
    if (!(user->flags & target->flags & 0x600)) {
        btlFlagAllUnitsDefeatCandidate();
    } else {
        func_001ECBF8(command, command);
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
    if (target->flags & user->flags & 0x600) {
        return;
    }
    if (action->motionProgress != 0) {
        return;
    }
    frames = func_001E2E58(user, user->unkEC);
    frames = (s32)((f32)frames / func_001ADBD0(action->link));
    if (action->state == frames && (target->flags & 0x200)) {
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
        func_001E88A8(out);
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
        func_001E88A8(out);
    }
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F0968);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F0C80);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F1120);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F1290);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F17C8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F1B00);

/* vu0 routine: measure camera clearance from the actor's adjusted muzzle position. */
void btlPrepareActionCameraPoseWithActorClearance(void *unit, f32 *pose, u8 *out) {
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    f32 *target = (f32 *)out;
    f32 span;
    f32 distance;

    for (; actor != 0; actor = actor->nextActor) {
        s32 flags = actor->flags;
        if (!(flags & 1)) {
            continue;
        }
        if (flags & 0x200) {
            break;
        }
    }
    func_001ECBF8(unit, target);
    btlCopyMotionTransform((BtlCamState *)pose, (BtlCamState *)out);
    span = func_00208000(0x200, 0, 0) * 0.5f;
    pose[0] -= span;
    target[0] += span;
    target[8] *= 0.8f;
    btlUnitGetMuzzlePosVU(actor);
    VU0_SET_VF10_COMPONENT(y, -btlUnitGetTopY(actor));
    VU0_LOAD_VF(vf11, out);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    distance += (actor->unkC0 * actor->scale * 2.0f) /
                func_00353228(target[9] * 0.5f);
    if (target[8] < distance) {
        target[8] = distance;
    }
}

void func_001F20B0(void *unit, f32 *pose, u8 *out) {
    btlPrepareUnitPoseWithTiltRotation(unit, pose, out);
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
    from->distance = length + selected->unkC0 * selected->scale * 3.5f /
                              func_00353228(fov * 0.5f);
    btlCopyMotionTransform(to, from);
    func_00336538(-(45.0f * 0.017453293f));
    VU0_LOAD_VF(vf10, to->direction);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, to->direction);
    to->distance = length + selected->unkC0 * selected->scale * 3.0f /
                            func_00353228(fov * 0.5f);
    func_001E88A8(from);
    func_001E88A8(to);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2308);

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
    func_001E88A8(out);
}

void btlResetUnitEffectVector(u8 *unit, f32 *vec) {
    func_001EC868(unit, vec, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2758);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2AE8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F2E30);

void func_001F3228(u32 action) {
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
    span /= func_001ADBD0(action->link);
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
    func_001E88A8(out);
}

void func_001F34C8(u32 action) {
    func_001F3228(action);
}

void func_001F34E0(BtlLinkedCommand *action, BtlCamState *out) {
    btlBuildHeightClampedApproachCamera(action, out);
}

void btlChooseActionPoseBlendFromActorCount(BtlLinkedCommand *action) {
    BtlState *work = (BtlState *)btlGetRuntime();
    u32 count;
    u32 mask;
    BtlUnit *unit;
    mask = ((BtlUnit *)btlGetIndexListEntry(action->targetList, 0))->flags & 0x600;
    count = 0;
    for (unit = work->units; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & mask) {
                count++;
            }
        }
    }
    if (count >= 2) {
        func_001EDFB8(action, &action->frontCamera, &action->backCamera);
        return;
    }
    btlPrepareUnitPoseWithTiltRotation(action, action->frontCamera.position, (u8 *)&action->backCamera);
    action->motionParameter = 200.0f;
    action->flags |= 0x41;
}

void func_001F35C0(void) {
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F35C8);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F3888);

/* Nine actor camera rows, each containing two 0x74-byte mode records. */
typedef struct BtlActionCameraPose {
    f32 position[4];
    f32 direction[4];
} BtlActionCameraPose;

typedef struct BtlActionCameraMode {
    BtlActionCameraPose front;
    BtlActionCameraPose back;
    u8 pad40[0x24];
    f32 motionParameter; /* 0x64 */
    u8 pad68[0xC];
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
        targetFlags |= target->flags & 0x600;
    }

    btlClearAllUnitDefeatCandidates();
    if (targetFlags == 0x200) {
        btlFlagUnitDefeatCandidate(user);
    } else {
        btlFlagMatchingUnitsDefeatCandidate(0x200);
    }

    btlInitMotionTransformFromComponents((u8 *)&action->frontCamera,
                                         D_003B6E50[unitId].mode[mode].front.position[0],
                                         D_003B6E50[unitId].mode[mode].front.position[1],
                                         D_003B6E50[unitId].mode[mode].front.position[2],
                                         D_003B6E50[unitId].mode[mode].front.direction[0],
                                         D_003B6E50[unitId].mode[mode].front.direction[1],
                                         D_003B6E50[unitId].mode[mode].front.direction[2],
                                         D_003B6E50[unitId].mode[mode].front.direction[3], 40.0f);
    btlInitMotionTransformFromComponents((u8 *)&action->backCamera,
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
    *(s32 *)action->pad140 = 0;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F3E48);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001F41F0);

void btlAdvancePlayerCursorAnimation(s32 action, s32 state) {
    if (!(((BtlLinkedCommand *)action)->link->unit->flags & 0x400)) {
        func_001FA480(action, state, D_003BBFA8[CURSOR->unk_0A]);
        func_001FBAC0(action, state);
        func_001FB908(action, (BtlCamState *)state, 0, 0);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

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

void func_001F4E30(BtlLinkedCommand *action) {
    CURSOR->frame = 0;
    if (!(action->link->unit->flags & 0x400)) {
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

extern s32 D_003BBFC8[];

void func_001F4F10(BtlLinkedCommand *action, s32 state) {
    if (!(action->link->unit->flags & 0x400)) {
        func_001FA480((s32)action, state, D_003BBFC8[CURSOR->unk_0C]);
        func_001FBAC0((s32)action, state);
        func_001FB908((s32)action, (BtlCamState *)state, 0, 0);
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
            func_001FA480((s32)action, state, D_003BBFC8[CURSOR->unk_0C]);
            func_001FBAC0((s32)action, state);
            func_001FB908((s32)action, (BtlCamState *)state, 0, 0);
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
extern s16 D_003BBD90[];
extern s16 D_003BBD70[];

typedef struct BtlCursorChoices {
    u16 values[3][3][8];
} BtlCursorChoices;

extern const BtlCursorChoices D_004184D8;

void func_001F5018(BtlLinkedCommand *action, s32 state) {
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
    func_001F5868((s32)action, state, 0, D_003BBD90[CURSOR->index]);
    func_001F5320((s32)action, state, 0, D_003BBD70[CURSOR->index]);
}

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

/* vu0 routine: constrain a camera pose endpoint to the enabled height planes. */
s32 func_001FB908(s32 action, BtlCamState *pose, s8 bypassUpper, s8 bypassLower) {
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


INCLUDE_ASM(const s32, "game/code_001DD390", func_001FBAC0);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FC5E0);

INCLUDE_ASM(const s32, "game/code_001DD390", func_001FD400);

BtlUnit *btlFindActiveActorById(s32 id) {
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
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
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            if (!(unit->flags & 0x20)) {
                count++;
            }
        }
    }
    return count;
}

func_001FF5D8(s32 action, s32 state) {
    memset(D_003BD7D0, 0, sizeof(SoundCursor));
    switch (((BtlLinkedCommand *)action)->link->unit->partyRecord.unitId) {
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
            func_001EEB78((BtlLinkedCommand *)action, &((BtlLinkedCommand *)action)->camera, 1);
        }
        break;
    }
}

void btlAdvanceCursorForUnmarkedUnit(s32 action, s32 state) {
    if (CURSOR->unk_00 == 1) {
        if (!(((BtlLinkedCommand *)action)->link->unit->flags & 0x400)) {
            func_001FA480(action, state, D_003BC0A0[CURSOR->unk_0C]);
            func_001FBAC0(action, state);
            func_001FB908(action, (BtlCamState *)state, 0, 1);
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
        if (((BtlLinkedCommand *)action)->link->unit->flags & 0x400) {
            return;
        }
        func_001FA480(action, state, D_003BC0C0[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        func_001FB908(action, (BtlCamState *)state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera((BtlLinkedCommand *)action, &((BtlLinkedCommand *)action)->camera);
    }
}

void btlInitCommandCursorForCategory(s32 action, s32 state) {
    memset(D_003BD7D0, 0, 0x130);
    switch (((BtlLinkedCommand *)action)->link->unit->partyRecord.unitId) {
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
        if (((BtlLinkedCommand *)action)->link->unit->flags & 0x400) {
            return;
        }
        func_001FA480(action, state, D_003BC0C8[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        func_001FB908(action, (BtlCamState *)state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    } else {
        btlBuildApproachCamera((BtlLinkedCommand *)action, &((BtlLinkedCommand *)action)->camera);
    }
}

void btlInitCommandCursorForFirstActor(s32 action, s32 state) {
    BtlUnit *first;
    btlGetRuntime();
    first = btlGetIndexListEntry(((BtlLinkedCommand *)action)->targetList, 0);
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

void btlAdvanceTargetCursorAnimation(s32 action, s32 state) {
    if (!(((BtlLinkedCommand *)action)->link->unit->flags & 0x400)) {
        func_001FA480(action, state, D_003BC090[CURSOR->unk_0C]);
        func_001FBAC0(action, state);
        func_001FB908(action, (BtlCamState *)state, 0, 1);
        CURSOR->frame++;
        CURSOR->frame = CURSOR->frame <= 0 ? 0 : CURSOR->frame >= 0x7FFF ? 0x7FFE : CURSOR->frame;
    }
}

extern void btlClearAllUnitDefeatCandidatesTask(void);

void btlInitLinkedUnitActionCursor(ActionStateLink *linkState) {
    BtlLinkedCommand *scene = (BtlLinkedCommand *)((u8 *)btlGetRuntime() + 0x70);
    scene->link = linkState;
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

/* vu0 routine: initialize the target cursor and orient flagged actors toward its center. */
void btlInitTargetCursorAndFacing(BtlLinkedCommand *action, void *state) {
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
    func_001F5868((s32)action, (s32)state, 2, 3);
    func_001F5320((s32)action, (s32)state, 0, 0);
    func_001FB908((s32)action, (BtlCamState *)state, 0, 1);
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
        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
        if (target->flags & 0x400) {
            func_00208000(0x400, 0, 0);
        } else {
            func_00208000(0x200, 0, 0);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlAimHorizontalDirectionVU((s128 *)aimPosition, (s128 *)position);
        VU0_STORE_VF_UNCLOBBERED(vf10, rotation);
        btlSetUnitRotation(unit, (s128 *)rotation);
    }
}

s32 btlInitCursorAndApplyAction(s32 action, s32 state) {
    s32 result;
    memset(D_003BD7D0, 0, 0x130);
    func_001F5868(action, state, 2, 6);
    func_001F5320(action, state, 0, 0);
    result = func_001FB908(action, (BtlCamState *)state, 0, 1);
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

extern BtlFieldArchiveRequest *fileQueuePlainDispatchRequest(const char *);

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
        args->request = fileQueuePlainDispatchRequest(path);
        args->fieldF1 = NULL;
        args->fieldF2 = NULL;
        args->fieldTB = NULL;
    } else {
        if (args->request != NULL && fileRequestIsReady(args->request)) {
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
                                             args->fieldF1, args->fieldF2, args->fieldTB, 0);
            btlInitializeSceneLightingAndTint();
            if (blocks->fieldTB != 0) {
                sdfQueueNonzeroResourceId(blocks->fieldTB);
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
    task->startCondition.kind = 1;
    task->taskId = 1;
    task->flags &= ~1;
    task->callback = btlPollFieldArchiveLoad;
    task->endCondition.kind = 0;
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

BtlRuntimeTask *btlCreateFloorLoadTask(s32 first, s32 second) {
    BtlRuntimeTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->taskId = 2;
    task->flags &= ~1;
    task->callback = btlPollFloorLoadTask;
    task->endCondition.kind = 0;
    args = btlGetTaskArguments(task);
    args->unk_08 = first;
    args->unk_0C = second;
    args->value = 0;
    args->option = 0;
    args->unk_10 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00200FB8);

extern u32 func_00200FB8(u32 *);

BtlRuntimeTask *btlCreateEffectTaskWithSourceParams(u8 *source, u32 value) {
    BtlRuntimeTask *task = btlAllocTask(0x34);
    u8 *arguments;
    task->startCondition.kind = 1;
    task->taskId = 3;
    task->flags |= 2;
    task->callback = func_00200FB8;
    task->endCondition.kind = 0;
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

s32 func_00201268(SceneLightRestoreArgs *args) {
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
                func_0023C870(unit->ext, args->value, firstColor, secondColor);
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
    task->startCondition.kind = 1;
    task->taskId = 4;
    task->flags |= 2;
    task->callback = func_00201268;
    task->endCondition.kind = 0;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

s64 func_00201520(SceneLightRestoreArgs *args) {
    D_00436AD4 = 1;
    return func_00201268(args);
}

BtlRuntimeTask *btlCreateSoundUpdateTask(u32 value) {
    BtlRuntimeTask *task = func_002014A8(value);
    task->taskId = 7;
    task->callback = func_00201520;
    return task;
}

s32 btlQueueTintTransitionWhenEnabled(u32 *taskArgs) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (!(work->battleFlags & 0x20000000)) {
        btlQueueTintTransition(taskArgs[0], *(u16 *)(taskArgs + 1));
    }
    btlTintTransitionHoldCount++;
    return 1;
}

BtlRuntimeTask *sndCreateAcquireTask(s32 value, s32 option) {
    BtlRuntimeTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->taskId = 5;
    task->callback = btlQueueTintTransitionWhenEnabled;
    task->endCondition.kind = 0;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
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
    if ((((BtlState *)context)->battleFlags & 0x20000000) != 0) {
        return 1;
    }
    btlQueueTintTransitionToZero(*soundId);
    return 1;
}

extern s32 sndTickFadeCounter();

BtlRuntimeTask *sndCreateReleaseTask(value)
    u32 value;
{
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->taskId = 6;
    task->callback = sndTickFadeCounter;
    task->endCondition.kind = 0;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

s64 func_00201718(void) {
    btlTintTransitionHoldCount = 1;
    return sndTickFadeCounter();
}

BtlRuntimeTask *btlCreateSoundReleaseTask(void) {
    BtlRuntimeTask *task = (BtlRuntimeTask *)sndCreateReleaseTask();
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

INCLUDE_ASM(const s32, "game/code_001DD390", func_00201828);

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

INCLUDE_ASM(const s32, "game/code_001DD390", func_00201C98);

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

extern s32 func_00201C98(SoundEffectReferenceArgs *);

BtlRuntimeTask *btlCreateReferencedSoundEffectTask(SoundResourceNode *effect, BtlUnit *source,
                                                 BtlUnit *actor, u16 frames) {
    s32 v1 = 0;
    s32 v2;
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundEffectReferenceArgs));
    SoundEffectReferenceArgs *args;
    BtlState *work;
    ActorEffectOwner sourceOwner;
    sourceOwner.unit = source;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x2E;
    task->flags |= 2;
    task->ownerId = actor->owner;
    task->onStart = sndAddEffectReferences;
    task->callback = func_00201C98;
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
    SoundVoice *effect;
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

extern u32 effBattleGetCurrentFrame(SoundVoice *);
extern void effBTLFieldColorSetSelectors(s32, u32, s32, s32);

s32 func_00202100(ActorEffectTaskArgs *args) {
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
    if (unit->flags & 4) {
        args->effect->flags |= 8;
    } else {
        args->effect->flags &= ~8;
    }
    if (unit->flags & 2) {
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

    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x2F;
    task->flags |= 2;
    task->ownerId = owner->owner;
    task->onStart = sndStartEffectTask;
    task->callback = func_00202100;
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
    TimedUnitEffectArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
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
    args->loadHandle = fileQueueDefaultCallbackRequest(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

s32 sndPollEffectLoad(EffectLoadArgs *args) {
    SoundResourceNode *effect = args->effect;
    s32 resource;
    if (effect->flags & 2) {
        return 1;
    }
    if (fileIsRequestReadyInCurrentMode(args->loadHandle) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:effect load end[%s]\n", args->name);
    resource = fileGetResourceHandle(args->loadHandle);
    effect->resourceHandle = sndMixerClone(sdfResourceRetainAddress(resource));
    sdfReleaseResourceAllocation(resource);
    filePollEntryCleanup(args->loadHandle);
    effect->flags = (effect->flags & ~1) | 2;
    return 0;
}


BtlRuntimeTask *sndCreateEffectLoadTask(SoundResourceNode *effect, char *name) {
    BtlRuntimeTask *task = btlAllocTask(strlen(name) + sizeof(EffectLoadArgs));
    EffectLoadArgs *args;
    char *copy;
    task->startCondition.kind = 1;
    task->taskId = 0x32;
    task->flags &= ~1;
    task->onStart = sndBeginEffectLoad;
    task->callback = sndPollEffectLoad;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->taskId = 0x33;
    task->callback = btlWaitUnitListIdle;
    task->endCondition.kind = 0;
    args = btlGetTaskArguments(task);
    args->value = value;
    return task;
}

u32 sndApplyToActiveActors(taskArgs)
    s32 *taskArgs;
{
    BtlUnit *unit;
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != 0; unit = unit->nextActor) {
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

BtlRuntimeTask *btlCreateApplyToActiveActorsTask(u32 value) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->taskId = 0x34;
    task->callback = sndApplyToActiveActors;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->callback = btlCancelTimedFadeTask;
    task->taskId = 0x35;
    task->endCondition.kind = 0;
    return task;
}

void sndAddSourceReferences(SoundEffectSourceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *unit;
    args->effect = 0;
    sndCreateSystemEffect(args->source);
    effect = args->source;
    unit = args->unit;
    effect->referenceCount = effect->referenceCount + 1;
    unit->effectLink.referenceCount = unit->effectLink.referenceCount + 1;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00202958);

void sndFinishEffectSourceTask(SoundEffectSourceArgs *args) {
    SoundResourceNode *effect;
    BtlUnit *unit;

    if (args->effect != 0) {
        effReleaseBattleVoiceOwner(args->effect);
    }
    effect = args->source;
    unit = args->unit;
    effect->referenceCount = effect->referenceCount - 1;
    unit->effectLink.referenceCount = unit->effectLink.referenceCount - 1;
    sndDeleteSystemEffect(effect);
}

extern s32 func_00202958(SoundEffectSourceArgs *);

BtlRuntimeTask *sndCreateEffectSourceTask(SoundResourceNode *effect, BtlUnit *owner, u64 resource) {
    BtlRuntimeTask *task = btlAllocTask(sizeof(SoundEffectSourceArgs));
    SoundEffectSourceArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x30;
    task->flags |= 2;
    task->ownerId = owner->owner;
    task->onStart = sndAddSourceReferences;
    task->callback = func_00202958;
    task->onFinish = sndFinishEffectSourceTask;
    args = btlGetTaskArguments(task);
    args->source = effect;
    args->unit = owner;
    args->resource = resource;
    args->effect = 0;
    args->duration = 0;
    args->counter = 0;
    return task;
}

u32 btlTaskStartFadeIn(u32 *taskArgs) {
    kwlnFadeStartIn(*taskArgs);
    return 1;
}

BtlRuntimeTask *btlCreateFadeInTask(u32 value) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->taskId = 0x38;
    task->callback = btlTaskStartFadeIn;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->taskId = 0x39;
    task->callback = btlTaskStartCustomFadeIn;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->callback = btlTaskSetBattleFlag40000;
    task->taskId = 0x3A;
    task->onStart = 0;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->callback = btlTaskClearBattleFlag40000;
    task->taskId = 0x3B;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_00202EA8);

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


extern void sdfReleaseChipBlock(void *);

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
    link->flags = 0;
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

INCLUDE_ASM(const s32, "game/code_001DD390", func_002034A8);

void btlMarkTaskReady(SoundResourceLink *resource) {
    resource->refreshRequested = 1;
}

SoundLink *sndAllocLink(BtlUnit *owner) {
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
        link->flags = effectId;
    } else if (link->effectHandle != 0) {
        effReleaseBattleVoiceOwner(link->effectHandle);
        link->effect->referenceCount--;
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
    task->startCondition.kind = 1;
    task->callback = btlDisableBattleFade;
    task->taskId = 0x36;
    task->onStart = 0;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->callback = btlEnableBattleFade;
    task->taskId = 0x37;
    task->onStart = 0;
    task->endCondition.kind = 0;
    return task;
}

/* Consume archive records only for enabled SYSEFF rows; clear unavailable entries. */
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
    s32 archive = fileQueuePlainDispatchRequest(path);
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
    for (; i < BTL_SOUND_ENTRY_COUNT; i++) {
        D_003BDE18[i].unk4 = 0;
        D_003BDE18[i].unk8 = 0;
    }
    func_002C7CE8(archive);
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
    u32 handle;
    void *actor;
} SoundHandleNode;

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(8);
    SoundEntry *entry = &D_003BDE18[index];
    node->actor = actor;
    node->handle = (u32)fileCloneQueueEntries((struct FileQueue *)entry->unk4);
    return node;
}


extern void fileQueueSetPosition(s32, f32 *);

extern void fileQueueUpdate(s32);

void btlUpdateJobPositionFromModel(s32 *args) {
    f32 pos[4];
    if (sdfLoadMapRecordPositionVector((SdfTextParam *)((MdlCtx *)args[1])->inner, 1) == 0) {
        mdlLoadPrimaryVectorVU((MdlCtx *)args[1]);
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
    task->startCondition.kind = 1;
    task->taskId = 0x5A;
    task->callback = sndPlayStationedSe;
    task->endCondition.kind = 0;
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
    task->startCondition.kind = 1;
    task->taskId = 0x57;
    task->callback = sndPlaySkillSeTask;
    task->endCondition.kind = 0;
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

typedef struct FileLoadArgs {
    SoundFileNode *node;
    void *loadHandle;
    s32 resourceHandle;
    s32 frames;
    const char *name;
} FileLoadArgs;

void sndStartFileLoad(u32 arguments) {
    FileLoadArgs *args = (FileLoadArgs *)arguments;
    SoundFileNode *node = args->node;
    args->loadHandle = fileQueueDefaultCallbackRequest(args->name);
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
                btlBossDebugPrintf("btl:sound SPU load start[%X][size:%d]\n", (u16)(node->position >> 16), size);
            }
            node->flags = (node->flags & ~1) | 2;
        }
    } else if (sndFindPackedTrackLoadStatus(node->position) != 0) {
        btlBossDebugPrintf("btl:sound SPU load end[%X]\n", (u16)(node->position >> 16));
        sdfReleaseResourceAllocation(request->resourceHandle);
        filePollEntryCleanup((s32)request->loadHandle);
        node->flags = (node->flags & ~8) | 0x10;
        return 1;
    }
    return 0;
}

BtlRuntimeTask *sndCreateFileLoadTask(s32 value, s32 option, char *name) {
    BtlRuntimeTask *task = btlAllocTask(strlen(name) + sizeof(FileLoadArgs));
    FileLoadArgs *args;
    char *copy;
    task->startCondition.kind = 1;
    task->taskId = 0x58;
    task->flags &= ~1;
    task->onStart = sndStartFileLoad;
    task->callback = sndPollMotSeFileAndSpu;
    task->endCondition.kind = 0;
    args = btlGetTaskArguments(task);
    copy = (char *)(args + 1);
    args->node = (SoundFileNode *)value;
    args->frames = option;
    args->name = copy;
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

BtlRuntimeTask *sndCreateDataFileLoadTask(BtlUnit *unit) {
    BtlRuntimeTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = sndLoadDataFile;
    task->taskId = 0x5B;
    task->ownerId = unit->owner;
    task->onStart = 0;
    args = btlGetTaskArguments(task);
    args->actor = unit;
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

void sndFormatResourceNameFromIndex(s32 source, s32 output) {
    func_0035C860(output, D_004192D8, D_00436AE8, (u16)(source + 0x200));
}

void sndFormatResourceNameFromUnitMode(s32 unit, s32 output) {
    func_0035C860(output, D_004192E8, ((BtlUnit *)unit)->partyRecord.unitId);
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
        u32 id = sndBuildMotSeResourceKey(sound, slot);
        if (id != 0) {
            if (slot != 0xB) {
                func_0035C860(filename, D_004192D8, D_00436AE8, id >> 16);
            } else if (sound[1] == 0) {
                func_0035C860(filename, D_004192F8, D_00419308, sound[2]);
            } else {
                func_0035C860(filename, D_00419318, D_00419308, sound[2]);
            }
            *(u32 *)(handleTable + offset) = fileQueueDefaultCallbackRequest(filename);
            btlBossDebugPrintf("btl:motSE file load start[%d][%p][%s]\n", slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound[0] |= 1;
}

/* Find the newest registered owner with both keys equal; return null if absent. */
void *sndFindListNodeForChannel(s32 category, s32 id) {
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
        sndLoadMotSeFiles((u32 *)owner);
    }
    return owner;
}

extern s32 filePollEntryCleanup(s32);

extern void sdfReleaseResourceAllocation(u32);

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
        if ((node->flags & 8) != 0) {
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
    if (owner->flags & 1) {
        return;
    }
    if (!(owner->flags & 2)) {
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
        owner->flags |= 4;
        owner->flags &= ~8;
        owner->flags &= ~0x10;
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
    if (owner->flags & 1) {
        return 1;
    }
    if (!(owner->flags & 2)) {
        return 1;
    }
    soundWork = &owner->work;
    if (soundWork->resourceHandles[args->unk_08] == 0) {
        return 1;
    }
    if (args->unk_08 != 0xB) {
        key = sndBuildMotSeResourceKey((u32 *)owner, args->unk_08);
        if (owner->flags & 0x10) {
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
            data = sdfMemoryGetBlockAddress(soundWork->resourceHandles[args->unk_08]);
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
        owner->flags &= ~8;
        owner->flags |= 0x10;
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
    task->endCondition.kind = 0;
    task->startCondition.kind = 1;
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

INCLUDE_RODATA(const s32, "game/code_001DD390", D_00419408);

INCLUDE_ASM(const s32, "game/code_001DD390", func_00205160);

/* Return whether a not-yet-file-ready owner still has an outstanding request. */
s32 sndHasOccupiedNodeSlots(void) {
    SoundSlotOwner *owner;
    u32 i;
    for (owner = ((BtlState *)btlGetRuntime())->soundSlotOwners; owner != 0; owner = owner->next) {
        if (!(owner->flags & 2)) {
            for (i = 0; i < 0x1D; i++) {
                if (owner->work.fileRequests[i] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 sndWaitForEarringPlayback(void) {
    s32 ready;
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

BtlRuntimeTask *sndCreateEarringTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = sndWaitForEarringPlayback;
    task->taskId = 0x5C;
    task->endCondition.kind = 0;
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
        args->loadHandle = (s32)fileQueueDefaultCallbackRequest(path);
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

BtlRuntimeTask *sndCreateAtracEffectLoadTask(s32 value) {
    BtlRuntimeTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->taskId = 0x5D;
    task->flags &= ~1;
    task->callback = sndPollAtrac3SELoadTask;
    task->endCondition.kind = 0;
    args = btlGetTaskArguments(task);
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
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    s32 id;
    char path[0x70];
    if (work->earringPlaybackCount == 0) {
        if (mnuGetSoundBufferStateLocked() != 0) {
            mnuReleaseSoundBufferLocked();
        }
        unit = args->unit;
        if (unit->flags & 0x200) {
            id = unit->partyRecord.unitId;
            if (unit->partyRecord.flags & 0x10) {
                id += 0x20;
            } else if (unit->flags & 0x1000) {
                id += 0x10;
            }
            func_0035C860(path, D_004192F8, D_00419308, id);
        } else {
            func_0035C860(path, D_00419318, D_00419308, unit->partyRecord.unitId);
        }
        args->handle = fileQueueDefaultCallbackRequest(path);
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
    ((BtlState *)work)->earringPlaybackCount += 0xFFFF;
}

BtlRuntimeTask *sndCreateEarringPlaybackTask(BtlUnit *owner) {
    BtlRuntimeTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->taskId = 0x5E;
    task->flags &= ~1;
    task->ownerId = owner->owner;
    task->onStart = sndStartDeadAtracLoad;
    task->callback = sndDeadAtracPlaybackTask;
    task->onFinish = sndFinishEarringPlaybackTask;
    args = btlGetTaskArguments(task);
    args->actor = owner;
    args->option = 0;
    args->unk_08 = 0;
    return task;
}

s32 btlPlayStationedSe1C(void) {
    sndLoadAndPlayStationedSe(0x1c);
    return 1;
}

BtlRuntimeTask *btlCreateStationedSe1CTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = btlPlayStationedSe1C;
    task->taskId = 0x5F;
    task->endCondition.kind = 0;
    return task;
}

s32 btlAdvanceTitleStateAfterSound(void) {
    btlAdvanceTitleState();
    return 1;
}

BtlRuntimeTask *btlCreateAdvanceTitleStateTask(void) {
    BtlRuntimeTask *task = btlAllocTask(0);
    task->startCondition.kind = 1;
    task->callback = btlAdvanceTitleStateAfterSound;
    task->taskId = 0x60;
    task->endCondition.kind = 0;
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

void btlMoveOtherUnitsAway(ActionStateLink *link) {
    f32 pos[4];
    BtlUnit *other = ((BtlState *)btlGetRuntime())->units;
    u32 mask = link->unit->flags & 0x600;
    for (; other != NULL; other = other->nextActor) {
        if ((other->flags & 1) && (other->flags & mask) && other != link->unit) {
            btlClearUnitDefeatCandidate(other);
            if ((btlUnitStatusPair(other) & 0x102) == 0x102) {
                func_001E3108(other, pos);
                pos[1] += 1000000.0f;
                pos[0] = 0;
                effObjSetInnerFirstVec(other->effectObject, pos);
                btlSetUnitPosition(other, pos);
            }
        }
    }
    func_001E3108(link->unit, pos);
    pos[0] = 0;
    btlSetUnitPosition(link->unit, pos);
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);
extern f32 sdfSinPoly(f32);

/* Place the three actor slots around the common battle center supplied in vf10. */
void btlPlaceTripleFormationAroundCenter(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
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
    radius = func_00208000(0x400, 0, 0) + 200.0f;
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

void btlPlaceTripleFormationAroundTarget(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 radius;
    BtlUnit *target;
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
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
        VU0_LOAD_VF(vf10, target->orientation);
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

void func_00206570(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 center[4];
    f32 pos[4];
    f32 dir[4];
    f32 rot[4];
    BtlUnit *unit;
    f32 radius;
    u32 i;
    BtlState *work = (BtlState *)btlGetRuntime();
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
        for (unit = work->units; unit != NULL; unit = unit->nextActor) {
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

void btlOrientFrontAndBackUnitsTowardTargets(ActionStateLink *link, BtlUnit *a, BtlUnit *b) {
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
    count = btlGetIndexListCount(link->indexWork.indices);
    target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
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

extern s32 btlGetBossSceneStateWhenActive(void);

/* vu0 routine: measure the center actor's displacement from its sole target. */
void btlAlignTripleFormationWithTarget(ActionStateLink *link, BtlUnit *first, BtlUnit *second) {
    BtlUnit *slot[3];
    f32 position[4];
    f32 targetPosition[4];
    BtlState *work;
    BtlUnit *target;
    f32 offsetX;
    f32 offset;
    u32 i;

    work = (BtlState *)btlGetRuntime();
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        target = (BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0);
        btlFlagAllUnitsDefeatCandidate();
        btlClearMatchingUnitDefeatCandidates(target->flags & 0x600);
        btlFlagUnitDefeatCandidate(target);
        slot[0] = NULL;
        slot[1] = NULL;
        slot[2] = NULL;
        slot[link->unit->lookupId] = link->unit;
        slot[first->lookupId] = first;
        slot[second->lookupId] = second;
        if ((work->commandRestrictFlags & 0x400000) != 0 &&
            btlGetBossSceneStateWhenActive() == 0) {
            switch (target->partyRecord.unitId) {
            case 0x111:
                offset = -550.0f;
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 350.0f;
                btlSetUnitPosition(slot[0], position);
                btlUnitFaceTarget(slot[0], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 120.0f;
                btlSetUnitPosition(slot[1], position);
                btlUnitFaceTarget(slot[1], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = -110.0f;
                btlSetUnitPosition(slot[2], position);
                btlUnitFaceTarget(slot[2], target);
                return;
            case 0x112:
                offset = 550.0f;
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 350.0f;
                btlSetUnitPosition(slot[0], position);
                btlUnitFaceTarget(slot[0], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = 120.0f;
                btlSetUnitPosition(slot[1], position);
                btlUnitFaceTarget(slot[1], target);
                position[0] = offset;
                position[1] = 0.0f;
                position[2] = -110.0f;
                btlSetUnitPosition(slot[2], position);
                btlUnitFaceTarget(slot[2], target);
                return;
            }
        }
        btlUnitGetMuzzlePosVU(slot[1]);
        VU0_STORE_VF_UNCLOBBERED(vf10, position);
        btlUnitGetMuzzlePosVU(target);
        VU0_STORE_VF_UNCLOBBERED(vf10, targetPosition);
        VU0_LOAD_VF(vf11, position);
        VU0_SUB(vf10, vf10, vf11);
        VU0_GET_VF10_X(offsetX);
        targetPosition[2] -= target->reach * target->scale;
        offset = -(500.0f - (targetPosition[2] - position[2]));
        if (work->commandRestrictFlags & 0x200) {
            offset = 0.0f;
        }
        for (i = 0; i < 3; i++) {
            func_001E3108(slot[i], position);
            position[0] += offsetX;
            position[2] += offset;
            btlSetUnitPosition(slot[i], position);
            btlUnitFaceTarget(slot[i], target);
        }
    }
}

void func_00206C10(void) {
}

extern s128 D_003BE0A0;

void func_00206C18(ActionStateLink *link, BtlUnit *other) {
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
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (unit != slot[0] && unit != slot[1]) {
                    btlClearUnitDefeatCandidate(unit);
                    btlSetUnitPosition(unit, (f32 *)&D_003BE0A0);
                    if ((btlUnitStatusPair(unit) & 0x102) == 0x102) {
                        PCP_COPY_VECTOR(pos, &D_003BE0A0);
                        pos[1] += 1000000.0f;
                        effObjSetInnerFirstVec(unit->effectObject, pos);
                    }
                }
            }
        }
    }
    if (btlGetIndexListCount(link->indexWork.indices) == 1) {
        btlUnitGetMuzzlePosVU((BtlUnit *)btlGetIndexListEntry(link->indexWork.indices, 0));
    } else {
        func_00208750(link->indexWork.indices, 0, 0);
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

s32 btlMoveOtherUnitsForCategory(u32 *command) {
    s32 category = command[1];
    if (category >= 0x1AB) {
        return 1;
    }
    if (category >= 0x5E) {
        return 1;
    }
    if (category >= 0x5B) {
        btlMoveOtherUnitsAway((ActionStateLink *)command[0]);
    }
    return 1;
}

BtlRuntimeTask *btlCreateMoveOtherUnitsTask(ActionStateLink *link, s32 option, s32 target) {
    BtlRuntimeTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = btlMoveOtherUnitsForCategory;
    task->taskId = 0x64;
    task->onStart = 0;
    task->ownerId = link->unit->owner;
    args = btlGetTaskArguments(task);
    args->actor = link;
    args->option = option;
    args->unk_08 = target;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DD390", func_002077C0);
extern s32 func_002077C0(u32 *);

BtlRuntimeTask *btlCreateSoundPlaybackTask(ActionStateLink *link, u32 soundId, u32 variant, u32 channel, u32 flags) {
    BtlRuntimeTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->startCondition.kind = 1;
    task->endCondition.kind = 0;
    task->callback = func_002077C0;
    task->taskId = 0x65;
    task->onStart = 0;
    task->ownerId = link->unit->owner;
    args = btlGetTaskArguments(task);
    args->actor = link;
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

