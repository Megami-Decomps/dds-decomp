#include "common.h"

extern s32 *btlFindGroupedEntity();

extern u8 D_00436F5D;

extern s32 mdlFlagTest(s32);

extern s32 D_00435DD0;

extern s32 func_001AA6F8(void);

extern s32 btlCountTasksByKind(u32);

extern void *func_00328E18(s32);

extern void func_002C7CE8(void *);

extern void sndReleaseSlotOwner(void *);

extern void func_0020D128();

extern u8 *D_00435E20;

extern void *btlAllocateIndexList(s32);

extern u32 func_001AC360(u64, u64, u64);

extern u32 func_001E8058();

extern void *func_001E8060(void *, u32);

extern void func_001E8018(void *);

extern u32 btlGetEffectActive(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetEffectActor(void);

extern s8 D_00453068[];

typedef struct BattleRuntimeState {
    u32 flags;
    u16 state;
    u8 unk_06;
    u8 pending;
    s8 active;
    u8 unk_09[3];
    u32 options;
    u32 color10;
    u32 color14;
    u8 unk_18[0x20];
    void *ownedData;
    void *resource;
    void *handle;
} BattleRuntimeState;

extern BattleRuntimeState D_00453060;

typedef struct PadButtons {
    s8 unk_0;
    s8 stick;
    u8 unk_2[2];
    u8 b4, b5, b6, b7;
} PadButtons;

extern PadButtons D_0040B7D8[];

extern s32 func_00106108();

extern void btlReleaseOwnedData(void);

extern void *memset(void *, s32, u32);

typedef struct BattleGraphicsCallback {
    u8 unknown[0x10];
    void (*invoke)(void *, void *);
} BattleGraphicsCallback;

extern BattleGraphicsCallback D_00380608;

extern u8 D_00380860[];

extern u8 D_00380870[];

extern s32 D_00435CC8;

extern void sdfFreeMemoryFromEitherHeap(void *);

extern void *func_003293C8(s32);

extern void *sdfResourceRetainAddress(void *);

extern void *sdfAllocatePacketList(s32);

extern void func_0032D3F0(void *);

extern void func_0032C768(void *, void *, s32, s32, s32, s32, void *, s32, s32, s32);

extern void func_0032D438(void *, void *);

extern void func_0032EB80(void *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfCreateDescriptorPacket(void *, s32, s32, s32, s32, s32, void *, s32);

extern void func_001053F0(s32, s32, f32);

extern s32 kwlnTextureSetReferenceFlagIfPresent(void);

extern void func_0022D040(void);

extern u8 D_003BF961[];

extern u16 D_003BF962[];

extern s32 sdfCreateSemaphore(s32, s32, s32);

extern u32 D_00438F90;

extern s32 D_003C86F0[];

extern s32 D_003C8710[];

extern void func_0010D818();

extern void func_0011A118(u32, u32);

extern s32 func_0022D2F8(u32, u32);

extern u32 btlAllocTask(u32);

extern u32 func_001E14F8(u32);

extern void func_0022BA08(void);

extern void func_0022A908(u32);

extern void scrSetCurrentActor(u32, u32);

extern s32 btlReleaseScriptResource(void);

extern s32 btlFindModelEntry();

typedef struct BattleEffectState {
    u32 actor;
    struct BtlUnit *linkedUnit;
    u32 value;
    u16 timer;
    u8 active, phase;
    u32 effect;
    f32 speed;
} BattleEffectState;

typedef struct BattleEffectContext {
    u8 pad00[0x2A0];
    u32 battleId;
    u8 pad2A4[0x24];
    u32 targetObject;
    u8 pad2CC[0x44C];
    BattleEffectState *effect;
} BattleEffectContext;

typedef struct BattleEffectUnitNode {
    u8 pad00[0x110];
    u32 status;
    u32 statusExtra;
    u8 pad118[0x24C];
    struct BattleEffectUnitNode *next;
} BattleEffectUnitNode;

typedef struct BattleScriptTask {
    u8 active;
    u8 pad01[0xF];
    u8 startFlag;
    u8 pad11[0xF];
    u16 kind;
    u8 pad22[0x2A];
    void (*callback)(void);
} BattleScriptTask;

typedef struct BattleScriptTaskData {
    u32 object;
    u32 group;
    u32 frames;
} BattleScriptTaskData;

extern void *func_001E5DA8(void *, s32, s32);

extern void func_0035C860();

extern char D_0041B650[];

extern char D_00436D00[];

extern s32 scrCreateTaskForProcessId(s32, s32, s32);

extern char D_00436D08[];

extern s32 func_0025D008(s16, s32);

extern char D_0041B7E0[];

typedef struct BattleCombatant {
    u8 unk_00[0x110];
    u32 status;
    u32 statusExtra;
    u8 unk_118[8];
    u16 entryFlags;
    u8 unk_122[2];
    u16 kind;
    u16 alternateKind;
    u8 unk_128[6];
    u16 ailment;
    u8 unk_130[0x234];
    struct BattleCombatant *next;
} BattleCombatant;

typedef struct BattleListEntry {
    u8 pad0[6];
    u16 primaryCount;
    u16 primaryLimit;
    u16 secondaryCount;
    u16 secondaryLimit;
    u16 flags;
} BattleListEntry;

extern s32 mdlRequestAsset(s32, s32, s32);

extern s32 fileRequestIsReady(void *);

extern void sdfBuildPacketE(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct BtlUnitModel {
    u8 unk_00[0x8C];
    u32 *flags;
} BtlUnitModel;

typedef struct BtlUnit {
    u8 unk_00[0xC8];
    u32 species;
    u8 unk_CC[0x20];
    s32 unk_EC;
    u8 unk_F0[0x20];
    u32 flags;
    u32 unk_114;
    u8 unk_118[8];
    u16 unk_120;
    u8 unk_122[2];
    u16 mode;
    u8 unk_126[8];
    u16 unk_12E;
    u8 unk_130[0x1EC];
    u32 unk_31C;
    u8 unk_320[0x20];
    struct BtlUnitModel *model;
    u8 unk_324[0x20];
    struct BtlUnit *next;
} BtlUnit;

typedef struct BtlTask {
    u8 unk_00[8];
    u32 flags;
    u8 unk_0C[0xC];
    BtlUnit *unit;
    u8 unk_1C[4];
    s32 result;
    s32 arg;
    u8 unk_28[0x38];
    s32 unk_60;
    u8 unk_64[0x108];
    struct BtlTask *next;
} BtlTask;

typedef struct BtlState {
    u8 unk_000[0x1E4];
    s16 unk_1C0;
    u8 unk_1C2[0x32];
    u32 unk_1F4;
    u8 unk_1F8[4];
    u32 unk_1FC;
    u8 unk_200[0x24];
    BtlTask *tasks;
    BtlUnit *units;
    u8 unk_22C[0x20];
    u16 unk_24C;
    u8 unk_24E[0x4A6];
    struct BattleEffectState *effect;
    u8 unk_698[0xC];
    s32 unk_6A4;
    s32 unk_6A8;
    u8 unk_6AC[8];
    s32 unk_6B4;
    s32 unk_6B8;
    u8 unk_6BC[8];
    s32 unk_6C4;
    u8 unk_6C8[0x10];
    s32 unk_6D8;
    u8 unk_6DC[8];
    s32 unk_6E4;
    s32 unk_6E8;
    u8 unk_6EC[8];
    s32 unk_6F4;
    u8 unk_6F8[0x10];
    s32 unk_708;
    s32 table0[0x20];
    s32 table1[0x180];
    s32 table2[0x20];
    s8 unk_E0C;
    u8 unk_E0D;
    s16 unk_E0E;
} BtlState;

extern s32 btlHasEffectActor(void);

extern s32 btlHasEffectActor(void);

extern s32 btlHasEffectActor(void);

extern s64 btlStartTask(void *);

extern s32 scrReadIntParameter(s32);

extern u8 *btlFindUnitByModeClear(s32);

extern u8 *btlFindUnitByModeFlagged(s32);

extern void *btlCreateModelChangeTask(void *, s32, s32, s32, s32, s32);

extern void *func_00328D68(s32);

extern void *func_00328D68(s32);

/* Battle state fields used while locating and launching battle script resources. */
typedef struct BattleScriptResources {
    u8 pad00[0x1E4];
    s16 scriptGroup;          /* 0x1E4: selects the resource path */
    u8 pad1E6[2];
    u32 startedFlags;         /* 0x1E8 */
    u32 eventFlags;           /* 0x1EC */
    s16 eventSubstate;        /* 0x1F0 */
    u8 pad1F2[2];
    s32 eventAction;          /* 0x1F4 */
    s32 eventResult;          /* 0x1F8 */
    void *eventAuxData;       /* 0x1FC */
    void *eventData;          /* 0x200 */
    BtlUnit *eventActor;      /* 0x204 */
    s32 sequenceBaseHandle;   /* 0x208 */
    s32 resourceHandle;       /* 0x20C */
    void *assetData;          /* 0x210 */
    u8 pad214[4];
    u32 resourceFlags;        /* 0x218 */
    u8 pad21C[0x60];
    u8 encounterMode;         /* 0x27C */
    u8 pad27D[0x47];
    u8 *scriptOwner;          /* 0x2C4 */
    s32 taskHandle;           /* 0x2C8 */
} BattleScriptResources;

typedef struct BattleGroupIdEntry {
    struct BattleGroupIdEntry *next;
    s32 id;
} BattleGroupIdEntry;

    s32 type;

typedef struct BattleGroupSlot {
    s32 unk_0;
    s32 unk_4;
    s32 unk_8;
    s32 resourceHandle;
} BattleGroupSlot;

typedef struct BattleGroupNode {
    struct BattleGroupNode *next;
    struct BattleGroupNode *prev;
    u16 group;
    u16 type;
    u8 flag;
    u8 unk_0D[3];
    s32 unk_10;
    s32 resourceList;
    s32 unk_18;
    s32 ownedResource;
    BattleGroupSlot slots[8];
    s32 unk_A0;
    s32 unk_A4;
    s32 unk_A8;
    f32 unk_AC;
    f32 unk_B0;
} BattleGroupNode;

extern char D_00436CF8[];

extern char D_0041B628[];

extern char D_0041B640[];

extern char D_0041B768[];

extern char D_0041B7D0[];

extern void func_003297C8(s32);

extern void *func_0019F448(s32, s32, u32, u32, s32, s32);

extern void func_0019D550(void *, s32, s32);

extern s32 func_0019C5B0(void *);

extern void func_0020D1C0(u8 *, u8 *, s32, s32, u32, u32);

extern s32 func_0022ED90(u8 *, u8 *, s32, u8 *, s32);

extern void *sdfAllocPacketAligned(s32);

extern void sdfResetPacketList(void *);

extern void sdfAppendPacket(void *, s32);

extern s32 func_0033D810(s32, s32, s32, s32, void *, u32);

extern u8 D_00436EF8[];

typedef struct BtlMenuDrawer {
    u8 unk_00[0x10];
    void (*draw)(struct BtlMenuDrawer *, s32);
} BtlMenuDrawer;

extern BtlMenuDrawer D_00380748;

extern void func_00328E48(void *);

extern void func_003298C0(void *);

extern void func_002322E8(s32);

extern void func_00234858(s32);

extern void sdfResourceListRelease(void *, s32);

extern void func_00328E48(void *);

void func_00227288(void) {
    func_00226F58();
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002272A0);

s32 btlIsEffectPhaseInRange(s32 unused, s32 value) {
    BattleEffectState *effect = ((BattleEffectContext *)func_001AA6F8())->effect;
    if (effect->active != 1) {
        return 0;
    }
    switch (value) {
    case 0x12:
    case 0x13:
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00227528);

s32 func_00227660(BattleCombatant *unit, s32 arg1) {
    u32 id;
    if (!(unit->status & 1)) {
        return arg1;
    }
    if (!(unit->status & 0x400)) {
        return arg1;
    }
    if ((*(BattleEffectState **)(func_001AA6F8() + 0x718))->active != 1) {
        return arg1;
    }
    id = unit->kind;
    if (id == 0x119) {
        return arg1;
    }
    switch (arg1) {
    case 1:
        if (id == 0x12E) {
            return 0xC8;
        }
        if (id == 0x12F) {
            return 1;
        }
        break;
    case 2:
        if (id == 0x12E) {
            return 0xC8;
        }
        if (id == 0x12F) {
            return 0x66;
        }
        break;
    default:
        if (id == 0x12E) {
            return arg1 + 0xC8;
        }
        if (id == 0x12F) {
            return arg1 + 0x64;
        }
        break;
    }
    return arg1;
}

BtlUnit *btlFindFlaggedSpecialSpeciesUnit(s32 category, s32 species) {
    BtlUnit *unit;
    if (category != 1) {
        return 0;
    }
    if (species != 0x119) {
        return 0;
    }
    unit = ((BtlState *)func_001AA6F8())->units;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 2) {
                    if (unit->species == 0x119) {
                        return unit;
                    }
                }
            }
        }
        unit = unit->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002277D8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227820);

INCLUDE_ASM(const s32, "game/code_00227288", func_002279F0);

s32 btlGetEffectTaskActorMatchCode(BtlTask *task) {
    BattleEffectState *effect;
    if ((task->flags & 8) == 0) {
        return -1;
    }
    effect = ((BattleEffectContext *)func_001AA6F8())->effect;
    return effect->actor == (u32)task->unit ? 12 : -1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00227CC8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00227DA8);

s32 btlIsEffectActor(u32 actor) {
    BattleEffectState *state = ((BattleEffectContext *)func_001AA6F8())->effect;
    u32 active = state->actor;
    if (active != 0) {
        return active == actor;
    }
    return 0;
}

s32 btlGetSoleTargetKind(void) {
    BtlState *state = (BtlState *)func_001AA6F8();
    BtlUnit *target;
    BtlUnit *unit;
    BtlUnit *last;
    s32 count;
    if (btlHasEffectActor() == 0) {
        return -1;
    }
    target = (BtlUnit *)btlGetEffectActor();
    last = NULL;
    count = 0;
    for (unit = state->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x200) {
                if (!(unit->flags & 0xE0)) {
                    if (!(unit->unk_12E & 0x800)) {
                        count++;
                        last = unit;
                    }
                }
            }
        }
    }
    if (count == 1 && (last == NULL || last == target)) {
        return 7;
    }
    return -1;
}

