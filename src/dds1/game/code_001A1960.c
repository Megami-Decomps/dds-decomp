#include "sdk_ps2.h"
#include "common.h"
#include "btl_actor_status_table_api.h"
#include "btl_special_ability.h"
#include "dat_unit_has_skill_api.h"
#include "btl_animation_mode_entry.h"
#include "btl_effect_position.h"
#include "eff_random_mod.h"
#include "eff_random.h"
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
#include "btl.h"
#include "btl_command_partner_output.h"
#include "btl_state.h"
#include "eff.h"
#include "btl_action.h"
#include "kwln.h"
#include "dat_state.h"
#include "btl_resolve_unit_value_api.h"
#include "btl_select_transform_parameters_api.h"
#include "dat_command.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"

extern void btlBossDebugPrintf(const char *format, ...);

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
    BtlSceneSlot slots[8];
    BtlTask *groupPrimary[20];
    BtlTask *groupSecondary[45];
    BtlTask *groupTertiary[15];
    u8 pad_42C[0x20];
    BtlSceneFadingRecord fadingRecords[8]; /* 0x44C */
    u8 pad_48C[0x18];
    EffectSlotSet *resA; /* 0x4A4 */
    EffectSlotSet *resB;
    EffectSlotSet *resC;
    u8 pad_4B0[0x100];
    s32 (*sceneCallback)();
    u8 pad_5B4[0xD4];
    u32 (*enemyLimitOverride)(BtlUnit *, DatBattleSceneRecord *, u32); /* 0x688 */
} BattleController;

typedef struct EntryPair {
    s16 first;
    s16 second;
    s16 initialValue;
    s16 countdown;
} EntryPair;

extern EntryPair btlActorEntryCodeDefaults[];

extern u32 datComputeSkillBoostedMaxHp(DatPartyRecord *);

extern u32 datComputeSkillBoostedMaxMp(DatPartyRecord *);

extern void datClearUnitStatusBits(DatPartyRecord *record, s32 mask);

extern s32 datGetClampedProfileAdjustedStat(DatPartyRecord *, s32);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

extern u8 btlSharedActorTransformParameters[];

extern void *btlCommandAttributeNames[];

extern void *btlPartyUnitGunTypeNames[];


extern DatEnemyRecord *datEnemyRecords;


extern s32 datCalculateCommandBaseValue(DatPartyRecord *, s32);
extern u8 *D_003BAA14;

extern u8 *D_003BAA28;

extern u8 *D_003BAA10;

extern u8 *D_003BAA20;

extern u8 *D_003BAA30;

extern u8 *datRosterDetails;

extern s32 fldCountSceneSlots(void);


extern char D_003A1A40[];

typedef struct BattleFearState {
    u8 pad_000[0x1FC];
    u32 flags;
} BattleFearState;

extern s32 dds3FindEntryIndex(s32);

extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);

extern EffRandState effSharedRandomState[];

/* Clear the supplied party-record status mask. */
void btlClearPartyRecordStatusBits(DatPartyRecord *record, s32 mask) {
    datClearUnitStatusBits(record, mask);
}

/* Set the actor's selected entry index. */
void btlSetActorSelectedEntryIndex(BtlUnit *actor, u32 index) {
    actor->selectedEntryIndex = index;
}

/* No selected entry is represented by -1. */
void btlClearActorSelectedEntryIndex(BtlUnit *actor) {
    actor->selectedEntryIndex = -1;
}


