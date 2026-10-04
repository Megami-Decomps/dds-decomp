#include "common.h"

enum {
    PTY_ACTIVE_ROSTER_COUNT = 5,
    PTY_ACTIVE_ROSTER_OFFSET = 0xA60,
    PTY_ACTIVE_ROSTER_STRIDE = 0x1C4,
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

enum {
    PTY_TEMPLATE_KEEP_BASE_LEVEL = 1,
    PTY_TEMPLATE_USE_PARTY_MAX_LEVEL = 4,
    PTY_ENTRY_STAT_COUNT = 5
};

extern s32 datAffinityRecords;
extern u8 D_00386350[];
extern s32 func_0011C6A8(s32, s32, u8);


extern s32 datItemSkillRecords;
extern s32 datBattleSceneRecords;

extern s32 dds3FindEntry();

extern u32 evtWorkScriptTask;

extern s32 datEnemyRecords;

extern s32 datGameState;

/* Item quantities occupy byte slots in the save-state block. */
typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

typedef struct Entry1A4 {
    u16 flags; /* 0x0 */
    u8 pad2[2]; /* 0x2 */
    u16 rosterIndex; /* 0x4 */
    u16 unk6; /* 0x6 */
    u16 unk8; /* 0x8 */
    u8 padA[2]; /* 0xA */
    u16 unkC; /* 0xC */
    u16 unkE; /* 0xE */
    u32 totalExp; /* 0x10 */
    u16 level; /* 0x14: clamped at level 99 by dds3Clamp99 */
    u8 stats[5]; /* 0x16 */
    u8 pad1B[0x37];
    u16 tableValue; /* 0x52 */
    u8 pad54[0x15E];
    u16 itemId; /* 0x1B2 */
    u8 pad1B4[0x10]; /* DDS2 entry stride is 0x1C4. */
} Entry1A4;

/* Native bulk backups assume eight-aligned mantra table storage. */
typedef struct PtyMantraBitmap {
    u32 words[12];
} __attribute__((aligned(8))) PtyMantraBitmap;

typedef struct PtyMantraProgress {
    u32 value;
    u32 flags;
} PtyMantraProgress;

typedef struct PtyStatePrefix {
    SaveItemCounts inventory;
    u8 pad1440[0x15AD0];
    PtyMantraBitmap mantraBits[16];
    PtyMantraProgress mantraProgress[16][0xB0] __attribute__((aligned(8)));
    Entry1A4 templates[8];
} PtyStatePrefix;

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

/* Both active script-entry pointers expose these same word offsets. */
typedef struct EventScriptEntry {
    u16 flags;           /* 0x00 */
    u8 pad02[2];
    u16 rosterIndex;     /* 0x04 */
    u16 value6;          /* 0x06 */
    u16 value8;          /* 0x08 */
    u8 pad0A[0x0A];
    u16 index14;         /* 0x14 */
} EventScriptEntry;

typedef struct EventModeSlot {
    s8 stat;
    s8 kind;             /* 0x01 */
} EventModeSlot;

extern void func_0011A118(s32 itemId, s32 quantityDelta);

extern void func_0011CA88(Entry1A4 *entry);

extern s32 D_0043E5C8[];

extern s32 scrSetIntegerReturnValue();

extern s32 D_0043E5CC[];

extern s32 scrReadIntParameter(s32 idx);

extern s32 datGetStatWithStatusOverride(s32 arg0, s32 arg1);

extern s32 datCommandRecords;

extern s32 datCommandSelectors;

extern s32 datRosterDetails;

extern s32 evtGetMirroredSolarPhase(void);

extern s32 D_0043E5C4[];

extern s32 D_0043E5C0[];

extern s32 D_0043E5D0[];

extern void scrSetFloatReturnValue(f32 value);

extern u32 effMiscRandMod(void *stream, u32 modulus);

extern s32 datAdjustCurrentHp(void *, s32);

extern s32 datAdjustCurrentMp(void *, s32);

extern u8 btlIsRuntimeAllocated(void);

extern f32 func_001AD978(void);

extern u32 func_001ADA10(void);

extern s32 D_00435E8C;

extern s32 mdlFlagTest(s32 flagIndex);

extern s32 D_00435E3C;

extern s32 D_00386288[];

extern void mnuSetPartyEntryCurrentId(Entry1A4 *, s32);

extern s32 func_0011AEE0(s32);

extern void scrRemoveAvailableSkillFlagAndSlot(s32, s32);

extern s32 D_0043E5C0[];

extern s32 datUnitHasSkill(Entry1A4 *, s32);

extern void func_0010C058(u32, s32);

extern void bfStepContext(u32);

extern s32 btlAverageAllCurrentForMask(u32 arg0);

extern s32 btlAverageAllMaximumForMask(u32 arg0);

extern s32 func_001B3A00(u32 arg0);

extern s32 scrCreateTaskWithDefaultOption(void);

extern void *memset(void *dst, s32 c, u32 n);
extern void mdlFlagSet(s32 flagIndex);

/* Flag-range items set their model flag regardless of quantityDelta.
 * Other items add the delta to their byte quantity and clamp it; DDS2 also
 * caps IDs at or above 0xC0 to one. Keep the native branches and goto layout. */
void func_0011A118(s32 itemId, s32 quantityDelta) {
    s32 quantity;

    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        mdlFlagSet(itemId + PTY_ITEM_MODEL_FLAG_BASE);
        return;
    }
    quantity = ((SaveItemCounts *)datGameState)->counts[itemId];
    quantity += quantityDelta;
    if (quantity < 0) {
        quantity = 0;
    }
    if (itemId >= 0xC0) {
        if (quantity >= PTY_ITEM_SINGLE_LIMIT) {
            quantity = PTY_ITEM_SINGLE_MAX;
        }
        goto store_quantity;
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
    ((SaveItemCounts *)datGameState)->counts[itemId] = quantity;
}

/* Flag-range items test their model flag and ignore minimumQuantity.
 * Other items require at least the requested byte quantity. */
s32 evtCheckValueThreshold(s32 itemId, s32 minimumQuantity) {
    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        return mdlFlagTest(itemId + PTY_ITEM_MODEL_FLAG_BASE) != 0;
    }
    if (((SaveItemCounts *)datGameState)->counts[itemId] < minimumQuantity) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A220);