s32 btlHasDifferentActiveTarget(u32 target) {
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    return btlGetEffectActor() != target;
}

s32 btlSetLinkFlagOff(BtlUnit *arg) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)func_001AA6F8())->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x12F) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == arg) {
            *other->model->flags &= ~1;
            return 1;
        }
        if (other != arg) {
            return 1;
        }
        if (unit->flags & 4) {
            *other->model->flags &= ~1;
        } else {
            *other->model->flags |= 1;
        }
        return 0;
    }
    return 1;
}

s32 btlSetLinkFlagOn(BtlUnit *arg) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)func_001AA6F8())->units; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x12F) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == arg) {
            *other->model->flags |= 1;
            return 1;
        }
        if (other != arg) {
            return 1;
        }
        if (unit->flags & 4) {
            *other->model->flags &= ~1;
        } else {
            *other->model->flags |= 1;
        }
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002286D8);

s32 btlTryScheduleMarkedUnitTask(BtlUnit *unit) {
    BtlState *battle = (BtlState *)func_001AA6F8();
    BattleEffectState *effect;
    BtlUnit *other;
    if ((unit->flags & 0x400) == 0) {
        return 1;
    }
    effect = battle->effect;
    if (effect->active != 0) {
        return 1;
    }
    if (unit->species == 0x119) {
        return 1;
    }
    other = battle->units;
    while (other != 0) {
        u32 flags = other->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (other != unit) {
                    if (flags & 0xe0) {
                        return 1;
                    }
                }
            }
        }
        other = other->next;
    }
    btlStartTask(func_001E5DA8(unit, 8, 10));
    unit->flags &= ~0x100;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00228B08);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228D68);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228F20);

