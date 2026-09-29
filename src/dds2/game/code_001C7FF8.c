#include "common.h"

extern s32 func_001AA6F8(void);

extern s64 func_00101740(u32);

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);

extern u32 func_001CCBB8(void);

extern s32 D_00435DEC;

extern u32 D_004367BC;

extern void func_001C35F0(s32, s32, s32);

extern u32 func_00101958();

extern void func_00230960(s32);

extern s32 D_003B6940[];

extern s32 func_00230978(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

typedef struct {
    void (*initialize)(s32);
    s32 (*update)(s32);
    s32 flags;
} SceneInitializer;

typedef struct RosterAvailability {
    u8 flags;
    u8 pad[7];
} RosterAvailability;

typedef struct SceneActor {
    u8 pad_00[0x108];
    s64 ownerId;
    u64 flags;
    u8 pad_118[8];
    u8 entryData[4];
    u16 kind;
    u8 pad_126[8];
    u16 selectionFlags;
    u8 pad_130[0x1E8];
    s32 resourceNode;
    u8 pad_31C[8];
    s32 listNode;
    u8 pad_328[0xC];
    s32 pendingResource;
    u8 pad_338[0x2C];
    struct SceneActor *next;
} SceneActor;
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
    u8 pad00[2];
    u8 id;
    u8 alpha;
    u8 pad04[4];
} SceneFadingRecord;

typedef struct BattleSceneWork {
    u8 pad00[0x218];
    u32 flags;
    u8 pad21C[4];
    u32 refreshFlags;
    u8 pad224[8];
    s32 currentScene;
    s32 queuedScene;
    s32 frame;
    s32 sceneState;
    u8 pad23C[0xC];
    void *linkedNodes;
    SceneActor *actors;
    u8 pad250[0x1E];
    u8 phaseFlag;
    u8 pad26F;
    u16 variant;
    u8 pad272[0x2E];
    s32 mode;
    u8 pad2A4[8];
    s16 tileX;
    s16 tileY;
    u8 pad2B0[0x20];
    u32 sceneObject;
    u32 spriteObject;
    u32 sceneStatus;
    u8 pad2DC[0x22];
    SceneSlot slots[8];
    u8 pad316[0x16A];
    SceneFadingRecord fading[8];
    u8 pad4C0[0x24];
    u32 values[1];
} BattleSceneWork;
extern SceneInitializer D_003B6938[];

extern u32 func_001B57B0(void);

extern s32 func_001AC750(s32, void *);

extern s32 D_004367C0;

extern char D_003B5D10[];

extern char D_003B5B10[];

extern s32 btlGetEntryFlagsUnlessDisabled(void *);

extern void func_001D3978(void);

extern u32 D_00435E64;

extern u32 D_00435E5C;

extern s32 D_00438F54;
extern s32 D_00435DD0;
extern s32 D_00435E38;

void fldInitializeBattleSceneFlow(void) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    func_00210CB0(7);
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

void fldIgnoreTaggedSceneEvent(s32 tag, ...) {
}

void func_001C80C0(void) {
}

void func_001C80C8(void) {
}

void func_001C80D0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    func_0019B8B0(0x13);
    temp_v0 = func_0019F5E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D550(temp_v0, 1, 0x53);
    func_0019C5B0(temp_v0);
    func_0019B8B0(0xffffffffffffffff);
}

void func_001C8158(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_0019B8B0(0x13);
    handle = func_0019F460(x << 4, y << 3, z, w, D_00435E64 + index * 17, 0);
    func_0019D530(handle, 1);
    func_0019C5B0(handle);
    func_0019B8B0(-1);
}

void func_001C81F8(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_0019B8B0(0x13);
    handle = func_0019F460(x << 4, y << 3, z, w, D_00435E5C + index * 25, 0);
    func_0019D530(handle, 1);
    func_0019C5B0(handle);
    func_0019B8B0(-1);
}

s32 func_001C82A0(s32 unused, u32 limit) {
    func_001AA6F8();
    if (limit < func_001B57B0()) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C82D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C83D0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8518);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8768);

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