u8 evtGetFlaggedRosterValue(Entry1A4 *entry) {
    if ((entry->flags & 0x20) == 0) {
        return 0;
    }
    return ((EventRosterRecord *)datEnemyRecords)[entry->rosterIndex].flaggedValue;
}

s32 dds3FindEntryIndex(rosterIndex)
    s32 rosterIndex;
{
    s32 slotIndex = 0;
    s32 entry = datGameState + PTY_ACTIVE_ROSTER_OFFSET;
    do {
        if (((Entry1A4 *)entry)->flags & 1) {
            if (((Entry1A4 *)entry)->rosterIndex == rosterIndex) {
                return slotIndex;
            }
        }
        slotIndex++;
        entry += PTY_ACTIVE_ROSTER_STRIDE;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return -1;
}

/* Read a signed byte relative to the first roster entry's stat-byte base;
 * the caller supplies a byte offset, not a whole-entry index. */
s8 func_0011A318(s32 byteOffset) {
    return *(s8 *)(byteOffset + datGameState + 0xa76);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A510);

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
    s32 slotIndex = 0;
    s32 entry = datGameState + PTY_ACTIVE_ROSTER_OFFSET;
    do {
        if (((Entry1A4 *)entry)->flags & 1) {
            if (((Entry1A4 *)entry)->unk6 != 0) {
                if (flagMode != 1 || (((Entry1A4 *)entry)->flags & 2)) {
                    if (((Entry1A4 *)entry)->unkE & statusMask) {
                        return 1;
                    }
                }
            }
        }
        slotIndex++;
        entry += PTY_ACTIVE_ROSTER_STRIDE;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

void evtAdvanceCounterValue(s32 counterAddress, s32 increment) {
    *(s32 *)(counterAddress + 0x10) = *(s32 *)(counterAddress + 0x10) + increment;
}

extern s32 datAbilityParameters;

/* Apply the owned skill's positive HP/MP recovery rate; unsupported skills do nothing. */
void func_0011A808(Entry1A4 *entry, u32 skillId) {
    f32 recoveryRate;
    s32 hpRecovery;
    s32 mpRecovery;

    if (datUnitHasSkill(entry, skillId) == 0) {
        return;
    }
    hpRecovery = 0;
    mpRecovery = 0;
    recoveryRate = *(f32 *)(datAbilityParameters + skillId * 8 - 0x1100);
    switch (skillId) {
    case 0x24B:
        if (recoveryRate > 0.0f) {
            hpRecovery = (s32)((f32)entry->unk8 * recoveryRate);
            mpRecovery = (s32)((f32)entry->unkC * recoveryRate);
        }
        break;
    case 0x24A:
    case 0x24C:
    case 0x270:
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
                func_0011A808(entry, 0x24C);
                func_0011A808(entry, 0x270);
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
s32 func_0011AA58(Entry1A4 *entry, s32 value) {
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
            (((EventRosterRecord *)datEnemyRecords)[entry->rosterIndex].flags & 0x10)) {
            return 0;
        }
        value = commands[commandId].percentage;
        break;
    }
    return value;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AB38);

u16 evtGetIndexedEventRecordId(s32 tableIndex) {
    return ((EventIndexRecord *)datItemSkillRecords)[tableIndex].index;
}

/* Read the entry's level, capped at the script-visible maximum. */
u16 dds3Clamp99(s32 entryAddress) {
    s32 level = ((Entry1A4 *)entryAddress)->level;

    return level < PTY_LEVEL_LIMIT ? level : PTY_MAX_LEVEL;
}

/* Find the first occupied slot with this roster identifier, or return zero. */
s32 dds3FindEntry(rosterIndex)
    s32 rosterIndex;
{
    s32 slotIndex = 0;
    s32 entry = datGameState + PTY_ACTIVE_ROSTER_OFFSET;
    do {
        if (((Entry1A4 *)entry)->rosterIndex == rosterIndex && (((Entry1A4 *)entry)->flags & 1)) {
            return entry;
        }
        slotIndex++;
        entry += PTY_ACTIVE_ROSTER_STRIDE;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AEE0);

u8 ptyIsRosterEntryPresent(void) {
    s64 entry;

    entry = dds3FindEntry();
    return entry != 0;
}

/* Return the highest occupied-slot level, or zero when no slot is occupied. */
s32 dds3EntryMax(void) {
    s32 maximumLevel = 0;
    s32 remaining = 4;
    s32 entry = datGameState + PTY_ACTIVE_ROSTER_OFFSET;
    do {
        remaining--;
        if (((Entry1A4 *)entry)->flags & 1) {
            s32 level = ((Entry1A4 *)entry)->level;
            if (maximumLevel < level) {
                maximumLevel = level;
            }
        }
        entry += PTY_ACTIVE_ROSTER_STRIDE;
    } while (remaining >= 0);
    return maximumLevel;
}

/* Ceiling average of occupied-slot levels, with zero for an empty roster. */
s32 ptyGetRoundedAveragePartyLevel(void) {
    s32 levelSum = 0;
    s32 activeCount = 0;
    s32 remaining = 4;
    s32 entry = datGameState + PTY_ACTIVE_ROSTER_OFFSET;
    do {
        remaining--;
        if ((((Entry1A4 *)entry)->flags & 1) != 0) {
            activeCount++;
            levelSum += ((Entry1A4 *)entry)->level;
        }
        entry += PTY_ACTIVE_ROSTER_STRIDE;
    } while (remaining >= 0);
    if (activeCount == 0) {
        return 0;
    }
    return (levelSum + activeCount - 1) / activeCount;
}

extern void ptyAccumulateStatGains(s32 *, s32, u8 *);
extern s32 ptyComputeTotalExp(u8 *, s32);
extern void ptyRecomputeMaxHpMp(u32);
extern void evtCopyRosterTableValue(s32);
extern void func_00286618(void);
extern void func_002866C8(void);
extern void func_003140C8(s32, u8 *);
extern Entry1A4 *D_00435DD4;
void ptyAssignRosterItemAndMarkOwned(Entry1A4 *entry);

/* Clone an entry template and raise it to the maximum occupied party level. */
void func_0011B328(Entry1A4 *entry, s32 templateIndex) {
    s32 targetLevel = dds3EntryMax();
    s32 statGains[PTY_ENTRY_STAT_COUNT];
    u8 *stat;
    s32 *gain;
    s32 remaining;

    *entry = D_00435DD4[templateIndex];
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, (u8 *)entry);
        stat = entry->stats;
        gain = statGains;
        for (remaining = PTY_ENTRY_STAT_COUNT - 1; remaining >= 0; remaining--) {
            *stat++ += *gain++;
        }
        entry->level = targetLevel;
    }
    entry->totalExp = ptyComputeTotalExp((u8 *)entry, 0);
    func_003140C8(0, (u8 *)entry);
    evtCopyRosterTableValue((s32)entry);
    ptyAssignRosterItemAndMarkOwned(entry);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B4B0);


/* Clone template 1 into roster 2 and inherit its mantra state, then clear the
 * assigned item. The maximum-party-level flag overrides keep-base-level;
 * otherwise use the rounded party average only when keep-base-level is clear. */
void func_0011B6F0(Entry1A4 *entry, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[PTY_ENTRY_STAT_COUNT];
    s32 statIndex;
    PtyStatePrefix *gameState;

    memcpy(entry, &((PtyStatePrefix *)datGameState)->templates[1], sizeof(*entry));
    entry->rosterIndex = 2;
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, (u8 *)entry);
        for (statIndex = 0; statIndex < PTY_ENTRY_STAT_COUNT; statIndex++) {
            entry->stats[statIndex] += statGains[statIndex];
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp((u8 *)entry, 0);
        ptyRecomputeMaxHpMp((u32)entry);
    }
    evtCopyRosterTableValue((s32)entry);
    gameState = (PtyStatePrefix *)datGameState;
    memcpy(&gameState->mantraBits[2], &gameState->mantraBits[1], sizeof(gameState->mantraBits[2]));
    memcpy(gameState->mantraProgress[2], gameState->mantraProgress[1], sizeof(gameState->mantraProgress[2]));
    func_00286618();
    entry->itemId = 0;
}

