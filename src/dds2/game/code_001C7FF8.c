#include "common.h"
#include "pcp_vu0.h"

extern s32 func_001AA6F8(void);
extern void btlDispatchStateHandler();

extern s32 func_00101740(const char *);

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);

extern s32 kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), s32);
extern void func_00101968(s32, s32);
extern s32 kwlnTaskDestroyWithHierarchy(s32, s32);
extern s32 func_001B88C8(s32);
extern s32 func_001B7DC0(void);
extern s32 func_001B81E8(void);
extern s32 func_001B8538(void);
extern s32 func_001B8740(void);
extern s32 func_001CC9C0(s32);
extern s32 btlBossDebugPrintf(const char *, ...);
extern u32 func_001CCBB8(void);
extern char *D_004367B8;
typedef struct SceneWorkBuffers {
    void *first;
    void *second;
} SceneWorkBuffers;

extern SceneWorkBuffers D_00438F58;

extern s32 D_00435DEC;

extern char *D_004367BC;

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
extern void func_00203F08();

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

extern SceneObject *func_001CA7E0(void);

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
    s32 actorHandle;
    u8 pad64[4];
    s64 ownerId;
} SceneTask;

typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;

typedef struct SceneSpriteWork {
    u8 pad00[0xC];
    u32 firstResource;
    u32 secondResource;
} SceneSpriteWork;

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
extern SceneControl *D_004367FC;
extern s16 *D_00438F48;
extern SceneDescriptor *D_00435E04;
extern f32 D_00433724;
extern f32 *D_0037F770[];
extern u32 func_001C82D8(s32, s8);
extern s32 func_00201108(BattleEffectParams *, s32);
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

extern SceneParty *D_00435DD0;
extern s32 D_00435E38;

extern s32 func_00206090();
extern void func_00206060();
extern s32 func_001AC648();
extern void func_001AEEA8();
extern void kwlnFadeBackgroundStartOut();
extern void kwlnDrawSetOffsetTransition();
extern void kwlnDrawEnableD88();
extern void kwlnDrawEnableDc8();
extern void kwlnDrawEnableE08();
extern void kwlnDrawSetupC70B();
extern void kwlnDrawEnableCd0();
extern void kwlnDrawEnableD30();
extern void func_0022E4C0();
extern void kwlnFadeStartIn();
extern s32 fldGetEncounterRuntimeResult();
extern void fldSetEncounterPendingValue();
extern void evtSetSolarOverlayFullyVisible();
extern void func_001AF060();
extern void func_00204000();
extern void func_001E9410();
extern void btlSpawnBattleWorldAction();
extern void btlCreateRainEffect();
extern s32 btlReleaseScriptResourceA();
extern s32 btlReleaseScriptResource();
extern s32 func_00201540();
extern s32 btlCreateSoundReleaseTask();
extern s32 btlCreateWaitUnitListIdleTask();
extern s32 btlCreateApplyToActiveActorsTask();
extern s32 func_002028C8();
extern void func_001B8078();
extern void func_001B81B0();
extern void func_001D3E00(void);
extern s8 D_0037F531[];

void fldInitializeBattleSceneFlow(void) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    btlNextScaledRandom(7);
    if ((scene->flags & 0x400) != 0) {
        if (scene->variant == 1) {
            func_001B82E8(0);
            func_00210E48();
        } else {
            func_001B82E8(1);
            func_00210E48();
        }
    }
    func_001BF640();
    func_001C7F10();
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

    func_0019B8B0(0x13);
    handle = func_0019F5E8(x << 4, y << 3, 0, first, second, 0);
    func_0019D550(handle, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(handle);
    func_0019B8B0(0xffffffffffffffff);
}

