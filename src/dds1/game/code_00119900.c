#include "common.h"
#include "sdf.h"
#include "btl_action.h"

enum {
    PTY_ACTIVE_ROSTER_COUNT = 5,
    PTY_ACTIVE_ROSTER_OFFSET = 0xA60,
    PTY_ACTIVE_ROSTER_STRIDE = 0x1A4,
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
    BTL_ENEMY_LEVEL_OFFSET = 5,
    PTY_ACTIVE_ROSTER_LEVEL_OFFSET = 0xA74
};

extern s32 datAffinityRecords;
extern u8 D_0032AF70[];
extern s32 func_0011B158(s32, s32, u8);


extern s32 datCommandSelectors;
extern s32 datGameState;
extern s32 datRosterDetails;
extern s32 datEnemyRecords;
extern s32 datCommandRecords;
extern s32 datItemSkillRecords;
extern s32 datBattleSceneRecords;
extern s32 D_003BAAB8;

typedef struct TableEntry32 {
    u16 value; /* 0x0: copied to active roster entry */
    u16 unk2; /* 0x2 */
} TableEntry32;

typedef struct Entry4 {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 pad3; /* 0x3 */
} Entry4;

typedef struct CommandValueRecord {
    u8 pad0[3];
    u8 mode;
    u16 percentage;
    u16 base;
    u8 pad8[0x30];
} CommandValueRecord;

typedef struct Entry1A4 {
    u16 flags; /* 0x0: active and flagged-entry bits */
    u8 pad2[2]; /* 0x2 */
    u16 rosterIndex; /* 0x4: entry identifier */
    u16 unk6; /* 0x6 */
    u16 unk8; /* 0x8: script-visible halfword */
    u8 padA[2];
    u16 unkC; /* 0xC */
    u16 unkE; /* 0xE */
    u8 pad10[4]; /* 0x10 */
    u16 level; /* 0x14: clamped at level 99 by dds3Clamp99 */
    u8 pad16[0x3C];      /* 0x016 */
    u16 tableValue;       /* 0x052 */
    u8 pad54[0x140];
    s32 randomizedValue;  /* 0x194 */
    u8 pad198[0xC];
} Entry1A4;

/* Script-visible data tables indexed by roster number or event parameter. */
typedef struct RosterDetail {
    s16 baseValue;     /* 0x00 */
    u8 lowValue;       /* 0x02 */
    u8 highValue;      /* 0x03 */
    f32 scale;         /* 0x04 */
    u8 pad8[0xC];
} RosterDetail; /* 0x14 */

typedef struct EventStatRow {
    u8 pad0[0x11];
    u8 stat;           /* 0x11 */
    u8 pad12[6];
    s16 stat18;        /* 0x18: script-selected value */
    u8 pad1A[2];
    s16 stat1C;        /* 0x1C: alternate script-selected value */
    u8 pad1E[7];
    u8 scaledStat;     /* 0x25 */
    u8 pad26[7];
    u8 grade;          /* 0x2D */
    u8 pad2E[6];
    s16 total;         /* 0x34 */
    s16 max;           /* 0x36 */
} EventStatRow; /* 0x38 */

typedef struct EventSelector {
    s8 stat;
    s8 kind;           /* 0x01: kind five uses roster details instead */
} EventSelector; /* 0x02 */

typedef struct RosterFlagValue {
    u32 flags;          /* 0x00: battle availability flags */
    u8 value;          /* 0x04 */
    u8 pad5[0x47];
} RosterFlagValue; /* 0x4C */
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

extern u32 evtWorkScriptTask;

extern s32 D_003C2E70[];
extern s32 D_003C2E74[];
extern s32 D_003C2E78[];
extern s32 D_003C2E7C[];
extern s32 D_003C2E80[];

extern s32 scrCreateTaskWithDefaultOption(void);
extern s32 scrReadIntParameter(s32 idx);
extern s32 scrSetIntegerReturnValue();
extern void scrSetFloatReturnValue(f32 value);
extern Entry1A4 *dds3FindEntry(s32 rosterIndex);
extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);
extern void ptyMergeStockSkills(Entry1A4 *unit);
extern s32 datGetStatWithStatusOverride(s32 arg0, s32 arg1);
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
 * Other items add the delta to their byte quantity and clamp it.
 * Keep the native range branches and goto layout. */
