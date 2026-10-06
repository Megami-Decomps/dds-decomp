#include "common.h"
#include "sdf.h"
#include "btl_action.h"
#include "dat_state.h"

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
    PTY_TEMPLATE_KEEP_BASE_LEVEL = 1,
    PTY_TEMPLATE_USE_PARTY_MAX_LEVEL = 4,
    PTY_ENTRY_STAT_COUNT = 5
};

extern u8 D_00386350[];
extern s32 func_0011C6A8(s32, s32, u8);


extern s32 datItemSkillRecords;

extern DatPartyRecord *dds3FindEntry();

extern u32 evtWorkScriptTask;

extern s32 datEnemyRecords;

typedef struct TableEntry32 {
    u16 value; /* 0x0: copied to active roster entry */
    u16 unk2; /* 0x2 */
} TableEntry32;

extern TableEntry32 D_00386248[];

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

extern Entry4 D_003862C8[];

/* Script-visible records: only the accessed fields are identified. */
typedef struct EventStatRecord {
    u8 pad00[0x11];
    u8 stat11;
    u8 pad12[6];
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
} EventStatRecord; /* stride 0x38 */

typedef struct EventRosterStat {
    s16 base;             /* 0x00 */
    u8 alternateA;        /* 0x02 */
    u8 alternateB;        /* 0x03 */
    f32 multiplier;       /* 0x04 */
    u8 pad08[0x0C];
} EventRosterStat; /* stride 0x14 */

typedef struct EventRosterRecord {
    u32 flags;              /* 0x00: battle availability flags */
    u8 flaggedValue;        /* 0x04 */
    u8 pad05[0x23];
    s32 panelValue;       /* 0x28 */
    u8 pad2C[0x20];
} EventRosterRecord; /* stride 0x4C */

typedef struct EventIndexRecord {
    u8 pad00[2];
    u16 index;            /* 0x02 */
    u8 pad04[4];
} EventIndexRecord; /* stride 0x08 */

/* This separate script context is cleared as one native 24-byte allocation. */
typedef struct EvtScriptContext {
    u16 stateFlags;
    u16 pad02;
    s32 third;
    s32 first;
    s32 second;
    s32 result;
    u16 options;
    u8 pad16[2];
} EvtScriptContext;
typedef char EvtScriptContextSizeCheck[sizeof(EvtScriptContext) == 0x18 ? 1 : -1];

typedef struct EventModeSlot {
    s8 stat;
    s8 kind;             /* 0x01 */
} EventModeSlot;

extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);

extern void func_0011CA88(DatPartyRecord *entry);


extern s32 scrSetIntegerReturnValue();


extern s32 scrReadIntParameter(s32 idx);

extern s32 datGetStatWithStatusOverride(s32 arg0, s32 arg1);

extern s32 datCommandRecords;

extern s32 datCommandSelectors;

extern s32 datRosterDetails;

extern s32 evtGetMirroredSolarPhase(void);


extern EvtScriptContext D_0043E5C0;


extern void scrSetFloatReturnValue(f32 value);

extern u32 effMiscRandMod(void *stream, u32 modulus);

extern s32 datAdjustCurrentHp(void *, s32);

extern s32 datAdjustCurrentMp(void *, s32);

extern u8 btlIsRuntimeAllocated(void);

extern f32 func_001AD978(void);

extern u32 func_001ADA10(void);

extern s32 D_00435E8C;

extern s32 mdlFlagTest(s32 flagIndex);

extern s8 (*D_00435E3C)[6];

extern s32 D_00386288[];

extern void mnuSetPartyEntryCurrentId(DatPartyRecord *, s32);

extern s32 func_0011AEE0(s32);

extern void scrRemoveAvailableSkillFlagAndSlot(DatPartyRecord *, s32);


extern s32 datUnitHasSkill(DatPartyRecord *, s32);

extern void func_0010C058(u32, s32);

extern void bfStepContext(u32);

extern s32 btlAverageAllCurrentForMask(u32 arg0);

