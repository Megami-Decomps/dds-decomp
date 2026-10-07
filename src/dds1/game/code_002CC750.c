#include "dsp_name.h"
#include "common.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "itf.h"
#include "dat_state.h"

#define SCR_FLAG_ID_MASK 0xFFFF
#define SCR_FLAG_SLOT_INDEX_MASK 7
#define SCR_FLAG_WORD_INDEX_SHIFT 3
#define SCR_FLAG_SLOT_BIT_SHIFT 2
#define SCR_FLAG_PRIMARY_BIT 1U
#define SCR_SKILL_STATE_TWO_BIT 2U
#define SCR_FLAG_SECONDARY_BIT 4U
#define SCR_SECONDARY_FLAG_COUNT 0x260
#define SCR_GLOBAL_FLAG_FIRST_ID 0x1AB
#define SCR_GLOBAL_FLAG_END_ID 0x200
/* Adding this bias in a u16 subtracts the first ID by wrapping. */
#define SCR_GLOBAL_FLAG_WRAP_BIAS 0xFE55
#define SCR_GLOBAL_FLAG_WORD_SHIFT 5
#define SCR_GLOBAL_FLAG_BIT_MASK 31
#define PTY_SKILL_SLOT_COUNT 24
#define PRF_SKILL_LIST_ENTRY_COUNT 8
#define PRF_SKILL_LIST_FLAGGED 4

#define PTY_PRESET_SKILL_SLOT_COUNT 8
#define PTY_PRESET_POOL_SKILL_COUNT 40
/* Observed gate for the post-mark call; its gameplay meaning is not established. */
#define PTY_PRESET_POOL_EXTRA_GATE 0xB90
#define PTY_PROFILE_UNIT_LAST_INDEX 4
#define PTY_PROFILE_UNIT_ID_COUNT 0x10
#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1
/* Stride in u16 entries, not bytes; only the first eight are skill IDs. */
#define PRF_SKILL_TABLE_STRIDE 14
#define PRF_PROFILE_COUNT 0x60
#define PRF_REQUIRED_PROFILE_COUNT 8
#define PRF_REQUIRED_PROFILE_IDS_OFFSET 0xC
#define PRF_FALLBACK_GROUP_COUNT 4
#define PRF_FALLBACK_FLAG_COUNT 2
#define PRF_REQUIREMENT_RULES_BIT 4
#define PRF_REQUIREMENT_EXCLUSION_BIT 1
#define SCR_PAIRED_OUTPUT_BYTES 8
#define SDF_FLAG_LIST_VALUE_SHIFT 3
#define SDF_FLAG_LIST_RESET_MARK 0xFFFFFFFF
#define SDF_FLAG_LIST_ENTRY_VERTEX_BYTES 32
#define SDF_FLAG_LIST_VERTICES_PER_ENTRY 2
#define SDF_FLAG_LIST_PACKET_BYTES 0x20
#define SDF_FLAG_SLOT_TABLE_SHIFT 7
#define SDF_FLAG_SLOT_COUNT 16

extern void memset();
/* DDS1 preset rows are 0xC0 bytes; the skill lists at +0x60/+0x70 belong
 * to the same row as its four script choices and four profile sets. */
typedef struct PtyPresetScriptChoice {
    u16 scriptId;
    u16 initialValue;
    u16 requiredProfiles[4];
} PtyPresetScriptChoice;

typedef struct PtyPresetRecord {
    PtyPresetScriptChoice scriptChoices[4];
    u8 profileIds[4][12];
    u16 skillSlots[8];
    u16 poolSkills[40];
} PtyPresetRecord;

extern PtyPresetRecord D_00393A80[];
extern void ptyMergeStockSkills(DatPartyRecord *);

extern void (*sdfTickCallback)(void);

extern u64 sdfAllocateBlockBySizeThreshold(u64);

extern u32 D_003BD2C8;

extern u32 prfGetCapValue(u16);

extern u32 prfIsRequirementExcluded(u16);

extern DatProfileRecord *ptyGetProfileRecord(DatPartyRecord *, u16);


/* Profile prerequisite operands reuse +0x08 as the required threshold. */
typedef struct PrfRequirementOperand {
    u8 pad00[8];
    u32 requiredValue;      /* 0x08 */
    u8 profileIds[8];       /* 0x0C */
} PrfRequirementOperand;


extern u8 frFontColoredGlyphResource[];

typedef struct PrfItemRequirement {
    s32 id;
    s32 minimum;
} PrfItemRequirement;

/* A24-byte prerequisite entry owns two inventory counts and its result flag. */
typedef struct Entry24B {
    u8 v0;
    u8 pad01[3];
    PrfItemRequirement requirement[2];
    u32 flagId;
} Entry24B;

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18


extern s32 CreateSema(void *);

extern Entry24B D_00393220[];

extern Entry24W D_00393234[];