void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta) {
    s32 quantityOffset;
    s32 quantity;

    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) >= PTY_ITEM_FLAG_COUNT) {
        goto update_quantity;
    }
    mdlFlagSet(itemId + PTY_ITEM_MODEL_FLAG_BASE);
    return;

update_quantity:
    quantityOffset = itemId + 0x12A0;
    quantity = *(u8 *)(datGameState + quantityOffset);
    quantity += quantityDelta;
    if (quantity < 0) {
        quantity = 0;
    }

    if (itemId >= 0xA0) {
        goto clamp_quantity;
    }
    if (itemId < PTY_ITEM_FLAG_FIRST) {
        goto lower_items;
    }
    if (quantity >= PTY_ITEM_SINGLE_LIMIT) {
        quantity = PTY_ITEM_SINGLE_MAX;
    }
    goto store_quantity;

lower_items:
    if (itemId < 0x60) {
        goto select_quantity_limit;
    }
clamp_quantity:
    if (quantity >= PTY_ITEM_QUANTITY_LIMIT) {
        quantity = PTY_ITEM_MAX_QUANTITY;
    }
    goto store_quantity;

select_quantity_limit:
    quantity = quantity < PTY_ITEM_QUANTITY_LIMIT ? quantity : PTY_ITEM_MAX_QUANTITY;

store_quantity:
    *(u8 *)(datGameState + quantityOffset) = quantity;
}

/* Flag-range items test their model flag and ignore minimumQuantity.
 * Other items require at least the requested byte quantity. */
s32 evtCheckValueThreshold(s32 itemId, s32 minimumQuantity) {
    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        return mdlFlagTest(itemId + PTY_ITEM_MODEL_FLAG_BASE) != 0;
    }
    if (*(u8 *)(itemId + datGameState + 0x12A0) < minimumQuantity) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_00119A00);

u8 evtGetFlaggedRosterValue(s32 entryAddress) {
    Entry1A4 *entry = (Entry1A4 *)entryAddress;
    if ((entry->flags & 0x20) == 0) {
        return 0;
    }
    return ((RosterFlagValue *)datEnemyRecords)[entry->rosterIndex].value;
}

