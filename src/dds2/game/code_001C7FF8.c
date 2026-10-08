#include "common.h"
#include "btl_scene_fade.h"
#include "btl_command.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "pcp_vu0.h"
#include "dat_state.h"
#include "kwln.h"
#include "eff.h"
#include "btl_resource.h"

extern BtlResBlock *btlResourceBlock;
extern void func_00306C28(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

extern SceneSlotFadeWork *D_00438F54;
extern ActorSlotOrder *D_00438F58[2];

extern s32 btlGetRuntime(void);

extern s32 kwlnTaskGetTaskByName(const char *);

extern u32 func_0019F5E8(s32, s32, s32, u32, char *, s32);
extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern s32 kwlnTaskCreate(const char *, s32, s32, s32, TaskUpdate, TaskDestroy, s32);

extern void func_00101968(s32, s32);

extern s32 kwlnTaskDestroyWithHierarchy(s32, s32);

extern s32 btlGetTrackedTaskHandle(s32);

extern s32 btlIsNamedBattleTaskRegistered(void);

extern s32 btlHasRegisteredGuidePanelTask(void);

extern s32 btlHasRegisteredSkillNamePanelTask(void);

extern s32 btlHasRegisteredAphNamePanelTask(void);

extern s32 btlCreateAiWork(s32);

extern u32 fldGetSceneScriptTaskUserData(void);

extern char *D_004367B8;

extern char *btlCommandPanelTaskNameRef;

extern void func_001C7DB8(s32, s32);

extern u32 kwlnTaskGetUserValue();

typedef struct RosterAvailability {
    u8 flags;
    u8 pad[7];
} RosterAvailability;

typedef struct SceneControl {
    u8 mode;
    s8 kind;
} SceneControl;


extern BattleSceneObject *fldGetSceneObjectTaskUserData(void);




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
    ActionStateLink *groupPrimary[20];    /* 0x318 */
    ActionStateLink *groupSecondary[45];  /* 0x368 */
    ActionStateLink *groupTertiary[15];   /* 0x41C */
    ActionStateLink *groupHandles[8];     /* 0x458 */
    u16 groupHandleCount;
    u8 pad47A[2];
    s32 activeGroupCount;
    BtlSceneFadingRecord fading[8];
    ActionStateLink *currentTask;
    u8 pad4C4[0x10];
    s32 scriptTarget;         /* 0x4D4 */
    u8 pad4D8[0xC];
    u32 values[64];
    s32 (*sceneCallback)();
} BattleSceneWork;

extern SceneControl *btlCommandPanelWork;

extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;

extern u32 func_001C82D8(s32, s8);

extern u32 btlCountFlaggedSceneActors(void);

extern s32 func_001AC750(s32, void *);

extern s32 D_004367C0;

extern char D_003B5D10[];

extern u8 D_003B5B10[];

extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);

extern u8 *D_00435E64;

extern u8 *D_00435E5C;






extern s32 datItemSkillRecords;

void fldInitializeBattleSceneFlow(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    btlNextScaledRandom(7);
    if ((scene->flags & 0x400) != 0) {
        if (scene->variant == 1) {
            btlInvalidateSceneFadeCounts(0);
            btlResetBattleHistoryCounters();
        } else {
            btlInvalidateSceneFadeCounts(1);
            btlResetBattleHistoryCounters();
        }
    }
    btlToggleModelFlagOnInput();
    btlUpdateCommandUiTransition();
}

void btlDebugPrintf(s32 tag, ...) {
}

void func_001C80C0(void) {
}

void func_001C80C8(void) {
}