/* Initialize enemy vitals/stats and pack its nonzero skill IDs into the party record. */
void btlInitializePartyRecordFromEnemy(DatPartyRecord *entry, s32 index) {
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

DatPartyRecord *btlGetActorEntryData(BtlUnit *actor) {
    DatPartyRecord *entry;

    if ((actor->status.flags & 0x400) == 0) {
        entry = btlGetIndexedPartyEntryRecord(actor->partyIndex);
        return entry;
    }
    return &actor->partyRecord;
}

DatPartyRecord *btlGetCurrentPartyEntryRecord(s32 rosterIndex) {
    s32 index;

    index = dds3FindEntryIndex(rosterIndex);
    return &datGameState->party[index];
}

DatPartyRecord *btlGetIndexedPartyEntryRecord(s32 index) {
    return &datGameState->party[index];
}

void btlSyncPlayerWork(BtlUnit *actor) {
    DatPartyRecord *src = &actor->partyRecord;
    DatPartyRecord *dst = btlGetIndexedPartyEntryRecord(actor->partyIndex);
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
    dst->level = src->level;
    maxHp = datComputeSkillBoostedMaxHp(dst);
    maxMp = datComputeSkillBoostedMaxMp(dst);
    dst->hp = src->hp < maxHp ? src->hp : maxHp;
    dst->mp = src->mp < maxMp ? src->mp : maxMp;
    memcpy(dst->baseStats, src->baseStats, sizeof(dst->baseStats));
    dst->status = src->status & 0x7FFF;
    dst->savedCommandPhase = src->savedCommandPhase;
    dst->unk18E = src->unk18E;
    dst->actionSlot = src->actionSlot;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

s32 btlFindPartyEntryIndexForActor(s32 unit) {
    return dds3FindEntryIndex(((BtlUnit *)unit)->partyRecord.unitId);
}

/* Retail no-op after party actor insertion, removal or roster swap. */
void btlOnPartyRosterChange(void) {
}

BtlUnit *btlFindActiveActorByKind(s32 index) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    for (; node != 0; node = node->next) {
        u32 flags = node->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if (index == node->partyIndex) {
                    return node;
                }
            }
        }
    }
    return 0;
}

extern void btlCopyUnitStats(BtlUnit *unit, DatPartyRecord *source);

/* Insert a complete party record before actors with a larger priority. */
void btlInsertPartyActorByPriority(BtlUnit *unit, u8 sourceIndex, u8 priority) {
    DatPartyRecord saved;
    s32 insertion = 0;
    s32 i;

    if (datGameState->party[0].flags & 2) {
        do {
            if ((u16)(datGameState->party[insertion].flags & 1) == 0) {
                break;
            }
            if (priority < btlFindActiveActorByKind(insertion)->lookupId) {
                break;
            }
            insertion++;
            if (insertion >= 5) {
                break;
            }
        } while (datGameState->party[insertion].flags & 2);
    }
    memcpy(&saved, &datGameState->party[sourceIndex], sizeof(saved));
    for (i = sourceIndex; i < 4; i++) {
        memcpy(&datGameState->party[i], &datGameState->party[i + 1], sizeof(saved));
        if (datGameState->party[i + 1].flags & 2) {
            btlFindActiveActorByKind(i + 1)->partyIndex = i;
        }
    }
    for (i = 4; i > insertion; i--) {
        memcpy(&datGameState->party[i], &datGameState->party[i - 1], sizeof(saved));
        if (datGameState->party[i - 1].flags & 2) {
            btlFindActiveActorByKind(i - 1)->partyIndex = i;
        }
    }
    memcpy(&datGameState->party[i], &saved, sizeof(saved));
    btlCopyUnitStats(unit, &saved);
    unit->partyRecord.flags |= 2;
    datGameState->party[i].flags |= 2;
    unit->partyIndex = i;
    btlOnPartyRosterChange();
    btlBossDebugPrintf("btl:party in %d->%d[%d]\n", sourceIndex, i, saved.unitId);
}

/* Move a departing actor behind the remaining occupied party records. */
void btlRemovePartyActorAndShiftEntries(BtlUnit *unit) {
    DatPartyRecord saved;
    DatGameState *scanState = datGameState;
    s32 originalIndex;
    s32 index;

    originalIndex = unit->partyIndex;
    memcpy(&saved, &datGameState->party[originalIndex], sizeof(saved));
    index = originalIndex;
    if (index < 4 && (u16)(scanState->party[index + 1].flags & 1)) {
        do {
            memcpy(&datGameState->party[index], &datGameState->party[index + 1], sizeof(saved));
            if (datGameState->party[index + 1].flags & 2) {
                btlFindActiveActorByKind(index + 1)->partyIndex = index;
            }
            index++;
            if (index >= 4) {
                break;
            }
            scanState = datGameState;
        } while ((u16)(scanState->party[index + 1].flags & 1));
    }
    memcpy(&datGameState->party[index], &saved, sizeof(saved));
    unit->partyRecord.flags &= ~2;
    datGameState->party[index].flags &= ~2;
    unit->partyIndex = 6;
    btlOnPartyRosterChange();
    btlBossDebugPrintf("btl:party out %d->%d[%d]\n", originalIndex, index,
                       saved.unitId);
}