extern s32 btlAverageAllMaximumForMask(u32 arg0);

extern s32 func_001B3A00(u32 arg0);

extern s32 scrCreateTaskWithDefaultOption(void);

extern void *memset(void *dst, s32 c, u32 n);
extern DatPartyRecord *D_00435DD4;
extern void mdlFlagSet(s32 flagIndex);

/* Flag-range items set their model flag regardless of quantityDelta.
 * Other items add the delta to their byte quantity and clamp it; DDS2 also
 * caps IDs at or above 0xC0 to one. */
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
    if (itemId >= 0xC0) {
        if (quantity >= PTY_ITEM_SINGLE_LIMIT) {
            quantity = PTY_ITEM_SINGLE_MAX;
        }
    } else if (itemId >= 0xA0) {
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

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A220);

u8 evtGetFlaggedRosterValue(DatPartyRecord *entry) {
    if ((entry->flags & 0x20) == 0) {
        return 0;
    }
    return ((EventRosterRecord *)datEnemyRecords)[entry->unitId].flaggedValue;
}

s32 dds3FindEntryIndex(rosterIndex)
    s32 rosterIndex;
{
    s32 slotIndex = 0;
    DatPartyRecord *entry = datGameState->party;
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

/* Read a signed stat byte in the first active roster record. */
s8 ptyReadSignedRosterStatByte(s32 statIndex) {
    return datGameState->party[0].baseStats[statIndex];
}

extern void sdfRaisePackedChannelValue(DatPartyRecord *, u32);

/* Event penalties affect living roster slots, then optionally raise a status channel. */
void func_0011A328(s32 mode) {
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
void func_0011A510(s32 mode) {
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
                    loss = nextHp / 10;
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
    if (mode == 3) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->flags & 2) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = (u32)nextHp / 2;
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
    if (mode == 4) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->flags & 2) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = nextHp * 30 / 100;
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
    s32 index;
    for (index = 0; index < PTY_ACTIVE_ROSTER_COUNT; index++) {
        DatPartyRecord *entry = &datGameState->party[index];
        if (entry->flags & 1) {
            datAdjustCurrentHp(entry, PTY_RECOVERY_DELTA);
            datAdjustCurrentMp(entry, PTY_RECOVERY_DELTA);
            entry->status &= 0x8000;
        }
    }
}

/* Test occupied entries with nonzero HP; mode 1 additionally requires flag bit 1. */
s32 ptyAnyUnitFlagMatch(u32 statusMask, s32 flagMode) {
    s32 slotIndex = 0;
    DatPartyRecord *entry = datGameState->party;
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
    case 0x24B:
        if (recoveryRate > 0.0f) {
            hpRecovery = (s32)((f32)entry->maxHp * recoveryRate);
            mpRecovery = (s32)((f32)entry->maxMp * recoveryRate);
        }
        break;
    case 0x24A:
    case 0x24C:
    case 0x270:
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
    s32 index;
    for (index = 0; index < PTY_ACTIVE_ROSTER_COUNT; index++) {
        DatPartyRecord *entry = &datGameState->party[index];
        if (entry->flags & 1) {
            if (entry->flags & 2) {
                ptyApplySkillRecovery(entry, 0x24C);
                ptyApplySkillRecovery(entry, 0x270);
            }
        }
    }
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
    CommandValueRecord *commands = (CommandValueRecord *)datCommandRecords;

    value = 0;
    switch (commands[commandId].mode) {
    case 1:
        if (entry->flags & 0x20) {
            return 0;
        }
        value = entry->maxHp * commands[commandId].percentage / 100 + commands[commandId].base;
        if (value <= 0) {
            value = 1;
        }
        break;
    case 2:
        if ((entry->flags & 0x20) &&
            (((EventRosterRecord *)datEnemyRecords)[entry->unitId].flags & 0x10)) {
            return 0;
        }
        value = commands[commandId].percentage;
        break;
    }
    return value;
}