INCLUDE_ASM(const s32, "game/code_00227288", func_00228F48);

u32 btlGetEffectActive(void) {
    BattleEffectContext *battle = (BattleEffectContext *)func_001AA6F8();
    u32 battleId = battle->battleId;
    BattleEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state != NULL) {
        return state->active;
    }
    return 0;
}

s32 btlHasEffectActor(void) {
    BattleEffectContext *battle = (BattleEffectContext *)func_001AA6F8();
    u32 battleId = battle->battleId;
    BattleEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state == NULL) {
        return 0;
    }
    return state->actor != 0;
}

u32 btlGetEffectValue(void) {
    BattleEffectContext *battle = (BattleEffectContext *)func_001AA6F8();
    u32 battleId = battle->battleId;
    BattleEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state != NULL) {
        return state->value;
    }
    return 0;
}

u32 btlGetEffectActor(void) {
    BattleEffectState *state = ((BattleEffectContext *)func_001AA6F8())->effect;
    return state->actor;
}

s32 func_002291C0(void) {
    BtlState *battle = (BtlState *)func_001AA6F8();
    BtlUnit *unit = battle->units;
    BattleEffectState *effect = battle->effect;
    while (unit != 0) {
        if ((unit->flags & 0x400) &&
            unit->mode == 0x12F) {
            break;
        }
        unit = unit->next;
    }
    if (unit == 0) {
        return 1;
    }
    if (!(unit->flags & 0xe0)) {
        return 0;
    }
    return effect->linkedUnit == unit;
}

void func_00229248(void) {
    u8 *puVar1;
    BattleEffectContext *battle;

    battle = (BattleEffectContext *)func_001AA6F8();
    puVar1 = (u8 *)battle->effect;
    puVar1[1] = 1;
    *puVar1 = 0;
}

u32 func_00229278(void) {
    return 0xffffffff;
}

s32 func_00229280(void) {
    s32 result = -1;
    s32 state;

    state = *(s32 *)(func_001AA6F8() + 0x718);
    if (*(s8 *)(state + 0) != 0) {
        if (*(s8 *)(state + 1) != 0) {
            *(u8 *)(state + 1) = 0;
            return btlFindScriptResource(D_00436CF8);
        }
        *(u8 *)(state + 0) = 0;
        return -1;
    }
    return result;
}

void func_002292D8(void) {
    u16 temp_v0;
    u16 *puVar2;
    u32 temp_v1;
    u32 temp_v2;
    u32 temp_v3;
    u8 temp_v4;

    temp_v4 = 0;
    temp_v1 = 0;
    temp_v3 = 0;
    puVar2 = (u16 *)(D_00435DD0 + 0xa60);
    temp_v2 = 0;
    do {
        temp_v0 = *puVar2;
        if ((temp_v0 & 1) != 0) {
            if (puVar2[2] == 2) {
                if ((temp_v0 & 2) != 0) {
                    return;
                }
                temp_v4 = 1;
            }
            temp_v3 = temp_v3 + 1;
            if ((temp_v0 & 2) != 0) {
                temp_v1 = temp_v1 + 1;
            }
        }
        temp_v2 = temp_v2 + 1;
        puVar2 = puVar2 + 0xe2;
    } while (temp_v2 < 5);
    if (((temp_v3 < 4) && (temp_v1 < 3)) && (temp_v4)) {
        func_0011AEE0(2);
        return;
    }
}

s32 func_00229378(void) {
    BattleEffectUnitNode *node = *(BattleEffectUnitNode **)(func_001AA6F8() + 0x24c);
    while (node != 0) {
        if (node->status & 1) {
            node->statusExtra &= ~0x100000;
        }
        node = node->next;
    }
    return -1;
}

void func_002293D8(BattleCombatant *unit) {
    u32 flags = unit->status;
    if ((flags & 0x200) && unit->kind == 2) {
        unit->status = flags | 0x1000;
        unit->statusExtra |= 0x100000;
        unit->entryFlags |= 0x1000;
    }
}

void func_00229420(BattleCombatant *unit) {
    if ((unit->status & 0x400) &&
        unit->kind == 0x144 &&
        mdlFlagTest(0x841)) {
        unit->alternateKind = 1;
    }
}

u32 func_00229470(void) {
    return 6;
}

void func_00229478(void) {
    func_0011AEE0(7);
}

void func_00229490(void) {
    func_0011AEE0(1);
    func_0011AEE0(4);
    func_0011AEE0(5);
}

void func_002294B8(void) {
    func_0011AEE0(8);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002294D0);

INCLUDE_ASM(const s32, "game/code_00227288", func_002295D8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00229690);

INCLUDE_ASM(const s32, "game/code_00227288", func_00229728);

void func_0022A7D0(void) {
    u8 *battle = (u8 *)func_001AA6F8();
    void (*cleanup)(void) = *(void (**)(void))(battle + 0x604);
    if (cleanup != 0) {
        cleanup();
    }
    func_0020D128(D_0041B628);
}

void btlReleaseBossData(void) {
    u8 *battle = (u8 *)func_001AA6F8();
    void (*cleanup)(void);
    if ((*(u32 *)(battle + 0x218) & 0x80000) == 0) {
        return;
    }
    cleanup = *(void (**)(void))(battle + 0x5C8);
    if (cleanup != 0) {
        cleanup();
    }
    func_0022A7D0();
    if (*(void **)(battle + 0x718) != 0) {
        func_00328470(*(void **)(battle + 0x718));
        *(void **)(battle + 0x718) = 0;
    }
    *(u32 *)(battle + 0x218) &= ~0x80000;
    func_0020D128(D_0041B640);
}