char *func_001C8A28(s32 object, s16 *value) {
    s32 cached = D_004367C0;
    if (cached == 0) {
        cached = func_001AC750(*(s32 *)(*(s32 *)(object + 0x2C) + 0x18), D_003B5D10);
        D_004367C0 = cached;
    }
    *value = cached;
    return D_003B5D10;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8A80);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C91D8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169C0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C92A0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C98E8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9BE8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9DC8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9EA0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA390);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169F0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A00);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A10);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA490);

extern void func_001BD978(void);

void func_001CA760(void) {
    func_00328E48(func_00101958());
    ((BattleSceneWork *)func_001AA6F8())->sceneObject = 0;
    func_001BD978();
}

void fldInitializeSceneObject(u32 *state, u32 owner) {
    memset(state, 0, 0x30);
    state[0] = 1;
    state[11] = owner;
    state[10] = owner + 0x20;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA7E0);

s64 func_001CA820(void) {
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367BC);
    if (temp_v0 == 0) {
        return temp_v0;
    }
    return *(s32 *)func_001CA7E0();
}

s32 func_001CA858(void) {
    SceneActor *actor = ((BattleSceneWork *)func_001AA6F8())->actors;
    while (actor != 0) {
        if ((actor->flags & 0x421) == 0x401) {
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

s32 func_001CA958(void) {
    SceneActor *actor = ((BattleSceneWork *)func_001AA6F8())->actors;
    while (actor != 0) {
        if ((actor->flags & 0x421) == 0x401 &&
            (actor->selectionFlags & 1) != 0) {
            return 1;
        }
        actor = actor->next;
    }
    return 0;
}

s32 func_001CA9C0(void) {
    SceneActor *actor = ((BattleSceneWork *)func_001AA6F8())->actors;
    while (actor != 0) {
        if ((actor->flags & 0x421) == 0x401 &&
            btlGetEntryFlagsUnlessDisabled(actor->entryData) != 0) {
            return 0;
        }
        actor = actor->next;
    }
    return 1;
}

extern u8 *D_004367F4;
extern s32 mdlFlagTest();
extern s32 func_001C0118();
extern s32 func_001CA8D8();

s32 func_001CAA30(void) {
    s32 flag = ((BattleSceneWork *)func_001AA6F8())->phaseFlag == 3;
    if (mdlFlagTest(0x801) != 0) {
        return 1;
    }
    if (mdlFlagTest(0x81D) == 0 && mdlFlagTest(0x801) == 0 && !(*(u32 *)(D_004367F4 + 0x3C) & 0x200)) {
        if (func_001CA858() != 0) {
            return 2;
        }
    }
    if (mdlFlagTest(0x81A) == 0) {
        if (flag != 0) {
            return 6;
        }
    }
    if (flag == 0) {
        if (func_001C0118() != 0) {
            return 5;
        }
    }
    if (mdlFlagTest(0x805) == 0 && !(*(u32 *)(D_004367F4 + 0x3C) & 0x200)) {
        if (func_001CA8D8() != 0 && func_001CA9C0() != 0) {
            return 3;
        }
    }
    if (mdlFlagTest(0x805) != 0) {
        if (mdlFlagTest(0x806) == 0) {
            if (func_001CA958() != 0) {
                return 4;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CAB60);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB158);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB190);

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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC7C8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D28);

extern s32 battleGetEffectActive();
extern void func_001CC020();
extern void func_001CC438();

s32 func_001CC8D0(s32 handle) {
    BattleSceneWork *work = (BattleSceneWork *)func_001AA6F8();
    s32 *state;
    s32 mode;
    if (work->flags & 0x04000000) {
        return 0;
    }
    state = (s32 *)func_00101958(handle);
    switch (*state) {
    case 1:
        *state = 2;
        break;
    case 2:
        mode = 1;
        if (work->mode == 0x312) {
            mode = battleGetEffectActive() == 1 ? 5 : 1;
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC9C0);

void func_001CCB50(SceneSpriteWork *work) {
    func_001E8018(work->secondResource);
    func_001E8018(work->firstResource);
    func_00328E48(work);
}

void fldReleaseSceneSprite(s64 arg) {
    func_001CCB50((SceneSpriteWork *)func_00101958(arg));
    ((BattleSceneWork *)func_001AA6F8())->spriteObject = 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCBB8);

u32 func_001CCBF8(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)func_001CCBB8();
    return state->state;
}

u32 func_001CCC18(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)func_001CCBB8();
    return state->value10;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCC38);

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

void func_001CE3E8(SceneCoordinateWork *work, s32 index) {
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF500);

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
        if (record->id != slot->id) {
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF720);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF800);

s32 fldUpdateConditionalSceneCleanup(void) {
    BattleSceneWork *scene = (BattleSceneWork *)func_001AA6F8();
    if ((scene->flags & 0x200) == 0) {
        return 0;
    }
    if (*(u32 *)(D_004367F4 + 0x38) == 1 &&
        (*(u32 *)(D_004367F4 + 0x3C) & 0x100) == 0) {
        return 0;
    }
    func_001CF800();
    return 0;
}

void func_001CFAC0(void) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    scene->sceneStatus = 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFAE0);

void func_001CFB20(void) {
    func_001AA6F8();
    func_001C7DB8(0, 8);
}

void func_001CFB48(void) {
    func_001B7238();
}

u32 func_001CFB60(s32 index) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)func_001AA6F8();
    return scene->values[index];
}