/* 84-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry84W {
    u32 v0;             // 0x00
    u8 pad_0x04[0x50]; // 0x04
} Entry84W; // 0x54

extern void ptySetProfileFlag1(DatPartyRecord *, u16);

extern Entry84W D_00391230[];

/* 28-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry28W {
    u32 v0;             // 0x00
    u8 pad_0x04[0x18]; // 0x04
} Entry28W; // 0x1C

typedef struct Entry28B {
    u8 v0;             // 0x00
    u8 pad_0x01[0x1B]; // 0x01
} Entry28B; // 0x1C

typedef struct Entry28H {
    u16 v0;            // 0x00
    u8 pad_0x02[0x1A]; // 0x02
} Entry28H; // 0x1C


extern Entry28W D_003907B8[];

extern Entry28B D_003907B4[];

extern Entry28H D_003907B6[];

extern Entry28B D_003907B5[];

extern Entry28W D_003907B0[];
extern s32 func_002FE950(const char *, const char *);
extern void func_002FE978(s32, const char *, s32, s32);
extern void func_002FE360(s32);
extern char sdfDebugLogAppendMode[];
extern char sdfDebugLogPairFormat[];
extern u32 itfDrawBankTextWithLayoutFlags(s32, s32, u32, u16, u32, u32);
extern void frFontSetChildColors(u32, u32);
extern void func_001958A0(u32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(u32);
extern void *fileResolvePrimaryBuffer(void *);
extern s32 ptyTestProfileFlag0(DatPartyRecord *, u16);
extern u16 D_003907BC[];
DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
s32 ptyGetCurrentProfileId(DatPartyRecord *);


extern void ptyRecomputeMaxVitals(DatPartyRecord *, const s32 *);

void sdfAppendFormattedDebugLogPair(s32 left, s32 right) {
    s32 file = func_002FE950("debug.log", sdfDebugLogAppendMode);
    if (file != 0) {
        func_002FE978(file, sdfDebugLogPairFormat, left, right);
        func_002FE360(file);
    }
}

/* Clear the complete per-unit profile-record table in game state. */
void ptyClearProfileRecords(void) {
    memset(datGameState->profileRecords, 0, sizeof(datGameState->profileRecords));
}

extern void ptySelectProfileStage(DatPartyRecord *);
INCLUDE_ASM(const s32, "game/code_002CC750", ptySelectProfileStage);

extern void ptyApplyProfilePreset(s32, DatPartyRecord *);
INCLUDE_ASM(const s32, "game/code_002CC750", ptyApplyProfilePreset);

/* Load nonzero preset IDs into their original slots; zero IDs leave slots unchanged. */
void ptyLoadPresetSkillSlots(DatPartyRecord *unit) {
    u16 *presetSkills = D_00393A80[unit->unitId].skillSlots;
    u16 *skillCursor = unit->effectData;
    u32 presetIndex;
    presetIndex = 0;
    do {
        u16 skillId = *presetSkills++;
        if (skillId != 0) {
            scrSetFlag(unit, skillId);
            *skillCursor = skillId;
        }
        skillCursor++;
        presetIndex++;
    } while (presetIndex < PTY_PRESET_SKILL_SLOT_COUNT);
}

/* Mark nonzero IDs in the preset pool, then perform the native flag-gated extra call. */
void ptyMarkPresetSkillPool(DatPartyRecord *unit) {
    u16 *presetSkills = D_00393A80[unit->unitId].poolSkills;
    u32 presetIndex = 0;
    do {
        u16 skillId = *presetSkills++;
        if (skillId != 0) {
            scrSetFlag(unit, skillId);
        }
        presetIndex++;
    } while (presetIndex < PTY_PRESET_POOL_SKILL_COUNT);
    if (mdlFlagTest(PTY_PRESET_POOL_EXTRA_GATE)) {
        ptyMergeStockSkills(unit);
    }
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyRebuildProfileSkills);

void ptyRebuildProfileSkills(s32 useCurrentProfile, DatPartyRecord *unit);