s32 btlFindScriptResource(char *name) {
    char path[128];
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    if (battle->resourceHandle == 0) {
        return -1;
    }
    func_0035C860(path, D_0041B650, battle->scriptGroup, name);
    return bfFindScriptIndexByName(battle->resourceHandle, path);
}

void func_0022A908(u32 skill) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    s32 handle;
    if (battle->scriptGroup == -1) {
        return;
    }
    handle = scrCreateTaskForProcessId(*(s32 *)(battle->scriptOwner + 0x20) - 1,
                            battle->resourceHandle, skill);
    scrSetCurrentActor(handle, 0);
    func_00101968((s32)battle->scriptOwner, handle);
    battle->taskHandle = handle;
}

s32 btlReleaseScriptResource(void) {
    extern s32 kwlnTaskIsRegistered(s32);
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    if (battle->scriptGroup == -1) {
        return 1;
    }
    if (kwlnTaskIsRegistered(battle->taskHandle) == 0) {
        battle->taskHandle = 0;
        return 1;
    }
    return 0;
}

s32 btlCanStartPrimaryScriptTask(void) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    if (battle->scriptGroup == -1) {
        return 0;
    }
    if (battle->startedFlags & 1) {
        return 0;
    }
    if (battle->resourceHandle == 0) {
        return 0;
    }
    return btlFindScriptResource(D_00436D00) != -1;
}

void btlStartPrimaryScriptTask(void) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    s32 scriptId;
    s32 taskId;
    if (battle->scriptGroup == -1) {
        return;
    }
    scriptId = btlFindScriptResource(D_00436D00);
    if (scriptId == -1) {
        return;
    }
    taskId = scrCreateTaskForProcessId(*(s32 *)(battle->scriptOwner + 0x20) - 1,
                            battle->resourceHandle, scriptId);
    scrSetCurrentActor(taskId, 0);
    func_00101968((s32)battle->scriptOwner, taskId);
    battle->taskHandle = taskId;
    battle->startedFlags |= 1;
}

s64 btlReleaseScriptResourceA(void) {
    return btlReleaseScriptResource();
}

s32 btlHasScriptResource(void) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();

    if (battle->scriptGroup == -1) {
        return 0;
    }
    if (battle->resourceHandle == 0) {
        return 0;
    }
    if ((battle->resourceFlags & 0x800) == 0 || battle->encounterMode != 1) {
        return 0;
    }
    return btlFindScriptResource(D_00436D08) != -1;
}

void btlStartSecondaryScriptTask(void) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    s32 scriptId;
    s32 taskId;
    if (battle->scriptGroup == -1) {
        return;
    }
    scriptId = btlFindScriptResource(D_00436D08);
    if (scriptId == -1) {
        return;
    }
    taskId = scrCreateTaskForProcessId(*(s32 *)(battle->scriptOwner + 0x20) - 1,
                            battle->resourceHandle, scriptId);
    scrSetCurrentActor(taskId, 0);
    func_00101968((s32)battle->scriptOwner, taskId);
    battle->taskHandle = taskId;
    battle->startedFlags |= 2;
}

s64 btlReleaseScriptResourceB(void) {
    return btlReleaseScriptResource();
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B628);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B640);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B650);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AC10);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022AF90);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B108);

void btlReleaseEventData(void) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    void *data;
    if ((battle->eventFlags & 2) == 0) {
        return;
    }
    data = battle->eventData;
    if (data != 0) {
        func_001686F0(data);
        battle->eventData = 0;
    }
    data = battle->eventAuxData;
    if (data != 0) {
        func_001683F0(data);
        battle->eventAuxData = 0;
    }
    battle->eventSubstate = 0;
    battle->eventAction = -1;
    battle->eventFlags &= ~2;
    func_0020D128(D_0041B768);
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B768);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B288);

void btlReleaseEventAssets(void) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    void *data;
    btlReleaseEventData();
    data = battle->assetData;
    if (data != 0) {
        func_003297C8(data);
        battle->assetData = 0;
    }
    func_0020D128(D_0041B7D0);
}

s32 btlCommandSelectEventAction(void) {
    s32 selector = scrReadIntParameter(0);
    s32 unitId = scrReadIntParameter(1);
    s32 action = scrReadIntParameter(2);
    BtlUnit *unit;
    BattleScriptResources *battle;
    s32 result;
    if (selector == 0) {
        unit = (BtlUnit *)btlFindUnitByModeClear(unitId);
    } else {
        unit = (BtlUnit *)btlFindUnitByModeFlagged(unitId);
    }
    if (unit == 0) {
        return 1;
    }
    if ((unit->flags & 2) == 0) {
        return 1;
    }
    battle = (BattleScriptResources *)func_001AA6F8();
    result = func_0025D008(battle->scriptGroup, action);
    if (result == 0) {
        return 1;
    }
    battle->eventResult = result;
    battle->eventActor = unit;
    battle->eventAction = action;
    battle->eventSubstate = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B460);

s32 func_0022B510(void) {
    s32 choice = scrReadIntParameter(0);
    s32 unitIndex = scrReadIntParameter(1);
    s32 first = scrReadIntParameter(2);
    s32 second = scrReadIntParameter(3);
    u8 *unit;
    if ((u32)unitIndex >= 0x180) {
        return 1;
    }
    if ((u32)first >= 0x180) {
        return 1;
    }
    if (choice == 0) {
        unit = btlFindUnitByModeClear(unitIndex);
    } else {
        unit = btlFindUnitByModeFlagged(unitIndex);
    }
    if (unit == NULL) {
        return 1;
    }
    btlStartTask(btlCreateModelChangeTask(unit, choice != 0, first, second, 0x18, 0));
    return 1;
}

u8 func_0022B5E0(void) {
    s64 taskCount;

    taskCount = btlCountTasksByKind(0x1a);
    return taskCount == 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B600);

s32 btlCommandSetSequenceVolumePan(void) {
    BattleScriptResources *battle = (BattleScriptResources *)func_001AA6F8();
    s32 index = scrReadIntParameter(0);
    if (func_00342168(battle->sequenceBaseHandle) != 0) {
        sndSetSequenceVolumePan(battle->sequenceBaseHandle + index, 0x7f, 0x3f);
        func_0020D128(D_0041B7E0, battle->sequenceBaseHandle + index);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B760);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022B7A0);

u32 func_0022B8E0(void) {
    u32 index = scrReadIntParameter(0);
    u32 value = 0;
    if (index < 100) {
        value = D_003BF961[index * 4];
    }
    func_0010D818(value);
    return 1;
}

u32 btlCommandCountModelFlags(void) {
    s64 isSet;
    s32 flag;
    u32 index;
    s32 count;

    index = 0;
    count = 0;
    do {
        flag = 0x8ff - index;
        index = index + 1;
        isSet = mdlFlagTest(flag);
        if (isSet != 0) {
            count = count + 1;
        }
    } while (index < 100);
    func_0010D818(count);
    return 1;
}