void func_001CFB90(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFB98);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFC40);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFCA8);

void func_001CFEF8(void) {
}

u32 func_001CFF00(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFF08);

s32 func_001CFFC8(void) {
    if (func_00203FD8() != 0 &&
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
    startBattleTask(tile);
    tile = (u8 *)func_00200F28(scene->tileX, scene->tileY);
    startBattleTask(tile);
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0078);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0140);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0710);

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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1190);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1200);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D14B0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1700);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416EF8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F08);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F18);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F28);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416FA0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D18D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D22D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2798);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D28D8);

void fldMarkLinkedSceneActors(u8 *scene) {
    u8 *node;
    func_00204050(scene);
    node = *(u8 **)(scene + 0x248);
    while (node != 0) {
        if (*(s32 *)node != 0x1F) {
            dispatchBattleStateHandler(node, 0x1F);
        }
        node = *(u8 **)(node + 0x178);
    }
    flagBattleTasksForUpdate();
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

void func_001D2CD0(u32 scene) {
    BattleSceneWork *work;

    work = (BattleSceneWork *)func_001AA6F8();
    work->queuedScene = scene;
}

void btlUpdateScene(void) {
    BattleSceneWork *work = (BattleSceneWork *)func_001AA6F8();
    s32 next;
    s32 event;
    next = work->queuedScene;
    if (next != 0) {
        btlSetScene(next);
        work->queuedScene = 0;
    }
    event = D_003B6938[work->currentScene].update((s32)work);
    if (event != 0) {
        func_001D2CD0(event);
    }
    work->frame = work->frame + 1;
}

void func_001D2D78(void) {
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

u8 *fldGetActorSceneGroupResource(u32 *request) {
    u8 *scene = (u8 *)func_001AA6F8();
    u8 *entry = scene + 0x368;
    u32 flags;
    if ((request[2] & 0x40) != 0) {
        return scene + 0x458;
    }
    flags = *(u32 *)(request[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200:
        entry = scene + 0x318;
        break;
    case 0x400:
        break;
    case 0x800:
        entry = scene + 0x41C;
        break;
    default:
        entry = 0;
        break;
    }
    return entry;
}

u8 *fldGetSceneGroupResource(u8 group) {
    u8 *scene = (u8 *)func_001AA6F8();
    u8 *entry;
    switch (group) {
    case 1:
        entry = scene + 0x318;
        break;
    case 2:
        entry = scene + 0x368;
        break;
    case 3:
        entry = scene + 0x41C;
        break;
    default:
        entry = 0;
        break;
    }
    return entry;
}

s32 func_001D2EC8(s32 *object) {
    u32 flags;
    func_001AA6F8();
    if (object[2] & 0x40) {
        return 8;
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200: return 0x14;
    case 0x400: return 0x2D;
    case 0x800: return 0xF;
    default: return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2F40);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3018);

s32 func_001D3098(u8 *object) {
    u32 flags = *(u32 *)(*(u8 **)(object + 0x18) + 0x110) & 0xE00;
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

void func_001D34B8(void) {
    u8 *slot = (u8 *)(func_001AA6F8() + 0x2FE);
    u32 i;
    if (slot[1] == 0) {
        for (i = 0; i < 7; i++) {
            slot[0] = slot[3];
            slot[1] = slot[4];
            slot[2] = slot[5];
            slot += 3;
        }
        slot[0] = 0;
        slot[1] = 0;
        slot[2] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3520);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3690);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D37E8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3898);

s32 func_001D38F0(void) {
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

void func_001D3978(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001D3018(temp_v0 + 0x318, 0x14);
    func_001D2F40(temp_v0 + 0x368, 0x2d);
    func_001D2F40(temp_v0 + 0x41c, 0xf);
    func_001D30E0();
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D39C8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3A78);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3B38);

void fldAppendSceneGroupHandle(s32 handle) {
    u8 *scene = (u8 *)func_001AA6F8();
    u32 *slot = (u32 *)(scene + 0x458);
    while (*slot != 0) {
        slot++;
    }
    *slot = handle;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3C00);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3D40);

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
    s32 value = *(s32 *)(scene + 0x458);
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

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3ED8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4020);

u32 func_001D4120(s32 *arg0) {
    u8 temp_v0;

    if (*arg0 == 0) {
        temp_v0 = (u8)arg0[2];
    }
    else {
        if ((*(u32 *)(*arg0 + 8) & 0x40) != 0) {
            return 1;
        }
        temp_v0 = (u8)arg0[2];
    }
    func_001D3520(arg0[1], temp_v0);
    return 1;
}

u8 *fldCreateSceneGroupAction(u8 *actor, u32 owner, s32 groupIndex) {
    u8 group = groupIndex;
    u8 *object = (u8 *)func_001E1468(0xC);
    u8 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x61;
    object[0x10] = 0;
    if (actor != 0) {
        *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(u32 *)(object + 0x4C) = (u32)func_001D4120;
    *(u32 *)(object + 0x48) = 0;
    fields = (u8 *)func_001E14F8(object);
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
    u8 *scene = (u8 *)func_001AA6F8();
    u32 *actor = (u32 *)request[0];
    *(u32 *)(scene + 0x218) |= 4;
    if (actor != 0 && (actor[2] & 0x40) != 0) {
        return 1;
    }
    func_001D3690(request[1]);
    return 1;
}

u8 *fldCreateSceneActorAction(u8 *actor, u32 owner) {
    u8 *object = (u8 *)func_001E1468(8);
    u32 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x62;
    object[0x10] = 0;
    if (actor != 0) {
        *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(u32 *)(object + 0x48) = (u32)func_001D4200;
    *(u32 *)(object + 0x4C) = (u32)fldActivateRequestedSceneActor;
    fields = (u32 *)func_001E14F8(object);
    fields[0] = (u32)actor;
    fields[1] = owner;
    return object;
}

u32 func_001D4328(u32 *arg0) {
    func_001D37E8(*arg0);
    return 1;
}

u8 *fldCreateActorAction(s32 owner) {
    u8 *object = (u8 *)func_001E1468(4);
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x63;
    *(u32 *)(object + 0x4C) = (u32)func_001D4328;
    object[0x10] = 0;
    *(u32 *)(object + 0x48) = 0;
    *(u32 *)func_001E14F8(object) = owner;
    return object;
}

void func_001D43B0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 1;
}

void func_001D43C0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffffe;
}

void func_001D43D8(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0x110);
    *(s32 *)(arg0 + 0x18) = arg1;
    if ((temp_v0 & 0x400) != 0) {
        if (0x17f < *(u16 *)(arg1 + 0x124)) {
            temp_v0 = *(u32 *)(arg0 + 8);
            goto LAB_001c8880;
        }
        *(u16 *)(arg0 + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(arg1 + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(arg1 + 0x124)) * 4 + D_00435DEC + 0x15);
    }
    temp_v0 = *(u32 *)(arg0 + 8);
LAB_001c8880:
    *(u32 *)(arg0 + 8) = temp_v0 | 8;
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417278);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417288);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4438);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4590);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D46A8);