/* Submit a scene object at fixed-point screen coordinates and retire its handle. */
void fldSubmitSceneObjectAtCoordinates(s32 x, s32 y, u32 color, char *text) {
    u32 glyph;

    itfSetTextDrawLimit(0x13);
    glyph = func_0019F5E8(x << 4, y << 3, 0, color, text, 0);
    func_0019D550(glyph, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(glyph);
    itfSetTextDrawLimit(-1);
}

void btlDrawIndexedBattleEntryGlyphs(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_00435E64 + index * 17, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

void btlQueueIndexedTextWithinDrawLimit(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_00435E5C + index * 25, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

s32 btlIsSceneActorCountWithinLimit(s32 unused, u32 limit) {
    btlGetRuntime();
    if (limit < btlCountFlaggedSceneActors()) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C82D8);

typedef struct SceneKindTable {
    u8 pad00[0x14];
    u32 value[8];
} SceneKindTable;

extern SceneKindTable *D_00438F4C;

extern void func_001C8518();

extern void fldCollectAvailableRosterEntries(s32, s16 *);

extern char *fldGetCachedSceneActorNameAndId(s32, s16 *);

/* Advance the per-kind scene counter table: refresh the slot for the current
 * scene kind, then return its value (clamped to 4 unless noClamp is set). */
u32 fldUpdateSceneKindCounter(s32 ctx, s8 kind, s8 noClamp) {
    s16 buf[8];
    u32 type = func_001C82D8(ctx, kind);
    if (type != 4) {
        D_004367C0 = 0;
    }
    switch (type) {
    case 0:
        func_001C8518(ctx, buf, 0, 2, 3);
        D_00438F4C->value[0] = buf[0] + 1;
        break;
    case 4:
        fldGetCachedSceneActorNameAndId(ctx, buf);
        D_00438F4C->value[4] = buf[0];
        break;
    case 2:
        fldCollectAvailableRosterEntries(ctx, buf);
        D_00438F4C->value[2] = buf[0];
        break;
    case 3:
        D_00438F4C->value[3] = 3;
        break;
    case 6:
        D_00438F4C->value[6] = 1;
        break;
    default:
        D_00438F4C->value[type] = 0;
        break;
    }
    if (noClamp == 0) {
        if (D_00438F4C->value[type] >= 4) {
            return 4;
        }
    }
    return D_00438F4C->value[type];
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8518);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8768);

/* Pack available roster IDs and their values into consecutive byte pairs. */
void fldCollectAvailableRosterEntries(s32 unused, s16 *count) {
    u8 *roster;
    RosterAvailability *availability;
    u8 *output;
    s32 found = 0;
    s32 i = 0;
    btlGetRuntime();
    roster = datGameState->inventory.counts;
    availability = (RosterAvailability *)datItemSkillRecords;
    output = D_003B5B10;
    do {
        if (*roster != 0 && (availability->flags & 2) != 0) {
            output[0] = i;
            found++;
            output[1] = *roster;
            output += 2;
        }
        i++;
        availability++;
        roster++;
    } while (i < 0x100);
    *count = found;
}

/* Cache the resolved entry ID while returning the shared name buffer. */
char *fldGetCachedSceneActorNameAndId(s32 object, s16 *outId) {
    s32 cachedId = D_004367C0;
    if (cachedId == 0) {
        cachedId = func_001AC750(*(s32 *)(*(s32 *)(object + 0x2C) + 0x18), D_003B5D10);
        D_004367C0 = cachedId;
    }
    *outId = cachedId;
    return D_003B5D10;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8A80);

extern void func_001C92A0(s32, s32, s32, s32);

extern void func_001C9EA0(s32);

extern void func_001C9BE8(s32);

extern void func_001C98E8(s32);

extern void btlDrawRetreatCommandLabel(s32);

void fldDispatchSceneKindHandler(s32 sceneContext) {
    switch (func_001C82D8(sceneContext, btlCommandPanelWork->kind)) {
    case 0:
        func_001C92A0(sceneContext, 0, 2, 3);
        return;
    case 4:
        func_001C9EA0(sceneContext);
        return;
    case 2:
        func_001C9BE8(sceneContext);
        return;
    case 3:
        func_001C98E8(sceneContext);
        return;
    case 6:
        btlDrawRetreatCommandLabel(sceneContext);
    }
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169C0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C92A0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C98E8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9BE8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436858);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436860);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436868);

void btlDrawRetreatCommandLabel(s32 unused) {
    u8 text[8] = "Retreat";
    s32 color;
    s32 handle;
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();

    if (datBattleSceneRecords[scene->mode].unk00 != 0) {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x504F6100;
    } else {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x89FEFF00;
    }
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(0x1A0, 0xA60, 0xFF0010, color, text, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9EA0);

typedef struct SceneCheckArgs {
    u16 mode;
    u16 pad2;
    s32 first;
    s32 second;
} SceneCheckArgs;

extern s32 func_001ABDE8();

extern s32 func_001ABA40();

/* Check helper for a pair of scene objects: the caller's own result wins,
 * then the first object's, then the second's. */
s32 btlCheckScenePairResult(s32 self, SceneCheckArgs *args) {
    s32 base;
    s32 resultA = 0;
    s32 resultB = 0;
    base = func_001ABDE8(self, self, args->first, args->second, args->mode);
    if (args->first != 0) {
        resultA = func_001ABDE8(args->first, args->first, self, args->second, args->mode);
        if (resultA == 6 && base == 0) {
            if (func_001ABA40(args->first, args->mode) == 0) {
                resultA = 0;
            }
        }
    }
    if (args->second != 0) {
        resultB = func_001ABDE8(args->second, args->second, self, args->first, args->mode);
        if (resultB == 6 && base == 0) {
            if (func_001ABA40(args->second, args->mode) == 0) {
                resultB = 0;
            }
        }
    }
    if (base != 0) {
        return base;
    }
    if (resultA != 0) {
        return resultA;
    }
    return resultB;
}



extern void func_001C0EF0(void);
extern void func_001C1300(void);
extern void func_001B5A20(BattleSceneObject *);
/* Task-update result: retail returns 0 or -1, even when this caller discards it. */
extern s32 func_001BFB58(KwlnTask *);
extern s32 func_001C8A80(BattleSceneObject *);
extern void func_001BD9A0(BattleSceneObject *, s32);
extern s32 btlHasHighPriorityState(void);
extern s32 btlAreLinkedSceneCountersAtThreshold(void);
extern char *D_004367CC;

/* The same native command-selection sequence occurs in four switch paths. */
static inline void btlUpdateSceneCommandSelection(BattleSceneObject *object) {
    fldDispatchSceneKindHandler((s32)object);
    func_001BD9A0(object, func_001C82D8((s32)object, btlCommandPanelWork->kind));
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169F0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A00);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A10);

