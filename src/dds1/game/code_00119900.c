#include "common.h"
#include "scr.h"
#include "dat_state.h"
#include "sdf.h"
#include "btl_action.h"
#include "dat_command.h"

enum {
    PTY_ACTIVE_ROSTER_COUNT = 5,
    PTY_RECOVERY_DELTA = 9999,
    PTY_MAX_LEVEL = 99,
    PTY_LEVEL_LIMIT = 100
};

enum {
    EVT_DEFAULT_STAT_VALUE = 100,
    EVT_ROSTER_DETAIL_KIND = 5,
    EVT_DEFAULT_GROUP_MASK = 4,
    EVT_ALTERNATE_GROUP_MASK = 0x20,
    EVT_CONTEXT_BYTES = 0x18,
    EVT_RESULT_WRITTEN_FLAG = 1,
    EVT_RESULT_RESET_MASK = 0xFFFE
};

enum {
    PTY_ITEM_FLAG_FIRST = 0x80,
    PTY_ITEM_FLAG_COUNT = 0x20,
    PTY_ITEM_MODEL_FLAG_BASE = 0x980,
    PTY_ITEM_MAX_QUANTITY = 99,
    PTY_ITEM_QUANTITY_LIMIT = 100,
    PTY_ITEM_SINGLE_MAX = 1,
    PTY_ITEM_SINGLE_LIMIT = 2
};

enum {
    BTL_SCENE_RECORD_BYTES = 0x28,
    BTL_SCENE_ENEMY_SLOT_COUNT = 11,
    BTL_ENEMY_RECORD_BYTES = 0x4C,
    BTL_ENEMY_LEVEL_OFFSET = 5
};

extern u8 D_0032AF70[];
extern s32 func_0011B158(s32, s32, u8);


extern s32 datRosterDetails;
extern DatEnemyRecord *datEnemyRecords;
extern s32 datItemSkillRecords;
extern s32 D_003BAAB8;
extern DatPartyRecord *D_003BAA04;

typedef struct TableEntry32 {
    u16 value; /* 0x0: copied to active roster entry */
    u16 unk2; /* 0x2 */
} TableEntry32;

typedef struct Entry4 {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 pad3; /* 0x3 */
} Entry4;


/* Script-visible data tables indexed by roster number or event parameter. */
typedef struct RosterDetail {
    s16 baseValue;     /* 0x00 */
    u8 lowValue;       /* 0x02 */
    u8 highValue;      /* 0x03 */
    f32 scale;         /* 0x04 */
    u8 pad8[0xC];
} RosterDetail; /* 0x14 */

typedef struct EventIndexRecord {
    u8 pad00[2];
    u16 index; /* 0x02 */
    u8 pad04[4];
} EventIndexRecord; /* stride 0x08 */


typedef struct EvtScriptContext {
    u16 stateFlags;      /* 0x00 */
    u16 pad02;
    s32 third;           /* 0x04 */
    s32 first;           /* 0x08 */
    s32 second;          /* 0x0C */
    s32 result;          /* 0x10 */
    u16 options;         /* 0x14 */
    u8 pad16[2];
} EvtScriptContext;

extern TableEntry32 D_0032AEA8[];
extern Entry4 D_0032AEE8[];

extern ScrData *evtWorkScriptTask;

extern s32 D_003C2E70[];
extern s32 D_003C2E74[];
extern s32 D_003C2E78[];
extern s32 D_003C2E7C[];
extern s32 D_003C2E80[];

extern s32 scrReadIntParameter(s32 idx);
extern s32 scrSetIntegerReturnValue();
extern void scrSetFloatReturnValue(f32 value);
extern DatPartyRecord *dds3FindEntry(s32 rosterIndex);
extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);
extern void ptyMergeStockSkills(DatPartyRecord *unit);
extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);
extern u8 btlIsRuntimeAllocated(void);
extern f32 func_001A4598(void);
extern u32 func_001A4630(void);
extern s32 btlAverageAllMaximumForMask(u32 arg0);
extern s32 btlAverageAllCurrentForMask(u32 arg0);
extern s32 func_001A94A0(u32 arg0);
extern void *memset(void *dst, s32 c, u32 n);
extern s32 mdlFlagTest(s32 flagIndex);
extern void mdlFlagSet(s32 flagIndex);
extern s32 effMiscRandMod(u32 stream, u32 modulus);

/* Flag-range items set their model flag regardless of quantityDelta.
 * Other items add the delta to their byte quantity and clamp it. */