extern s32 func_00202F80(s32);
extern s32 func_002046A0(s32);
extern void sndFreeResourceNode(s32);
extern void sndFreeListNode(s32);
extern s32 func_001E97C0(u8 *);
extern void func_001E9860(void);
extern s32 btlCountTasksForOwner(s64);
extern s32 btlCountTasksByKind(s32);

s32 func_001D4820(SceneActor *actor) {
    if (actor->resourceNode != 0) {
        if (func_00202F80(actor->resourceNode) != 0) {
            return 0;
        }
        sndFreeResourceNode(actor->resourceNode);
        actor->resourceNode = 0;
    }
    if (actor->listNode != 0) {
        if (func_002046A0(actor->listNode) != 0) {
            return 0;
        }
        sndFreeListNode(actor->listNode);
        actor->listNode = 0;
    }
    if (actor->pendingResource != 0) {
        return 0;
    }
    if (func_001E97C0((u8 *)actor) != 0) {
        func_001E9860();
        return 0;
    }
    if (btlCountTasksForOwner(actor->ownerId) != 0) {
        return 0;
    }
    return btlCountTasksByKind(0x2E) == 0;
}

void func_001D48E0(void) {
}

void func_001D48E8(void) {
}

void func_001D48F0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffdff;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4908);

void func_001D4A30(s32 arg0) {
    *(u32 *)(arg0 + 8) = (*(u32 *)(arg0 + 8) | 0x10) & ~0x200;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4A48);