u32 func_0022B988(void) {
    u32 index = scrReadIntParameter(0);
    u32 value = 1;
    if (index < 100) {
        value = D_003BF962[index * 2];
    }
    func_0010D818(value);
    return 1;
}

u32 func_0022B9D0(void) {
    s32 index = scrReadIntParameter(0);
    if (index < 0x100) {
        func_0011A118(index, 1);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022BA08);

u32 btlCreateScriptResourceTask(u32 object, u32 group) {
    BattleScriptTask *task = (BattleScriptTask *)btlAllocTask(12);
    BattleScriptTaskData *data;
    task->active = 1;
    task->kind = 0x68;
    task->callback = func_0022BA08;
    task->startFlag = 0;
    data = (BattleScriptTaskData *)func_001E14F8((u32)task);
    data->object = object;
    data->group = group;
    data->frames = 0;
    return (u32)task;
}

u32 btlUpdateScriptResourceTask(BattleScriptTaskData *record) {
    if (record->frames == 0) {
        BattleEffectContext *battle = (BattleEffectContext *)func_001AA6F8();
        u32 object;
        func_0022A908(record->group);
        object = battle->targetObject;
        if (object != 0) {
            scrSetCurrentActor(object, record->object);
        }
    }
    if (btlReleaseScriptResource()) {
        return 1;
    }
    ++record->frames;
    return 0;
}

typedef struct BattleActionTask {
    u8 active;
    u8 unk_01[0xf];
    u8 phase;
    u8 unk_11[0xf];
    s16 kind;
    u8 unk_22[0x2a];
    s32 (*update)(void *);
} BattleActionTask;

typedef struct BattleActionTaskData {
    void *battler;
    s32 action;
    s32 finished;
} BattleActionTaskData;

void *btlCreateActionTask(void *battler, s32 action) {
    BattleActionTask *task = (BattleActionTask *)btlAllocTask(12);
    BattleActionTaskData *data;

    task->active = 1;
    task->kind = 0x69;
    task->update = (s32 (*)(void *))btlUpdateScriptResourceTask;
    task->phase = 0;
    data = (BattleActionTaskData *)func_001E14F8((u32)task);
    data->battler = battler;
    data->action = action;
    data->finished = 0;
    return task;
}

s32 btlHasRestrictedUnit(void) {
    BattleCombatant *unit = *(BattleCombatant **)(func_001AA6F8() + 0x24C);
    while (unit != 0) {
        u32 status = unit->status;
        if (status & 0x200) {
            if (status & 0xe0) {
                return 1;
            }
            if (unit->ailment & 0x4000) {
                return 1;
            }
        }
        unit = unit->next;
    }
    return 0;
}

s32 btlListHasMarkedFlag(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        BattleListEntry *entry = (BattleListEntry *)entries[i];
        if ((entry->flags & 0x7fff) == 0x4000) {
            return 1;
        }
    }
    return 0;
}

s32 btlListCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        BattleListEntry *entry = (BattleListEntry *)entries[i];
        if (entry->primaryCount < entry->primaryLimit) {
            return 0;
        }
    }
    return 1;
}

s32 btlListSecondaryCountersWithinLimits(u8 **entries, s32 count) {
    s32 i;
    for (i = 0; i < count; i++) {
        BattleListEntry *entry = (BattleListEntry *)entries[i];
        if (entry->secondaryCount < entry->secondaryLimit) {
            return 0;
        }
    }
    return 1;
}

s32 btlListHasMatchingFlag(u8 **entries, s32 count, u32 flags) {
    s32 i;
    for (i = 0; i < count; i++) {
        BattleListEntry *entry = (BattleListEntry *)entries[i];
        if ((entry->flags & 0x7fff) & flags) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022BDC0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022BEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B7D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B7E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B800);

u16 func_0022C040(u8 **entries, s32 count, s32 unused, s32 command) {
    s32 result = -1;
    if (D_00435E20[command * 0x38 + 9] & 1) {
        if (*(s32 *)(D_00435E20 + command * 0x38 + 0x28) == 0) {
            switch (*(u16 *)(D_00435E20 + command * 0x38 + 0x16)) {
            case 2:
            case 5:
            case 7:
            case 9:
            case 11:
            case 15:
                result = btlListCountersWithinLimits(entries, count) ? 3 : 0;
                break;
            }
        }
        if (result != 0) {
            if (*(s32 *)(D_00435E20 + command * 0x38 + 0x28) == 0) {
                switch (*(u16 *)(D_00435E20 + command * 0x38 + 0x1A)) {
                case 2:
                case 5:
                case 7:
                case 9:
                case 11:
                case 15:
                    result = btlListSecondaryCountersWithinLimits(entries, count) ? 3 : 0;
                    break;
                }
            }
            if (result != 0) {
                if (*(s32 *)(D_00435E20 + command * 0x38 + 0x28) == 0) {
                    if (D_00435E20[command * 0x38 + 0x24] == 2) {
                        result = btlListHasMatchingFlag(entries, count, *(u16 *)(D_00435E20 + command * 0x38 + 0x26)) == 0 ? 3 : 0;
                    }
                }
            }
        }
    }
    return (result < 0) ? 0 : result;
}

typedef struct BtlCommandRecord {
    u8 flags;
    u8 unk_01[0xB];
    u16 restriction;
    u8 unk_0E[0x16];
    u32 attributeBits;
    u8 unk_28[0x10];
} BtlCommandRecord;

s32 func_0022C1B0(BtlTask *task, s32 command) {
    BtlCommandRecord *record;
    void *list;
    s32 count;
    s32 flaggedCount;
    s32 i;
    if (*(u32 *)(func_001AA6F8() + 0x220) & 0x800) {
        return 0;
    }
    if (command <= 0) {
        return 0;
    }
    record = (BtlCommandRecord *)(command * 0x38 + (s32)D_00435E20);
    if ((record->attributeBits & 0x400000FF) == 0x40000002) {
        if ((~record->restriction & 0x7FFF) == 0x4000) {
            if (btlHasRestrictedUnit() == 0) {
                return 2;
            }
        }
    }
    flaggedCount = 0;
    list = btlAllocateIndexList(0xD);
    func_001AC360((s32)task, (s32)list, 0);
    count = func_001E8058(list);
    if (D_00435E20[command * 0x38] & 8) {
        for (i = 0; i < count; i++) {
            if (((BtlUnit *)func_001E8060(list, i))->unk_12E & 0x800) {
                flaggedCount++;
            }
        }
    }
    func_001E8018(list);
    if (count != 0) {
        if (count != flaggedCount) {
            return 0;
        }
    }
    return 0xA;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C308);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C518);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022C600);

typedef struct BattleModelEntry {
    s32 kind;
    s32 id;
    s32 refs;
    s8 state;
    u8 unk_0d[3];
    void *resource;
    void *actor;
    struct BattleModelEntry *prev;
    struct BattleModelEntry *next;
} BattleModelEntry;

BattleModelEntry *btlCreateModelEntry(void) {
    BattleModelEntry *entry = func_00328E18(sizeof(BattleModelEntry));
    u8 *battle;
    BattleModelEntry *head;
    entry->refs = 1;
    entry->state = 0;
    battle = (u8 *)func_001AA6F8();
    entry->prev = 0;
    head = *(BattleModelEntry **)(battle + 0x264);
    if (head != 0) {
        head->prev = entry;
        entry->next = *(BattleModelEntry **)(battle + 0x264);
    } else {
        entry->next = 0;
    }
    *(BattleModelEntry **)(battle + 0x264) = entry;
    return entry;
}