/* Swap complete records while keeping the actor in its original party slot. */
void btlSynchronizePartyActorRecords(BtlUnit *actor, u8 targetIndex) {
    DatPartyRecord previous;

    if (actor->partyIndex != targetIndex) {
        memcpy(&previous, &datGameState->party[actor->partyIndex], sizeof(previous));
        memcpy(&datGameState->party[actor->partyIndex], &datGameState->party[targetIndex], sizeof(previous));
        memcpy(&datGameState->party[targetIndex], &previous, sizeof(previous));
        btlCopyUnitStats(actor, &datGameState->party[actor->partyIndex]);
        actor->partyRecord.flags |= 2;
        datGameState->party[actor->partyIndex].flags |= 2;
        datGameState->party[targetIndex].flags &= ~2;
        btlOnPartyRosterChange();
        btlBossDebugPrintf("btl:party change %d<->%d[%d<->%d]\n",
            actor->partyIndex, targetIndex, previous.unitId, actor->partyRecord.unitId);
    }
}

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record) {
    if ((record->flags & 4) != 0) {
        return 0;
    }
    return datEnemyRecords[record->unitId].flags;
}

/* Native tail jump forwards the clamped profile-adjusted stat in v0;
 * retain the existing void C surface for matching. */
void btlQueryClampedProfileAdjustedStat(DatPartyRecord *unit, s32 statIndex) {
    datGetClampedProfileAdjustedStat(unit, statIndex);
}

/* Return the requested party-record stat with its native status override. */
s32 btlGetStatWithStatusOverride(DatPartyRecord *unit, s32 statIndex) {
    return datGetStatWithStatusOverride(unit, statIndex);
}

