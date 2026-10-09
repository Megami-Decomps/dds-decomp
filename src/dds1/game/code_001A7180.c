#include "common.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "fr_font_measure.h"
#include "fr_font.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "pcp_vu0.h"
#include "btl_ui.h"
#include "sdf.h"
#include "btl.h"
#include "btl_state.h"
#include "eff.h"
#include "btl_action.h"
#include "kwln.h"
#include "dat_state.h"
#include "dat_command.h"
#include "kwln_task_lifecycle.h"

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
    u8 pad_254[6];
    u16 spawnCount; /* 0x25A: enemies spawned so far */
    u8 pad_25C[0x14];
    s32 adjustmentRecordIndex;
    s32 adjustmentGroupIndex;
    s32 adjustmentEntryIndex;
    s32 mode;
    u8 pad_280[0x1C];
    KwlnTask *taskParent;
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
    u8 pad_5B4[0xD4];
    u32 (*enemyLimitOverride)(BtlUnit *, DatBattleSceneRecord *, u32); /* 0x688 */
} BattleController;

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

extern s32 D_00358510[];

extern DatEnemyRecord *datEnemyRecords;

extern s32 func_001A8DD8(s32, s32 *);

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

extern s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *);

extern s32 btlGetRuntime(void);

extern s32 effMiscRandMod(s32, s32);

extern s32 func_001A9488(u32);

extern s32 btlCalculateEnemyExperienceReward(u8 *, u8 *);

extern s32 btlGetEnemyMoney(u8 *, u8 *);

u32 btlEncodeActorIndexAsSelectionMask(u32 id);

s32 btlGetAbilityAttributeMultiplierPercent(BtlUnit *actor, s32 attr);

f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge);

s8 btlGetActorIndexedSignedValue(BtlUnit *unit, s32 index);

s32 btlGetCommandFailureReason(BtlUnit *unit, s32 command);

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record);

s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *unit, u32 slot);

s32 btlHasMappedSpecialAbilityForSlot(BtlUnit *unit, u32 slot);

u32 btlHasSpecialAbilityWhenArgumentUnset(BtlUnit *arg0, s64 arg1);

s8 func_001A2AE8(s32 command);

s32 func_001A2F50(BtlUnit *battler, s32 element);

s32 func_001A4060(s32 flag);

extern s32 sdfQueryChannelValue(s32, DatPartyRecord *, DatPartyRecord *);
extern s32 func_003003F0(const char *, ...);

s32 func_001A7180(BtlUnit *attacker, BtlUnit *defender, s32 command, s32 variant, s32 mode) {
    DatPartyRecord *record;
    s32 value;
    s32 guard;

    if (((BtlState *)btlGetRuntime())->unk_1FC & 0x80) {
        return 0;
    }
    if (datCommandRecords[command].effectType != 2) {
        if (mode != 1) {
            return 0;
        }
    } else if (mode == 1) {
        return 0;
    }
    record = &attacker->partyRecord;
    value = sdfQueryChannelValue(command, record, &defender->partyRecord);
    if ((value & 0x8000) && !(defender->status.flags & 0x1000)) {
        value &= ~0x8000;
    }
    if (value == 0 && variant == 2) {
        if (btlCheckSpecialAbility(record, 0x20A)) {
            return 0x20;
        }
        if (btlCheckSpecialAbility(record, 0x20B)) {
            return 0x100;
        }
        if (btlCheckSpecialAbility(record, 0x20C)) {
            return 0x800;
        }
    }
    if (datCommandRecords[command].unk30 == 4 && (defender->partyRecord.status & 0x7FFF) == 8) {
        return value;
    }
    if (value != 0 && !(datCommandRecords[command].options & 1)) {
        guard = func_001A4060(value);
        if (btlHasMappedSpecialAbilityForSlot(defender, guard) || btlHasSpecialAbilityWhenArgumentUnset(defender, guard) ||
            btlHasEnabledSpecialAbilityForSlot(defender, guard)) {
            value = 0;
            func_003003F0("btl:bad auto guard[%X]\n", guard);
        } else if (btlTestSelectedItemCategoryMask(defender, guard)) {
            value = 0;
            func_003003F0("btl:bad counter guard[%X]\n", guard);
        }
    }
    return value;
}