void ptyInitRuntime(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        memset(&datGameState->party[i], 0, sizeof(DatPartyRecord));
        datGameState->pad1334[i] = i;
    }
    datGameState->partyCount = 0;
    datGameState->party[0] = D_00435DD4[1];
    datGameState->party[1] = D_00435DD4[4];
    datGameState->party[2] = D_00435DD4[5];
    datGameState->partyCount = 3;
    for (i = 0; i < 5; i++) {
        datGameState->party[i].menuValue = 0;
        datGameState->party[i].itemId = 0;
    }
    datGameState->header.currency = 0;
    for (i = 0; i < 256; i++) {
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

/* Find the first occupied slot with this roster identifier, or return zero. */
DatPartyRecord *dds3FindEntry(rosterIndex)
    s32 rosterIndex;
{
    s32 slotIndex = 0;
    DatPartyRecord *entry = datGameState->party;
    do {
        if (entry->unitId == rosterIndex && (entry->flags & 1)) {
            return entry;
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AEE0);

u8 ptyIsRosterEntryPresent(void) {
    DatPartyRecord *entry;

    entry = dds3FindEntry();
    return entry != 0;
}

/* Return the highest occupied-slot level, or zero when no slot is occupied. */
s32 dds3EntryMax(void) {
    s32 maximumLevel = 0;
    s32 remaining = 4;
    DatPartyRecord *entry = datGameState->party;
    do {
        remaining--;
        if (entry->flags & 1) {
            s32 level = entry->level;
            if (maximumLevel < level) {
                maximumLevel = level;
            }
        }
        entry++;
    } while (remaining >= 0);
    return maximumLevel;
}

/* Ceiling average of occupied-slot levels, with zero for an empty roster. */
s32 ptyGetRoundedAveragePartyLevel(void) {
    s32 levelSum = 0;
    s32 activeCount = 0;
    s32 remaining = 4;
    DatPartyRecord *entry = datGameState->party;
    do {
        remaining--;
        if ((entry->flags & 1) != 0) {
            activeCount++;
            levelSum += entry->level;
        }
        entry++;
    } while (remaining >= 0);
    if (activeCount == 0) {
        return 0;
    }
    return (levelSum + activeCount - 1) / activeCount;
}

extern void ptyAccumulateStatGains(s32 *, s32, DatPartyRecord *);
extern s32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern void ptyRecomputeMaxHpMp(DatPartyRecord *);
extern void evtCopyRosterTableValue(DatPartyRecord *);
extern void func_00286618(void);
extern void func_002866C8(void);
extern void func_003140C8(s32, DatPartyRecord *);
void ptyAssignRosterItemAndMarkOwned(DatPartyRecord *entry);

/* Clone an entry template and raise it to the maximum occupied party level. */
void ptyCloneTemplateAtPartyMaxLevel(DatPartyRecord *entry, s32 templateIndex) {
    s32 targetLevel = dds3EntryMax();
    s32 statGains[PTY_ENTRY_STAT_COUNT];
    s8 *stat;
    s32 *gain;
    s32 remaining;

    *entry = D_00435DD4[templateIndex];
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        stat = entry->baseStats;
        gain = statGains;
        for (remaining = PTY_ENTRY_STAT_COUNT - 1; remaining >= 0; remaining--) {
            *stat++ += *gain++;
        }
        entry->level = targetLevel;
    }
    entry->totalExp = ptyComputeTotalExp(entry, 0);
    func_003140C8(0, entry);
    evtCopyRosterTableValue(entry);
    ptyAssignRosterItemAndMarkOwned(entry);
}

extern void func_00286BA8(void *record);

/* Consume a saved template, optionally raise its level, then mark its item owned. */
void func_0011B4B0(DatPartyRecord *entry, s32 templateIndex, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[PTY_ENTRY_STAT_COUNT];
    s8 *stat;
    s32 *gain;
    s32 remaining;

    memcpy(entry, &datGameState->templates[templateIndex], sizeof(*entry));
    memset(&datGameState->templates[templateIndex], 0, sizeof(*entry));
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        stat = entry->baseStats;
        gain = statGains;
        for (remaining = PTY_ENTRY_STAT_COUNT - 1; remaining >= 0; remaining--) {
            *stat++ += *gain++;
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp(entry, 0);
        ptyRecomputeMaxHpMp(entry);
    }
    if (!(initFlags & 2)) {
        func_003140C8(1, entry);
    }
    func_00286BA8(entry);
    if (entry->itemId != 0) {
        datGameState->inventory.counts[entry->itemId] = 1;
    }
}



/* Clone template 1 into roster 2 and inherit its mantra state, then clear the
 * assigned item. The maximum-party-level flag overrides keep-base-level;
 * otherwise use the rounded party average only when keep-base-level is clear. */
void ptyInitRosterAndClearItem(DatPartyRecord *entry, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[PTY_ENTRY_STAT_COUNT];
    s32 statIndex;
    DatGameState *gameState;

    memcpy(entry, &datGameState->templates[1], sizeof(*entry));
    entry->unitId = 2;
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        for (statIndex = 0; statIndex < PTY_ENTRY_STAT_COUNT; statIndex++) {
            entry->baseStats[statIndex] += statGains[statIndex];
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp(entry, 0);
        ptyRecomputeMaxHpMp(entry);
    }
    evtCopyRosterTableValue(entry);
    gameState = datGameState;
    memcpy(&gameState->mantraBits[2], &gameState->mantraBits[1], sizeof(gameState->mantraBits[2]));
    memcpy(gameState->profileRecords[2], gameState->profileRecords[1], sizeof(gameState->profileRecords[2]));
    func_00286618();
    entry->itemId = 0;
}

/* Clone template 7 into roster 3 and inherit its mantra state. After optional
 * post-initialization, mark any assigned item owned. Level-flag precedence is
 * the same as the adjacent initializer; flag mask 2 skips the optional call. */
void func_0011B9A0(DatPartyRecord *entry, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[PTY_ENTRY_STAT_COUNT];
    s32 statIndex;
    DatGameState *gameState;

    memcpy(entry, &datGameState->templates[7], sizeof(*entry));
    entry->unitId = 3;
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        for (statIndex = 0; statIndex < PTY_ENTRY_STAT_COUNT; statIndex++) {
            entry->baseStats[statIndex] += statGains[statIndex];
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp(entry, 0);
        ptyRecomputeMaxHpMp(entry);
    }
    gameState = datGameState;
    memcpy(&gameState->mantraBits[3], &gameState->mantraBits[7], sizeof(gameState->mantraBits[3]));
    memcpy(gameState->profileRecords[3], gameState->profileRecords[7], sizeof(gameState->profileRecords[3]));
    func_002866C8();
    if (!(initFlags & 2)) {
        func_003140C8(1, entry);
    }
    if (entry->itemId != 0) {
        datGameState->inventory.counts[entry->itemId] = 1;
    }
}

extern s32 func_00314B00(DatPartyRecord *, u16);
extern void func_00314A80(DatPartyRecord *, u16);
extern s32 func_00314990(DatPartyRecord *, u16);
extern void func_00314868(DatPartyRecord *, u16);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);
extern void ptySetProfileRecordValue(DatPartyRecord *, u16, u32);
extern void func_00286738(void);
extern u32 ptyGetSkillNibbleState(DatPartyRecord *, u16);
extern s32 scrSetFlag(DatPartyRecord *, u16);

/* Merge saved templates 1 and 2 into roster 8, taking their greater level
 * and stats and combining both sets of profile and skill flags. */
void func_0011BC80(DatPartyRecord *entry) {
    DatPartyRecord first;
    DatPartyRecord second;
    s32 index;
    s32 present;
    s32 firstValue;
    s32 secondValue;

    memcpy(entry, &datGameState->templates[1], sizeof(*entry));
    entry->unitId = 8;
    memcpy(&first, &datGameState->templates[1], sizeof(first));
    memcpy(&second, &datGameState->templates[2], sizeof(second));
    if (second.menuValue != 0) {
        ptyAdjustItemQuantity(second.menuValue, 1);
    }
    if (first.level > second.level) {
        entry->level = first.level;
    } else {
        entry->level = second.level;
    }
    for (index = 0; index < PTY_ENTRY_STAT_COUNT; index++) {
        if (first.baseStats[index] > second.baseStats[index]) {
            entry->baseStats[index] = first.baseStats[index];
        } else {
            entry->baseStats[index] = second.baseStats[index];
        }
    }
    ptyRecomputeMaxHpMp(entry);
    entry->totalExp = ptyComputeTotalExp(entry, 0);
    for (index = 1; index < 0xB0; index++) {
        present = func_00314B00(&first, index) != 0;
        if (func_00314B00(&second, index)) {
            present = 1;
        }
        if (present) {
            func_00314A80(entry, index);
        }
        present = func_00314990(&first, index) != 0;
        if (func_00314990(&second, index)) {
            present = 1;
        }
        if (present) {
            func_00314868(entry, index);
        }
        /* This merge compares the raw profile words as signed counters. */
        firstValue = ptyGetProfileRecordValue(&first, index);
        secondValue = ptyGetProfileRecordValue(&second, index);
        if (firstValue >= secondValue) {
            ptySetProfileRecordValue(entry, index, ptyGetProfileRecordValue(&first, index));
        } else {
            ptySetProfileRecordValue(entry, index, ptyGetProfileRecordValue(&second, index));
        }
    }
    func_00286738();
    for (index = 1; index < 0x2A0; index++) {
        present = ptyGetSkillNibbleState(&first, index) != 0;
        if (ptyGetSkillNibbleState(&second, index)) {
            present = 1;
        }
        if (present) {
            scrSetFlag(entry, index);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C0B0);

void func_0011C328(u32 arg0) {
    func_0011C0B0(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C340);

void func_0011C680(u32 arg0) {
    func_0011C340(arg0, 0);
}

u32 func_0011C698(void) {
    return 0;
}

u32 func_0011C6A0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C6A8);

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
        for (i = 0, orderCursor = D_00386350 + orderOffset, actorCursor = actors; i < 3; i++, orderCursor++) {
            actorValue = *actorCursor++;
            if (actorValue != 0 && func_0011C6A8(actorValue, affinity, *orderCursor) != 0) {
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
    entry->menuValue = D_00386248[entry->unitId].value;
}

/* Copy table values only for occupied slots whose roster identifier is below 16. */
void evtUpdateFlaggedEntries(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
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
    }
}

/* Apply the two configured count/flag updates; a zero index disables its record. */
void dds3ForEachEntry(void) {
    Entry4 *updates = D_003862C8;
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

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CA88);

/* Visit occupied roster slots for native per-entry processing. */
void dds3ForEachFlagged(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];

        if (entry->flags & 1) {
            func_0011CA88(entry);
        }
    }
}

void ptyClearSelectedSkillFlagsFromActiveEntries(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if ((entry->flags & 1) != 0) {
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5B);
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5C);
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5D);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D0D8);