void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta) {
    s32 quantity;

    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        mdlFlagSet(itemId + PTY_ITEM_MODEL_FLAG_BASE);
        return;
    }
    quantity = datGameState->inventory.counts[itemId];
    quantity += quantityDelta;
    if (quantity < 0) {
        quantity = 0;
    }

    if (itemId >= 0xA0) {
        if (quantity >= PTY_ITEM_QUANTITY_LIMIT) {
            quantity = PTY_ITEM_MAX_QUANTITY;
        }
    } else if (itemId >= PTY_ITEM_FLAG_FIRST) {
        if (quantity >= PTY_ITEM_SINGLE_LIMIT) {
            quantity = PTY_ITEM_SINGLE_MAX;
        }
    } else if (itemId >= 0x60) {
        if (quantity >= PTY_ITEM_QUANTITY_LIMIT) {
            quantity = PTY_ITEM_MAX_QUANTITY;
        }
    } else {
        quantity = quantity < PTY_ITEM_QUANTITY_LIMIT ? quantity : PTY_ITEM_MAX_QUANTITY;
    }
    datGameState->inventory.counts[itemId] = quantity;
}

/* Flag-range items test their model flag and ignore minimumQuantity.
 * Other items require at least the requested byte quantity. */
s32 evtCheckValueThreshold(s32 itemId, s32 minimumQuantity) {
    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        return mdlFlagTest(itemId + PTY_ITEM_MODEL_FLAG_BASE) != 0;
    }
    if (datGameState->inventory.counts[itemId] < minimumQuantity) {
        return 0;
    }
    return 1;
}

/* Flag-range items have one slot; every quantity range caps at 99. */
s32 func_00119A00(s32 itemId) {
    s32 quantity = datGameState->inventory.counts[itemId];

    if (itemId >= 0xA0) {
        if (quantity >= PTY_ITEM_MAX_QUANTITY) {
            return 1;
        }
    } else if (itemId >= PTY_ITEM_FLAG_FIRST) {
        if (quantity > 0) {
            return 1;
        }
    } else if (itemId >= 0x60) {
        if (quantity >= PTY_ITEM_MAX_QUANTITY) {
            return 1;
        }
    } else if (quantity >= PTY_ITEM_MAX_QUANTITY) {
        return 1;
    }
    return 0;
}

u8 evtGetFlaggedRosterValue(s32 entryAddress) {
    DatPartyRecord *entry = (DatPartyRecord *)entryAddress;
    if ((entry->flags & 0x20) == 0) {
        return 0;
    }
    return datEnemyRecords[entry->unitId].pad04;
}

s32 dds3FindEntryIndex(s32 rosterIndex) {
    DatPartyRecord *entry = datGameState->party;
    s32 slotIndex = 0;

    do {
        if (entry->flags & 1) {
            if (entry->unitId == rosterIndex) {
                return slotIndex;
            }
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return -1;
}

/* Read a signed byte relative to the first roster entry's stat-byte base;
 * the caller supplies a byte offset, not a whole-entry index. */
s8 ptyReadSignedRosterStatByte(s32 byteOffset) {
    return datGameState->party[0].baseStats[byteOffset];
}

extern void sdfRaisePackedChannelValue(DatPartyRecord *, u32);

/* Event penalties affect living roster slots, then optionally raise a status channel. */
void func_00119B08(s32 mode) {
    s32 nextHp, loss, slotIndex;
    if (mode == 1 || mode == 4 || mode == 5 || mode == 6) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->hp != 0) {
                nextHp = entry->hp;
                loss = nextHp / 10;
                if (loss == 0) loss = 1;
                nextHp -= loss;
                if (nextHp <= 0) nextHp = 1;
                entry->hp = nextHp;
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (mode == 2) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->hp != 0) {
                nextHp = entry->hp;
                loss = (u32)nextHp / 2;
                if (loss == 0) loss = 1;
                nextHp -= loss;
                if (nextHp <= 0) nextHp = 1;
                entry->hp = nextHp;
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (mode == 3) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->hp != 0) entry->hp = 1;
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (mode >= 4 && mode <= 6) {
        slotIndex = 0;
        do {
            if (datGameState->party[slotIndex].hp != 0) {
                if (mode == 4) sdfRaisePackedChannelValue(&datGameState->party[slotIndex], 0x80);
                if (mode == 5) sdfRaisePackedChannelValue(&datGameState->party[slotIndex], 0x40);
                if (mode == 6) sdfRaisePackedChannelValue(&datGameState->party[slotIndex], 0x10);
            }
            slotIndex++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
}


/* Apply field HP attrition without allowing a living roster entry to reach zero. */
void func_00119CF0(s32 mode) {
    s32 nextHp, loss, slotIndex;
    if (mode == 1) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if ((entry->flags & 2) && (entry->status & 0x80)) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = nextHp * 3 / 100;
                    if (loss == 0) loss = 1;
                    nextHp -= loss;
                    if (nextHp <= 0) nextHp = 1;
                    entry->hp = nextHp;
                }
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (mode == 2) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->flags & 2) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = nextHp * 3 / 100;
                    if (loss == 0) loss = 1;
                    nextHp -= loss;
                    if (nextHp <= 0) nextHp = 1;
                    entry->hp = nextHp;
                }
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
}


