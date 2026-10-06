#include "common.h"
#include "pcp_vu0.h"
#include "btl_ui.h"
#include "sdf.h"
#include "btl.h"
#include "eff.h"
#include "btl_action.h"

extern void sdfReleaseChipBlock(void *block);

extern void btlBossDebugPrintf(const char *format, ...);


typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;


typedef struct BattleItemDrop {
    u16 id;
    u8 count;
    u8 pad03;
} BattleItemDrop;

typedef struct SceneFadingRecord {
    SceneSlot slot;
    u8 alpha;
    s32 target;
} SceneFadingRecord;

typedef struct BattleController {
    u8 pad_000[0x1F4];
    u32 flags;
    u8 pad_1F8[0x2C];
    BtlTask *taskHead; /* 0x224 */
    BtlUnit *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad_254[0x1C];
    s32 adjustmentRecordIndex;
    s32 adjustmentGroupIndex;
    s32 adjustmentEntryIndex;
    s32 mode;
    u8 pad_280[0x1C];
    s32 taskParent;
    u8 pad_2A0[8];
    s32 sceneObjectTask;
    s32 spriteObject;
    s32 cleanupTask;
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2C0 */
    u8 pad2C4[4];
    s32 experienceEarned; /* 0x2C8 */
    s32 epEarned; /* 0x2CC */
    u8 pad2D0[4];
    SceneSlot slots[8];
    BtlTask *groupPrimary[20];
    BtlTask *groupSecondary[45];
    BtlTask *groupTertiary[15];
    u8 pad_42C[0x20];
    SceneFadingRecord fadingRecords[8]; /* 0x44C */
    u8 pad_48C[0x18];
    EffectSlotSet *resA; /* 0x4A4 */
    EffectSlotSet *resB;
    EffectSlotSet *resC;
    u8 pad_4B0[0x100];
    s32 (*sceneCallback)();
} BattleController;

/* The saved party profile is 0x1A4 bytes; battle units embed its stat data. */
typedef struct BtlEntry {
    u16 flags;
    u8 pad2[4];
    u16 hp;
    u16 maxHp;
    u16 mp;
    u8 padC[2];
    u16 status;
    u32 totalExp;
    u16 unk14;
    u8 unk16[5];
    u8 pad1B[0x171];
    u16 unk18C;
    u16 unk18E;
    u16 unk190;
    u8 pad192[0x12];
} BtlEntry;

typedef struct EntryPair {
    s16 first;
    s16 second;
    s16 initialValue;
    s16 countdown;
} EntryPair;

extern EntryPair D_003583D0[];


extern s32 datComputeSkillBoostedMaxHp();

extern s32 btlGetEffectActive(void);

extern s32 datComputeSkillBoostedMaxMp();

extern void func_001BCB88(s32, s32);

extern s32 datGetStatWithStatusOverride(s32, s32);


typedef struct BtlActorWork {
    u8 pad_00[0x228];
    BtlUnit *actorList;
} BtlActorWork;

typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    s8 edge22;
    s8 edge23;
    u8 pad24[2];
    s8 prev;
    s8 next;
} SndPad;

extern SndPad D_00324510;

extern void *sdfAllocAndClearQuadwords(s32);

extern s8 D_00324530[];

extern u8 D_003583A0[];

extern void *D_00358408[];

extern void *D_00358450[];

extern s32 D_00358510[];

extern s32 D_00359A78[];

extern s32 mdlFlagTest(u32);

extern s32 datEnemyRecords;

extern u32 fldGetSceneScriptTaskUserData(void);

extern u64 func_001978E8(s32, s32, u64, u64, u64, u64);

extern BattleTrackedTaskWork *btlTrackedTaskHandles;

extern u32 D_003BB3DC;

extern u32 D_003BD834;

extern u32 D_003BD838;

extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;

extern u32 D_003BB3A8;

extern u32 btlCommandPanelTaskNameRef;

extern u32 D_003BB3B0;

extern u32 D_003BB3AC;

extern u32 D_003BB3BC;

extern u32 D_003BB3C4;

extern u32 btlAnalyzPanelTaskNameRef;

extern u32 kwlnTaskGetUserValue(s64);

extern u32 btlMahenPanelTaskNameRef;

extern s64 kwlnTaskGetTaskByName(u32);

extern s64 kwlnTaskIsRegistered(s64);

extern s32 btlGetTrackedTaskHandle(s32);

extern BattleCmdPanel *btlCommandPanelWork;

extern s32 func_001A8DD8(s32, s32 *);

extern s32 btlCheckSpecialAbility(s32, s32);

extern void func_001B83D8(s32, s32, s32);

extern s32 btlGetRuntime(void);

extern u8 *datBattleSceneRecords;

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern char D_003BB450[];

extern s32 datItemSkillRecords;

extern s32 D_003BAA14;

extern s32 D_003BAA28;

extern s32 D_003BAA10;

extern s32 D_003BAA20;

extern s32 D_003BAA30;

extern s32 datCommandSelectors;

extern s32 datCommandRecords;

extern s32 datRosterDetails;

extern s32 fldCountSceneSlots(void);

extern s32 effMiscRandMod(s32, s32);

extern char D_003A1A40[];

typedef struct BattleFearState {
    u8 pad_000[0x1FC];
    u32 flags;
} BattleFearState;

extern s32 datGameState;

extern s32 datAffinityRecords;

extern s32 dds3FindEntryIndex();

extern s32 btlGetIndexedPartyEntryRecord(s32);

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern s8 effSharedRandomState[];

extern s32 sdfAllocGeneralBlock(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern void func_001C45F0(void);

extern void itfMesDestroyWindowIfPresent(s32);

void func_001A1960() {
    datClearUnitStatusBits();
}

/* Set the actor's selected entry index. */
void btlSetActorSelectedEntryIndex(s32 actor, u32 index) {
    ((BtlUnit *)actor)->selectedEntryIndex = index;
}

/* No selected entry is represented by -1. */
void btlClearActorSelectedEntryIndex(s32 actor) {
    ((BtlUnit *)actor)->selectedEntryIndex = -1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A1990);

s32 btlGetActorEntryData(s32 arg0) {
    s32 temp_v0;

    if ((*(u32 *)(arg0 + 0x110) & 0x400) == 0) {
        temp_v0 = btlGetIndexedPartyEntryRecord(*(u8 *)(arg0 + 0x2c4));
        return temp_v0;
    }
    return arg0 + 0x120;
}

s32 btlGetCurrentPartyEntryRecord(void) {
    s32 temp_v0;

    temp_v0 = dds3FindEntryIndex();
    return datGameState + temp_v0 * 0x1a4 + 0xa60;
}

s32 btlGetIndexedPartyEntryRecord(s32 arg0) {
    return datGameState + arg0 * 0x1a4 + 0xa60;
}

void btlSyncPlayerWork(BtlUnit *actor) {
    BtlEntry *src = (BtlEntry *)&actor->statBits;
    BtlEntry *dst = (BtlEntry *)btlGetIndexedPartyEntryRecord(actor->unk2C4);
    s32 maxHp;
    s32 maxMp;
    if (src->flags & 0x1000) {
        dst->flags |= 0x1000;
    } else {
        dst->flags &= ~0x1000;
    }
    if (src->flags & 0x4000) {
        dst->flags |= 0x4000;
    } else {
        dst->flags &= ~0x4000;
    }
    dst->unk14 = src->unk14;
    maxHp = datComputeSkillBoostedMaxHp(dst);
    maxMp = datComputeSkillBoostedMaxMp(dst);
    dst->hp = src->hp < maxHp ? src->hp : maxHp;
    dst->mp = src->mp < maxMp ? src->mp : maxMp;
    memcpy(dst->unk16, src->unk16, 5);
    dst->status = src->status & 0x7FFF;
    dst->unk18C = src->unk18C;
    dst->unk18E = src->unk18E;
    dst->unk190 = src->unk190;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

s32 btlFindPartyEntryIndexForActor(s32 arg0) {
    return dds3FindEntryIndex(*(u16 *)(arg0 + 0x124));
}

void func_001A1CD0(void) {
}

s32 btlFindActiveActorByKind(s32 index) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if (index == *(u8 *)(node + 0x2C4)) {
                    return node;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A1D48);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A2258);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A2608);

s32 btlGetEntryFlagsUnlessDisabled(s32 entry) {
    if ((*(u16 *)entry & 4) != 0) {
        return 0;
    }
    return *(s32 *)(datEnemyRecords + *(u16 *)(entry + 4) * 76);
}

void func_001A29B8(void) {
    datGetClampedProfileAdjustedStat();
}

u32 func_001A29D0(s32 arg0, s32 arg1) {
    return datGetStatWithStatusOverride(arg0, arg1);
}

s32 btlApplyCommandAbilityMultiplier(s32 arg0, s32 arg1) {
    u32 value = datCalculateCommandBaseValue(arg0, arg1);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (*(u8 *)(datCommandRecords + arg1 * 56 + 3)) {
    case 1:
        if (btlCheckSpecialAbility(arg0, 0x234)) {
            scale = datAbilityParameters[0x234 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 2:
        if (btlCheckSpecialAbility(arg0, 0x235)) {
            scale = datAbilityParameters[0x235 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    value = (s32)((f32)value * scale);
    return value == 0 ? 1 : value;
}

s8 func_001A2AE8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = datAffinityRecords + arg0 * 0x10;
    return *(s8 *)(temp_v0 - 0x1aa4);
}

s32 btlGetCommandFailureReason(BtlUnit *unit, s32 command) {
    s32 result = 0;
    u32 cost;

    if (command == 0) {
        return 0;
    }
    if (*(u8 *)(datCommandRecords + command * 56 + 2) == 1 ||
        *(u8 *)(datCommandRecords + command * 56 + 3) == 2) {
        if ((unit->conditionFlags & 0x7FFF) == 0x10) {
            return 3;
        }
    }
    if (*(s8 *)(datCommandSelectors + command * 2 + 1) == 2 &&
        (unit->conditionFlags & 0x7FFF) == 0x40) {
        return 5;
    }
    if (*(s8 *)(datCommandSelectors + command * 2 + 1) == 1) {
        if ((unit->conditionFlags & 0x7FFF) == 0x1000) {
            return 4;
        }
        if (fldCountSceneSlots() < (u32)func_001A2AE8(command)) {
            return 6;
        }
    }
    if (*(u8 *)(datCommandRecords + command * 56) & 4) {
        return 0;
    }

    cost = btlApplyCommandAbilityMultiplier((s32)&unit->statBits, command);
    switch (*(u8 *)(datCommandRecords + command * 56 + 3)) {
    case 1:
        if ((*(u8 *)(datCommandRecords + command * 56) & 8) == 0) {
            if (cost >= unit->hp) {
                result = 1;
            }
        } else if (unit->hp < cost) {
            result = 1;
        }
        break;
    case 2:
        result = unit->unk_12A < cost ? 2 : 0;
        break;
    }
    return result;
}

s32 btlGetCombinedPartyCommandPower(BtlEntry *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    BtlEntry snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;
    s32 average;

    if (first != NULL) {
        totalMaxHp = first->maxHp;
        count = 1;
    }
    if (second != NULL) {
        totalMaxHp += second->maxHp;
        count++;
    }
    if (third != NULL) {
        totalMaxHp += third->maxHp;
        count++;
    }
    average = totalMaxHp / count;
    snapshot.maxHp = average;
    snapshot.hp = average;
    return btlApplyCommandAbilityMultiplier((s32)&snapshot, command);
}


s32 func_001A2DE8(BtlUnit *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    BtlUnit snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;

    if (first != NULL) {
        if ((first->flags & 0x100) == 0) {
            return 3;
        }
        if ((first->conditionFlags & 0x2A0E) != 0) {
            return 3;
        }
        totalMaxHp = first->maxHp;
        count = 1;
    }
    if (second != NULL) {
        if ((second->flags & 0x100) == 0) {
            return 3;
        }
        if ((second->conditionFlags & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += second->maxHp;
    }
    if (third != NULL) {
        if ((third->flags & 0x100) == 0) {
            return 3;
        }
        if ((third->conditionFlags & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += third->maxHp;
    }
    snapshot.maxHp = totalMaxHp / count;
    return btlGetCommandFailureReason(&snapshot, command);
}

s8 btlGetActorIndexedSignedValue(s32 object, s32 index) {
    if (index == 0 && (*(u32 *)(object + 0x110) & 0x400) != 0) {
        return *(s8 *)(datEnemyRecords + *(u16 *)(object + 0x124) * 76 + 0x46);
    }
    return *(s8 *)(datCommandSelectors + index * 2);
}

void func_001A2F50(s32 arg0) {
    btlResolveUnitValueWithOverride(arg0 + 0x120);
}

struct DatUnitStatus;
extern s32 datGetEffectiveAffinity(struct DatUnitStatus *, s32);

s32 btlResolveUnitValueWithOverride(s32 object, s32 value) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x670);
    if (handler != 0) {
        s32 result = handler(object, value);
        if (result != -1) {
            return result;
        }
    }
    return datGetEffectiveAffinity((struct DatUnitStatus *)object, value);
}

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA10 + arg1 * 0x270;
    }
    return D_003BAA20 + arg1 * 0x270;
}

s32 btlSelectSharedOrIndexedTransformParameters(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return (s32)D_003583A0;
    }
    return D_003BAA30 + arg1 * 24;
}

s32 btlSelectSideIndexedActorParameterTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA14 + arg1 * 0x74;
    }
    return D_003BAA28 + arg1 * 0x74;
}

extern char D_003A1788[];

s32 btlGetLoggedIndexedCommandItem(s32 index) {
    u16 item = *(u16 *)(datItemSkillRecords + index * 8 + 2);
    btlBossDebugPrintf(D_003A1788, index, item);
    return item;
}

s32 btlGetActorBedAssetIdFromIndex(s32 arg0) {
    return *(u16 *)(arg0 * 8 + datItemSkillRecords + 2);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1788);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A30F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3360);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, BtlIndexList *arg1);
INCLUDE_ASM(const s32, "game/code_001A1960", btlFindEligibleTargetForMultiActorCommand);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3638);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3740);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3CE0);

u32 btlEncodeActorIndexAsSelectionMask(u32 id) {
    u32 mask;
    switch (id) {
    case 0xFFFFFFFF: mask = 0x1; break;
    case 0: mask = 0x2; break;
    case 1: mask = 0x4; break;
    case 2: mask = 0x8; break;
    case 3: mask = 0x10; break;
    case 4: mask = 0x20; break;
    case 5: mask = 0x40; break;
    case 6: mask = 0x80; break;
    case 7: mask = 0x100; break;
    case 8: mask = 0x200; break;
    case 9: mask = 0x400; break;
    case 10: mask = 0x800; break;
    case 11: mask = 0x1000; break;
    case 12: mask = 0x2000; break;
    case 13: mask = 0x4000; break;
    case 14: mask = 0x8000; break;
    case 15: mask = 0x10000; break;
    case 16: mask = 0x20000; break;
    case 17: mask = 0x40000; break;
    case 18: mask = 0x80000; break;
    default: mask = 0; break;
    }
    return mask;
}

extern s32 datFlagToElementIndex(s32);

s32 func_001A4060(s32 flag) {
    return datFlagToElementIndex(flag);
}

s32 btlCheckSpecialAbility(s32 object, s32 flag) {
    if (datUnitHasSkill(object, flag) == 0) {
        return 0;
    }
    switch (flag) {
    case 0x207:
        return evtGetMirroredSolarPhase() == 8;
    case 0x208:
        return evtGetMirroredSolarPhase() == 0;
    default:
        return 1;
    }
}

extern s8 effSharedRandomState[];