/* Clone template 7 into roster 3 and inherit its mantra state. After optional
 * post-initialization, mark any assigned item owned. Level-flag precedence is
 * the same as the adjacent initializer; flag mask 2 skips the optional call. */
void func_0011B9A0(Entry1A4 *entry, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[PTY_ENTRY_STAT_COUNT];
    s32 statIndex;
    PtyStatePrefix *gameState;

    memcpy(entry, &((PtyStatePrefix *)datGameState)->templates[7], sizeof(*entry));
    entry->rosterIndex = 3;
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, (u8 *)entry);
        for (statIndex = 0; statIndex < PTY_ENTRY_STAT_COUNT; statIndex++) {
            entry->stats[statIndex] += statGains[statIndex];
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp((u8 *)entry, 0);
        ptyRecomputeMaxHpMp((u32)entry);
    }
    gameState = (PtyStatePrefix *)datGameState;
    memcpy(&gameState->mantraBits[3], &gameState->mantraBits[7], sizeof(gameState->mantraBits[3]));
    memcpy(gameState->mantraProgress[3], gameState->mantraProgress[7], sizeof(gameState->mantraProgress[3]));
    func_002866C8();
    if (!(initFlags & 2)) {
        func_003140C8(1, (u8 *)entry);
    }
    if (entry->itemId != 0) {
        ((PtyStatePrefix *)datGameState)->inventory.counts[entry->itemId] = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011BC80);

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