s32 btlUpdateBattleSceneCommands(KwlnTask *task) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleSceneObject *object = (BattleSceneObject *)kwlnTaskGetUserValue(task);
    BattleActorPanelWork *panel;
    u8 slot;

    if (!(battle->battleFlags & 0x200)) {
        return 0;
    }
    func_001C0EF0();
    func_001C1300();
    func_001B5A20(object);
    func_001BFB58(task);
    switch (object->state) {
    case 5:
        return -1;
    case 1:
        func_001C8A80(object);
        btlUpdateSceneCommandSelection(object);
        if (btlHasHighPriorityState() && object->state == 1) {
            object->state = 2;
        }
        break;
    case 2:
        func_001C8A80(object);
    case 3:
    case 6:
    case 8:
    case 10:
    case 11:
        btlUpdateSceneCommandSelection(object);
        break;
    case 7:
        if (func_001C8A80(object)) {
            btlUpdateSceneCommandSelection(object);
        } else {
            btlUpdateSceneCommandSelection(object);
            if (btlAreLinkedSceneCountersAtThreshold()) {
                object->state = 2;
            }
        }
        break;
    case 9:
        btlUpdateSceneCommandSelection(object);
        if (btlAreLinkedSceneCountersAtThreshold()) {
            object->commandData->phase = 9;
            panel = (BattleActorPanelWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
            object->commandData->unk18 = panel->partyRecordIndex;
            object->state = 3;
            ((SceneScriptState *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367B8)))->state = 3;
            slot = object->commandData->linkedUnit->lookupId;
            panel->activeEntries[slot].presentation.unkF0 = 0;
            panel->activeEntries[slot].presentation.pendingSceneState = 5;
            panel->activeEntries[slot].presentation.hpState = 3;
            panel->activeEntries[slot].presentation.hpLevel = datGameState->party[panel->partyRecordIndex].hp;
            panel->activeEntries[slot].presentation.hpTarget = panel->activeEntries[slot].presentation.hpLevel;
            panel->activeEntries[slot].presentation.mpState = 3;
            panel->activeEntries[slot].presentation.mpLevel = datGameState->party[panel->partyRecordIndex].mp;
            panel->activeEntries[slot].presentation.mpTarget = panel->activeEntries[slot].presentation.mpLevel;
            panel->activeEntries[slot].presentation.presentationState = 2;
            panel->activeEntries[slot].presentation.presentationValue = 0;
        }
        break;
    }
    return 0;
}

extern void btlReleaseBattleScratchBlocks(void);
extern void sdfReleaseChipBlock(void *);

void fldClearBattleSceneObject(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    ((BattleSceneWork *)btlGetRuntime())->sceneObject = 0;
    btlReleaseBattleScratchBlocks();
}

void fldInitializeSceneObject(BattleSceneObject *object, ActionStateLink *owner) {
    memset(object, 0, sizeof(*object));
    object->state = 1;
    object->owner = owner;
    object->commandData = &owner->indexWork;
}

BattleSceneObject *fldGetSceneObjectTaskUserData(void) {
    u32 handle = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (handle == 0) {
        return (BattleSceneObject *)handle;
    }
    return (BattleSceneObject *)kwlnTaskGetUserValue(handle);
}

s32 fldGetSceneObjectState(void) {
    s32 handle = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (handle == 0) {
        return handle;
    }
    return fldGetSceneObjectTaskUserData()->state;
}

s32 btlHasSpecialActiveSceneActor(void) {
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((btlUnitStatusPair(actor) & 0x421) == 0x401) {
            u16 kind = actor->partyRecord.unitId;
            if (kind == 0x4C || kind == 0x3C) {
                return 1;
            }
        }
        actor = actor->nextActor;
    }
    return 0;
}

s32 func_001CA8D8(void) {
    u32 requiredFlags = 0x201;
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    s32 i;

    if (actor != 0) {
        do {
            if ((actor->flags & requiredFlags) == requiredFlags) {
                for (i = 0; i < 8; i++) {
                    if ((u16)(actor->partyRecord.effectData[i] - 0xE0) < 0x20) {
                        return actor->partyRecord.effectData[i];
                    }
                }
            }
            actor = actor->nextActor;
        } while (actor != 0);
    }
    return 0;
}

s32 btlHasSelectedActiveSceneActor(void) {
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((btlUnitStatusPair(actor) & 0x421) == 0x401 &&
            (actor->partyRecord.status & 1) != 0) {
            return 1;
        }
        actor = actor->nextActor;
    }
    return 0;
}

s32 btlHaveActiveSceneActorEntriesCleared(void) {
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((btlUnitStatusPair(actor) & 0x421) == 0x401 &&
            btlGetEntryFlagsUnlessDisabled(&actor->partyRecord) != 0) {
            return 0;
        }
        actor = actor->nextActor;
    }
    return 1;
}

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

s32 fldSelectSceneMode(ActionStateLink *task) {
    s32 flag = ((BattleSceneWork *)btlGetRuntime())->phaseFlag == 3;
    if (mdlFlagTest(0x801) != 0) {
        return 1;
    }
    if (mdlFlagTest(0x81D) == 0 && mdlFlagTest(0x801) == 0 && !(((SceneGlobalState *)btlTrackedTaskHandles)->flags & 0x200)) {
        if (btlHasSpecialActiveSceneActor() != 0) {
            return 2;
        }
    }
    if (mdlFlagTest(0x81A) == 0) {
        if (flag != 0) {
            return 6;
        }
    }
    if (flag == 0) {
        if (btlGetTaskState6() != 0) {
            return 5;
        }
    }
    if (mdlFlagTest(0x805) == 0 && !(((SceneGlobalState *)btlTrackedTaskHandles)->flags & 0x200)) {
        if (func_001CA8D8() != 0 && btlHaveActiveSceneActorEntriesCleared() != 0) {
            return 3;
        }
    }
    if (mdlFlagTest(0x805) != 0) {
        if (mdlFlagTest(0x806) == 0) {
            if (btlHasSelectedActiveSceneActor() != 0) {
                return 4;
            }
        }
    }
    return 0;
}