s32 btlSelectActorAction(s32 object) {
    s32 result = func_001A4130(object, 1);
    if (result == 0) {
        result = (effMiscRand(effSharedRandomState) & 1) != 0 ? 2 : 7;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4130);

void func_001A4240(u16 item) {
    BattleController *controller = (BattleController *)btlGetRuntime();

    if (item != 0) {
        s32 found = 0;
        u16 i;

        for (i = 0; i < 3; i++) {
            if (controller->itemDrops[i].id == item) {
                found = 1;
                controller->itemDrops[i].count++;
                break;
            }
        }
        if (!found) {
            for (i = 0; i < 3; i++) {
                if (controller->itemDrops[i].id == 0) {
                    controller->itemDrops[i].id = item;
                    controller->itemDrops[i].count = 1;
                    break;
                }
            }
        }
    }
}

/* The 0x4C-byte enemy table supplies skills and all three reward quantities. */
typedef struct DatEnemyRecord {
    u32 flags;            /* 0x00 */
    u8 pad04;
    u8 level;             /* 0x05 */
    u8 pad06[0x12];
    u16 skills[8];        /* 0x18 */
    s32 money;            /* 0x28 */
    u16 unk2C;
    u16 experience;       /* 0x2E */
    u16 huntExperience;   /* 0x30 */
    u8 pad32[0x1A];
} DatEnemyRecord;

extern s32 func_001A9488(u32);
extern s32 btlCalculateEnemyExperienceReward(u8 *, u8 *);
extern s32 btlGetEnemyMoney(u8 *, u8 *);
extern char D_003A17F8[];
extern char D_003A1818[];
extern char D_003A1830[];
extern char D_003A1848[];

void btlAccumulateEnemyDefeatRewards(BtlUnit *enemy) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    DatEnemyRecord *record = &((DatEnemyRecord *)datEnemyRecords)[enemy->mode];
    s32 level = func_001A9488(4);
    s32 enemyLevel = record->level;
    s32 allowance = datBattleParameters->rewardLevelAllowance;
    s32 reward;
    s32 amount;
    s32 item;

    if (level > 60) {
        level = 60;
    }
    if (level >= enemyLevel + allowance && datBattleParameters->rewardDivisor != 0.0f) {
        reward = (s32)(record->unk2C / datBattleParameters->rewardDivisor);
        btlBossDebugPrintf(D_003A17F8, reward, level, enemyLevel, allowance,
                          datBattleParameters->rewardDivisor);
    } else {
        reward = record->unk2C;
    }
    if (record->flags & 0x2000) {
        reward *= 100;
    }
    controller->experienceEarned += reward;
    if ((enemy->flags64 & 0x200800000ULL) == 0) {
        amount = btlCalculateEnemyExperienceReward(NULL, (u8 *)enemy);
        controller->epEarned += amount;
        btlBossDebugPrintf(D_003A1818, controller->epEarned, amount);
    }
    if ((enemy->stateFlags & 0x400) == 0) {
        amount = btlGetEnemyMoney(NULL, (u8 *)enemy);
        controller->moneyEarned += amount;
        btlBossDebugPrintf(D_003A1830, controller->moneyEarned, amount);
    }
    item = func_001A4130((s32)enemy, 0);
    if (item != 0) {
        func_001A4240(item);
    }
    enemy->stateFlags |= 1;
    btlBossDebugPrintf(D_003A1848, enemy);
}

s32 btlAllActiveUnitsReady(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 0xC0) != 0) {
                    return 0;
                }
                if ((flags & 0x20) != 0) {
                    if ((*(u32 *)(node + 0x114) & 1) == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

typedef struct BattleAdjustmentEntry {
    s8 value;
    u8 pad_01[5];
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    u8 pad_00[0x24];
    BattleAdjustmentEntry entries[14];
    u8 pad_78[4];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    BattleAdjustmentGroup groups[4];
    u8 pad_1F0[0x1C];
} BattleAdjustmentRecord;

extern BattleAdjustmentRecord *D_003BAA3C;


f32 func_001A4598(void) {
    BattleController *runtime = (BattleController *)btlGetRuntime();
    s32 adjustment = D_003BAA3C[runtime->adjustmentRecordIndex]
                         .groups[runtime->adjustmentGroupIndex]
                         .entries[runtime->adjustmentEntryIndex]
                         .value;

    if (adjustment < -3) {
        adjustment = -3;
    } else if (adjustment > 3) {
        adjustment = 3;
    }
    return datBattleParameters->adjustmentScale[adjustment + 3];
}

u32 func_001A4630(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(temp_v0 + 0x254);
}

/* Return a random eligible actor task, or 0 if no candidate is available. */
s32 btlChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    BtlTask *node = ((BattleController *)btlGetRuntime())->taskHead;
    for (; node != 0; node = node->next) {
        if ((node->flags & 8) != 0) {
            BtlUnit *actor = node->unit;
            u32 flags = actor->flags;
            if ((flags & 1) != 0) {
                if ((flags & 0x200) != 0) {
                    if ((flags & 2) != 0) {
                        if ((flags & 0xE0) == 0) {
                            candidates[count++] = (s32)node;
                        }
                    }
                }
            }
        }
    }
    if (count != 0) {
        return candidates[effMiscRandMod(0, count)];
    }
    return 0;
}

s32 btlCountAvailableUnits(void) {
    BtlUnit *node = ((BattleController *)btlGetRuntime())->actors;
    s32 count = 0;
    for (; node != 0; node = node->next) {
        u32 flags = node->flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    count++;
                }
            }
        }
    }
    return count;
}

s32 btlCountAvailableParticipants(void) {
    s32 count = 0;
    BtlUnit *node = ((BattleController *)btlGetRuntime())->actors;
    for (; node != 0; node = node->next) {
        u32 flags = node->flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    count++;
                }
            }
        }
    }
    {
        u8 *entry = (u8 *)(datGameState + 0xA60);
        s32 i;
        for (i = 4; i >= 0; i--, entry += 0x1A4) {
            u16 flags = ((BtlEntry *)entry)->flags;
            if ((flags & 1) != 0) {
                if ((flags & 2) == 0) {
                    if ((((BtlEntry *)entry)->status & 0x4000) == 0) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

INCLUDE_ASM(const f32, "game/code_001A1960", func_001A47F0);

void btlClearAllActorEntrySlots(u32 arg0) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        btlClearActorEntrySlot(arg0, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

s32 btlActorEntryIsExpired(BtlUnit *unit, s32 index) {
    if (unit->entrySlots[index].code == 0) {
        return 0;
    }
    return unit->entrySlots[index].countdown < 1;
}

s32 btlMatchActorEntryCode(BtlUnit *unit, s32 index) {
    s16 value = unit->entrySlots[index].code;
    if (D_003583D0[index].first != 0) {
        if (D_003583D0[index].first == value) {
            return 1;
        }
    }
    if (D_003583D0[index].second != 0) {
        if (D_003583D0[index].second == value) {
            return 2;
        }
    }
    return 0;
}

void func_001A4948(BtlUnit *unit, s32 index, s16 delta) {
    s16 code = unit->entrySlots[index].code;
    code += delta;

    if (code > D_003583D0[index].first) {
        code = D_003583D0[index].first;
    }
    if (code < D_003583D0[index].second) {
        code = D_003583D0[index].second;
    }
    if (code != 0) {
        unit->entrySlots[index].unk02 = D_003583D0[index].initialValue;
        unit->entrySlots[index].countdown = D_003583D0[index].countdown;
    }
    unit->entrySlots[index].code = code;
}

void btlSetActorEntryCode(BtlUnit *unit, s32 index, u16 code) {
    unit->entrySlots[index].code = code;
}

void btlClearActorEntrySlot(BtlUnit *unit, s32 index) {
    unit->entrySlots[index].code = 0;
    unit->entrySlots[index].unk02 = -1;
    unit->entrySlots[index].countdown = -1;
}

s16 btlGetActorEntryCode(BtlUnit *unit, s32 index) {
    return unit->entrySlots[index].code;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A17F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1818);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1830);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1848);

f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge) {
    f32 factor;
    s32 stage;

    stage = btlGetActorEntryCode(unit, index);
    factor = 1.0f;
    switch (index) {
    case 2:
    case 3:
        if (stage < 0) {
            if (index == 2) {
                factor = ((f32)-stage + 8.0f) * 0.125f;
            } else {
                factor = ((f32)(-stage * 2) + 8.0f) * 0.125f;
            }
        } else if (stage > 0) {
            if (index == 2) {
                factor = ((f32)-stage * 0.5f + 8.0f) * 0.125f;
            } else {
                factor = ((f32)-stage + 8.0f) * 0.125f;
            }
        }
        break;
    case 0:
    case 1:
    case 4:
        if (stage < 0) {
            if (index == 4 && (unit->flags & 0x200)) {
                factor = ((f32)stage * 0.5f + 8.0f) * 0.125f;
            } else {
                factor = ((f32)stage + 8.0f) * 0.125f;
            }
        } else if (stage > 0) {
            if (index == 4 && (unit->flags & 0x400)) {
                factor = ((f32)stage + 8.0f) * 0.125f;
            } else {
                factor = ((f32)(stage * 2) + 8.0f) * 0.125f;
            }
        }
        break;
    }
    if (index == 0 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 5) > 0) {
        if (btlActorEntryIsExpired(unit, 5) != 0) {
            factor *= 2.5f;
            btlBossDebugPrintf("btl:BUTURIx2\n");
        }
    }
    if (index == 1 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 6) > 0) {
        if (btlActorEntryIsExpired(unit, 6) != 0) {
            factor *= 2.5f;
            btlBossDebugPrintf("btl:MAGICx2\n");
        }
    }
    return factor;
}

void func_001A4C68(BtlUnit *unit, u32 flags, s16 delta) {
    if (flags == 0) {
        return;
    }
    if (flags & 1) {
        func_001A4948(unit, 0, delta);
    }
    if (flags & 2) {
        func_001A4948(unit, 0, -delta);
    }
    if (flags & 4) {
        func_001A4948(unit, 1, delta);
    }
    if (flags & 8) {
        func_001A4948(unit, 1, -delta);
    }
    if (flags & 0x10) {
        func_001A4948(unit, 2, delta);
    }
    if (flags & 0x20) {
        func_001A4948(unit, 2, -delta);
    }
    if (flags & 0x40) {
        func_001A4948(unit, 3, delta);
    }
    if (flags & 0x80) {
        func_001A4948(unit, 3, -delta);
    }
    if (flags & 0x100) {
        func_001A4948(unit, 4, delta);
    }
    if (flags & 0x200) {
        func_001A4948(unit, 4, -delta);
    }
    if (flags & 0x400) {
        func_001A4948(unit, 5, delta);
    }
    if (flags & 0x2000) {
        func_001A4948(unit, 6, delta);
    }
    if (flags & 0x800) {
        if (btlGetActorEntryCode(unit, 0) > 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) > 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) > 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) > 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) > 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
    if (flags & 0x1000) {
        if (btlGetActorEntryCode(unit, 0) < 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) < 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) < 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) < 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) < 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlLowestSetPairIndex);

void btlTickActorEntryCountdowns(u8 *scene) {
    u32 index;
    s16 *timer = (s16 *)(scene + 0x2CA);
    for (index = 0; index < 7; index++, timer += 3) {
        if (*timer >= 0) {
            if (*timer == 0) {
                *timer = -1;
            }
            --*timer;
        }
    }
}

