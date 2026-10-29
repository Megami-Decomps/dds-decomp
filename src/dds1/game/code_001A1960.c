#include "sdk_ps2.h"
#include "evt_run_context_api.h"
#include "common.h"
#include "btl_selected_entry_api.h"
#include "btl_actor_command_selector_value_api.h"
#include "pty_active_roster_index_api.h"
#include "btl_command_use_error.h"
#include "btl_command_cost_api.h"
#include "btl_action_press_result.h"
#include "btl_command_target_policy.h"
#include "btl_target_list_builder.h"
#include "btl_command_status_target_index.h"
#include "btl_actor_resource_bank_api.h"
#include "btl_actor_entry_code_defaults.h"
#include "btl_special_ability.h"
#include "dat_unit_has_skill_api.h"
#include "btl_animation_mode_entry.h"
#include "btl_effect_position.h"
#include "eff_random_mod.h"
#include "eff_random.h"
#include "btl_enemy_selection_result.h"
#include "btl_enemy_selection_context.h"
#include "btl_item_drop.h"
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
#include "btl_indexed_ui_resource_api.h"
#include "btl.h"
#include "btl_actor_entry_code_api.h"
#include "btl_flag_to_element_index_api.h"
#include "btl_command_affinity_api.h"
#include "btl_active_units_ready_api.h"
#include "btl_state.h"
#include "eff.h"
#include "btl_action.h"
#include "kwln.h"
#include "dat_state.h"
#include "dat_calculated_stat_value_api.h"
#include "dat_affinity_selector.h"
#include "dat_effective_affinity_api.h"
#include "dat_calculate_command_base_value_api.h"
#include "btl_resolve_unit_value_api.h"
#include "btl_select_transform_parameters_api.h"
#include "dat_command.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"
#include "snd_slot.h"

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 datComputeSkillBoostedMaxHp(datUnitWork_t *);

extern u32 datComputeSkillBoostedMaxMp(datUnitWork_t *);

extern void datResetBadStat(datUnitWork_t *record, s32 mask);

extern u8 btlSharedActorTransformParameters[];

extern void *btlCommandAttributeNames[];

extern const u8 *btlPartyUnitGunTypeNames[];


extern DatEnemyRecord *datEnemyRecords;


extern u8 *D_003BAA14;

extern u8 *D_003BAA28;

extern u8 *D_003BAA10;

extern u8 *D_003BAA20;

extern u8 *D_003BAA30;

extern EventRosterStat *datRosterDetails;

extern s32 fldCountSceneSlots(void);


extern char D_003A1A40[];

extern datUnitWork_t *btlGetIndexedPartyEntryRecord(s32);

extern EffRandState effSharedRandomState[];

/* Clear the supplied party-record status mask. */
void btlClearPartyRecordStatusBits(datUnitWork_t *record, s32 mask) {
    datResetBadStat(record, mask);
}

/* Set the actor's selected command-record index. */
void btlSetActorSelectedEntryIndex(BtlUnit *actor, BtlCommandId commandId) {
    actor->selectedEntryIndex = commandId;
}

/* BTL_COMMAND_ID_INVALID represents no selected entry. */
void btlClearActorSelectedEntryIndex(BtlUnit *actor) {
    actor->selectedEntryIndex = BTL_COMMAND_ID_INVALID;
}


/* Initialize enemy vitals/stats and pack its nonzero skill IDs into the party record. */
void btlInitializePartyRecordFromEnemy(datUnitWork_t *entry, s32 index) {
    u32 count;
    u32 i;

    memset(entry, 0, sizeof(*entry));
    entry->flags = 1;
    entry->affinityTableIndex = 0;
    entry->unitId = index;
    entry->level = datEnemyRecords[index].level;
    entry->totalExp = 0;
    for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
        entry->baseStats[i] = datEnemyRecords[index].baseStats[i];
    }
    count = 0;
    for (i = 0; i < 8; i++) {
        if (datEnemyRecords[index].skills[i] != 0) {
            entry->effectData[count++] = datEnemyRecords[index].skills[i];
        }
    }
    for (i = count; i < 24; i++) {
        entry->effectData[i] = 0;
    }
    entry->effectDataCount = count;
    entry->maxHp = datEnemyRecords[index].maxHp;
    entry->hp = datEnemyRecords[index].hp;
    entry->maxMp = datEnemyRecords[index].maxMp;
    entry->mp = datEnemyRecords[index].mp;
}

datUnitWork_t *btlGetActorEntryData(BtlUnit *actor) {
    datUnitWork_t *entry;

    if ((actor->status.flags & 0x400) == 0) {
        entry = btlGetIndexedPartyEntryRecord(actor->partyIndex);
        return entry;
    }
    return &actor->partyRecord;
}

datUnitWork_t *btlGetCurrentPartyEntryRecord(DatUnitId unitId) {
    PtyActiveRosterIndex index;

    index = dds3FindEntryIndex(unitId);
    return &DDS3_GBWK->party[index];
}

datUnitWork_t *btlGetIndexedPartyEntryRecord(s32 index) {
    return &DDS3_GBWK->party[index];
}

void btlSyncPlayerWork(BtlUnit *actor) {
    datUnitWork_t *src = &actor->partyRecord;
    datUnitWork_t *dst = btlGetIndexedPartyEntryRecord(actor->partyIndex);
    u32 maxHp;
    u32 maxMp;

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
    dst->level = src->level;
    maxHp = datComputeSkillBoostedMaxHp(dst);
    maxMp = datComputeSkillBoostedMaxMp(dst);
    /* Keep the calculator's full-width results; retail compares them signed. */
    dst->hp = src->hp < (s32)maxHp ? src->hp : (s32)maxHp;
    dst->mp = src->mp < (s32)maxMp ? src->mp : (s32)maxMp;
    memcpy(dst->baseStats, src->baseStats, sizeof(dst->baseStats));
    dst->status = src->status & 0x7FFF;
    dst->savedCommandPhase = src->savedCommandPhase;
    dst->savedCommandSelectionEntry = src->savedCommandSelectionEntry;
    dst->actionSlot = src->actionSlot;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

PtyActiveRosterIndex btlFindPartyEntryIndexForActor(BtlUnit *unit) {
    return dds3FindEntryIndex(unit->partyRecord.unitId);
}

/* Retail no-op after party actor insertion, removal or roster swap. */
void btlOnPartyRosterChange(void) {
}

BtlUnit *btlFindActiveActorByPartyIndex(PtyActiveRosterIndex partyIndex) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    for (; node != 0; node = node->next) {
        u32 flags = node->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if (partyIndex == node->partyIndex) {
                    return node;
                }
            }
        }
    }
    return 0;
}

extern void btlCopyUnitStats(BtlUnit *unit, datUnitWork_t *source);

/* Insert a complete party record before actors with a larger priority. */
void btlInsertPartyActorByPriority(BtlUnit *unit, u8 sourceIndex, u8 priority) {
    datUnitWork_t saved;
    s32 insertion = 0;
    s32 i;

    if (DDS3_GBWK->party[0].flags & 2) {
        do {
            if ((u16)(DDS3_GBWK->party[insertion].flags & 1) == 0) {
                break;
            }
            if (priority < btlFindActiveActorByPartyIndex(insertion)->battleSideSlotIndex) {
                break;
            }
            insertion++;
            if (insertion >= 5) {
                break;
            }
        } while (DDS3_GBWK->party[insertion].flags & 2);
    }
    memcpy(&saved, &DDS3_GBWK->party[sourceIndex], sizeof(saved));
    for (i = sourceIndex; i < 4; i++) {
        memcpy(&DDS3_GBWK->party[i], &DDS3_GBWK->party[i + 1], sizeof(saved));
        if (DDS3_GBWK->party[i + 1].flags & 2) {
            btlFindActiveActorByPartyIndex(i + 1)->partyIndex = i;
        }
    }
    for (i = 4; i > insertion; i--) {
        memcpy(&DDS3_GBWK->party[i], &DDS3_GBWK->party[i - 1], sizeof(saved));
        if (DDS3_GBWK->party[i - 1].flags & 2) {
            btlFindActiveActorByPartyIndex(i - 1)->partyIndex = i;
        }
    }
    memcpy(&DDS3_GBWK->party[i], &saved, sizeof(saved));
    btlCopyUnitStats(unit, &saved);
    unit->partyRecord.flags |= 2;
    DDS3_GBWK->party[i].flags |= 2;
    unit->partyIndex = i;
    btlOnPartyRosterChange();
    btlBossDebugPrintf("btl:party in %d->%d[%d]\n", sourceIndex, i, saved.unitId);
}