/* Rebuild skill lists for the five occupied party slots. */
void ptyRebuildAllProfiles(void) {
    s32 unitIndex;
    for (unitIndex = 0; unitIndex < 5; unitIndex++) {
        DatPartyRecord *unit = &datGameState->party[unitIndex];
        if ((unit->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
            s32 candidateUnitId;
            for (candidateUnitId = 0; candidateUnitId < PTY_PROFILE_UNIT_ID_COUNT; candidateUnitId++) {
                if (unit->unitId == candidateUnitId) {
                    ptyRebuildProfileSkills(0, unit);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyRecomputeMaxVitals);

void ptyRecomputeMaxHpMp(DatPartyRecord *unit) {
    ptyRecomputeMaxVitals(unit, NULL);
}

extern DspMantraName *D_003BAA78;
extern u32 strlen(const char *text);
extern char *strcpy(char *destination, const char *source);
extern void *memcpy(void *destination, const void *source, u32 size);

void func_002CD0D8(u16 scriptId, s32 mode, char *destination) {
    u8 *name = D_003BAA78[scriptId].encodedText;
    s32 split = 0;
    s32 offset = 0;

    for (; offset < strlen((char *)name); offset += 2) {
        if (name[offset] == 0x82 && name[offset + 1] == 0x8A) {
            split = offset;
        }
    }
    switch (mode) {
    case 1:
        if (split >= 4) {
            strcpy(destination, (char *)&name[split + 2]);
        } else {
            strcpy(destination, (char *)name);
        }
        return;
    case 2:
        memcpy(destination, name, split);
        destination[split] = 0;
        break;
    default:
        strcpy(destination, (char *)name);
        break;
    }
}


INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD240);

/* Read the configured capacity for an unchecked profile ID. */
u32 prfGetCapValue(u16 profileId) {
    return D_003907B8[profileId].v0;
}

/* Store the configured capacity in an unchecked profile record. */
void ptySetProfileRecordToCap(DatPartyRecord *unitAddress, u16 profileId) {
    DatProfileRecord *record;
    u32 recordCap;

    record = ptyGetProfileRecord(unitAddress, profileId);
    recordCap = prfGetCapValue(profileId);
    record->value = recordCap;
}

/* Add to the selected record and cap the unsigned result; no selection returns zero. */
u32 ptyAddProfilePoints(DatPartyRecord *unit, s32 increment) {
    DatProfileRecord *record;
    u32 recordCap;
    u32 recordValue;
    s32 selectedProfile;
    if (ptyGetCurrentProfileId(unit) == 0) {
        return 0;
    }
    record = ptyGetCurrentProfileRecord(unit);
    selectedProfile = unit->profileId;
    record->value += increment;
    recordCap = prfGetCapValue(selectedProfile & SCR_FLAG_ID_MASK);
    recordValue = record->value;
    if (recordCap < recordValue) {
        record->value = recordCap;
        recordValue = recordCap;
    }
    return recordValue;
}

void prfDecodeFlagPair(u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 0xF;
    v >>= 4;
    *a = v;
    *b = lo << 1;
}

typedef struct ScriptFlagSlot {
    u8 id;             /* 0x00 */
    u8 pad01[3];
    u32 flag;          /* 0x04 */
} ScriptFlagSlot;

extern ScriptFlagSlot D_00393280[];

/* Search the selected 16-entry table for an exact nonzero byte ID.
 * Set and return its global flag ID; return zero if no entry matches. */
u32 sdfSetFlagBySlotId(DatPartyRecord *unit, u32 slotId) {
    ScriptFlagSlot *slotCursor = (ScriptFlagSlot *)((u8 *)D_00393280 + (unit->unitId << SDF_FLAG_SLOT_TABLE_SHIFT));
    u32 slotIndex;

    for (slotIndex = 0; slotIndex < SDF_FLAG_SLOT_COUNT; slotIndex++, slotCursor++) {
        if (slotCursor->id != 0 && slotCursor->id == slotId) {
            u32 globalFlagId = slotCursor->flag;
            mdlFlagSet(globalFlagId);
            return globalFlagId;
        }
    }
    return 0;
}

extern void ptyApplyProfile(DatPartyRecord *, u16);
INCLUDE_ASM(const s32, "game/code_002CC750", ptyApplyProfile);

s32 ptyTestProfileFlag0(DatPartyRecord *work, u16 id) {
    u32 word;
    u32 shift;

    prfDecodeFlagPair(id, &word, &shift);
    return (datGameState->mantraBits[work->unitId].words[word] & (1 << shift)) != 0;
}

/* Require the primary profile flag for every row not marked as excluded. */
s32 scrCheckStateBits(DatPartyRecord *unit) {
    u32 profileIndex = 0;
    do {
        u16 profileId = profileIndex;
        profileIndex++;
        if ((prfIsRequirementExcluded(profileId) & PRF_REQUIREMENT_EXCLUSION_BIT) == 0 &&
            !ptyTestProfileFlag0(unit, profileId)) {
            return 0;
        }
    } while (profileIndex < PRF_PROFILE_COUNT);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptySetProfileFlag1);

s32 ptyTestProfileFlag1(DatPartyRecord *work, u16 id) {
    u32 word;
    u32 shift;

    prfDecodeFlagPair(id, &word, &shift);
    shift++;
    return (datGameState->mantraBits[work->unitId].words[word] & (1 << shift)) != 0;
}

s8 scrGetSelectedOperandIndex(DatPartyRecord *op) {
    return op->profileId;
}

/* Address an unchecked profile record within the unit's record bank. */
DatProfileRecord *ptyGetProfileRecord(DatPartyRecord *unit, u16 profileId) {
    return &datGameState->profileRecords[unit->unitId][profileId];
}

u32 ptyGetProfileRecordValue(DatPartyRecord *unit, u16 profileId) {
    DatProfileRecord *record;

    record = ptyGetProfileRecord(unit, profileId);
    return record->value;
}

/* Resolve the selected ID using the native low-16-bit conversion. */
DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *unit) {
    return ptyGetProfileRecord(unit, scrGetSelectedOperandIndex(unit) & SCR_FLAG_ID_MASK);
}

s32 ptyGetCurrentProfileId(DatPartyRecord *op) {
    return op->profileId;
}

/* Store the narrowed selection, mark its paired profile flag, and return the stored ID. */
s8 scrSelectOperandIndex(DatPartyRecord *unit, s32 profileId) {
    unit->profileId = profileId;
    ptySetProfileFlag1(unit, profileId & SCR_FLAG_ID_MASK);
    return unit->profileId;
}

u32 func_002CD7F0(u32 arg0, u32 arg1) {
    return arg1;
}

u32 func_002CD7F8(void) {
    return 0;
}

/* Retail stub retains the caller's unused item-ID word. */
u32 func_002CD800(u32 itemId) {
    (void)itemId;
    return 1;
}

/* Decode the low 16 bits: eight four-bit flag slots per word.
 * The unused first argument remains part of the existing calling convention. */
void scrDecodePackedFlagIndex(DatPartyRecord *unused, u32 value, u32 *wordIndex, u32 *bitShift) {
    u32 slotIndex;

    value &= SCR_FLAG_ID_MASK;
    slotIndex = value & SCR_FLAG_SLOT_INDEX_MASK;
    value >>= SCR_FLAG_WORD_INDEX_SHIFT;
    *wordIndex = value;
    *bitShift = slotIndex << SCR_FLAG_SLOT_BIT_SHIFT;
}

/* Set bit 0 of the selected nibble; the return value is always 1. */
s32 scrSetFlag(DatPartyRecord *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    work->skillFlags[wordIndex] |= SCR_FLAG_PRIMARY_BIT << bitShift;
    return 1;
}

/* Set the global bit for an ID in the supported half-open range. */
void scrSetGlobalSeenBit(u32 flagId) {
    u16 bitIndex;
    flagId &= SCR_FLAG_ID_MASK;
    if (flagId < SCR_GLOBAL_FLAG_FIRST_ID) return;
    if (flagId >= SCR_GLOBAL_FLAG_END_ID) return;
    bitIndex = flagId + SCR_GLOBAL_FLAG_WRAP_BIAS;
    datGameState->scriptFlags[bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT] |= SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK);
}

/* Return the global bit's raw word mask, not a normalized boolean. */
u32 scrTestGlobalSeenBit(u32 flagId) {
    u16 bitIndex = flagId;
    if (bitIndex < SCR_GLOBAL_FLAG_FIRST_ID) return 0;
    if (bitIndex >= SCR_GLOBAL_FLAG_END_ID) return 0;
    bitIndex = bitIndex + SCR_GLOBAL_FLAG_WRAP_BIAS;
    return datGameState->scriptFlags[bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT] & (SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK));
}

/* Set only bit 2 of the selected flag nibble. */
void scrSetSecondaryScriptFlag(DatPartyRecord *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    work->skillFlags[wordIndex] |= SCR_FLAG_SECONDARY_BIT << bitShift;
}

/* Clear only bit 2 of the selected flag nibble. */
void scrClearSecondaryScriptFlag(DatPartyRecord *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    work->skillFlags[wordIndex] &= ~(SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Clear secondary bits over the fixed ID range, preserving other nibble bits. */
void scrClearFlags(DatPartyRecord *work) {
    s32 flagId;
    for (flagId = 0; flagId < SCR_SECONDARY_FLAG_COUNT; flagId++) {
        scrClearSecondaryScriptFlag(work, flagId);
    }
}

/* Return bit 2's raw word mask, not a normalized boolean. */
u32 scrGetSecondaryScriptFlag(DatPartyRecord *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    return work->skillFlags[wordIndex] & (SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Bit 1 takes precedence and returns state 2; otherwise return boolean bit 0.
 * The secondary bit does not affect this state. */
u32 ptyGetSkillNibbleState(DatPartyRecord *work, u16 flagId) {
    u32 wordIndex, bitShift;
    u32 flagWord;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    flagWord = work->skillFlags[wordIndex];
    if (flagWord & (SCR_SKILL_STATE_TWO_BIT << bitShift)) {
        return 2;
    }
    return (flagWord & (SCR_FLAG_PRIMARY_BIT << bitShift)) != 0;
}

/* Search all slots for the low 16-bit ID; zero can match an empty slot. */
s32 ptyHasSkill(DatPartyRecord *unit, s32 skillId) {
    u32 maskedSkillId = skillId & SCR_FLAG_ID_MASK;
    u16 *skillSlot = unit->effectData;
    u32 slotIndex = 0;

    do {
        if (*skillSlot++ == maskedSkillId) {
            return 1;
        }
        slotIndex++;
    } while (slotIndex < PTY_SKILL_SLOT_COUNT);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyRemoveProfileSkills);

/* Return the first exact-ID slot, including zero IDs, or -1 when absent. */
s32 scrFindSlot(DatPartyRecord *unit, u16 skillId) {
    u32 slotIndex;
    u16 *skillSlots = unit->effectData;
    for (slotIndex = 0; slotIndex < PTY_SKILL_SLOT_COUNT; slotIndex++) {
        if (skillSlots[slotIndex] == skillId) {
            return slotIndex;
        }
    }
    return -1;
}

/* Read a slot's ID, returning zero for an out-of-range unsigned index. */
u16 scrGetSlot(DatPartyRecord *unit, u32 slotIndex) {
    if (slotIndex >= PTY_SKILL_SLOT_COUNT) {
        return 0;
    }
    return unit->effectData[slotIndex];
}

/* Count nonzero skill IDs across every slot. */
u32 scrCountSlots(DatPartyRecord *unit) {
    u32 occupiedCount = 0;
    u32 slotIndex;

    for (slotIndex = 0; slotIndex < PTY_SKILL_SLOT_COUNT; slotIndex++) {
        if (unit->effectData[slotIndex] != 0) {
            occupiedCount++;
        }
    }
    return occupiedCount;
}

/* Replace an unchecked skill-slot index and return its previous identifier. */
u16 scrSetSlot(DatPartyRecord *unit, s32 slotIndex, u16 skillId) {
    u16 previousSkillId = unit->effectData[slotIndex];

    unit->effectData[slotIndex] = skillId;
    return previousSkillId;
}

/* Clear the first exact-ID match and report whether a slot was found. */
s32 scrRemoveSlot(DatPartyRecord *unit, u16 skillId) {
    s32 slotIndex = scrFindSlot(unit, skillId);
    if (slotIndex >= 0) {
        unit->effectData[slotIndex] = 0;
        return 1;
    }
    return 0;
}

/* Read a byte parameter from the unchecked profile row. */
u8 prfGetParamWord7b4(u16 profileId) {
    return D_003907B4[profileId].v0;
}

/* Read the threshold used by the profile-level check. */
s32 prfGetParamWord7b6(u16 profileId) {
    return D_003907B6[profileId].v0;
}

/* Read a byte parameter from the unchecked profile row. */
u8 prfGetParamWord7b5(u16 profileId) {
    return D_003907B5[profileId].v0;
}

u32 func_002CDDB0(u16 profileId, s32 statIndex) {
    return 0;
}

/* DDS2 reads an indexed profile byte here; DDS1's native callee returns zero.
 * Retain both selection reads and the no-selection fall-through. */
u32 scrCallIfOperandReady(DatPartyRecord *operand, u32 byteIndex) {
    s32 selectedProfileId = ptyGetCurrentProfileId(operand);

    if (ptyGetCurrentProfileId(operand)) {
        return func_002CDDB0(selectedProfileId, byteIndex);
    }
}

/* Read one of the profile row's eight skills; an unsigned out-of-range slot returns zero. */
u16 prfGetSkillAtIndex(u16 profileId, u32 skillIndex) {
    if (skillIndex >= PRF_SKILL_LIST_ENTRY_COUNT) {
        return 0;
    }
    return D_003907BC[profileId * PRF_SKILL_TABLE_STRIDE + skillIndex];
}

u32 func_002CDE60(void) {
    return 0;
}

u32 ptyCheckLevelAtLeastProfileParam7b6(DatPartyRecord *operand, u16 index) {
    u16 value = operand->level;
    if (value < prfGetParamWord7b6(index)) {
        return 0;
    }
    return 1;
}

/* Clear the paired output words; the first two arguments are unused. */
void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, SCR_PAIRED_OUTPUT_BYTES);
}

u8 ptyIsCurrentProfileId(DatPartyRecord *operand, u32 profileId) {
    return operand->profileId == (profileId & 0xffff);
}


s32 prfBuildRawSkillList(u16 profile, PrfSkillList *output) {
    PrfSkillList list;
    u32 i;
    u16 *skills;
    u16 skill;

    memset(&list, 0, sizeof(PrfSkillList));
    list.count = 0;
    skills = &D_003907BC[profile * 14];
    for (i = 0; i < 8; i++) {
        skill = *skills++;
        if (skill != 0) {
            list.flags[list.count] = 0;
            list.skills[list.count] = skill;
            list.count++;
        }
    }
    if (output != NULL) {
        *output = list;
    }
    return list.count;
}

/* Compact the selected profile's nonzero skills, optionally including nonzero
 * nibble states and marking them in the output. The profile argument is unused;
 * output may be NULL when only the resulting count is needed. */
s32 prfBuildSkillList(DatPartyRecord *unit, DatProfileRecord *unusedProfile, PrfSkillList *output, s32 includeFlagged) {
    PrfSkillList compactedList;
    u32 sourceIndex;
    u16 *profileSkillCursor;
    u16 skillId;
    u32 skillState;
    s32 selectedProfileId;

    memset(&compactedList, 0, sizeof(PrfSkillList));
    selectedProfileId = unit->profileId;
    compactedList.count = 0;
    if (selectedProfileId != 0) {
        profileSkillCursor = &D_003907BC[selectedProfileId * PRF_SKILL_TABLE_STRIDE];
        for (sourceIndex = 0; sourceIndex < PRF_SKILL_LIST_ENTRY_COUNT; sourceIndex++) {
            skillId = *profileSkillCursor++;
            if (skillId != 0) {
                skillState = ptyGetSkillNibbleState(unit, skillId);
                if (skillState != 0 && includeFlagged == 0) {
                    continue;
                }
                compactedList.flags[compactedList.count] = 0;
                if (skillState != 0) {
                    compactedList.flags[compactedList.count] |= PRF_SKILL_LIST_FLAGGED;
                }
                compactedList.skills[compactedList.count] = skillId;
                compactedList.count++;
            }
        }
    }
    if (output != NULL) {
        *output = compactedList;
    }
    return compactedList.count;
}

/* Build the stored selection's list without flagged skills; the second argument is unused. */
u32 prfBuildSkillListState0(DatPartyRecord *unit, DatProfileRecord *unusedProfile, PrfSkillList *output) {
    return prfBuildSkillList(unit, unusedProfile, output, 0);
}

/* Count leading profile IDs up to the first zero or the eight-entry bound. */
u32 prfCountProfileList(u8 *operand) {
    u8 *profileIds = operand + PRF_REQUIRED_PROFILE_IDS_OFFSET;
    u32 profileIndex;
    for (profileIndex = 0; profileIndex < PRF_REQUIRED_PROFILE_COUNT; profileIndex++) {
        if (profileIds[profileIndex] == 0) {
            return profileIndex;
        }
    }
    return PRF_REQUIRED_PROFILE_COUNT;
}

/* Require each listed profile's primary flag; an empty list succeeds. */
s32 ptyHasAllReqProfiles(DatPartyRecord *unit, u8 *operand) {
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand + PRF_REQUIRED_PROFILE_IDS_OFFSET;
        do {
            if (ptyTestProfileFlag0(unit, profileIds[profileIndex++]) == 0) {
                return 0;
            }
        } while (profileIndex < profileCount);
    }
    return 1;
}

/* Compare the number of flagged IDs in the list with its minimum count. */
s32 ptyReqProfileCountAtLeast(DatPartyRecord *unit, u8 *operand) {
    u32 matchedCount = 0;
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand + PRF_REQUIRED_PROFILE_IDS_OFFSET;
        do {
            if (ptyTestProfileFlag0(unit, profileIds[profileIndex++]) != 0) {
                matchedCount++;
            }
        } while (profileIndex < profileCount);
    }
    if (matchedCount < ((PrfRequirementOperand *)operand)->requiredValue) {
        return 0;
    }
    return 1;
}

typedef struct PtyReqEntry {
    u8 unk0[5];
    u8 value5;
    u8 pad6[0x16];
} PtyReqEntry;

/* Count flagged profile rows whose byte-five value reaches the operand's threshold. */
s32 ptyProfileCountAtLeast(DatPartyRecord *unit, u8 *operand) {
    PtyReqEntry *profileTable = (PtyReqEntry *)D_003907B0;
    u32 profileIndex;
    u32 matchedCount = 0;

    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        if (ptyTestProfileFlag0(unit, (u16)profileIndex)) {
            if (profileTable[profileIndex].value5 >= operand[5]) {
                matchedCount++;
            }
        }
    }
    if (matchedCount < ((PrfRequirementOperand *)operand)->requiredValue) {
        return 0;
    }
    return 1;
}