f32 func_001A5030(u8 *actor, s32 attr) {
    f32 scale = 1.0f;

    switch (attr) {
    case 2:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x20D)) {
            scale *= datAbilityParameters[0x20D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x212)) {
            scale *= datAbilityParameters[0x212 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 3:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x20E)) {
            scale *= datAbilityParameters[0x20E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x213)) {
            scale *= datAbilityParameters[0x213 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 4:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x20F)) {
            scale *= datAbilityParameters[0x20F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x214)) {
            scale *= datAbilityParameters[0x214 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 5:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x210)) {
            scale *= datAbilityParameters[0x210 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x215)) {
            scale *= datAbilityParameters[0x215 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 6:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x211)) {
            scale *= datAbilityParameters[0x211 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x216)) {
            scale *= datAbilityParameters[0x216 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    return scale;
}

s32 btlGetAbilityAttributeMultiplierPercent(u8 *actor, s32 attr) {
    u32 value = 100;

    switch (attr) {
    case 0:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23C)) {
            value = (u32)(datAbilityParameters[0x23C - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 2:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23D)) {
            value = (u32)(datAbilityParameters[0x23D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 3:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23E)) {
            value = (u32)(datAbilityParameters[0x23E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 4:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23F)) {
            value = (u32)(datAbilityParameters[0x23F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 5:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x240)) {
            value = (u32)(datAbilityParameters[0x240 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 6:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x241)) {
            value = (u32)(datAbilityParameters[0x241 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 8:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x242)) {
            value = (u32)(datAbilityParameters[0x242 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 9:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x243)) {
            value = (u32)(datAbilityParameters[0x243 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    }
    return value;
}

s32 btlHasMappedSpecialAbilityForSlot(s32 unit, u32 slot) {
    if (slot == 0 && btlCheckSpecialAbility(unit + 0x120, 0x24F) != 0) {
        return 1;
    }
    if (slot == 8 && btlCheckSpecialAbility(unit + 0x120, 0x251) != 0) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(unit + 0x120, 0x252) != 0) {
        return 1;
    }
    if (slot == 10 && btlCheckSpecialAbility(unit + 0x120, 0x244) != 0) {
        return 1;
    }
    if (slot == 11 && btlCheckSpecialAbility(unit + 0x120, 0x245) != 0) {
        return 1;
    }
    if (slot == 12 && btlCheckSpecialAbility(unit + 0x120, 0x246) != 0) {
        return 1;
    }
    if (slot == 13 && btlCheckSpecialAbility(unit + 0x120, 0x247) != 0) {
        return 1;
    }
    if (slot == 14 && btlCheckSpecialAbility(unit + 0x120, 0x248) != 0) {
        return 1;
    }
    if (slot < 0xF) {
        if (slot >= 0xA && btlCheckSpecialAbility(unit + 0x120, 0x253) != 0) {
            return 1;
        }
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(unit + 0x120, 0x257) != 0) {
            return 1;
        }
    }
    if (slot != 7 && btlCheckSpecialAbility(unit + 0x120, 0x249) != 0) {
        return 1;
    }
    return 0;
}

u32 btlHasSpecialAbilityWhenArgumentUnset(s32 arg0, s64 arg1) {
    if (arg1 == 0 && btlCheckSpecialAbility(arg0 + 0x120, 0x254) != 0) {
        return 1;
    }
    return 0;
}

s32 btlHasEnabledSpecialAbilityForSlot(s32 unit, u32 slot) {
    if (slot == 8 && btlCheckSpecialAbility(unit + 0x120, 0x255)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(unit + 0x120, 0x256)) {
        return 1;
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(unit + 0x120, 0x258)) {
            return 1;
        }
    }
    return 0;
}

s32 sndGetResourceForIndex(s32 index) {
    s8 resource = *(s8 *)(datCommandSelectors + index * 2);
    if (resource < 0) {
        return 0;
    }
    return (s32)D_00358408[resource];
}

void *btlGetIndexedUiResource(s32 arg0) {
    return D_00358450[*(u16 *)(arg0 + 0x124)];
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A18F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1908);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1918);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5690);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A57A0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5958);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5C40);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6118);

typedef struct BtlHitResult {
    s32 amount;
    u8 pad04[0x24];
} BtlHitResult;

typedef struct BtlTargetResult {
    u8 hitCount;
    u8 pad01[7];
    u32 kind;
    u8 pad0C[4];
    u8 skipped;
    u8 pad11[3];
    u8 blocked;
    u8 pad15[7];
    BtlHitResult hits[64];
} BtlTargetResult;

s32 btlSumOtherTargetHitAmounts(u8 *action) {
    BtlTargetResult *result = *(BtlTargetResult **)(action + 0x80);
    u32 count = btlGetIndexListCount(*(BtlIndexList **)(action + 0x60));
    u32 i;
    u32 j;
    s32 total = 0;

    for (i = 0; i < count; i++, result++) {
        if (result->skipped != 0 || result->blocked != 0) {
            continue;
        }
        switch (result->kind) {
        case 2:
        case 4:
        case 0x10000:
        case 0x20000:
        case 0x40000:
            break;
        default:
            if (*(BtlUnit **)(action + 0x18) !=
                btlGetIndexListEntry(*(BtlIndexList **)(action + 0x60), i)) {
                for (j = 0; j < result->hitCount; j++) {
                    total += result->hits[j].amount;
                }
            }
            break;
        }
    }
    return total;
}

s32 btlTestActorStatusPredicate(s32 object) {
    s32 (*predicate)(s32) = *(s32 (**)(s32))(btlGetRuntime() + 0x650);
    if (predicate != 0 && predicate(object) != 0) {
        return 1;
    }
    return (((BtlUnit *)object)->conditionFlags & 0x806) != 0;
}

s32 btlComputeStatusPenaltyFifth(s32 object) {
    s32 result = 0;
    u16 category = ((BtlUnit *)object)->conditionFlags & 0x7FFF;
    switch (category) {
    case 0x80:
    case 0x400:
    case 0x2000:
        result = -((BtlUnit *)object)->maxHp / 5;
        break;
    }
    return result;
}

s32 btlRollFearChance(s32 unused, u8 *actor, u32 flags, u32 options) {
    s32 ratio;
    if ((*(u32 *)(btlGetRuntime() + 0x1FC) & 0x80) != 0) return 0;
    if ((((BtlUnit *)actor)->stateFlags & 8) != 0) return 0;
    if ((flags & 1) == 0) return 0;
    if ((((BtlUnit *)actor)->conditionFlags & 1) != 0) return 0;
    ratio = 0;
    if (options & 2) {
        ratio = 30;
    } else if (options & 4) {
        ratio = 40;
    }
    btlBossDebugPrintf("btl:fear ratio[%d]\n", ratio);
    return btlRollAiBucket() < ratio;
}

s32 btlRollAllFearChance(s32 unused, BtlUnit *unit, u32 flags, s32 unusedFlags,
                  u8 useSelectedAction) {
    s32 actionThreshold;
    s32 statusThreshold;
    s32 threshold;
    u32 category;

    if (((BattleFearState *)btlGetRuntime())->flags & 0x80) {
        return 0;
    }
    actionThreshold = 0;
    statusThreshold = 0;
    if (useSelectedAction != 0) {
        if (unit->selectedEntryIndex == -1) {
            return 0;
        }
        category = btlGetActionRecordLookupValue(unit->selectedEntryIndex);
        switch (category) {
        case 0x20000:
        case 0x40000:
            actionThreshold = 40;
            break;
        case 0x10000:
            actionThreshold = 30;
            if (fldCountSceneSlots() >= 3) {
                actionThreshold = 0;
            }
            break;
        }
    }
    if (flags & 0x60000) {
        statusThreshold = 40;
    } else if (flags & 0x10000) {
        statusThreshold = 30;
        if (fldCountSceneSlots() >= 3) {
            statusThreshold = 0;
        }
    }
    threshold = statusThreshold < actionThreshold ? actionThreshold : statusThreshold;
    btlBossDebugPrintf(D_003A1A40, threshold);
    return btlRollAiBucket() < threshold;
}

f32 func_001A6958(void) {
    return 1.5f;
}

typedef struct BattleCommandRangeContext {
    u8 pad_000[0x690];
    s32 (*commandRangeOverride)(BtlUnit *, s32);
} BattleCommandRangeContext;

typedef struct EventModeSlot {
    s8 stat;
    s8 kind;
} EventModeSlot;

typedef struct EventRosterStat {
    s16 base;
    u8 alternateA;
    u8 alternateB;
    f32 multiplier;
    u8 pad08[6];
    u8 rangeMin;
    u8 rangeMax;
    u8 pad10[4];
} EventRosterStat;

typedef struct EventStatRecord {
    u8 pad00[0x11];
    u8 stat11;
    u8 pad12[2];
    u8 rangeMin;
    u8 rangeMax;
    u8 pad16[2];
    s16 stat18;
    u8 pad1A[2];
    s16 stat1C;
    u8 pad1E[7];
    u8 stat25;
    u8 pad26[7];
    u8 stat2D;
    u8 pad2E[6];
    s16 stat34;
    s16 stat36;
} EventStatRecord;

u8 func_001A6968(BtlUnit *unit, s32 command) {
    BattleCommandRangeContext *context = (BattleCommandRangeContext *)btlGetRuntime();
    s32 result;
    s32 minimum;
    s32 maximum;

    if (context->commandRangeOverride != NULL) {
        result = context->commandRangeOverride(unit, command);
        if (result > 0) {
            return result;
        }
    }
    if (command == 0) {
        return 1;
    }
    if (((EventModeSlot *)datCommandSelectors)[command].kind == 5 &&
        (unit->flags & 0x200) != 0) {
        minimum = ((EventRosterStat *)datRosterDetails)[unit->mode].rangeMin;
        maximum = ((EventRosterStat *)datRosterDetails)[unit->mode].rangeMax;
    } else {
        minimum = ((EventStatRecord *)datCommandRecords)[command].rangeMin;
        maximum = ((EventStatRecord *)datCommandRecords)[command].rangeMax;
    }
    if (minimum < maximum) {
        result = minimum + effMiscRandMod(0, maximum - minimum + 1);
    } else {
        result = minimum;
    }
    return result;
}

u8 btlGetActorDisplayByteWithDefault(s32 object, s32 index) {
    if (index == 0) {
        if ((((BtlUnit *)object)->flags & 0x400) != 0) {
            return *(u8 *)(datEnemyRecords + ((BtlUnit *)object)->mode * 76 + 0x48);
        }
        return 12;
    }
    return 12;
}

typedef struct BtlActionDelayRecord {
    u8 pad00;
    u8 delayIndex;
    u8 pad02[0x1E];
} BtlActionDelayRecord;

extern s32 datActionAnimationRecords;
extern s32 D_00358490[];

extern char D_003A1A58[]; /* "btl:delay=%d\n" */

s32 func_001A6AA0(BtlUnit *unit, s32 actionId) {
    s32 *delayTable;
    u32 delayIndex;
    s32 *delay;

    if ((actionId == 0) || (*(s8 *)(datCommandSelectors + actionId * 2 + 1) == 5)) {
        if (((unit->flags & 0x200) != 0) && (unit->mode == 6)) {
            return 9;
        }
    }
    if (actionId == 0) {
        return 1;
    }
    delayTable = D_00358490;
    delayTable++;
    delayIndex = ((BtlActionDelayRecord *)datActionAnimationRecords)[actionId].delayIndex;
    delay = delayTable + delayIndex * 2;
    btlBossDebugPrintf(D_003A1A58, *delay);
    return *delay;
}

s32 btlResolveSkillCategory(s32 unused, u32 id) {
    switch (id) {
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x17C:
    case 0x17F:
        return 0x37;
    case 0x185:
        return 0x35;
    case 0x186:
        return 0x1E;
    default:
        return *(s8 *)(datCommandSelectors + id * 2 + 1) == 2 ? 0x2D : 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A40);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A58);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6BE0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6EC0);

void btlDistributeRandomTargetHits(BtlUnit *unit, BtlIndexList *targets,
                   BtlTargetResult *results, s32 command) {
    u8 selected[13];
    BtlIndexList *copy;
    void *previous;
    void *entry;
    u32 targetCount;
    u32 maximumHits;
    u32 hitCount;
    u32 i;
    u32 index;
    u32 resultIndex;
    s32 allowConsecutiveHits;

    targetCount = btlGetIndexListCount(targets);
    allowConsecutiveHits = targetCount < 2;
    maximumHits = targetCount + effMiscRandMod(0, 2);
    if (allowConsecutiveHits) {
        results[0].hitCount = maximumHits;
        return;
    }

    previous = NULL;
    copy = btlAllocateIndexList(ARRAY_COUNT(selected));
    i = 0;
    btlCopyIndexList(copy, targets);
    btlClearIndexList(targets);
    hitCount = func_001A6968(unit, command);
    memset(selected, 0, sizeof(selected));
    while (i < hitCount) {
        index = effMiscRandMod(0, targetCount);
        entry = btlGetIndexListEntry(copy, index);
        if (!allowConsecutiveHits && entry == previous) {
            index = (index + effMiscRandMod(0, targetCount - 1) + 1) % targetCount;
            entry = btlGetIndexListEntry(copy, index);
        }
        previous = entry;
        if (!selected[index]) {
            btlAppendIndexListEntry(targets, entry);
            resultIndex = btlFindListIndex(targets, entry);
            selected[index] = 1;
            results[resultIndex].hitCount = 1;
        } else {
            resultIndex = btlFindListIndex(targets, entry);
            if (results[resultIndex].hitCount < maximumHits) {
                results[resultIndex].hitCount++;
            }
        }
        i++;
    }
    btlFreeIndexList(copy);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A7180);

s32 btlQueryUnitChannelFlags(s32 first, s32 second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = sdfQueryChannelBits(other, first + 0x120, second + 0x120);
    if ((((BtlUnit *)second)->conditionFlags & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A7410);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A7548);

void func_001A7AD0(void) {
}

extern f32 func_001A7C20(u8 *, u8 *, s32);

/* Scale the enemy's normal EP reward; flag 0x2000 multiplies it by 100. */
s32 btlCalculateEnemyExperienceReward(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    DatEnemyRecord *entry;
    f32 ratio;
    u32 ep;

    if (!(((BtlUnit *)enemy)->flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(((BtlUnit *)acquirer)->flags & 0x200)) {
        return result;
    }
    entry = &((DatEnemyRecord *)datEnemyRecords)[((BtlUnit *)enemy)->mode];
    ratio = func_001A7C20(acquirer, enemy, 1);
    ep = (u32)((f32)entry->experience * ratio);
    if (entry->flags & 0x2000) {
        ep *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f]\n", ep, entry->experience, ratio);
    } else {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f](acquisition)\n", ep, entry->experience, ratio);
    }
    return ep;
}

extern u8 *datBattleSceneRecords;

extern char D_003A1B90[];

extern char D_003A1BA8[];

f32 func_001A7C20(u8 *acquirer, u8 *enemy, s32 rewardKind) {
    s32 level;
    s32 difference;
    f32 factor;

    if (*(u16 *)(datBattleSceneRecords + ((BattleController *)btlGetRuntime())->mode * 40 + 0x20) & 0x400) {
        btlBossDebugPrintf(D_003A1B90);
        return 1.0f;
    }
    if (acquirer == NULL) {
        level = func_001A9488(4);
    } else {
        level = ((BtlUnit *)acquirer)->actionTime;
    }
    if (level > 60) {
        level = 60;
    }
    difference = level - ((BtlUnit *)enemy)->actionTime;
    if (difference > 15) {
        difference = 15;
    } else if (difference < -15) {
        difference = -15;
    }
    factor = datBattleParameters->rewardLevelScale[(15 - difference) * 2 + rewardKind];
    btlBossDebugPrintf(D_003A1BA8, factor, difference, rewardKind);
    return factor;
}

/* Read the money reward with the same eligibility and 100-fold table flag. */
INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1B90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1BA8);

s32 btlGetEnemyMoney(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    s32 money;
    DatEnemyRecord *entry;
    if (!(((BtlUnit *)enemy)->flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(((BtlUnit *)acquirer)->flags & 0x200)) {
        return result;
    }
    entry = &((DatEnemyRecord *)datEnemyRecords)[((BtlUnit *)enemy)->mode];
    money = entry->money;
    if (entry->flags & 0x2000) {
        money *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:money=%d\n", money, acquirer);
    } else {
        btlBossDebugPrintf("btl:money=%d(acquisition)\n", money);
    }
    return money;
}

extern f32 func_001A7C20(u8 *, u8 *, s32);

/* Hunt EP uses its own table quantity and the ratio calculator's mode 0. */
s32 btlCalculateHuntEpReward(u8 *arg0, u8 *arg1) {
    DatEnemyRecord *entry = &((DatEnemyRecord *)datEnemyRecords)[((BtlUnit *)arg1)->mode];
    f32 ratio = func_001A7C20(arg0, arg1, 0);
    u32 ep = (u32)((f32)entry->huntExperience * ratio);
    if (entry->flags & 0x2000) {
        ep *= 100;
    }
    btlBossDebugPrintf("btl:ep=%d[%d,%.3f](hunt)\n", ep, entry->huntExperience, ratio);
    return ep;
}

u32 func_001A7ED8(void) {
    return 0;
}

extern char D_003A1C10[]; /* "btl:hunt mp rec[%d]\n" */

s32 btlCalculateAbilityRecoveryAmount(u8 *actor) {
    s32 recovery = 0;
    if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x24E)) {
        recovery = (s32)(*(u16 *)(actor + 0x12C) * datAbilityParameters[0x24E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    } else if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x229)) {
        recovery = (s32)(*(u16 *)(actor + 0x12C) * datAbilityParameters[0x229 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    }
    btlBossDebugPrintf(D_003A1C10, recovery);
    return recovery;
}

s32 btlIsUnitDefeatTriggeredByValueDelta(u8 *actor, s32 delta) {
    if (btlGetEntryFlagsUnlessDisabled((s32)(actor + 0x120)) & 4) return 0;
    if (btlHasEnemyRecordDefeatExemptionFlag((s32)actor)) return 0;
    if ((((BtlUnit *)actor)->conditionFlags & 0x7FFF) == 0x4000) return 1;
    if ((*(u32 *)(btlGetRuntime() + 0x1F4) & 0x80) == 0) return 0;
    return ((BtlUnit *)actor)->hp + delta < 1;
}

s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *object) {
    return object->hp * 100 / object->maxHp < 25;
}

s32 btlWouldUiValueFallBelowQuarter(BtlUnit *object, s32 delta) {
    s32 value = object->hp + delta;
    if (value <= 0) {
        return 1;
    }
    return value * 100 / object->maxHp < 25;
}

s32 btlBothSidesActive(BtlUnit *unit) {
    BtlUnit *actor;
    s32 a;
    s32 b;
    if (btlIsUnitDefeatTriggeredByValueDelta((u8 *)unit, 0) != 0) {
        return 0;
    }
    if (unit->flags & 0x60) {
        return 0;
    }
    a = 0;
    b = 0;
    for (actor = ((BattleController *)btlGetRuntime())->actors; actor != 0; actor = actor->next) {
        if (actor->flags & 1) {
            if (!(actor->flags & 0xE0)) {
                if (actor->flags & 0x200) {
                    a++;
                }
                if (actor->flags & 0x400) {
                    b++;
                }
            }
        }
    }
    if (a != 0 && b != 0) {
        return 1;
    }
    return 0;
}

s32 btlIsUiObjectIndexAllowed(BtlUnit *object) {
    if ((object->flags & 0x400) != 0) {
        if (object->mode >= 0x100) {
            return 0;
        }
    }
    return 1;
}