/* Recover HP/MP in each occupied roster slot and preserve only status bit 15. */
void ptyRecoverAllUnits(void) {
    s32 index = 0;

    do {
        DatPartyRecord *entry = &datGameState->party[index];
        if (entry->flags & 1) {
            datAdjustCurrentHp(entry, PTY_RECOVERY_DELTA);
            datAdjustCurrentMp(entry, PTY_RECOVERY_DELTA);
            entry->status &= 0x8000;
        }
        index++;
    } while (index < PTY_ACTIVE_ROSTER_COUNT);
}

/* Test occupied entries with nonzero HP; mode 1 additionally requires flag bit 1. */
s32 ptyAnyUnitFlagMatch(u32 statusMask, s32 flagMode) {
    DatPartyRecord *entry = datGameState->party;
    s32 slotIndex = 0;

    do {
        if (entry->flags & 1) {
            if (entry->hp != 0) {
                if (flagMode != 1 || (entry->flags & 2)) {
                    if (entry->status & statusMask) {
                        return 1;
                    }
                }
            }
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

void evtAdvanceCounterValue(s32 counterAddress, s32 increment) {
    *(s32 *)(counterAddress + 0x10) = *(s32 *)(counterAddress + 0x10) + increment;
}

extern s32 datUnitHasSkill(DatPartyRecord *, s32);

/* Apply the owned skill's positive HP/MP recovery rate; unsupported skills do nothing. */
void ptyApplySkillRecovery(DatPartyRecord *entry, u32 skillId) {
    f32 recoveryRate;
    s32 hpRecovery;
    s32 mpRecovery;

    if (datUnitHasSkill(entry, skillId) == 0) {
        return;
    }
    hpRecovery = 0;
    mpRecovery = 0;
    recoveryRate = datAbilityParameters[skillId - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    switch (skillId) {
    case 0x22B:
        if (recoveryRate > 0.0f) {
            hpRecovery = (s32)((f32)entry->maxHp * recoveryRate);
            mpRecovery = (s32)((f32)entry->maxMp * recoveryRate);
        }
        break;
    case 0x22A:
    case 0x22C:
    case 0x250:
        if (recoveryRate > 0.0f) {
            mpRecovery = (s32)((f32)entry->maxMp * recoveryRate);
        }
        break;
    }
    if (hpRecovery > 0) {
        datAdjustCurrentHp(entry, hpRecovery);
    }
    if (mpRecovery > 0) {
        datAdjustCurrentMp(entry, mpRecovery);
    }
}

/* Apply the two selected recovery skills to occupied entries carrying flag bit 1. */
void evtUpdateFlaggedStats(void) {
    s32 slotIndex = 0;

    do {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if (entry->flags & 1) {
            if (entry->flags & 2) {
                ptyApplySkillRecovery(entry, 0x22C);
                ptyApplySkillRecovery(entry, 0x250);
            }
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

/* Return whether an occupied, flag-bit-1 entry owns the requested skill. */
s32 evtHasMatchingFlaggedEntry(s32 skillId) {
    s32 slotIndex = 0;
    do {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if ((entry->flags & 1) && (entry->flags & 2)) {
            if (datUnitHasSkill(entry, skillId)) {
                return 1;
            }
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

/* Resolve the command's unmodified integer value for this entry.
 * Mode one scales max HP and floors the result at one unless the entry is
 * enemy-flagged; mode two returns a fixed value subject to the enemy flag gate.
 * value is intentionally reused: incoming command ID, then resolved result. */
s32 datCalculateCommandBaseValue(DatPartyRecord *entry, s32 value) {
    s32 commandId = value;
    DatCommandRecord *commands = datCommandRecords;

    value = 0;
    switch (commands[commandId].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if (entry->flags & 0x20) {
            return 0;
        }
        value = entry->maxHp * commands[commandId].costPercentage / 100 +
                commands[commandId].costBase;
        if (value <= 0) {
            value = 1;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if ((entry->flags & 0x20) &&
            (datEnemyRecords[entry->unitId].flags & 0x10)) {
            return 0;
        }
        value = commands[commandId].costPercentage;
        break;
    }
    return value;
}

void ptyInitRuntime(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        memset(&datGameState->party[i], 0, sizeof(DatPartyRecord));
        datGameState->partyOrder[i] = i;
    }
    datGameState->partyCount = 0;
    datGameState->party[0] = D_003BAA04[1];
    datGameState->party[1] = D_003BAA04[3];
    datGameState->party[2] = D_003BAA04[4];
    datGameState->partyCount = 3;
    for (i = 0; i < 5; i++) {
        datGameState->party[i].menuValue = 0;
    }
    datGameState->header.currency = 0x7D0;
    for (i = 0; i < 192; i++) {
        datGameState->inventory.counts[i] = 0;
    }
    for (i = 0; i < 16; i++) {
        memset(&datGameState->templates[i], 0, sizeof(DatPartyRecord));
    }
}

u16 evtGetIndexedEventRecordId(s32 tableIndex) {
    return ((EventIndexRecord *)datItemSkillRecords)[tableIndex].index;
}

/* Read the entry's level, capped at the script-visible maximum. */
u16 dds3Clamp99(s32 entryAddress) {
    s32 level = ((DatPartyRecord *)entryAddress)->level;

    return level < PTY_LEVEL_LIMIT ? level : PTY_MAX_LEVEL;
}

/* Find the first occupied slot with this roster identifier, or return NULL. */
DatPartyRecord *dds3FindEntry(s32 rosterIndex) {
    DatPartyRecord *entry = datGameState->party;
    s32 slotIndex = 0;

    do {
        if (entry->unitId != rosterIndex) {
            slotIndex++;
        } else {
            if (entry->flags & 1) {
                return entry;
            }
            slotIndex++;
        }
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_00119900", ptyRebalanceFrontline);

u8 ptyIsRosterEntryPresent(s32 rosterIndex) {
    return dds3FindEntry(rosterIndex) != 0;
}

/* Return the highest occupied-slot level, or zero when no slot is occupied. */
s32 dds3EntryMax(void) {
    DatPartyRecord *entry = datGameState->party;
    s32 maximumLevel = 0;
    s32 remaining = 4;

    do {
        if (entry->flags & 1) {
            if (maximumLevel < entry->level) {
                maximumLevel = entry->level;
            }
        }
        entry++;
        remaining--;
    } while (remaining >= 0);
    return maximumLevel;
}

/* Ceiling average of occupied-slot levels; native code assumes a nonempty roster. */
s32 ptyGetAverageLevel(void) {
    DatPartyRecord *entry = datGameState->party;
    s32 levelSum = 0;
    s32 activeCount = 0;
    s32 remaining = 4;

    do {
        if (entry->flags & 1) {
            activeCount++;
            levelSum += entry->level;
        }
        entry++;
        remaining--;
    } while (remaining >= 0);
    return (levelSum + activeCount - 1) / activeCount;
}

INCLUDE_ASM(const s32, "game/code_00119900", ptyAddUnit);

INCLUDE_ASM(const s32, "game/code_00119900", ptyRemoveUnit);

u32 func_0011B140(void) {
    return 0;
}

u32 func_0011B148(void) {
    return 1;
}

u32 func_0011B150(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B158);

/* Try the six stored orders; success requires exactly the non-sentinel requirement count. */
s32 ptyMatchAffinityPermutation(s32 *actors, s32 affinity) {
    s32 *requirementCursor = datAffinityRecords[affinity - DAT_AFFINITY_FIRST_COMMAND].requirements;
    u32 i;
    s32 requiredCount = 0;
    u32 orderIndex;
    s32 orderOffset;
    s32 matchedCount;
    u8 *orderCursor;
    s32 *actorCursor;
    s32 actorValue;

    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    for (orderIndex = 0, orderOffset = 0; orderIndex < 6; orderIndex++, orderOffset += 3) {
        matchedCount = 0;
        for (i = 0, orderCursor = D_0032AF70 + orderOffset, actorCursor = actors; i < 3; i++, orderCursor++) {
            actorValue = *actorCursor++;
            if (actorValue != 0 && func_0011B158(actorValue, affinity, *orderCursor) != 0) {
                matchedCount++;
            }
        }
        if (requiredCount == matchedCount) {
            return 1;
        }
    }
    return 0;
}

void evtCopyRosterTableValue(DatPartyRecord *entry) {
    entry->menuValue = D_0032AEA8[entry->unitId].value;
}

/* Copy table values only for occupied slots whose roster identifier is below 16. */
void evtUpdateFlaggedEntries(void) {
    s32 slotIndex = 0;

    do {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if (entry->flags & 1) {
            s32 rosterIndex = 0;
            do {
                if (entry->unitId == rosterIndex) {
                    evtCopyRosterTableValue(entry);
                }
                rosterIndex++;
            } while (rosterIndex < 16);
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

/* Apply the two configured count/flag updates; a zero index disables its record. */
void dds3ForEachEntry(void) {
    Entry4 *updates = D_0032AEE8;
    u32 updateIndex = 0;

    do {
        u16 valueIndex = updates->unk0;
        u8 delta = updates->unk2;

        updates++;
        if (valueIndex != 0) {
            ptyAdjustItemQuantity(valueIndex, delta);
        }
        updateIndex++;
    } while (updateIndex < 2);
}

extern u16 D_0032AEF0[16][4];
extern void scrClearFlags(DatPartyRecord *unit);
extern s32 scrSetFlag(DatPartyRecord *unit, u16 skill);
extern void scrSetSecondaryScriptFlag(DatPartyRecord *unit, u16 skill);

void ptyMergeStockSkills(DatPartyRecord *unit) {
    u16 unitId = unit->unitId;
    DatPartyRecord *stock = &datGameState->templates[unitId];
    u16 occupied = stock->flags & 1;
    u32 i;
    u16 skill;

    if (occupied != 0) {
        for (i = 0; i < 77; i++) {
            unit->skillSnapshotWords[i] |= stock->skillSnapshotWords[i];
        }
        unit->profileId = stock->profileId;
    }
    scrClearFlags(unit);
    for (i = 0; i < 4; i++) {
        u32 value = D_0032AEF0[unitId][i];
        skill = value;
        if (value != 0) {
            scrSetFlag(unit, skill);
            scrSetSecondaryScriptFlag(unit, skill);
        }
    }
}

/* Visit occupied roster slots for native per-entry processing. */
void dds3ForEachFlagged(void) {
    s32 slotIndex = 0;

    do {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if (entry->flags & 1) {
            ptyMergeStockSkills(entry);
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

INCLUDE_ASM(const s32, "game/code_00119900", ptySaveActiveUnitsToStock);

void evtRandomizeEntryValue(s32 entryAddress) {
    s32 randomOffset;

    randomOffset = effMiscRandMod(0, 4);
    ((DatPartyRecord *)entryAddress)->randomizedValue = 0x12 - randomOffset;
}


extern s32 effMiscRandMod(u32 stream, u32 modulus);

/* Clear a subset of per-unit status flags on occupied qualifying entries,
 * gated by the event RNG; report whether any flags were cleared. */
s32 evtClearRandomStatusFlags(void) {
    s32 clearedAny = 0;
    s32 remaining;
    DatPartyRecord *entry;
    s32 roll = (s32)effMiscRandMod(0, 100);

    if (roll >= 51) {
        return 0;
    }
    remaining = 4;
    entry = datGameState->party;
    do {
        if ((entry->flags & 1) != 0 && entry->hp != 0) {
            u16 statusFlags = entry->status;
            if ((statusFlags & 0x5D0) != 0) {
                entry->status = statusFlags & ~0x5D0;
                clearedAny = 1;
            }
        }
        remaining--;
        entry++;
    } while (remaining >= 0);
    return clearedAny;
}

extern s32 func_0010BE30(ScrData *context, s32 procedureIndex);

/* Step the script with these context values; clear the written flag, not the stored result. */
s32 evtRunContext(s32 script, s32 first, s32 second, s32 third, u16 options) {
    func_0010BE30(evtWorkScriptTask, script);
    ((EvtScriptContext *)D_003C2E70)->third = third;
    ((EvtScriptContext *)D_003C2E70)->first = first;
    ((EvtScriptContext *)D_003C2E70)->second = second;
    ((EvtScriptContext *)D_003C2E70)->options = options;
    ((EvtScriptContext *)D_003C2E70)->stateFlags &= EVT_RESULT_RESET_MASK;
    bfStepContext(evtWorkScriptTask);
    return ((EvtScriptContext *)D_003C2E70)->result;
}

/* Create the script task and clear its separate 24-byte context. */
void dds3WorkInit(void *header) {
    evtWorkScriptTask = scrCreateTaskWithDefaultOption(header);
    memset(D_003C2E70, 0, EVT_CONTEXT_BYTES);
}

u32 scrGetWorkTaskHandle(void) {
    return (u32)evtWorkScriptTask;
}

void scrDestroyWorkTask(void) {
    scrProcDestroyTask(evtWorkScriptTask);
    evtWorkScriptTask = 0;
}

s32 evtPushFirstRosterLevel(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_003C2E78[0])->level);
    return 1;
}

s32 evtPushSecondRosterLevel(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_003C2E7C[0])->level);
    return 1;
}

s32 evtPushFirstRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_003C2E78[0])->hp);
    return 1;
}

s32 evtPushSecondRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_003C2E7C[0])->hp);
    return 1;
}

s32 evtPushFirstRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_003C2E78[0])->maxHp);
    return 1;
}

s32 evtPushSecondRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_003C2E7C[0])->maxHp);
    return 1;
}

extern s32 btlResolveUnitValueWithOverride(DatPartyRecord *, s32);
extern u32 datReadLowHalfOfCalculatedValue(DatPartyRecord *, s32);
/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void) {
    s8 statIndex = datCommandSelectors[((EvtScriptContext *)D_003C2E70)->third].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->first, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void) {
    s8 statIndex = datCommandSelectors[((EvtScriptContext *)D_003C2E70)->third].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->second, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

extern s32 datFlagToElementIndex(s32);

/* Push the first entry's option-selected stat, retaining the same default selectors. */
s32 evtPushFirstRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(((EvtScriptContext *)D_003C2E70)->options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->first, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's option-selected stat, retaining the same default selectors. */
s32 evtPushSecondRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(((EvtScriptContext *)D_003C2E70)->options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->second, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride((DatPartyRecord *)D_003C2E78[0], statIndex));
    return 1;
}

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride((DatPartyRecord *)D_003C2E7C[0], statIndex));
    return 1;
}

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void) {
    EvtScriptContext *scriptContext = (EvtScriptContext *)D_003C2E70;
    s32 commandIndex = scriptContext->third;
    s32 statValue;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        statValue = ((RosterDetail *)datRosterDetails)[((DatPartyRecord *)scriptContext->first)->unitId].lowValue;
    } else {
        statValue = datCommandRecords[commandIndex].stat11;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Kind 5 scales the command stat by the first entry's roster multiplier, then truncates. */
s32 evtPushSelectedScaledStat(void) {
    EvtScriptContext *scriptContext = (EvtScriptContext *)D_003C2E70;
    s32 commandIndex = scriptContext->third;
    s32 statValue = datCommandRecords[commandIndex].attribute.parts.hitChance;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        f32 rosterScale = ((RosterDetail *)datRosterDetails)[((DatPartyRecord *)scriptContext->first)->unitId].scale;
        statValue = (s32)((f32)statValue * rosterScale);
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatGrade(void) {
    scrSetIntegerReturnValue(datCommandRecords[D_003C2E74[0]].stat2D);
    return 1;
}

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void) {
    s32 statValue;

    switch (((EvtScriptContext *)D_003C2E70)->options) {
    case 1:
        statValue = datCommandRecords[((EvtScriptContext *)D_003C2E70)->third].stat18;
        break;
    case 2:
        statValue = datCommandRecords[((EvtScriptContext *)D_003C2E70)->third].stat1C;
        break;
    default:
        statValue = 0;
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Select the alternate stat through the first entry's table value, not the context selector. */
s32 evtPushEntryIndexedStatOption(void) {
    s32 *contextWords = D_003C2E70;
    s32 statValue;
    u16 statOption = ((EvtScriptContext *)contextWords)->options;
    u16 commandIndex = ((EventIndexRecord *)datItemSkillRecords)[((DatPartyRecord *)contextWords[2])->menuValue].index;

    switch (statOption) {
    case 1:
        statValue = datCommandRecords[commandIndex].stat18;
        break;
    case 2:
        statValue = datCommandRecords[commandIndex].stat1C;
        break;
    default:
        statValue = 0;
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Kind 5 selects the first entry's high roster detail instead of the command total. */
s32 evtPushSelectedTotalOrRosterHighValue(void) {
    EvtScriptContext *scriptContext = (EvtScriptContext *)D_003C2E70;
    s32 commandIndex = scriptContext->third;
    s32 statValue;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        statValue = ((RosterDetail *)datRosterDetails)[((DatPartyRecord *)scriptContext->first)->unitId].highValue;
    } else {
        statValue = datCommandRecords[commandIndex].stat34;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatMaximum(void) {
    scrSetIntegerReturnValue(datCommandRecords[D_003C2E74[0]].stat36);
    return 1;
}

/* Store the script argument as the context result and mark it written. */
s32 evtStoreScriptParameterResult(void) {
    u16 stateFlags = ((EvtScriptContext *)D_003C2E70)->stateFlags | EVT_RESULT_WRITTEN_FLAG;

    ((EvtScriptContext *)D_003C2E70)->stateFlags = stateFlags;
    ((EvtScriptContext *)D_003C2E70)->result = scrReadIntParameter(0);
    return 1;
}

s32 evtPushScriptContextResult(void) {
    scrSetIntegerReturnValue(D_003C2E80[0]);
    return 1;
}

s32 evtPushEntryFlagBitInverted(void) {
    scrSetIntegerReturnValue((((((DatPartyRecord *)D_003C2E78[0])->flags) >> 5) ^ 1) & 1);
    return 1;
}

/* For positive ranges, return 1.0 plus a percentage jitter; the upper endpoint is excluded. */
s32 evtRollRandomScale(void) {
    s32 percentRange = scrReadIntParameter(0);
    s32 roll = effMiscRandMod(0, percentRange * 2);

    scrSetFloatReturnValue((f32)(roll - percentRange + 100) / 100.0f);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageCurrentValueForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        averageValue = btlAverageAllCurrentForMask(groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK);
    }
    else {
        averageValue = 0;
    }
    scrSetIntegerReturnValue(averageValue);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageMaximumValueForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        averageValue = btlAverageAllMaximumForMask(groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK);
    }
    else {
        averageValue = 0;
    }
    scrSetIntegerReturnValue(averageValue);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageActorStatForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        averageValue = func_001A94A0(groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK);
    }
    else {
        averageValue = 0;
    }
    scrSetIntegerReturnValue(averageValue);
    return 1;
}

/* Divide the numeric group mask, not an actor stat; preserve native unsigned division. */
s32 evtPushAvailableChoiceRatio(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    u64 quotient;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        s32 divisor = scrReadIntParameter(1);
        s32 selectedMask = groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK;
        quotient = (u64)selectedMask / divisor;
    }
    else {
        quotient = 0;
    }
    scrSetIntegerReturnValue(quotient);
    return 1;
}

/* Push the native battle float when a runtime exists, otherwise zero. */
s32 evtPushAvailableFloatValue(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    f32 value = 0.0f;

    if (battleAvailable != 0) {
        value = func_001A4598();
    }
    scrSetFloatReturnValue(value);
    return 1;
}

/* Push the native battle integer when a runtime exists, otherwise zero. */
s32 evtPushAvailableIntegerValue(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 value = 0;

    if (battleAvailable != 0) {
        value = func_001A4630();
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

void func_0011C390(void) {
    scrSetFloatReturnValue(datBattleParameters->maxHpGrowth[((DatPartyRecord *)D_003C2E78[0])->level - 1]);
}

void func_0011C3C0(void) {
    scrSetFloatReturnValue(datBattleParameters->maxMpGrowth[((DatPartyRecord *)D_003C2E78[0])->level - 1]);
}

extern s32 datComputeSkillBoostedMaxHp(s32);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
void evtSelectStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(((EvtScriptContext *)D_003C2E70)->second);
    s32 currentHp = ((DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->second)->hp;
    s32 hpPercent = (s32)((f32)currentHp / (f32)maximumHp * 100.0f);
    s32 gradeIndex = 0;

    if (hpPercent != 100) {
        gradeIndex = 1;
        if (hpPercent < 80) {
            gradeIndex = 2;
            if (hpPercent < 60) {
                gradeIndex = 3;
                if (hpPercent < 40) {
                    gradeIndex = 4;
                    if (hpPercent < 30) {
                        gradeIndex = 5;
                        if (hpPercent < 20) {
                            gradeIndex = hpPercent >= 10 ? 6 : 7;
                        }
                    }
                }
            }
        }
    }
    scrSetFloatReturnValue(datBattleParameters->hpGradeValues[gradeIndex]);
}

void func_0011C4C0(void) {
    scrSetFloatReturnValue(datBattleParameters->levelValuesA[((DatPartyRecord *)D_003C2E78[0])->level - 1]);
}

void func_0011C4F0(void) {
    scrSetFloatReturnValue(datBattleParameters->levelValuesB[((DatPartyRecord *)D_003C2E78[0])->level - 1]);
}

void func_0011C520(void) {
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[((DatPartyRecord *)D_003C2E78[0])->level - 1]);
}

void func_0011C550(void) {
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[((DatPartyRecord *)D_003C2E78[0])->level - 1]);
}

void evtScriptSelectRandomValue(void) {
    s32 value;
    s32 roll;

    if ((((DatPartyRecord *)D_003C2E7C[0])->flags & 0x20) == 0) {
        roll = effMiscRandMod(0, 0x20);
        value = roll != 0 ? 10 : 0x80;
    } else {
        roll = effMiscRandMod(0, 0x30);
        value = roll != 0 ? 0 : 0x80;
    }
    scrSetIntegerReturnValue(value);
}

void evtPushRosterBaseValue(void) {
    scrSetIntegerReturnValue(((RosterDetail *)datRosterDetails)[((DatPartyRecord *)D_003C2E78[0])->unitId].baseValue);
}

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
void evtSelectFineStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(((EvtScriptContext *)D_003C2E70)->second);
    s32 currentHp = ((DatPartyRecord *)((EvtScriptContext *)D_003C2E70)->second)->hp;
    s32 hpPercent = (s32)((f32)currentHp / (f32)maximumHp * 100.0f);
    s32 gradeIndex = 0;

    if (hpPercent != 100) {
        gradeIndex = 1;
        if (hpPercent < 90) {
            gradeIndex = 2;
            if (hpPercent < 80) {
                gradeIndex = 3;
                if (hpPercent < 70) {
                    gradeIndex = 4;
                    if (hpPercent < 60) {
                        gradeIndex = 5;
                        if (hpPercent < 50) {
                            gradeIndex = 6;
                            if (hpPercent < 40) {
                                gradeIndex = 7;
                                if (hpPercent < 30) {
                                    gradeIndex = 8;
                                    if (hpPercent < 20) {
                                        gradeIndex = hpPercent >= 10 ? 9 : 10;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    scrSetFloatReturnValue(datBattleParameters->hpFineGradeValues[gradeIndex]);
}

extern s32 evtGetMirroredSolarPhase(void);

s32 evtTestSolarPhaseOrModelFlag(u32 flags) {
    u32 type = flags >> 16;
    switch (type) {
    case 0:
        break;
    case 1:
        if (flags & (1 << evtGetMirroredSolarPhase()) & 0xFFFF) {
            return 1;
        }
        break;
    case 2:
        if (mdlFlagTest(flags & 0xFFFF)) {
            return 1;
        }
        break;
    }
    return 0;
}


u8 func_0011C790(s32 index) {
    s8 enabled[3];
    s32 i;
    u8 code;

    for (i = 0; i < 3; i++) {
        enabled[i] = evtTestSolarPhaseOrModelFlag(D_003BAA3C[index].conditions[i]);
    }
    code = D_003BAA3C[index].variantCodes[0];
    if (enabled[0] && enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_003BAA3C[index].variantCodes[1];
    if (enabled[0] && enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_003BAA3C[index].variantCodes[2];
    if (enabled[0] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_003BAA3C[index].variantCodes[3];
    if (enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_003BAA3C[index].variantCodes[4];
    if (enabled[0]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_003BAA3C[index].variantCodes[5];
    if (enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_003BAA3C[index].variantCodes[6];
    if (enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_003BAA3C[index].variantCodes[7];
    return code == 8 ? 0 : code;
}

/* Compare truncated averages: enemy average + party average / 4 >= party average.
 * Requires a nonzero scene, a nonempty enemy list, and party entries carrying
 * all three flag masks 1/2/4. No eligible entries returns zero.
 * count and remaining serve both loops; word holds enemy IDs, then entry flags.
 * partyLevel is accumulated first and divided in place before comparison. */
INCLUDE_ASM(const s32, "game/code_00119900", btlCheckScenePartyLevelThreshold);

/* Reuse an active interval, or seed one for a nonempty encounter group. */
s32 func_0011CAB0(s32 index) {
    s32 variant;
    s32 total;
    s32 i;
    s32 interval;
    if (index == 0) {
        return 0;
    }
    variant = func_0011C790(index);
    total = 0;
    for (i = 0; i < 20; i++) {
        total += D_003BAA3C[index].groups[variant].entries[i].weight;
    }
    if (total == 0) {
        return 0;
    }
    if (D_003BAAB8 > 0) {
        return D_003BAAB8;
    }
    interval = D_003BAA3C[index].groups[variant].interval;
    if (interval <= 0) {
        return 0;
    }
    D_003BAAB8 = effMiscRandMod(0, interval * 2 - 32) + 16;
    return D_003BAAB8;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CB90);

void dds3WorkClear(void) {
    datGameState->unk1360 = 0;
    datGameState->unk1364 = 0;
    D_003BAAB8 = 0;
}

void func_0011CE30(void) {
}

void func_0011CE38(void) {
}

void func_0011CE40(void) {
}

void func_0011CE48(void) {
}

s32 evtPushRequestedRosterPresence(void) {
    s32 rosterIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(ptyIsRosterEntryPresent(rosterIndex) == 1);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00119900", datBattleSceneRecords);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA38);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA3C);

INCLUDE_SDATA(const s32, "game/code_00119900", fldEncounterRollTable);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA44);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA48);

INCLUDE_SDATA(const s32, "game/code_00119900", datCommandSelectors);

INCLUDE_SDATA(const s32, "game/code_00119900", datCommandRecords);

INCLUDE_SDATA(const s32, "game/code_00119900", datAffinityRecords);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA58);

INCLUDE_SDATA(const s32, "game/code_00119900", datAbilityParameters);

INCLUDE_SDATA(const s32, "game/code_00119900", datActionAnimationRecords);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA64);

INCLUDE_SDATA(const s32, "game/code_00119900", datItemSkillRecords);

INCLUDE_SDATA(const s32, "game/code_00119900", datBattleParameters);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA70);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA74);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA78);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA7C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA80);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA84);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA88);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA8C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA90);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA94);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA98);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA9C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAA0);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAA4);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAA8);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAAC);

INCLUDE_SDATA(const s32, "game/code_00119900", evtWorkScriptTask);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAB8);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAABC);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAC0);

