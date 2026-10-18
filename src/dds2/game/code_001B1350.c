#include "fld_area_work.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "btl_special_ability.h"
#include "evt_solar.h"
#include "btl_item_drop.h"
#include "itf_mes_window.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "eff.h"
#include "itf.h"
#include "itf_panel_draw.h"
#include "itf_panel_api.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "btl_action.h"
#include "scr.h"
#include "dat_state.h"
#include "mnu_result.h"
#include "dat_command.h"
#include "sdf_sif_command.h"
#include "mdl_resource_table.h"
#include "kwln_task_lifecycle.h"
#include "btl_target_result.h"
/* Declared before first use (were implicit int). */
extern s32 btlTestSelectedItemCategoryMask(BtlUnit *unit, s32 actorIndex);
extern u32 sdfQueryChannelBits(s32 channel, s32 queryArg, DatPartyRecord *item);
extern u32 sdfQueryChannelValue(s32 channel, s32 queryArg, DatPartyRecord *item);

typedef struct BattleController {
    u8 pad_000[0x214];
    s32 frame;
    u32 flags;
    u32 flags21C;
    u8 pad_220[0x28];
    ActionStateLink *taskHead; /* 0x248 */
    BtlUnit *actors;
    u8 pad_250[0x1E];
    u8 mode; /* 0x26E: mode 3 scales defeat experience in DDS2. */
    u8 pad26F;
    u16 variant; /* 0x270 */
    u8 pad_272[0x22];
    s32 adjustmentRecordIndex; /* 0x294 */
    s32 adjustmentGroupIndex;  /* 0x298 */
    s32 adjustmentEntryIndex;  /* 0x29C */
    u8 pad_2A0[0x24];
    KwlnTask *drawTask;
    u8 pad2C8[0x14];
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2E8 */
    u32 rewardMacca; /* 0x2EC */
    s32 experienceEarned; /* 0x2F0 */
    s32 epEarned; /* 0x2F4 */
    s32 totalAp; /* 0x2F8: defeat EP accumulated for the result screen. */
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103. */
    u8 pad2FE[0x3DE];
    s32 (*commandRangeOverride)(BtlUnit *, s32);
} BattleController;
typedef char BattleControllerTotalApOffsetCheck[
    ((u32)&((BattleController *)0)->totalAp == 0x2F8) ? 1 : -1];

extern s32 D_003B4F70[];

extern DatEnemyRecord *datEnemyRecords;

extern void btlBossDebugPrintf(const char *, ...);

extern s32 D_003B4F78[];

extern s32 D_003B4F74[];


extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);


#include "sdf_debug_api.h"


extern s32 btlRollAiBucket(void);

extern s32 btlAverageLevelsForMaskExcludingStatus20(u32);

extern s32 btlCalculateEnemyExperienceReward(BtlUnit *, BtlUnit *);

extern s32 btlGetEnemyMoney(BtlUnit *, BtlUnit *);

extern u32 btlGetAdjustedSlotAffinity(BtlUnit *, s32, s32);

extern s32 evtRunContext(s32, s32, s32, s32, u16);

extern s32 btlRollAiBucket();

extern s32 btlGetActionRecordLookupValue(s32);

u32 btlEncodeActorIndexAsSelectionMask(u32 id);

f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge);

s8 btlGetActorIndexedSignedValue(BtlUnit *object, s32 index);

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record);

s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *unit, u32 slot);

s32 btlHasMappedSpecialAbilityForSlot(BtlUnit *unit, u32 slot);

s32 btlHasSpecialAbility274(BtlUnit *unit, u32 slot);

s32 btlGetUnitElementAffinity(BtlUnit *battler, s32 element);

s32 btlFlagToElementIndex(s32 flag);

s32 btlGetAbilityAttributeMultiplierPercent(BtlUnit *actor, s32 attr);