s32 btlIsUnitStatusFlagClear(BtlUnit *object) {
    if ((object->conditionFlags & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1C10);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8188);

extern s32 D_00358514[];

extern s32 D_00358518[];

s32 btlSelectedEntryHitsElement(s32 arg0, BtlUnit *unit, s32 arg2) {
    u32 kind;
    s32 mask;
    u32 power;
    if (unit->selectedEntryIndex <= 0) {
        return 0;
    }
    btlGetRuntime();
    kind = btlGetActorIndexedSignedValue(arg0, arg2);
    mask = btlEncodeActorIndexAsSelectionMask(kind);
    power = *(u16 *)(unit->selectedEntryIndex * 0x38 + datCommandRecords + 0x2E);
    if (power == 0) {
        return 0;
    }
    if (kind >= 0x10 && (kind < 0x12 || kind == -1)) {
        return 0;
    }
    if (power >= 0x20) {
        return 0;
    }
    return (D_00358518[power * 3] & mask) != 0;
}

s32 btlGetActionRecordLookupValue(s32 arg0) {
    u16 temp_v0;

    temp_v0 = *(u16 *)(datCommandRecords + arg0 * 56 + 0x2e);
    return D_00358510[temp_v0 * 3];
}

s32 btlTestSelectedItemCategoryMask(s32 object, s32 mask) {
    s32 index = *(s32 *)(object + 0x2F0);
    u16 item;
    if (index == -1) {
        return 0;
    }
    item = *(u16 *)(datCommandRecords + index * 56 + 0x2E);
    return (D_00358518[item * 3] & btlEncodeActorIndexAsSelectionMask(mask)) != 0;
}

s32 fldGetSelectedUnitStat(s32 object) {
    s32 item = *(s32 *)(object + 0x2F0);
    if (item == -1) {
        return 0;
    }
    return btlGetActionRecordLookupValue(item);
}

s32 btlGetSelectedUnitProperty(s32 object) {
    s32 index = *(s32 *)(object + 0x2F0);
    if (index == -1) {
        return 0;
    }
    return D_00358514[*(u16 *)(datCommandRecords + index * 56 + 0x2E) * 3];
}

s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *targets, BtlTargetResult *results) {
    s32 i = 0;
    u32 sides = 0;
    BattleController *controller = (BattleController *)btlGetRuntime();
    u32 count = btlGetIndexListCount(targets);
    s32 skipped;
    BtlUnit *actor;

    for (; i < count; i++) {
        sides |= ((BtlUnit *)btlGetIndexListEntry(targets, i))->flags & 0x600;
    }
    skipped = 0;
    for (i = 0; i < count; i++, results++) {
        if (results->skipped != 0) {
            skipped++;
        }
    }
    count = 0;
    for (actor = controller->actors; actor != NULL; actor = actor->next) {
        if (actor->flags & 1) {
            if (actor->flags & 0xE0) {
                continue;
            }
            if (actor->flags & sides) {
                count++;
            }
        }
    }
    return skipped == count;
}

extern u8 *datBattleSceneRecords;

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8640);

extern u8 *datBattleSceneRecords;

extern u8 *datEnemyAiRecords;

s32 btlIsSelectedActorStatusAndRecordClear(s32 object) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    if (*(s8 *)(datBattleSceneRecords + index * 40) != 0) {
        return 0;
    }
    if ((((BtlUnit *)object)->conditionFlags & 0x2A0F) != 0) {
        return 0;
    }
    return datEnemyAiRecords[((BtlUnit *)object)->mode * 0x15C] == 0;
}

/* Return true when neither actor status nor its entry flags contain bit 0x40. */
s32 btlAreUnitStatusAndEntryFlagsClear(s32 actor) {
    if ((((BtlUnit *)actor)->conditionFlags & 0x40) != 0) {
        return 0;
    }
    return (btlGetEntryFlagsUnlessDisabled(actor + 0x120) & 0x40) < 1;
}

extern s32 ptyMatchAffinityPermutation(s32 *actors, s32 affinity);

s32 btlFindCommandPartnersByAffinity(BtlUnit *unit, s32 command, void **first, void **second) {
    s32 statAddresses[3];
    u32 requiredCount = 0;
    BattleController *controller = (BattleController *)btlGetRuntime();
    s32 *requirementCursor;
    BtlUnit *candidate;
    BtlUnit *partner;
    u32 i;

    requirementCursor = (s32 *)(datAffinityRecords + command * 16 - 0x1AB0);
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount < 2) {
        return 0;
    }

    statAddresses[0] = (s32)&unit->statBits;
    for (candidate = controller->actors; candidate != NULL; candidate = candidate->next) {
        u32 flags = candidate->flags;

        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 0x400) == 0) {
            continue;
        }
        if ((candidate->conditionFlags & 0x2A0E) != 0) {
            continue;
        }
        if (unit == candidate) {
            continue;
        }
        statAddresses[1] = (s32)&candidate->statBits;
        if (requiredCount == 3) {
            for (partner = controller->actors; partner != NULL; partner = partner->next) {
                u32 partnerFlags = partner->flags;

                if ((partnerFlags & 1) == 0) {
                    continue;
                }
                if ((partnerFlags & 0x400) == 0) {
                    continue;
                }
                if ((partner->conditionFlags & 0x2A0E) != 0) {
                    continue;
                }
                if (unit == partner || candidate == partner) {
                    continue;
                }
                statAddresses[2] = (s32)&partner->statBits;
                if (ptyMatchAffinityPermutation(statAddresses, command) == 0) {
                    continue;
                }
                if (first != NULL) {
                    *first = candidate;
                }
                if (second != NULL) {
                    *second = partner;
                }
                return 1;
            }
        } else {
            statAddresses[2] = 0;
            if (ptyMatchAffinityPermutation(statAddresses, command) != 0) {
                if (first != NULL) {
                    *first = candidate;
                }
                if (second != NULL) {
                    *second = NULL;
                }
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8A30);

s32 btlHasAdjacentActorRecordStatus(s32 object) {
    u8 *status = (u8 *)(datEnemyRecords + ((BtlUnit *)object)->mode * 76 + 0x3E);
    u32 i;
    for (i = 0; i < 2; i++) {
        if (*status++ != 0) {
            return 1;
        }
    }
    return 0;
}

/* Return 1 when bit 26 is clear, preserving the original signed word shift. */
u32 btlIsActorHighStateFlagClear(s32 actor) {
    return (((s32)((BtlUnit *)actor)->flags >> 0x1a) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8CE0);

s32 func_001A8DD8(s32 object, s32 *choices) {
    u16 *ids = (u16 *)(object + 0x142);
    s32 count = 0;
    u32 i;

    for (i = 0; i < 24; i++) {
        u16 id = *ids++;

        if (id < 0xA0) {
            continue;
        }
        if (id >= 0xA6) {
            if (id >= 0xB9) {
                continue;
            }
            if (id < 0xB5) {
                continue;
            }
        }
        if (btlGetCommandFailureReason((BtlUnit *)object, id) != 0) {
            continue;
        }
        if (choices != NULL) {
            choices[count] = id;
        }
        count++;
    }
    return count;
}

u8 btlHasAvailableOption(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001A8DD8(arg0, 0);
    return temp_v0 != 0;
}

s32 btlChooseRandomAvailableOption(s32 object) {
    s32 choices[24];
    s32 count = func_001A8DD8(object, choices);
    if (count != 0) {
        return choices[effMiscRandMod(0, count)];
    }
    return -1;
}

s32 btlChooseEligibleSkill(s32 object) {
    s32 choices[24];
    s32 count = 0;
    u32 i;
    u16 *ids = (u16 *)(object + 0x142);
    for (i = 0; i < 24; i++) {
        u32 id = *ids++;
        if (id != 0) {
            if (id < 0x260) {
                s32 category = *(s8 *)(datCommandSelectors + id * 2 + 1);
                if (category != 2) {
                    if (category != 4) {
                        if ((*(u8 *)(datCommandRecords + id * 56 + 1) & 2) != 0) {
                            if (id < 0xAB || (id >= 0xAD && id != 0xBF)) {
                                choices[count++] = id;
                            }
                        }
                    }
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    return choices[effMiscRandMod(0, count)];
}

f32 btlGetActionCategoryMultiplier(s32 object, s32 unused, s32 index) {
    s32 category = *(u16 *)(datCommandRecords + index * 56 + 0x16);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    if (index == 0 && btlCheckSpecialAbility(object + 0x120, 0x21D) != 0) {
        return datAbilityParameters[0x21D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    return 0.0f;
}

f32 btlGetActionCategoryGateAsFloat(s32 unused0, s32 unused1, s32 index) {
    s32 category = *(u16 *)(datCommandRecords + index * 56 + 0x1A);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 btlGetActionRecordPercentAsFraction(s32 unused0, s32 unused1, s32 index) {
    return (f32)*(u16 *)(datCommandRecords + index * 56 + 0x22) / 100.0f;
}

u32 btlAdjustPointsForCombatFlags(u32 flags, u32 secondary, u32 points, s32 index) {
    if (flags & 0x20000) return 0x1194;
    if (flags & 0x40000) return 0x1194;
    if (flags & 0x10000) return points + 0x64;
    if (flags & 2) return points + 0x64;
    if (secondary & 4) return points >> 1;
    if (secondary & 2) return points >> 1;
    if (flags & 4) {
        if (*(u16 *)(datCommandRecords + index * 56 + 0x16) == 8 ||
            *(u16 *)(datCommandRecords + index * 56 + 0x16) == 10) {
            return points;
        }
        if (*(u8 *)(datCommandRecords + index * 56 + 2) == 2) return points;
        return points + 0x64;
    }
    return points;
}

s32 btlGetCommandResultKindFromFlags(u32 flags, u32 secondary) {
    if (flags & 0x20000) return 1;
    if (flags & 0x40000) return 1;
    if (flags & 0x10000) return 1;
    if (flags & 2) return 1;
    if (secondary & 4) return 3;
    return secondary & 2 ? 2 : 1;
}

s32 btlAverageMaximumValueForMask(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x128);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void btlAverageFilteredMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 1);
}

void btlAverageAllMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 0);
}

s32 btlAverageCurrentValueForMask(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x126);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void btlAverageFilteredCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 1);
}

void btlAverageAllCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 0);
}

s32 btlAverageMaskedActorStat(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x134);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

s32 func_001A9488(u32 arg0) {
    return btlAverageMaskedActorStat(arg0, 1);
}

void func_001A94A0(u32 arg0) {
    btlAverageMaskedActorStat(arg0, 0);
}

s32 btlSumOrAverageActorAttribute(u32 mask, s32 attribute, s8 allowDisabled) {
    s32 sum = 0;
    s32 count = 0;
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    s32 value = datGetStatWithStatusOverride(node + 0x120, attribute);
                    count++;
                    sum += value;
                }
            }
        }
    }
    if (count >= 2) {
        sum /= count;
    }
    return sum;
}

void btlAverageFilteredActorAttribute(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 1);
}

void __udivdi3(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A95C8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A9780);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1D28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A99B0);

void btlClearUnitStatusMask(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                *(u32 *)(node + 0x110) = flags & ~0x1000;
                *(u16 *)(node + 0x120) &= ~0x1000;
            }
        }
    }
}

extern char D_003A1D78[];

extern s32 evtRunContext(s32, u8 *, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A9D38);

extern char D_003A1DA0[]; /* "btl:endure=%d%%[ratio=%.2f]\n" */

s32 btlRollActorEligibilityWithAbilityOverride(u8 *actor) {
    u8 *battle = (u8 *)btlGetRuntime();
    s32 (*predicate)(u8 *) = *(s32 (**)(u8 *))(battle + 0x65C);
    if (predicate != 0 && predicate(actor) == 0) return 0;
    if (((BtlUnit *)actor)->stateFlags & 0x2000) return 0;
    if ((((BtlUnit *)actor)->conditionFlags & 0x7FFF) == 0x4000) return 0;
    if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x231)) return 1;
    btlBossDebugPrintf(D_003A1DA0, 5, 1.0);
    return btlRollAiBucket() < 5;
}

s32 btlHasEnemyRecordDefeatExemptionFlag(s32 object) {
    if ((((BtlUnit *)object)->flags & 0x400) == 0) {
        return 0;
    }
    return ((s32)((DatEnemyRecord *)datEnemyRecords)[((BtlUnit *)object)->mode].flags & 0x100) > 0;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1DA0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A9F40);

extern s32 D_00358690[];

s32 btlRollPassiveAbilityAction(u8 *actor) {
    u32 i;
    s32 result;
    s32 threshold;

    if (btlIsUnitDefeatTriggeredByValueDelta(actor, 0)) {
        return -1;
    }
    result = 0;
    for (i = 0; i < 3; i++) {
        if (btlCheckSpecialAbility((s32)(actor + 0x120), D_00358690[i * 2])) {
            threshold = (s32)(datAbilityParameters[D_00358690[i * 2] - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * 100.0f);
            if (btlRollAiBucket() < threshold) {
                result = (D_00358690 + i * 2)[1];
                break;
            }
        }
    }
    return result != 0 ? result : -1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AA130);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AA548);

u32 func_001AA848(void) {
    btlGetRuntime();
    return 0xffffffff;
}