/* Move a departing actor behind the remaining occupied party records. */
void btlRemovePartyActorAndShiftEntries(BtlUnit *unit) {
    datUnitWork_t saved;
    DatGameState *scanState = DDS3_GBWK;
    s32 originalIndex;
    s32 index;

    originalIndex = unit->partyIndex;
    memcpy(&saved, &DDS3_GBWK->party[originalIndex], sizeof(saved));
    index = originalIndex;
    if (index < 4 && (u16)(scanState->party[index + 1].flags & 1)) {
        do {
            memcpy(&DDS3_GBWK->party[index], &DDS3_GBWK->party[index + 1], sizeof(saved));
            if (DDS3_GBWK->party[index + 1].flags & 2) {
                btlFindActiveActorByPartyIndex(index + 1)->partyIndex = index;
            }
            index++;
            if (index >= 4) {
                break;
            }
            scanState = DDS3_GBWK;
        } while ((u16)(scanState->party[index + 1].flags & 1));
    }
    memcpy(&DDS3_GBWK->party[index], &saved, sizeof(saved));
    unit->partyRecord.flags &= ~2;
    DDS3_GBWK->party[index].flags &= ~2;
    unit->partyIndex = BTL_UNIT_PARTY_INDEX_NO_ACTIVE_SLOT;
    btlOnPartyRosterChange();
    btlBossDebugPrintf("btl:party out %d->%d[%d]\n", originalIndex, index,
                       saved.unitId);
}

/* Swap complete records while keeping the actor in its original party slot. */
void btlSynchronizePartyActorRecords(BtlUnit *actor, u8 targetIndex) {
    datUnitWork_t previous;

    if (actor->partyIndex != targetIndex) {
        memcpy(&previous, &DDS3_GBWK->party[actor->partyIndex], sizeof(previous));
        memcpy(&DDS3_GBWK->party[actor->partyIndex], &DDS3_GBWK->party[targetIndex], sizeof(previous));
        memcpy(&DDS3_GBWK->party[targetIndex], &previous, sizeof(previous));
        btlCopyUnitStats(actor, &DDS3_GBWK->party[actor->partyIndex]);
        actor->partyRecord.flags |= 2;
        DDS3_GBWK->party[actor->partyIndex].flags |= 2;
        DDS3_GBWK->party[targetIndex].flags &= ~2;
        btlOnPartyRosterChange();
        btlBossDebugPrintf("btl:party change %d<->%d[%d<->%d]\n",
            actor->partyIndex, targetIndex, previous.unitId, actor->partyRecord.unitId);
    }
}

s32 btlGetEntryFlagsUnlessDisabled(datUnitWork_t *record) {
    if ((record->flags & 4) != 0) {
        return 0;
    }
    return datEnemyRecords[record->unitId].flags;
}

/* Native tail jump forwards the clamped profile-adjusted stat in v0;
 * retain the existing void C surface for matching. */
void btlQueryClampedProfileAdjustedStat(datUnitWork_t *unit, s32 statIndex) {
    datGetClampedProfileAdjustedStat(unit, statIndex);
}

/* Return the requested party-record stat with its native status override. */
s32 btlGetStatWithStatusOverride(datUnitWork_t *unit, s32 statIndex) {
    return datGetStatWithStatusOverride(unit, statIndex);
}