s32 btlApplyCommandAbilityMultiplier(DatPartyRecord *battler, s32 command) {
    u32 value = datCalculateCommandBaseValue(battler, command);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (datCommandRecords[command].costMode) {
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
s8 btlGetLinkageSlotCost(s32 command) {
    return datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].slotCost;
}

s32 btlGetCommandFailureReason(BtlUnit *unit, s32 command) {
    s32 result = 0;
    u32 cost;

    if (command == 0) {
        return 0;
    }
    if (datCommandRecords[command].effectType == 1 ||
        datCommandRecords[command].costMode == DAT_COMMAND_COST_MODE_MP) {
        if ((unit->partyRecord.status & 0x7FFF) == 0x10) {
            return 3;
        }
    }
    if (datCommandSelectors[command].kind == 2 &&
        (unit->partyRecord.status & 0x7FFF) == 0x40) {
        return 5;
    }
    if (datCommandSelectors[command].kind == 1) {
        if ((unit->partyRecord.status & 0x7FFF) == 0x1000) {
            return 4;
        }
        if (fldCountSceneSlots() < (u32)btlGetLinkageSlotCost(command)) {
            return 6;
        }
    }
    if (datCommandRecords[command].flags & 4) {
        return 0;
    }

    cost = btlApplyCommandAbilityMultiplier(&unit->partyRecord, command);
    switch (datCommandRecords[command].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if ((datCommandRecords[command].flags & 8) == 0) {
            if (cost >= unit->partyRecord.hp) {
                result = 1;
            }
        } else if (unit->partyRecord.hp < cost) {
            result = 1;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        result = unit->partyRecord.mp < cost ? 2 : 0;
        break;
    }
    return result;
}

s32 btlGetCombinedPartyCommandPower(DatPartyRecord *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    DatPartyRecord snapshot = *base;
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
    return btlApplyCommandAbilityMultiplier(&snapshot, command);
}

s32 btlGetLinkedPartyCommandFailureReason(BtlUnit *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    BtlUnit snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;

    if (first != NULL) {
        if ((first->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((first->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        totalMaxHp = first->partyRecord.maxHp;
        count = 1;
    }
    if (second != NULL) {
        if ((second->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((second->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += second->partyRecord.maxHp;
    }
    if (third != NULL) {
        if ((third->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((third->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += third->partyRecord.maxHp;
    }
    snapshot.partyRecord.maxHp = totalMaxHp / count;
    return btlGetCommandFailureReason(&snapshot, command);
}

s8 btlGetActorIndexedSignedValue(BtlUnit *unit, s32 index) {
    if (index == 0 && (unit->status.flags & 0x400) != 0) {
        return datEnemyRecords[unit->partyRecord.unitId].indexedValueOverride;
    }
    return datCommandSelectors[index].stat;
}

s32 btlGetUnitElementAffinity(BtlUnit *battler, s32 element) {
    return btlResolveUnitValueWithOverride(&battler->partyRecord, element);
}

extern s32 datGetEffectiveAffinity(DatPartyRecord *, s32);

s32 btlResolveUnitValueWithOverride(DatPartyRecord *record, s32 value) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))((u8 *)btlGetRuntime() + 0x670);
    if (handler != 0) {
        s32 result = handler((s32)record, value);
        if (result != -1) {
            return result;
        }
    }
    return datGetEffectiveAffinity(record, value);
}

void *btlGetSideIndexedActorStatusTable(s32 side, s32 index) {
    if (side == 0) {
        return (void *)(u32)(D_003BAA10 + index * 0x270);
    }
    return (void *)(u32)(D_003BAA20 + index * 0x270);
}

BtlActorTransformParameters *btlSelectSharedOrIndexedTransformParameters(s32 side, s32 index) {
    if (side == 0) {
        return (BtlActorTransformParameters *)btlSharedActorTransformParameters;
    }
    return (BtlActorTransformParameters *)(D_003BAA30 + index * 24);
}

s32 btlSelectSideIndexedActorParameterTable(s32 category, s32 actorId) {
    if (category == 0) {
        return D_003BAA14 + actorId * 0x74;
    }
    return D_003BAA28 + actorId * 0x74;
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

s32 btlBuildEligibleTargetIndexList(void *actionArg, BtlIndexList *targets,
                  s32 area, s32 rule, s32 excludedStatus) {
    BtlTask *action = (BtlTask *)actionArg;
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
    areaMask = 0;
    if ((area & 1) != 0) {
        areaMask = alliedArea;
    }
    if ((area & 2) != 0) {
        areaMask |= opposingArea;
    }
    btlClearIndexList(targets);
    if ((rule & 1) != 0) {
        btlAppendIndexListEntry(targets, action->unit);
        return 1;
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
            if (excludedStatus == 0) {
                continue;
            }
            if ((excludedStatus & 0x4000) != 0 && excludedStatus != -1) {
                continue;
            }
        }
        if ((flags & areaMask) == 0) {
            continue;
        }
        if ((rule & 2) != 0 && (unit->status.flags & 1) != 0) {
            if (action->unit == unit) {
                continue;
            }
            collected[count++] = unit;
        } else if ((rule & 8) != 0) {
            if ((unit->status.flags & 1) != 0) {
                continue;
            }
            collected[count] = unit;
            selected = count++;
        } else {
            if ((rule & 4) == 0) {
                if ((unit->status.flags & 1) == 0) {
                    continue;
                }
            } else {
                if ((unit->status.flags & 1) == 0) {
                    selected = count;
                }
            }
            if ((unit->status.flags & 1) != 0) {
                if (excludedStatus != 0) {
                    if (excludedStatus != -1) {
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
    return count != 0;
}

extern s32 btlBuildEligibleTargetIndexList(void *, BtlIndexList *, s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1788);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A17A8);

u32 btlSelectCommandTargetPolicy(void *action, BtlIndexList *destination, s32 unused) {
    s32 category = 3;
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 mode = 0;
    s32 flags = 0;
    s32 mask = 0;
    BtlTask *task = (BtlTask *)action;
    s32 value = task->indexWork.phase;

    switch (value) {
    case 1: {
        BtlUnit *actor = task->unit;
        if (btlUnitStatusPair(actor) & 0x1400) {
            if (btlCheckSpecialAbility(&actor->partyRecord, 0x21E) != 0) {
                category = 1;
                task->unit->pendingAbilityId = 0x21E;
            } else {
                category = 0;
            }
            mode = 2;
            flags = 0;
            mask = 0;
        } else {
            value = actor->partyRecord.unitId;
            category = ((EventRosterStat *)datRosterDetails)[value].targetType;
            mode = ((EventRosterStat *)datRosterDetails)[value].options;
            flags = ((EventRosterStat *)datRosterDetails)[value].targetFlags;
            mask = ((EventRosterStat *)datRosterDetails)[value].restriction;
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
        mode = datCommandRecords[value].options;
        flags = datCommandRecords[value].targetFlags;
        mask = datCommandRecords[value].restriction;
        if ((datCommandRecords[value].attribute.bits & 0x400000FF) == 0x40000002 && mask == 0) {
            mask = -1;
        }
        break;
    }
    case 9:
        category = 0;
        mode = 1;
        flags = 4;
        mask = -1;
        break;
    }
    if (battle->unk_1FC & BTL_STATE_FLAG_BYPASS_COMMAND_BLOCKS) {
        mode = 3;
        flags &= ~0xB;
        mask = 0;
    }
    if (destination != NULL) {
        btlBuildEligibleTargetIndexList(task, destination, mode, flags, mask);
    }
    return category;
}

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 actionTask, BtlIndexList *targets);

s32 btlFindEligibleTargetForMultiActorCommand(s32 actionTask, BtlIndexList *targets) {
    BtlTask *action = (BtlTask *)actionTask;
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
    s32 lowestId;

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
        lowestId = 4;
        unit = NULL;
        if (count > 0) {
            remaining = count;
            read = eligible;
            do {
                BtlUnit *candidate = *read++;
                s32 lookupId = candidate->lookupId;
                if (lookupId < lowestId) {
                    unit = candidate;
                    lowestId = lookupId;
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
    DatAffinityRecord *record;
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
extern s32 btlFindCommandPartnersByAffinity(BtlUnit *unit, s32 command, BtlUnit **first, BtlUnit **second);

s32 btlResolveCommandAffinityPartners(BtlUnit *unit, s32 command, BtlCommandPartnerOutput *output) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    s32 *requirementCursor;
    u32 requiredCount;
    u32 i;
    s32 result;

    if (datCommandSelectors[command].kind != 1) {
        return 0;
    }
    if ((u32)(command - DAT_AFFINITY_FIRST_COMMAND) >= 0x55) {
        return 0;
    }
    requiredCount = 0;
    requirementCursor = datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].requirements;
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount == 0) {
        return 0;
    }
    if ((datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].flags & 2) == 0 &&
        (datBattleSceneRecords[controller->mode].flags & 1) == 0) {
        output->commandId = command;
        result = btlFindCommandPartnersByAffinity(unit, command, &output->firstPartner, &output->secondPartner);
    } else {
        output->commandId = command;
        output->firstPartner = NULL;
        output->secondPartner = NULL;
        if (requiredCount == 3) {
            for (i = 1; i < requiredCount; i++) {
                BtlTask *entry = fldGetSceneGroupEntry(i);
                BtlUnit *partner;

                if (entry == NULL) {
                    return 0;
                }
                partner = entry->unit;
                if ((partner->status.flags & 0x400) == 0) {
                    return 0;
                }
                if ((partner->status.flags & 0xE0) != 0) {
                    return 0;
                }
                if ((partner->partyRecord.status & 0x2A0E) != 0) {
                    return 0;
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
            return 1;
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
                    return 0;
                }
                partner = entry->unit;
                if ((partner->status.flags & 0x400) == 0) {
                    return 0;
                }
                if ((partner->status.flags & 0xE0) != 0) {
                    return 0;
                }
                if ((partner->partyRecord.status & 0x2A0E) == 0) {
                    output->firstPartner = partner;
                } else {
                    return 0;
                }
            }
        } else {
            return 0;
        }
        result = 1;
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

/* Decode an element flag through the shared native flag-to-index table. */
s32 btlFlagToElementIndex(s32 flag) {
    return datFlagToElementIndex(flag);
}

extern s32 evtGetMirroredSolarPhase(void);

s32 btlCheckSpecialAbility(DatPartyRecord *object, s32 flag) {
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

extern EffRandState effSharedRandomState[];

extern s32 btlSelectEnemyDropItem(s32 actorAddress, s32 mode);

s32 btlSelectActorAction(s32 object) {
    s32 result = btlSelectEnemyDropItem(object, 1);
    if (result == 0) {
        result = (sdfGetRand((EffRandState *)effSharedRandomState) & 1) != 0 ? 2 : 7;
    }
    return result;
}

extern s32 btlRollAiBucket(void);

/* Select an enemy item drop; mode 1 bypasses the story-flag override item. */
s32 btlSelectEnemyDropItem(s32 actorAddress, s32 mode) {
    BtlUnit *actor = (BtlUnit *)actorAddress;
    DatEnemyRecord *enemy = &datEnemyRecords[actor->partyRecord.unitId];
    s32 item = 0;
    u16 i;


    if (mode != 1 && enemy->overrideAction != 0 && enemy->overrideFlag != 0 &&
        mdlFlagTest(enemy->overrideFlag) != 0) {
        if ((u8)btlRollAiBucket() < enemy->overrideChance) {
            item = enemy->overrideAction;
        }
    }
    for (i = 0; i < 2 && item == 0; i++) {
        if (enemy->additionalDropItemIds[i] != 0 && (u8)btlRollAiBucket() < enemy->actionChances[i]) {
            item = enemy->additionalDropItemIds[i];
        }
    }
    return item;
}

/* Accumulate one item drop; new IDs are discarded if all three slots are occupied. */
void btlAddItemDrop(u16 item) {
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

extern s32 btlAverageLevelsForMaskExcludingStatus20(u32);

extern s32 btlCalculateEnemyExperienceReward(u8 *, u8 *);

extern s32 btlGetEnemyMoney(u8 *, u8 *);

extern char D_003A17F8[];

extern char D_003A1818[];

extern char D_003A1830[];

extern char D_003A1848[];

void btlAccumulateEnemyDefeatRewards(BtlUnit *enemy) {
    BattleController *controller = (BattleController *)btlGetRuntime();
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
        amount = btlCalculateEnemyExperienceReward(NULL, (u8 *)enemy);
        controller->epEarned += amount;
        btlBossDebugPrintf(D_003A1818, controller->epEarned, amount);
    }
    if ((enemy->status.stateFlags & 0x400) == 0) {
        amount = btlGetEnemyMoney(NULL, (u8 *)enemy);
        controller->moneyEarned += amount;
        btlBossDebugPrintf(D_003A1830, controller->moneyEarned, amount);
    }
    item = btlSelectEnemyDropItem((s32)enemy, 0);
    if (item != 0) {
        btlAddItemDrop(item);
    }
    enemy->status.stateFlags |= 1;
    btlBossDebugPrintf(D_003A1848, enemy);
}

s32 btlAllActiveUnitsReady(void) {
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    for (; actor != NULL; actor = actor->next) {
        u32 flags = actor->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 0xC0) != 0) {
                    return 0;
                }
                if ((flags & 0x20) != 0) {
                    if ((actor->status.stateFlags & 1) == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

/* Return the battle-parameter scale for the selected encounter's clamped adjustment. */
f32 btlGetEncounterAdjustmentScale(void) {
    BattleController *runtime = (BattleController *)btlGetRuntime();
    s32 adjustment = datBattleAdjustmentRecords[runtime->adjustmentRecordIndex]
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
s32 btlChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    BtlTask *node = ((BattleController *)btlGetRuntime())->taskHead;
    for (; node != 0; node = node->next) {
        if ((node->flags & 8) != 0) {
            BtlUnit *actor = node->unit;
            u32 flags = actor->status.flags;
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
        return candidates[sdfGetRandInt(0, count)];
    }
    return 0;
}

s32 btlCountAvailableUnits(void) {
    BtlUnit *node = ((BattleController *)btlGetRuntime())->actors;
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
    BtlUnit *node = ((BattleController *)btlGetRuntime())->actors;
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
        DatPartyRecord *entry = datGameState->party;
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

void btlClearActorEntrySlot(BtlUnit *unit, s32 index);

void btlClearAllActorEntrySlots(BtlUnit *unit) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        btlClearActorEntrySlot(unit, temp_v1);
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
    if (btlActorEntryCodeDefaults[index].first != 0) {
        if (btlActorEntryCodeDefaults[index].first == value) {
            return 1;
        }
    }
    if (btlActorEntryCodeDefaults[index].second != 0) {
        if (btlActorEntryCodeDefaults[index].second == value) {
            return 2;
        }
    }
    return 0;
}

/* Clamp a signed entry-code delta and refresh timing only when the result is nonzero. */
void btlAdjustActorEntryCode(BtlUnit *unit, s32 index, s16 delta) {
    s16 code = unit->entrySlots[index].code;
    code += delta;

    if (code > btlActorEntryCodeDefaults[index].first) {
        code = btlActorEntryCodeDefaults[index].first;
    }
    if (code < btlActorEntryCodeDefaults[index].second) {
        code = btlActorEntryCodeDefaults[index].second;
    }
    if (code != 0) {
        unit->entrySlots[index].unk02 = btlActorEntryCodeDefaults[index].initialValue;
        unit->entrySlots[index].countdown = btlActorEntryCodeDefaults[index].countdown;
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

#include "btl_actor_entry_code_api.h"

s16 btlGetActorEntryCode(const BtlUnit *unit, s32 index) {
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
        btlAdjustActorEntryCode(unit, 5, delta);
    }
    if (flags & 0x2000) {
        btlAdjustActorEntryCode(unit, 6, delta);
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
s32 btlLowestSetPairIndex(u32 mask) {
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

s32 btlGetCommandAttributeName(s32 index) {
    s8 resource = datCommandSelectors[index].stat;
    if (resource < 0) {
        return 0;
    }
    return (s32)btlCommandAttributeNames[resource];
}

/* The label resource index is the copied party record's unit ID. */
void *btlGetIndexedUiResource(BtlUnit *unit) {
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
                sdfDebugPrintf("btl:state push[%d:%X]\n", count,
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
                        sdfDebugPrintf("btl:state pos[%d:%X]\n", slot, unit->partyRecord.unitId);
                        break;
                    }
                }
            }
        }
    }
}

extern u32 btlGetAdjustedSlotAffinity(BtlUnit *, s32, s32);
extern s32 evtRunContext(s32, s32, s32, s32, u16);

s32 btlDetermineActionPressResult(BtlUnit *attacker, BtlUnit *target, u32 command) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 attribute;
    u32 mask;
    s32 ratioReason = -1;
    s32 autoReason = -1;
    s32 chance;
    s32 fixedChance;
    f32 multiplier;
    s32 autoChance;
    DatPartyRecord *record;

    attribute = btlGetActorIndexedSignedValue(attacker, command);
    mask = btlEncodeActorIndexAsSelectionMask(attribute);
    if (mask & 0xE0001) {
        return 1;
    }
    if (btlGetAdjustedSlotAffinity(target, command, attribute) & 0x80000000u) {
        return 4;
    }
    if (!(mask & 0x10006)) {
        return 1;
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
        if (battle->unk_1FC & 0x1000) {
            return 2;
        }
        if (chance == 0) {
            chance = evtRunContext(4, (s32)&attacker->partyRecord, (s32)record, command, 0);
        }
        chance = (s32)((f32)chance * multiplier);
        if (chance >= 36) {
            chance = 35;
        }
        btlBossDebugPrintf("btl:critical=%d%%[raito=%.3f,fix=%d,auto=%d]\n", chance, multiplier, fixedChance,
                           autoChance);
        if ((s32)btlRollAiBucket() < fixedChance) {
            return 2;
        }
        if ((s32)btlRollAiBucket() < autoChance) {
            if (autoReason != -1) {
                attacker->pendingAbilityId = autoReason;
            }
            return 2;
        }
        if ((s32)btlRollAiBucket() < chance) {
            if (ratioReason != -1) {
                attacker->pendingAbilityId = ratioReason;
            }
            return 2;
        }
    }
    return 1;
}