void evtCopyRosterTableValue(s32 entryAddress) {
    ((Entry1A4 *)entryAddress)->tableValue = D_00386248[((Entry1A4 *)entryAddress)->rosterIndex].value;
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
                    evtCopyRosterTableValue(entry);
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
    Entry4 *updates = D_003862C8;
    u32 updateIndex = 0;

    do {
        u16 valueIndex = updates->unk0;
        u8 delta = updates->unk2;

        updates++;
        if (valueIndex != 0) {
            func_0011A118(valueIndex, delta);
        }
        updateIndex++;
    } while (updateIndex < 2);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CA88);

/* Visit occupied roster slots for native per-entry processing. */
void dds3ForEachFlagged(void) {
    s32 entryOffset = 0;
    s32 remaining = 4;

    do {
        Entry1A4 *entry = (Entry1A4 *)(datGameState + entryOffset + PTY_ACTIVE_ROSTER_OFFSET);

        if (entry->flags & 1) {
            func_0011CA88(entry);
        }
        entryOffset += PTY_ACTIVE_ROSTER_STRIDE;
        remaining--;
    } while (remaining >= 0);
}

void ptyClearSelectedSkillFlagsFromActiveEntries(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        s32 entry = datGameState + offset + 0xA60;
        offset += 0x1C4;
        if ((((Entry1A4 *)entry)->flags & 1) != 0) {
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5B);
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5C);
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5D);
        }
        remaining--;
    } while (remaining >= 0);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D0D8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D130);