u32 btlGetCommandAddedStatus(BtlUnit *attacker, BtlUnit *defender, u32 command,
                  s32 reaction, s32 mode) {
    u32 guardMask;
    s32 guardAbility;
    u32 result;

    if (((BtlState *)btlGetRuntime())->unk220 & 0x80) {
        return 0;
    }
    guardAbility = -1;
    guardMask = 0;
    if (datCommandRecords[command].effectType != 2) {
        if (mode != 1) {
            return 0;
        }
    } else if (mode == 1) {
        return 0;
    }

    result = sdfQueryChannelValue(command, &attacker->partyRecord,
                                 &defender->partyRecord);
    if ((result & 0x8000) && !(defender->status.flags & 0x1000)) {
        result &= ~0x8000;
    }
    switch (command) {
    case 0x94:
        guardMask = 0x20;
        guardAbility = 0x22A;
        break;
    case 0x95:
        guardMask = 0x100;
        guardAbility = 0x22B;
        break;
    case 0x96:
        guardMask = 0x800;
        guardAbility = 0x22C;
        break;
    }

    if (guardMask != 0 && result == 0 && reaction == 2) {
        result = guardMask;
        if (btlGetUnitElementAffinity(defender, btlFlagToElementIndex(guardMask)) & 0x170000) {
            result = 0;
        }
        if ((defender->partyRecord.status & 0x7FFF) >= result) {
            result = 0;
        }
    }
    switch (datCommandRecords[command].unk30) {
    case 4:
        if ((defender->partyRecord.status & 0x7FFF) == 8) {
            return result;
        }
    case 0x14:
        return (defender->partyRecord.status & 0x7FFF) == 0x10 ? result : 0;
    case 0x17:
        return result;
    }

    if (result != 0 && !(datCommandRecords[command].options & 1)) {
        s32 element = btlFlagToElementIndex(result);
        if (btlHasMappedSpecialAbilityForSlot(defender, element) != 0 ||
            btlHasSpecialAbility274(defender, element) != 0 ||
            btlHasEnabledSpecialAbilityForSlot(defender, element) != 0) {
            result = 0;
            sdfDebugPrintf("btl:bad auto guard[%X]\n", element);
        } else if (btlTestSelectedItemCategoryMask(defender, element) != 0) {
            result = 0;
            sdfDebugPrintf("btl:bad counter guard[%X]\n", element);
        }
    }
    if (guardAbility != -1 && (guardMask & result)) {
        attacker->pendingAbilityId = guardAbility;
    }
    return result;
}

