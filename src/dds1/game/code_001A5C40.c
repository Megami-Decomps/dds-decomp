/*
 * Translation unit boundary recovered at func_001A5C40. btlGetActorEntryMultiplier
 * and the other helpers declared below belong to the preceding unit
 * (code_001A1960); the retail calls to them keep their possibly-throwing
 * (non-nothrow) shape, so they must stay external here. A same-unit definition
 * makes reorg's branch-slot liveness scan see through the call and annuls the
 * `bnez s1` slot instead of filling it unannulled.
 */
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
/* Functions defined earlier in the previous translation unit. */
DatPartyRecord *btlGetIndexedPartyEntryRecord(s32 index);
s8 btlGetActorIndexedSignedValue(BtlUnit *unit, s32 index);
s32 btlCheckSpecialAbility(DatPartyRecord *object, s32 flag);
f32 btlGetElementAbilityMultiplier(BtlUnit *actor, s32 attr);
s32 btlGetAbilityAttributeMultiplierPercent(BtlUnit *actor, s32 attr);
f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge);

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

s32 func_001A5C40(BtlUnit *attacker, BtlUnit *recipient, s32 command,
                   s32 maxHits, s32 press, s32 attackMode, u32 calculationFlags) {
    f32 attack;
    f32 defense = 1.0f;
    f32 status;
    f32 automatic;
    s8 includeCharge = (datCommandRecords[command].flags ^ 1) & 1;
    s32 bypass;
    s32 attribute;
    s32 base;
    s32 result;
    s32 limit;

    switch (datCommandRecords[command].effectType) {
    case 0:
        bypass = calculationFlags & 1;
        attack = !bypass
            ? btlGetActorEntryMultiplier(attacker, 0, includeCharge) : 1.0f;
        if (attackMode == 1) {
            defense = btlGetActorEntryMultiplier(recipient, 3, includeCharge);
        }
        {
            s32 ailment = attacker->partyRecord.status & 0x7FFF;
            status = 0.5f;
            if (ailment != 1) {
                status = 1.0f;
                if (ailment == 0x1000) {
                    status = 0.1f;
                }
            }
        }
        break;
    case 1:
        if (attackMode == 1 && datCommandRecords[command].hpType != 8 &&
            datCommandRecords[command].hpType != 10) {
            bypass = calculationFlags & 1;
            attack = 1.0f;
            if (!bypass) {
                attack = btlGetActorEntryMultiplier(attacker, 1, includeCharge);
            }
            defense = btlGetActorEntryMultiplier(recipient, 3, includeCharge);
        } else {
            defense = 1.0f;
            bypass = calculationFlags & 1;
            attack = 1.0f;
        }
        status = 1.0f;
        break;
    case 2:
        return 0;
    default:
        defense = 0.0f;
        bypass = calculationFlags & 1;
        attack = 0.0f;
        status = 0.0f;
        break;
    }
    attribute = btlGetActorIndexedSignedValue(attacker, command);
    if (!bypass) {
        automatic = btlGetElementAbilityMultiplier(attacker, attribute);
    } else {
        automatic = 1.0f;
    }
    if (press == 2 && btlCheckSpecialAbility(&attacker->partyRecord, 0x209)) {
        defense *= datAbilityParameters[0x209 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    if (attackMode == 1 && attribute != -1 &&
        datCommandRecords[command].hpType != 8 && datCommandRecords[command].hpType != 10) {
        automatic *= (f32)btlGetAbilityAttributeMultiplierPercent(recipient, attribute) / 100.0f;
    }
    if (attackMode == 0 && !bypass && btlCheckSpecialAbility(&attacker->partyRecord, 0x239)) {
        automatic *= datAbilityParameters[0x239 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    btlBossDebugPrintf("btl:hp[atk=%.2f,def=%.2f,bat=%.2f,auto=%.2f]\n",
                       attack, defense, status, automatic);
    result = sdfCalculateCommandVitalDelta(command, (s32)&attacker->partyRecord, &recipient->partyRecord, maxHits, 1);
    base = result;
    result = (s32)((f32)result * attack * defense * status * automatic);
    if (!(recipient->status.flags & 0x10) && press == 2 && result != 0) {
        result = (s32)((f32)result * (f32)evtRunContext(6, (s32)&attacker->partyRecord, 0, 0, 0) / 100.0f);
    }
    if ((recipient->status.flags & 0x10) && attackMode == 1) {
        result = (s32)((f32)result * 2.0f);
        btlBossDebugPrintf("btl:hp[back]\n");
    }
    limit = datCommandRecords[command].stat36;
    if (result < -limit) {
        s32 randomScale = effMiscRandMod(0, 11) + 95;
        result = -(limit * randomScale) / 100;
    } else if (result > limit) {
        s32 randomScale = effMiscRandMod(0, 11) + 95;
        result = limit * randomScale / 100;
    }
    if (result == 0 && base != 0) {
        result = base < 0 ? -1 : 1;
        btlBossDebugPrintf("btl:hp limit 1\n");
    }
    switch (attackMode) {
    case 0:
        return result > 0 ? result : -1;
    case 1:
        return result < 0 ? -result : -1;
    default:
        return -1;
    }
}

extern s32 sdfCalculateCommandVitalDelta(s32,s32,DatPartyRecord *,u32,u8);
s32 func_001A6118(BtlUnit *attacker, BtlUnit *recipient, s32 command,
                   s32 maxHits, s32 press, s32 attackMode, u32 calculationFlags) {
    f32 attack;
    f32 defense = 1.0f;
    f32 status;
    f32 automatic;
    u32 flags;
    s8 includeCharge = (datCommandRecords[command].flags ^ 1) & 1;
    s32 attribute;
    s32 base;
    s32 result;
    s32 limit;

    switch (datCommandRecords[command].effectType) {
    case 0:
        flags = calculationFlags & 1;
        attack = !flags ? btlGetActorEntryMultiplier(attacker, 0, includeCharge) : 1.0f;
        if (attackMode == 1) {
            defense = btlGetActorEntryMultiplier(recipient, 3, includeCharge);
        }
        {
            s32 ailment = attacker->partyRecord.status & 0x7FFF;
            status = 0.5f;
            if (ailment != 1) {
                status = 1.0f;
                if (ailment == 0x1000) {
                    status = 0.1f;
                }
            }
        }
        break;
    case 1:
        if (attackMode == 1 && datCommandRecords[command].mpType != 8 &&
            datCommandRecords[command].mpType != 10) {
            flags = calculationFlags & 1;
            attack = 1.0f;
            if (!flags) {
                attack = btlGetActorEntryMultiplier(attacker, 1, includeCharge);
            }
            defense = btlGetActorEntryMultiplier(recipient, 3, includeCharge);
        } else {
            defense = 1.0f;
            flags = calculationFlags & 1;
            attack = 1.0f;
        }
        status = 1.0f;
        break;
    case 2:
        return 0;
    default:
        defense = 0.0f;
        flags = calculationFlags & 1;
        attack = 0.0f;
        status = 0.0f;
        break;
    }
    attribute = btlGetActorIndexedSignedValue(attacker, command);
    if (!flags) {
        automatic = btlGetElementAbilityMultiplier(attacker, attribute);
    } else {
        automatic = 1.0f;
    }
    if (press == 2 && btlCheckSpecialAbility(&attacker->partyRecord, 0x209)) {
        defense *= datAbilityParameters[0x209 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    btlBossDebugPrintf("btl:mp[atk=%.2f,def=%.2f,bat=%.2f,auto=%.2f]\n",
                       attack, defense, status, automatic);
    result = sdfCalculateCommandVitalDelta(command, (s32)&attacker->partyRecord, &recipient->partyRecord, maxHits, 2);
    base = result;
    result = (s32)((f32)result * attack * defense * status * automatic);
    if (press == 2 && result != 0) {
        result = (s32)((f32)result * (f32)evtRunContext(6, (s32)&attacker->partyRecord, 0, 0, 0) / 100.0f);
    }
    if ((recipient->status.flags & 0x10) && attackMode == 1) {
        result = (s32)((f32)result * 2.0f);
        btlBossDebugPrintf("btl:mp[back]\n");
    }
    limit = datCommandRecords[command].stat36;
    if (result < -limit) {
        s32 randomScale = effMiscRandMod(0, 11) + 95;
        result = -(limit * randomScale) / 100;
    } else if (result > limit) {
        s32 randomScale = effMiscRandMod(0, 11) + 95;
        result = limit * randomScale / 100;
    }
    if (result == 0 && base != 0) {
        result = base < 0 ? -1 : 1;
        btlBossDebugPrintf("btl:mp limit 1\n");
    }
    switch (attackMode) {
    case 0:
        return result > 0 ? result : -1;
    case 1:
        {
            if (recipient->partyRecord.mp < -result) {
                result = -(s32)recipient->partyRecord.mp;
            }
            return result < 0 ? -result : -1;
        }
    default:
        return -1;
    }
}


s32 btlSumOtherTargetHitAmounts(u8 *action) {
    BtlOperandGroup *result = *(BtlOperandGroup **)(action + 0x80);
    u32 count = btlGetIndexListCount(*(BtlIndexList **)(action + 0x60));
    u32 i;
    u32 j;
    s32 total = 0;

    for (i = 0; i < count; i++, result++) {
        if (result->inactive != 0 || result->reflected != 0) {
            continue;
        }
        switch ((u32)result->kind) {
        case 2:
        case 4:
        case 0x10000:
        case 0x20000:
        case 0x40000:
            break;
        default:
            if (*(BtlUnit **)(action + 0x18) !=
                btlGetIndexListEntry(*(BtlIndexList **)(action + 0x60), i)) {
                for (j = 0; j < result->count; j++) {
                    total += result->entries[j].hpDelta;
                }
            }
            break;
        }
    }
    return total;
}

s32 btlTestActorStatusPredicate(BtlUnit *object) {
    s32 (*predicate)(BtlUnit *) = *(s32 (**)(BtlUnit *))((u8 *)btlGetRuntime() + 0x650);
    if (predicate != 0 && predicate(object) != 0) {
        return 1;
    }
    return (object->partyRecord.status & 0x806) != 0;
}

s32 btlComputeStatusPenaltyFifth(BtlUnit *object) {
    s32 result = 0;
    u16 category = object->partyRecord.status & 0x7FFF;
    switch (category) {
    case 0x80:
    case 0x400:
    case 0x2000:
        result = -object->partyRecord.maxHp / 5;
        break;
    }
    return result;
}

extern u32 btlRollAiBucket(void);

s32 btlRollFearChance(s32 unused, u8 *actor, u32 flags, u32 options) {
    s32 ratio;
    if ((((BtlState *)btlGetRuntime())->unk_1FC & 0x80) != 0) return 0;
    if ((((BtlUnit *)actor)->status.stateFlags & 8) != 0) return 0;
    if ((flags & 1) == 0) return 0;
    if ((((BtlUnit *)actor)->partyRecord.status & 1) != 0) return 0;
    ratio = 0;
    if (options & 2) {
        ratio = 30;
    } else if (options & 4) {
        ratio = 40;
    }
    btlBossDebugPrintf("btl:fear ratio[%d]\n", ratio);
    return (s32)btlRollAiBucket() < ratio;
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
    return (s32)btlRollAiBucket() < threshold;
}

f32 func_001A6958(void) {
    return 1.5f;
}



u8 btlSelectCommandHitCount(BtlUnit *unit, s32 command) {
    BtlState *context = (BtlState *)btlGetRuntime();
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
    if (datCommandSelectors[command].kind == 5 &&
        (unit->status.flags & 0x200) != 0) {
        minimum = ((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].rangeMin;
        maximum = ((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].rangeMax;
    } else {
        minimum = datCommandRecords[command].rangeMin;
        maximum = datCommandRecords[command].rangeMax;
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
        if ((((BtlUnit *)object)->status.flags & 0x400) != 0) {
            return datEnemyRecords[((BtlUnit *)object)->partyRecord.unitId].attackSpacing;
        }
        return 12;
    }
    return 12;
}

extern BtlActionAnimationRecord *datActionAnimationRecords;

extern s32 btlActionAnimationModeRows[];

extern char D_003A1A58[]; /* "btl:delay=%d\n" */

/* Return the action animation's delay, retaining command-zero and party-unit-six cases. */
s32 btlGetActionAnimationDelay(BtlUnit *unit, s32 actionId) {
    s32 *delayTable;
    u32 delayIndex;
    s32 *delay;

    if ((actionId == 0) || (datCommandSelectors[actionId].kind == 5)) {
        if (((unit->status.flags & 0x200) != 0) && (unit->partyRecord.unitId == 6)) {
            return 9;
        }
    }
    if (actionId == 0) {
        return 1;
    }
    delayTable = btlActionAnimationModeRows;
    delayTable++;
    delayIndex = datActionAnimationRecords[actionId].delayIndex;
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
        return datCommandSelectors[id].kind == 2 ? 0x2D : 0;
    }
}


extern const char D_003A1A68[];

extern const char D_003A1A78[];

extern const char D_003A1A88[];

extern const char D_003A1A98[];

/* Sort adjacent same-side targets for the selected animation, or shuffle them. */
void btlSortActorMuzzleDirections(BtlUnit *unit, BtlIndexList *targets, s32 actionId) {
    BtlUnit *pair[2];
    f32 muzzle[2][4];
    u32 randomIndices[2];
    u32 count;
    u32 mode;
    u32 i;
    u32 last;
    s32 sorted;

    count = btlGetIndexListCount(targets);
    if (count < 2) {
        return;
    }
    if ((unit->status.flags & 0x200) != 0 &&
        datCommandSelectors[actionId].kind == 5) {
        mode = unit->partyRecord.unitId == 6 ? 2 : 0;
    } else {
        u32 delayIndex = datActionAnimationRecords[actionId].delayIndex;
        mode = ((BtlAnimationModeRow *)btlActionAnimationModeRows)[delayIndex].mode;
    }
    switch (mode) {
    case 0:
        btlBossDebugPrintf(D_003A1A68);
        return;
    case 1:
        btlBossDebugPrintf(D_003A1A78);
        break;
    case 2:
        btlBossDebugPrintf(D_003A1A88);
        break;
    case 3:
        btlBossDebugPrintf(D_003A1A98);
        break;
    }
    if (mode != 3) {
        last = count - 1;
        do {
            sorted = 1;
            for (i = 0; i < last; i++) {
                pair[0] = btlGetIndexListEntry(targets, i);
                pair[1] = btlGetIndexListEntry(targets, i + 1);
                btlUnitGetMuzzlePosVU(pair[0]);
                VU0_STORE_VF(vf10, muzzle[0]);
                btlUnitGetMuzzlePosVU(pair[1]);
                VU0_STORE_VF(vf10, muzzle[1]);
                if ((pair[0]->status.flags & 0x400) && (pair[1]->status.flags & 0x400)) {
                    if (mode == 1) {
                        if (muzzle[0][0] > muzzle[1][0]) {
                            btlSwapIndexListEntries(targets, i, i + 1);
                            sorted = 0;
                        }
                    } else if (muzzle[0][0] < muzzle[1][0]) {
                        btlSwapIndexListEntries(targets, i, i + 1);
                        sorted = 0;
                    }
                } else if ((pair[0]->status.flags & 0x200) && (pair[1]->status.flags & 0x200)) {
                    if (mode == 1) {
                        if (muzzle[0][0] < muzzle[1][0]) {
                            btlSwapIndexListEntries(targets, i, i + 1);
                            sorted = 0;
                        }
                    } else if (muzzle[0][0] > muzzle[1][0]) {
                        btlSwapIndexListEntries(targets, i, i + 1);
                        sorted = 0;
                    }
                }
            }
        } while (sorted == 0);
    } else {
        for (i = 0; i < 13; i++) {
            randomIndices[0] = effMiscRandMod(0, count);
            randomIndices[1] = effMiscRandMod(0, count);
            if (randomIndices[0] != randomIndices[1]) {
                btlSwapIndexListEntries(targets, randomIndices[0], randomIndices[1]);
            }
        }
    }
}

/* Share the policy rejection; see docs/idioms.md, "Shared rejection blocks: battle command availability". */
s32 btlShouldDistributeRandomTargetHits(BtlUnit *unit, s32 command) {
    BtlState *runtime = btlGetRuntime();

    if (runtime->commandAvailabilityOverride != NULL) {
        if (runtime->commandAvailabilityOverride(unit, command) == 0) {
            return 0;
        }
    }
    if ((unit->status.flags & 0x200) && datCommandSelectors[command].kind == 5) {
        if (((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].pad0B == 1) {
            return 1;
        }
    } else if (datCommandRecords[command].distributeRandomTargetHits == 1) {
        return 1;
    }
    return 0;
}

void btlDistributeRandomTargetHits(BtlUnit *unit, BtlIndexList *targets,
                   BtlOperandGroup *results, s32 command) {
    u8 selected[13];
    BtlIndexList *copy;
    void *previous;
    void *entry;
    u32 targetCount;
    u32 maximumHits;
    u32 hitCount;
    u32 i;
    u32 index;
    s32 resultIndex;
    s32 allowConsecutiveHits;

    targetCount = btlGetIndexListCount(targets);
    allowConsecutiveHits = targetCount < 2;
    maximumHits = targetCount + effMiscRandMod(0, 2);
    if (allowConsecutiveHits) {
        results[0].count = maximumHits;
        return;
    }

    previous = NULL;
    copy = btlAllocateIndexList(ARRAY_COUNT(selected));
    i = 0;
    btlCopyIndexList(copy, targets);
    btlClearIndexList(targets);
    hitCount = btlSelectCommandHitCount(unit, command);
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
            results[resultIndex].count = 1;
        } else {
            resultIndex = btlFindListIndex(targets, entry);
            if (results[resultIndex].count < maximumHits) {
                results[resultIndex].count++;
            }
        }
        i++;
    }
    btlFreeIndexList(copy);
}

INCLUDE_RODATA(const s32, "game/code_001A5C40", D_003A1A40);

INCLUDE_RODATA(const s32, "game/code_001A5C40", D_003A1A58);

INCLUDE_RODATA(const s32, "game/code_001A5C40", D_003A1A68);

INCLUDE_RODATA(const s32, "game/code_001A5C40", D_003A1A78);

INCLUDE_RODATA(const s32, "game/code_001A5C40", D_003A1A88);

INCLUDE_RODATA(const s32, "game/code_001A5C40", D_003A1A98);