void btlRestoreUnitMinimumValueAndClearStatus(s32 object, s32 status) {
    *(u16 *)(status + 0x26) &= ~3;
    *(u32 *)(object + 0x110) &= ~0x20;
    *(u16 *)(object + 0x12E) &= ~0x4080;
    if (*(u16 *)(object + 0x126) == 0) {
        *(u16 *)(object + 0x126) = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AA8B0);

s32 btlIsBattleRecordEligible(u8 *actor, u8 *target, s32 recordIndex, s32 speciesIndex) {
    s32 (*callback)(u8 *, u8 *, s32) =
        *(s32 (**)(u8 *, u8 *, s32))(btlGetRuntime() + 0x680);
    u8 *resource;
    if (callback != 0 && callback(actor, target, speciesIndex) == 0) {
        return 0;
    }
    resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(actor + 0xC4), *(s32 *)(actor + 0xC8));
    if (*(s16 *)(resource + recordIndex * 20 + 0x2C) != 2) {
        return 0;
    }
    if (speciesIndex != 0 &&
        *(u8 *)(datCommandRecords + speciesIndex * 56 + 8) != 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorModeActionCodeAllowed(u8 *actor) {
    if (*(u32 *)(actor + 0xDC) != 1) {
        return 1;
    }
    switch (*(u32 *)(actor + 0xE0)) {
    case 0x26:
    case 0x54:
    case 0x153:
        return 0;
    default:
        return 1;
    }
}

s32 btlHasHighPriorityState(void) {
    BattleSelectionWork *resource;
    s8 index;
    s32 count;
    if (btlCommandPanelWork->state == 2) {
        resource = btlLinkedSelectionTaskBuffer;
        index = resource->selectedRow;
        count = resource->rowFade[index - 1];
        if (count >= 0x80) {
            if (resource->positions[index - 1].x >= 11) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlCountFlaggedSceneActors(void) {
    u8 *actor = *(u8 **)(btlGetRuntime() + 0x228);
    s32 count = 0;
    while (actor != 0) {
        if ((*(u64 *)(actor + 0x110) & 0x321) == 0x301 &&
            (*(u16 *)(actor + 0x12E) & 0x800) == 0) {
            count++;
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return count;
}


typedef struct BattleCmdPanelSlotTable {
    u32 values[5];
} BattleCmdPanelSlotTable;

extern const BattleCmdPanelSlotTable D_003A1FA8;

extern const BattleCmdPanelSlotTable D_003A1FC0;

void btlInitializeCommandPanelSlotTables(void) {
    BattleCmdPanelSlotTable tableA = D_003A1FA8;
    BattleCmdPanelSlotTable tableB = D_003A1FC0;
    s32 i;

    btlCommandPanelWork = sdfAllocAndClearQuadwords(0xCC);
    btlCommandPanelWork->state = 1;
    btlCommandPanelWork->classIndex = 0;
    btlCommandPanelWork->labelEntry = btlGetActorIdForClass(btlCommandPanelWork->classIndex);
    btlGetActorClassPair(btlCommandPanelWork->classIndex,
                  &btlCommandPanelWork->classX,
                  &btlCommandPanelWork->classY);
    for (i = 0; i < 5; i++) {
        btlCommandPanelWork->slotsA[i].unk01 = 0;
        btlCommandPanelWork->slotsA[i].fadeValue = tableB.values[i];
        btlCommandPanelWork->slotsB[i].unk01 = 1;
        btlCommandPanelWork->slotsB[i].fadeValue = tableA.values[i];
    }
}

typedef struct ActorClassIds {
    s16 values[8];
} ActorClassIds;

extern const ActorClassIds btlActorIdsByClass;

s16 btlGetActorIdForClass(s8 classId) {
    ActorClassIds table = btlActorIdsByClass;
    return table.values[classId];
}

typedef struct ActorClassPairTable {
    u32 values[14];
} ActorClassPairTable;

extern const ActorClassPairTable btlActorClassPairs;

void btlGetActorClassPair(s8 classId, u32 *first, u32 *second) {
    ActorClassPairTable pairs = btlActorClassPairs;
    *first = pairs.values[classId * 2];
    *second = pairs.values[classId * 2 + 1];
}

extern SndPad D_00324510;

extern u8 D_00359160[];

extern void func_001B83D8(s32, s32, s32);

extern void sndSetStationedSeVolume(u32);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1FA8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1FC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", btlActorIdsByClass);

INCLUDE_RODATA(const s32, "game/code_001A1960", btlActorClassPairs);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AADF8);

typedef struct BattleCmdPanelGridTable {
    u8 values[30];
} BattleCmdPanelGridTable;

extern const BattleCmdPanelGridTable D_003A2050;

extern const BattleCmdPanelGridTable D_003A2070;

void btlPopulateCommandPanelGrid(void) {
    BattleCmdPanelGridTable table1 = D_003A2050;
    BattleCmdPanelGridTable table2 = D_003A2070;
    s32 i;

    for (i = 0; i < 5; i++) {
        btlCommandPanelWork->slotsA[i].hidden =
            table1.values[btlCommandPanelWork->classIndex * 5 + i];
        btlCommandPanelWork->slotsB[i].hidden =
            table2.values[btlCommandPanelWork->classIndex * 5 + i];
    }
}

/* Ramp both five-row banks by panel state; narrow each update before clamping. */
void btlUpdateCommandPanelRowFades(void) {
    s32 i;

    switch (btlCommandPanelWork->state) {
    case 1:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue += 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue += 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 2:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0x80 ? 0x80 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0x80 ? 0x80 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 3:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 4:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AB558);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AB810);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ABDF8);

void btlReleaseAndClearChipBlock(void) {
    sdfReleaseChipBlock(btlCommandPanelWork);
    btlCommandPanelWork = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AC080);

typedef struct BtlResBlock {
    s32 unk0;
    s32 nameA;
    s32 nameB;
    s32 nameC;
    EffectSlotSet *resA;
    EffectSlotSet *resB;
    EffectSlotSet *resC;
    s32 unk1C;
} BtlResBlock;
extern BtlResBlock *btlResourceBlock;

/* 0x2C-byte action record shared by the initializer, updater and renderer. */
typedef struct BattleMirroredSpriteRecord {
    s8 active;
    u8 pad01;
    s16 slot;
    f32 scale;
    s32 restoredWidth;
    s32 restoredHeight;
    s32 width;
    s32 height;
    s32 x;
    s32 y;
    s32 secondX;
    s32 frame;
    s8 alpha;
    u8 pad29[3];
} BattleMirroredSpriteRecord;

void func_001AC398(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
    if (count > 0) {
        BattleMirroredSpriteRecord *record = records;
        s32 remaining = count;
        do {
            switch (record->active) {
            case 1: {
                s32 scale = (s32)record->scale;
                s32 x = record->x;
                s32 y = record->y;
                s32 width = btlResourceBlock->resA->workEntries[record->slot].sourceWidth;
                s32 height;
                s32 scaledWidth;
                s32 scaledHeight;
                record->restoredWidth = width;
                scale = scale / 2;
                height = btlResourceBlock->resA->workEntries[record->slot].sourceHeight;
                scaledWidth = width * scale;
                scaledHeight = height * scale;
                record->active = 2;
                record->restoredHeight = height;
                x -= (scaledWidth - width) / 2;
                y -= (scaledHeight - height) / 2;
                record->secondX = x;
                record->y = y;
                record->width = scaledWidth << 4;
                record->height = scaledHeight << 3;
                break;
            }
            case 2:
                record->width -= 0x800;
                record->height -= 0x800;
                if (record->width <= 0x1000) record->active = 0;
                break;
            }
            remaining--;
            record++;
        } while (remaining != 0);
    }
}

typedef struct BtlResBlock {
    s32 unk0;
    s32 nameA;
    s32 nameB;
    s32 nameC;
    EffectSlotSet *resA;
    EffectSlotSet *resB;
    EffectSlotSet *resC;
    s32 unk1C;
} BtlResBlock;
extern BtlResBlock *btlResourceBlock;
extern void func_002BF438(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

typedef struct BattlePanelColors {
    u32 values[4];
} BattlePanelColors;
extern const BattlePanelColors D_003A21B8;

/* 0x2C-byte action record shared by the initializer, updater and renderer. */
typedef struct BattleMirroredSpriteRecord {
    s8 active;
    u8 pad01;
    s16 slot;
    f32 scale;
    s32 restoredWidth;
    s32 restoredHeight;
    s32 width;
    s32 height;
    s32 x;
    s32 y;
    s32 secondX;
    s32 frame;
    s8 alpha;
    u8 pad29[3];
} BattleMirroredSpriteRecord;

void func_001AC4A8(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
    BattlePanelColors colors = D_003A21B8;
    if (count > 0) {
        BattleMirroredSpriteRecord *record = records;
        s32 remaining = count;
        do {
            if (record->active != 0) {
                u32 color;
                btlResourceBlock->resA->workEntries[record->slot].width = record->width;
                btlResourceBlock->resA->workEntries[record->slot].height = record->height;
                color = record->alpha | 0x80808000;
                colors.values[0] = color;
                colors.values[1] = color;
                colors.values[2] = color;
                colors.values[3] = color;
                func_002BF438(record->x << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                func_002BF438(record->secondX << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                btlResourceBlock->resA->workEntries[record->slot].width = record->restoredWidth << 4;
                btlResourceBlock->resA->workEntries[record->slot].height = record->restoredHeight << 3;
            }
            record++;
        } while (--remaining != 0);
    }
}

void btlInitializeActionRecordWithScale(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    *(u8 *)(arg0 + 0) = 1;
    *(u8 *)(arg0 + 0x28) = arg4 * 8 + 0x18;
    *(u16 *)(arg0 + 2) = arg1;
    *(f32 *)(arg0 + 4) = arg5;
    *(u32 *)(arg0 + 0x18) = arg2;
    *(u32 *)(arg0 + 0x1c) = arg3;
    *(u32 *)(arg0 + 0x24) = 0;
}

extern u8 D_003BB3E4;

extern u8 btlResourceBlockLoaded;

extern char D_003A21C8[]; /* "/battle/panel/batle_01.spr" */

extern char D_003A21E8[]; /* "/battle/panel/batle_02.spr" */

extern char D_003A2208[]; /* "/battle/panel/battle_03.spr" */

extern s32 sdfAllocGeneralBlock(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern s32 sdfReadNamedResource(char *, void *, s32);

void btlPanelResourcesLoad(void) {
    u8 params[16];
    s32 handle;
    BtlResBlock *block;
    if (D_003BB3E4 == 0) {
        handle = sdfAllocGeneralBlock(0x28);
        block = (BtlResBlock *)sdfResourceRetainAddress(handle);
        btlResourceBlock = block;
        block->unk0 = handle;
        block->resA = 0;
        block->resB = 0;
        block->unk1C = 0;
        btlResourceBlock->nameA = sdfReadNamedResource(D_003A21C8, params, 0);
        btlResourceBlock->nameB = sdfReadNamedResource(D_003A21E8, params, 0);
        btlResourceBlock->nameC = sdfReadNamedResource(D_003A2208, params, 0);
        btlResourceBlockLoaded = 0;
    }
    D_003BB3E4 = 1;
}


extern u32 func_002BD9C0(u32, u32);

void btlLoadResourceBlock(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    if (btlResourceBlockLoaded == 0) {
        btlResourceBlock->resA = (EffectSlotSet *)func_002BD9C0(btlResourceBlock->nameA, 0);
        btlResourceBlock->resB = (EffectSlotSet *)func_002BD9C0(btlResourceBlock->nameB, 0);
        btlResourceBlock->resC = (EffectSlotSet *)func_002BD9C0(btlResourceBlock->nameC, 0);
        work->resA = btlResourceBlock->resA;
        work->resB = btlResourceBlock->resB;
        btlResourceBlockLoaded = 1;
    }
}

extern u32 effDestroyResourceSlotSet(u32);

void btlReleaseResourceBlock(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    if (btlResourceBlockLoaded != 0) {
        effDestroyResourceSlotSet((u32)btlResourceBlock->resA);
        btlResourceBlock->resA = 0;
        effDestroyResourceSlotSet((u32)btlResourceBlock->resB);
        btlResourceBlock->resB = 0;
        effDestroyResourceSlotSet((u32)btlResourceBlock->resC);
        btlResourceBlock->resC = 0;
        work->resA = 0;
        work->resB = 0;
        work->resC = 0;
        btlResourceBlockLoaded = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AC7D8);

/* Clear each task's eight opaque words, forward for variant 1 and backward otherwise. */
void btlClearTaskActorSlots(void) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlTask *node = context->taskHead;
    while (node != 0) {
        s32 i;
        BtlUnit *actor = node->unit;
        if (actor != 0) {
            if (context->variant == 1) {
                if (actor->flags & 0x200) {
                    u32 *entries;
                    i = 0;
                    entries = &node->actions[0];
                    for (; i < 8; i++) {
                        *entries++ = 0;
                    }
                }
            } else if (actor->flags & 0x400) {
                u32 *entries;
                i = 7;
                entries = &node->actions[7];
                for (; i >= 0; i--) {
                    *entries-- = 0;
                }
            }
        }
        node = node->next;
    }
}

typedef struct ActorSlotOrder {
    u8 pad00[0xC];
    s32 entries[12];
} ActorSlotOrder;

typedef struct ActorOrder12 {
    s32 entries[12];
} ActorOrder12;

typedef struct ActorOrder6 {
    s32 entries[6];
} ActorOrder6;

extern ActorSlotOrder *D_003BD840[2];
extern const ActorOrder12 D_003A2228;
extern const ActorOrder6 D_003A2258;

void func_001AC9E8(s32 selector, s32 count) {
    ActorOrder12 primaryOrder = D_003A2228;
    ActorOrder6 secondaryOrder = D_003A2258;
    s32 *order = selector != 0 ? primaryOrder.entries : secondaryOrder.entries;
    ActorSlotOrder *destination;
    s32 i;

    i = 0;
    if (count > 0) {
        destination = D_003BD840[selector];
        do {
            destination->entries[i] = order[i];
            i++;
        } while (i < count);
    }
}

extern s32 fldCountSceneFadeKinds(BattleController *, s32 *);

s32 func_001ACAE0(void) {
    BattleController *scene;
    s32 fadeCounts[4];

    scene = (BattleController *)btlGetRuntime();
    return fldCountSceneFadeKinds(scene, fadeCounts);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACB08);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACC20);

void btlClearSharedBattleStateWords(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 2;
    puVar1 = (u32 *)(datGameState + 0x2e9dc);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

s32 func_001ACD30(s32 id, s32 operation) {
    s32 selector;
    s32 offset;
    u32 word;
    u32 bit;

    selector = (u16)id;
    offset = selector - 0x1AB;
    selector = (s8)operation;
    if (offset != 0) {
        word = (u32)offset >> 5;
        bit = offset & 0x1F;
    } else {
        word = 0;
        bit = 0;
    }
    switch (selector) {
    case 0:
        ((u32 *)(datGameState + 0x2E9DC))[word] |= 1 << bit;
        break;
    case 1:
        ((u32 *)(datGameState + 0x2E9DC))[word] &= ~(1 << bit);
        break;
    default:
        return ((((u32 *)(datGameState + 0x2E9DC))[word] & (1 << bit)) != 0);
    }
    return 1;
}

void func_001ACDF0(void) {
    s32 *temp_v0;
    s32 temp_v1;

    temp_v0 = D_00359A78;
    temp_v0 += 2;
    temp_v1 = 2;
    do {
        temp_v1 = temp_v1 - 1;
        *temp_v0 = 0;
        temp_v0 = temp_v0 - 1;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACE28);

extern u32 D_003BB3A0;

s64 btlSetTaskPhase2(void) {
    s64 task = kwlnTaskGetTaskByName(D_003BB3A0);
    if (task != 0) {
        *(s32 *)kwlnTaskGetUserValue(task) = 2;
        return 1;
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACF10);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACFE0);

u32 btlIsNamedBattleTaskRegistered(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(10);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001AD1F8(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

typedef struct BtlAnalysisPanelWork {
    s8 state;
    u8 pad01[3];
    s32 entry;
    s32 duration;
    s32 frame;
    u8 pad10[0x18];
    s32 x;
    s32 y;
    s8 page;
    u8 pad31[0x6F];
} BtlAnalysisPanelWork;

extern s32 func_001AE540(s64);
extern void btlReleaseTaskAndRefreshCursorIfFlagged(s32);

/* Create the analysis panel, replacing any panel task already registered. */
s32 btlCreateAnalysisPanelTask(s32 entry, s32 duration) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlAnalysisPanelWork *data;
    s32 task;

    if (btlHasRegisteredAnalysisPanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(11), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(10), 0);
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
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->page = 0;
    data->x = -20;
    data->y = 133;
    data->duration = duration;
    data->entry = entry;
    data->frame = 0;
    task = kwlnTaskCreate(btlAnalyzPanelTaskNameRef, 0x2B0E, 1, 1, func_001AE540,
                          btlReleaseTaskAndRefreshCursorIfFlagged, (u32)data);
    func_00101A80(context->taskParent, task);
    btlSetTrackedTaskHandle(11, task);
    context->flags &= ~0x100000;
    return 1;
}

u32 btlHasRegisteredAnalysisPanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0xb);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlGetRegisteredTaskValueOrDefault(void) {
    if (btlHasRegisteredAnalysisPanelTask() == 0) {
        return 0x80;
    }
    return *(s8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef));
}

u32 func_001AD428(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
    return 1;
}

typedef struct BtlGuidePanelWork {
    s8 state;
    u8 pad01[0x1B];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} BtlGuidePanelWork;

extern s32 func_001B09A8(s64);
extern void btlReleaseMessageWindowTask(s64);
extern u32 btlHasRegisteredGuidePanelTask(void);
extern u32 btlHasRegisteredSkillNamePanelTask(void);
extern u32 btlHasRegisteredAphNamePanelTask(void);
extern void btlSetTrackedTaskHandle(s32, s32);

s32 btlCreateGuidePanelTask(s32 arg0, s32 arg1) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlGuidePanelWork *data;
    s32 task;

    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(10), 0);
    }
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->unk1C = 0x18;
    data->unk20 = 0x60;
    data->unk24 = arg0;
    data->unk28 = arg1;
    task = kwlnTaskCreate(D_003BB3C4, 0x2B0E, 1, 1, func_001B09A8,
                          btlReleaseMessageWindowTask, (u32)data);
    func_00101A80(context->taskParent, task);
    btlSetTrackedTaskHandle(9, task);
    return 1;
}

void func_001AD5A0(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

u32 btlHasRegisteredGuidePanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(9);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

typedef struct BtlPhaseTask {
    s32 phase;
} BtlPhaseTask;

void btlSetTaskPhase5(void) {
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        ((BtlPhaseTask *)kwlnTaskGetUserValue(task))->phase = 5;
        btlCommandPanelWork->state = 3;
    }
}

void btlSetTrackedTaskDisplayMode(s32 mode) {
    s32 task = btlGetTrackedTaskHandle(7);
    if (task != 0) {
        s32 *data = (s32 *)kwlnTaskGetUserValue(task);
        data[2] = mode;
        if (mode == 0) {
            data[1] = 1;
            data[5] = 0x80;
        } else {
            data[5] = 0xFF;
            data[1] = 4;
            data[6] = 0xFF;
        }
    }
}

void btlInvalidateSceneFadeCounts(s32 unused) {
    btlGetRuntime();
    kwlnTaskGetUserValue(btlGetTrackedTaskHandle(7));
    btlTrackedTaskHandles->fadeKindsCached = 0;
}

u32 btlHasRegisteredPsechgPanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(6);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3BC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AD758);

u32 btlHasRegisteredSkillNamePanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(1);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3AC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AD970);

u32 btlHasRegisteredAphNamePanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3A8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

typedef struct MsgQueueTaskData {
    s32 task;
    s32 id;
    s32 unk08;
    s32 counter;
    s32 value;
    s16 fade;
} MsgQueueTaskData;

extern const BattlePanelColors D_003A2A90;
extern s32 itfMesMeasureEntryItem(s32, s32, s32);
extern void itfMesBlk24MoveTo(s32, s32, s32);

extern s32 btlDrawTimedDialogTask(s64);

extern void btlReleaseDialogTaskData(s64);

s32 btlReplaceDialogTasksAndQueueMessage(s32 arg0, s32 arg1) {
    s32 context = btlGetRuntime();
    s32 task = btlGetTrackedTaskHandle(0);
    MsgQueueTaskData *data;

    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
    data = (MsgQueueTaskData *)sdfAllocAndClearQuadwords(0x40);
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(10), 0);
    }
    data->id = arg0;
    data->unk08 = arg1;
    data->value = 0x2D;
    task = kwlnTaskCreate(D_003BB3A8, 0x2B0E, 1, 1, btlDrawTimedDialogTask, btlReleaseDialogTaskData, (u32)data);
    func_00101A80(*(u32 *)(context + 0x29C), task);
    data->task = task;
    btlSetTrackedTaskHandle(0, task);
    return 1;
}

/* Query one of the fourteen task slots retained by the command UI. */
s32 btlGetTrackedTaskHandle(s32 slotIndex) {
    s32 *handle;

    handle = &btlTrackedTaskHandles->handles[slotIndex];
    return *handle;
}

void btlSetTrackedTaskHandle(s32 slotIndex, s32 taskHandle) {
    s32 *handle;

    handle = &btlTrackedTaskHandles->handles[slotIndex];
    *handle = taskHandle;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ADCE8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ADE68);

typedef struct { u16 flag; u16 unk_02; } MesWindowState;

typedef struct { u32 unk_00; s32 window[9]; u8 unk_28[0x20]; MesWindowState state[9]; } MesWindowList;

void itfMesCloseAllWindows(s32 handle) {
    MesWindowList *list;
    s32 i;
    btlGetRuntime();
    list = (MesWindowList *)kwlnTaskGetUserValue(handle);
    for (i = 0; i < 9; i++) {
        if (list->state[i].flag != 0) {
            itfMesCleanupWindow(list->window[i], 0);
            itfMesDestroyWindowIfPresent(list->window[i]);
        }
    }
    sdfReleaseChipBlock(list);
    btlSetTrackedTaskHandle(10, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2050);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2070);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2090);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A20A0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A20D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2100);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2128);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2158);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2168);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A21A8);