extern void func_001C35F0(s32, s32, s32);
extern void btlInitializeSelectionWork(void);
extern void btlInitializeCommandPanelSlotTables(void);
extern void btlCreateMessageWindow(void);
extern void func_001BD6E8(void *);
extern void btlBossDebugPrintf(const char *format, ...);
extern void mdlFlagClear(s32);
extern void mdlFlagSet(s32);
extern void btlSetTrackedTaskHandle(s32, s32);
extern s32 dspCloseChannel(void);
extern void evtCreateMessageWindowIfMissing(void *);
extern s32 dspStartEntry(s32);
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern s32 func_0035C860(char *, const char *, ...);
extern void *memset(void *, s32, u32);
extern s32 func_001C0630(KwlnTask *);
extern s32 func_001C0828(KwlnTask *);
extern s32 func_001C0240(KwlnTask *);
extern void btlReleaseDialogTaskAndMarkBattleState(KwlnTask *);
extern void btlFinishTrackedBattleTaskAndCloseWindow(KwlnTask *);
extern char *D_004367F0;
extern char *D_004367EC;
extern u8 D_003B52D0[];
extern u8 D_00385228[];
extern char D_00436828[];

/* Open the battle command panel task and, on the first eligible ally turn, start the matching tutorial dialog. */
void func_001CAB60(ActionStateLink *task) {
    char text[32];
    BattleSceneWork *scene;
    BattleSceneObject *object;
    s32 handle;

    if (kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef) == 0) {
        D_004367C0 = 0;
        scene = (BattleSceneWork *)btlGetRuntime();
        object = sdfAllocAndClearQuadwords(0x30);
        fldInitializeSceneObject(object, task);
        handle = kwlnTaskCreate(btlCommandPanelTaskNameRef, 0x2B0E, 1, 1, btlUpdateBattleSceneCommands, fldClearBattleSceneObject,
                                (s32)object);
        func_00101968(scene->taskParent, handle);
        scene->sceneObject = handle;
        func_001C35F0((s32)task, 0, 0);
        btlInitializeSelectionWork();
        btlInitializeCommandPanelSlotTables();
        btlCreateMessageWindow();
        func_001BD6E8(object);
        if ((task->unit->flags & 0x200) && !(scene->flags & 0x1000000)) {
            switch (fldSelectSceneMode(task)) {
            case 1:
                btlBossDebugPrintf("-----------------First Battle!!-------------------\n");
                mdlFlagClear(0x801);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x100;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (s32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_003B52D0);
                dspStartEntry(2);
                func_001C7DB8(0, 8);
                break;
            case 6:
                btlBossDebugPrintf("-----------------Tutorial Majin!!-------------------\n");
                mdlFlagSet(0x81A);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x300;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0828,
                                        btlReleaseDialogTaskAndMarkBattleState, (s32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                dspStartEntry(4);
                func_001C7DB8(0, 8);
                break;
            case 2:
                btlBossDebugPrintf("-----------------Tutorial Weak!!-------------------\n");
                mdlFlagSet(0x81D);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x300;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (s32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                dspStartEntry(1);
                func_001C7DB8(0, 8);
                break;
            case 3:
                btlBossDebugPrintf("-----------------Tutorial Hunt!!-------------------\n");
                mdlFlagSet(0x805);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x100;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (s32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                memset(text, 0, sizeof(text));
                func_0035C860(text, D_00436828, D_00435E64 + func_001CA8D8() * 17);
                evtCopyEntryStringToActiveWindow(0, text);
                dspStartEntry(2);
                func_001C7DB8(0, 8);
                break;
            case 4:
                btlBossDebugPrintf("-----------------Tutorial Fear!!-------------------\n");
                mdlFlagSet(0x806);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x100;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (s32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                dspStartEntry(3);
                func_001C7DB8(0, 8);
                break;
            case 5:
                btlBossDebugPrintf("-----------------New Linkage!!-------------------\n");
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x200;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367EC, 0x2B0E, 1, 1, func_001C0240,
                                        btlFinishTrackedBattleTaskAndCloseWindow,
                                        (s32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xC, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_003B52D0);
                func_001C7DB8(0, 8);
                break;
            default:
                scene->flags |= 0x100000;
                break;
            }
        }
    }
    object = fldGetSceneObjectTaskUserData();
    if (object->state == 3) {
        btlCommandPanelWork->mode = 2;
        object->state = 1;
    } else if (object->state == 8) {
        object->state = 6;
        btlCommandPanelWork->mode = 2;
    } else if (object->state != 0xB) {
        btlCommandPanelWork->mode = 1;
        object->state = 1;
    }
    btlLinkedSelectionTaskBuffer->selectedRow = fldUpdateSceneKindCounter((s32)object, btlCommandPanelWork->kind, 0);
}

void fldSetSceneObjectAndGroupStates(void) {
    BattleSceneObject *object = fldGetSceneObjectTaskUserData();
    if (object != 0) {
        object->state = 5;
        btlCommandPanelWork->mode = 3;
    }
}

extern s32 D_00416BB8[];

void fldGetSceneDirectionStepOffset(s32 *outX, s32 *outY, s32 dir, s32 step) {
    s32 offsets[3][8][2];

    memcpy(offsets, D_00416BB8, sizeof(offsets));
    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

void func_001CB278(SceneAiWork *work) {
    s32 i;
    switch (work->animationPhase) {
    case 0:
        for (i = 0; i < 3; i++) {
            work->rowFade[i] = 128;
            work->rowScale[i] = 80.0f;
            work->rowPhase[i] = 0;
            fldGetSceneDirectionStepOffset(&work->rowPosition[i][0], &work->rowPosition[i][1], i, 0);
        }
        break;
    case 4:
        break;
    case 5:
        for (i = 0; i < 3; i++) {
            switch (work->rowPhase[i]) {
            case 0:
                work->rowStep[i]++;
                work->rowStep[i] = work->rowStep[i] <= 0 ? 0 : work->rowStep[i] > 5 ? 5 : work->rowStep[i];
                fldGetSceneDirectionStepOffset(&work->rowPosition[i][0], &work->rowPosition[i][1], i, work->rowStep[i]);
                if (work->rowStep[i] >= 5) work->rowPhase[i]++;
                break;
            case 1:
                work->rowStep[i]--;
                work->rowStep[i] = work->rowStep[i] <= 0 ? 0 : work->rowStep[i] > 5 ? 5 : work->rowStep[i];
                if (work->rowStep[i] <= 0) work->rowPhase[i]++;
                break;
            case 2:
                break;
            }
        }
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416BB8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB498);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416C98);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416CC8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB7A8);

u32 func_001CC018(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D00);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC020);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC438);


void btlDrawCenteredPanelSegments(s32 width) {
    u32 color[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 half = width / 2;
    s32 x = width - half + 0x105;
    func_00306C28(x * 0x10, 0x200, 0, color, 0, btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = width << 4;
    func_00306C28((0x100 - half) * 0x10, 0x200, 0, color, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_00306C28((0x92 - half) * 0x10, 0x200, 0, color, 0, btlResourceBlock->resC, 0x15, 0x53);
}

extern s32 btlGetEffectActive();

extern void func_001CC020();

extern void func_001CC438();

s32 fldStepSceneStateMachine(KwlnTask *handle) {
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    s32 *state;
    s32 mode;
    if (work->flags & 0x04000000) {
        return 0;
    }
    state = (s32 *)kwlnTaskGetUserValue(handle);
    switch (*state) {
    case 1:
        *state = 2;
        break;
    case 2:
        mode = 1;
        if (work->mode == 0x312) {
            mode = btlGetEffectActive() == 1 ? 5 : 1;
        }
        func_001CC020(work, state, mode);
        func_001CC438(state);
        break;
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        return -1;
    }
    return 0;
}



extern void *sdfAllocAndClearQuadwords(s32);






extern void func_001AC0F8(s32 source, BtlIndexList *list, s32, s32, s32);

extern s32 func_001AC360(s32 source, BtlIndexList *list, s32);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 source, BtlIndexList *list);

/* Creates the AI work object for `source`: allocates two index lists and
 * fills them according to the current scene object state. */
s32 btlCreateAiWork(s32 source) {
    SceneAiWork *work;
    BattleSceneObject *object;
    BattleActorPanelWork *other;
    s32 id;
    s32 count;
    btlGetRuntime();
    work = (SceneAiWork *)sdfAllocAndClearQuadwords(0xA4);
    object = (BattleSceneObject *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef));
    work->listA = btlAllocateIndexList(0xD);
    work->listB = btlAllocateIndexList(0xD);
    if (object->state == 8) {
        other = (BattleActorPanelWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
        count = btlCountFlaggedSceneActors();
        if (count < 2 && (datGameState->party[other->partyRecordIndex].status & 0x4800)) {
            func_001AC0F8(source, work->listA, 1, 4, -0x4801);
        } else {
            func_001AC0F8(source, work->listA, 1, 4, -1);
        }
        btlGetIndexListCount(work->listA);
        work->result = 0;
    } else if (source != 0) {
        work->result = func_001AC360(source, work->listA, 0);
    }
    work->entry = 0;
    switch (work->result) {
    case 0:
        id = btlFindEligibleTargetForMultiActorCommand(source, work->listA);
        work->entry = id;
        btlAppendIndexListEntry(work->listB, btlGetIndexListEntry(work->listA, id));
        break;
    case 1:
    case 2:
        btlCopyIndexList(work->listB, work->listA);
        break;
    }
    work->source = source;
    work->state = 1;
    return (s32)work;
}

void fldReleaseSceneSpriteWork(SceneAiWork *work) {
    btlFreeIndexList(work->listB);
    btlFreeIndexList(work->listA);
    sdfReleaseChipBlock(work);
}

void fldReleaseSceneSprite(KwlnTask *arg) {
    fldReleaseSceneSpriteWork((SceneAiWork *)kwlnTaskGetUserValue(arg));
    ((BattleSceneWork *)btlGetRuntime())->spriteObject = 0;
}

u32 fldGetSceneScriptTaskUserData(void) {
    u32 handle = kwlnTaskGetTaskByName(D_004367B8);
    if (handle == 0) {
        return handle;
    }
    return kwlnTaskGetUserValue(handle);
}

u32 fldGetSceneScriptState(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)fldGetSceneScriptTaskUserData();
    return state->state;
}

u32 fldGetSceneScriptValue(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)fldGetSceneScriptTaskUserData();
    return state->value10;
}

void fldCreateSceneSpriteTask(s32 sourceTask) {
    BattleSceneWork *scene;
    s32 task;
    kwlnTaskGetTaskByName(D_004367B8);
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xA), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0), 0);
    }
    scene = (BattleSceneWork *)btlGetRuntime();
    task = kwlnTaskCreate(D_004367B8, 0x2B0E, 1, 1, fldStepSceneStateMachine,
                          fldReleaseSceneSprite, btlCreateAiWork(sourceTask));
    func_00101968(scene->taskParent, task);
    scene->spriteObject = task;
}