/* Each listed profile must be flagged in at least one occupied party slot. */
s32 ptyAreReqProfilesInParty(u8 *operand) {
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand + PRF_REQUIRED_PROFILE_IDS_OFFSET;
        do {
            s32 profilePresent = 0;
            s32 profileId = profileIds[profileIndex];
            s32 unitIndex;
            for (unitIndex = 0; unitIndex < 5; unitIndex++) {
                DatPartyRecord *partyUnit = &datGameState->party[unitIndex];
                if ((partyUnit->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
                    if (ptyTestProfileFlag0(partyUnit, (u16)profileId) != 0) {
                        profilePresent = 1;
                    }
                }
            }
            if (profilePresent == 0) {
                return 0;
            }
            profileIndex++;
        } while (profileIndex < profileCount);
    }
    return 1;
}

/* Compare the unit's level word with the prerequisite's byte threshold. */
u32 prfReqCheckUnitLevel(DatPartyRecord *unit, u8 *operand) {
    if (unit->level < operand[4]) {
        return 0;
    }
    return 1;
}

/* Test the global currency counter against a prerequisite threshold. */
u32 prfReqCheckGlobalCounter(u8 *operand) {
    if (datGameState->header.currency < ((PrfRequirementOperand *)operand)->requiredValue) {
        return 0;
    }
    return 1;
}