const BattlePanelColors D_003A21B8 = {{0x80808080, 0x80808080, 0x80808080, 0x80808080}};

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A21C8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A21E8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2208);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2228);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2258);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2270);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2298);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A22C0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AE250);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AE540);

void btlReleaseTaskAndRefreshCursorIfFlagged(s32 handle) {
    u8 *context = (u8 *)btlGetRuntime();
    u8 *data = (u8 *)kwlnTaskGetUserValue(handle);
    sdfReleaseChipBlock(data);
    btlSetTrackedTaskHandle(11, 0);
    if (*(u16 *)(*(u8 **)(*(u8 **)(context + 0x164) + 0x18) + 0x12E) & 0x80) {
        btlInitCursorAndApplyAction(context + 0x70, context + 0x70);
    }
    *(u32 *)(context + 0x1F4) |= 0x100000;
}

typedef struct BattlePanelEdgeWork {
    u8 pad00[0x31];
    s8 edgePhase;
    u8 pad32[0x1E];
    f32 corners[4][4];
} BattlePanelEdgeWork;
extern f32 D_003A2450[4];

/* Advance one corner toward the panel boundary before moving to the next edge. */
s32 btlAdvancePanelCornerPhase(s32 task, BattlePanelEdgeWork *work) {
    f32 center[4];

    memcpy(center, D_003A2450, sizeof(center));

    switch (work->edgePhase) {
    case 0:
        work->corners[0][1] += 80.0f;
        work->corners[0][1] =
            work->corners[0][1] <= center[1] - 174.0f ? center[1] - 174.0f :
            center[1] + 174.0f <= work->corners[0][1] ? center[1] + 174.0f :
            work->corners[0][1];
        if (center[1] + 174.0f <= work->corners[0][1]) {
            work->edgePhase++;
        }
        break;
    case 1:
        work->corners[2][0] += 80.0f;
        work->corners[2][0] =
            work->corners[2][0] <= center[0] - 200.0f ? center[0] - 200.0f :
            center[0] + 200.0f <= work->corners[2][0] ? center[0] + 200.0f :
            work->corners[2][0];
        if (center[0] + 200.0f <= work->corners[2][0]) {
            work->edgePhase++;
        }
        break;
    case 2:
        work->corners[1][1] -= 80.0f;
        work->corners[1][1] =
            work->corners[1][1] <= center[1] - 174.0f ? center[1] - 174.0f :
            center[1] + 174.0f <= work->corners[1][1] ? center[1] + 174.0f :
            work->corners[1][1];
        if (work->corners[1][1] <= center[1] - 174.0f) {
            work->edgePhase++;
        }
        break;
    case 3:
        work->corners[3][0] -= 80.0f;
        work->corners[3][0] =
            work->corners[3][0] <= center[0] - 200.0f ? center[0] - 200.0f :
            center[0] + 200.0f <= work->corners[3][0] ? center[0] + 200.0f :
            work->corners[3][0];
        if (work->corners[3][0] <= center[0] - 200.0f) {
            work->edgePhase++;
        }
        break;
    default:
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2450);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2460);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AF058);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AF5D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2490);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A24A0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A24D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2500);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2530);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AFF78);

typedef struct BtlPanelStrip {
    s32 x;
    s32 y;
    s32 texture;
} BtlPanelStrip;

extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A25C0);

void btlDrawThreePanelSpriteStrips(s32 unused, s32 x, s32 y, s32 delta) {
    BtlPanelStrip strips[3] = { {0, 0, 0x40}, {5, 0, 0x41}, {0x6B, 0, 0x42} };
    u32 colors[4] = { 0x80808080, 0x80808080, 0x80808080, 0x80808080 };
    s32 i, j;
    BtlPanelStrip *strip;

    for (strip = strips, i = 0; i < 3; i++, strip++) {
        for (j = 0; j < 4; j++) {
            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA, strip->texture, j, delta);
        }
        func_002BF438((x + strip->x) << 4, (y + strip->y) << 3,
                     0, colors, 0, btlResourceBlock->resA, strip->texture, 0x53);
    }
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2608);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A27C8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B05D0);

u32 btlSetSlotLowByteClamped(EffectSlotSet *owner, s32 group, s32 slot, s32 delta) {
    u32 word = owner->workEntries[group].savedColors[slot];
    u32 limit;
    u32 value;
    if (delta > 0) {
        limit = value = word & 0xFF;
        if ((u32)delta < value) {
            value = delta;
        }
    } else {
        limit = word & 0xFF;
        value = 0;
    }
    delta = value;
    if (limit > 0x80) {
        if (delta >= 0x80) {
            delta = limit;
        }
    }
    return (word & ~0xFF) | delta;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B09A8);

void btlReleaseMessageWindowTask(s64 task) {
    s32 *entry = (s32 *)kwlnTaskGetUserValue(task);
    itfMesCleanupWindow(entry[9], 0);
    sdfReleaseChipBlock(entry);
    btlSetTrackedTaskHandle(9, 0);
}

u32 func_001B0CB8(BtlUnit *object, s32 current, s32 total, s8 mode) {
    u32 color;

    if (mode == 1 && (object->flags & 0x20) != 0) {
        color = 0x4F4E3E40;
    } else if ((object->conditionFlags & 0x4800) != 0) {
        color = 0x4F4E3E40;
    } else if (current * 2 >= total) {
        color = 0xA09DC380;
    } else if (current <= 0) {
        color = 0x4F4E3E40;
    } else {
        u32 nearColor = 0xC8747380;
        color = 0xD1BA7180;
        if (current * 4 < total) {
            color = nearColor;
        }
    }
    return ((color >> 24) | ((color & 0xFF00) << 8)) |
           ((color << 24) | ((color >> 8) & 0xFF00));
}

u8 btlHasRequiredActorStatusBits(s32 arg0) {
    return (~*(u64 *)(arg0 + 0x110) & 0x201) == 0;
}

void func_001B0D70(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x2c) + 0x18);
    *(s16 *)(temp_v0 + 0x2ac) = arg1;
    *(s16 *)(temp_v0 + 0x2ae) = arg2;
    *(s16 *)(temp_v0 + 0x2b0) = arg3;
}

s32 btlCountEligibleLinkedActors(s32 context) {
    s32 node = *(s32 *)(context + 0x228);
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        if ((*(u64 *)(node + 0x110) & 0x201) == 0x201) {
            if ((*(u16 *)(node + 0x120) & 2) != 0) {
                count++;
            }
        }
    }
    return count;
}

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern void func_00101A80(u32, u32);

extern u32 D_003BB3C0;

extern s32 func_001B0E68(s64);

extern void btlReleaseRegisteredChildTaskWork(s64);

void btlStartRegisteredChildTask(void) {
    s32 context = btlGetRuntime();
    u32 data = (u32)sdfAllocAndClearQuadwords(0x20);
    u32 task = kwlnTaskCreate(D_003BB3C0, 0x2B0E, 1, 1, func_001B0E68, btlReleaseRegisteredChildTaskWork, data);
    func_00101A80(*(u32 *)(context + 0x29C), task);
    btlSetTrackedTaskHandle(7, task);
}

void func_001B0E48(s32 arg0) {
    *(u32 *)(arg0 + 4) = 1;
    *(u32 *)(arg0 + 12) = 0x80;
    *(u32 *)(arg0 + 16) = 0;
    *(u32 *)(arg0 + 0) = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B0E68);

void btlReleaseRegisteredChildTaskWork(s64 arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(temp_v0);
    btlSetTrackedTaskHandle(7, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2870);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2880);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2890);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A28C0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B1518);

/* AD758 allocates 0x138 bytes. The strip renderer uses the first three rows;
 * the later panel updater handles the other five points and their fades. */
typedef struct BattlePhasePanelWork {
    s32 frames;
    s32 mode;
    s8 phase;
    u8 pad09[0x2F];
    BattleSelectionPosition current[8]; /* 0x38 */
    BattleSelectionPosition saved[8];   /* 0x78 */
    s32 fade[8][4];                    /* 0xB8 */
} BattlePhasePanelWork;

typedef char BattlePhasePanelWork_size_must_be_0x138[
    (sizeof(BattlePhasePanelWork) == 0x138) ? 1 : -1];

extern void fldScaleSceneCoordinateRecord(s32, s32);

extern void func_001B19F8(BattlePhasePanelWork *);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A28F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B19F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2918);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2928);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2938);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B1C88);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2968);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B2390);

s32 btlUpdatePhaseGatedTaskUntilTimeout(s64 task) {
    BattlePhasePanelWork *state = (BattlePhasePanelWork *)kwlnTaskGetUserValue(task);
    s32 phase = btlGetNamedTaskPairStatusOrUnavailable();
    if ((u8)(phase - 1) < 2) {
        return 0;
    }
    func_001B1518(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001B19F8(state);
    }
    func_001B1C88(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001B2390(state);
    }
    ++state->frames;
    return state->frames < 50 ? 0 : -1;
}

void btlReleasePsechgPanelWork(s64 arg0) {
    BattlePhasePanelWork *work;

    work = (BattlePhasePanelWork *)kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(work);
    btlSetTrackedTaskHandle(6, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2988);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2998);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B2AC8);

void btlReleaseBattleScratchBlocks(void) {
    sdfReleaseChipBlock(D_003BD834);
    sdfReleaseChipBlock(D_003BD838);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A29F0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B2D80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A30);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B3300);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B3DC8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B3FB0);