void fldMarkActiveSceneScriptState(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)fldGetSceneScriptTaskUserData();
    if (state != 0) {
        state->state = 6;
    }
}

void func_001CCD80(s32 index, s8 operation) {
    s32 bank = D_00438F54->bank;
    s32 count;
    s32 i;
    if (D_00438F54->enabled[bank] == 0) return;
    switch (operation) {
    case 0:
        if (D_00438F58[bank]->state[index] >= 3) return;
        D_00438F58[bank]->state[index] = 3;
        count = D_00438F54->currentIndex;
        for (i = 0; i < count; i++) {
            if ((u8)(D_00438F58[bank]->state[i] - 4) < 3) {
                D_00438F58[bank]->state[i] = 6;
                D_00438F58[bank]->scalePercent[i][0] = 132.0f;
                D_00438F58[bank]->slotValues[i][0] = 16;
                D_00438F58[bank]->scalePercent[i][1] = 105.0f;
                D_00438F58[bank]->slotValues[i][1] = 24;
            }
        }
        break;
    case 1:
        D_00438F58[bank]->state[index] = 7;
        D_00438F54->phase[index + 1][bank] = 4;
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D58);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D88);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCEB8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416DE0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416DF0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416E50);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CD6E0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CDD38);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416ED0);

void func_001CDEC0(s32 bank, s32 index) {
    if (D_00438F58[bank]->secondaryState[index] == 8) {
        D_00438F58[bank]->colorAdjustments[index][0] -= 8;
        D_00438F58[bank]->colorAdjustments[index][0] = D_00438F58[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][0] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][0];
        D_00438F58[bank]->colorAdjustments[index][1] -= 8;
        D_00438F58[bank]->colorAdjustments[index][1] = D_00438F58[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][1] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][1];
        D_00438F58[bank]->colorAdjustments[index][2] -= 8;
        D_00438F58[bank]->colorAdjustments[index][2] = D_00438F58[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][2] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][2];
        D_00438F58[bank]->colorAdjustments[index][3] -= 8;
        D_00438F58[bank]->colorAdjustments[index][3] = D_00438F58[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][3] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][3];
        if (D_00438F58[bank]->colorAdjustments[index][0] <= 0) D_00438F58[bank]->secondaryState[index] = 0;
    }
    if (D_00438F58[bank]->secondaryState[index] == 7) {
        D_00438F58[bank]->colorAdjustments[index][0] -= 24;
        D_00438F58[bank]->colorAdjustments[index][0] = D_00438F58[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][0] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][0];
        if (D_00438F58[bank]->colorAdjustments[index][0] <= 0) D_00438F58[bank]->secondaryState[index]++;
    }
    if ((u8)(D_00438F58[bank]->secondaryState[index] - 6) < 2) {
        D_00438F58[bank]->colorAdjustments[index][1] -= 27;
        D_00438F58[bank]->colorAdjustments[index][1] = D_00438F58[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][1] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][1];
    }
    if ((u8)(D_00438F58[bank]->secondaryState[index] - 5) < 3) {
        D_00438F58[bank]->colorAdjustments[index][3] -= 29;
        D_00438F58[bank]->colorAdjustments[index][3] = D_00438F58[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][3] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][3];
    }
    if ((u8)(D_00438F58[bank]->secondaryState[index] - 4) < 4) {
        D_00438F58[bank]->colorAdjustments[index][2] -= 32;
        D_00438F58[bank]->colorAdjustments[index][2] = D_00438F58[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][2] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][2];
    }
    switch (D_00438F58[bank]->secondaryState[index]) {
    case 3:
        D_00438F58[bank]->colorAdjustments[index][2] += 64;
        D_00438F58[bank]->colorAdjustments[index][2] = D_00438F58[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][2] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][2];
        if (D_00438F58[bank]->colorAdjustments[index][2] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 4:
        D_00438F58[bank]->colorAdjustments[index][3] += 64;
        D_00438F58[bank]->colorAdjustments[index][3] = D_00438F58[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][3] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][3];
        if (D_00438F58[bank]->colorAdjustments[index][3] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 5:
        D_00438F58[bank]->colorAdjustments[index][1] += 64;
        D_00438F58[bank]->colorAdjustments[index][1] = D_00438F58[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][1] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][1];
        if (D_00438F58[bank]->colorAdjustments[index][1] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 6:
        D_00438F58[bank]->colorAdjustments[index][0] += 64;
        D_00438F58[bank]->colorAdjustments[index][0] = D_00438F58[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][0] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][0];
        if (D_00438F58[bank]->colorAdjustments[index][0] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 2:
        D_00438F58[bank]->secondaryState[index]++;
        break;
    case 1:
        D_00438F58[bank]->colorAdjustments[index][0] = 127;
        D_00438F58[bank]->colorAdjustments[index][1] = 127;
        D_00438F58[bank]->colorAdjustments[index][2] = 127;
        D_00438F58[bank]->colorAdjustments[index][3] = 127;
        D_00438F58[bank]->secondaryState[index]++;
        break;
    }
}