/* Save occupied active entries to stock; clear the absent special-character slots. */
extern void func_0011D130(void);
INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D130);


void evtRandomizeEntryValue(DatPartyRecord *unit) {
    s32 randomOffset;

    randomOffset = effMiscRandMod(0, 4);
    unit->randomizedValue = 0x12 - randomOffset;
}

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

s32 ptyGetCombinedRecordAndSlotValue(s32 id, s32 slot) {
    s32 index = id - 0xC0;
    if (index <= 0) {
        return 0;
    }
    return D_00435E3C[index][slot] + datGameState->itemStatBonuses[index][slot];
}

void ptyAddClampedEntryValue(DatPartyRecord *entry, s32 statIndex, s32 amount) {
    s32 updated = entry->baseStats[statIndex] + amount;
    if (updated < 0) {
        updated = 0;
    }
    if (updated >= 100) {
        updated = 99;
    }
    entry->baseStats[statIndex] = updated;
}

void ptyAssignRosterItemAndMarkOwned(DatPartyRecord *entry) {
    s32 value = D_00386288[entry->unitId];
    mnuSetPartyEntryCurrentId(entry, value);
    if (value != 0) {
        datGameState->inventory.counts[value] = 1;
    }
}

void ptyAssignPartyRosterItemsAndMarkOwned(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if (entry->flags & 1) {
            s32 index = 0;
            do {
                if (entry->unitId == index) {
                    ptyAssignRosterItemAndMarkOwned(entry);
                }
                index++;
            } while (index < 16);
        }
    }
}