s32 btlQueryUnitChannelFlags(s32 first, s32 second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = sdfQueryChannelBits(other, first + 0x120, second + 0x120);
    if ((((BtlUnit *)second)->partyRecord.status & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

u32 btlGetAdjustedSlotAffinity(BtlUnit *unit, s32 unused, s32 index) {
    s32 selection;
    u32 value;
    u32 ratio;

    if (index == -1) {
        return 0;
    }
    selection = btlEncodeActorIndexAsSelectionMask(index);
    if ((selection & 1) != 0) {
        return 0;
    }
    if ((selection & 0xE0000) != 0) {
        return 100;
    }
    value = func_001A2F50(unit, index);
    ratio = btlGetAbilityAttributeMultiplierPercent(unit, index);
    if (ratio != 100) {
        ratio = (u16)value * ratio / 100;
        value = (value & 0xFFFF0000) | ratio;
        value &= 0x7FFFFFFF;
    }
    if (btlHasEnabledSpecialAbilityForSlot(unit, index) != 0) {
        value |= 0x20000;
    }
    if (btlHasSpecialAbilityWhenArgumentUnset(unit, index) != 0) {
        value |= 0x40000;
    }
    if (btlHasMappedSpecialAbilityForSlot(unit, index) != 0) {
        value |= 0x10000;
    }
    btlBossDebugPrintf("btl:aisyo=%d%%[%X][ratio=%d]\n",
                      (u16)value, value & 0xFFFF0000, ratio);
    return value;
}

INCLUDE_ASM(const s32, "game/code_001A7180", func_001A7548);

void func_001A7AD0(void) {
}

extern f32 func_001A7C20(u8 *, u8 *, s32);

/* Scale the enemy's normal EP reward; flag 0x2000 multiplies it by 100. */
s32 btlCalculateEnemyExperienceReward(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    DatEnemyRecord *entry;
    f32 ratio;
    u32 ep;

    if (!(((BtlUnit *)enemy)->status.flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(((BtlUnit *)acquirer)->status.flags & 0x200)) {
        return result;
    }
    entry = &datEnemyRecords[((BtlUnit *)enemy)->partyRecord.unitId];
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

extern char D_003A1B90[];

extern char D_003A1BA8[];

f32 func_001A7C20(u8 *acquirer, u8 *enemy, s32 rewardKind) {
    s32 level;
    s32 difference;
    f32 factor;

    if (datBattleSceneRecords[((BattleController *)btlGetRuntime())->mode].flags & 0x400) {
        btlBossDebugPrintf(D_003A1B90);
        return 1.0f;
    }
    if (acquirer == NULL) {
        level = func_001A9488(4);
    } else {
        level = ((BtlUnit *)acquirer)->partyRecord.level;
    }
    if (level > 60) {
        level = 60;
    }
    difference = level - ((BtlUnit *)enemy)->partyRecord.level;
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
INCLUDE_RODATA(const s32, "game/code_001A7180", D_003A1B90);

INCLUDE_RODATA(const s32, "game/code_001A7180", D_003A1BA8);

s32 btlGetEnemyMoney(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    s32 money;
    DatEnemyRecord *entry;
    if (!(((BtlUnit *)enemy)->status.flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(((BtlUnit *)acquirer)->status.flags & 0x200)) {
        return result;
    }
    entry = &datEnemyRecords[((BtlUnit *)enemy)->partyRecord.unitId];
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
s32 btlCalculateHuntEpReward(u8 *acquirer, u8 *enemy) {
    DatEnemyRecord *entry = &datEnemyRecords[((BtlUnit *)enemy)->partyRecord.unitId];
    f32 ratio = func_001A7C20(acquirer, enemy, 0);
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

s32 btlCalculateAbilityRecoveryAmount(BtlUnit *actor) {
    s32 recovery = 0;
    if (btlCheckSpecialAbility(&actor->partyRecord, 0x24E)) {
        recovery = (s32)(actor->partyRecord.maxMp * datAbilityParameters[0x24E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    } else if (btlCheckSpecialAbility(&actor->partyRecord, 0x229)) {
        recovery = (s32)(actor->partyRecord.maxMp * datAbilityParameters[0x229 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    }
    btlBossDebugPrintf(D_003A1C10, recovery);
    return recovery;
}

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta) {
    if (btlGetEntryFlagsUnlessDisabled(&actor->partyRecord) & 4) return 0;
    if (btlHasEnemyRecordDefeatExemptionFlag(actor)) return 0;
    if ((actor->partyRecord.status & 0x7FFF) == 0x4000) return 1;
    if ((((BtlState *)btlGetRuntime())->battleFlags & 0x80) == 0) return 0;
    return actor->partyRecord.hp + delta < 1;
}

s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *object) {
    return object->partyRecord.hp * 100 / object->partyRecord.maxHp < 25;
}

s32 btlWouldUiValueFallBelowQuarter(BtlUnit *object, s32 delta) {
    s32 value = object->partyRecord.hp + delta;
    if (value <= 0) {
        return 1;
    }
    return value * 100 / object->partyRecord.maxHp < 25;
}

s32 btlBothSidesActive(BtlUnit *unit) {
    BtlUnit *actor;
    s32 a;
    s32 b;
    if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0) != 0) {
        return 0;
    }
    if (unit->status.flags & 0x60) {
        return 0;
    }
    a = 0;
    b = 0;
    for (actor = ((BattleController *)btlGetRuntime())->actors; actor != 0; actor = actor->next) {
        if (actor->status.flags & 1) {
            if (!(actor->status.flags & 0xE0)) {
                if (actor->status.flags & 0x200) {
                    a++;
                }
                if (actor->status.flags & 0x400) {
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
    if ((object->status.flags & 0x400) != 0) {
        if (object->partyRecord.unitId >= 0x100) {
            return 0;
        }
    }
    return 1;
}

s32 btlIsUnitStatusFlagClear(BtlUnit *object) {
    if ((object->partyRecord.status & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001A7180", D_003A1C10);

INCLUDE_ASM(const s32, "game/code_001A7180", func_001A8188);

extern s32 D_00358514[];

extern s32 D_00358518[];

s32 btlSelectedEntryHitsElement(s32 unitAddress, BtlUnit *unit, s32 selectorIndex) {
    u32 indexedSelectorValue;
    s32 selectionMask;
    u32 maskTableIndex;
    if (unit->selectedEntryIndex <= 0) {
        return 0;
    }
    btlGetRuntime();
    indexedSelectorValue = btlGetActorIndexedSignedValue(unitAddress, selectorIndex);
    selectionMask = btlEncodeActorIndexAsSelectionMask(indexedSelectorValue);
    maskTableIndex = datCommandRecords[unit->selectedEntryIndex].unk2E;
    if (maskTableIndex == 0) {
        return 0;
    }
    if (indexedSelectorValue >= 0x10 && (indexedSelectorValue < 0x12 || indexedSelectorValue == -1)) {
        return 0;
    }
    if (maskTableIndex >= 0x20) {
        return 0;
    }
    return (D_00358518[maskTableIndex * 3] & selectionMask) != 0;
}

s32 btlGetActionRecordLookupValue(s32 actionRecordIndex) {
    u16 lookupTableIndex;

    lookupTableIndex = datCommandRecords[actionRecordIndex].unk2E;
    return D_00358510[lookupTableIndex * 3];
}

s32 btlTestSelectedItemCategoryMask(BtlUnit *unit, s32 actorIndex) {
    s32 selectedEntryIndex = unit->selectedEntryIndex;
    u16 maskTableIndex;
    if (selectedEntryIndex == -1) {
        return 0;
    }
    maskTableIndex = datCommandRecords[selectedEntryIndex].unk2E;
    return (D_00358518[maskTableIndex * 3] & btlEncodeActorIndexAsSelectionMask(actorIndex)) != 0;
}

s32 fldGetSelectedUnitStat(s32 unitAddress) {
    s32 selectedEntryIndex = ((BtlUnit *)unitAddress)->selectedEntryIndex;
    if (selectedEntryIndex == -1) {
        return 0;
    }
    return btlGetActionRecordLookupValue(selectedEntryIndex);
}

s32 btlGetSelectedUnitProperty(s32 unitAddress) {
    s32 selectedEntryIndex = ((BtlUnit *)unitAddress)->selectedEntryIndex;
    if (selectedEntryIndex == -1) {
        return 0;
    }
    return D_00358514[datCommandRecords[selectedEntryIndex].unk2E * 3];
}

s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *targets, BtlOperandGroup *results) {
    s32 i = 0;
    u32 sides = 0;
    BattleController *controller = (BattleController *)btlGetRuntime();
    u32 count = btlGetIndexListCount(targets);
    s32 skipped;
    BtlUnit *actor;

    for (; i < count; i++) {
        sides |= ((BtlUnit *)btlGetIndexListEntry(targets, i))->status.flags & 0x600;
    }
    skipped = 0;
    for (i = 0; i < count; i++, results++) {
        if (results->inactive != 0) {
            skipped++;
        }
    }
    count = 0;
    for (actor = controller->actors; actor != NULL; actor = actor->next) {
        if (actor->status.flags & 1) {
            if (actor->status.flags & 0xE0) {
                continue;
            }
            if (actor->status.flags & sides) {
                count++;
            }
        }
    }
    return skipped == count;
}

const char D_003A1C88[] = "btl:escape=%d%%[ratio=%.2f]\n";

extern s32 evtRunContext(s32, s32, s32, s32, u16);

s32 btlRollEscapeChance(BtlUnit *actor) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *enemy;
    s32 noEligibleEnemy;
    s32 chance;
    f32 ratio;
    DatPartyRecord *record;

    if (datBattleSceneRecords[battle->battleMode].unk00 != 0) {
        return 0;
    }
    if (datBattleSceneRecords[battle->battleMode].flags & 0x20) {
        return 1;
    }
    noEligibleEnemy = 1;
    for (enemy = battle->units; enemy != NULL; enemy = enemy->next) {
        if ((enemy->status.flags & 1) != 0) {
            if ((enemy->status.flags & 0x400) != 0) {
                if ((enemy->partyRecord.status & 0x2A0F) == 0) {
                    noEligibleEnemy = 0;
                    break;
                }
            }
        }
    }
    if (noEligibleEnemy) {
        return 1;
    }
    record = &actor->partyRecord;
    chance = evtRunContext(0x14, (s32)record, 0, 0, 0);
    ratio = 1.0f;
    if (btlCheckSpecialAbility(record, 0x230)) {
        ratio = datAbilityParameters[0x230 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    chance = chance * ratio;
    if (chance > 95) {
        chance = 95;
    }
    btlBossDebugPrintf(D_003A1C88, chance, ratio);
    return btlRollAiBucket() < chance;
}

extern u8 *datEnemyAiRecords;

s32 btlIsSelectedActorStatusAndRecordClear(s32 object) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    if (datBattleSceneRecords[index].unk00 != 0) {
        return 0;
    }
    if ((((BtlUnit *)object)->partyRecord.status & 0x2A0F) != 0) {
        return 0;
    }
    return datEnemyAiRecords[((BtlUnit *)object)->partyRecord.unitId * 0x15C] == 0;
}

/* Return true when neither actor status nor its entry flags contain bit 0x40. */
s32 btlAreUnitStatusAndEntryFlagsClear(s32 actor) {
    if ((((BtlUnit *)actor)->partyRecord.status & 0x40) != 0) {
        return 0;
    }
    return (btlGetEntryFlagsUnlessDisabled(&((BtlUnit *)actor)->partyRecord) & 0x40) < 1;
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

    requirementCursor = datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].requirements;
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount < 2) {
        return 0;
    }

    statAddresses[0] = (s32)&unit->partyRecord;
    for (candidate = controller->actors; candidate != NULL; candidate = candidate->next) {
        u32 flags = candidate->status.flags;

        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 0x400) == 0) {
            continue;
        }
        if ((candidate->partyRecord.status & 0x2A0E) != 0) {
            continue;
        }
        if (unit == candidate) {
            continue;
        }
        statAddresses[1] = (s32)&candidate->partyRecord;
        if (requiredCount == 3) {
            for (partner = controller->actors; partner != NULL; partner = partner->next) {
                u32 partnerFlags = partner->status.flags;

                if ((partnerFlags & 1) == 0) {
                    continue;
                }
                if ((partnerFlags & 0x400) == 0) {
                    continue;
                }
                if ((partner->partyRecord.status & 0x2A0E) != 0) {
                    continue;
                }
                if (unit == partner || candidate == partner) {
                    continue;
                }
                statAddresses[2] = (s32)&partner->partyRecord;
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

INCLUDE_ASM(const s32, "game/code_001A7180", func_001A8A30);

s32 btlHasAdjacentActorRecordStatus(s32 object) {
    u8 *status = datEnemyRecords[((BtlUnit *)object)->partyRecord.unitId].unk3E;
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
    return (((s32)((BtlUnit *)actor)->status.flags >> 0x1a) ^ 1U) & 1;
}

/* Return 1 when the scene can still accept another active enemy: below the active cap and the cumulative spawn limit. */
s32 func_001A8CE0(BtlUnit *requester) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    u32 limit;
    u32 active;
    u32 spawnLimit;
    BtlUnit *unit;

    if (datBattleSceneRecords[controller->mode].maxActiveEnemies >= 6) {
        return 0;
    }
    limit = 5;
    if (datBattleSceneRecords[controller->mode].maxActiveEnemies != 0) {
        limit = datBattleSceneRecords[controller->mode].maxActiveEnemies;
    }
    if (controller->enemyLimitOverride != 0) {
        limit = controller->enemyLimitOverride(requester, &datBattleSceneRecords[controller->mode], limit);
    }
    active = 0;
    for (unit = controller->actors; unit != 0; unit = unit->next) {
        s32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                active++;
            }
        }
    }
    if (active >= limit) {
        return 0;
    }
    if (datBattleSceneRecords[controller->mode].maxEnemySpawns != 0) {
        spawnLimit = datBattleSceneRecords[controller->mode].maxEnemySpawns;
        if (controller->spawnCount >= spawnLimit) {
            return 0;
        }
    }
    return 1;
}

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
                s32 category = datCommandSelectors[id].kind;
                if (category != 2) {
                    if (category != 4) {
                        if ((datCommandRecords[id].unk_01 & 2) != 0) {
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

f32 btlGetActionCategoryMultiplier(BtlUnit *object, s32 unused, s32 index) {
    s32 category = datCommandRecords[index].hpType;
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    if (index == 0 && btlCheckSpecialAbility(&object->partyRecord, 0x21D) != 0) {
        return datAbilityParameters[0x21D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    return 0.0f;
}

f32 btlGetActionCategoryGateAsFloat(s32 unused0, s32 unused1, s32 index) {
    s32 category = datCommandRecords[index].mpType;
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 btlGetActionRecordPercentAsFraction(s32 unused0, s32 unused1, s32 index) {
    return (f32)datCommandRecords[index].effectPercent / 100.0f;
}

u32 btlAdjustPointsForCombatFlags(u32 flags, u32 secondary, u32 points, s32 index) {
    if (flags & 0x20000) return 0x1194;
    if (flags & 0x40000) return 0x1194;
    if (flags & 0x10000) return points + 0x64;
    if (flags & 2) return points + 0x64;
    if (secondary & 4) return points >> 1;
    if (secondary & 2) return points >> 1;
    if (flags & 4) {
        if (datCommandRecords[index].hpType == 8 ||
            datCommandRecords[index].hpType == 10) {
            return points;
        }
        if (datCommandRecords[index].effectType == 2) return points;
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
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    s32 sum = 0;
    s32 count = 0;
    for (; actor != NULL; actor = actor->next) {
        u32 flags = actor->status.flags;
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((actor->partyRecord.flags & mask) != 0) {
                    count++;
                    sum += actor->partyRecord.maxHp;
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
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    s32 sum = 0;
    s32 count = 0;
    for (; actor != NULL; actor = actor->next) {
        u32 flags = actor->status.flags;
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((actor->partyRecord.flags & mask) != 0) {
                    count++;
                    sum += actor->partyRecord.hp;
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
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    s32 sum = 0;
    s32 count = 0;
    for (; actor != NULL; actor = actor->next) {
        u32 flags = actor->status.flags;
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((actor->partyRecord.flags & mask) != 0) {
                    count++;
                    sum += actor->partyRecord.level;
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
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    for (; actor != NULL; actor = actor->next) {
        u32 flags = actor->status.flags;
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((actor->partyRecord.flags & mask) != 0) {
                    s32 value = datGetStatWithStatusOverride(&actor->partyRecord, attribute);
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

extern void func_0011CE38();

const char D_003A1CA8[] = "btl:preemptive=%d%%[ratio=%.2f]\n";

s32 func_001A95C8(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    s32 chance;
    f32 ratio;
    u32 i;

    if (state->battleFlags & 0x4000) {
        if (state->requestMode == 2 || state->requestMode == 4) {
            if (datBattleSceneRecords[state->battleMode].flags & 2) {
                return 2;
            }
        }
        return 1;
    }
    if (datBattleSceneRecords[state->battleMode].flags & 4) {
        return 1;
    }
    if (datBattleSceneRecords[state->battleMode].flags & 2) {
        return 2;
    }
    ratio = 1.0f;
    chance = evtRunContext(0x16, 0, 0, 0, 0);
    for (i = 0; i < 5; i++) {
        if (datGameState->party[i].flags & 1) {
            if (datGameState->party[i].flags & 2) {
                if (btlCheckSpecialAbility(&datGameState->party[i], 0x22E)) {
                    ratio *= datAbilityParameters[0x22E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
                }
            }
        }
    }
    chance = (s32)((f32)chance * ratio);
    if (chance < 60) {
        chance = 60;
    } else if (chance > 80) {
        chance = 80;
    }
    btlBossDebugPrintf(D_003A1CA8, chance, ratio);
    func_0011CE38(chance);
    return btlRollAiBucket() < chance ? 1 : 2;
}