void func_001D4B90(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffffef;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4BA8);

void func_001D4C98(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4CA0);

void func_001D4FE0(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4FE8);

void func_001D5330(u32 arg0) {
    func_001AA6F8();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    func_001CAB60(arg0);
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5368);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D54B8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5668);

void func_001D5938(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5950);

void func_001D5B38(s32 arg0) {
    func_001C35F0(arg0, 0, 0);
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 0x20;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5B70);

void func_001D5BD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5BE0);

void func_001D5CE8(s32 arg0) {
    func_001B7830();
    func_001DF700(arg0 + 0x20);
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x314) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5D20);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5E50);

extern s32 func_00210498();
extern void startBattleTask();
extern void func_00201828();
extern void func_001DD390();

void func_001D5EE8(u8 *task) {
    u8 *arg = task + 0x20;
    u8 *work = (u8 *)func_001AA6F8();
    s32 owner = *(s32 *)(task + 0x18);
    void (*hook)(u8 *);
    if (func_001D46A8(owner) != 0) {
        if (*(u16 *)(task + 0x50) == 2) {
            startBattleTask(func_00210498(owner, *(s32 *)(task + 0x54)));
        }
        func_00201828(task, arg);
        hook = *(void (**)(u8 *))(work + 0x634);
        if (hook != 0) {
            hook(task);
        }
        func_001DD390(task, arg);
    }
}

void func_001D5FA8(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5FB0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417348);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417360);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436858);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436860);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436868);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436870);

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

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436920);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436928);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436930);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436938);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436940);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436948);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436950);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436958);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436960);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436968);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436970);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436978);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436980);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436988);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436990);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436998);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369A0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369A8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369B0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369B8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369C0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369C8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369D0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369D8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369E0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369E8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369F0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369F8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436A00);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436A08);