s32 dds3FindEntryIndex(s32 rosterIndex) {
    Entry1A4 *entry = (Entry1A4 *)(datGameState + PTY_ACTIVE_ROSTER_OFFSET);
    s32 slotIndex = 0;

    do {
        if (entry->flags & 1) {
            if (entry->rosterIndex == rosterIndex) {
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
    return *(s8 *)(byteOffset + datGameState + 0xa76);
}

INCLUDE_ASM(const s32, "game/code_00119900", func_00119B08);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119CF0);

/* Recover HP/MP in each occupied roster slot and preserve only status bit 15. */
void ptyRecoverAllUnits(void) {
    s32 offset = 0;
    s32 remaining = 4;

    do {
        Entry1A4 *entry = (Entry1A4 *)(datGameState + offset + PTY_ACTIVE_ROSTER_OFFSET);

        offset += PTY_ACTIVE_ROSTER_STRIDE;
        if (entry->flags & 1) {
            datAdjustCurrentHp(entry, PTY_RECOVERY_DELTA);
            datAdjustCurrentMp(entry, PTY_RECOVERY_DELTA);
            entry->unkE &= 0x8000;
        }
        remaining--;
    } while (remaining >= 0);
}

/* Test occupied entries with nonzero HP; mode 1 additionally requires flag bit 1. */
s32 ptyAnyUnitFlagMatch(u32 statusMask, s32 flagMode) {
    Entry1A4 *entry = (Entry1A4 *)(datGameState + PTY_ACTIVE_ROSTER_OFFSET);
    s32 slotIndex = 0;

    do {
        if (entry->flags & 1) {
            if (entry->unk6 != 0) {
                if (flagMode != 1 || (entry->flags & 2)) {
                    if (entry->unkE & statusMask) {
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

extern s32 datUnitHasSkill(Entry1A4 *, s32);

/* Apply the owned skill's positive HP/MP recovery rate; unsupported skills do nothing. */
void ptyApplySkillRecovery(Entry1A4 *entry, u32 skillId) {
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
            hpRecovery = (s32)((f32)entry->unk8 * recoveryRate);
            mpRecovery = (s32)((f32)entry->unkC * recoveryRate);
        }
        break;
    case 0x22A:
    case 0x22C:
    case 0x250:
        if (recoveryRate > 0.0f) {
            mpRecovery = (s32)((f32)entry->unkC * recoveryRate);
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
    s32 entryOffset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(datGameState + entryOffset + PTY_ACTIVE_ROSTER_OFFSET);
        entryOffset += PTY_ACTIVE_ROSTER_STRIDE;
        if (entry->flags & 1) {
            if (entry->flags & 2) {
                ptyApplySkillRecovery(entry, 0x22C);
                ptyApplySkillRecovery(entry, 0x250);
            }
        }
        remaining--;
    } while (remaining >= 0);
}

/* Return whether an occupied, flag-bit-1 entry owns the requested skill. */
s32 evtHasMatchingFlaggedEntry(s32 skillId) {
    s32 slotIndex = 0;
    s32 entryOffset = 0;
    do {
        Entry1A4 *entry = (Entry1A4 *)(datGameState + entryOffset + PTY_ACTIVE_ROSTER_OFFSET);
        entryOffset += PTY_ACTIVE_ROSTER_STRIDE;
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
s32 datCalculateCommandBaseValue(Entry1A4 *entry, s32 value) {
    s32 commandId = value;
    CommandValueRecord *commands = (CommandValueRecord *)datCommandRecords;

    value = 0;
    switch (commands[commandId].mode) {
    case 1:
        if (entry->flags & 0x20) {
            return 0;
        }
        value = entry->unk8 * commands[commandId].percentage / 100 + commands[commandId].base;
        if (value <= 0) {
            value = 1;
        }
        break;
    case 2:
        if ((entry->flags & 0x20) &&
            (((RosterFlagValue *)datEnemyRecords)[entry->rosterIndex].flags & 0x10)) {
            return 0;
        }
        value = commands[commandId].percentage;
        break;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_00119900", ptyInitRuntime);

u16 evtGetIndexedEventRecordId(s32 tableIndex) {
    return ((EventIndexRecord *)datItemSkillRecords)[tableIndex].index;
}

/* Read the entry's level, capped at the script-visible maximum. */
u16 dds3Clamp99(s32 entryAddress) {
    s32 level = ((Entry1A4 *)entryAddress)->level;

    return level < PTY_LEVEL_LIMIT ? level : PTY_MAX_LEVEL;
}

/* Find the first occupied slot with this roster identifier, or return NULL. */
Entry1A4 *dds3FindEntry(s32 rosterIndex) {
    Entry1A4 *entry = (Entry1A4 *)(datGameState + PTY_ACTIVE_ROSTER_OFFSET);
    s32 slotIndex = 0;

    do {
        if (entry->rosterIndex != rosterIndex) {
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
    Entry1A4 *entry = (Entry1A4 *)(datGameState + PTY_ACTIVE_ROSTER_OFFSET);
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
    Entry1A4 *entry = (Entry1A4 *)(datGameState + PTY_ACTIVE_ROSTER_OFFSET);
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
    s32 *requirementCursor = (s32 *)(datAffinityRecords + affinity * 16 - 0x1AB0);
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

void evtCopyRosterTableValue(s32 entryAddress) {
    ((Entry1A4 *)entryAddress)->tableValue = D_0032AEA8[((Entry1A4 *)entryAddress)->rosterIndex].value;
}

/* Copy table values only for occupied slots whose roster identifier is below 16. */
void evtUpdateFlaggedEntries(void) {
    s32 entryOffset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(datGameState + entryOffset + PTY_ACTIVE_ROSTER_OFFSET);
        if (entry->flags & 1) {
            s32 rosterIndex = 0;
            do {
                if (entry->rosterIndex == rosterIndex) {
                    evtCopyRosterTableValue((s32)entry);
                }
                rosterIndex++;
            } while (rosterIndex < 16);
        }
        remaining--;
        entryOffset += PTY_ACTIVE_ROSTER_STRIDE;
    } while (remaining >= 0);
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

INCLUDE_ASM(const s32, "game/code_00119900", ptyMergeStockSkills);

/* Visit occupied roster slots for native per-entry processing. */
void dds3ForEachFlagged(void) {
    s32 entryOffset = 0;
    s32 remaining = 4;

    do {
        Entry1A4 *entry = (Entry1A4 *)(datGameState + entryOffset + PTY_ACTIVE_ROSTER_OFFSET);

        if (entry->flags & 1) {
            ptyMergeStockSkills(entry);
        }
        entryOffset += PTY_ACTIVE_ROSTER_STRIDE;
        remaining--;
    } while (remaining >= 0);
}

void ptySaveActiveUnitsToStock(void) {
    s32 index;
    for (index = 0; index < 5; index++) {
        Entry1A4 *entry = (Entry1A4 *)(datGameState +
            index * sizeof(Entry1A4) + 0xA60);
        u16 active = entry->flags & 1;
        if (active != 0) {
            Entry1A4 *stock = (Entry1A4 *)(entry->rosterIndex *
                sizeof(Entry1A4) + datGameState + 0x31BB0);
            *stock = *entry;
        }
    }
}

void evtRandomizeEntryValue(s32 entryAddress) {
    s32 randomOffset;

    randomOffset = effMiscRandMod(0, 4);
    ((Entry1A4 *)entryAddress)->randomizedValue = 0x12 - randomOffset;
}

extern s32 datGameState;

extern s32 effMiscRandMod(u32 stream, u32 modulus);

/* Clear a subset of per-unit status flags on occupied qualifying entries,
 * gated by the event RNG; report whether any flags were cleared. */
s32 evtClearRandomStatusFlags(void) {
    s32 clearedAny = 0;
    s32 remaining;
    s32 entryAddress;
    if (effMiscRandMod(0, 100) >= 51) {
        return 0;
    }
    remaining = 4;
    entryAddress = datGameState + PTY_ACTIVE_ROSTER_OFFSET;
    do {
        if ((((Entry1A4 *)entryAddress)->flags & 1) != 0 && ((Entry1A4 *)entryAddress)->unk6 != 0) {
            u16 statusFlags = ((Entry1A4 *)entryAddress)->unkE;
            if ((statusFlags & 0x5D0) != 0) {
                ((Entry1A4 *)entryAddress)->unkE = statusFlags & ~0x5D0;
                clearedAny = 1;
            }
        }
        remaining--;
        entryAddress += PTY_ACTIVE_ROSTER_STRIDE;
    } while (remaining >= 0);
    return clearedAny;
}

extern void func_0010BE30(u32, s32);
extern void bfStepContext(u32);

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
void dds3WorkInit(void) {
    evtWorkScriptTask = scrCreateTaskWithDefaultOption();
    memset(D_003C2E70, 0, EVT_CONTEXT_BYTES);
}

u32 scrGetWorkTaskHandle(void) {
    return evtWorkScriptTask;
}

void scrDestroyWorkTask(void) {
    scrProcDestroyTask(evtWorkScriptTask);
    evtWorkScriptTask = 0;
}

s32 evtPushFirstRosterLevel(void) {
    scrSetIntegerReturnValue(((Entry1A4 *)D_003C2E78[0])->level);
    return 1;
}

s32 evtPushSecondRosterLevel(void) {
    scrSetIntegerReturnValue(((Entry1A4 *)D_003C2E7C[0])->level);
    return 1;
}

s32 evtPushFirstRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((Entry1A4 *)D_003C2E78[0])->unk6);
    return 1;
}

s32 evtPushSecondRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((Entry1A4 *)D_003C2E7C[0])->unk6);
    return 1;
}

s32 evtPushFirstRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((Entry1A4 *)D_003C2E78[0])->unk8);
    return 1;
}

s32 evtPushSecondRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((Entry1A4 *)D_003C2E7C[0])->unk8);
    return 1;
}

extern s32 btlResolveUnitValueWithOverride(s32, s32);
struct DatUnitStatus;
extern u32 datReadLowHalfOfCalculatedValue(struct DatUnitStatus *, s32);
/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void) {
    s8 statIndex = ((EventSelector *)datCommandSelectors)[
        ((EvtScriptContext *)D_003C2E70)->third].stat;
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
                ((EvtScriptContext *)D_003C2E70)->first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (struct DatUnitStatus *)((EvtScriptContext *)D_003C2E70)->first, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void) {
    s8 statIndex = ((EventSelector *)datCommandSelectors)[
        ((EvtScriptContext *)D_003C2E70)->third].stat;
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
                ((EvtScriptContext *)D_003C2E70)->second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (struct DatUnitStatus *)((EvtScriptContext *)D_003C2E70)->second, statIndex);
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
                ((EvtScriptContext *)D_003C2E70)->first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (struct DatUnitStatus *)((EvtScriptContext *)D_003C2E70)->first, statIndex);
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
                ((EvtScriptContext *)D_003C2E70)->second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (struct DatUnitStatus *)((EvtScriptContext *)D_003C2E70)->second, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride(D_003C2E78[0], statIndex));
    return 1;
}

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride(D_003C2E7C[0], statIndex));
    return 1;
}

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void) {
    EvtScriptContext *scriptContext = (EvtScriptContext *)D_003C2E70;
    s32 commandIndex = scriptContext->third;
    s32 statValue;
    if (((EventSelector *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        statValue = ((RosterDetail *)datRosterDetails)[((Entry1A4 *)scriptContext->first)->rosterIndex].lowValue;
    } else {
        statValue = ((EventStatRow *)datCommandRecords)[commandIndex].stat;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Kind 5 scales the command stat by the first entry's roster multiplier, then truncates. */
s32 evtPushSelectedScaledStat(void) {
    EvtScriptContext *scriptContext = (EvtScriptContext *)D_003C2E70;
    s32 commandIndex = scriptContext->third;
    s32 statValue = ((EventStatRow *)datCommandRecords)[commandIndex].scaledStat;
    if (((EventSelector *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        f32 rosterScale = ((RosterDetail *)datRosterDetails)[((Entry1A4 *)scriptContext->first)->rosterIndex].scale;
        statValue = (s32)((f32)statValue * rosterScale);
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatGrade(void) {
    scrSetIntegerReturnValue(((EventStatRow *)datCommandRecords)[D_003C2E74[0]].grade);
    return 1;
}

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void) {
    s32 statValue;

    switch (((EvtScriptContext *)D_003C2E70)->options) {
    case 1:
        statValue = ((EventStatRow *)datCommandRecords)[((EvtScriptContext *)D_003C2E70)->third].stat18;
        break;
    case 2:
        statValue = ((EventStatRow *)datCommandRecords)[((EvtScriptContext *)D_003C2E70)->third].stat1C;
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
    u16 commandIndex = ((EventIndexRecord *)datItemSkillRecords)[((Entry1A4 *)contextWords[2])->tableValue].index;

    switch (statOption) {
    case 1:
        statValue = ((EventStatRow *)datCommandRecords)[commandIndex].stat18;
        break;
    case 2:
        statValue = ((EventStatRow *)datCommandRecords)[commandIndex].stat1C;
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
    if (((EventSelector *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        statValue = ((RosterDetail *)datRosterDetails)[((Entry1A4 *)scriptContext->first)->rosterIndex].highValue;
    } else {
        statValue = ((EventStatRow *)datCommandRecords)[commandIndex].total;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatMaximum(void) {
    scrSetIntegerReturnValue(((EventStatRow *)datCommandRecords)[D_003C2E74[0]].max);
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
    scrSetIntegerReturnValue((((((Entry1A4 *)D_003C2E78[0])->flags) >> 5) ^ 1) & 1);
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
    scrSetFloatReturnValue(datBattleParameters->maxHpGrowth[((Entry1A4 *)D_003C2E78[0])->level - 1]);
}

void func_0011C3C0(void) {
    scrSetFloatReturnValue(datBattleParameters->maxMpGrowth[((Entry1A4 *)D_003C2E78[0])->level - 1]);
}

extern s32 datComputeSkillBoostedMaxHp(s32);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
void evtSelectStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(((EvtScriptContext *)D_003C2E70)->second);
    s32 currentHp = ((Entry1A4 *)((EvtScriptContext *)D_003C2E70)->second)->unk6;
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
    scrSetFloatReturnValue(datBattleParameters->levelValuesA[((Entry1A4 *)D_003C2E78[0])->level - 1]);
}

void func_0011C4F0(void) {
    scrSetFloatReturnValue(datBattleParameters->levelValuesB[((Entry1A4 *)D_003C2E78[0])->level - 1]);
}

void func_0011C520(void) {
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[((Entry1A4 *)D_003C2E78[0])->level - 1]);
}

void func_0011C550(void) {
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[((Entry1A4 *)D_003C2E78[0])->level - 1]);
}

void evtScriptSelectRandomValue(void) {
    s32 value;
    s32 roll;

    if ((((Entry1A4 *)D_003C2E7C[0])->flags & 0x20) == 0) {
        roll = effMiscRandMod(0, 0x20);
        value = roll != 0 ? 10 : 0x80;
    } else {
        roll = effMiscRandMod(0, 0x30);
        value = roll != 0 ? 0 : 0x80;
    }
    scrSetIntegerReturnValue(value);
}

void evtPushRosterBaseValue(void) {
    scrSetIntegerReturnValue(((RosterDetail *)datRosterDetails)[((Entry1A4 *)D_003C2E78[0])->rosterIndex].baseValue);
}

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
void evtSelectFineStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(((EvtScriptContext *)D_003C2E70)->second);
    s32 currentHp = ((Entry1A4 *)((EvtScriptContext *)D_003C2E70)->second)->unk6;
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

/* Four conditional encounter groups follow the record's condition header. */
typedef struct BattleAdjustmentEntry {
    u16 sceneIndex;
    u16 weight;
    s8 value;
    u8 unk05;
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    s32 interval;
    BattleAdjustmentEntry entries[20];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    u8 pad00[8];
    u32 conditions[3];
    u8 variantCodes[8];
    BattleAdjustmentGroup groups[4];
} BattleAdjustmentRecord;

extern BattleAdjustmentRecord *D_003BAA3C;

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
s32 btlCheckScenePartyLevelThreshold(s32 sceneIndex) {
    s32 meetsThreshold = 0;
    s32 enemyLevelSum;
    s32 partyLevelAddress;
    s32 count;
    s32 remaining;
    s32 enemyIdAddress;
    s32 enemyTableBase;
    s32 partyFlagsAddress;
    s32 partyLevel;
    s32 enemyLevelAverage;
    s32 sceneRecordOffset;
    u16 word;

    if (sceneIndex != 0) {
        enemyLevelSum = 0;
        sceneRecordOffset = sceneIndex * BTL_SCENE_RECORD_BYTES;
        enemyIdAddress = sceneRecordOffset + datBattleSceneRecords + 6;
        enemyTableBase = datEnemyRecords;
        count = 0;
        remaining = BTL_SCENE_ENEMY_SLOT_COUNT - 1;
        do {
            word = *(u16 *)enemyIdAddress;
            enemyIdAddress += 2;
            if (word != 0) {
                count++;
                enemyLevelSum += *(u8 *)(enemyTableBase + word * BTL_ENEMY_RECORD_BYTES + BTL_ENEMY_LEVEL_OFFSET);
            }
            remaining--;
        } while (remaining >= 0);
        meetsThreshold = 0;
        if (count != 0) {
            enemyLevelAverage = enemyLevelSum / count;
            partyLevel = 0;
            count = 0;
            remaining = PTY_ACTIVE_ROSTER_COUNT - 1;
            partyLevelAddress = datGameState + PTY_ACTIVE_ROSTER_LEVEL_OFFSET;
            partyFlagsAddress = datGameState + PTY_ACTIVE_ROSTER_OFFSET;
            do {
                word = *(u16 *)partyFlagsAddress;
                partyFlagsAddress += PTY_ACTIVE_ROSTER_STRIDE;
                if (word & 1) {
                    if (word & 4) {
                        if (word & 2) {
                            count++;
                            partyLevel += *(u16 *)partyLevelAddress;
                        }
                    }
                }
                remaining--;
                partyLevelAddress += PTY_ACTIVE_ROSTER_STRIDE;
            } while (remaining >= 0);
            meetsThreshold = 0;
            if (count != 0) {
                partyLevel = partyLevel / count;
                meetsThreshold = enemyLevelAverage + partyLevel / 4 < partyLevel;
                meetsThreshold = meetsThreshold == 0;
            }
        }
    }
    return meetsThreshold;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CAB0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CB90);

void dds3WorkClear(void) {
    s32 base = datGameState;

    *(s32 *)(base + 0x1360) = 0;
    *(s16 *)(base + 0x1364) = 0;
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