extern s32 prfReqEvaluateRules(u32, u32, u16, u32 *);
INCLUDE_ASM(const s32, "game/code_002CC750", prfReqEvaluateRules);

void prfReq54Evaluate(u32 state, u32 operand, u16 requirementId) {
    prfReqEvaluateRules(state, operand, requirementId, 0);
}

/* Read requirement flags for an unchecked profile ID. */
u32 prfIsRequirementExcluded(u16 requirementId) {
    return D_003907B0[requirementId].v0;
}

/* Read the rule-state word for an unchecked requirement ID. */
u32 prfReq54GetWord1230(u16 requirementId) {
    return D_00391230[requirementId].v0;
}

typedef struct PrfFallbackGroup {
    u8 id;
    u8 pad01[3];
    u32 flags[2];
} PrfFallbackGroup;

extern PrfFallbackGroup D_003931B0[];
/* A zero evaluator return selects its result; otherwise test the fallback flags.
 * Missing fallback IDs succeed, as do entries with no nonzero flag requirements. */
s32 prfReqCheckWithFallback(void *operand, u16 requirementId) {

    u32 ruleResult;
    u32 groupIndex;
    u32 flagIndex;

    requirementId &= SCR_FLAG_ID_MASK;
    if ((prfReq54GetWord1230(requirementId) & PRF_REQUIREMENT_RULES_BIT) != 0) {
        if (prfReqEvaluateRules(0, (u32)operand, requirementId, &ruleResult) == 0) {
            return ruleResult != 0;
        }
    }
    for (groupIndex = 0; groupIndex < PRF_FALLBACK_GROUP_COUNT; groupIndex++) {
        if (D_003931B0[groupIndex].id == requirementId) {
            for (flagIndex = 0; flagIndex < PRF_FALLBACK_FLAG_COUNT; flagIndex++) {
                u32 flagId = D_003931B0[groupIndex].flags[flagIndex];

                if (flagId != 0 && mdlFlagTest(flagId) == 0) {
                    return 0;
                }
            }
            return 1;
        }
    }
    return 1;
}

