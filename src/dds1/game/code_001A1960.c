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

typedef struct EntryPair {
    s16 first;
    s16 second;
    s16 initialValue;
    s16 countdown;
} EntryPair;

extern EntryPair D_003583D0[];

extern u32 datComputeSkillBoostedMaxHp(DatPartyRecord *);

extern u32 datComputeSkillBoostedMaxMp(DatPartyRecord *);

extern void datClearUnitStatusBits(DatPartyRecord *record, s32 mask);

extern s32 datGetClampedProfileAdjustedStat(DatPartyRecord *, s32);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

extern u8 D_003583A0[];

extern void *D_00358408[];

extern void *D_00358450[];

extern s32 mdlFlagTest(s32);

extern DatEnemyRecord *datEnemyRecords;

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

extern s32 datUnitHasSkill(DatPartyRecord *, s32);

extern s32 datCalculateCommandBaseValue(DatPartyRecord *, s32);

extern s32 btlGetRuntime(void);

extern s32 D_003BAA14;

extern s32 D_003BAA28;

extern s32 D_003BAA10;

extern s32 D_003BAA20;

extern s32 D_003BAA30;

extern s32 datRosterDetails;

extern s32 fldCountSceneSlots(void);

extern s32 effMiscRandMod(s32, s32);

extern char D_003A1A40[];

typedef struct BattleFearState {
    u8 pad_000[0x1FC];
    u32 flags;
} BattleFearState;

extern s32 dds3FindEntryIndex(s32);

extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);

extern s8 effSharedRandomState[];

void func_001A1960(DatPartyRecord *record, s32 mask) {
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
void func_001A1990(DatPartyRecord *entry, s32 index) {
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
    entry->unk20 = count;
    entry->maxHp = datEnemyRecords[index].maxHp;
    entry->hp = datEnemyRecords[index].hp;
    entry->maxMp = datEnemyRecords[index].maxMp;
    entry->mp = datEnemyRecords[index].mp;
}

DatPartyRecord *btlGetActorEntryData(BtlUnit *actor) {
    DatPartyRecord *entry;

    if ((actor->status.flags & 0x400) == 0) {
        entry = btlGetIndexedPartyEntryRecord(actor->unk2C4);
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
    DatPartyRecord *dst = btlGetIndexedPartyEntryRecord(actor->unk2C4);
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
    dst->unk18C = src->unk18C;
    dst->unk18E = src->unk18E;
    dst->unk190 = src->unk190;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

s32 btlFindPartyEntryIndexForActor(s32 arg0) {
    return dds3FindEntryIndex(((BtlUnit *)arg0)->partyRecord.unitId);
}

void func_001A1CD0(void) {
}

BtlUnit *btlFindActiveActorByKind(s32 index) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    for (; node != 0; node = node->next) {
        u32 flags = node->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if (index == node->unk2C4) {
                    return node;
                }
            }
        }
    }
    return 0;
}

extern void btlCopyUnitStats(s32 actorAddress, s32 recordAddress);

/* Insert a complete party record before actors with a larger priority. */
void func_001A1D48(BtlUnit *unit, u8 sourceIndex, u8 priority) {
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
            btlFindActiveActorByKind(i + 1)->unk2C4 = i;
        }
    }
    for (i = 4; i > insertion; i--) {
        memcpy(&datGameState->party[i], &datGameState->party[i - 1], sizeof(saved));
        if (datGameState->party[i - 1].flags & 2) {
            btlFindActiveActorByKind(i - 1)->unk2C4 = i;
        }
    }
    memcpy(&datGameState->party[i], &saved, sizeof(saved));
    btlCopyUnitStats((s32)unit, (s32)&saved);
    unit->partyRecord.flags |= 2;
    datGameState->party[i].flags |= 2;
    unit->unk2C4 = i;
    func_001A1CD0();
    btlBossDebugPrintf("btl:party in %d->%d[%d]\n", sourceIndex, i, saved.unitId);
}