void btlReleaseModelEntry(BattleModelEntry *entry) {
    if (--entry->refs != 0) {
        return;
    }
    if (entry->resource != 0) {
        func_002C7CE8(entry->resource);
    }
    if (entry->actor != 0) {
        sndReleaseSlotOwner(entry->actor);
    }
    if (entry->next != 0) {
        entry->next->prev = entry->prev;
    }
    if (entry->prev != 0) {
        entry->prev->next = entry->next;
    } else {
        *(BattleModelEntry **)((u8 *)func_001AA6F8() + 0x264) = entry->next;
    }
    func_00328E48(entry);
    func_0020D128("btl:pack free[%X,%X]\n", entry->kind, entry->id);
}

void btlReleaseAllModelEntries(void) {
    BattleModelEntry *entry = *(BattleModelEntry **)((u8 *)func_001AA6F8() + 0x264);
    BattleModelEntry *next;
    while (entry != 0) {
        next = entry->next;
        btlReleaseModelEntry(entry);
        entry = next;
    }
}

void btlFormatModelResourcePath(s32 isDevil, s32 modelId, char *filename) {
    extern void func_0035C860(char *, const char *, const char *, s32);
    if (isDevil == 0) {
        func_0035C860(filename, "%spc%03X_ms.LB", "/model/human/", modelId);
    } else {
        func_0035C860(filename, "%s%03X_ms.LB", "/model/devil/", modelId);
    }
}

s32 func_0022C948(u8 *task) {
    s32 result;
    if (*(s8 *)(task + 0xc) != 0) {
        return 1;
    }
    if (mdlRequestAsset(*(s32 *)task, *(s32 *)(task + 4), 0) == 0 ||
        mdlRequestAsset(*(s32 *)task, *(s32 *)(task + 4), 0) == -1) {
        return 0;
    }
    if (*(void **)(task + 0x10) == 0) {
        return 1;
    }
    result = fileRequestIsReady(*(void **)(task + 0x10));
    return (s8)result;
}

s32 btlFindModelEntry(kind, id)
s32 kind;
s32 id;
{
    BattleModelEntry *entry = *(BattleModelEntry **)((u8 *)func_001AA6F8() + 0x264);
    while (entry != 0) {
        if (entry->kind == kind && entry->id == id) {
            return (s32)entry;
        }
        entry = entry->next;
    }
    return 0;
}

void func_0022CA40(void) {
}

void func_0022CA48(void) {
    btlReleaseAllModelEntries();
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CA60);

void func_0022CB68(void) {
    s32 entry;

    entry = btlFindModelEntry();
    if (entry != 0) {
        btlReleaseModelEntry((BattleModelEntry *)entry);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CBA0);

s32 btlGetEntryState(s32 kind, s32 value) {
    u8 *entry = (u8 *)btlFindModelEntry(kind, value);
    if (entry != 0) {
        return *(s8 *)(entry + 0xc);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CD30);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CD60);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CE30);

void func_0022CF58(s32 packet, s32 first, s32 second, s32 color) {
    sdfBuildPacketE(packet, second, first, 0x7000, 0x7900, 0x9000, 0x7900, 0x7000,
                  0x8700, 0x9000, 0x8700, color, 0);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022CFB0);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022D040);

void btlReleaseOwnedData(void) {
    void *data = D_00453060.ownedData;
    if (data != 0) {
        sdfFreeMemoryFromEitherHeap(data);
        D_00453060.ownedData = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022D2F8);

void func_0022DD18(void) {
    func_0022D040();
    D_00453060.color10 = 0x80808080;
    if (D_00453060.unk_06 < 2) {
        D_00453060.color14 = 0x20FFFFFF;
    } else {
        D_00453060.color14 = 0x00FFFFFF;
    }
}

s32 func_0022DD70(void) {
    func_0022D2F8(D_00453060.color14, D_00453060.color10);
    if ((D_00453060.color14 & 0xFF000000) != 0) {
        D_00453060.color14 -= 0x08000000;
        return 0;
    }
    return 1;
}

void func_0022DDC8(void) {
}

s32 func_0022DDD0(void) {
    s32 result = func_0022D2F8(D_00453060.color14, D_00453060.color10);
    if ((D_00453060.color10 & 0xFF000000) > 0x08000000) {
        D_00453060.color10 -= 0x08000000;
    } else {
        D_00453060.color10 &= 0x00FFFFFF;
    }
    return result;
}

void func_0022DE38(void) {
    u8 *entry;
    s32 i;
    u64 clearValue;
    func_001053F0(0x200, 0xe0, 0.0f);
    kwlnTextureSetReferenceFlagIfPresent();
    entry = D_00380870;
    i = 0;
    clearValue = 0x80008000ULL;
    entry += 0x1b70;
    do {
        i++;
        *(u64 *)(entry - 0x10) = clearValue;
        *(u64 *)entry = clearValue;
        entry += 0x1f40;
    } while (i != 2);
    D_00453060.options |= 2;
}

void func_0022DEB8(void) {
    func_00105538();
    kwlnTextureReleaseHeldReference();
}

void btlInitializeGraphicsRuntime(void) {
    BattleRuntimeState *runtime = &D_00453060;
    void *surface;
    void *context;
    runtime->handle = func_003293C8(0x70000);
    runtime->resource = sdfResourceRetainAddress(runtime->handle);
    surface = sdfAllocatePacketList(0);
    context = sdfAllocPacketAligned(16);
    func_0032D3F0(context);
    func_0032C768(surface, context, 0, 0, 0x200, 0xe0, runtime->resource, 0, 0, 0);
    func_0032D438(D_00380860, context);
    D_00380608.invoke(&D_00380608, surface);
}

void func_0022DF98(void) {
    BattleRuntimeState *runtime = &D_00453060;
    void *surface = sdfAllocatePacketList(0);
    sdfCreateDescriptorPacket(surface, *(s32 *)(D_00435CC8 + 0x10), 0, 0, 0x200, 0xe0, runtime->resource, 0);
    D_00380608.invoke(&D_00380608, surface);
    func_003298C0(runtime->handle);
    runtime->handle = 0;
    runtime->resource = 0;
    runtime->options |= 1;
}

void btlInitializeOverlayGraphics(void) {
    void *surface = sdfAllocatePacketList(0);
    void *context = sdfAllocPacketAligned(16);
    func_0032D3F0(context);
    func_0032EB80(surface, context, *(s32 *)(D_00435CC8 + 0x10), 0, 0, 0, 0, 0x200, 0xe0, 0, 0);
    func_0032D438(D_00380860, context);
    D_00380608.invoke(&D_00380608, surface);
    D_00453060.options |= 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022E0E0);

void func_0022E338(void) {
    BattleRuntimeState *state = &D_00453060;
    memset(state, 0, sizeof(*state));
    state->flags = 0;
    state->state = 0;
    state->unk_06 = 0;
    state->pending = 0;
    state->active = 0;
    state->options = 0;
    state->handle = 0;
    state->resource = 0;
}

void btlResetRuntimeState(void);

void btlResetAsyncState(void) {
    void *handle = D_00453060.handle;
    if (handle != 0) {
        func_003298C0(handle);
        D_00453060.handle = 0;
        D_00453060.resource = 0;
    }
    btlResetRuntimeState();
}

void btlActivateRuntime(u8 condition) {
    BattleRuntimeState *battle = &D_00453060;
    battle->unk_06 = condition;
    battle->flags = 0;
    battle->state = 1;
    battle->active = 1;
    battle->pending = 0;
    battle->options = 0;
    if (func_00106108() != 0) {
        battle->options |= 4;
    }
}

void btlResetRuntimeState(void) {
    D_00453060.state = 0;
    D_00453060.active = 0;
    btlReleaseOwnedData();
}

s32 func_0022E450(void) {
    return D_00453068[0];
}

s32 func_0022E460(void) {
    u16 state;

    if (D_00453060.active == 0) {
        return 1;
    }
    state = D_00453060.state;
    if (state == 0) {
        return 1;
    }
    return state == 2;
}

s32 func_0022E490(void) {
    u16 state;

    if (D_00453060.active == 0) {
        return 1;
    }
    state = D_00453060.state;
    if (state == 0) {
        return 1;
    }
    return state == 4;
}

void func_0022E4C0(void) {
    if (D_00453060.active != 0) {
        D_00453060.pending = 1;
    }
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B9D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BB98);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BBF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BCF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041BEC8);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022E4E0);