s32 btlQueryUnitChannelFlags(BtlUnit *source, BtlUnit *recipient, s32 command, s32 press, s32 kind) {
    s32 flags;
    if (kind != 1) {
        return 0;
    }
    flags = sdfQueryChannelBits(command, (s32)&source->partyRecord, (s32)&recipient->partyRecord);
    if ((recipient->partyRecord.status & 8) != 0 && press == 2) {
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
    value = btlGetUnitElementAffinity(unit, index);
    ratio = btlGetAbilityAttributeMultiplierPercent(unit, index);
    if (ratio != 100) {
        ratio = (u16)value * ratio / 100;
        value = (value & 0xFFFF0000) | ratio;
        value &= 0x7FFFFFFF;
    }
    if (btlHasEnabledSpecialAbilityForSlot(unit, index) != 0) {
        value |= 0x20000;
    }
    if (btlHasSpecialAbility274(unit, index) != 0) {
        value |= 0x40000;
    }
    if (btlHasMappedSpecialAbilityForSlot(unit, index) != 0) {
        value |= 0x10000;
    }
    btlBossDebugPrintf("btl:aisyo=%d%%[%X][ratio=%d]\n",
                      (u16)value, value & 0xFFFF0000, ratio);
    return value;
}

/* DDS2 native 001B17E8. DDS1 PiM checkpoint001 is a semantic reference;
 * DDS2 command-flag, scale-hook, solar and minimum-chance paths are native-led. */
s32 btlAreUnitStatusAndEntryFlagsClear(BtlUnit *);

s32 btlResolveCommandHitResult(BtlUnit *source, BtlUnit *target, s32 command) {
    BtlState *runtime;
    DatPartyRecord *targetRecord;
    s32 entryFlags;
    u32 targetFlags;
    s32 runtimeFlags;
    s32 attribute;
    u32 selection;
    u32 affinity;
    s32 ailment;
    s32 chance;
    f32 hitMultiplier;
    f32 escapeMultiplier;
    f32 conditionMultiplier;
    f32 autoMultiplier;
    f32 affinityMultiplier;

    runtime = btlGetRuntime();
    if (runtime->hitResultOverride != NULL) {
        s32 result = runtime->hitResultOverride(source, target, command);
        if (result != 0) return result;
    }
    targetRecord = &target->partyRecord;
    /* CONTRIBUTING's dead-code-trace exception: native 001B185C retains
     * command << 3 before the entry-flags query. Stock GCSE shares this
     * initialization's shift with the adjacent live command-flags access;
     * flow deletes its early table/address/flags reads. Real live record-owner
     * controls instead retain the full address or do not create this trace.
     * Son2 #6244570135 / BtlSlot #6246044967 document the same mechanism. */
    entryFlags = datCommandRecords[command].flags;
    entryFlags = btlGetEntryFlagsUnlessDisabled(targetRecord);
    targetFlags = target->status.stateFlags;
    runtimeFlags = runtime->unk220;
    if (targetFlags & 0x4000) entryFlags |= 0x20;
    if (runtimeFlags & 0x10) return 0x20000;
    if (runtimeFlags & 8) return 0x40000;
    if ((runtimeFlags & 0x20) && !(targetFlags & 0x200))
        return (entryFlags & 0x20) ? 4 : 2;
    if (runtimeFlags & 0x40) return 1;
    if (datCommandRecords[command].flags & 0x10) return 1;
    attribute = btlGetActorIndexedSignedValue(source, command);
    selection = btlEncodeActorIndexAsSelectionMask(attribute);
    if (!(selection & 0x60001)) {
        affinity = btlGetAdjustedSlotAffinity(target, command, attribute);
        ailment = target->partyRecord.status;
        if (!(((ailment & 0x7FFF) == 4 || (source->partyRecord.flags & 0x10)) && (selection & 6))) {
            if (affinity & 0x20000) return 0x20000;
            if (affinity & 0x40000) return 0x40000;
            if (affinity & 0x10000) return 0x10000;
            if ((ailment & 8) && btlCheckSpecialAbility(targetRecord, 0x26A))
                return (entryFlags & 0x20) ? 4 : 2;
        }
    }
    switch (datCommandRecords[command].unk30) {
    case 4:
        return ((target->partyRecord.status & 0x7FFF) == 8) ? 1 : 4;
    case 20:
        if ((target->partyRecord.status & 0x7FFF) == 16) return 1;
        break;
    }
    switch (datCommandRecords[command].effectType) {
    case 0:
    case 1:
        if (target->partyRecord.status & 0x290F) return 1;
        break;
    }
    if (datCommandRecords[command].stat11 >= 100) return 1;
    conditionMultiplier = 1.0f;
    hitMultiplier = btlGetActorEntryMultiplier(source, 4, 1);
    escapeMultiplier = btlGetActorEntryMultiplier(target, 2, 1);
    if ((source->partyRecord.status & 0x7FFF) == 0x100)
        conditionMultiplier = 0.25f;
    autoMultiplier = 1.0f;
    if (btlDoesEnabledStatusMatchCurrentId(targetRecord, 0xDA) &&
        !(target->status.flags & 0x1000) && !(targetRecord->flags & 0x10))
        autoMultiplier = 0.7f;
    if (runtime->hitChanceScale != NULL)
        autoMultiplier *= runtime->hitChanceScale(source, target, command);
    if (datCommandRecords[command].effectType != 2) {
        if ((source->partyRecord.flags & 0x10) && (source->status.flags & 0x200))
            escapeMultiplier *= datBattleParameters->unkB84;
        if ((target->partyRecord.flags & 0x10) && (target->status.flags & 0x200))
            escapeMultiplier *= datBattleParameters->unkB88;
    }
    switch (datCommandRecords[command].effectType) {
    case 0:
        chance = evtRunContext(3, (s32)&source->partyRecord, (s32)targetRecord, command, 0);
        break;
    case 1:
        chance = evtRunContext(8, (s32)&source->partyRecord, (s32)targetRecord, command, 0);
        if (datCommandRecords[command].hpType == 8 || datCommandRecords[command].hpType == 10) {
            affinity = btlGetAdjustedSlotAffinity(target, command, attribute);
            affinity &= 0xFFFF;
            affinityMultiplier = (f32)affinity / 100.0f;
            chance = (s32)(chance * affinityMultiplier);
            hitMultiplier = 1.0f;
            escapeMultiplier = 1.0f;
            conditionMultiplier = 1.0f;
            autoMultiplier = 1.0f;
            btlBossDebugPrintf("btl:PERDMG hit=%d%%[%.3f]\n", chance, affinityMultiplier);
        }
        break;
    case 2:
        if (!btlAreUnitStatusAndEntryFlagsClear(target)) return 4;
        if ((target->partyRecord.status & 0x7FFF) == 0x800) return 0x10000;
        if (btlGetAdjustedSlotAffinity(target, command, attribute) & 0x100000) return 4;
        chance = evtRunContext(11, (s32)&source->partyRecord, (s32)targetRecord, command, 0);
        switch (command) {
        case 0xE7:
            if (evtGetMirroredSolarPhase() != 8) autoMultiplier *= 0.2f;
            break;
        case 0xE8:
            if (evtGetMirroredSolarPhase() != 0) autoMultiplier *= 0.2f;
            break;
        }
        if (target->status.flags & 0x2000000) chance += 30;
        if (chance >= 96) chance = 95;
        break;
    default:
        chance = 0;
        break;
    }
    btlBossDebugPrintf("btl:[hit:%.3f,esc:%.3f,bat:%.3f,auto:%.3f]\n",
                      hitMultiplier, escapeMultiplier, conditionMultiplier, autoMultiplier);
    chance = (s32)(chance * hitMultiplier * escapeMultiplier * conditionMultiplier * autoMultiplier);
    if (chance < 0) chance = 0;
    if (conditionMultiplier == 1.0f && datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10 && datCommandRecords[command].effectType != 2) {
        if ((source->partyRecord.flags & 0x10) && (source->status.flags & 0x200)) {
            if (chance < datBattleParameters->unkB8C) chance = datBattleParameters->unkB8C;
        }
        if ((target->partyRecord.flags & 0x10) && (target->status.flags & 0x200)) {
            if (chance < datBattleParameters->unkB8D) chance = datBattleParameters->unkB8D;
        }
    }
    btlBossDebugPrintf("btl:hit=%d%%\n", chance);
    if (target->status.stateFlags & 0x200) return 1;
    if (btlRollAiBucket() >= chance) {
        if (datCommandRecords[command].hpType == 8 || datCommandRecords[command].hpType == 10) return 4;
        return (entryFlags & 0x20) ? 4 : 2;
    }
    return 1;
}


void func_001B1F78(void) {
}

extern f32 btlGetRewardLevelMultiplier(BtlUnit *, BtlUnit *, s32);

/* Scale the enemy's normal EP reward; flag 0x2000 multiplies it by 100. */
s32 btlCalculateEnemyExperienceReward(BtlUnit *acquirer, BtlUnit *enemy) {
    s32 result = 0;
    DatEnemyRecord *entry;
    f32 ratio;
    u32 ep;

    if (!(enemy->status.flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(acquirer->status.flags & 0x200)) {
        return result;
    }
    entry = &datEnemyRecords[enemy->partyRecord.unitId];
    ratio = btlGetRewardLevelMultiplier(acquirer, enemy, 1);
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

/* Return the normal/hunt EP level multiplier with the native level-difference caps. */
f32 btlGetRewardLevelMultiplier(BtlUnit *acquirer, BtlUnit *enemy, s32 rewardKind) {
    s32 level;
    s32 difference;
    f32 factor;

    if (datBattleSceneRecords[((BtlState *)btlGetRuntime())->battleMode].flags & 0x400) {
        btlBossDebugPrintf("btl:ep hosei off\n");
        return 1.0f;
    }
    if (acquirer == NULL) {
        level = btlAverageLevelsForMaskExcludingStatus20(4);
    } else {
        level = acquirer->partyRecord.level;
    }
    if (level > 60) {
        level = 60;
    }
    difference = level - enemy->partyRecord.level;
    if (difference > 15) {
        difference = 15;
    } else if (difference < -15) {
        difference = -15;
    }
    factor = datBattleParameters->rewardLevelScale[(15 - difference) * 2 + rewardKind];
    btlBossDebugPrintf("btl:ep[lv=%.3f(%d,%d)]\n",
                       factor, difference, rewardKind);
    return factor;
}

/* Read the money reward with the same eligibility and 100-fold table flag. */
s32 btlGetEnemyMoney(BtlUnit *acquirer, BtlUnit *enemy) {
    s32 result = 0;
    s32 money;
    DatEnemyRecord *entry;
    if (!(enemy->status.flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(acquirer->status.flags & 0x200)) {
        return result;
    }
    entry = &datEnemyRecords[enemy->partyRecord.unitId];
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

/* Hunt EP uses its own table quantity and the ratio calculator's mode 0. */
s32 btlCalculateHuntEpReward(BtlUnit *acquirer, BtlUnit *enemy) {
    DatEnemyRecord *entry = &datEnemyRecords[enemy->partyRecord.unitId];
    f32 ratio = btlGetRewardLevelMultiplier(acquirer, enemy, 0);
    u32 ep = (u32)((f32)entry->huntExperience * ratio);
    if (entry->flags & 0x2000) {
        ep *= 100;
    }
    btlBossDebugPrintf("btl:ep=%d[%d,%.3f](hunt)\n", ep, entry->huntExperience, ratio);
    return ep;
}

/* Recovery hooks receive the acting and selected units. This HP hook is
 * inactive; the MP implementation below does not use the selected unit. */
u32 btlGetHuntHpRecoveryAmount(BtlUnit *actor, BtlUnit *target) {
    return 0;
}

s32 btlCalculateAbilityRecoveryAmount(BtlUnit *unit, BtlUnit *target) {
    s32 recovery = 0;
    if (btlCheckSpecialAbility(&unit->partyRecord, 0x26E) != 0) {
        recovery = (s32)(unit->partyRecord.maxMp * datAbilityParameters[0x26E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    } else if (btlCheckSpecialAbility(&unit->partyRecord, 0x249) != 0) {
        recovery = (s32)(unit->partyRecord.maxMp * datAbilityParameters[0x249 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    }
    btlBossDebugPrintf("btl:hunt mp rec[%d]\n", recovery);
    return recovery;
}

extern s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *);

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *unit, s32 delta) {
    if (btlGetEntryFlagsUnlessDisabled(&unit->partyRecord) & 4) {
        return 0;
    }
    if (btlHasEnemyRecordDefeatExemptionFlag(unit) != 0) {
        return 0;
    }
    if ((unit->partyRecord.status & 0x7FFF) == 0x4000) {
        return 1;
    }
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 0x80)) {
        return 0;
    }
    return unit->partyRecord.hp + delta < 1;
}

s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *unit) {
    return unit->partyRecord.hp * 100 / unit->partyRecord.maxHp < 26;
}

s32 btlWouldUiValueFallBelowQuarter(BtlUnit *unit, s32 delta) {
    s32 adjustedHp = unit->partyRecord.hp + delta;
    if (adjustedHp <= 0) {
        return 1;
    }
    return adjustedHp * 100 / unit->partyRecord.maxHp < 26;
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
    for (actor = ((BattleController *)btlGetRuntime())->actors; actor != 0; actor = actor->nextActor) {
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


/* Terminal policies publish and return; see docs/idioms.md,
 * "Outcome classifiers: late publication sharing". */
s32 btlResolveBattleTerminalOutcome(void) {
    BtlState *state;
    BtlUnit *unit;
    s32 partyCount = 0;
    s32 enemyCount = 0;
    s32 stoneCount = 0;
    s32 defeatedCount = 0;

    state = btlGetRuntime();
    state->eventReady = 0;
    if (state->battleOutcomeOverride != NULL) {
        s32 result = state->battleOutcomeOverride();
        if (result != -1) {
            state->eventReady = result;
            return 1;
        }
    }
    for (unit = state->units; unit != NULL; unit = unit->nextActor) {
        if ((btlUnitStatusPair(unit) & 0x4000000020ULL) == 0x20) {
            defeatedCount++;
        } else if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                partyCount++;
                if (unit->partyRecord.status & 0x800) {
                    stoneCount++;
                }
            }
            if (unit->status.flags & 0x400) {
                enemyCount++;
            }
        }
    }
    if (partyCount == 0) {
        if (defeatedCount != 0) {
            btlBossDebugPrintf("btl:lose[%d,%d,%d]\n", partyCount, enemyCount, stoneCount);
            state->eventReady = 2;
            return 1;
        } else {
            state->eventReady = 4;
            return 1;
        }
    }
    if (state->battleFlags & 0x2000) {
        btlBossDebugPrintf("btl:esc[%d,%d,%d]\n", partyCount, enemyCount, stoneCount);
        state->eventReady = 4;
        return 1;
    }
    if (partyCount == stoneCount) {
        btlBossDebugPrintf("btl:stone[%d,%d,%d]\n", partyCount, enemyCount, stoneCount);
        state->eventReady = 3;
        return 1;
    }
    if (enemyCount == 0) {
        btlBossDebugPrintf("btl:win[%d,%d,%d]\n", partyCount, enemyCount, stoneCount);
        if (state->requestArgument != 0) {
            state->eventReady = 5;
        } else {
            state->eventReady = 1;
        }
        return 1;
    }
    return 0;
}

s32 btlSelectedEntryHitsElement(BtlUnit *source, BtlUnit *unit, s32 selectorIndex) {
    u32 indexedSelectorValue;
    s32 selectionMask;
    u32 maskTableIndex;
    if (unit->selectedEntryIndex <= 0) {
        return 0;
    }
    btlGetRuntime();
    indexedSelectorValue = btlGetActorIndexedSignedValue(source, selectorIndex);
    selectionMask = btlEncodeActorIndexAsSelectionMask(indexedSelectorValue);
    maskTableIndex = datCommandRecords[unit->selectedEntryIndex].commandEffectId;
    if (maskTableIndex == 0) {
        return 0;
    }
    if (indexedSelectorValue >= 0x10 && (indexedSelectorValue < 0x12 || indexedSelectorValue == -1)) {
        return 0;
    }
    if (maskTableIndex >= 0x21) {
        return 0;
    }
    return (D_003B4F78[maskTableIndex * 3] & selectionMask) != 0;
}

s32 btlGetActionRecordLookupValue(s32 actionRecordIndex) {
    u16 lookupTableIndex;

    lookupTableIndex = datCommandRecords[actionRecordIndex].commandEffectId;
    return D_003B4F70[lookupTableIndex * 3];
}

s32 btlTestSelectedItemCategoryMask(BtlUnit *unit, s32 actorIndex) {
    s32 selectedEntryIndex = unit->selectedEntryIndex;
    u16 maskTableIndex;
    if (selectedEntryIndex == -1) {
        return 0;
    }
    maskTableIndex = datCommandRecords[selectedEntryIndex].commandEffectId;
    return (D_003B4F78[maskTableIndex * 3] & btlEncodeActorIndexAsSelectionMask(actorIndex)) != 0;
}

s32 fldGetSelectedUnitStat(BtlUnit *unit) {
    s32 index = unit->selectedEntryIndex;
    if (index == -1) {
        return 0;
    }
    return btlGetActionRecordLookupValue(index);
}

s32 btlGetSelectedUnitProperty(BtlUnit *unit) {
    s32 index = unit->selectedEntryIndex;
    u16 property;
    if (index == -1) {
        return 0;
    }
    property = datCommandRecords[index].commandEffectId;
    return D_003B4F74[property * 3];
}

s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *targets, BtlTargetResult *results) {
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
        if (results->skipped != 0) {
            skipped++;
        }
    }
    count = 0;
    for (actor = controller->actors; actor != NULL; actor = actor->nextActor) {
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