void evtRandomizeEntryValue(s32 unit) {
    s32 randomOffset;

    randomOffset = effMiscRandMod(0, 4);
    *(s32 *)(unit + 0x1B4) = 0x12 - randomOffset;
}

/* Clear a subset of per-unit status flags on occupied qualifying entries,
 * gated by the event RNG; report whether any flags were cleared. */
s32 evtClearRandomStatusFlags(void) {
    s32 clearedAny = 0;
    s32 remaining;
    s32 entryAddress;
    s32 roll = (s32)effMiscRandMod(0, 100);
    if (roll >= 51) {
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

s32 ptyGetCombinedRecordAndSlotValue(s32 id, s32 slot) {
    s32 index = id - 0xC0;
    s32 tableOffset = index * 6;
    s32 flagOffset = slot + index * 5;
    if (index <= 0) {
        return 0;
    }
    return *(s8 *)(tableOffset + D_00435E3C + slot) +
           *(u8 *)(flagOffset + datGameState + 0x1E670);
}

void ptyAddClampedEntryValue(s32 base, s32 offset, s32 amount) {
    s8 *value = (s8 *)(offset + base + 0x16);
    s32 updated = *value + amount;
    if (updated < 0) {
        updated = 0;
    }
    if (updated >= 100) {
        updated = 99;
    }
    *value = updated;
}

void ptyAssignRosterItemAndMarkOwned(Entry1A4 *entry) {
    s32 value = D_00386288[entry->rosterIndex];
    mnuSetPartyEntryCurrentId(entry, value);
    if (value != 0) {
        ((SaveItemCounts *)datGameState)->counts[value] = 1;
    }
}

void ptyAssignPartyRosterItemsAndMarkOwned(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(datGameState + offset + 0xA60);
        if (entry->flags & 1) {
            s32 index = 0;
            do {
                if (entry->rosterIndex == index) {
                    ptyAssignRosterItemAndMarkOwned(entry);
                }
                index++;
            } while (index < 16);
        }
        remaining--;
        offset += 0x1C4;
    } while (remaining >= 0);
}

/* Step the script with these context values; clear the written flag, not the stored result. */
s32 evtRunContext(s32 script, s32 first, s32 second, s32 third, u16 options) {
    func_0010C058(evtWorkScriptTask, script);
    D_0043E5C0[1] = third;
    D_0043E5C0[2] = first;
    D_0043E5C0[3] = second;
    ((EventScriptEntry *)D_0043E5C0)->index14 = options;
    ((EventScriptEntry *)D_0043E5C0)->flags &= EVT_RESULT_RESET_MASK;
    bfStepContext(evtWorkScriptTask);
    return D_0043E5C0[4];
}

/* Create the script task and clear its separate 24-byte context. */
void dds3WorkInit(void) {
    evtWorkScriptTask = scrCreateTaskWithDefaultOption();
    memset(D_0043E5C0, 0, EVT_CONTEXT_BYTES);
}

u32 scrGetWorkTaskHandle(void) {
    return evtWorkScriptTask;
}

void scrDestroyWorkTask(void) {
    scrProcDestroyTask(evtWorkScriptTask);
    evtWorkScriptTask = 0;
}

s32 evtPushFirstRosterLevel(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5C8[0])->index14);
    return 1;
}

s32 evtPushSecondRosterLevel(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5CC[0])->index14);
    return 1;
}

s32 evtPushFirstRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5C8[0])->value6);
    return 1;
}