typedef struct MenuList {
    u32 count;
    u32 cursor;
    u32 top;
    u32 rows;
} MenuList;

s32 mnuListMoveCursor(MenuList *list) {
    if (D_0040B7D8->b6 & 2) {
        if (list->cursor != 0) {
            list->cursor--;
            if (list->cursor == list->top && list->cursor != 0) {
                list->top = list->cursor - 1;
            }
        } else {
            list->cursor = list->count - 1;
            list->top = list->count - list->rows;
        }
    } else if (D_0040B7D8->b7 & 2) {
        if (list->cursor >= list->count - 1) {
            list->cursor = 0;
            list->top = 0;
        } else {
            list->cursor++;
            if (list->cursor == list->top + list->rows - 1 && list->top < list->count - list->rows) {
                list->top++;
            }
        }
    } else if (D_0040B7D8->b5 & 2) {
        if (list->top + list->rows * 2 < list->count) {
            list->top += list->rows;
            list->cursor += list->rows;
        } else {
            list->cursor = list->count - 1;
            list->top = list->count - list->rows;
        }
    } else if (D_0040B7D8->b4 & 2) {
        if (list->top >= list->rows) {
            list->top -= list->rows;
            list->cursor -= list->rows;
        } else {
            list->cursor = 0;
            list->top = 0;
        }
    } else if (D_0040B7D8->stick < 0) {
        return 1;
    }
    return 0;
}

s32 func_0022ED38(s32 width, s32 height, s32 mode) {
    void *packet = func_0019F448(width << 4, height << 3, 0xff0000, 0xa09dc380, mode, 0);
    func_0019D550(packet, 0, 0x60);
    return func_0019C5B0(packet);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022ED90);

s32 func_0022EE88(u8 *first, u8 *second, s32 mode, u8 *settings, s32 extra) {
    s32 offset = *(s32 *)(settings + 0xc) * 24 + 4;
    func_0020D1C0(first - 4, second - 4, mode, offset, 0x80806020, 0x30000000);
    return func_0022ED90(first, second, mode, settings, extra);
}

s32 func_0022EF18(u8 *x, u8 *y, s32 mode, u8 *menu, s32 extra) {
    void *handle;
    u32 first;
    u32 count;
    u32 index;
    u32 end;
    u32 selected;
    s32 rowY;
    func_0020D1C0(x - 4, y - 4, mode, *(s32 *)(menu + 0xC) * 24 + 4, 0x80806020, 0x30000000);
    handle = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(handle);
    first = *(u32 *)(menu + 8);
    count = *(u32 *)(menu + 0xC);
    end = first + count;
    index = first;
    selected = *(u32 *)(menu + 4);
    if (index < end) {
        rowY = (s32)y * 8 + 0x7900;
        for (; index < end; index++) {
            s32 flag = 4;
            if (index != selected) {
                flag = 0;
            }
            sdfAppendPacket(handle, func_0033D810((s32)x * 16 + 0x7000, rowY, 0xFF0000, flag, D_00436EF8, index));
            rowY += 0xC0;
        }
    }
    D_00380748.draw(&D_00380748, (s32)handle);
    return func_0022ED90(x + 0x2C, y, mode, menu, extra);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_0022F068);

INCLUDE_ASM(const s32, "game/code_00227288", func_0022F180);

INCLUDE_ASM(const s32, "game/code_00227288", func_002303D0);

void func_00230960(void) {
    D_00436F5D = 0;
    dds3WorkClear();
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C118);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C128);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C138);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C148);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C158);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C168);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C178);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C188);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C198);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C1F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C208);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C218);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C228);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C238);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C248);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C258);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C268);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C278);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C288);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C298);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C2D8);

INCLUDE_ASM(const s32, "game/code_00227288", func_00230978);

INCLUDE_ASM(const s32, "game/code_00227288", func_00230D90);

void func_002311C0(void) {
    s32 i;

    D_00438F90 = sdfCreateSemaphore(1, 0x7f, 0);
    for (i = 0; i != 8; i++) {
        D_003C86F0[i] = 0;
        D_003C8710[i] = 0;
    }
}

s32 *btlFindGroupedEntity(group, type)
    s32 group;

    s32 type;

{
    s32 *entry = (s32 *)D_003C86F0[group];
    while (entry != 0) {
        if (*(u16 *)((u8 *)entry + 0xa) == type) {
            break;
        }
        entry = (s32 *)*entry;
    }
    return entry;
}

s32 btlGroupContainsId(s32 group, s32 id) {
    s32 *entry = (s32 *)D_003C8710[group];
    while (entry != 0) {
        if (entry[1] == id) {
            return 1;
        }
        entry = (s32 *)*entry;
    }
    return 0;
}

void btlAddGroupId(s32 group, s32 id) {
    BattleGroupIdEntry *node = func_00328D68(sizeof(BattleGroupIdEntry));
    BattleGroupIdEntry **head = (BattleGroupIdEntry **)&D_003C8710[group];
    node->id = id;
    node->next = *head;
    *head = node;
}

void btlRemoveGroupId(s32 group, s32 id) {
    s32 *link;
    s32 *node;

    link = &D_003C8710[group];
    node = (s32 *)*link;
    if (node == 0) {
        return;
    }
    do {
        if (*(node + 1) == id) {
            *link = *node;
            func_00328E48(node);
            break;
        } else {
            link = node;
            node = (s32 *)*node;
        }
    } while (node != 0);
}