BtlCommandVitalCostAmount btlApplyCommandAbilityMultiplier(datUnitWork_t *battler, BtlCommandId commandId) {
    u32 value = datCalculateCommandBaseValue(battler, commandId);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (datCommandRecords[commandId].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if (btlCheckSpecialAbility(battler, 0x234)) {
            scale = datAbilityParameters[0x234 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if (btlCheckSpecialAbility(battler, 0x235)) {
            scale = datAbilityParameters[0x235 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    value = (s32)((f32)value * scale);
    return value == 0 ? 1 : value;
}

/* Return the LINKAGE command's signed native affinity-record slot cost. */
s8 btlGetLinkageSlotCost(BtlCommandId commandId) {
    return datAffinityRecords[commandId - DAT_AFFINITY_FIRST_COMMAND].slotCost;
}

BtlCommandUseError btlGetCommandFailureReason(BtlUnit *unit, BtlCommandId commandId) {
    BtlCommandUseError result = BTL_COMMAND_USE_ERROR_NONE;
    u32 cost;

    if (commandId == 0) {
        return BTL_COMMAND_USE_ERROR_NONE;
    }
    if (datCommandRecords[commandId].effectType == 1 ||
        datCommandRecords[commandId].costMode == DAT_COMMAND_COST_MODE_MP) {
        if ((unit->partyRecord.status & 0x7FFF) == 0x10) {
            return BTL_COMMAND_USE_ERROR_ACTOR_STATE;
        }
    }
    if (datCommandSelectors[commandId].kind == 2 &&
        (unit->partyRecord.status & 0x7FFF) == 0x40) {
        return BTL_COMMAND_USE_ERROR_SELECTOR_STATE;
    }
    if (datCommandSelectors[commandId].kind == 1) {
        if ((unit->partyRecord.status & 0x7FFF) == 0x1000) {
            return BTL_COMMAND_USE_ERROR_LINKAGE_STATE;
        }
        if (fldCountSceneSlots() < (u32)btlGetLinkageSlotCost(commandId)) {
            return BTL_COMMAND_USE_ERROR_INSUFFICIENT_SCENE_SLOTS;
        }
    }
    if (datCommandRecords[commandId].flags & 4) {
        return BTL_COMMAND_USE_ERROR_NONE;
    }

    cost = btlApplyCommandAbilityMultiplier(&unit->partyRecord, commandId);
    switch (datCommandRecords[commandId].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if ((datCommandRecords[commandId].flags & 8) == 0) {
            if (cost >= unit->partyRecord.hp) {
                result = BTL_COMMAND_USE_ERROR_INSUFFICIENT_HP;
            }
        } else if (unit->partyRecord.hp < cost) {
            result = BTL_COMMAND_USE_ERROR_INSUFFICIENT_HP;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        result = unit->partyRecord.mp < cost ? BTL_COMMAND_USE_ERROR_INSUFFICIENT_MP : BTL_COMMAND_USE_ERROR_NONE;
        break;
    }
    return result;
}

BtlCommandVitalCostAmount btlGetCombinedPartyCommandPower(datUnitWork_t *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, BtlCommandId commandId) {
    datUnitWork_t snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;
    s32 average;

    if (first != NULL) {
        totalMaxHp = first->partyRecord.maxHp;
        count = 1;
    }
    if (second != NULL) {
        totalMaxHp += second->partyRecord.maxHp;
        count++;
    }
    if (third != NULL) {
        totalMaxHp += third->partyRecord.maxHp;
        count++;
    }
    average = totalMaxHp / count;
    snapshot.maxHp = average;
    snapshot.hp = average;
    return btlApplyCommandAbilityMultiplier(&snapshot, commandId);
}

BtlCommandUseError btlGetLinkedPartyCommandFailureReason(BtlUnit *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, BtlCommandId commandId) {
    BtlUnit snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;

    if (first != NULL) {
        if ((first->status.flags & 0x100) == 0) {
            return BTL_COMMAND_USE_ERROR_ACTOR_STATE;
        }
        if ((first->partyRecord.status & 0x2A0E) != 0) {
            return BTL_COMMAND_USE_ERROR_ACTOR_STATE;
        }
        totalMaxHp = first->partyRecord.maxHp;
        count = 1;
    }
    if (second != NULL) {
        if ((second->status.flags & 0x100) == 0) {
            return BTL_COMMAND_USE_ERROR_ACTOR_STATE;
        }
        if ((second->partyRecord.status & 0x2A0E) != 0) {
            return BTL_COMMAND_USE_ERROR_ACTOR_STATE;
        }
        count++;
        totalMaxHp += second->partyRecord.maxHp;
    }
    if (third != NULL) {
        if ((third->status.flags & 0x100) == 0) {
            return BTL_COMMAND_USE_ERROR_ACTOR_STATE;
        }
        if ((third->partyRecord.status & 0x2A0E) != 0) {
            return BTL_COMMAND_USE_ERROR_ACTOR_STATE;
        }
        count++;
        totalMaxHp += third->partyRecord.maxHp;
    }
    snapshot.partyRecord.maxHp = totalMaxHp / count;
    return btlGetCommandFailureReason(&snapshot, commandId);
}

s8 btlGetActorIndexedSignedValue(const BtlUnit *actor, BtlCommandId commandId) {
    if (commandId == 0 && (actor->status.flags & 0x400) != 0) {
        return datEnemyRecords[actor->partyRecord.unitId].indexedValueOverride;
    }
    return datCommandSelectors[commandId].stat;
}

DatEffectiveAffinityResult btlGetUnitElementAffinity(
    BtlUnit *battler, DatAffinitySelector selector) {
    return btlResolveUnitValueWithOverride(&battler->partyRecord, selector);
}

DatEffectiveAffinityResult btlResolveUnitValueWithOverride(
    datUnitWork_t *record, DatAffinitySelector selector) {
    BtlUnitValueOverrideHook handler = ((BtlState *)btlGetRuntime())->unitValueOverrideHook;
    if (handler != 0) {
        DatEffectiveAffinityResult result = handler(record, selector);
        if (result != -1) {
            return result;
        }
    }
    return datGetEffectiveAffinity(record, selector);
}

void *btlGetActorResourceBankEntry(
    BtlActorResourceTableKind resourceTableKind, s32 index) {
    if (resourceTableKind == BTL_ACTOR_RESOURCE_TABLE_PARTY) {
        return (void *)(u32)(D_003BAA10 + index * 0x270);
    }
    return (void *)(u32)(D_003BAA20 + index * 0x270);
}

BtlActorTransformParameters *btlSelectSharedOrIndexedTransformParameters(
    BtlActorResourceTableKind resourceTableKind, s32 index) {
    if (resourceTableKind == BTL_ACTOR_RESOURCE_TABLE_PARTY) {
        return (BtlActorTransformParameters *)btlSharedActorTransformParameters;
    }
    return (BtlActorTransformParameters *)(D_003BAA30 + index * 24);
}

const SoundSlotTableEntry *btlSelectSideIndexedActorParameterTable(s32 category, s32 actorId) {
    if (category == 0) {
        return (const SoundSlotTableEntry *)(D_003BAA14 + actorId * 0x74);
    }
    return (const SoundSlotTableEntry *)(D_003BAA28 + actorId * 0x74);
}

extern char D_003A1788[];

s32 btlGetLoggedIndexedCommandItem(s32 index) {
    u16 item = datItemSkillRecords[index].commandIndex;
    btlBossDebugPrintf(D_003A1788, index, item);
    return item;
}

s32 btlGetActorBedAssetIdFromIndex(s32 itemIndex) {
    return datItemSkillRecords[itemIndex].commandIndex;
}

extern const char D_003A17A8[];

BtlTargetListBuildResult btlBuildEligibleTargetIndexList(
    BtlTask *action, BtlIndexList *targets, BtlTargetListArea area,
    BtlTargetListRule rule, BtlTargetStatusExclusion excludedStatus) {
    BtlUnit *collected[16];
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 alliedArea;
    s32 opposingArea;
    s32 areaMask;
    BtlUnit *unit;
    s32 count;
    s32 selected;
    s32 i;

    if ((action->unit->status.flags & 0x200) != 0) {
        alliedArea = 0x200;
        opposingArea = 0x400;
    } else {
        alliedArea = 0x400;
        opposingArea = 0x200;
    }
    areaMask = BTL_TARGET_LIST_AREA_NONE;
    if ((area & BTL_TARGET_LIST_AREA_ALLIES) != 0) {
        areaMask = alliedArea;
    }
    if ((area & BTL_TARGET_LIST_AREA_OPPOSING_SIDE) != 0) {
        areaMask |= opposingArea;
    }
    btlClearIndexList(targets);
    if ((rule & BTL_TARGET_LIST_RULE_SELF_ONLY) != 0) {
        btlAppendIndexListEntry(targets, action->unit);
        return BTL_TARGET_LIST_BUILD_NONEMPTY;
    }
    unit = battle->units;
    count = 0;
    selected = -1;
    for (; unit != NULL; unit = unit->next) {
        s32 flags = unit->status.flags;

        if ((flags & 0x100) == 0) {
            continue;
        }
        if ((flags & 0xE0) != 0) {
            if (excludedStatus == BTL_TARGET_STATUS_EXCLUSION_NO_MASK) {
                continue;
            }
            if ((excludedStatus & 0x4000) != 0 &&
                excludedStatus != BTL_TARGET_STATUS_EXCLUSION_ANY) {
                continue;
            }
        }
        if ((flags & areaMask) == 0) {
            continue;
        }
        if ((rule & BTL_TARGET_LIST_RULE_LIVING_NON_ACTOR) != 0 &&
            (unit->status.flags & 1) != 0) {
            if (action->unit == unit) {
                continue;
            }
            collected[count++] = unit;
        } else if ((rule & BTL_TARGET_LIST_RULE_DEFEATED_ONLY) != 0) {
            if ((unit->status.flags & 1) != 0) {
                continue;
            }
            collected[count] = unit;
            selected = count++;
        } else {
            if ((rule & BTL_TARGET_LIST_RULE_INCLUDE_DEFEATED) == 0) {
                if ((unit->status.flags & 1) == 0) {
                    continue;
                }
            } else {
                if ((unit->status.flags & 1) == 0) {
                    selected = count;
                }
            }
            if ((unit->status.flags & 1) != 0) {
                if (excludedStatus != BTL_TARGET_STATUS_EXCLUSION_NO_MASK) {
                    if (excludedStatus != BTL_TARGET_STATUS_EXCLUSION_ANY) {
                        if (unit->partyRecord.status == 0) {
                            continue;
                        }
                        if ((excludedStatus & unit->partyRecord.status) != 0) {
                            continue;
                        }
                    }
                }
            }
            collected[count++] = unit;
        }
    }
    if (count > 0) {
        if (selected < 0) {
            selected = count - 1;
        }
        for (i = 0; i < count; i++) {
            s32 index = selected - i;

            if (index < 0) {
                index += count;
            } else {
                index %= count;
            }
            btlAppendIndexListEntry(targets, collected[index]);
        }
    }
    btlBossDebugPrintf(D_003A17A8, count, area, rule, excludedStatus);
    return count != 0 ? BTL_TARGET_LIST_BUILD_NONEMPTY : BTL_TARGET_LIST_BUILD_EMPTY;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1788);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A17A8);

BtlCommandTargetPolicy btlSelectCommandTargetPolicy(BtlTask *task, BtlIndexList *destination, s32 unused) {
    s32 category = 3;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlTargetListArea area = BTL_TARGET_LIST_AREA_NONE;
    BtlTargetListRule targetRules = BTL_TARGET_LIST_RULE_NONE;
    BtlTargetStatusExclusion excludedStatus = BTL_TARGET_STATUS_EXCLUSION_NO_MASK;
    s32 value = task->indexWork.phase;

    switch (value) {
    case 1: {
        BtlUnit *actor = task->unit;
        if (btlUnitStatusPair(actor) & 0x1400) {
            if (btlCheckSpecialAbility(&actor->partyRecord, 0x21E) != 0) {
                category = 1;
                task->unit->pendingSkillNameIndex = 0x21E;
            } else {
                category = 0;
            }
            area = BTL_TARGET_LIST_AREA_OPPOSING_SIDE;
            targetRules = BTL_TARGET_LIST_RULE_NONE;
            excludedStatus = BTL_TARGET_STATUS_EXCLUSION_NO_MASK;
        } else {
            value = actor->partyRecord.unitId;
            category = datRosterDetails[value].targetType;
            area = datRosterDetails[value].options;
            targetRules = datRosterDetails[value].targetFlags;
            excludedStatus = datRosterDetails[value].restriction;
        }
        break;
    }
    case 2:
    case 3:
    case 4:
    case 7:
    case 8: {
        if (value == 4) {
            value = btlGetLoggedIndexedCommandItem(task->indexWork.reference);
        } else {
            value = task->indexWork.skillId;
        }
        category = datCommandRecords[value].targetType;
        area = datCommandRecords[value].options;
        targetRules = datCommandRecords[value].targetFlags;
        excludedStatus = datCommandRecords[value].restriction;
        if ((datCommandRecords[value].attribute.bits & 0x400000FF) == 0x40000002 &&
            excludedStatus == BTL_TARGET_STATUS_EXCLUSION_NO_MASK) {
            excludedStatus = BTL_TARGET_STATUS_EXCLUSION_ANY;
        }
        break;
    }
    case 9:
        category = 0;
        area = BTL_TARGET_LIST_AREA_ALLIES;
        targetRules = BTL_TARGET_LIST_RULE_INCLUDE_DEFEATED;
        excludedStatus = BTL_TARGET_STATUS_EXCLUSION_ANY;
        break;
    }
    if (battle->runtimeFlags & BTL_STATE_FLAG_BYPASS_COMMAND_BLOCKS) {
        area = BTL_TARGET_LIST_AREA_ALLIES | BTL_TARGET_LIST_AREA_OPPOSING_SIDE;
        targetRules &= ~(BTL_TARGET_LIST_RULE_SELF_ONLY |
                         BTL_TARGET_LIST_RULE_LIVING_NON_ACTOR |
                         BTL_TARGET_LIST_RULE_DEFEATED_ONLY);
        excludedStatus = BTL_TARGET_STATUS_EXCLUSION_NO_MASK;
    }
    if (destination != NULL) {
        btlBuildEligibleTargetIndexList(task, destination, area, targetRules, excludedStatus);
    }
    return category;
}

BtlCommandStatusTargetListIndex btlFindCommandStatusMatchInTargetList(
    BtlTask *actionTask, BtlIndexList *targets) {
    BtlTask *action = actionTask;
    u32 count;
    u32 i;
    s32 command;
    s32 index;

    if (action == NULL || targets == NULL) {
        return 0;
    }
    count = btlGetIndexListCount(targets);
    if (count < 2) {
        return 0;
    }
    command = action->indexWork.phase;
    switch (command) {
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
        if (command == 4) {
            index = btlGetLoggedIndexedCommandItem(action->indexWork.reference);
        } else {
            index = action->indexWork.skillId;
        }
        if (datCommandRecords[index].targetType == 0 &&
            datCommandRecords[index].attribute.parts.kind == DAT_COMMAND_ATTRIBUTE_KIND_FLAG_MASK &&
            datCommandRecords[index].attribute.parts.valueMask != 0) {
            for (i = 0; i < count; i++) {
                if ((datCommandRecords[index].attribute.parts.valueMask &
                     ((BtlUnit *)btlGetIndexListEntry(targets, i))->partyRecord.status) != 0) {
                    return i;
                }
            }
        }
        break;
    }
    return 0;
}

s32 btlMarkInactiveActorCandidates(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *eligible[16];
    BtlUnit *unit;
    BtlUnit **read;
    s32 count;
    s32 remaining;
    s32 lowestSlotIndex;

    unit = battle->units;
    count = 0;
    if (unit != NULL) {
        do {
            s32 flags = unit->status.flags;
            if ((flags & 0x200) != 0) {
                if ((flags & 1) == 0) {
                    eligible[count] = unit;
                    unit->status.flags = flags & ~0x100;
                    count++;
                }
            }
            unit = unit->next;
        } while (unit != NULL);
    }
    if (count == 0) {
        return 0;
    }
    if (battle->cameraPresetMode == 3 && count == 2) {
        eligible[0]->status.flags |= 0x100;
        eligible[1]->status.flags |= 0x100;
    } else {
        lowestSlotIndex = 4;
        unit = NULL;
        if (count > 0) {
            remaining = count;
            read = eligible;
            do {
                BtlUnit *candidate = *read++;
                s32 slotIndex = candidate->battleSideSlotIndex;
                if (slotIndex < lowestSlotIndex) {
                    unit = candidate;
                    lowestSlotIndex = slotIndex;
                }
            } while (--remaining != 0);
        }
        unit->status.flags |= 0x100;
    }
    return 1;
}

#include "btl_roster_pair.h"

extern u8 btlLinkageParticipantOrders[];
extern DatSkillList *D_003BAA58;

/* Build distinct LINKAGE skill/partner rows for actor; return the roster count. */
s32 btlBuildActorLinkageSkillList(BtlUnit *actor, BattleRosterTable *table) {
    BtlUnit *actors[3];
    BtlState *battle;
    BtlUnit *unit;
    const DatAffinityRecord *record;
    s32 actorCount;
    u32 count;
    u32 affinity;
    u32 order;
    s32 command;
    u32 participant;
    u32 skillIndex;
    u32 previous;
    s32 allRequired;
    s32 requirement;
    s32 value;
    u32 matched;
    s32 duplicate;

    battle = (BtlState *)btlGetRuntime();
    table->count = 0;
    actors[0] = actor;
    actors[1] = NULL;
    actors[2] = NULL;
    actorCount = 1;
    for (unit = battle->units; unit != NULL; unit = unit->next) {
        if ((unit->status.flags & 0x200) == 0) {
            continue;
        }
        if (unit == actor) {
            continue;
        }
        if ((unit->status.flags & 1) == 0) {
            continue;
        }
        if ((unit->status.flags & 0xC0) != 0) {
            continue;
        }
        if ((unit->status.flags & 0x20) != 0) {
            continue;
        }
        actors[actorCount++] = unit;
    }
    if (actorCount == 1) {
        return 0;
    }

    count = 0;
    record = datAffinityRecords;
    for (command = DAT_AFFINITY_FIRST_COMMAND, affinity = 0;
         affinity < 85; affinity++, command++, record++) {
        allRequired = 1;
        for (order = 0; order < 6; order++) {
            matched = 0;
            for (participant = 0; participant < 3; participant++) {
                requirement = record->requirements[btlLinkageParticipantOrders[participant + order * 3]];
                if ((battle->battleFlags & 0x1000000) == 0) {
                    if (requirement == -1) {
                        allRequired = 0;
                        continue;
                    }
                } else {
                    if (requirement == -1) {
                        allRequired = 0;
                    } else {
                        matched |= 1 << participant;
                    }
                    continue;
                }
                if (actors[participant] == NULL) {
                    continue;
                }
                if ((record->flags & 1) != 0 &&
                    (actors[participant]->status.flags & 0x200) == 0) {
                    continue;
                }
                if ((record->flags & 2) != 0 &&
                    (actors[participant]->status.flags & 0x400) == 0 &&
                    actors[participant]->partyRecord.unitId < 0x100) {
                    continue;
                }
                switch (requirement & 0xF0000000) {
                case 0:
                    if ((actors[participant]->status.flags & 0x1000) == 0) {
                        continue;
                    }
                    if (requirement == 0 ||
                        btlCheckSpecialAbility(&actors[participant]->partyRecord, requirement) != 0) {
                        matched |= 1 << participant;
                    }
                    break;
                case 0x10000000:
                    if ((actors[participant]->status.flags & 0x1000) == 0) {
                        continue;
                    }
                    value = requirement & 0x0FFFFFFF;
                    for (skillIndex = 0; skillIndex < 24; skillIndex++) {
                        if (actors[participant]->partyRecord.effectData[skillIndex] != 0 &&
                            (1 << datCommandSelectors[actors[participant]->partyRecord.effectData[skillIndex]].stat) == value) {
                            matched |= 1 << participant;
                            break;
                        }
                    }
                    continue;
                case 0x20000000:
                    if ((actors[participant]->status.flags & 0x1000) != 0) {
                        continue;
                    }
                    value = requirement & 0x0FFFFFFF;
                    if (value != 0 && value != (1 << actors[participant]->partyRecord.unitId)) {
                        continue;
                    }
                    matched |= 1 << participant;
                    break;
                case 0x40000000:
                    if ((actors[participant]->status.flags & 0x1000) == 0) {
                        continue;
                    }
                    value = requirement & 0x0FFFFFFF;
                    for (skillIndex = 0; skillIndex < 16; skillIndex++) {
                        s16 skill = D_003BAA58[value].skills[skillIndex];
                        if (skill >= 0 && btlCheckSpecialAbility(&actors[participant]->partyRecord, skill)) {
                            matched |= 1 << participant;
                            break;
                        }
                    }
                    continue;
                default:
                    continue;
                }
            }
            if ((matched & 1) != 0 && (matched & 6) != 0) {
                duplicate = 0;
                if (matched == 7) {
                    table->entries[count].skill = command;
                    table->entries[count].first = actors[1];
                    table->entries[count].second = actors[2];
                } else if (!allRequired) {
                    if (matched == 3) {
                        table->entries[count].skill = command;
                        table->entries[count].first = actors[1];
                        table->entries[count].second = NULL;
                    } else if (matched == 5) {
                        table->entries[count].skill = command;
                        table->entries[count].first = actors[2];
                        table->entries[count].second = NULL;
                    }
                } else {
                    duplicate = 1;
                }
                if (!duplicate) {
                    for (previous = 0; previous < count; previous++) {
                        if (table->entries[count].skill == table->entries[previous].skill &&
                            ((table->entries[count].first == table->entries[previous].first &&
                              table->entries[count].second == table->entries[previous].second) ||
                             (table->entries[count].first == table->entries[previous].second &&
                              table->entries[count].second == table->entries[previous].first))) {
                            duplicate = 1;
                            break;
                        }
                    }
                }
                if (!duplicate) {
                    count++;
                }
            }
        }
    }
    table->count = count;
    return count;
}


extern BtlTask *fldGetSceneGroupEntry(s32 entryIndex);
extern s32 btlFindCommandPartnersByAffinity(
    BtlUnit *unit, BtlCommandId commandId, BtlUnit **first, BtlUnit **second);

BtlCommandAffinityPartnerResolution btlResolveCommandAffinityPartners(
    BtlUnit *unit, BtlCommandId commandId, BtlCommandPartnerOutput *output) {
    BtlState *controller = (BtlState *)btlGetRuntime();
    const s32 *requirementCursor;
    u32 requiredCount;
    u32 i;
    BtlCommandAffinityPartnerResolution result;

    if (datCommandSelectors[commandId].kind != 1) {
        return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
    }
    if ((u32)(commandId - DAT_AFFINITY_FIRST_COMMAND) >= 0x55) {
        return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
    }
    requiredCount = 0;
    requirementCursor = datAffinityRecords[commandId - DAT_AFFINITY_FIRST_COMMAND].requirements;
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount == 0) {
        return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
    }
    if ((datAffinityRecords[commandId - DAT_AFFINITY_FIRST_COMMAND].flags & 2) == 0 &&
        (datBattleSceneRecords[controller->battleMode].flags & 1) == 0) {
        output->commandId = commandId;
        result = btlFindCommandPartnersByAffinity(unit, commandId, &output->firstPartner, &output->secondPartner);
    } else {
        output->commandId = commandId;
        output->firstPartner = NULL;
        output->secondPartner = NULL;
        if (requiredCount == 3) {
            for (i = 1; i < requiredCount; i++) {
                BtlTask *entry = fldGetSceneGroupEntry(i);
                BtlUnit *partner;

                if (entry == NULL) {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
                partner = entry->unit;
                if ((partner->status.flags & 0x400) == 0) {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
                if ((partner->status.flags & 0xE0) != 0) {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
                if ((partner->partyRecord.status & 0x2A0E) != 0) {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
                switch (i) {
                case 1:
                    output->firstPartner = partner;
                    break;
                case 2:
                    output->secondPartner = partner;
                    break;
                }
            }
            return BTL_COMMAND_AFFINITY_PARTNERS_RESOLVED;
        }
        if (requiredCount == 2) {
            BtlTask *entry = fldGetSceneGroupEntry(2);

            if (entry != NULL && (entry->unit->partyRecord.status & 0x2A0E) == 0 &&
                (btlUnitStatusPair(entry->unit) & 0x4E0) == 0x400) {
                output->firstPartner = entry->unit;
            } else {
                BtlUnit *partner;

                entry = fldGetSceneGroupEntry(1);
                if (entry == NULL) {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
                partner = entry->unit;
                if ((partner->status.flags & 0x400) == 0) {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
                if ((partner->status.flags & 0xE0) != 0) {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
                if ((partner->partyRecord.status & 0x2A0E) == 0) {
                    output->firstPartner = partner;
                } else {
                    return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
                }
            }
        } else {
            return BTL_COMMAND_AFFINITY_PARTNERS_UNRESOLVED;
        }
        result = BTL_COMMAND_AFFINITY_PARTNERS_RESOLVED;
    }
    return result;
}

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

/* Map a battle channel flag to the shared affinity selector. */
DatAffinitySelector btlFlagToElementIndex(s32 channelFlag) {
    return datFlagToElementIndex(channelFlag);
}

extern s32 evtGetMirroredSolarPhase(void);

BtlSpecialAbilityAvailability btlCheckSpecialAbility(
    const datUnitWork_t *object, DatSkillId skillId) {
    if (datUnitHasSkill(object, skillId) == DAT_UNIT_SKILL_RESULT_ABSENT) {
        return BTL_SPECIAL_ABILITY_UNAVAILABLE;
    }
    switch (skillId) {
    case 0x207:
        return evtGetMirroredSolarPhase() == 8 ? BTL_SPECIAL_ABILITY_AVAILABLE
                                               : BTL_SPECIAL_ABILITY_UNAVAILABLE;
    case 0x208:
        return evtGetMirroredSolarPhase() == 0 ? BTL_SPECIAL_ABILITY_AVAILABLE
                                               : BTL_SPECIAL_ABILITY_UNAVAILABLE;
    default:
        return BTL_SPECIAL_ABILITY_AVAILABLE;
    }
}

extern EffRandState effSharedRandomState[];

extern BtlEnemySelectionResult btlSelectEnemyDropItem(
    BtlUnit *actor, BtlEnemySelectionContext selectionContext);

s32 btlSelectActorAction(BtlUnit *actor) {
    s32 result = btlSelectEnemyDropItem(
        actor, BTL_ENEMY_SELECTION_CONTEXT_ACTOR_ACTION);
    if (result == BTL_ENEMY_SELECTION_NONE) {
        result = (dds3GetRandA((EffRandState *)effSharedRandomState) & 1) != 0 ? 2 : 7;
    }
    return result;
}

extern s32 btlRollAiBucket(void);

/* Select a record byte; its context determines whether it is an item or action. */
BtlEnemySelectionResult btlSelectEnemyDropItem(
    BtlUnit *actor, BtlEnemySelectionContext selectionContext) {
    DatEnemyRecord *enemy = &datEnemyRecords[actor->partyRecord.unitId];
    BtlEnemySelectionResult item = BTL_ENEMY_SELECTION_NONE;
    u16 i;


    if (selectionContext != BTL_ENEMY_SELECTION_CONTEXT_ACTOR_ACTION &&
        enemy->overrideAction != 0 && enemy->overrideFlag != 0 &&
        evtBitCheck(enemy->overrideFlag) != 0) {
        if ((u8)btlRollAiBucket() < enemy->overrideChance) {
            item = enemy->overrideAction;
        }
    }
    for (i = 0; i < 2 && item == BTL_ENEMY_SELECTION_NONE; i++) {
        if (enemy->additionalDropItemIds[i] != 0 && (u8)btlRollAiBucket() < enemy->actionChances[i]) {
            item = enemy->additionalDropItemIds[i];
        }
    }
    return item;
}

/* Accumulate one item drop; new IDs are discarded if all three slots are occupied. */
void btlAddItemDrop(u16 item) {
    BtlState *controller = (BtlState *)btlGetRuntime();

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

extern s32 btlAverageLevelsForMaskExcludingStatus20(u32);

extern s32 btlCalculateEnemyExperienceReward(BtlUnit *, BtlUnit *);

extern s32 btlGetEnemyMoney(BtlUnit *, BtlUnit *);

extern char D_003A17F8[];

extern char D_003A1818[];

extern char D_003A1830[];

extern char D_003A1848[];

void btlAccumulateEnemyDefeatRewards(BtlUnit *enemy) {
    BtlState *controller = (BtlState *)btlGetRuntime();
    DatEnemyRecord *record = &datEnemyRecords[enemy->partyRecord.unitId];
    s32 level = btlAverageLevelsForMaskExcludingStatus20(4);
    s32 enemyLevel = record->level;
    s32 allowance = datBattleParameters->rewardLevelAllowance;
    s32 reward;
    s32 amount;
    s32 item;

    if (level > 60) {
        level = 60;
    }
    if (level >= enemyLevel + allowance && datBattleParameters->rewardDivisor != 0.0f) {
        reward = (s32)(record->battleExperienceReward / datBattleParameters->rewardDivisor);
        btlBossDebugPrintf(D_003A17F8, reward, level, enemyLevel, allowance,
                          datBattleParameters->rewardDivisor);
    } else {
        reward = record->battleExperienceReward;
    }
    if (record->flags & 0x2000) {
        reward *= 100;
    }
    controller->experienceEarned += reward;
    if ((btlUnitStatusPair(enemy) & 0x200800000ULL) == 0) {
        amount = btlCalculateEnemyExperienceReward(NULL, enemy);
        controller->epEarned += amount;
        btlBossDebugPrintf(D_003A1818, controller->epEarned, amount);
    }
    if ((enemy->status.stateFlags & 0x400) == 0) {
        amount = btlGetEnemyMoney(NULL, enemy);
        controller->moneyEarned += amount;
        btlBossDebugPrintf(D_003A1830, controller->moneyEarned, amount);
    }
    item = btlSelectEnemyDropItem(
        enemy, BTL_ENEMY_SELECTION_CONTEXT_ITEM_DROP);
    if (item != BTL_ENEMY_SELECTION_NONE) {
        btlAddItemDrop(item);
    }
    enemy->status.stateFlags |= 1;
    btlBossDebugPrintf(D_003A1848, enemy);
}

BtlActiveUnitsReadiness btlAllActiveUnitsReady(void) {
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    for (; actor != NULL; actor = actor->next) {
        u32 flags = actor->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 0xC0) != 0) {
                    return BTL_ACTIVE_UNITS_NOT_READY;
                }
                if ((flags & 0x20) != 0) {
                    if ((actor->status.stateFlags & 1) == 0) {
                        return BTL_ACTIVE_UNITS_NOT_READY;
                    }
                }
            }
        }
    }
    return BTL_ACTIVE_UNITS_READY;
}

/* Return the battle-parameter scale for the selected encounter's clamped adjustment. */
f32 btlGetEncounterAdjustmentScale(void) {
    BtlState *runtime = (BtlState *)btlGetRuntime();
    s32 adjustment = datBattleAdjustmentRecords[runtime->encounterPack]
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

/* Read the counter incremented by the escape-attempt result callback. */
u32 btlGetEscapeAttemptCount(void) {
    BtlState *battleWork;

    battleWork = btlGetRuntime();
    return battleWork->escapeAttemptCount;
}

/* Return a random eligible actor task, or 0 if no candidate is available. */
BtlTask *btlChooseAvailableUnit(void) {
    BtlTask *candidates[16];
    s32 count = 0;
    BtlTask *node = ((BtlState *)btlGetRuntime())->tasks;
    for (; node != 0; node = node->next) {
        if ((node->flags & 8) != 0) {
            BtlUnit *actor = node->unit;
            u32 flags = actor->status.flags;
            if ((flags & 1) != 0) {
                if ((flags & 0x200) != 0) {
                    if ((flags & 2) != 0) {
                        if ((flags & 0xE0) == 0) {
                            candidates[count++] = node;
                        }
                    }
                }
            }
        }
    }
    if (count != 0) {
        return candidates[dds3GetRandIntA(0, count)];
    }
    return 0;
}

s32 btlCountAvailableUnits(void) {
    BtlUnit *node = ((BtlState *)btlGetRuntime())->units;
    s32 count = 0;
    for (; node != 0; node = node->next) {
        u32 flags = node->status.flags;
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
    BtlUnit *node = ((BtlState *)btlGetRuntime())->units;
    for (; node != 0; node = node->next) {
        u32 flags = node->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    count++;
                }
            }
        }
    }
    {
        datUnitWork_t *entry = DDS3_GBWK->party;
        s32 i;
        for (i = 4; i >= 0; i--, entry++) {
            u16 flags = entry->flags;
            if ((flags & 1) != 0) {
                if ((flags & 2) == 0) {
                    if ((entry->status & 0x4000) == 0) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

f32 btlGetActorEffectScale(BtlTask *task) {
    BtlState *battle;
    BtlUnit *unit;

    if (task == NULL) {
        return 1.0f;
    }
    battle = (BtlState *)btlGetRuntime();
    if ((battle->battleFlags & 0x8000) != 0) {
        unit = task->unit;
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200) {
            return 1.0f;
        }
        return 3.0f;
    }
    return 1.0f;
}

void btlClearAllActorEntrySlots(BtlUnit *unit) {
    u32 nextIndex;
    BtlActorEntrySlotIndex slotIndex;

    slotIndex = 0;
    do {
        nextIndex = slotIndex + 1;
        btlClearActorEntrySlot(unit, slotIndex);
        slotIndex = nextIndex;
    } while (nextIndex < 7);
}

BtlActorEntryExpirationResult btlActorEntryIsExpired(
    const BtlUnit *unit, BtlActorEntrySlotIndex slotIndex) {
    if (unit->entrySlots[slotIndex].code == 0) {
        return BTL_ACTOR_ENTRY_EXPIRATION_RESULT_NOT_EXPIRED;
    }
    return unit->entrySlots[slotIndex].countdown < 1
               ? BTL_ACTOR_ENTRY_EXPIRATION_RESULT_EXPIRED
               : BTL_ACTOR_ENTRY_EXPIRATION_RESULT_NOT_EXPIRED;
}

BtlActorEntryCodeMatch btlMatchActorEntryCode(
    const BtlUnit *unit, BtlActorEntrySlotIndex slotIndex) {
    s16 value = unit->entrySlots[slotIndex].code;
    if (btlActorEntryCodeDefaults[slotIndex].maxCode != 0) {
        if (btlActorEntryCodeDefaults[slotIndex].maxCode == value) {
            return BTL_ACTOR_ENTRY_CODE_MATCH_MAX_CODE;
        }
    }
    if (btlActorEntryCodeDefaults[slotIndex].minCode != 0) {
        if (btlActorEntryCodeDefaults[slotIndex].minCode == value) {
            return BTL_ACTOR_ENTRY_CODE_MATCH_MIN_CODE;
        }
    }
    return BTL_ACTOR_ENTRY_CODE_MATCH_NONE;
}

/* Clamp a signed entry-code delta and refresh timing only when the result is nonzero. */
void btlAdjustActorEntryCode(BtlUnit *unit, BtlActorEntrySlotIndex slotIndex, s16 delta) {
    s16 code = unit->entrySlots[slotIndex].code;
    code += delta;

    if (code > btlActorEntryCodeDefaults[slotIndex].maxCode) {
        code = btlActorEntryCodeDefaults[slotIndex].maxCode;
    }
    if (code < btlActorEntryCodeDefaults[slotIndex].minCode) {
        code = btlActorEntryCodeDefaults[slotIndex].minCode;
    }
    if (code != 0) {
        unit->entrySlots[slotIndex].initialValue = btlActorEntryCodeDefaults[slotIndex].initialValue;
        unit->entrySlots[slotIndex].countdown = btlActorEntryCodeDefaults[slotIndex].countdownFrames;
    }
    unit->entrySlots[slotIndex].code = code;
}

void btlSetActorEntryCode(BtlUnit *unit, BtlActorEntrySlotIndex slotIndex,
                          BtlActorEntryCodeValue code) {
    unit->entrySlots[slotIndex].code = code;
}

void btlClearActorEntrySlot(BtlUnit *unit, BtlActorEntrySlotIndex slotIndex) {
    unit->entrySlots[slotIndex].code = 0;
    unit->entrySlots[slotIndex].initialValue = -1;
    unit->entrySlots[slotIndex].countdown = -1;
}

BtlActorEntryCodeValue btlGetActorEntryCode(
    const BtlUnit *unit, BtlActorEntrySlotIndex slotIndex) {
    return unit->entrySlots[slotIndex].code;
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
            if (index == 4 && (unit->status.flags & 0x200)) {
                factor = ((f32)stage * 0.5f + 8.0f) * 0.125f;
            } else {
                factor = ((f32)stage + 8.0f) * 0.125f;
            }
        } else if (stage > 0) {
            if (index == 4 && (unit->status.flags & 0x400)) {
                factor = ((f32)stage + 8.0f) * 0.125f;
            } else {
                factor = ((f32)(stage * 2) + 8.0f) * 0.125f;
            }
        }
        break;
    }
    if (index == 0 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, BTL_ACTOR_ENTRY_SLOT_PHYSICAL_CHARGE) > 0) {
        if (btlActorEntryIsExpired(unit, BTL_ACTOR_ENTRY_SLOT_PHYSICAL_CHARGE) != 0) {
            factor *= 2.5f;
            btlBossDebugPrintf("btl:BUTURIx2\n");
        }
    }
    if (index == 1 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, BTL_ACTOR_ENTRY_SLOT_MAGIC_CHARGE) > 0) {
        if (btlActorEntryIsExpired(unit, BTL_ACTOR_ENTRY_SLOT_MAGIC_CHARGE) != 0) {
            factor *= 2.5f;
            btlBossDebugPrintf("btl:MAGICx2\n");
        }
    }
    return factor;
}

/* Apply mask-selected signed entry deltas, then sign-specific clears. */
void btlApplyMaskedActorEntryAdjustments(BtlUnit *unit, u32 flags, s16 delta) {
    if (flags == 0) {
        return;
    }
    if (flags & 1) {
        btlAdjustActorEntryCode(unit, 0, delta);
    }
    if (flags & 2) {
        btlAdjustActorEntryCode(unit, 0, -delta);
    }
    if (flags & 4) {
        btlAdjustActorEntryCode(unit, 1, delta);
    }
    if (flags & 8) {
        btlAdjustActorEntryCode(unit, 1, -delta);
    }
    if (flags & 0x10) {
        btlAdjustActorEntryCode(unit, 2, delta);
    }
    if (flags & 0x20) {
        btlAdjustActorEntryCode(unit, 2, -delta);
    }
    if (flags & 0x40) {
        btlAdjustActorEntryCode(unit, 3, delta);
    }
    if (flags & 0x80) {
        btlAdjustActorEntryCode(unit, 3, -delta);
    }
    if (flags & 0x100) {
        btlAdjustActorEntryCode(unit, 4, delta);
    }
    if (flags & 0x200) {
        btlAdjustActorEntryCode(unit, 4, -delta);
    }
    if (flags & 0x400) {
        btlAdjustActorEntryCode(unit, BTL_ACTOR_ENTRY_SLOT_PHYSICAL_CHARGE, delta);
    }
    if (flags & 0x2000) {
        btlAdjustActorEntryCode(unit, BTL_ACTOR_ENTRY_SLOT_MAGIC_CHARGE, delta);
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

/* Index of the lowest set bit pair in the mask (bits 10 and 13 are single), or -1. */
BtlActorEntrySlotIndexResult btlLowestSetPairIndex(u32 mask) {
    s32 index = 0;
    s32 step = 1;

    if (mask == 0)
        return -step;
    if (mask & 0x0001)
        return index;
    if (mask & 0x0002)
        return index;
    index += step;
    if (mask & 0x0004)
        return index;
    if (mask & 0x0008)
        return index;
    index += step;
    if (mask & 0x0010)
        return index;
    if (mask & 0x0020)
        return index;
    index += step;
    if (mask & 0x0040)
        return index;
    if (mask & 0x0080)
        return index;
    index += step;
    if (mask & 0x0100)
        return index;
    if (mask & 0x0200)
        return index;
    index += step;
    if (mask & 0x0400)
        return index;
    index += step;
    if (mask & 0x2000)
        return index;
    return -step;
}

void btlTickActorEntryCountdowns(BtlUnit *unit) {
    u32 index;
    /* Stride through the slot-array bytes, then access each actual countdown member. */
    u8 *countdownBytes = (u8 *)&unit->entrySlots + (u32)&((BtlUnitEntrySlot *)0)->countdown;
    for (index = 0; index < 7; index++, countdownBytes += sizeof(BtlUnitEntrySlot)) {
        s16 *countdown = (s16 *)countdownBytes;
        if (*countdown >= 0) {
            if (*countdown == 0) {
                *countdown = -1;
            }
            --*countdown;
        }
    }
}

/* Return the product of enabled element-ability parameters, or one. */
f32 btlGetElementAbilityMultiplier(BtlUnit *actor, s32 attr) {
    f32 scale = 1.0f;

    switch (attr) {
    case 2:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x20D)) {
            scale *= datAbilityParameters[0x20D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x212)) {
            scale *= datAbilityParameters[0x212 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 3:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x20E)) {
            scale *= datAbilityParameters[0x20E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x213)) {
            scale *= datAbilityParameters[0x213 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 4:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x20F)) {
            scale *= datAbilityParameters[0x20F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x214)) {
            scale *= datAbilityParameters[0x214 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 5:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x210)) {
            scale *= datAbilityParameters[0x210 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x215)) {
            scale *= datAbilityParameters[0x215 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case 6:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x211)) {
            scale *= datAbilityParameters[0x211 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x216)) {
            scale *= datAbilityParameters[0x216 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    return scale;
}

s32 btlGetAbilityAttributeMultiplierPercent(BtlUnit *actor, s32 attr) {
    u32 value = 100;

    switch (attr) {
    case 0:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x23C)) {
            value = (u32)(datAbilityParameters[0x23C - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 2:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x23D)) {
            value = (u32)(datAbilityParameters[0x23D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 3:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x23E)) {
            value = (u32)(datAbilityParameters[0x23E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 4:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x23F)) {
            value = (u32)(datAbilityParameters[0x23F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 5:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x240)) {
            value = (u32)(datAbilityParameters[0x240 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 6:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x241)) {
            value = (u32)(datAbilityParameters[0x241 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 8:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x242)) {
            value = (u32)(datAbilityParameters[0x242 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 9:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x243)) {
            value = (u32)(datAbilityParameters[0x243 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    }
    return value;
}

s32 btlHasMappedSpecialAbilityForSlot(BtlUnit *unit, u32 slot) {
    if (slot == 0 && btlCheckSpecialAbility(&unit->partyRecord, 0x24F) != 0) {
        return 1;
    }
    if (slot == 8 && btlCheckSpecialAbility(&unit->partyRecord, 0x251) != 0) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(&unit->partyRecord, 0x252) != 0) {
        return 1;
    }
    if (slot == 10 && btlCheckSpecialAbility(&unit->partyRecord, 0x244) != 0) {
        return 1;
    }
    if (slot == 11 && btlCheckSpecialAbility(&unit->partyRecord, 0x245) != 0) {
        return 1;
    }
    if (slot == 12 && btlCheckSpecialAbility(&unit->partyRecord, 0x246) != 0) {
        return 1;
    }
    if (slot == 13 && btlCheckSpecialAbility(&unit->partyRecord, 0x247) != 0) {
        return 1;
    }
    if (slot == 14 && btlCheckSpecialAbility(&unit->partyRecord, 0x248) != 0) {
        return 1;
    }
    if (slot < 0xF) {
        if (slot >= 0xA && btlCheckSpecialAbility(&unit->partyRecord, 0x253) != 0) {
            return 1;
        }
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x257) != 0) {
            return 1;
        }
    }
    if (slot != 7 && btlCheckSpecialAbility(&unit->partyRecord, 0x249) != 0) {
        return 1;
    }
    return 0;
}

u32 btlHasSpecialAbilityWhenArgumentUnset(BtlUnit *unit, s64 argumentValue) {
    if (argumentValue == 0 && btlCheckSpecialAbility(&unit->partyRecord, 0x254) != 0) {
        return 1;
    }
    return 0;
}

s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *unit, u32 slot) {
    if (slot == 8 && btlCheckSpecialAbility(&unit->partyRecord, 0x255)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(&unit->partyRecord, 0x256)) {
        return 1;
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x258)) {
            return 1;
        }
    }
    return 0;
}

void *btlGetCommandAttributeName(s32 index) {
    s8 resource = datCommandSelectors[index].stat;
    if (resource < 0) {
        return NULL;
    }
    return btlCommandAttributeNames[resource];
}

/* The label resource index is the copied party record's unit ID. */
const u8 *btlGetIndexedUiResource(const BtlUnit *unit) {
    return btlPartyUnitGunTypeNames[unit->partyRecord.unitId];
}

/* Three 0x38-byte saved records; the snapshot writer clears all 0xA8 bytes. */
typedef struct BtlActorStateSnapshot {
    BtlUnitEntrySlot entrySlots[7];
    u8 pad2A[2];
    u32 flags;
    u8 pad30[4];
    u16 status;
    u16 unitId;
} BtlActorStateSnapshot;

extern BtlActorStateSnapshot btlSavedPartyUnitStates[3];

#include "sdf_debug_api.h"

/* Snapshot active party actors for the later party-slot restoration pass. */
INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A18F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1908);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1918);

void btlPushSavedPartyUnitStates(void) {
    s32 count = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;

    memset(btlSavedPartyUnitStates, 0, sizeof(btlSavedPartyUnitStates));
    for (unit = battle->units; unit != NULL; unit = unit->next) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                btlSavedPartyUnitStates[count].flags = unit->status.flags;
                btlSavedPartyUnitStates[count].unitId = unit->partyRecord.unitId;
                memcpy(btlSavedPartyUnitStates[count].entrySlots, unit->entrySlots,
                       sizeof(btlSavedPartyUnitStates[count].entrySlots));
                btlSavedPartyUnitStates[count].status = unit->partyRecord.status & 0xEFF9;
                count++;
                printf("btl:state push[%d:%X]\n", count,
                              unit->partyRecord.unitId);
            }
        }
    }
}

void btlRestorePartySlotSnapshots(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 slot;
    BtlUnit *unit;
    u16 status;

    for (slot = 0; slot < 3; slot++) {
        if (btlSavedPartyUnitStates[slot].unitId == 0) {
            continue;
        }
        for (unit = battle->units; unit != NULL; unit = unit->next) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x200) {
                    if (unit->partyRecord.unitId == btlSavedPartyUnitStates[slot].unitId) {
                        if (btlSavedPartyUnitStates[slot].flags & 0x1000) {
                            unit->status.flags |= 0x1000;
                            unit->partyRecord.flags |= 0x1000;
                        } else {
                            unit->status.flags &= ~0x1000;
                            unit->partyRecord.flags &= ~0x1000;
                        }
                        memcpy(unit->entrySlots, btlSavedPartyUnitStates[slot].entrySlots,
                               sizeof(btlSavedPartyUnitStates[slot].entrySlots));
                        status = btlSavedPartyUnitStates[slot].status;
                        unit->partyRecord.status = status;
                        if ((status & 0x7FFF) == 0x4000) {
                            unit->status.flags |= 0x20;
                            unit->partyRecord.hp = 0;
                        }
                        printf("btl:state pos[%d:%X]\n", slot, unit->partyRecord.unitId);
                        break;
                    }
                }
            }
        }
    }
}

extern u32 btlGetAdjustedSlotAffinity(BtlUnit *, s32, s32);

BtlActionPressResult btlDetermineActionPressResult(BtlUnit *attacker, BtlUnit *target, u32 command) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 attribute;
    u32 mask;
    s32 ratioReason = -1;
    s32 autoReason = -1;
    s32 chance;
    s32 fixedChance;
    f32 multiplier;
    s32 autoChance;
    datUnitWork_t *record;

    attribute = btlGetActorIndexedSignedValue(attacker, command);
    mask = btlEncodeActorIndexAsSelectionMask(attribute);
    if (mask & 0xE0001) {
        return BTL_ACTION_PRESS_RESULT_NORMAL;
    }
    if (btlGetAdjustedSlotAffinity(target, command, attribute) & 0x80000000u) {
        return BTL_ACTION_PRESS_RESULT_WEAKNESS;
    }
    if (!(mask & 0x10006)) {
        return BTL_ACTION_PRESS_RESULT_NORMAL;
    }
    chance = 0;
    fixedChance = 0;
    multiplier = 1.0f;
    autoChance = 0;
    if (command == 0) {
        record = &attacker->partyRecord;
        if (btlCheckSpecialAbility(record, 0x206)) {
            ratioReason = 0x206;
            multiplier = datAbilityParameters[0x206 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(record, 0x207)) {
            autoReason = 0x207;
            autoChance = (s32)(datAbilityParameters[0x207 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * 100.0f);
        }
        if (btlCheckSpecialAbility(record, 0x208)) {
            autoReason = 0x208;
            autoChance = (s32)(datAbilityParameters[0x208 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * 100.0f);
        }
    }
    switch (target->partyRecord.status & 0x7FFF) {
    case 8:
        fixedChance = 60;
        break;
    case 2:
    case 4:
        fixedChance = 100;
        break;
    }
    record = &target->partyRecord;
    if (btlCheckSpecialAbility(record, 0x22F)) {
        multiplier *= datAbilityParameters[0x22F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    if (datCommandRecords[command].criticalPercent == 100) {
        chance = 100;
        fixedChance = 100;
    }
    if (datCommandRecords[command].effectType == 0) {
        if (battle->runtimeFlags & 0x1000) {
            return BTL_ACTION_PRESS_RESULT_CRITICAL;
        }
        if (chance == 0) {
            chance = evtRunContextWithArguments(4, evtScriptArgumentFromPointer(&attacker->partyRecord),
                evtScriptArgumentFromPointer(record), command, 0);
        }
        chance = (s32)((f32)chance * multiplier);
        if (chance >= 36) {
            chance = 35;
        }
        btlBossDebugPrintf("btl:critical=%d%%[raito=%.3f,fix=%d,auto=%d]\n", chance, multiplier, fixedChance,
                           autoChance);
        if ((s32)btlRollAiBucket() < fixedChance) {
            return BTL_ACTION_PRESS_RESULT_CRITICAL;
        }
        if ((s32)btlRollAiBucket() < autoChance) {
            if (autoReason != -1) {
                attacker->pendingSkillNameIndex = autoReason;
            }
            return BTL_ACTION_PRESS_RESULT_CRITICAL;
        }
        if ((s32)btlRollAiBucket() < chance) {
            if (ratioReason != -1) {
                attacker->pendingSkillNameIndex = ratioReason;
            }
            return BTL_ACTION_PRESS_RESULT_CRITICAL;
        }
    }
    return BTL_ACTION_PRESS_RESULT_NORMAL;
}