void func_001C8158(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_0019B8B0(0x13);
    handle = func_0019F460(x << 4, y << 3, z, w, D_00435E64 + index * 17, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    func_0019B8B0(-1);
}

void func_001C81F8(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_0019B8B0(0x13);
    handle = func_0019F460(x << 4, y << 3, z, w, D_00435E5C + index * 25, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    func_0019B8B0(-1);
}

s32 btlIsSceneActorCountWithinLimit(s32 unused, u32 limit) {
    func_001AA6F8();
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
extern char *func_001C8A28(s32, s16 *);

/* Advance the per-kind scene counter table: refresh the slot for the current
 * scene kind, then return its value (clamped to 4 unless noClamp is set). */
u32 func_001C83D0(s32 ctx, s8 kind, s8 noClamp) {
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
        func_001C8A28(ctx, buf);
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
    func_001AA6F8();
    roster = (u8 *)D_00435DD0 + 0x1340;
    availability = (RosterAvailability *)D_00435E38;
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
char *func_001C8A28(s32 object, s16 *outId) {
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
extern void func_001C9DC8(s32);

void fldDispatchSceneKindHandler(s32 sceneContext) {
    switch (func_001C82D8(sceneContext, D_004367FC->kind)) {
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
        func_001C9DC8(sceneContext);
    }
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169C0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C92A0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C98E8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9BE8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436858);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436860);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436868);

void func_001C9DC8(s32 unused) {
    char text[8] = "Retreat";
    s32 color;
    s32 handle;
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();

    if (D_00435E04[scene->mode].unk00 != 0) {
        color = D_00438F48[1] | 0x504F6100;
    } else {
        color = D_00438F48[1] | 0x89FEFF00;
    }
    func_0019B8B0(0x13);
    handle = func_0019F460(0x1A0, 0xA60, 0xFF0010, color, (s32)text, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    func_0019B8B0(-1);
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
s32 func_001CA390(s32 self, SceneCheckArgs *args) {
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

extern void func_001BD978(void);

void fldClearBattleSceneObject(void) {
    sdfReleaseChipBlock(kwlnTaskGetUserValue());
    ((BattleSceneWork *)func_001AA6F8())->sceneObject = 0;
    func_001BD978();
}

void fldInitializeSceneObject(u32 *state, u32 owner) {
    memset(state, 0, 0x30);
    state[0] = 1;
    state[11] = owner;
    state[10] = owner + 0x20;
}

SceneObject *func_001CA7E0(void) {
    u32 handle = func_00101740(D_004367BC);
    if (handle == 0) {
        return (SceneObject *)handle;
    }
    return (SceneObject *)kwlnTaskGetUserValue(handle);
}

s32 fldGetSceneObjectState(void) {
    s32 handle = func_00101740(D_004367BC);
    if (handle == 0) {
        return handle;
    }
    return func_001CA7E0()->state;
}

s32 btlHasSpecialActiveSceneActor(void) {
    SceneActor *actor = ((BattleSceneWork *)func_001AA6F8())->actors;
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA8D8);

s32 btlHasSelectedActiveSceneActor(void) {
    SceneActor *actor = ((BattleSceneWork *)func_001AA6F8())->actors;
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
    SceneActor *actor = ((BattleSceneWork *)func_001AA6F8())->actors;
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

extern u8 *D_004367F4;
extern s32 mdlFlagTest();
extern s32 btlGetTaskState6();

s32 fldSelectSceneMode(void) {
    s32 flag = ((BattleSceneWork *)func_001AA6F8())->phaseFlag == 3;
    if (mdlFlagTest(0x801) != 0) {
        return 1;
    }
    if (mdlFlagTest(0x81D) == 0 && mdlFlagTest(0x801) == 0 && !(((SceneGlobalState *)D_004367F4)->flags & 0x200)) {
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
    if (mdlFlagTest(0x805) == 0 && !(((SceneGlobalState *)D_004367F4)->flags & 0x200)) {
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

void func_001CB158(void) {
    SceneObject *object = func_001CA7E0();
    if (object != 0) {
        object->state = 5;
        D_004367FC->mode = 3;
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

extern BtlPanelBlock *D_00436804;
extern void func_00306C28(s32, s32, s32, u8 *, s32, BtlPanelRes *, s32, s32);

void func_001CC7C8(s32 width) {
    u8 color[16] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
    s32 half = width / 2;
    s32 x = width - half + 0x105;
    func_00306C28(x * 0x10, 0x200, 0, color, 0, D_00436804->res, 0x17, 0x53);
    D_00436804->res->inner->fDCC = width << 4;
    func_00306C28((0x100 - half) * 0x10, 0x200, 0, color, 0, D_00436804->res, 0x16, 0x53);
    D_00436804->res->inner->fDCC = D_00436804->res->inner->fE3C << 4;
    func_00306C28((0x92 - half) * 0x10, 0x200, 0, color, 0, D_00436804->res, 0x15, 0x53);
}

extern s32 btlGetEffectActive();
extern void func_001CC020();
extern void func_001CC438();

s32 fldStepSceneStateMachine(s32 handle) {
    BattleSceneWork *work = (BattleSceneWork *)func_001AA6F8();
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

typedef struct SceneAiWork {
    s32 state;                /* 0x00 */
    u32 result;               /* 0x04 */
    s32 entry;                /* 0x08 */
    void *listA;              /* 0x0C */
    void *listB;              /* 0x10 */
    s32 source;               /* 0x14 */
    u8 pad18[0x8C];
} SceneAiWork;

typedef struct SceneAiOther {
    u8 pad00[0x7BE];
    s16 index;                /* 0x7BE */
} SceneAiOther;

extern char *D_004367CC;
extern void *sdfAllocAndClearQuadwords(s32);
extern void *btlAllocateIndexList();
extern u32 btlGetIndexListCount();
extern u32 btlGetIndexListEntry();
extern void btlAppendIndexListEntry();
extern void btlCopyIndexList();
extern void func_001AC0F8();
extern s32 func_001AC360();
extern s32 func_001AC510();

/* Creates the AI work object for `source`: allocates two index lists and
 * fills them according to the current scene object state. */
s32 func_001CC9C0(s32 source) {
    SceneAiWork *work;
    SceneObject *object;
    SceneAiOther *other;
    s32 id;
    s32 count;
    func_001AA6F8();
    work = (SceneAiWork *)sdfAllocAndClearQuadwords(0xA4);
    object = (SceneObject *)kwlnTaskGetUserValue(func_00101740(D_004367BC));
    work->listA = btlAllocateIndexList(0xD);
    work->listB = btlAllocateIndexList(0xD);
    if (object->state == 8) {
        other = (SceneAiOther *)kwlnTaskGetUserValue(func_00101740(D_004367CC));
        count = btlCountFlaggedSceneActors();
        if (count < 2 && (D_00435DD0->entry[other->index].mask & 0x4800)) {
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
        id = func_001AC510(source, work->listA);
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

void fldReleaseSceneSpriteWork(SceneSpriteWork *work) {
    btlFreeIndexList(work->secondResource);
    btlFreeIndexList(work->firstResource);
    sdfReleaseChipBlock(work);
}

void fldReleaseSceneSprite(s64 arg) {
    fldReleaseSceneSpriteWork((SceneSpriteWork *)kwlnTaskGetUserValue(arg));
    ((BattleSceneWork *)func_001AA6F8())->spriteObject = 0;
}

u32 func_001CCBB8(void) {
    u32 handle = func_00101740(D_004367B8);
    if (handle == 0) {
        return handle;
    }
    return kwlnTaskGetUserValue(handle);
}

u32 fldGetSceneScriptState(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)func_001CCBB8();
    return state->state;
}

u32 fldGetSceneScriptValue(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)func_001CCBB8();
    return state->value10;
}

void fldCreateSceneSpriteTask(s32 sourceTask) {
    BattleSceneWork *scene;
    s32 task;
    func_00101740(D_004367B8);
    if (func_001B7DC0() != 0) {
        kwlnTaskDestroyWithHierarchy(func_001B88C8(0xA), 0);
    }
    if (func_001B81E8() != 0) {
        kwlnTaskDestroyWithHierarchy(func_001B88C8(9), 0);
    }
    if (func_001B8538() != 0) {
        kwlnTaskDestroyWithHierarchy(func_001B88C8(1), 0);
    }
    if (func_001B8740() != 0) {
        kwlnTaskDestroyWithHierarchy(func_001B88C8(0), 0);
    }
    scene = (BattleSceneWork *)func_001AA6F8();
    task = kwlnTaskCreate(D_004367B8, 0x2B0E, 1, 1, (void (*)(void))fldStepSceneStateMachine,
                          (void (*)(void))fldReleaseSceneSprite, func_001CC9C0(sourceTask));
    func_00101968(scene->taskParent, task);
    scene->spriteObject = task;
}

void func_001CCD50(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)func_001CCBB8();
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
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
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
        memset(rec, 0, 8);
        rec->target = -1;
        i++;
        rec++;
    }
}

s32 btlFindSceneSlotById(SceneSlot *request) {
    SceneSlot *slot = ((BattleSceneWork *)func_001AA6F8())->slots;
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
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
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
    BattleSceneWork *work = (BattleSceneWork *)func_001AA6F8();
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
    global = (SceneGlobalState *)D_004367F4;
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
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    if ((scene->flags & 0x200) == 0) {
        return 0;
    }
    if (((SceneGlobalState *)D_004367F4)->stage == 1 &&
        (((SceneGlobalState *)D_004367F4)->flags & 0x100) == 0) {
        return 0;
    }
    func_001CF800();
    return 0;
}

void fldResetSceneStatus(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    scene->sceneStatus = 0;
}

void fldBeginSceneTransition(void) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    func_001C7DB8(1, 8);
    scene->flags |= 0x200;
}

void fldClearSceneTransition(void) {
    func_001AA6F8();
    func_001C7DB8(0, 8);
}

void func_001CFB48(void) {
    btlPanelResourcesLoad();
}

u32 fldGetSceneIndexedValue(s32 index) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    return scene->values[index];
}

void func_001CFB90(void) {
}

extern char *D_004368B0;
extern void btlLoadResourceBlock(void);
extern void func_001BB9E8(void);
extern void func_001C1520(void);
extern void func_001C16B0(s32);

void fldCreateSceneCleanupTask(void) {
    BattleSceneWork *scene;
    s32 task;
    if (func_00101740(D_004368B0) == 0) {
        func_001AA6F8();
    }
    scene = (BattleSceneWork *)func_001AA6F8();
    task = kwlnTaskCreate(D_004368B0, 0x2B0E, 1, 1, (void (*)(void))fldUpdateConditionalSceneCleanup, fldResetSceneStatus, 0);
    func_00101968(scene->taskParent, task);
    scene->sceneStatus = task;
    btlLoadResourceBlock();
    func_001BB9E8();
    func_001C1520();
    func_001C16B0(1);
    fldBeginSceneTransition();
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFC40);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFCA8);

void func_001CFEF8(void) {
}

u32 func_001CFF00(void) {
    return 0;
}

void fldBattleSceneEnterInit(u8 *scene) {
    u32 id = ((BattleSceneWork *)scene)->mode;

    if (id < 0x400 && (D_00435E04[id].flags & 0x8000) != 0) {
        ((BattleSceneWork *)scene)->subFlags |= 8;
        kwlnFadeInStart(0xFF, 0xFF, 0xFF, 0);
    }
    if (func_001B4040() == 0) {
        if (func_001B4210() == 0) {
            ((BattleSceneWork *)scene)->phaseFlag = 0;
        } else {
            ((BattleSceneWork *)scene)->phaseFlag = 2;
        }
    } else {
        ((BattleSceneWork *)scene)->phaseFlag = 3;
    }
    func_0022AF90();
    func_00229728(((BattleSceneWork *)scene)->mode);
    func_00203F08(((BattleSceneWork *)scene)->effectLayer, ((BattleSceneWork *)scene)->mode);
    VU0_STORE_VF($vf0, scene);
}

s32 func_001CFFC8(void) {
    if (sndIsStreamStatusTwoOrThree() != 0 &&
        func_0022B108() != 0 &&
        func_0022E460() != 0) {
        sndLoadBattleBank();
        func_0022B288();
        return 3;
    }
    return 0;
}

void fldMarkGridTiles(BattleSceneWork *scene) {
    u8 *tile = (u8 *)func_00200D00(scene->tileX, scene->tileY);
    *(u64 *)(tile + 0x40) = 0x8000000000000001ULL;
    btlStartTask(tile);
    tile = (u8 *)func_00200F28(scene->tileX, scene->tileY);
    btlStartTask(tile);
}

s32 fldSceneStateStartTileEffect(BattleSceneWork *scene) {
    BattleEffectParams params;
    f32 *origin;
    if (btlCountTasksForOwner(0x8000000000000001LL) == 0) {
        func_00204000();
        func_001E9410();
        btlSpawnBattleWorldAction();
        btlCreateRainEffect(scene->tileX, scene->tileY);
        if (scene->subFlags & 0x40000) {
            params.rotation[0] = 0.19607843f;
            params.rotation[1] = 0.19607843f;
            params.rotation[2] = 0.19607843f;
            params.scale[0] = 0.19607843f;
            params.scale[1] = 0.19607843f;
            params.scale[2] = 0.19607843f;
            origin = D_0037F770[0];
            params.position[0] = origin[4];
            params.position[1] = origin[5];
            params.position[2] = origin[6];
            btlStartTask(func_00201108(&params, 0));
        }
        return 4;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0140);

s32 fldSceneStateRestoreDisplay(BattleSceneWork *scene) {
    if (btlCountTasksForOwner(0x8000000000000002LL) == 0) {
        func_00206090();
        if (!(scene->flags & 0x4000)) {
            func_00206060();
        }
        func_001AC648();
        if (scene->subFlags & 0x80) {
            func_001AEEA8();
            scene->subFlags &= ~0x80;
        }
        if (!(scene->flags & 0x4000)) {
            kwlnFadeBackgroundStartOut(0);
            kwlnDrawSetOffsetTransition(0, 0, 1);
            kwlnDrawEnableD88(0);
            kwlnDrawEnableDc8(0);
            kwlnDrawEnableE08(0);
            kwlnDrawSetupC70B(0);
            kwlnDrawEnableCd0(0);
            kwlnDrawEnableD30(0);
            if (!(scene->subFlags & 0x100000)) {
                func_0022E4C0();
            }
            if (!(scene->subFlags & 8)) {
                kwlnFadeStartIn(0);
            }
            if (fldGetEncounterRuntimeResult() != 0) {
                fldSetEncounterPendingValue(1);
            }
            evtSetSolarOverlayFullyVisible();
        }
        if (scene->sceneStatus == 0) {
            fldCreateSceneCleanupTask();
            if (!(scene->subFlags & 0x2000)) {
                scene->flags |= 0x100000;
            } else {
                scene->subFlags &= ~0x2000;
                scene->flags &= 0xFFEFFFFF;
            }
        }
        func_001AF060();
        return 5;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D08A8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0FE0);

void func_001D1120(void) {
}

s32 fldConsumeSceneInputFlags(BattleSceneWork *scene) {
    u32 flags = scene->flags;
    s32 result;
    if ((flags & 0x800) != 0) {
        func_001D3E28();
        result = 8;
    } else if ((flags & 0x400) != 0) {
        func_001D3E28();
        result = 7;
    } else {
        return 0;
    }
    scene->flags |= 0x20;
    return result;
}

void func_001D1190(BattleSceneWork *scene) {
    s32 notFirst = scene->variant != 1;
    scene->variant = 2 - notFirst;
    if (scene->sceneCallback != 0) {
        s32 variant = scene->sceneCallback();
        if (variant != -1) {
            scene->variant = variant;
        }
    }
    scene->groupHandleCount = 0;
    scene->step = scene->step + 1;
    fldInitializeSceneGroups();
    func_001B73E8();
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1200);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D14B0);

s32 fldSceneStateWaitScriptRelease(BattleSceneWork *scene) {
    s32 finished = 1;
    s32 state = scene->scriptState;

    switch (state) {
    case 0xF000002:
        if (btlReleaseScriptResourceA() == 0) {
            finished = 0;
        } else if (!(scene->subFlags & 0x40000)) {
            btlStartTask(func_00201540(0xC));
            btlStartTask(btlCreateSoundReleaseTask(0xC));
            btlStartTask(btlCreateWaitUnitListIdleTask(0xC));
            btlStartTask(btlCreateApplyToActiveActorsTask(0xC));
            btlStartTask(func_002028C8());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
        }
        break;
    case 0xF000000:
        finished = 0;
        if (scene->frame == 0x14) {
            func_001B8078(scene->scriptTarget, scene->scriptArg);
        } else if (scene->frame >= 0x2D) {
            if (func_001B81E8() != 0) {
                if (D_0037F531[0] < 0) {
                    func_001B81B0();
                    func_001C7DB8(1, 0x14);
                }
            } else {
                finished = 1;
            }
        }
        break;
    case 0xF000003:
        break;
    default:
        if (state != -1) {
            finished = btlReleaseScriptResource() != 0;
        }
        break;
    }

    if (finished != 0) {
        if ((scene->flags & 0x800) == 0) {
            func_001D3E00();
            scene->flags &= ~0x400;
            scene->flags &= ~0x1000;
            scene->flags &= ~0x20;
            return 6;
        }
        return 9;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416EF8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F08);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F18);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F28);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416FA0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D18D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D22D8);

extern void itfMesClearFlags();
extern void brsTaskAllowUpdate();
extern void func_002457E0();
extern void func_001AA868();
extern s32 func_0029D000();
extern void datMoveCursorX();

/* Nudge every active party entry's cursor by weight * 0.05 once scene flag
 * 0x40 is clear. */
void func_001D2798(BattleSceneWork *scene) {
    u32 i;
    f32 scale;
    i = 0;
    itfMesClearFlags(1);
    scale = 0.05f;
    brsTaskAllowUpdate();
    func_002457E0(8);
    do {
        func_001AA868(&D_00435DD0->entry[i], -0x45D1);
        if (!(scene->subFlags & 0x40)) {
            if (D_00435DD0->entry[i].flags & 2) {
                if (scene->unk27C == 1) {
                    if (D_00435DD0->entry[i].link != 0 || scene->unk2F8 != 0) {
                        if (!(D_00435DD0->entry[i].mask & 0x40)) {
                            if (func_0029D000(&D_00435DD0->entry[i]) == 0) {
                                datMoveCursorX(&D_00435DD0->entry[i], (s32)((f32)D_00435DD0->entry[i].weight * scale));
                            }
                        }
                    }
                }
            }
        }
        i++;
    } while (i < 5);
    scene->loadStep = 0;
}

extern s32 func_002998D8();
extern s32 fldLoadAreaResource();
extern void btlReleaseEventAssets();
extern void btlReleaseBossData();
extern void fldPollAreaResourceLoad();
extern s32 fldGetResourceReadyFlag();
extern s32 brsTaskPollDone();

/* Field-load step: while loading, sends every linked scene node to state
 * 0x1F, otherwise advances loadStep (0 start, 1 polling, 2 done). Returns
 * 0xB while a load is still pending. */
u32 func_001D28D8(BattleSceneWork *work) {
    SceneLinkedNode *node;
    s32 step;
    if (func_002998D8() == 1) {
        work->subFlags |= 0x20;
        if (work->linkedNodes != 0) {
            for (node = work->linkedNodes; node != 0; node = node->next) {
                if (node->state != 0x1F) {
                    btlDispatchStateHandler(node, 0x1F);
                }
            }
        } else {
            switch (work->loadStep) {
            case 0:
                work->flags &= ~0x10;
                if (fldLoadAreaResource() != 0) {
                    work->loadStep = 1;
                } else {
                    work->loadStep = 2;
                }
                btlReleaseEventAssets();
                btlReleaseBossData();
                break;
            case 1:
                fldPollAreaResourceLoad();
                if (fldGetResourceReadyFlag() == 0) {
                    work->loadStep = 2;
                    btlBossDebugPrintf("btl:field loding end\n");
                }
                break;
            case 2:
                break;
            }
        }
    }
    step = work->loadStep;
    if (step < 3) {
        if (step > 0) {
            if (brsTaskPollDone() == 0) {
                return 0xB;
            }
        }
    }
    return 0;
}

void fldMarkLinkedSceneActors(u8 *scene) {
    u8 *node;
    btlAdvanceTitleStateWithAudioCleanup(scene);
    node = (u8 *)((BattleSceneWork *)scene)->linkedNodes;
    while (node != 0) {
        if (((SceneLinkedNode *)node)->state != 0x1F) {
            btlDispatchStateHandler(node, 0x1F);
        }
        node = (u8 *)((SceneLinkedNode *)node)->next;
    }
    btlFlagTasksForUpdate();
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2A78);

void fldMarkSceneRefresh(BattleSceneWork *scene) {
    func_00230960((s32)scene);
    scene->refreshFlags |= 4;
}

u32 func_001D2C40(void) {
    if (func_00230978() != 0) {
        kwlnFadeInStart(0, 0, 0, 0);
        return 2;
    }
    return 0;
}

void btlSetScene(s32 scene) {
    BattleSceneWork *work = (BattleSceneWork *)func_001AA6F8();
    work->currentScene = scene;
    work->frame = 0;
    work->sceneState = 0;
    D_003B6938[scene].initialize((s32)work);
}

void btlQueueScene(u32 scene) {
    BattleSceneWork *work;

    work = (BattleSceneWork *)func_001AA6F8();
    work->queuedScene = scene;
}

void btlUpdateScene(void) {
    BattleSceneWork *work = (BattleSceneWork *)func_001AA6F8();
    SceneInitializer *entry;
    s32 event;
    if (work->queuedScene != 0) {
        btlSetScene(work->queuedScene);
        work->queuedScene = 0;
    }
    entry = &D_003B6938[work->currentScene];
    event = entry->update((s32)work);
    if (event != 0) {
        btlQueueScene(event);
    }
    work->frame++;
}

void btlResetToInitialScene(void) {
    BattleSceneWork *work;

    work = (BattleSceneWork *)func_001AA6F8();
    btlSetScene(1);
    work->queuedScene = 0;
}

void func_001D2DB0(void) {
}

s32 fldGetSceneDescriptorProperty(void) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    s32 index = scene->currentScene;
    return D_003B6940[index * 3];
}

SceneTask **fldGetActorSceneGroupResource(SceneTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    SceneTask **entry = scene->groupSecondary;
    u32 flags;
    if ((task->flags & 0x40) != 0) {
        return scene->groupHandles;
    }
    flags = (u32)task->actor->status.flags & 0xE00;
    switch (flags) {
    case 0x200:
        entry = scene->groupPrimary;
        break;
    case 0x400:
        break;
    case 0x800:
        entry = scene->groupTertiary;
        break;
    default:
        entry = 0;
        break;
    }
    return entry;
}

SceneTask **fldGetSceneGroupResource(u8 group) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    SceneTask **entry;
    switch (group) {
    case 1:
        entry = scene->groupPrimary;
        break;
    case 2:
        entry = scene->groupSecondary;
        break;
    case 3:
        entry = scene->groupTertiary;
        break;
    default:
        entry = 0;
        break;
    }
    return entry;
}

s32 fldClassifyActorSceneGroup(SceneTask *task) {
    u32 flags;
    func_001AA6F8();
    if (task->flags & 0x40) {
        return 8;
    }
    flags = (u32)task->actor->status.flags & 0xE00;
    switch (flags) {
    case 0x200: return 0x14;
    case 0x400: return 0x2D;
    case 0x800: return 0xF;
    default: return 0;
    }
}

extern u32 func_001AB8D8();

void btlSortSceneGroupByPriorityDesc(SceneTask **list, s32 count) {
    s32 swapped;
    do {
        SceneTask **entry = list;
        u32 i = 0;
        swapped = 0;
        for (; i < count - 1; i++, entry++) {
            SceneTask *first = entry[0];
            SceneTask *second = entry[1];
            if (first != 0 && second != 0 &&
                func_001AB8D8((s32)first->actor + 0x120, 3) < func_001AB8D8((s32)second->actor + 0x120, 3)) {
                entry[0] = second;
                swapped = 1;
                entry[1] = first;
            }
        }
    } while (swapped != 0);
}

void fldSortGroupByPriority(SceneTask **group, s32 count) {
    s32 swapped;
    do {
        SceneTask **entry = group;
        u32 i = 0;
        swapped = 0;
        for (; i < count - 1; i++, entry++) {
            SceneTask *first = entry[0];
            SceneTask *second = entry[1];
            if (first != 0 && second != 0 &&
                first->actor->priority > second->actor->priority) {
                entry[0] = second;
                swapped = 1;
                entry[1] = first;
            }
        }
    } while (swapped != 0);
}

s32 fldGetSceneGroupIndexByActorFlags(u8 *object) {
    u32 flags = ((SceneTask *)object)->actor->status.words.activeFlags & 0xE00;
    s32 result;
    switch (flags) {
    case 0x200:
        result = 1;
        break;
    case 0x400:
        result = 2;
        break;
    case 0x800:
        result = 3;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D30E0);

void fldCompactSceneSlots(void) {
    SceneSlot *slot = ((BattleSceneWork *)func_001AA6F8())->slots;
    u32 i;
    if (slot->b == 0) {
        for (i = 0; i < 7; i++) {
            slot[0].a = slot[1].a;
            slot[0].b = slot[1].b;
            slot[0].id = slot[1].id;
            slot++;
        }
        slot->a = 0;
        slot->b = 0;
        slot->id = 0;
    }
}

/* Consume `amount` scene-slot time: mode 1 drains the front slot's counter
 * (popping it when empty), modes 2 and 3 drain full slots of 100 in steps of 50. */
void func_001D3520(s32 amount, u8 mode) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    u8 head;
    u32 i;
    if (scene->flags & 0x100) {
        head = scene->slots[0].a;
        switch (mode) {
        case 0:
            break;
        case 1:
            if (amount < 100) {
                if (amount < scene->slots[0].b) {
                    scene->slots[0].b -= amount;
                } else {
                    scene->slots[0].b = 0;
                    fldCompactSceneSlots();
                }
            } else {
                while (amount > 0) {
                    scene->slots[0].b = 0;
                    fldCompactSceneSlots();
                    if (scene->slots[0].a != head) {
                        return;
                    }
                    if (scene->slots[0].b == 0) {
                        break;
                    }
                    amount -= 100;
                }
            }
            break;
        case 2:
        case 3:
            while (amount > 0) {
                for (i = 0; i < 8; i++) {
                    if (scene->slots[i].a == head && scene->slots[i].b == 100) {
                        break;
                    }
                }
                if (i < 8) {
                    scene->slots[i].b -= 50;
                } else {
                    scene->slots[0].b = 0;
                    fldCompactSceneSlots();
                }
                amount -= 50;
            }
            break;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", fldInsertSceneSlots);

void fldSwapSceneSlots(s32 index) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    if (scene->flags & 0x100) {
        u8 firstId = scene->slots[0].a;
        u8 count = scene->slots[0].b;
        func_001D3520(index, 1);
        if (index < count) {
            if (scene->slots[1].a != 0 && scene->slots[1].a != firstId) {
                u8 a = scene->slots[0].a;
                u8 b = scene->slots[0].b;
                u8 id = scene->slots[0].id;
                scene->slots[0].a = scene->slots[1].a;
                scene->slots[0].b = scene->slots[1].b;
                scene->slots[0].id = scene->slots[1].id;
                scene->slots[1].a = a;
                scene->slots[1].b = b;
                scene->slots[1].id = id;
            }
        }
    }
}

s32 fldCountSceneSlots(void) {
    SceneSlot *slot = ((BattleSceneWork *)func_001AA6F8())->slots;
    s32 count = 0;
    u32 i;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->a != 0 && slot->b != 0) {
            count++;
        }
    }
    return count;
}

s32 fldAreSceneSlotsFinished(void) {
    BattleSceneWork *work = (BattleSceneWork *)func_001AA6F8();
    SceneSlot *slot;
    u32 i;
    if (work->flags & 0x1000) {
        return 1;
    }
    slot = work->slots;
    for (i = 0; i < 8; i++) {
        if (slot->a != 0) {
            return 0;
        }
        slot++;
    }
    return 1;
}

void fldInitializeSceneGroups(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    fldSortGroupByPriority(scene->groupPrimary, 0x14);
    btlSortSceneGroupByPriorityDesc(scene->groupSecondary, 0x2d);
    btlSortSceneGroupByPriorityDesc(scene->groupTertiary, 0xf);
    func_001D30E0();
}

void btlMoveTaskToGroupTail(SceneTask *task) {
    SceneTask **group = fldGetActorSceneGroupResource(task);
    u32 count = fldClassifyActorSceneGroup(task);
    u32 last;
    u32 i;
    for (i = 0; i < count; i++, group++) {
        if (*group == task) {
            break;
        }
    }
    last = count - 1;
    for (; i < last && group[1] != 0; i++, group++) {
        *group = group[1];
    }
    *group = task;
}

void btlRotateGroupUntilTaskFirst(SceneTask *task) {
    BattleSceneWork *scene;
    u32 i;
    if (task != 0 && (task->flags & 8) != 0 && task->actor != 0 && !(task->flags & 0x40) &&
        ((u32)task->actor->status.flags & 0x200) != 0) {
        i = 0;
        scene = (BattleSceneWork *)func_001AA6F8();
        fldSortGroupByPriority(scene->groupPrimary, 0x14);
        for (; i < 0x14 && scene->groupPrimary[0] != task; i++) {
            btlMoveTaskToGroupTail(scene->groupPrimary[0]);
        }
    }
}

void fldAppendTaskToGroup(SceneTask *task) {
    SceneTask **slot = fldGetActorSceneGroupResource(task);
    SceneTask *head;
    fldClassifyActorSceneGroup(task);
    head = *slot;
    while (*slot != 0) {
        slot++;
    }
    *slot = task;
    btlRotateGroupUntilTaskFirst(head);
}

void fldAppendSceneGroupHandle(s32 handle) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    u32 *slot = scene->groupHandles;
    while (*slot != 0) {
        slot++;
    }
    *slot = handle;
}

void fldUpdateSceneGroupTask(SceneTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    SceneActor *actor;
    s32 kind;

    if ((task->flags & 0x40) == 0) {
        actor = task->actor;
        kind = (actor->status.words.activeFlags & 0x200) != 0 ? 1 : 2;
        scene->groupHandleCount++;
        if (kind == 1) {
            scene->activeGroupCount++;
        }
        if (scene->currentTask == task && scene->variant == kind) {
            scene->flags |= 8;
        }
        if ((*(u64 *)&task->flags & 0x400000100LL) == 0) {
            if (func_001AD1C0((u8 *)actor + 0x120, 0xDE) != 0) {
                task->options |= 4;
            }
        } else {
            task->options &= ~4;
        }
        if ((scene->flags & 0x40) != 0 && (task->options & 4) == 0) {
            btlMoveTaskToGroupTail(task);
        }
    } else {
        btlRemoveTaskFromSceneGroup(task);
        scene->flags |= 8;
        task->flags &= ~0x40;
    }
}

void btlRemoveTaskFromSceneGroup(SceneTask *task) {
    SceneTask **group = fldGetActorSceneGroupResource(task);
    u32 count = fldClassifyActorSceneGroup(task);
    u32 i = 0;
    u32 last;
    for (; i < count; i++) {
        if (group[i] == task) {
            group[i] = 0;
            break;
        }
    }
    last = count - 1;
    for (; i < last; i++) {
        SceneTask *current = group[i];
        SceneTask *next = group[i + 1];
        group[i + 1] = current;
        group[i] = next;
    }
}

void func_001D3E00(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    scene->flags = scene->flags | 0xc;
}

void func_001D3E28(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    scene->flags = scene->flags & 0xfffffffb;
}

s32 fldGetActiveSceneGroupValue(void) {
    u8 *scene = (u8 *)func_001AA6F8();
    s32 value = (s32)((BattleSceneWork *)scene)->groupHandles[0];
    if (value != 0) {
        return value;
    }
    return *(s32 *)fldGetSceneGroupResource(scene[0x2FE]);
}

s32 fldGetSceneGroupEntry(s32 index) {
    u8 *scene = (u8 *)func_001AA6F8();
    if (scene[0x2FE] == 0) {
        return 0;
    }
    return ((s32 *)fldGetSceneGroupResource(scene[0x2FE]))[index];
}

extern s32 func_001B2630();

/* Per-frame scene advance: picks the next group task (or finishes the
 * slot sequence) and dispatches it once flags 4 and 8 are set. */
void func_001D3ED8(void) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    SceneTask *task;
    void (*hook)(void);
    if (scene->flags & 4) {
        if (scene->flags & 8) {
            if (scene->flags & 0x800) {
                return;
            }
            if (scene->flags & 0x400) {
                return;
            }
            if (func_001B2630() != 0) {
                scene->flags |= 0x800;
                return;
            }
            hook = *(void (**)(void))((u8 *)scene + 0x62C);
            if (hook != 0) {
                hook();
            }
            task = scene->groupHandles[0];
            if (task != 0) {
                if (task->state != 2) {
                    return;
                }
                scene->currentTask = task;
                task->flags |= 0x40;
                btlDispatchStateHandler(task, 3);
                scene->flags &= ~8;
            } else {
                if (fldAreSceneSlotsFinished() != 0) {
                    scene->flags |= 0x400;
                    return;
                }
                task = *fldGetSceneGroupResource(((u8 *)scene)[0x2FE]);
                if (task == 0) {
                    return;
                }
                if (task->actor->status.words.activeFlags & 0xE0) {
                    return;
                }
                if (task->state != 2) {
                    return;
                }
                if (task->actor->status.words.activeFlags & 0x30400000) {
                    return;
                }
                scene->currentTask = task;
                btlDispatchStateHandler(task, 3);
                scene->flags &= ~8;
            }
        }
    }
}

void fldClearSceneSlotsAndGroups(void) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    u32 i;

    for (i = 0; i < 8; i++) {
        scene->slots[i].a = 0;
        scene->slots[i].b = 0;
        scene->slots[i].id = 0;
    }
    for (i = 0; i < 20; i++) {
        scene->groupPrimary[i] = 0;
    }
    for (i = 0; i < 45; i++) {
        scene->groupSecondary[i] = 0;
    }
    for (i = 0; i < 15; i++) {
        scene->groupTertiary[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        scene->groupHandles[i] = 0;
    }
    func_001D3E28();
}

u32 func_001D4120(s32 *request) {
    u8 groupIndex;

    if (*request == 0) {
        groupIndex = (u8)request[2];
    }
    else {
        if ((*(u32 *)(*request + 8) & 0x40) != 0) {
            return 1;
        }
        groupIndex = (u8)request[2];
    }
    func_001D3520(request[1], groupIndex);
    return 1;
}

u8 *fldCreateSceneGroupAction(u8 *actor, u32 owner, s32 groupIndex) {
    u8 group = groupIndex;
    u8 *object = (u8 *)btlAllocTask(0xC);
    u8 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x61;
    object[0x10] = 0;
    if (actor != 0) {
        *(u64 *)(object + 0x40) = ((SceneTask *)actor)->actor->ownerId;
    }
    ((SceneTask *)object)->onComplete = (void (*)(void))func_001D4120;
    ((SceneTask *)object)->onUpdate = 0;
    fields = (u8 *)btlGetTaskArguments(object);
    *(u32 *)(fields + 0) = (u32)actor;
    *(u32 *)(fields + 4) = owner;
    fields[8] = group;
    return object;
}

void func_001D4200(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    scene->flags = scene->flags & 0xfffffffb;
}

s32 fldActivateRequestedSceneActor(u32 *request) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    u32 *actor = (u32 *)request[0];
    scene->flags |= 4;
    if (actor != 0 && (actor[2] & 0x40) != 0) {
        return 1;
    }
    fldInsertSceneSlots(request[1]);
    return 1;
}

u8 *fldCreateSceneActorAction(u8 *actor, u32 owner) {
    u8 *object = (u8 *)btlAllocTask(8);
    u32 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x62;
    object[0x10] = 0;
    if (actor != 0) {
        *(u64 *)(object + 0x40) = ((SceneTask *)actor)->actor->ownerId;
    }
    ((SceneTask *)object)->onUpdate = (void (*)(void))func_001D4200;
    ((SceneTask *)object)->onComplete = (void (*)(void))fldActivateRequestedSceneActor;
    fields = (u32 *)btlGetTaskArguments(object);
    fields[0] = (u32)actor;
    fields[1] = owner;
    return object;
}

u32 func_001D4328(u32 *slotIndex) {
    fldSwapSceneSlots(*slotIndex);
    return 1;
}

u8 *fldCreateActorAction(s32 owner) {
    u8 *object = (u8 *)btlAllocTask(4);
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x63;
    ((SceneTask *)object)->onComplete = (void (*)(void))func_001D4328;
    object[0x10] = 0;
    ((SceneTask *)object)->onUpdate = 0;
    *(u32 *)btlGetTaskArguments(object) = owner;
    return object;
}

void func_001D43B0(s32 task) {
    ((SceneTask *)task)->flags |= 1;
}

void func_001D43C0(s32 task) {
    ((SceneTask *)task)->flags &= ~1;
}

void func_001D43D8(s32 task, s32 actor) {
    u32 flags;

    flags = ((SceneActor *)actor)->status.words.activeFlags;
    ((SceneTask *)task)->actor = (SceneActor *)actor;
    if ((flags & 0x400) != 0) {
        if (0x17f < ((SceneActor *)actor)->kind) {
            flags = ((SceneTask *)task)->flags;
            goto LAB_001c8880;
        }
        ((SceneTask *)task)->actionNumber =
                  (u16)*(u8 *)(((u32)((SceneActor *)actor)->kind * 0x14 -
                                                      (u32)((SceneActor *)actor)->kind) * 4 + D_00435DEC + 0x15);
    }
    flags = ((SceneTask *)task)->flags;
LAB_001c8880:
    ((SceneTask *)task)->flags = flags | 8;
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417278);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417288);

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

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368C8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368D0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368D8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368E0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368E8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368F0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368F8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436900);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436908);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436910);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436918);