void btlCreateGroupNode(s32 group, s32 type, s32 flag, s32 arg3, s32 arg4, s32 arg5) {
    BattleGroupNode *node;
    BattleGroupNode *head;
    s32 i;
    func_00231588(group, type);
    node = func_00328D68(sizeof(BattleGroupNode));
    head = (BattleGroupNode *)D_003C86F0[group];
    if (head != NULL) {
        head->prev = node;
    }
    D_003C86F0[group] = (s32)node;
    node->next = head;
    node->group = group;
    node->type = type;
    node->resourceList = arg3;
    node->unk_18 = arg4;
    node->ownedResource = arg5;
    node->prev = NULL;
    node->unk_10 = 0;
    for (i = 0; i != 8; i++) {
        node->slots[i].unk_0 = 0;
        node->slots[i].unk_8 = 0;
        node->slots[i].resourceHandle = 0;
    }
    node->flag = flag & 1;
    node->unk_A0 = 0;
    node->unk_A4 = 0;
    node->unk_A8 = 0;
    node->unk_AC = 1.0f;
    node->unk_B0 = 100.0f;
}

void btlDestroyGroupNode(BattleGroupNode *node) {
    BattleGroupNode *prev;
    BattleGroupNode *next;
    u8 flag;
    s32 i;
    if (node == NULL) {
        return;
    }
    prev = node->prev;
    next = node->next;
    if (prev == NULL) {
        D_003C86F0[node->group] = (s32)next;
    } else {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    flag = node->flag;
    node->flag = 0;
    if (node->unk_10 != 0) {
        do {
            func_002322E8(node->unk_10);
        } while (node->unk_10 != 0);
    }
    if (flag != 0) {
        sdfResourceListRelease((void *)node->resourceList, 1);
        func_003298C0((void *)node->ownedResource);
        for (i = 0; i != 8; i++) {
            if (node->slots[i].resourceHandle != 0) {
                func_003297C8(node->slots[i].resourceHandle);
            }
        }
    }
    func_00234858(node->unk_A8);
    func_003297C8(node->unk_A0);
    func_00328E48(node);
}

void func_00231588(void) {
    s32 *node;

    node = btlFindGroupedEntity();
    btlDestroyGroupNode((BattleGroupNode *)node);
}

void btlReleaseAllEntities(void) {
    u32 i = 0;
    s32 *head = D_003C86F0;
    do {
        s32 *node = (s32 *)*head;
        while (node != 0) {
            s32 *next = (s32 *)*node;
            btlDestroyGroupNode(node);
            node = next;
        }
        i++;
        head++;
    } while (i < 8);
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00231618);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C300);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C320);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C330);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C350);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C360);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C380);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C390);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C3F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C410);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C420);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C430);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C450);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C460);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C480);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C490);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C4F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C500);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C510);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C520);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C530);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C540);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C560);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C570);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C590);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C5F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C600);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C610);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C630);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C640);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C650);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C660);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C680);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C690);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C6F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C700);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C720);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C730);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C740);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C780);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C790);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C7F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C800);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C820);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C830);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C840);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C860);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C880);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C890);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C8F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C900);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C910);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C920);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C940);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C950);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C960);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C970);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C980);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041C9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CBF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CCF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CD90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CED0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CEF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CF90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041CFF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D010);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D030);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D050);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D070);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D090);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D0F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D100);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D110);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D130);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D140);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D150);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D160);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D170);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D180);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D190);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D1F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D200);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D210);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D220);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D230);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D240);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D250);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D260);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D270);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D280);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D290);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D2F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D300);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D320);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D330);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D350);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D360);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D380);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D390);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D3F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D410);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D420);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D430);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D450);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D460);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D480);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D490);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D4F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D500);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D510);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D520);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D530);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D540);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D560);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D570);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D590);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D5F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D600);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D610);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D630);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D640);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D650);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D660);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D680);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D690);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D6F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D700);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D720);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D730);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D740);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D780);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D798);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D7F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D828);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D840);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D858);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D888);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D8E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D900);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D918);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D948);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D960);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D978);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041D9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DBF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DCF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DD90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DED0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DEF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DF90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041DFF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E010);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E030);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E050);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E070);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E090);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E0F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E100);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E110);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E130);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E140);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E150);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E160);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E170);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E180);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E190);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E1F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E200);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E210);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E220);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E230);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E240);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E250);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E260);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E270);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E280);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E290);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E2F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E300);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E320);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E330);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E350);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E360);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E380);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E390);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E3F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E410);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E420);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E430);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E450);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E460);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E480);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E490);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E4F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E500);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E510);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E520);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E530);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E540);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E560);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E570);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E590);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E5F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E600);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E610);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E630);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E640);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E650);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E660);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E680);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E690);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E6F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E700);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E720);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E730);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E740);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E780);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E790);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E7F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E800);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E820);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E830);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E840);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E860);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E880);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E890);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E8F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E900);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E910);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E920);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E940);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E950);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E960);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E970);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E980);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041E9F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EA90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EBF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EC90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ECF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041ED90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EDF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EE90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EED0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EEF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EF90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041EFF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F010);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F030);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F050);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F070);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F098);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F0F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F108);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F138);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F158);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F170);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F188);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F1E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F200);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F218);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F230);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F248);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F260);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F270);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F288);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F2F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F310);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F328);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F340);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F358);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F370);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F388);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F3E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F400);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F418);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F428);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F440);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F458);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F470);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F488);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F4F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F508);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F518);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F528);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F538);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F550);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F568);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F580);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F5A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F5C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F5F8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F620);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F648);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F670);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F698);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F6C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F6E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F710);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F738);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F760);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F788);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F7B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F7D8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F800);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F828);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F878);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F8A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F8C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F8F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F918);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F940);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F968);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F990);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F9B8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041F9E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FA78);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FAA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FAC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FAF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB18);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FB90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FBB8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FBE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC58);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FC80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FCA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FCD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FCF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD48);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FD98);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FDC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FDE8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FE88);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FEB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FED8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF00);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF28);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FF78);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FFA0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FFC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041FFE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420000);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420020);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420040);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420060);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420080);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004200A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004200C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004200E0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420100);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420120);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420140);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420168);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420188);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004201A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004201C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004201E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420208);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420228);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420248);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420268);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420288);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004202A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004202C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004202E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420308);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420328);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420348);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420368);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420388);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004203A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004203C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004203E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420408);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420428);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420448);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420468);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420488);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004204A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004204C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004204E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420508);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420528);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420548);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420568);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420588);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004205A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004205C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004205E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420608);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420628);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420648);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420668);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420688);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004206A8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004206C8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004206E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420708);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420728);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420750);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420770);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420790);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004207B0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004207D0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004207F0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420810);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420830);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420850);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420870);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420898);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004208C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004208E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420910);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420930);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420958);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420978);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004209A0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004209C0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_004209E8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A58);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420A80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420AA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420AD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420AF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B18);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B68);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420B90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420BB8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420BE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C08);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C58);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420C80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420CA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420CD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420CF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D18);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D38);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420D80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420DA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420DD0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420DF0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E78);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420E98);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EB0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EC8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EE0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420EF8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F10);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F20);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F30);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F40);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F50);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F60);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F70);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F80);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420F90);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420FA8);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420FC0);

INCLUDE_RODATA(const s32, "game/code_00227288", D_00420FD8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436CF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D8E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5D);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5F);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F61);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F62);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F64);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F74);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F7C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F84);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F96);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F98);

