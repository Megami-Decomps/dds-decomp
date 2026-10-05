#include "common.h"
#include "btl_task.h"
#include "pcp_vu0.h"

extern s32 btlGetRuntime(void);

extern s32 kwlnTaskGetTaskByName(const char *);

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);
extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern s32 kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), s32);

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

typedef struct SceneDescriptor {
    s8 unk00;
    u8 pad01[0x1F];
    u16 flags;
    u8 pad22[6];
} SceneDescriptor;

typedef struct SceneObject {
    s32 state;
} SceneObject;

extern SceneObject *fldGetSceneObjectTaskUserData(void);

typedef struct SceneActor {
    u8 pad_00[0xC8];
    s32 species;              /* 0xC8 */
    u8 pad_CC[0x3C];
    s64 ownerId;
    union {
        u64 flags;            /* 0x110: combined status mask */
        struct {
            u32 activeFlags;  /* 0x110 */
            u32 stateFlags;   /* 0x114 */
        } words;
    } status;
    u32 extraFlags;           /* 0x118 */
    u8 priority;
    u8 pad_11D[3];
    u8 entryData[4];
    u16 kind;
    u8 pad_126[2];
    u16 statA;                /* 0x128 */
    u8 pad_12A[2];
    u16 statB;                /* 0x12C */
    u16 selectionFlags;
    u8 pad_130[0x12];
    u16 cards[8];
    u8 pad_152[0x1C2];
    s32 actionResource;       /* 0x314 */
    s32 resourceNode;
    u8 pad_31C[8];
    s32 listNode;
    u8 pad_328[0xC];
    s32 pendingResource;
    u8 pad_338[0x2C];
    struct SceneActor *next;
} SceneActor;

typedef struct SceneTask {
    s32 state;
    u16 actionNumber;         /* 0x04 */
    u8 pad06[2];
    u32 flags;
    u32 options;              /* 0x0C */
    u8 pad10[8];
    SceneActor *actor;
    u8 pad1C[4];
    s32 command;
    s32 commandValue;         /* 0x24 */
    s32 commandReference;      /* 0x28: resolved for command 4 */
    u8 pad2C[0x1C];
    void (*onUpdate)(void);   /* 0x48 */
    void (*onComplete)(void); /* 0x4C */
    u16 actionStage;          /* 0x50 */
    u8 pad52[2];
    s32 effect;               /* 0x54 */
    u8 pad58[8];
    BtlIndexList *targetList; /* 0x60 */
    u8 pad64[4];
    s64 ownerId;
} SceneTask;

typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;

typedef struct SceneScriptState {
    u32 state;
    u8 pad04[0xC];
    u32 value10;
} SceneScriptState;

typedef struct SceneFadingRecord {
    SceneSlot slot;
    u8 alpha;
    s32 target;
} SceneFadingRecord;

typedef struct SceneLinkedNode {
    s32 state;
    u8 pad04[0x174];
    struct SceneLinkedNode *next; /* 0x178: scene-linked chain */
} SceneLinkedNode;

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
    SceneLinkedNode *linkedNodes;
    SceneActor *actors;
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
    SceneSlot slots[8];
    u8 pad316[2];
    SceneTask *groupPrimary[20];    /* 0x318 */
    SceneTask *groupSecondary[45];  /* 0x368 */
    SceneTask *groupTertiary[15];   /* 0x41C */
    SceneTask *groupHandles[8];     /* 0x458 */
    u16 groupHandleCount;
    u8 pad47A[2];
    s32 activeGroupCount;
    SceneFadingRecord fading[8];
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

extern u32 func_001C82D8(s32, s8);

extern u32 btlCountFlaggedSceneActors(void);

extern s32 func_001AC750(s32, void *);

extern s32 D_004367C0;

extern char D_003B5D10[];

extern char D_003B5B10[];

extern s32 btlGetEntryFlagsUnlessDisabled(void *);

extern u8 *D_00435E64;