void btlReleaseCmsleffPanelWork(s64 arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(temp_v0);
    btlSetTrackedTaskHandle(5, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B4308);

typedef struct BtlPanelTransitionWork {
    u8 pad00[0x10];
    s32 width;
    u8 pad14[0xC];
    s16 fadeLevels[4];
    BattleSelectionPosition initial[2];
    s8 phase;
    u8 pad39[3];
    BattleSelectionPosition current[2];
} BtlPanelTransitionWork;

extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

void btlUpdatePanelTransitionGradients(BtlPanelTransitionWork *work) {
    s16 *fade;
    s32 i;

    for (i = 3, fade = work->fadeLevels; i >= 0; i--, fade++) {
        *fade += 0x20;
        *fade = *fade <= 0 ? 0 : *fade > 0x80 ? 0x80 : *fade;
    }
    switch (work->phase) {
    case 0:
        work->current[0].x = work->initial[0].x + 0x10;
        work->current[0].y = work->initial[0].y;
        work->current[1].x = work->initial[1].x - 0x10;
        work->current[1].y = work->initial[1].y;
        work->phase++;
        break;
    case 1:
        work->current[0].x -= 2;
        work->current[0].x =
            work->current[0].x <= work->width - work->width / 2 + 0x105 ? work->width - work->width / 2 + 0x105 :
            work->width - work->width / 2 + 0x115 <= work->current[0].x ? work->width - work->width / 2 + 0x115 :
            work->current[0].x;
        work->current[1].x += 2;
        work->current[1].x =
            work->current[1].x <= 0x82 - work->width / 2 ? 0x82 - work->width / 2 :
            0x92 - work->width / 2 <= work->current[1].x ? 0x92 - work->width / 2 :
            work->current[1].x;
        break;
    }
    if (work->fadeLevels[1] < 0x80) {
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        evtSubmitDefaultDepthGradientRect(0x100 - work->width / 2,
            work->current[0].y + 0x1A, work->current[0].x - 0x92, 2,
            (work->fadeLevels[1] << 23) | 0x808080, 0,
            (work->fadeLevels[1] << 23) | 0x808080, 0);
        evtSubmitDefaultDepthGradientRect(work->current[1].x,
            work->current[1].y + 2, work->width / 2 - work->current[1].x + 0x100, 2,
            0, (work->fadeLevels[1] << 23) | 0x808080,
            0, (work->fadeLevels[1] << 23) | 0x808080);
    }
}

void btlFreeRegisteredTaskData(s64 task) {
    btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(1, 0);
}

void btlToggleModelFlagOnInput(void) {
    if (D_00324530[13] < 0) {
        if (mdlFlagTest(0xC0E)) {
            mdlFlagClear(0xC0E);
        } else {
            mdlFlagSet(0xC0E);
        }
    }
}

s32 btlDrawTimedDialogTask(s64 task) {
    BattlePanelColors colors = D_003A2A90;
    MsgQueueTaskData *data;
    s32 expired;
    s32 width;
    s32 half;
    s32 i;

    if (btlGetTrackedTaskHandle(0) == 0) {
        return 0;
    }
    data = (MsgQueueTaskData *)kwlnTaskGetUserValue(task);
    if (data->counter++ < data->value) {
        expired = 0;
    } else {
        expired = 1;
    }
    width = itfMesMeasureEntryItem(data->id, data->unk08, 0);
    data->fade = 0x80;
    for (i = 0; i < 4; i++) {
        colors.values[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x15, i, data->fade);
    }
    half = width / 32;
    func_002BF438(((width >> 4) - half + 0x105) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].width = (width >> 4) << 4;
    func_002BF438((0x100 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].width =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_002BF438((0x92 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x15, 0x53);
    if (expired) {
        return -1;
    }
    itfMesBlk24MoveTo(data->id, (0x100 - (width >> 5)) << 4, 0x220);
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(0);
    itfMesStartEntry(data->id, data->unk08, 0);
    return 0;
}

void btlReleaseDialogTaskData(s64 task) {
    s32 *entry;
    btlGetRuntime();
    entry = (s32 *)kwlnTaskGetUserValue(task);
    itfMesCleanupWindow(entry[1], 0);
    sdfReleaseChipBlock(entry);
    btlSetTrackedTaskHandle(0, 0);
}

void btlCreateMessageWindow(void) {
    u8 *window;
    btlGetRuntime();
    window = (u8 *)sdfAllocAndClearQuadwords(0x40);
    D_003BB3DC = (u32)window;
    *(s32 *)(window + 0x10) = 0x14;
    *(s32 *)(window + 0x18) = 0x1800080;
    *(s32 *)(window + 0x1C) = 0x40800080;
    *(s32 *)(window + 0x20) = 0x40800080;
    *(s32 *)(window + 0x24) = 0x60808080;
    *(s32 *)(window + 0x38) = 0xBB;
    *(s32 *)(window + 0x3C) = 0x196;
    btlSetTrackedTaskHandle(4, 1);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B4D58);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AA0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AB0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AD0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AE0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AF0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B4F10);

void btlReleaseRegisteredTaskBuffer(s64 unused) {
    btlGetRuntime();
    sdfReleaseChipBlock(D_003BB3DC);
    D_003BB3DC = 0;
    btlSetTrackedTaskHandle(4, 0);
}

s32 sndAreSlotsEmpty(void) {
    s32 *slot = (s32 *)(datGameState + 0x2E9DC);
    s32 i;
    for (i = 0; i < 3; i++) {
        if (slot[i] != 0) {
            return 0;
        }
    }
    return 1;
}

extern u8 D_00359160[];

s32 func_001B53E8(s32 arg0) {
    s32 count;
    s32 i;

    func_001ACDF0();
    count = func_001A3740(arg0, D_00359160);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (func_001ACD30(*(u16 *)(D_00359160 + 4 + i * 12), 2) == 0) {
                return 1;
            }
        }
        return 0;
    }
    return count;
}

s64 btlGetTaskState6(void) {
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        s32 *state = *(s32 **)(kwlnTaskGetUserValue(task) + 0x2C);
        return func_001B53E8(state[6]);
    }
    return task;
}

typedef struct FlagEntry {
    u32 unk0;
    u16 id;
    u8 pad6[6];
} FlagEntry;

extern u8 *func_001BD708(u8 *, u16 *);

s64 btlClearFlagEntries(void) {
    u16 count;
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    FlagEntry *entries;
    s32 i;
    if (task != 0) {
        entries = (FlagEntry *)func_001BD708((u8 *)kwlnTaskGetUserValue(task), &count);
        for (i = 0; i < count; i++) {
            func_001ACD30(entries[i].id, 0);
        }
        return 1;
    }
    return task;
}

extern u32 D_003BB3D0;

extern u32 D_003BB3D4;

s64 btlDestroyTaskC(void) {
    s64 result = kwlnTaskGetTaskByName(D_003BB3D0);
    if (result != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xC) != 0) {
            kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xC), 0);
        }
        result = 1;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2B20);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B55A8);

void btlFinishTrackedBattleTaskAndCloseWindow(s64 task) {
    s32 context = btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(12, 0);
    *(u32 *)(context + 0x1F4) |= 0x100000;
    evtFinishMessageWindowAndNotify();
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2B50);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B5970);

s64 btlDestroyTaskD(void) {
    s64 result = kwlnTaskGetTaskByName(D_003BB3D4);
    if (result != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xD) != 0) {
            kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xD), 0);
        }
        result = 1;
    }
    return result;
}

void btlReleaseWindowTask(s64 task) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(13, 0);
    battle->flags |= 0x100000;
    btlTrackedTaskHandles->status.bytes.blocked = 0;
    evtFinishMessageWindowAndNotify();
}

extern u8 btlSoundSlotDefaults[];

void btlInitializeSelectionWork(void) {
    u8 initial[0x20];
    BattleSelectionWork *allocated;
    u32 *source;
    s32 *destination;
    s16 *state;
    s32 i;
    memcpy(initial, btlSoundSlotDefaults, sizeof(initial));
    allocated = sdfAllocAndClearQuadwords(0x30);
    btlLinkedSelectionTaskBuffer = allocated;
    state = allocated->rowFade;
    /* Retail copies coordinate pairs with its cursor on each row's Y word. */
    destination = &allocated->positions[0].y;
    source = (u32 *)initial;
    for (i = 3; i >= 0; i--) {
        *state = 0;
        state++;
        destination[-1] = source[0];
        destination[0] = source[1];
        destination += 2;
        source += 2;
    }
    btlSetTrackedTaskHandle(2, 1);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", btlSoundSlotDefaults);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B5CD8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2BD0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B60E8);

void btlReleaseSelectionTaskBuffer(void) {
    if (btlGetTrackedTaskHandle(2) != 0) {
        sdfReleaseChipBlock(btlLinkedSelectionTaskBuffer);
    }
    btlSetTrackedTaskHandle(2, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6308);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6498);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C10);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6850);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6CF8);

void btlReleaseStwrPanelResource(s64 arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseResourceAllocation(*(s32 *)temp_v0);
    btlSetTrackedTaskHandle(3, 0);
}

s32 btlAreLinkedSceneCountersAtThreshold(void) {
    BattleSelectionWork *base;
    s32 i;
    if (btlGetTrackedTaskHandle(8) == 0) {
        if (btlGetTrackedTaskHandle(2) != 0) {
            base = btlLinkedSelectionTaskBuffer;
            for (i = 0; i < 4; i++) {
                if (base->rowFade[i] < 0x80) {
                    return 0;
                }
                if (base->positions[i].x < 11) {
                    return 0;
                }
            }
            if (base->positions[1].y == base->positions[0].y + 23) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7238);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B74C8);

typedef struct BtlSlot {
    u8 pad0[4];
    u8 state;
    u8 pad5[0x28B];
} BtlSlot;

typedef struct BtlSlotBank {
    u8 pad0[8];
    s32 count;
    u8 padC[0x7C4];
    BtlSlot slots[1];
} BtlSlotBank;

void btlSlotBankPromoteStates(BtlSlotBank *bank) {
    s32 i;
    for (i = 0; i < bank->count; i++) {
        BtlSlot *slot = &bank->slots[i];
        s32 state = slot->state;
        if (state == 1 || state == 2) {
            slot->state = 4;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7880);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7C90);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7F50);

void btlReleaseTrackedTaskResource(void) {
    s64 temp_v0;
    u32 temp_v1;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3B0);
    temp_v1 = kwlnTaskGetUserValue(temp_v0);
    sdfReleaseResourceAllocation(*(s32 *)(temp_v1 + 0x1200));
    btlSetTrackedTaskHandle(8, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B83D8);

typedef struct BtlSlotRow {
    u8 pad_00[0x10];
    u8 state;
    u8 value;
} BtlSlotRow;

void btlUpdateActorSlotPresentationState(BtlUnit *object, s8 mode, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    BtlUnit *node = ((BtlActorWork *)btlGetRuntime())->actorList;
    u8 *entry;
    BtlSlotRow *slotEntry;
    s32 offset;
    for (; node != 0; node = node->next) {
        if (btlHasRequiredActorStatusBits((s32)node) != 0) {
            slot = node->lookupId;
            if (object->identity == node->identity) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        s64 task = kwlnTaskGetTaskByName(D_003BB3B0);
        if (task != 0) {
            entry = (u8 *)kwlnTaskGetUserValue(task);
            if (mode != 2) {
                func_001B8838(entry, mode);
            }
            offset = slot * 0x290 + 0x10;
            slotEntry = (BtlSlotRow *)(entry + offset);
            slotEntry->state = 2;
            slotEntry->value = value;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8650);

void btlResetActorSlotPresentationValue(u8 *object) {
    s32 count = 0;
    u8 slot = 0;
    u8 *node = *(u8 **)(btlGetRuntime() + 0x228);
    u8 *entry;
    s32 offset;
    for (; node != 0; node = *(u8 **)(node + 0x344)) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = *(u8 *)(object + 0x11C);
            if (*(s64 *)(object + 0x108) == *(s64 *)(node + 0x108)) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        entry = (u8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_003BB3B0));
        offset = slot * 0x290 + 0x10;
        entry += offset;
        *(u8 *)(entry + 0x10) = 2;
        *(u8 *)(entry + 0x11) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8838);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B89B0);

void btlUpdateActorSlotStates(u8 *context, s8 mode) {
    u8 *entry = context + 0x80;
    s32 modeZeroState = 3;
    s32 modeNonzeroState = 4;
    s32 i = 2;
    do {
        s32 state = entry[0xC];
        if (state == 1 || state == 2) {
            entry[0xC] = mode == 0 ? modeZeroState : modeNonzeroState;
        }
        i--;
        entry += 0x290;
    } while (i >= 0);
}

typedef struct UiSlotEntry {
    u8 pad00[0x18];
    s8 state;
    u8 pad19[0x277];
} UiSlotEntry;

void btlAdvancePendingSceneSlotStates(u8 *scene) {
    UiSlotEntry *entry = (UiSlotEntry *)(scene + 0xE0);
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->state == 1) {
            entry->state = 5;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8BB0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8CB8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B91F0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CB0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CD0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CE8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D00);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B9318);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B96F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B9A50);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B9E98);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BA198);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BA408);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BA660);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BAB08);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BAE08);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB118);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB440);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D58);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D68);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB6C8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB990);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BBE18);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BC540);

typedef struct UiDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct UiDrawSurface *, void *);
} UiDrawSurface;

extern UiDrawSurface D_003255A8;
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *list);
extern s32 sdfConsCreateDrawPacket(s32 list, s32 texture, s32 context);
extern void sdfQueueGouraudTexturedQuad(
    s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
    s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
    s32 x2, s32 y2, s32 u2, s32 v2, s32 color2,
    s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocate)(s32));

/* Draw a textured command-panel quad with independently colored corners. */
s32 btlDrawGouraudTexturedPanelQuad(s32 x0, s32 y0, s32 x1, s32 y1,
                  s32 x2, s32 y2, s32 x3, s32 y3,
                  s32 u, s32 v, s32 width, s32 height,
                  const s32 *colors, s32 texture) {
    SdfListHead *list;
    s32 uFixed;
    s32 vFixed;
    s32 uRight;
    s32 vBottom;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsCreateDrawPacket((s32)list, texture, 0);
    uFixed = u * 0x10;
    vFixed = v * 0x10;
    uRight = uFixed + width * 0x10;
    vBottom = vFixed + height * 0x10;
    sdfQueueGouraudTexturedQuad((s32)list, 0x40,
        x0 * 0x10 + 0x7000, y0 * 8 + 0x7900, uFixed, vFixed, colors[0],
        x1 * 0x10 + 0x7000, y1 * 8 + 0x7900, uRight, vFixed, colors[1],
        x2 * 0x10 + 0x7000, y2 * 8 + 0x7900, uFixed, vBottom, colors[2],
        x3 * 0x10 + 0x7000, y3 * 8 + 0x7900, uRight, vBottom, colors[3],
        0xFEFFD0, NULL);
    D_003255A8.submit(&D_003255A8, list);
    return 1;
}

s32 btlGetNamedTaskPairStatusOrUnavailable(void) {
    s64 first;
    s64 second;
    if (btlTrackedTaskHandles != 0) {
        first = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
        second = kwlnTaskGetTaskByName(D_003BB3B0);
        if (first == 0 && second == 0) {
            return -128;
        }
        if (btlTrackedTaskHandles->status.bytes.blocked == 0) {
            return btlTrackedTaskHandles->status.bytes.state;
        }
        return 0;
    }
    return -128;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BCB88);


s32 btlUpdateCommandUiTransition(void) {
    BattleTrackedTaskWork *flow;
    s64 task;
    s32 counter;

    if (btlTrackedTaskHandles != 0) {
        task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
        flow = btlTrackedTaskHandles;
        switch (flow->status.bytes.state) {
        case 1:
            flow->counter++;
            if (flow->counter >= flow->threshold) {
                flow->status.bytes.state = 2;
            }
            break;
        case 3:
            counter = flow->counter + 1;
            flow->counter = counter;
            if (counter < flow->threshold) {
                break;
            }
            counter = counter <= 0 ? 0 :
                (counter < flow->threshold ? counter : flow->threshold);
            flow->counter = counter;
            if (btlAreLinkedSceneCountersAtThreshold() != 0) {
                if (task != 0) {
                    func_001B83D8(*(s32 *)(kwlnTaskGetUserValue(task) + 0x2C), 2, 0);
                }
                btlTrackedTaskHandles->status.bytes.state = 0;
            }
            break;
        }
    }
    return 0;
}

void fldInitializeBattleSceneFlow(void) {
    s32 context = btlGetRuntime();
    btlNextScaledRandom(7);
    if ((*(u32 *)(context + 0x1F4) & 0x400) != 0) {
        if (*(u16 *)(context + 0x24C) == 1) {
            btlInvalidateSceneFadeCounts(0);
            btlClearNodeFlags();
        } else {
            btlInvalidateSceneFadeCounts(1);
            btlClearNodeFlags();
        }
    }
    btlToggleModelFlagOnInput();
    btlUpdateCommandUiTransition();
}

void btlDebugPrintf(const char *fmt, ...) {
}

void func_001BCE90(void) {
}

void func_001BCE98(void) {
}

void fldSubmitSceneObjectAtCoordinates(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    itfSetTextDrawLimit(0x13);
    temp_v0 = func_001978E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_001958A0(temp_v0, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(temp_v0);
    itfSetTextDrawLimit(0xffffffffffffffff);
}

extern u8 *D_003BAA8C;

void btlDrawIndexedBattleEntryGlyphs(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_003BAA8C + index * 17, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

extern u8 *D_003BAA84;

void btlQueueIndexedTextWithinDrawLimit(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_003BAA84 + index * 25, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

extern u8 *datBattleSceneRecords;

s32 btlIsSceneActorLimitSatisfied(s32 unused, u32 limit) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    if (*(u16 *)(datBattleSceneRecords + index * 40 + 0x20) & 0x800) {
        return 1;
    }
    if (limit < (u32)btlCountFlaggedSceneActors()) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD0D0);

extern s32 func_001BD0D0(s32, s8);
extern void func_001BD2C0(s32, s16 *, s32, s32, s32);
extern void btlBuildEligibleActorList(s32, s16 *);
extern u8 *func_001BD708(u8 *, u16 *);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2DB0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2DC8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2DD8);

u32 btlGetCommandOptionCount(s32 object, s8 mode, s8 unlimited) {
    s32 kind = func_001BD0D0(object, mode);
    s16 count;

    switch (kind) {
    case 0:
        func_001BD2C0(object, &count, 0, 2, 3);
        *(u32 *)(D_003BD834 + 0x14) = count + 1;
        break;
    case 4:
        func_001BD708((u8 *)object, (u16 *)&count);
        *(u32 *)(D_003BD834 + 0x24) = count;
        break;
    case 2:
        btlBuildEligibleActorList(object, &count);
        *(u32 *)(D_003BD834 + 0x1C) = count;
        break;
    case 3:
        *(u32 *)(D_003BD834 + 0x20) = 3;
        break;
    case 6:
        *(u32 *)(D_003BD834 + 0x2C) = 1;
        break;
    default:
        ((u32 *)(D_003BD834 + 0x14))[kind] = 0;
        break;
    }
    if (unlimited == 0) {
        if (((u32 *)(D_003BD834 + 0x14))[kind] >= 4) {
            return 4;
        }
    }
    return ((u32 *)(D_003BD834 + 0x14))[kind];
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD2C0);

extern u16 D_00359960[];

extern u16 D_00358B20[];

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD4F0);