/* Step the script with these context values; clear the written flag, not the stored result. */
s32 evtRunContext(s32 script, s32 first, s32 second, s32 third, u16 options) {
    func_0010C058(evtWorkScriptTask, script);
    D_0043E5C0.third = third;
    D_0043E5C0.first = first;
    D_0043E5C0.second = second;
    D_0043E5C0.options = options;
    D_0043E5C0.stateFlags &= EVT_RESULT_RESET_MASK;
    bfStepContext(evtWorkScriptTask);
    return D_0043E5C0.result;
}

/* Create the script task and clear its separate 24-byte context. */
void dds3WorkInit(void) {
    evtWorkScriptTask = scrCreateTaskWithDefaultOption();
    memset(&D_0043E5C0, 0, sizeof(D_0043E5C0));
}

u32 scrGetWorkTaskHandle(void) {
    return evtWorkScriptTask;
}

void scrDestroyWorkTask(void) {
    scrProcDestroyTask(evtWorkScriptTask);
    evtWorkScriptTask = 0;
}

s32 evtPushFirstRosterLevel(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.first)->level);
    return 1;
}

s32 evtPushSecondRosterLevel(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.second)->level);
    return 1;
}

s32 evtPushFirstRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.first)->hp);
    return 1;
}

s32 evtPushSecondRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.second)->hp);
    return 1;
}