extern u8 *D_00435E5C;

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
void fldSubmitSceneObjectAtCoordinates(s32 x, s32 y, u64 first, u64 second) {
    u64 handle;

    itfSetTextDrawLimit(0x13);
    handle = func_0019F5E8(x << 4, y << 3, 0, first, second, 0);
    func_0019D550(handle, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(0xffffffffffffffff);
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
    roster = (u8 *)datGameState + 0x1340;
    availability = (RosterAvailability *)datItemSkillRecords;
    output = (u8 *)D_003B5B10;
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
        color = btlLinkedSelectionTaskBuffer[1] | 0x504F6100;
    } else {
        color = btlLinkedSelectionTaskBuffer[1] | 0x89FEFF00;
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

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169F0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A00);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A10);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA490);

extern void btlReleaseBattleScratchBlocks(void);

void fldClearBattleSceneObject(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    ((BattleSceneWork *)btlGetRuntime())->sceneObject = 0;
    btlReleaseBattleScratchBlocks();
}

void fldInitializeSceneObject(u32 *state, u32 owner) {
    memset(state, 0, 0x30);
    state[0] = 1;
    state[11] = owner;
    state[10] = owner + 0x20;
}

SceneObject *fldGetSceneObjectTaskUserData(void) {
    u32 handle = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (handle == 0) {
        return (SceneObject *)handle;
    }
    return (SceneObject *)kwlnTaskGetUserValue(handle);
}

s32 fldGetSceneObjectState(void) {
    s32 handle = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (handle == 0) {
        return handle;
    }
    return fldGetSceneObjectTaskUserData()->state;
}

s32 btlHasSpecialActiveSceneActor(void) {
    SceneActor *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((actor->status.flags & 0x421) == 0x401) {
            u16 kind = actor->kind;
            if (kind == 0x4C || kind == 0x3C) {
                return 1;
            }
        }
        actor = actor->next;
    }
    return 0;
}

s32 func_001CA8D8(void) {
    u32 requiredFlags = 0x201;
    SceneActor *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    s32 i;

    if (actor != 0) {
        do {
            if ((actor->status.words.activeFlags & requiredFlags) == requiredFlags) {
                for (i = 0; i < 8; i++) {
                    if ((u16)(actor->cards[i] - 0xE0) < 0x20) {
                        return actor->cards[i];
                    }
                }
            }
            actor = actor->next;
        } while (actor != 0);
    }
    return 0;
}

s32 btlHasSelectedActiveSceneActor(void) {
    SceneActor *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((actor->status.flags & 0x421) == 0x401 &&
            (actor->selectionFlags & 1) != 0) {
            return 1;
        }
        actor = actor->next;
    }
    return 0;
}

s32 btlHaveActiveSceneActorEntriesCleared(void) {
    SceneActor *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((actor->status.flags & 0x421) == 0x401 &&
            btlGetEntryFlagsUnlessDisabled(actor->entryData) != 0) {
            return 0;
        }
        actor = actor->next;
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

s32 fldSelectSceneMode(void) {
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CAB60);

void fldSetSceneObjectAndGroupStates(void) {
    SceneObject *object = fldGetSceneObjectTaskUserData();
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB278);

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

void btlDrawCenteredPanelSegments(s32 width) {
    u8 color[16] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
    s32 half = width / 2;
    s32 x = width - half + 0x105;
    func_00306C28(x * 0x10, 0x200, 0, color, 0, btlResourceBlock->res, 0x17, 0x53);
    btlResourceBlock->res->inner->fDCC = width << 4;
    func_00306C28((0x100 - half) * 0x10, 0x200, 0, color, 0, btlResourceBlock->res, 0x16, 0x53);
    btlResourceBlock->res->inner->fDCC = btlResourceBlock->res->inner->fE3C << 4;
    func_00306C28((0x92 - half) * 0x10, 0x200, 0, color, 0, btlResourceBlock->res, 0x15, 0x53);
}

extern s32 btlGetEffectActive();

extern void func_001CC020();

extern void func_001CC438();

s32 fldStepSceneStateMachine(s32 handle) {
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

typedef struct SceneAiOther {
    u8 pad00[0x7BE];
    s16 index;                /* 0x7BE */
} SceneAiOther;

extern char *D_004367CC;

extern void *sdfAllocAndClearQuadwords(s32);






extern void func_001AC0F8(s32 source, BtlIndexList *list, s32, s32, s32);

extern s32 func_001AC360(s32 source, BtlIndexList *list, s32);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 source, BtlIndexList *list);

/* Creates the AI work object for `source`: allocates two index lists and
 * fills them according to the current scene object state. */
s32 btlCreateAiWork(s32 source) {
    SceneAiWork *work;
    SceneObject *object;
    SceneAiOther *other;
    s32 id;
    s32 count;
    btlGetRuntime();
    work = (SceneAiWork *)sdfAllocAndClearQuadwords(0xA4);
    object = (SceneObject *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef));
    work->listA = btlAllocateIndexList(0xD);
    work->listB = btlAllocateIndexList(0xD);
    if (object->state == 8) {
        other = (SceneAiOther *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
        count = btlCountFlaggedSceneActors();
        if (count < 2 && (datGameState->entry[other->index].mask & 0x4800)) {
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

void fldReleaseSceneSprite(s64 arg) {
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
    task = kwlnTaskCreate(D_004367B8, 0x2B0E, 1, 1, (void (*)(void))fldStepSceneStateMachine,
                          (void (*)(void))fldReleaseSceneSprite, btlCreateAiWork(sourceTask));
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCD80);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D58);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D88);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCEB8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416DE0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416DF0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416E50);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CD6E0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CDD38);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416ED0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CDEC0);

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