extern u8 D_00358FE0[];

void btlBuildEligibleActorList(s32 unused, s16 *count) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    s32 total = 0;
    if ((*(u16 *)(datBattleSceneRecords + index * 40 + 0x20) & 0x800) == 0) {
        u8 *selected = (u8 *)(datGameState + 0x12A0);
        u8 *flags = (u8 *)datItemSkillRecords;
        u8 *out = D_00358FE0;
        s32 i;
        for (i = 0; i < 0xC0; i++, flags += 8, selected++) {
            if (*selected != 0 && (*flags & 2)) {
                out[0] = i;
                out[1] = *selected;
                out += 2;
                total++;
            }
        }
    }
    *count = total;
}

extern u8 D_00359160[];

u8 *func_001BD708(u8 *object, u16 *value) {
    s32 result = func_001A3740(*(s32 *)(*(u8 **)(object + 0x2C) + 0x18), D_00359160);
    *value = result;
    return D_00359160;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD750);

extern void btlDrawRetreatCommandLabel(s32);

void fldDispatchSceneKindHandler(s32 arg0) {
    switch (func_001BD0D0(arg0, btlCommandPanelWork->classIndex)) {
    case 0:
        func_001BDF60(arg0, 0, 2, 3);
        return;
    case 4:
        func_001BEB58(arg0);
        return;
    case 2:
        func_001BE8A0(arg0);
        return;
    case 3:
        func_001BE590(arg0);
        return;
    case 6:
        btlDrawRetreatCommandLabel(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BDF60);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BE590);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BE8A0);

void btlDrawRetreatCommandLabel(s32 unused) {
    u8 text[8];
    BattleController *battle;
    s8 *modeFlags;
    s32 color;
    s32 handle;

    memcpy(text, D_003BB450, sizeof(text));
    battle = (BattleController *)btlGetRuntime();
    modeFlags = (s8 *)datBattleSceneRecords;
    if (modeFlags[battle->mode * 0x28] != 0) {
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

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BEB58);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF040);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E50);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E60);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2EA0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF0F8);

void fldClearBattleSceneObject(s64 arg0) {
    u32 temp_v0;
    BattleController *battle;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(temp_v0);
    battle = (BattleController *)btlGetRuntime();
    battle->sceneObjectTask = 0;
    btlReleaseBattleScratchBlocks();
}

void fldInitializeSceneObject(s32 object, s32 owner) {
    memset((void *)object, 0, 0x30);
    *(s32 *)object = 1;
    *(s32 *)(object + 0x28) = owner + 0x20;
    *(s32 *)(object + 0x2C) = owner;
}

INCLUDE_ASM(const s32, "game/code_001A1960", fldGetSceneObjectTaskUserData);

s64 fldGetSceneObjectState(void) {
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (temp_v0 == 0) {
        return temp_v0;
    }
    return *(s32 *)fldGetSceneObjectTaskUserData();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF4C0);

void fldSetSceneObjectAndGroupStates(void) {
    s32 *task = (s32 *)fldGetSceneObjectTaskUserData();
    if (task != 0) {
        BattleCmdPanel *panel = btlCommandPanelWork;
        *task = 5;
        panel->state = 3;
    }
}

void fldGetSceneDirectionStepOffset(s32 *outX, s32 *outY, s32 dir, s32 step) {
    s32 offsets[3][8][2] = {
        {{-7, 3}, {-6, 4}, {-5, 4}, {-4, 5}, {-3, 5}, {-2, 6}, {0, 0}, {0, 0}},
        {{0x22, 3}, {0x21, 4}, {0x20, 4}, {0x1F, 5}, {0x1E, 5}, {0x1D, 6}, {0, 0}, {0, 0}},
        {{0x11, 0x29}, {0x11, 0x28}, {0x11, 0x27}, {0x11, 0x26}, {0x11, 0x25}, {0x11, 0x24}, {0, 0}, {0, 0}},
    };

    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF8B0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BFAD0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2FB8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2FE8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BFDE0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C0650);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3020);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3030);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C08B8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C0DF8);


void btlDrawCenteredPanelSegments(s32 width) {
    u32 color[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 half = width / 2;
    s32 x = width - half + 0x105;
    func_002BF438(x * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].width = width << 4;
    func_002BF438((0x100 - half) * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].width =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_002BF438((0x92 - half) * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x15, 0x53);
}

s32 fldStepSceneStateMachine(s64 handle) {
    BattleController *work = (BattleController *)btlGetRuntime();
    SceneAiWork *state;
    s32 mode;
    s32 i;
    s32 count;
    BtlIndexList *owner;
    if (work->flags & 0x04000000) {
        return 0;
    }
    state = (SceneAiWork *)kwlnTaskGetUserValue(handle);
    switch (state->state) {
    case 1:
        state->state = 2;
        break;
    case 2:
        owner = state->listA;
        mode = 1;
        switch (work->mode) {
        case 0x108:
            if (btlGetEffectActive() == 1) {
                mode = 5;
            }
            break;
        case 0x10E:
            count = btlGetIndexListCount(owner);
            for (i = 0; i < count; i++) {
                if (((BtlUnit *)btlGetIndexListEntry(state->listA, i))->flags & 0x200) {
                    break;
                }
            }
            if (i >= count) {
                mode = 5;
            }
            break;
        }
        func_001C08B8(work, state, mode);
        func_001C0DF8(state);
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

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C13E8);

void fldReleaseSceneSpriteWork(SceneAiWork *work) {
    btlFreeIndexList(work->listB);
    btlFreeIndexList(work->listA);
    sdfReleaseChipBlock(work);
}

void fldReleaseSceneSprite(s64 arg0) {
    SceneAiWork *temp_v0;
    s32 temp_v1;

    temp_v0 = (SceneAiWork *)kwlnTaskGetUserValue(arg0);
    fldReleaseSceneSpriteWork(temp_v0);
    temp_v1 = btlGetRuntime();
    *(u32 *)(temp_v1 + 0x2ac) = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", fldGetSceneScriptTaskUserData);

u32 fldGetSceneScriptState(void) {
    u32 *puVar1;

    puVar1 = (u32 *)fldGetSceneScriptTaskUserData();
    return *puVar1;
}

u32 fldGetSceneScriptValue(void) {
    u32 *puVar1;

    puVar1 = (u32 *)(fldGetSceneScriptTaskUserData() + 0x10);
    return *puVar1;
}

extern s32 fldStepSceneStateMachine(s64);

extern u32 func_001C13E8(s32);

void fldCreateSceneSpriteTask(s32 arg0) {
    BattleController *scene;
    s32 task;
    kwlnTaskGetTaskByName(D_003BB3A0);
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
    scene = (BattleController *)btlGetRuntime();
    task = kwlnTaskCreate(D_003BB3A0, 0x2B0E, 1, 1, fldStepSceneStateMachine, fldReleaseSceneSprite,
                          func_001C13E8(arg0));
    func_00101A80(scene->taskParent, task);
    scene->spriteObject = task;
}

void fldMarkActiveSceneScriptState(void) {
    u32 *temp_v0;

    temp_v0 = (u32 *)fldGetSceneScriptTaskUserData();
    if (temp_v0 != 0) {
        *temp_v0 = 6;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C1850);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A30A8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A30D8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C1988);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3130);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3140);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A31A0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C2158);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C27B0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3220);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C2938);

void fldScaleSceneCoordinateRecord(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    *(s32 *)(temp_v0 + 0xc) = *(s32 *)(temp_v0 + 0x7c) << 4;
    *(s32 *)(temp_v0 + 0x10) = *(s32 *)(temp_v0 + 0x80) << 3;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C2E90);

extern s32 D_003BD83C;

void fldSetSceneSlotRange(s32 index) {
    u8 *scene = (u8 *)D_003BD83C;
    if (*(s8 *)(scene + 0x20) < index) {
        s32 i;
        for (i = 0; i <= index; i++) {
            s32 offset = i * 2;
            scene = (u8 *)D_003BD83C;
            *(s32 *)(scene + (offset + *(s32 *)(scene + 4)) * 4 + 0x38) = 0x80;
            *(u8 *)((*(s32 *)(scene + 4) + offset) + (s32)scene + 0x22) = 3;
        }
        ((u8 *)D_003BD83C)[0x21] = index;
    }
    ((u8 *)D_003BD83C)[0x20] = index;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C3040);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C32B0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C3B28);

void fldInitSceneFadeRecords(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    SceneSlot *slot = scene->slots;
    SceneFadingRecord *record = scene->fadingRecords;
    u32 i = 0;

    while (i < 8 && slot->a != 0) {
        memcpy(&record->slot, slot, sizeof(SceneSlot));
        record->alpha = 0x80;
        record->target = -1;
        i++;
        slot++;
        record++;
    }
    while (i < 8) {
        memset(record, 0, sizeof(*record));
        record->target = -1;
        i++;
        record++;
    }
}

s32 btlFindSceneSlotById(SceneSlot *entry) {
    BattleController *context = (BattleController *)btlGetRuntime();
    SceneSlot *slot = context->slots;
    u32 i;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->id == entry->id) {
            return i;
        }
    }
    return -1;
}

SceneSlot *fldFindSceneSlotRecord(SceneSlot *entry) {
    BattleController *context = (BattleController *)btlGetRuntime();
    s32 index = btlFindSceneSlotById(entry);
    SceneSlot *record = 0;

    if (index != -1) {
        record = &context->slots[index];
    }
    return record;
}

s32 btlFadeStaleSceneSlots(void) {
    BattleController *context = (BattleController *)btlGetRuntime();
    SceneFadingRecord *scene = context->fadingRecords;
    SceneSlot *slot = context->slots;
    u32 i;
    s32 changed = 0;
    for (i = 0; i < 8; i++, slot++, scene++) {
        if (scene->slot.id != slot->id && fldFindSceneSlotRecord(&scene->slot) == 0) {
            if (scene->alpha != 0) {
                scene->alpha -= 8;
                changed = 1;
            }
        }
    }
    return changed;
}

s32 fldCountSceneFadeKinds(BattleController *scene, s32 *outFadeCount) {
    SceneFadingRecord *record = scene->fadingRecords;
    SceneSlot *slot;
    u32 fadeCount = 0;
    s32 countA = 0;
    s32 countB = 0;
    u32 slotCount;
    BattleTrackedTaskWork *global;

    while (fadeCount < 8 && record[fadeCount].slot.a != 0) {
        if (record[fadeCount].slot.a == 1) {
            countA++;
        }
        if (record[fadeCount].slot.a == 2) {
            countB++;
        }
        fadeCount++;
    }
    global = btlTrackedTaskHandles;
    if (global->fadeKindsCached == 0) {
        if (countA != 0 || countB != 0) {
            global->fadeKindACount = countA;
            global->fadeKindBCount = countB;
            global->fadeKindsCached = 1;
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

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C4278);

s32 fldSceneCleanupTask(s64 task) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    if ((scene->flags & 0x200) == 0) {
        return 0;
    }
    if (btlTrackedTaskHandles->presentationState == 1 && btlTrackedTaskHandles->status.bytes.blocked == 0) {
        return 0;
    }
    func_001C4278();
    return 0;
}

void fldResetSceneStatus(void) {
    BattleController *scene;

    scene = (BattleController *)btlGetRuntime();
    scene->cleanupTask = 0;
}

void fldBeginSceneTransition(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    func_001BCB88(1, 8);
    scene->flags |= 0x200;
}

void fldClearSceneTransition(void) {
    btlGetRuntime();
    func_001BCB88(0, 8);
}

void func_001C44F8(void) {
    btlPanelResourcesLoad();
}

u32 fldGetSceneIndexedValue(s32 arg0) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(arg0 * 4 + temp_v0 + 0x4b0);
}

void func_001C4540(void) {
}

extern u32 D_003BB488;

extern s32 fldSceneCleanupTask(s64);

void fldCreateSceneCleanupTask(void) {
    s64 oldTask = kwlnTaskGetTaskByName(D_003BB488);
    BattleController *context;
    u32 task;
    if (oldTask == 0) {
        btlGetRuntime();
    }
    context = (BattleController *)btlGetRuntime();
    task = kwlnTaskCreate(D_003BB488, 0x2B0E, 1, 1, fldSceneCleanupTask, fldResetSceneStatus, 0);
    func_00101A80(context->taskParent, task);
    context->cleanupTask = task;
    btlLoadResourceBlock();
    btlStartRegisteredChildTask();
    func_001B6308();
    func_001B6498(1);
    fldBeginSceneTransition();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C45F0);

static inline void btlDestroyTrackedTaskIfPresent(s32 slot) {
    if (btlGetTrackedTaskHandle(slot) != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(slot), 0);
    }
}

void fldDestroySceneTasksAndBuffers(void) {
    BattleController *battle = (BattleController *)btlGetRuntime();

    if (battle->sceneObjectTask != 0) {
        kwlnTaskDestroyWithHierarchy(battle->sceneObjectTask, 0);
        battle->sceneObjectTask = 0;
    }
    if (battle->spriteObject != 0) {
        kwlnTaskDestroyWithHierarchy(battle->spriteObject, 0);
        battle->spriteObject = 0;
    }
    if (battle->cleanupTask != 0) {
        kwlnTaskDestroyWithHierarchy(battle->cleanupTask, 0);
        battle->cleanupTask = 0;
    }
    if (btlHasRegisteredPsechgPanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(6), 0);
    }
    btlDestroyTrackedTaskIfPresent(5);
    btlDestroyTrackedTaskIfPresent(1);
    btlDestroyTrackedTaskIfPresent(0);
    btlDestroyTrackedTaskIfPresent(8);
    btlDestroyTrackedTaskIfPresent(3);
    btlDestroyTrackedTaskIfPresent(9);
    btlDestroyTrackedTaskIfPresent(10);
    if (btlGetTrackedTaskHandle(2) != 0) {
        btlReleaseSelectionTaskBuffer();
    }
    if (btlGetTrackedTaskHandle(12) != 0) {
        btlDestroyTaskC();
    }
    if (btlGetTrackedTaskHandle(13) != 0) {
        btlDestroyTaskD();
    }
    if (btlCommandPanelWork != NULL) {
        btlReleaseAndClearChipBlock();
    }
    if (btlGetTrackedTaskHandle(4) != 0) {
        btlReleaseRegisteredTaskBuffer(0);
    }
    btlDestroyTrackedTaskIfPresent(7);
    btlReleaseResourceBlock();
    sdfReleaseChipBlock(D_003BD840[1]);
    sdfReleaseChipBlock(D_003BD840[0]);
    sdfReleaseChipBlock((void *)D_003BD83C);
    sdfReleaseChipBlock(btlTrackedTaskHandles);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3248);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3258);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3268);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3278);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A32F0);