s32 evtPushFirstRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.first)->maxHp);
    return 1;
}

s32 evtPushSecondRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.second)->maxHp);
    return 1;
}

extern s32 btlResolveUnitValueWithOverride(s32, s32);
extern u32 datReadLowHalfOfCalculatedValue(s32, s32);
/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void) {
    s8 statIndex = ((EventModeSlot *)datCommandSelectors)[D_0043E5C0.third].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0.first, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void) {
    s8 statIndex = ((EventModeSlot *)datCommandSelectors)[D_0043E5C0.third].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0.second, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

extern s32 datFlagToElementIndex(s32);

/* Push the first entry's option-selected stat, retaining the same default selectors. */
s32 evtPushFirstRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(D_0043E5C0.options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0.first, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's option-selected stat, retaining the same default selectors. */
s32 evtPushSecondRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(D_0043E5C0.options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0.second, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride(D_0043E5C0.first, statIndex));
    return 1;
}

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride(D_0043E5C0.second, statIndex));
    return 1;
}

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void) {
    s32 statValue;
    s32 commandIndex = D_0043E5C0.third;
    if (((EventModeSlot *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
        statValue = ((EventRosterStat *)datRosterDetails)[rosterIndex].alternateA;
    } else {
        statValue = ((EventStatRecord *)datCommandRecords)[commandIndex].stat11;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Kind 5 scales the command stat by the first entry's roster multiplier, then truncates. */
s32 evtPushSelectedScaledStat(void) {
    s32 statValue;
    s32 commandIndex = D_0043E5C0.third;
    statValue = ((EventStatRecord *)datCommandRecords)[commandIndex].stat25;
    if (((EventModeSlot *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
        statValue = (s32)((f32)statValue * ((EventRosterStat *)datRosterDetails)[rosterIndex].multiplier);
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatGrade(void) {
    scrSetIntegerReturnValue(((EventStatRecord *)datCommandRecords)[D_0043E5C0.third].stat2D);
    return 1;
}

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void) {
    EvtScriptContext *context = &D_0043E5C0;
    s32 statValue;
    u16 statOption = context->options;
    switch (statOption) {
    case 1:
        statValue = ((EventStatRecord *)datCommandRecords)[context->third].stat18;
        break;
    case 2:
        statValue = ((EventStatRecord *)datCommandRecords)[context->third].stat1C;
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
    EvtScriptContext *context = &D_0043E5C0;
    s32 statValue;
    u16 statOption = context->options;
    u16 commandIndex = ((EventIndexRecord *)datItemSkillRecords)[((DatPartyRecord *)context->first)->menuValue].index;
    switch (statOption) {
    case 1:
        statValue = ((EventStatRecord *)datCommandRecords)[commandIndex].stat18;
        break;
    case 2:
        statValue = ((EventStatRecord *)datCommandRecords)[commandIndex].stat1C;
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
    s32 statValue;
    s32 commandIndex = D_0043E5C0.third;
    if (((EventModeSlot *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
        statValue = ((EventRosterStat *)datRosterDetails)[rosterIndex].alternateB;
    } else {
        statValue = ((EventStatRecord *)datCommandRecords)[commandIndex].stat34;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatMaximum(void) {
    scrSetIntegerReturnValue(((EventStatRecord *)datCommandRecords)[D_0043E5C0.third].stat36);
    return 1;
}

/* Store the script argument as the context result and mark it written. */
s32 evtStoreScriptParameterResult(void) {
    u16 stateFlags = D_0043E5C0.stateFlags | EVT_RESULT_WRITTEN_FLAG;

    D_0043E5C0.stateFlags = stateFlags;
    D_0043E5C0.result = scrReadIntParameter(0);
    return 1;
}

s32 evtPushScriptContextResult(void) {
    scrSetIntegerReturnValue(D_0043E5C0.result);
    return 1;
}

s32 evtPushEntryFlagBitInverted(void) {
    scrSetIntegerReturnValue(((((DatPartyRecord *)D_0043E5C0.first)->flags >> 5) ^ 1) & 1);
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
        averageValue = func_001B3A00(groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK);
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
        value = func_001AD978();
    }
    scrSetFloatReturnValue(value);
    return 1;
}

/* Push the native battle integer when a runtime exists, otherwise zero. */
s32 evtPushAvailableIntegerValue(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 value = 0;

    if (battleAvailable != 0) {
        value = func_001ADA10();
    }
    scrSetIntegerReturnValue(value);
    return 1;
}


/* The paired DDS1 readers identify the halfword index as the entry's level.
 * The shared coefficient arrays start at level one. */
s32 func_0011DFE0(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->maxHpGrowth[levelIndex - 1]);
    return 1;
}

s32 func_0011E018(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->maxMpGrowth[levelIndex - 1]);
    return 1;
}

extern s32 datComputeSkillBoostedMaxHp(s32);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(D_0043E5C0.second);
    s32 currentHp = ((DatPartyRecord *)D_0043E5C0.second)->hp;
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
    return 1;
}

s32 func_0011E128(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesA[levelIndex - 1]);
    return 1;
}

s32 func_0011E160(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesB[levelIndex - 1]);
    return 1;
}

s32 func_0011E198(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[levelIndex - 1]);
    return 1;
}

s32 func_0011E1D0(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[levelIndex - 1]);
    return 1;
}

s32 evtRollFlagDependentResultCode(void) {
    s32 value;
    if ((((DatPartyRecord *)D_0043E5C0.second)->flags & 0x20) == 0) {
        value = effMiscRandMod(0, 0x20) != 0 ? 0xA : 0x80;
    } else {
        value = effMiscRandMod(0, 0x30) != 0 ? 0 : 0x80;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushRosterBaseValue(void) {
    u16 index = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
    scrSetIntegerReturnValue(((EventRosterStat *)datRosterDetails)[index].base);
    return 1;
}

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectFineStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(D_0043E5C0.second);
    s32 currentHp = ((DatPartyRecord *)D_0043E5C0.second)->hp;
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
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E3A0);

s32 evtPushRosterOrGlobalCounterValue(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    s32 result;
    if (!(entry->flags & 0x20)) {
        result = datGameState->header.currency;
    } else {
        s32 index = entry->unitId;
        result = ((EventRosterRecord *)datEnemyRecords)[index].panelValue;
    }
    scrSetIntegerReturnValue(result);
    return 1;
}

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

/* Three conditional encounter groups follow the record's condition header. */
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
    BattleAdjustmentGroup groups[3];
} BattleAdjustmentRecord;

extern BattleAdjustmentRecord *D_00435E0C;

u8 func_0011E528(s32 index) {
    s8 enabled[3];
    s32 i;
    u8 code;

    for (i = 0; i < 3; i++) {
        enabled[i] = evtTestSolarPhaseOrModelFlag(D_00435E0C[index].conditions[i]);
    }
    code = D_00435E0C[index].variantCodes[0];
    if (enabled[0] && enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[1];
    if (enabled[0] && enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[2];
    if (enabled[0] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[3];
    if (enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[4];
    if (enabled[0]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[5];
    if (enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[6];
    if (enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[7];
    return code == 8 ? 0 : code;
}

INCLUDE_ASM(const s32, "game/code_0011A118", btlCheckScenePartyLevelThreshold);

/* Reuse an active interval, or seed one for a nonempty encounter group. */
s32 func_0011E848(s32 index) {
    s32 variant;
    s32 total;
    s32 i;
    s32 interval;
    if (index == 0) {
        return 0;
    }
    variant = func_0011E528(index);
    total = 0;
    for (i = 0; i < 20; i++) {
        total += D_00435E0C[index].groups[variant].entries[i].weight;
    }
    if (total == 0) {
        return 0;
    }
    if (D_00435E8C > 0) {
        return D_00435E8C;
    }
    interval = D_00435E0C[index].groups[variant].interval;
    if (interval <= 0) {
        return 0;
    }
    D_00435E8C = effMiscRandMod(0, interval * 2 - 60) + 30;
    return D_00435E8C;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E930);

void dds3WorkClear(void) {
    DatGameState *state = datGameState;

    state->unk1440 = 0;
    state->unk1444 = 0;
    D_00435E8C = 0;
}

void func_0011EBE0(void) {
}

void func_0011EBE8(void) {
}

void func_0011EBF0(void) {
}

void func_0011EBF8(void) {
}

/* Mode zero queries presence; nonzero mode selects/moves the entry into the
 * frontline group. The latter is not a pure presence test: its retail
 * callee changes flag mask 2 and swaps roster records. Preserve the short-arity
 * presence call and push whether the operation returned exactly one. */
s32 func_0011EC00(void) {
    s32 rosterIndex = scrReadIntParameter(0);
    s32 result;
    if (scrReadIntParameter(1) == 0) {
        result = ptyIsRosterEntryPresent();
    } else {
        result = func_0011AEE0(rosterIndex);
    }
    scrSetIntegerReturnValue(result == 1);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_0011A118", datBattleSceneRecords);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E08);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E0C);

INCLUDE_SDATA(const s32, "game/code_0011A118", fldEncounterRollTable);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E14);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E18);

INCLUDE_SDATA(const s32, "game/code_0011A118", datCommandSelectors);

INCLUDE_SDATA(const s32, "game/code_0011A118", datCommandRecords);

INCLUDE_SDATA(const s32, "game/code_0011A118", datAffinityRecords);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E28);

INCLUDE_SDATA(const s32, "game/code_0011A118", datAbilityParameters);

INCLUDE_SDATA(const s32, "game/code_0011A118", datActionAnimationRecords);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E34);

INCLUDE_SDATA(const s32, "game/code_0011A118", datItemSkillRecords);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E3C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E40);

INCLUDE_SDATA(const s32, "game/code_0011A118", datBattleParameters);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E48);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E4C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E50);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E54);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E58);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E5C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E60);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E64);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E68);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E6C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E70);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E74);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E78);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E7C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E80);

INCLUDE_SDATA(const s32, "game/code_0011A118", evtWorkScriptTask);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E8C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E90);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E94);