s32 evtPushSecondRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5CC[0])->value6);
    return 1;
}

s32 evtPushFirstRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5C8[0])->value8);
    return 1;
}

s32 evtPushSecondRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5CC[0])->value8);
    return 1;
}

extern s32 btlResolveUnitValueWithOverride(s32, s32);
extern u32 datReadLowHalfOfCalculatedValue(s32, s32);
/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void) {
    s8 statIndex = ((EventModeSlot *)datCommandSelectors)[D_0043E5C0[1]].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0[2], statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0[2], statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void) {
    s8 statIndex = ((EventModeSlot *)datCommandSelectors)[D_0043E5C0[1]].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0[3], statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0[3], statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

extern s32 datFlagToElementIndex(s32);

/* Push the first entry's option-selected stat, retaining the same default selectors. */
s32 evtPushFirstRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(((EventScriptEntry *)D_0043E5C0)->index14);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0[2], statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0[2], statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's option-selected stat, retaining the same default selectors. */
s32 evtPushSecondRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(((EventScriptEntry *)D_0043E5C0)->index14);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0[3], statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(D_0043E5C0[3], statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride(D_0043E5C8[0], statIndex));
    return 1;
}

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride(D_0043E5CC[0], statIndex));
    return 1;
}

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void) {
    s32 statValue;
    s32 commandIndex = D_0043E5C0[1];
    if (((EventModeSlot *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((EventScriptEntry *)D_0043E5C0[2])->rosterIndex;
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
    s32 commandIndex = D_0043E5C0[1];
    statValue = ((EventStatRecord *)datCommandRecords)[commandIndex].stat25;
    if (((EventModeSlot *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((EventScriptEntry *)D_0043E5C0[2])->rosterIndex;
        statValue = (s32)((f32)statValue * ((EventRosterStat *)datRosterDetails)[rosterIndex].multiplier);
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatGrade(void) {
    scrSetIntegerReturnValue(((EventStatRecord *)datCommandRecords)[D_0043E5C4[0]].stat2D);
    return 1;
}

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void) {
    s32 *contextWords = D_0043E5C0;
    s32 statValue;
    u16 statOption = ((EventScriptEntry *)contextWords)->index14;
    switch (statOption) {
    case 1:
        statValue = ((EventStatRecord *)datCommandRecords)[contextWords[1]].stat18;
        break;
    case 2:
        statValue = ((EventStatRecord *)datCommandRecords)[contextWords[1]].stat1C;
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
    s32 *contextWords = D_0043E5C0;
    s32 statValue;
    u16 statOption = ((EventScriptEntry *)contextWords)->index14;
    u16 commandIndex = ((EventIndexRecord *)datItemSkillRecords)[((Entry1A4 *)contextWords[2])->tableValue].index;
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
    s32 commandIndex = D_0043E5C0[1];
    if (((EventModeSlot *)datCommandSelectors)[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((EventScriptEntry *)D_0043E5C0[2])->rosterIndex;
        statValue = ((EventRosterStat *)datRosterDetails)[rosterIndex].alternateB;
    } else {
        statValue = ((EventStatRecord *)datCommandRecords)[commandIndex].stat34;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatMaximum(void) {
    scrSetIntegerReturnValue(((EventStatRecord *)datCommandRecords)[D_0043E5C4[0]].stat36);
    return 1;
}

/* Store the script argument as the context result and mark it written. */
s32 evtStoreScriptParameterResult(void) {
    u16 stateFlags = ((EventScriptEntry *)D_0043E5C0)->flags | EVT_RESULT_WRITTEN_FLAG;

    ((EventScriptEntry *)D_0043E5C0)->flags = stateFlags;
    D_0043E5C0[4] = scrReadIntParameter(0);
    return 1;
}

s32 evtPushScriptContextResult(void) {
    scrSetIntegerReturnValue(D_0043E5D0[0]);
    return 1;
}

s32 evtPushEntryFlagBitInverted(void) {
    scrSetIntegerReturnValue((((((EventScriptEntry *)D_0043E5C8[0])->flags) >> 5) ^ 1) & 1);
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

extern s32 datBattleParameters;

/* The paired DDS1 readers identify this index as the entry's level.
 * Keep the native index type, each distinct table offset, and completion value. */
s32 func_0011DFE0(void) {
    s32 entryAddress = D_0043E5C8[0];
    u32 levelIndex = ((EventScriptEntry *)entryAddress)->index14;
    scrSetFloatReturnValue(*(f32 *)((datBattleParameters - 4) + levelIndex * 4));
    return 1;
}

s32 func_0011E018(void) {
    s32 entryAddress = D_0043E5C8[0];
    u32 levelIndex = ((EventScriptEntry *)entryAddress)->index14;
    scrSetFloatReturnValue(*(f32 *)(datBattleParameters + levelIndex * 4 + 0x188));
    return 1;
}

extern s32 datComputeSkillBoostedMaxHp(s32);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(D_0043E5C0[3]);
    s32 currentHp = ((Entry1A4 *)D_0043E5C0[3])->unk6;
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
    scrSetFloatReturnValue(*(f32 *)(datBattleParameters + gradeIndex * 4 + 0x318));
    return 1;
}

s32 func_0011E128(void) {
    s32 entryAddress = D_0043E5C8[0];
    u32 levelIndex = ((EventScriptEntry *)entryAddress)->index14;
    scrSetFloatReturnValue(*(f32 *)(datBattleParameters + levelIndex * 4 + 0x360));
    return 1;
}

s32 func_0011E160(void) {
    s32 entryAddress = D_0043E5C8[0];
    u32 levelIndex = ((EventScriptEntry *)entryAddress)->index14;
    scrSetFloatReturnValue(*(f32 *)(datBattleParameters + levelIndex * 4 + 0x4EC));
    return 1;
}

s32 func_0011E198(void) {
    s32 entryAddress = D_0043E5C8[0];
    u32 levelIndex = ((EventScriptEntry *)entryAddress)->index14;
    scrSetFloatReturnValue(*(f32 *)(datBattleParameters + levelIndex * 4 + 0x678));
    return 1;
}

s32 func_0011E1D0(void) {
    s32 entryAddress = D_0043E5C8[0];
    u32 levelIndex = ((EventScriptEntry *)entryAddress)->index14;
    scrSetFloatReturnValue(*(f32 *)(datBattleParameters + levelIndex * 4 + 0x678));
    return 1;
}

s32 evtRollFlagDependentResultCode(void) {
    s32 value;
    if ((((EventScriptEntry *)D_0043E5CC[0])->flags & 0x20) == 0) {
        value = effMiscRandMod(0, 0x20) != 0 ? 0xA : 0x80;
    } else {
        value = effMiscRandMod(0, 0x30) != 0 ? 0 : 0x80;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushRosterBaseValue(void) {
    u16 index = ((EventScriptEntry *)D_0043E5C8[0])->rosterIndex;
    scrSetIntegerReturnValue(((EventRosterStat *)datRosterDetails)[index].base);
    return 1;
}

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectFineStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp(D_0043E5C0[3]);
    s32 currentHp = ((Entry1A4 *)D_0043E5C0[3])->unk6;
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
    scrSetFloatReturnValue(*(f32 *)(datBattleParameters + gradeIndex * 4 + 0x338));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E3A0);

s32 evtPushRosterOrGlobalCounterValue(void) {
    s32 entry = D_0043E5C8[0];
    s32 result;
    if (!(((EventScriptEntry *)entry)->flags & 0x20)) {
        result = *(s32 *)(datGameState + 0x3C);
    } else {
        s32 index = ((EventScriptEntry *)entry)->rosterIndex;
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

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E528);

/* Compare truncated averages: enemy average + party average / 4 >= party average.
 * Requires a nonzero scene, a nonempty enemy list, and party entries carrying
 * all three flag masks 1/2/4. No eligible entries returns zero.
 * count and remaining serve both loops; word holds enemy IDs, then entry flags.
 * partyLevel is accumulated first and divided in place before comparison. */
s32 func_0011E728(s32 sceneIndex) {
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

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E848);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E930);

void dds3WorkClear(void) {
    s32 base = datGameState;

    *(s32 *)(base + 0x1440) = 0;
    *(s16 *)(base + 0x1444) = 0;
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