/* Move a departing actor behind the remaining occupied party records. */
void func_001A2258(BtlUnit *unit) {
    DatPartyRecord saved;
    DatGameState *scanState = datGameState;
    s32 originalIndex;
    s32 index;

    originalIndex = unit->unk2C4;
    memcpy(&saved, &datGameState->party[originalIndex], sizeof(saved));
    index = originalIndex;
    if (index < 4 && (u16)(scanState->party[index + 1].flags & 1)) {
        do {
            memcpy(&datGameState->party[index], &datGameState->party[index + 1], sizeof(saved));
            if (datGameState->party[index + 1].flags & 2) {
                btlFindActiveActorByKind(index + 1)->unk2C4 = index;
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
    unit->unk2C4 = 6;
    func_001A1CD0();
    btlBossDebugPrintf("btl:party out %d->%d[%d]\n", originalIndex, index,
                       saved.unitId);
}

/* Swap complete records while keeping the actor in its original party slot. */
void func_001A2608(BtlUnit *actor, u8 targetIndex) {
    DatPartyRecord previous;

    if (actor->unk2C4 != targetIndex) {
        memcpy(&previous, &datGameState->party[actor->unk2C4], sizeof(previous));
        memcpy(&datGameState->party[actor->unk2C4], &datGameState->party[targetIndex], sizeof(previous));
        memcpy(&datGameState->party[targetIndex], &previous, sizeof(previous));
        btlCopyUnitStats((s32)actor, (s32)&datGameState->party[actor->unk2C4]);
        actor->partyRecord.flags |= 2;
        datGameState->party[actor->unk2C4].flags |= 2;
        datGameState->party[targetIndex].flags &= ~2;
        func_001A1CD0();
        btlBossDebugPrintf("btl:party change %d<->%d[%d<->%d]\n",
            actor->unk2C4, targetIndex, previous.unitId, actor->partyRecord.unitId);
    }
}

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record) {
    if ((record->flags & 4) != 0) {
        return 0;
    }
    return datEnemyRecords[record->unitId].flags;
}

void func_001A29B8(DatPartyRecord *unit, s32 statIndex) {
    datGetClampedProfileAdjustedStat(unit, statIndex);
}

s32 func_001A29D0(DatPartyRecord *unit, s32 statIndex) {
    return datGetStatWithStatusOverride(unit, statIndex);
}

s32 btlApplyCommandAbilityMultiplier(DatPartyRecord *arg0, s32 arg1) {
    u32 value = datCalculateCommandBaseValue(arg0, arg1);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (datCommandRecords[arg1].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if (btlCheckSpecialAbility(arg0, 0x234)) {
            scale = datAbilityParameters[0x234 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if (btlCheckSpecialAbility(arg0, 0x235)) {
            scale = datAbilityParameters[0x235 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    value = (s32)((f32)value * scale);
    return value == 0 ? 1 : value;
}

s8 func_001A2AE8(s32 command) {
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
        if (fldCountSceneSlots() < (u32)func_001A2AE8(command)) {
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

s32 func_001A2DE8(BtlUnit *base, BtlUnit *first, BtlUnit *second,
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
        return datEnemyRecords[unit->partyRecord.unitId].unk46;
    }
    return datCommandSelectors[index].stat;
}

extern s32 btlResolveUnitValueWithOverride(s32, s32);

s32 func_001A2F50(BtlUnit *battler, s32 element) {
    return btlResolveUnitValueWithOverride((s32)&battler->partyRecord, element);
}

extern s32 datGetEffectiveAffinity(DatPartyRecord *, s32);

s32 btlResolveUnitValueWithOverride(s32 object, s32 value) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x670);
    if (handler != 0) {
        s32 result = handler(object, value);
        if (result != -1) {
            return result;
        }
    }
    return datGetEffectiveAffinity((DatPartyRecord *)object, value);
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
    u16 item = datItemSkillRecords[index].commandIndex;
    btlBossDebugPrintf(D_003A1788, index, item);
    return item;
}

s32 btlGetActorBedAssetIdFromIndex(s32 arg0) {
    return datItemSkillRecords[arg0].commandIndex;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1788);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A30F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3360);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, BtlIndexList *arg1);

s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, BtlIndexList *targets) {
    BtlTask *action = (BtlTask *)arg0;
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

s32 func_001A3638(void) {
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
            if ((flags & 0x200) == 0) {
                goto next_unit;
            }
            if ((flags & 1) != 0) {
                goto next_unit;
            }
            eligible[count] = unit;
            unit->status.flags = flags & ~0x100;
            count++;
next_unit:
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

extern s8 effSharedRandomState[];

s32 btlSelectActorAction(s32 object) {
    s32 result = func_001A4130(object, 1);
    if (result == 0) {
        result = (effMiscRand(effSharedRandomState) & 1) != 0 ? 2 : 7;
    }
    return result;
}

s32 func_001A4130(s32 actorAddress, s32 mode) {
    BtlUnit *actor = (BtlUnit *)actorAddress;
    DatEnemyRecord *enemy = &datEnemyRecords[actor->partyRecord.unitId];
    s32 action = 0;
    u16 i;

    extern u32 btlRollAiBucket(void);

    if (mode != 1 && enemy->overrideAction != 0 && enemy->overrideFlag != 0 &&
        mdlFlagTest(enemy->overrideFlag) != 0) {
        if ((u8)btlRollAiBucket() < enemy->overrideChance) {
            action = enemy->overrideAction;
        }
    }
    for (i = 0; i < 2 && action == 0; i++) {
        if (enemy->unk3E[i] != 0 && (u8)btlRollAiBucket() < enemy->actionChances[i]) {
            action = enemy->unk3E[i];
        }
    }
    return action;
}

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

extern s32 func_001A9488(u32);

extern s32 btlCalculateEnemyExperienceReward(u8 *, u8 *);

extern s32 btlGetEnemyMoney(u8 *, u8 *);

extern char D_003A17F8[];

extern char D_003A1818[];

extern char D_003A1830[];

extern char D_003A1848[];

void btlAccumulateEnemyDefeatRewards(BtlUnit *enemy) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    DatEnemyRecord *record = &datEnemyRecords[enemy->partyRecord.unitId];
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
    item = func_001A4130((s32)enemy, 0);
    if (item != 0) {
        func_001A4240(item);
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
        return candidates[effMiscRandMod(0, count)];
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

f32 func_001A47F0(BtlTask *task) {
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

f32 func_001A5030(BtlUnit *actor, s32 attr) {
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

u32 btlHasSpecialAbilityWhenArgumentUnset(BtlUnit *arg0, s64 arg1) {
    if (arg1 == 0 && btlCheckSpecialAbility(&arg0->partyRecord, 0x254) != 0) {
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

s32 sndGetResourceForIndex(s32 index) {
    s8 resource = datCommandSelectors[index].stat;
    if (resource < 0) {
        return 0;
    }
    return (s32)D_00358408[resource];
}

/* The label resource index is the copied party record's unit ID. */
void *btlGetIndexedUiResource(BtlUnit *unit) {
    return D_00358450[unit->partyRecord.unitId];
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

extern BtlActorStateSnapshot D_003D73F0[3];

extern s32 func_003003F0(const char *, ...);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A18F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1908);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1918);

void func_001A5690(void) {
    s32 count = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;

    memset(D_003D73F0, 0, sizeof(D_003D73F0));
    for (unit = battle->units; unit != NULL; unit = unit->next) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                D_003D73F0[count].flags = unit->status.flags;
                D_003D73F0[count].unitId = unit->partyRecord.unitId;
                memcpy(D_003D73F0[count].entrySlots, unit->entrySlots,
                       sizeof(D_003D73F0[count].entrySlots));
                D_003D73F0[count].status = unit->partyRecord.status & 0xEFF9;
                count++;
                func_003003F0("btl:state push[%d:%X]\n", count,
                              unit->partyRecord.unitId);
            }
        }
    }
}

void func_001A57A0(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 slot;
    BtlUnit *unit;
    u16 status;

    for (slot = 0; slot < 3; slot++) {
        if (D_003D73F0[slot].unitId == 0) {
            continue;
        }
        for (unit = battle->units; unit != NULL; unit = unit->next) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x200) {
                    if (unit->partyRecord.unitId == D_003D73F0[slot].unitId) {
                        if (D_003D73F0[slot].flags & 0x1000) {
                            unit->status.flags |= 0x1000;
                            unit->partyRecord.flags |= 0x1000;
                        } else {
                            unit->status.flags &= ~0x1000;
                            unit->partyRecord.flags &= ~0x1000;
                        }
                        memcpy(unit->entrySlots, D_003D73F0[slot].entrySlots,
                               sizeof(D_003D73F0[slot].entrySlots));
                        status = D_003D73F0[slot].status;
                        unit->partyRecord.status = status;
                        if ((status & 0x7FFF) == 0x4000) {
                            unit->status.flags |= 0x20;
                            unit->partyRecord.hp = 0;
                        }
                        func_003003F0("btl:state pos[%d:%X]\n", slot, unit->partyRecord.unitId);
                        break;
                    }
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5958);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5C40);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6118);

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
    s32 (*predicate)(BtlUnit *) = *(s32 (**)(BtlUnit *))(btlGetRuntime() + 0x650);
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
            return datEnemyRecords[((BtlUnit *)object)->partyRecord.unitId].unk48;
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

    if ((actionId == 0) || (datCommandSelectors[actionId].kind == 5)) {
        if (((unit->status.flags & 0x200) != 0) && (unit->partyRecord.unitId == 6)) {
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
        return datCommandSelectors[id].kind == 2 ? 0x2D : 0;
    }
}

extern void btlUnitGetMuzzlePosVU(void *);

typedef struct BtlAnimationModeRow {
    u8 mode;
    u8 pad01[3];
    s32 delay;
} BtlAnimationModeRow;

extern const char D_003A1A68[];

extern const char D_003A1A78[];

extern const char D_003A1A88[];

extern const char D_003A1A98[];

/* Sort adjacent same-side targets for the selected animation, or shuffle them. */
void func_001A6BE0(BtlUnit *unit, BtlIndexList *targets, s32 actionId) {
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
        u32 delayIndex = ((BtlActionAnimationRecord *)datActionAnimationRecords)[actionId].delayIndex;
        mode = ((BtlAnimationModeRow *)D_00358490)[delayIndex].mode;
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

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6EC0);

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
    u32 resultIndex;
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

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A40);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A58);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A68);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A78);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A88);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1A98);