void fldScaleSceneCoordinateRecord(SceneCoordinateWork *work, s32 index) {
    s32 address = index * 0xA0 + (s32)work->records;
    SceneCoordinateRecord *record = (SceneCoordinateRecord *)address;
    record->scaledX = record->sourceX << 4;
    record->scaledY = record->sourceY << 3;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE418);

void fldSetSceneSlotRange(s32 index) {
    u8 *scene = (u8 *)D_00438F54;
    if (*(s8 *)(scene + 0x20) < index) {
        s32 i;
        for (i = 0; i <= index; i++) {
            s32 offset = i * 2;
            scene = (u8 *)D_00438F54;
            *(s32 *)(scene + (offset + *(s32 *)(scene + 4)) * 4 + 0x38) = 0x80;
            *(u8 *)((*(s32 *)(scene + 4) + offset) + (s32)scene + 0x22) = 3;
        }
        ((u8 *)D_00438F54)[0x21] = index;
    }
    ((u8 *)D_00438F54)[0x20] = index;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE5C8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE838);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF0B0);

void fldInitSceneFadeRecords(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    SceneSlot *slot = scene->slots;
    SceneFadingRecord *rec = scene->fading;
    u32 i = 0;
    while (i < 8 && slot->a != 0) {
        memcpy(&rec->slot, slot, sizeof(SceneSlot));
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

s32 btlFindSceneSlotById(SceneSlot *request) {
    SceneSlot *slot = ((BattleSceneWork *)btlGetRuntime())->slots;
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
    s32 slot = btlFindSceneSlotById((SceneSlot *)index);
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
    SceneFadingRecord *record = work->fading;
    SceneSlot *slot = work->slots;
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
    SceneFadingRecord *rec = scene->fading;
    SceneSlot *slot;
    u32 fadeCount = 0;
    s32 countA = 0;
    s32 countB = 0;
    u32 slotCount;
    while (fadeCount < 8 && rec[fadeCount].slot.a != 0) {
        if (rec[fadeCount].slot.a == 1) {
            countA++;
        }
        if (rec[fadeCount].slot.a == 2) {
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
    while (slotCount < 8 && slot->a != 0) {
        slotCount++;
        slot++;
    }
    *outFadeCount = fadeCount;
    return slotCount;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF800);

s32 fldUpdateConditionalSceneCleanup(void) {
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

void fldResetSceneStatus(void) {
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
    task = kwlnTaskCreate(D_004368B0, 0x2B0E, 1, 1, (void (*)(void))fldUpdateConditionalSceneCleanup, fldResetSceneStatus, 0);
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

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416EF8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F08);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F18);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F28);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416FA0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436878);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436880);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436888);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436890);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436898);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368A0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368A8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368B0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368BC);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368BE);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368C0);