/* Select the first active, satisfied prerequisite group whose flag is unset. */
s32 prfSelectSatisfiedUnsetRequirementGroup(u32 start) {
    u32 i;

    for (i = start; i < PRF_FALLBACK_GROUP_COUNT; i++) {
        if (D_00393220[i].v0 != 0) {
            s32 met = 1;
            u32 j;
            u32 flagId = D_00393220[i].flagId;

            for (j = 0; j < 2; j++) {
                s32 id = D_00393220[i].requirement[j].id;
                s32 minimum = D_00393220[i].requirement[j].minimum;

                if (id != 0) {
                    if (datGameState->inventory.counts[id] < minimum) {
                        met = 0;
                    }
                }
            }
            if (met != 0 && mdlFlagTest(flagId) == 0) {
                return i;
            }
        }
    }
    return -1;
}

/* Read the byte in an unchecked prerequisite-table entry. */
u8 prfReq18GetWord3220(s32 entryIndex) {
    return D_00393220[entryIndex].v0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", prfReqGetPair);
/* Read the flag ID in an unchecked prerequisite-table entry. */
u32 prfReq18GetWord3234(s32 entryIndex) {
    return D_00393234[entryIndex].v0;
}

/* Set flags returned by the native selector until it yields a negative index. */
void sdfSetAllFlagsFromTable(void) {
    s32 entryIndex = 0;
    do {
        entryIndex = prfSelectSatisfiedUnsetRequirementGroup(entryIndex);
        if (entryIndex >= 0) {
            u32 flagId = prfReq18GetWord3234(entryIndex);
            if (flagId != 0) {
                mdlFlagSet(flagId);
            }
        }
    } while (entryIndex++ >= 0);
}

Entry84W *prfReqGetEntryRecord(u16 index) {
    return &D_00391230[index];
}

void frFontQueueColoredGlyph(s32 x, s32 y, u32 first, u16 width, u32 second, s32 option) {
    u32 handle = itfDrawBankTextWithLayoutFlags(x, y, first, width, (u32)frFontColoredGlyphResource, 0);
    frFontSetChildColors(handle, second);
    func_001958A0(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
}

u8 *frFontGetColoredGlyphResource(void) {
    return frFontColoredGlyphResource;
}


/* Reset each entry's timer to free, then clear both colour words per entry. */
void sdfResetFlagListEntries(SdfFlagListWork *work) {
    u32 entryCount;
    SdfFlagListMark *markCursor;
    u32 entryIndex;

    entryIndex = 0;
    entryCount = work->params.count;
    markCursor = work->marks;
    if (entryCount != 0) {
        do {
            entryIndex = entryIndex + 1;
            markCursor->timer = SDF_FLAG_LIST_RESET_MARK;
            markCursor++;
        } while (entryIndex < entryCount);
    }
    memset(work->colors, 0, entryCount << SDF_FLAG_LIST_VALUE_SHIFT);
}

extern s32 sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(u32);

SdfFlagListWork *func_002CEAE8(const SdfFlagListParams *source) {
    u32 count;
    u32 arrayBytes;
    u32 resource;
    u8 *buffer;
    SdfFlagListWork *work;

    count = source->count;
    arrayBytes = count * (sizeof(f32[2][4]) + sizeof(u32[2]) + sizeof(SdfFlagListMark));
    resource = sdfAllocGeneralBlock(arrayBytes + sizeof(SdfFlagListWork));
    buffer = (u8 *)sdfResourceRetainAddress(resource);
    work = (SdfFlagListWork *)(buffer + arrayBytes);
    work->vertices = (f32 (*)[4])buffer;
    buffer += count * sizeof(f32[2][4]);
    work->colors = (u32 *)buffer;
    buffer += count * sizeof(u32[2]);
    work->marks = (SdfFlagListMark *)buffer;
    work->unk04 = 0x80808080;
    work->resource = resource;
    work->frame = 0;
    memcpy(&work->params, source, sizeof(work->params));
    sdfResetFlagListEntries(work);
    return work;
}

SdfFlagListWork *sdfInitializeFlagListFromResource(void *file) {
    return effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer(file));
}

void sdfReleaseFlagListResource(SdfFlagListWork *work) {
    sdfReleaseResourceAllocation(work->resource);
}

extern f32 sdfViewTargetVector[4];
extern void vuBuildLookAtBasis(void);
extern u32 func_00296F58(const void *, const void *, s32, s32);
extern f32 sdfAtan2Poly(f32 ratio);
extern f32 effMiscRandUnitFloat(void *state);
extern u32 effMiscRand(void *state);
extern u8 effSharedRandomState[];

/* Advance the drop list: respawn a random number of free drops around the camera, move live drops along their
 * own direction, and fade them over their last ten frames. */
void func_002CEC40(SdfFlagListWork *work) {
    f32 pos[4];
    u32 count;
    u32 i;
    u32 color;
    SdfFlagListMark *mark;
    f32 (*vertices)[4];
    u32 *colors;
    u32 spawn;
    u32 spawnRange;
    s32 frame;
    s32 maxFrames;
    f32 speed;
    f32 halfFov;
    f32 spreadX;
    f32 spreadY;
    f32 depth;
    f32 near;
    f32 lateral;
    f32 step;
    u32 alpha;
    s32 timer;
    u32 packed;

    maxFrames = work->params.maxFrames;
    speed = work->params.speed;
    frame = work->frame;
    if (maxFrames > 0 && frame >= maxFrames) {
        return;
    }
    spawn = 1;
    vuBuildLookAtBasis();
    color = func_00296F58(&work->params.color, &work->params.alpha, frame, maxFrames);
    count = work->params.count;
    spawnRange = count >> 4;
    halfFov = sdfSceneProjectionParameters.camera.fov * 0.5f;
    mark = work->marks;
    vertices = work->vertices;
    colors = work->colors;
    spreadX = sdfAtan2Poly(halfFov * 1.5f * 512.0f / 448.0f);
    spreadY = sdfAtan2Poly(halfFov);
    /* Retail evaluates these two as well and never uses the results. */
    sdfAtan2Poly(sdfSceneProjectionParameters.camera.fov);
    sdfAtan2Poly(3.14159265f / 4.0f);
    if (spawnRange != 0) {
        spawn = effMiscRand(effSharedRandomState) % spawnRange + 1;
    }
    for (i = 0; i < count; i++, mark++, vertices += 2, colors += 2) {
        timer = mark->timer;
        if (timer == -1) {
            if (spawn != 0) {
                spawn--;
                near = effMiscRandUnitFloat(effSharedRandomState);
                lateral = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f;
                if (effMiscRandUnitFloat(effSharedRandomState) > 0.5f) {
                    depth = near * 2000.0f + 1000.0f;
                    pos[0] = depth * spreadX * ((effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f);
                    pos[1] = depth * spreadY * lateral;
                    pos[2] = depth;
                    VU0_LOAD_VF(vf10, pos);
                    VU0_TRANSFORM_POINT(vf10, vf10);
                    VU0_STORE_VF(vf10, vertices[0]);
                    vertices[0][1] -= depth * spreadY * 1.7f;
                    if (sdfViewTargetVector[1] + 500.0f < vertices[0][1]) {
                        vertices[0][1] -= vertices[0][1] - sdfViewTargetVector[1];
                    }
                    if (D_003BD2C8 != 0) {
                        VU0_LOAD_VF(vf10, vertices[0]);
                        VU0_LOAD_VF(vf11, sdfViewTargetVector);
                        VU0_SUB(vf10, vf10, vf11);
                        VU0_STORE_VF(vf10, vertices[0]);
                    }
                } else {
                    depth = 0.0f;
                    pos[0] = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f * 1000.0f;
                    pos[2] = (effMiscRandUnitFloat(effSharedRandomState) - 0.5f) * 2.0f * 1000.0f;
                    pos[1] = lateral * 100.0f + -200.0f;
                    VU0_LOAD_VF(vf10, pos);
                    VU0_LOAD_VF(vf11, sdfViewTargetVector);
                    VU0_ADD(vf10, vf10, vf11);
                    if (D_003BD2C8 != 0) {
                        VU0_LOAD_VF(vf11, sdfViewTargetVector);
                        VU0_SUB(vf10, vf10, vf11);
                    }
                    VU0_STORE_VF(vf10, vertices[0]);
                }
                vertices[1][0] = vertices[0][0];
                vertices[1][2] = vertices[0][2];
                vertices[1][1] = vertices[0][1] - 100.0f;
                alpha = 0x80;
                if (!(depth < 1.0f)) {
                    alpha = (1.0f - near) * 64.0f + 64.0f;
                }
                alpha = (f32)alpha * ((f32)(color >> 24) * (1.0f / 128.0f));
                mark->alpha = alpha;
                packed = (color & 0xFFFFFF) | (alpha << 24);
                colors[0] = packed;
                colors[1] = packed & 0xFFFFFF;
                mark->timer = 0;
            }
        } else {
            step = speed + (speed * 0.2f + (f32)timer / 30.0f);
            VU0_LOAD_VF(vf10, vertices[0]);
            VU0_LOAD_VF(vf11, vertices[1]);
            VU0_SUB(vf10, vf10, vf11);
            VU0_NORMALIZE_VF10();
            VU0_MOVE_VF(vf11, vf10);
            VU0_SET_ONES_XYZ(vf10);
            VU0_SCALAR_OP_R3(step, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_MUL(vf10, vf10, vf11);
            VU0_MOVE_VF(vf12, vf10);
            VU0_LOAD_VF(vf10, vertices[0]);
            VU0_ADD(vf10, vf10, vf12);
            VU0_LOAD_VF(vf11, vertices[1]);
            VU0_ADD(vf11, vf11, vf12);
            VU0_STORE_VF(vf10, vertices[0]);
            VU0_STORE_VF(vf11, vertices[1]);
            if (timer < 20) {
                alpha = mark->alpha;
            } else {
                alpha = (u8)(u32)(mark->alpha * (1.0f - (f32)(timer - 20) / 10.0f));
            }
            packed = (color & 0xFFFFFF) | (alpha << 24);
            colors[0] = packed;
            colors[1] = packed & 0xFFFFFF;
            if (timer == 30) {
                colors[0] = 0;
                colors[1] = 0;
                mark->timer = -1;
            } else {
                mark->timer = timer + 1;
            }
        }
    }
    work->frame++;
}


extern SdfPoolNode *D_00398098[];
extern void *sdfMemoryGetBlockAddress(s32);
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void *func_002EF2B0(const f32 (*)[4], const u32 *, s32, u32);

/* Copy vertex pairs, optionally add the view target, and submit the packet list.
 * count is reused first as an entry count and then as a vertex count. */
void func_002CF248(SdfFlagListWork *work) {
    f32 (*sourceVertices)[4];
    f32 (*copiedVertices)[4];
    u32 count;
    u32 vertexIndex;
    s32 vertexAllocation;
    SdfListHead *packetList;
    SdfPoolNode *surface;

    if (work->params.maxFrames != 0 && work->frame >= work->params.maxFrames) {
        return;
    }
    count = work->params.count;
    sourceVertices = work->vertices;
    vertexAllocation = sdfAllocGeneralBlock(count * SDF_FLAG_LIST_ENTRY_VERTEX_BYTES);
    count *= SDF_FLAG_LIST_VERTICES_PER_ENTRY;
    copiedVertices = sdfMemoryGetBlockAddress(vertexAllocation);
    for (vertexIndex = 0; vertexIndex < count; vertexIndex++) {
        VU0_LOAD_VF(vf10, &sourceVertices[vertexIndex]);
        if (D_003BD2C8 != 0) {
            VU0_LOAD_VF(vf11, sdfViewTargetVector);
            VU0_ADD(vf10, vf10, vf11);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, &copiedVertices[vertexIndex]);
    }
    packetList = (SdfListHead *)sdfAllocPacketAligned(SDF_FLAG_LIST_PACKET_BYTES);
    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, (u32)func_002EF2B0(copiedVertices, work->colors, work->params.count * SDF_FLAG_LIST_VERTICES_PER_ENTRY, 0x40));
    surface = D_00398098[work->params.alpha.surfaceIndex];
    surface->append((SdfListHead *)surface, packetList);
    sdfReleaseResourceAllocation(vertexAllocation);
}

/* Read the camera color effect's stored float. */
float scrGetOperandFloatValue(SdfFlagListWork *work) {
    return work->params.speed;
}

/* Replace the camera color effect's stored float without validation. */
void scrSetOperandFloatValue(SdfFlagListWork *work, float value) {
    work->params.speed = value;
}

void func_002CF3A0(SdfFlagListWork *work) {
    func_00296F58(&work->params.color, &work->params.alpha, 0, 0);
}

void itfSetPackedRgbAlpha(SdfFlagListWork *p, u32 color) {
    p->params.color.colorA = color & 0xFFFFFF;
    p->params.alpha.alpha = color >> 24;
}

u32 func_002CF3E8(SdfFlagListWork *work) {
    return work->params.alpha.surfaceIndex;
}

void func_002CF3F0(SdfFlagListWork *work, u32 value) {
    work->params.alpha.surfaceIndex = value;
}

void itfCopyColorFields(SdfFlagListWork *dst, const SdfFlagListParams *src) {
    dst->params.color.colorA = src->color.colorA;
    dst->params.speed = src->speed;
    dst->params.alpha.alpha = src->alpha.alpha;
    dst->params.alpha.surfaceIndex = src->alpha.surfaceIndex;
}

void func_002CF420(void) {
    D_003BD2C8 = 1;
}

void func_002CF430(void) {
    D_003BD2C8 = 0;
}

void sdfCreateSemaphoreFromOptions(void) {
}

s32 sdfCreateSemaphore(u32 initial, u32 option, u32 maximum) {
    struct {
        u32 attr;
        u32 option;
        u32 initial;
        u32 reserved[2];
        u32 maximum;
    } sema;

    sema.initial = initial;
    sema.option = option;
    sema.maximum = maximum;
    return CreateSema(&sema);
}

INCLUDE_ASM(const s32, "game/code_002CC750", sdfCreateThread);

void sdfCreateThreadWithAllocatedWorkspace(u64 destination, u64 encoded, u64 option) {
    u64 decoded;

    decoded = sdfAllocateBlockBySizeThreshold(encoded);
    sdfCreateThread(destination, decoded, encoded, option);
}

INCLUDE_SDATA(const s32, "game/code_002CC750", sdfDebugLogAppendMode);

INCLUDE_SDATA(const s32, "game/code_002CC750", sdfDebugLogPairFormat);

INCLUDE_SDATA(const s32, "game/code_002CC750", D_003BD2C8);