void fldScaleSceneCoordinateRecord(EffectSlotSet *work, s32 index) {
    work->workEntries[index].geometry.bounds[2] = work->workEntries[index].sourceWidth << 4;
    work->workEntries[index].geometry.bounds[3] = work->workEntries[index].sourceHeight << 3;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE418);

void fldSetSceneSlotRange(s32 index) {
    SceneSlotFadeWork *scene = D_00438F54;
    if (scene->currentIndex < index) {
        s32 i;
        for (i = 0; i <= index; i++) {
            scene = D_00438F54;
            scene->fade[i][scene->bank] = 0x80;
            scene->phase[i][scene->bank] = 3;
        }
        D_00438F54->lastIndex = index;
    }
    D_00438F54->currentIndex = index;
}

extern s32 btlGetNamedTaskPairStatusOrUnavailable(void);

void func_001CE5C8(void) {
    s32 bank = D_00438F54->bank;
    s32 last = D_00438F54->lastIndex;
    s32 status = btlGetNamedTaskPairStatusOrUnavailable();
    s32 i;
    if (D_00438F54->enabled[bank] == 0) return;
    if (status != 0 && status != 3) return;
    for (i = 0; i <= last; i++) {
        switch (D_00438F54->phase[i][bank]) {
        case 1:
            D_00438F54->timer++;
            D_00438F54->timer = D_00438F54->timer <= 0 ? 0 : D_00438F54->timer > 34 ? 34 : D_00438F54->timer;
            if (D_00438F54->timer >= 34) {
                D_00438F54->fade[i][bank] += 64;
                D_00438F54->fade[i][bank] = D_00438F54->fade[i][bank] <= 0 ? 0 : D_00438F54->fade[i][bank] > 255 ? 255 : D_00438F54->fade[i][bank];
                if (D_00438F54->fade[i][bank] >= 255) {
                    D_00438F54->phase[i][bank]++;
                    D_00438F54->phase[i + 1][bank] = 1;
                }
            }
            break;
        case 2:
            D_00438F54->fade[i][bank] -= 16;
            D_00438F54->fade[i][bank] = D_00438F54->fade[i][bank] <= 128 ? 128 : D_00438F54->fade[i][bank] > 255 ? 255 : D_00438F54->fade[i][bank];
            if (D_00438F54->fade[i][bank] <= 128) {
                D_00438F54->phase[i][bank]++;
                if (i == last) D_00438F54->completed = 1;
            }
            break;
        case 3:
            break;
        case 4:
            D_00438F54->fade[i][bank] -= 64;
            D_00438F54->fade[i][bank] = D_00438F54->fade[i][bank] <= 0 ? 0 : D_00438F54->fade[i][bank] > 128 ? 128 : D_00438F54->fade[i][bank];
            break;
        }
    }
}


extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436878);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436880);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436888);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436890);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436898);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368A0);

void func_001CE838(void) {
    u32 overlays[2] = {0x0000FF00, 0xFF000000};
    u32 colors[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 bank;
    s32 last;
    s32 channel;
    s32 fade;

    bank = D_00438F54->bank;
    if (D_00438F54->enabled[bank] <= 0) {
        return;
    }
    last = D_00438F54->currentIndex - 1;
    if (last >= 0) {
        /* Solid background strips use the live fade again after color lookup. */
        if (last == 1 || last == 2) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 16, channel,
                D_00438F54->fade[2][bank]);
                if (D_00438F54->fade[2][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(425 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 16, 0x53);
        }
        if (last >= 2) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 17, channel,
                D_00438F54->fade[3][bank]);
                if (D_00438F54->fade[3][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(392 << 4, 33 << 3, 0, colors, 0,
            btlResourceBlock->resC, 17, 0x53);
        }
        if (last >= 3) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 18, channel,
                D_00438F54->fade[4][bank]);
                if (D_00438F54->fade[4][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(425 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 18, 0x53);
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 19, channel,
                D_00438F54->fade[4][bank]);
                if (D_00438F54->fade[4][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(349 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 19, 0x53);
        }
        if (last >= 4) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 20, channel,
                D_00438F54->fade[5][bank]);
                if (D_00438F54->fade[5][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(298 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 20, 0x53);
        }
        if (last >= 7) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 17, channel,
                D_00438F54->fade[8][bank]);
                if (D_00438F54->fade[8][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(206 << 4, 33 << 3, 0, colors, 0,
            btlResourceBlock->resC, 17, 0x53);
        }

    }

    if (last >= 0) {
        /* Foreground connectors retain the selected corner fade across lookup. */
        for (channel = 0; channel < 4; channel++) {
            if (channel == 0 || channel == 2) {
                fade = D_00438F54->fade[1][bank];
            } else {
                fade = D_00438F54->fade[0][bank];
            }
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 10, channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[bank];
            }
        }
        func_00306C28(440 << 4, 6 << 3, 0, colors, 0,
        btlResourceBlock->resC, 10, 0x53);
        if (last >= 3) {
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[4][bank];
                } else {
                    fade = D_00438F54->fade[3][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 11, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(395 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 11, 0x53);
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[4][bank];
                } else {
                    fade = D_00438F54->fade[3][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 12, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(362 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 12, 0x53);
        }
        if (last == 5) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 13, channel,
                D_00438F54->fade[6][bank]);
                if (D_00438F54->fade[6][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(302 << 4, 46 << 3, 0, colors, 0,
            btlResourceBlock->resC, 13, 0x53);
        }
        if (last >= 6) {
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[7][bank];
                } else {
                    fade = D_00438F54->fade[6][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 14, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(302 << 4, 48 << 3, 0, colors, 0,
            btlResourceBlock->resC, 14, 0x53);
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[7][bank];
                } else {
                    fade = D_00438F54->fade[6][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 15, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(269 << 4, 48 << 3, 0, colors, 0,
            btlResourceBlock->resC, 15, 0x53);
        }
    }
}


extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);
extern const s32 D_00416FA0[10][3];


void func_001CF0B0(void) {
    u32 overlays[4] = {0x0000FF00, 0xFF000000, 0x8080FF00, 0xFF808000};
    u32 colors[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 rows[10][3] = {
        {435, 28, 6}, {404, 28, 7}, {373, 28, 8}, {342, 28, 7},
        {311, 28, 8}, {280, 28, 7}, {249, 28, 8}, {218, 28, 7},
        {187, 28, 8}, {156, 28, 7}
    };
    s32 finalRows[10][3];
    s32 row;
    s32 last;
    s32 channel;
    s32 fade;

    memcpy(finalRows, D_00416FA0, sizeof(finalRows));

    if (D_00438F54->enabled[D_00438F54->bank] <= 0) {
        return;
    }
    last = D_00438F54->currentIndex - 1;
    for (row = 0; row < D_00438F54->currentIndex; row++) {
        for (channel = 0; channel < 4; channel++) {
            if (channel == 0 || channel == 2) {
                fade = D_00438F54->fade[row + 1][D_00438F54->bank];
            } else {
                fade = D_00438F54->fade[row][D_00438F54->bank];
            }
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC,
                                                       rows[row][2], channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[D_00438F54->bank];
            }
        }
        func_00306C28(rows[row][0] << 4, rows[row][1] << 3, 0, colors,
                      0, btlResourceBlock->resC, rows[row][2], 0x53);
    }
    if (last >= 0) {
        for (channel = 0; channel < 4; channel++) {
            fade = D_00438F54->fade[last + 1][D_00438F54->bank];
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC,
                                                       finalRows[last][2], channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[D_00438F54->bank];
            }
        }
        func_00306C28(finalRows[last][0] << 4, finalRows[last][1] << 3,
                      0, colors, 0, btlResourceBlock->resC, finalRows[last][2], 0x53);
    }
}


void fldInitSceneFadeRecords(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    BtlSceneSlot *slot = scene->slots;
    BtlSceneFadingRecord *rec = scene->fading;
    u32 i = 0;
    while (i < 8 && slot->group != 0) {
        memcpy(&rec->slot, slot, sizeof(BtlSceneSlot));
        rec->alpha = 0x80;
        rec->target = -1;
        i++;
        slot++;
        rec++;
    }
    while (i < 8) {
        memset(rec, 0, sizeof(*rec));
        rec->target = -1;
        i++;
        rec++;
    }
}

s32 btlFindSceneSlotById(BtlSceneSlot *request) {
    BtlSceneSlot *slot = ((BattleSceneWork *)btlGetRuntime())->slots;
    u8 id = request->id;
    u32 i;
    for (i = 0; i < 8; i++) {
        if (slot->id == id) {
            return i;
        }
        slot++;
    }
    return -1;
}

u8 *fldFindSceneSlotRecord(s32 index) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    s32 slot = btlFindSceneSlotById((BtlSceneSlot *)index);
    u8 *entry = 0;
    if (slot != -1) {
        entry = (u8 *)&scene->slots[slot];
    }
    return entry;
}

s32 btlFadeStaleSceneSlots(void) {
    u32 i = 0;
    s32 changed = 0;
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    BtlSceneFadingRecord *record = work->fading;
    BtlSceneSlot *slot = work->slots;
    for (; i < 8; i++, slot++, record++) {
        if (record->slot.id != slot->id) {
            if (fldFindSceneSlotRecord((s32)record) == 0) {
                if (record->alpha != 0) {
                    record->alpha = record->alpha - 8;
                    changed = 1;
                }
            }
        }
    }
    return changed;
}

s32 fldCountSceneFadeKinds(BattleSceneWork *scene, s32 *outFadeCount) {
    SceneGlobalState *global;
    BtlSceneFadingRecord *rec = scene->fading;
    BtlSceneSlot *slot;
    u32 fadeCount = 0;
    s32 countA = 0;
    s32 countB = 0;
    u32 slotCount;
    while (fadeCount < 8 && rec[fadeCount].slot.group != 0) {
        if (rec[fadeCount].slot.group == 1) {
            countA++;
        }
        if (rec[fadeCount].slot.group == 2) {
            countB++;
        }
        fadeCount++;
    }
    global = (SceneGlobalState *)btlTrackedTaskHandles;
    if (global->fadeLatched == 0) {
        if (countA != 0 || countB != 0) {
            global->fadeKindA = countA;
            global->fadeKindB = countB;
            global->fadeLatched = 1;
        }
    }
    fadeCount--;
    slot = scene->slots;
    slotCount = 0;
    while (slotCount < 8 && slot->group != 0) {
        slotCount++;
        slot++;
    }
    *outFadeCount = fadeCount;
    return slotCount;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF800);

s32 fldUpdateConditionalSceneCleanup(KwlnTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    if ((scene->flags & 0x200) == 0) {
        return 0;
    }
    if (((SceneGlobalState *)btlTrackedTaskHandles)->stage == 1 &&
        (((SceneGlobalState *)btlTrackedTaskHandles)->flags & 0x100) == 0) {
        return 0;
    }
    func_001CF800();
    return 0;
}

void fldResetSceneStatus(KwlnTask *task) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->sceneStatus = 0;
}

void fldBeginSceneTransition(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    func_001C7DB8(1, 8);
    scene->flags |= 0x200;
}

void fldClearSceneTransition(void) {
    btlGetRuntime();
    func_001C7DB8(0, 8);
}

void func_001CFB48(void) {
    btlPanelResourcesLoad();
}

u32 fldGetSceneIndexedValue(s32 index) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    return scene->values[index];
}

void func_001CFB90(void) {
}

extern char *D_004368B0;

extern void btlLoadResourceBlock(void);

extern void btlStartRegisteredChildTask(void);

extern void func_001C1520(void);

extern void func_001C16B0(s32);

void fldCreateSceneCleanupTask(void) {
    BattleSceneWork *scene;
    s32 task;
    if (kwlnTaskGetTaskByName(D_004368B0) == 0) {
        btlGetRuntime();
    }
    scene = (BattleSceneWork *)btlGetRuntime();
    task = kwlnTaskCreate(D_004368B0, 0x2B0E, 1, 1, fldUpdateConditionalSceneCleanup, fldResetSceneStatus, 0);
    func_00101968(scene->taskParent, task);
    scene->sceneStatus = task;
    btlLoadResourceBlock();
    btlStartRegisteredChildTask();
    func_001C1520();
    func_001C16B0(1);
    fldBeginSceneTransition();
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFC40);

INCLUDE_ASM(const s32, "game/code_001C7FF8", fldDestroySceneTasksAndBuffers);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416FA0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368B0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368BC);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368BE);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368C0);

