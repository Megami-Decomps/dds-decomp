/* Keeps its own declaration of the functions below (see sdk_libc.h note). */
#define SDK_SKIP_memset
#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "dsp_name.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_kernel_thread_param.h"
#include "sdf_thread_workspace_api.h"
#include "eff_random.h"
#include "sdf_resource.h"
#include "sdf_vu_matrix.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "eff_curve.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "itf.h"
#include "fr_font.h"
#include "dat_state.h"
#include "file.h"
#include "prf_profile_cap.h"
#include "prf_requirement.h"
#include "prf_requirement_fallback.h"
#include "prf_fallback_item_requirement.h"
#include "mdl_flag_api.h"
#include "script_flag_slot.h"
#include "pty_preset_record.h"
#include "prf_profile_parameters.h"
/* Declared before first use (were implicit int). */
extern u16 prfGetSkillAtIndex(u16 profileId, u32 skillIndex);
extern s32 scrSetFlag(datUnitWork_t *work, u16 flagId);

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
#define PRF_SKILL_LIST_FLAGGED 4

#define PTY_PRESET_SKILL_SLOT_COUNT 8
#define PTY_PRESET_POOL_SKILL_COUNT 40
/* Observed gate for the post-mark call; its gameplay meaning is not established. */
#define PTY_PRESET_POOL_EXTRA_GATE 0xB90
#define PTY_PROFILE_UNIT_LAST_INDEX 4
#define PTY_PROFILE_UNIT_ID_COUNT 0x10
#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1
#define PRF_REQUIRED_PROFILE_COUNT 8
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

extern void memset(void *dst, s32 value, s32 size);
extern PtyPresetRecord D_00393A80[];
extern void ptyMergeStockSkills(datUnitWork_t *);

extern void (*sdfTickCallback)(void);

extern s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

extern u32 D_003BD2C8;

extern u32 prfIsRequirementExcluded(u16);

extern DatProfileRecord *ptyGetProfileRecord(datUnitWork_t *, u16);


extern FrFontTextBank frFontColoredGlyphResource;

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; 
extern PrfFallbackItemRequirement D_00393220[];

extern Entry24W D_00393234[];

extern SdkFileStream *fopen(const char *, const char *);
extern void fprintf(SdkFileStream *, const char *, s32, s32);
extern void fclose(SdkFileStream *);
extern char sdfDebugLogAppendMode[];
extern char sdfDebugLogPairFormat[];
extern _FRQ *itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, FrFontTextBank *, s32);
extern s32 frFontDrawGlyphChain(_FRQ *, s8, u32);
extern s32 ptyTestPrimaryProfileFlag(datUnitWork_t *, u16);
DatProfileRecord *ptyGetCurrentProfileRecord(datUnitWork_t *);
s32 ptyGetCurrentProfileId(datUnitWork_t *);


extern void ptyRecomputeMaxVitals(datUnitWork_t *, const s32 *);

void sdfAppendFormattedDebugLogPair(s32 left, s32 right) {
    SdkFileStream *file = fopen("debug.log", sdfDebugLogAppendMode);
    if (file != NULL) {
        fprintf(file, sdfDebugLogPairFormat, left, right);
        fclose(file);
    }
}

/* Clear the complete per-unit profile-record table in game state. */
void ptyClearProfileRecords(void) {
    memset(DDS3_GBWK->profileRecords, 0, sizeof(DDS3_GBWK->profileRecords));
}

s8 scrSelectOperandIndex(datUnitWork_t *unit, s32 profileId);
extern void ptySetSecondaryProfileFlag(datUnitWork_t *, u16);
u32 ptyAddProfilePoints(datUnitWork_t *unit, s32 increment);

/* Pick the highest preset stage whose required profiles all sit at their cap, then apply that stage's script. */
void ptySelectProfileStage(datUnitWork_t *unit) {
    u16 unitId = unit->unitId;
    s32 best = 0;
    s32 stage;
    s32 satisfied;
    s32 i;
    u16 scriptId;
    u16 initialValue;

    for (stage = 1; stage < 4; stage++) {
        if (D_00393A80[unitId].scriptChoices[stage].scriptId != 0) {
            satisfied = 1;
            for (i = 0; i < 4; i++) {
                u16 entry = D_00393A80[unitId].scriptChoices[stage].requiredProfiles[i];
                s32 profileId = entry & SCR_FLAG_ID_MASK;

                if (entry != 0) {
                    if (ptyGetProfileRecord(unit, profileId)->value != prfGetCapValue(profileId)) {
                        satisfied = 0;
                    }
                }
            }
            if (satisfied) {
                best = stage;
            }
        }
    }
    scriptId = D_00393A80[unitId].scriptChoices[best].scriptId;
    initialValue = D_00393A80[unitId].scriptChoices[best].initialValue;
    if (scriptId != 0) {
        scrSelectOperandIndex(unit, scriptId);
        ptySetSecondaryProfileFlag(unit, scriptId);
        ptyAddProfilePoints(unit, initialValue);
    }
}

extern void ptyApplyProfile(datUnitWork_t *, u16);

/* Choose the least-conflicting profile group and install its nonzero IDs. */
void ptyApplyProfilePreset(s32 useCurrentProfile, datUnitWork_t *unit) {
    s32 presetIndex = unit->unitId;
    s32 bestScore;
    u32 selectedGroup;
    u32 groupIndex;
    u32 profileIndex;
    s32 score;
    s32 available;
    s32 i;
    s32 profileId;
    u16 occupied;
    u16 selectedProfileId;
    datUnitWork_t *partyUnit;

    if (useCurrentProfile == 0) {
        ptySelectProfileStage(unit);
    }
    bestScore = 0;
    selectedGroup = 0;
    for (groupIndex = 0; groupIndex < 4; groupIndex++) {
        score = 0;
        for (profileIndex = 0; profileIndex < 12; profileIndex++) {
            profileId = D_00393A80[presetIndex].profileIds[groupIndex][profileIndex];
            available = 1;
            for (i = 0; i < 5; i++) {
                partyUnit = &DDS3_GBWK->party[i];
                occupied = partyUnit->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT;
                if (occupied != 0 && profileId != 0) {
                    if (ptyTestPrimaryProfileFlag(partyUnit, profileId) != 0) {
                        available = 0;
                    }
                }
            }
            if (available != 0) {
                score++;
            }
        }
        if (bestScore < score) {
            bestScore = score;
            selectedGroup = groupIndex;
        }
    }
    for (i = 0; i < sizeof(D_00393A80[presetIndex].profileIds[selectedGroup]); i++) {
        profileId = D_00393A80[presetIndex].profileIds[selectedGroup][i];
        if (profileId != 0) {
            selectedProfileId = profileId;
            ptyApplyProfile(unit, selectedProfileId);
            ptySetSecondaryProfileFlag(unit, selectedProfileId);
        }
    }
}

/* Load nonzero preset IDs into their original slots; zero IDs leave slots unchanged. */
void ptyLoadPresetSkillSlots(datUnitWork_t *unit) {
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
void ptyMarkPresetSkillPool(datUnitWork_t *unit) {
    u16 *presetSkills = D_00393A80[unit->unitId].poolSkills;
    u32 presetIndex = 0;
    do {
        u16 skillId = *presetSkills++;
        if (skillId != 0) {
            scrSetFlag(unit, skillId);
        }
        presetIndex++;
    } while (presetIndex < PTY_PRESET_POOL_SKILL_COUNT);
    if (evtBitCheck(PTY_PRESET_POOL_EXTRA_GATE)) {
        ptyMergeStockSkills(unit);
    }
}

extern void ptyRecomputeMaxHpMp(datUnitWork_t *);
extern s32 prfBuildRawSkillList(u16, PrfSkillList *);
extern s32 scrSetFlag(datUnitWork_t *, u16);
extern void scrSetSecondaryScriptFlag(datUnitWork_t *, u16);
extern void scrClearFlags(datUnitWork_t *);

/* Rebuild skill flags from the preset (no current profile) or from the current profile. */
void ptyRebuildProfileSkills(s32 useCurrentProfile, datUnitWork_t *unit) {
    PrfSkillList skillList;
    s32 profileId;
    u16 profile;
    u32 skillIndex;
    u16 skillId;

    ptyApplyProfilePreset(useCurrentProfile, unit);
    if (useCurrentProfile == 0) {
        ptyLoadPresetSkillSlots(unit);
        ptyMarkPresetSkillPool(unit);
        ptyRecomputeMaxHpMp(unit);
        unit->hp = unit->maxHp;
        unit->mp = unit->maxMp;
    } else {
        profileId = ptyGetCurrentProfileId(unit);
        if (profileId != 0) {
            profile = (u16)profileId;
            ptyApplyProfile(unit, profile);
            memset(&skillList, 0, sizeof(PrfSkillList));
            prfBuildRawSkillList(profile, &skillList);
            if (skillList.count != 0) {
                scrClearFlags(unit);
                for (skillIndex = 0; skillIndex < skillList.count; skillIndex++) {
                    skillId = skillList.skills[skillIndex];
                    scrSetFlag(unit, skillId);
                    scrSetSecondaryScriptFlag(unit, skillId);
                }
            }
        }
    }
}

/* Rebuild skill lists for the five occupied party slots. */
void ptyRebuildAllProfiles(void) {
    s32 unitIndex;
    for (unitIndex = 0; unitIndex < 5; unitIndex++) {
        datUnitWork_t *unit = &DDS3_GBWK->party[unitIndex];
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

extern u32 datComputeSkillBoostedMaxHp(datUnitWork_t *unit);
extern u32 datComputeSkillBoostedMaxMp(datUnitWork_t *unit);

/* Recompute effective maxima from the profile plus optional stat gains, then
 * preserve each current resource's proportion of its former maximum. */
void ptyRecomputeMaxVitals(datUnitWork_t *unit, const s32 *statDeltas) {
    datUnitWork_t adjusted = *unit;
    u16 oldMaxHp;
    u16 oldMaxMp;
    s32 newMaxHp;
    s32 newMaxMp;
    f32 maximum;

    if (statDeltas != NULL) {
        s32 i;

        for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
            adjusted.baseStats[i] += ((const u8 *)statDeltas)[i * 4];
        }
    }

    oldMaxHp = unit->maxHp;
    oldMaxMp = unit->maxMp;
    newMaxHp = datComputeSkillBoostedMaxHp(&adjusted);
    newMaxMp = datComputeSkillBoostedMaxMp(&adjusted);

    if (oldMaxHp != newMaxHp) {
        u32 adjustedHp;

        unit->maxHp = newMaxHp;
        maximum = (f32)newMaxHp;
        adjustedHp = (u32)((f32)unit->hp *
            (maximum / (f32)oldMaxHp) + 0.49999f);
        unit->hp = adjustedHp;
    }
    if (oldMaxMp != newMaxMp) {
        u32 adjustedMp;

        unit->maxMp = newMaxMp;
        maximum = (f32)newMaxMp;
        adjustedMp = (u32)((f32)unit->mp *
            (maximum / (f32)oldMaxMp) + 0.49999f);
        unit->mp = adjustedMp;
    }

    if (unit->hp == 0) {
        unit->hp = 1;
    }
    if (unit->mp == 0) {
        unit->mp = 1;
    }
    if (unit->hp > unit->maxHp) {
        unit->hp = unit->maxHp;
    }
    if (unit->mp > unit->maxMp) {
        unit->mp = unit->maxMp;
    }
}

void ptyRecomputeMaxHpMp(datUnitWork_t *unit) {
    ptyRecomputeMaxVitals(unit, NULL);
}

extern DspMantraName *D_003BAA78;

void prfCopyMantraNamePart(u16 scriptId, s32 namePartMode, char *destination) {
    u8 *name = D_003BAA78[scriptId].encodedText;
    s32 split = 0;
    s32 offset = 0;

    for (; offset < strlen((char *)name); offset += 2) {
        if (name[offset] == 0x82 && name[offset + 1] == 0x8A) {
            split = offset;
        }
    }
    switch (namePartMode) {
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


extern u8 *D_0037CDF8 __attribute__((section(".data")));

s32 prfResolveMantraNameOrFallback(u16 profileId, u8 **out) {
    u8 *text;

    if (evtBitCheck(0x902)) {
        text = D_003BAA78[profileId].encodedText;
        *out = text;
        return 1;
    }
    *out = D_0037CDF8;
    return 0;
}

/* Read the configured capacity for an unchecked profile ID. */
u32 prfGetCapValue(u16 profileId) {
    return D_003907B0[profileId].capacity;
}

/* Store the configured capacity in an unchecked profile record. */
void ptySetProfileRecordToCap(datUnitWork_t *unitAddress, u16 profileId) {
    DatProfileRecord *record;
    u32 recordCap;

    record = ptyGetProfileRecord(unitAddress, profileId);
    recordCap = prfGetCapValue(profileId);
    record->value = recordCap;
}

/* Add to the selected record and cap the unsigned result; no selection returns zero. */
u32 ptyAddProfilePoints(datUnitWork_t *unit, s32 increment) {
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

extern ScriptFlagSlot D_00393280[];

/* Search the selected 16-entry table for an exact nonzero byte ID.
 * Set and return its global flag ID; return zero if no entry matches. */
u32 ptySetMappedGlobalFlagForSlot(datUnitWork_t *unit, u32 slotId) {
    ScriptFlagSlot *slotCursor = (ScriptFlagSlot *)((u8 *)D_00393280 + (unit->unitId << SDF_FLAG_SLOT_TABLE_SHIFT));
    u32 slotIndex;

    for (slotIndex = 0; slotIndex < SDF_FLAG_SLOT_COUNT; slotIndex++, slotCursor++) {
        if (slotCursor->id != 0 && slotCursor->id == slotId) {
            u32 globalFlagId = slotCursor->flag;
            evtBitOn(globalFlagId);
            return globalFlagId;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyApplyProfile);

/* Test the first bit of the decoded profile flag pair for this unit. */
s32 ptyTestPrimaryProfileFlag(datUnitWork_t *partyRecord, u16 id) {
    u32 word;
    u32 shift;

    prfDecodeFlagPair(id, &word, &shift);
    return (DDS3_GBWK->mantraBits[partyRecord->unitId].words[word] & (1 << shift)) != 0;
}

/* Require the primary profile flag for every row not marked as excluded. */
s32 scrCheckStateBits(datUnitWork_t *unit) {
    u32 profileIndex = 0;
    do {
        u16 profileId = profileIndex;
        profileIndex++;
        if ((prfIsRequirementExcluded(profileId) & PRF_REQUIREMENT_EXCLUSION_BIT) == 0 &&
            !ptyTestPrimaryProfileFlag(unit, profileId)) {
            return 0;
        }
    } while (profileIndex < PRF_PROFILE_COUNT);
    return 1;
}

/* Mark the second bit of the decoded mantra pair for this unit. */
void ptySetSecondaryProfileFlag(datUnitWork_t *unit, u16 id) {
    u32 word;
    u32 shift;

    prfDecodeFlagPair(id, &word, &shift);
    shift++;
    DDS3_GBWK->mantraBits[unit->unitId].words[word] |= 1U << shift;
}

/* Test the second bit of the decoded profile flag pair for this unit. */
s32 ptyTestSecondaryProfileFlag(datUnitWork_t *partyRecord, u16 id) {
    u32 word;
    u32 shift;

    prfDecodeFlagPair(id, &word, &shift);
    shift++;
    return (DDS3_GBWK->mantraBits[partyRecord->unitId].words[word] & (1 << shift)) != 0;
}

s8 scrGetSelectedOperandIndex(datUnitWork_t *op) {
    return op->profileId;
}

/* Address an unchecked profile record within the unit's record bank. */
DatProfileRecord *ptyGetProfileRecord(datUnitWork_t *unit, u16 profileId) {
    return &DDS3_GBWK->profileRecords[unit->unitId][profileId];
}

u32 ptyGetProfileRecordValue(datUnitWork_t *unit, u16 profileId) {
    DatProfileRecord *record;

    record = ptyGetProfileRecord(unit, profileId);
    return record->value;
}

/* Resolve the selected ID using the native low-16-bit conversion. */
DatProfileRecord *ptyGetCurrentProfileRecord(datUnitWork_t *unit) {
    return ptyGetProfileRecord(unit, scrGetSelectedOperandIndex(unit) & SCR_FLAG_ID_MASK);
}

s32 ptyGetCurrentProfileId(datUnitWork_t *op) {
    return op->profileId;
}

/* Store the narrowed selection, mark its paired profile flag, and return the stored ID. */
s8 scrSelectOperandIndex(datUnitWork_t *unit, s32 profileId) {
    unit->profileId = profileId;
    ptySetSecondaryProfileFlag(unit, profileId & SCR_FLAG_ID_MASK);
    return unit->profileId;
}

u32 func_002CD7F0(u32 unused, u32 value) {
    return value;
}

/* Retail mode-1 item predicate: never blocks an entry; itemId is unused. */
u32 mnuShopIsMode1PurchaseBlocked(u32 itemId) {
    (void)itemId;
    return 0;
}

/* Retail mode-1 purchase hook: unused itemId, no mutation, ignored return one. */
u32 mnuShopCommitMode1PurchaseNoOp(u32 itemId) {
    (void)itemId;
    return 1;
}

/* Decode the low 16 bits: eight four-bit flag slots per word.
 * The unused first argument remains part of the existing calling convention. */
void scrDecodePackedFlagIndex(datUnitWork_t *unused, u32 flagId, u32 *wordIndex, u32 *bitShift) {
    u32 slotIndex;

    flagId &= SCR_FLAG_ID_MASK;
    slotIndex = flagId & SCR_FLAG_SLOT_INDEX_MASK;
    flagId >>= SCR_FLAG_WORD_INDEX_SHIFT;
    *wordIndex = flagId;
    *bitShift = slotIndex << SCR_FLAG_SLOT_BIT_SHIFT;
}

/* Set bit 0 of the selected nibble; the return value is always 1. */
s32 scrSetFlag(datUnitWork_t *work, u16 flagId) {
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
    DDS3_GBWK->scriptFlags[bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT] |= SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK);
}

/* Return the global bit's raw word mask, not a normalized boolean. */
u32 scrTestGlobalSeenBit(u32 flagId) {
    u16 bitIndex = flagId;
    if (bitIndex < SCR_GLOBAL_FLAG_FIRST_ID) return 0;
    if (bitIndex >= SCR_GLOBAL_FLAG_END_ID) return 0;
    bitIndex = bitIndex + SCR_GLOBAL_FLAG_WRAP_BIAS;
    return DDS3_GBWK->scriptFlags[bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT] & (SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK));
}

/* Set only bit 2 of the selected flag nibble. */
void scrSetSecondaryScriptFlag(datUnitWork_t *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    work->skillFlags[wordIndex] |= SCR_FLAG_SECONDARY_BIT << bitShift;
}

/* Clear only bit 2 of the selected flag nibble. */
void scrClearSecondaryScriptFlag(datUnitWork_t *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    work->skillFlags[wordIndex] &= ~(SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Clear secondary bits over the fixed ID range, preserving other nibble bits. */
void scrClearFlags(datUnitWork_t *work) {
    s32 flagId;
    for (flagId = 0; flagId < SCR_SECONDARY_FLAG_COUNT; flagId++) {
        scrClearSecondaryScriptFlag(work, flagId);
    }
}

/* Return bit 2's raw word mask, not a normalized boolean. */
u32 scrGetSecondaryScriptFlag(datUnitWork_t *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    return work->skillFlags[wordIndex] & (SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Bit 1 takes precedence and returns state 2; otherwise return boolean bit 0.
 * The secondary bit does not affect this state. */
u32 ptyGetSkillNibbleState(datUnitWork_t *partyRecord, u16 flagId) {
    u32 wordIndex, bitShift;
    u32 flagWord;
    scrDecodePackedFlagIndex(partyRecord, flagId, &wordIndex, &bitShift);
    flagWord = partyRecord->skillFlags[wordIndex];
    if (flagWord & (SCR_SKILL_STATE_TWO_BIT << bitShift)) {
        return 2;
    }
    return (flagWord & (SCR_FLAG_PRIMARY_BIT << bitShift)) != 0;
}

/* Search all slots for the low 16-bit ID; zero can match an empty slot. */
s32 ptyHasSkill(datUnitWork_t *unit, s32 skillId) {
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

extern s32 scrFindSlot(datUnitWork_t *unit, u16 skillId);
extern s32 scrRemoveSlot(datUnitWork_t *unit, u16 skillId);

void ptyRemoveProfileSkills(datUnitWork_t *unit, u16 profileId) {
    u32 wordIndex, bitShift;
    u32 index = 0;
    s32 skill;
    u32 state;
    u16 flagId;

    do {
        skill = prfGetSkillAtIndex(profileId, index);
        flagId = skill & 0xFFFF;
        state = ptyGetSkillNibbleState(unit, flagId);
        if (state == 1) {
            scrDecodePackedFlagIndex(unit, flagId, &wordIndex, &bitShift);
            if (skill != 0) {
                unit->skillFlags[wordIndex] &= ~(state << bitShift);
                if (scrFindSlot(unit, flagId) >= 0) {
                    scrRemoveSlot(unit, flagId);
                }
            }
        }
        index++;
    } while (skill != 0);
}

/* Return the first exact-ID slot, including zero IDs, or -1 when absent. */
s32 scrFindSlot(datUnitWork_t *unit, u16 skillId) {
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
u16 scrGetSlot(datUnitWork_t *unit, u32 slotIndex) {
    if (slotIndex >= PTY_SKILL_SLOT_COUNT) {
        return 0;
    }
    return unit->effectData[slotIndex];
}

/* Count nonzero skill IDs across every slot. */
u32 scrCountSlots(datUnitWork_t *unit) {
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
u16 scrSetSlot(datUnitWork_t *unit, s32 slotIndex, u16 skillId) {
    u16 previousSkillId = unit->effectData[slotIndex];

    unit->effectData[slotIndex] = skillId;
    return previousSkillId;
}

/* Clear the first exact-ID match and report whether a slot was found. */
s32 scrRemoveSlot(datUnitWork_t *unit, u16 skillId) {
    s32 slotIndex = scrFindSlot(unit, skillId);
    if (slotIndex >= 0) {
        unit->effectData[slotIndex] = 0;
        return 1;
    }
    return 0;
}

/* Read the unchecked profile's label-display byte; DDS1 exposes it as a word. */
s32 prfGetMantraDisplayParameter(u16 profileId) {
    return D_003907B0[profileId].value04;
}

/* Read the threshold used by the profile-level check. */
u16 prfGetRequiredProfileLevel(u16 profileId) {
    return D_003907B0[profileId].requiredLevel;
}

/* Read byte 5 from the unchecked profile parameter row. */
u8 prfGetProfileParameterByte5(u16 profileId) {
    return D_003907B0[profileId].value05;
}

u32 prfGetIndexedProfileByte(u16 profileId, s32 statIndex) {
    return 0;
}

/* DDS2 reads an indexed profile byte here; DDS1's native callee returns zero.
 * Retain both selection reads and the no-selection fall-through. */
u32 scrCallIfOperandReady(datUnitWork_t *operand, u32 byteIndex) {
    s32 selectedProfileId = ptyGetCurrentProfileId(operand);

    if (ptyGetCurrentProfileId(operand)) {
        return prfGetIndexedProfileByte(selectedProfileId, byteIndex);
    }
}

/* Read one of the profile row's eight skills; an unsigned out-of-range slot returns zero. */
u16 prfGetSkillAtIndex(u16 profileId, u32 skillIndex) {
    if (skillIndex >= PRF_SKILL_LIST_ENTRY_COUNT) {
        return 0;
    }
    return D_003907B0[profileId].skills[skillIndex];
}

u32 func_002CDE60(void) {
    return 0;
}

u32 ptyCheckLevelAtLeastProfileParam7b6(datUnitWork_t *operand, u16 index) {
    s32 value = operand->level;
    if (value < (s32)prfGetRequiredProfileLevel(index)) {
        return 0;
    }
    return 1;
}

/* Clear the paired output words; the first two arguments are unused. */
void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, SCR_PAIRED_OUTPUT_BYTES);
}

u8 ptyIsCurrentProfileId(datUnitWork_t *operand, u32 profileId) {
    return operand->profileId == (profileId & 0xffff);
}


s32 prfBuildRawSkillList(u16 profile, PrfSkillList *output) {
    PrfSkillList list;
    u32 i;
    u16 *skills;
    u16 skill;

    memset(&list, 0, sizeof(PrfSkillList));
    list.count = 0;
    skills = D_003907B0[profile].skills;
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
s32 prfBuildSkillList(datUnitWork_t *unit, DatProfileRecord *unusedProfile, PrfSkillList *output, s32 includeFlagged) {
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
        profileSkillCursor = D_003907B0[selectedProfileId].skills;
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
u32 prfBuildSkillListState0(datUnitWork_t *unit, DatProfileRecord *unusedProfile, PrfSkillList *output) {
    return prfBuildSkillList(unit, unusedProfile, output, 0);
}

/* Count leading IDs in a DDS1 requirement slot up to the first zero or bound. */
u32 prfCountProfileList(PrfDds1RequirementSlot *operand) {
    u8 *profileIds = operand->profileIds;
    u32 profileIndex;
    for (profileIndex = 0; profileIndex < PRF_REQUIRED_PROFILE_COUNT; profileIndex++) {
        if (profileIds[profileIndex] == 0) {
            return profileIndex;
        }
    }
    return PRF_REQUIRED_PROFILE_COUNT;
}

/* Require each listed profile's primary flag; an empty list succeeds. */
s32 ptyHasAllReqProfiles(datUnitWork_t *unit, PrfDds1RequirementSlot *operand) {
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand->profileIds;
        do {
            if (ptyTestPrimaryProfileFlag(unit, profileIds[profileIndex++]) == 0) {
                return 0;
            }
        } while (profileIndex < profileCount);
    }
    return 1;
}

/* Compare the number of flagged IDs in the list with its minimum count. */
s32 ptyReqProfileCountAtLeast(datUnitWork_t *unit, PrfDds1RequirementSlot *operand) {
    u32 matchedCount = 0;
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand->profileIds;
        do {
            if (ptyTestPrimaryProfileFlag(unit, profileIds[profileIndex++]) != 0) {
                matchedCount++;
            }
        } while (profileIndex < profileCount);
    }
    if (matchedCount < (u32)operand->sourceValue) {
        return 0;
    }
    return 1;
}

/* Count flagged profile rows whose byte-five value reaches the operand's threshold. */
s32 ptyProfileCountAtLeast(datUnitWork_t *unit, PrfDds1RequirementSlot *operand) {
    u32 profileIndex;
    u32 matchedCount = 0;

    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        if (ptyTestPrimaryProfileFlag(unit, (u16)profileIndex)) {
            if (D_003907B0[profileIndex].value05 >= operand->unknown04[1]) {
                matchedCount++;
            }
        }
    }
    if (matchedCount < (u32)operand->sourceValue) {
        return 0;
    }
    return 1;
}

/* Each listed profile must be flagged in at least one occupied party slot. */
s32 ptyAreReqProfilesInParty(PrfDds1RequirementSlot *operand) {
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand->profileIds;
        do {
            s32 profilePresent = 0;
            s32 profileId = profileIds[profileIndex];
            s32 unitIndex;
            for (unitIndex = 0; unitIndex < 5; unitIndex++) {
                datUnitWork_t *partyUnit = &DDS3_GBWK->party[unitIndex];
                if ((partyUnit->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
                    if (ptyTestPrimaryProfileFlag(partyUnit, (u16)profileId) != 0) {
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
u32 prfReqCheckUnitLevel(datUnitWork_t *unit, PrfDds1RequirementSlot *operand) {
    if (unit->level < operand->unknown04[0]) {
        return 0;
    }
    return 1;
}

/* Test the global currency counter against a prerequisite threshold. */
u32 prfReqCheckGlobalCounter(PrfDds1RequirementSlot *operand) {
    if (DDS3_GBWK->header.currency < (u32)operand->sourceValue) {
        return 0;
    }
    return 1;
}

extern s32 prfReqEvaluateRules(u32, u32, u16, u32 *);
s32 prfReqEvaluateRules(u32 state, u32 unitAddress, u16 requirementId, u32 *mismatchCount) {
    PrfDds1RequirementSlot *rules = NULL;
    u32 expected = 0;
    u32 count = 0;
    u32 result = 1;
    u32 i;

    switch (state) {
    case 0:
        rules = (PrfDds1RequirementSlot *)&D_00391230[requirementId].stateZeroFirstOperation;
        if (D_00391230[requirementId].stateFlags & 8) {
            result = 0;
            expected = 1;
        }
        break;
    case 1:
        rules = D_00391230[requirementId].slots;
        if (D_00391230[requirementId].stateFlags & 0x10) {
            result = 0;
            expected = 1;
        }
        break;
    }
    if (rules == NULL) {
        return 0;
    }
    for (i = 0; i < 2; i++) {
        switch (rules[i].operation) {
        case 1:
            if (ptyHasAllReqProfiles((datUnitWork_t *)unitAddress, &rules[i]) == expected) {
                result = expected;
            } else {
                count++;
            }
            break;
        case 2:
            if (ptyReqProfileCountAtLeast((datUnitWork_t *)unitAddress, &rules[i]) == expected) {
                result = expected;
            } else {
                count++;
            }
            break;
        case 4:
            if (ptyProfileCountAtLeast((datUnitWork_t *)unitAddress, &rules[i]) == expected) {
                result = expected;
            } else {
                count++;
            }
            break;
        case 8:
            if (ptyAreReqProfilesInParty(&rules[i]) == expected) {
                result = expected;
            } else {
                count++;
            }
            break;
        case 16:
            if (prfReqCheckUnitLevel((datUnitWork_t *)unitAddress, &rules[i]) == expected) {
                result = expected;
            } else {
                count++;
            }
            break;
        case 32:
            if (prfReqCheckGlobalCounter(&rules[i]) == expected) {
                result = expected;
            } else {
                count++;
            }
            break;
        }
    }
    if (mismatchCount != NULL) {
        *mismatchCount = count;
    }
    return result;
}

/* Evaluate rules without mismatch-count output; retain DDS1's existing void form. */
s32 prfCheckRequirementRules(u32 state, u32 operand, u16 requirementId) {
    return prfReqEvaluateRules(state, operand, requirementId, 0);
}

/* Read requirement flags for an unchecked profile ID. */
u32 prfIsRequirementExcluded(u16 requirementId) {
    return D_003907B0[requirementId].flags;
}

/* Read the rule-state word for an unchecked requirement ID. */
u32 prfReq54GetWord1230(u16 requirementId) {
    return D_00391230[requirementId].stateFlags;
}

typedef struct PrfFallbackGroup {
    u8 id;
    u8 pad01[3];
    u32 flags[2];
} PrfFallbackGroup;

extern PrfFallbackGroup D_003931B0[];
/* A zero evaluator return selects its result; otherwise test the fallback flags.
 * Missing fallback IDs succeed, as do entries with no nonzero flag requirements. */
s32 prfReqCheckWithFallback(datUnitWork_t *operand, u16 requirementId) {

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

                if (flagId != 0 && evtBitCheck(flagId) == 0) {
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
        if (D_00393220[i].active != 0) {
            s32 met = 1;
            u32 j;
            u32 flagId = D_00393220[i].flagId;

            for (j = 0; j < 2; j++) {
                s32 id = D_00393220[i].requirement[j].id;
                s32 minimum = D_00393220[i].requirement[j].minimum;

                if (id != 0) {
                    if (DDS3_GBWK->inventory.counts[id] < minimum) {
                        met = 0;
                    }
                }
            }
            if (met != 0 && evtBitCheck(flagId) == 0) {
                return i;
            }
        }
    }
    return -1;
}

/* Read the byte in an unchecked prerequisite-table entry. */
u8 prfReq18GetWord3220(s32 entryIndex) {
    return D_00393220[entryIndex].active;
}

s32 prfReqGetPair(s32 entryIndex, s32 requirementIndex, s32 *minimum) {
    if (minimum != NULL) {
        *minimum = D_00393220[entryIndex].requirement[requirementIndex].minimum;
    }
    return D_00393220[entryIndex].requirement[requirementIndex].id;
}

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
                evtBitOn(flagId);
            }
        }
    } while (entryIndex++ >= 0);
}

PrfDds1RequirementRecord *prfReqGetEntryRecord(u16 profileId) {
    return &D_00391230[profileId];
}

void frFontQueueColoredGlyph(s32 x, s32 y, u32 first, u16 width, u32 second, s32 option) {
    _FRQ *handle = itfDrawBankTextWithLayoutFlags(x, y, first, width, &frFontColoredGlyphResource, 0);
    frSetFontColor(handle, second);
    frFontDrawGlyphChain(handle, 1, option);
    frDestroyFontRequestAfter(handle);
}

FrFontTextBank *frFontGetColoredGlyphResource(void) {
    return &frFontColoredGlyphResource;
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


SdfFlagListWork *sdfCreateFlagListWork(const SdfFlagListParams *source) {
    u32 count;
    u32 arrayBytes;
    sdfMemHandle_t *allocation;
    u8 *buffer;
    SdfFlagListWork *work;

    count = source->count;
    arrayBytes = count * (sizeof(f32[2][4]) + sizeof(u32[2]) + sizeof(SdfFlagListMark));
    allocation = sdfMemAllocDirect(arrayBytes + sizeof(SdfFlagListWork));
    buffer = (u8 *)sdfMemLock(allocation);
    work = (SdfFlagListWork *)(buffer + arrayBytes);
    work->vertices = (f32 (*)[4])buffer;
    buffer += count * sizeof(f32[2][4]);
    work->colors = (u32 *)buffer;
    buffer += count * sizeof(u32[2]);
    work->marks = (SdfFlagListMark *)buffer;
    work->unk04 = 0x80808080;
    work->allocation = allocation;
    work->frame = 0;
    memcpy(&work->params, source, sizeof(work->params));
    sdfResetFlagListEntries(work);
    return work;
}

SdfFlagListWork *sdfInitializeFlagListFromResource(void *file) {
    return effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer(file));
}

void sdfReleaseFlagListResource(SdfFlagListWork *work) {
    sdfMemFreeDirect(work->allocation);
}

extern f32 sdfViewTargetVector[4];
extern f32 sdfAtan(f32 ratio);
extern EffRandState effSharedRandomState[];

/* Advance the drop list: respawn a random number of free drops around the camera, move live drops along their
 * own direction, and fade them over their last ten frames. */
void sdfUpdateFlagListEntries(SdfFlagListWork *work) {
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
    color = effSampleColorAlphaTracks(&work->params.color, &work->params.alpha, frame, maxFrames);
    count = work->params.count;
    spawnRange = count >> 4;
    halfFov = dds3ScreenParam.camera.fov * 0.5f;
    mark = work->marks;
    vertices = work->vertices;
    colors = work->colors;
    spreadX = sdfAtan(halfFov * 1.5f * 512.0f / 448.0f);
    spreadY = sdfAtan(halfFov);
    /* Retail evaluates these two as well and never uses the results. */
    sdfAtan(dds3ScreenParam.camera.fov);
    sdfAtan(3.14159265f / 4.0f);
    if (spawnRange != 0) {
        spawn = dds3GetRandA((EffRandState *)effSharedRandomState) % spawnRange + 1;
    }
    for (i = 0; i < count; i++, mark++, vertices += 2, colors += 2) {
        timer = mark->timer;
        if (timer == -1) {
            if (spawn != 0) {
                spawn--;
                near = dds3GetRandFloatA(effSharedRandomState);
                lateral = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f;
                if (dds3GetRandFloatA(effSharedRandomState) > 0.5f) {
                    depth = near * 2000.0f + 1000.0f;
                    pos[0] = depth * spreadX * ((dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f);
                    pos[1] = depth * spreadY * lateral;
                    pos[2] = depth;
                    SDF_VU0_LOADVECTOR(pos);
                    SDF_VU0_APPLYMATRIX_XYZ1();
                    SDF_VU0_STOREVECTOR(vertices[0]);
                    vertices[0][1] -= depth * spreadY * 1.7f;
                    if (sdfViewTargetVector[1] + 500.0f < vertices[0][1]) {
                        vertices[0][1] -= vertices[0][1] - sdfViewTargetVector[1];
                    }
                    if (D_003BD2C8 != 0) {
                        SDF_VU0_LOADVECTOR(vertices[0]);
                        SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_STOREVECTOR(vertices[0]);
                    }
                } else {
                    depth = 0.0f;
                    pos[0] = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 1000.0f;
                    pos[2] = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 1000.0f;
                    pos[1] = lateral * 100.0f + -200.0f;
                    SDF_VU0_LOADVECTOR(pos);
                    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
                    SDF_VU0_ADDVECTOR();
                    if (D_003BD2C8 != 0) {
                        SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
                        SDF_VU0_SUBVECTOR();
                    }
                    SDF_VU0_STOREVECTOR(vertices[0]);
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
            SDF_VU0_LOADVECTOR(vertices[0]);
            SDF_VU0_LOADVECTOR_B(vertices[1]);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_SETSCALE1VECTOR();
            VU0_SCALAR_OP_R3(step, "vmulx.xyzw vf10, vf10, vf2x");
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(vertices[0]);
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(vertices[1]);
            SDF_VU0_ADDVECTOR_B_C();
            SDF_VU0_STOREVECTOR(vertices[0]);
            SDF_VU0_STOREVECTOR_B(vertices[1]);
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
#include "sdf_packet_alloc.h"

/* Copy vertex pairs, optionally add the view target, and submit the packet list.
 * count is reused first as an entry count and then as a vertex count. */
void sdfDrawFlagListVertices(SdfFlagListWork *work) {
    f32 (*sourceVertices)[4];
    f32 (*copiedVertices)[4];
    u32 count;
    u32 vertexIndex;
    struct sdfMemHandle_t *vertexAllocation;
    sdfDrawTag_t *packetList;
    SdfPoolNode *surface;

    if (work->params.maxFrames != 0 && work->frame >= work->params.maxFrames) {
        return;
    }
    count = work->params.count;
    sourceVertices = work->vertices;
    vertexAllocation = sdfMemAllocDirect(count * SDF_FLAG_LIST_ENTRY_VERTEX_BYTES);
    count *= SDF_FLAG_LIST_VERTICES_PER_ENTRY;
    copiedVertices = (f32 (*)[4])sdfMemGetPtr(vertexAllocation);
    for (vertexIndex = 0; vertexIndex < count; vertexIndex++) {
        SDF_VU0_LOADVECTOR(&sourceVertices[vertexIndex]);
        if (D_003BD2C8 != 0) {
            SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_STOREVECTOR_NOMEM(&copiedVertices[vertexIndex]);
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(SDF_FLAG_LIST_PACKET_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)effDebugDrawWireModel(copiedVertices, work->colors, work->params.count * SDF_FLAG_LIST_VERTICES_PER_ENTRY, 0x40));
    surface = D_00398098[work->params.alpha.surfaceIndex];
    surface->append(surface, packetList);
    sdfMemFreeDirect(vertexAllocation);
}

/* Return the drop list's movement speed; this is not a script operand. */
float sdfGetFlagListSpeed(SdfFlagListWork *work) {
    return work->params.speed;
}

/* Replace the drop list's movement speed without validation. */
void sdfSetFlagListSpeed(SdfFlagListWork *work, float speed) {
    work->params.speed = speed;
}

/* Sample color/alpha at frame zero with duration zero; discard the return value. */
void sdfSampleFlagListStartColor(SdfFlagListWork *work) {
    effSampleColorAlphaTracks(&work->params.color, &work->params.alpha, 0, 0);
}

void itfSetPackedRgbAlpha(SdfFlagListWork *p, u32 color) {
    p->params.color.initialColor = color & 0xFFFFFF;
    p->params.alpha.alpha = color >> 24;
}

u32 sdfGetFlagListAlphaSurfaceIndex(SdfFlagListWork *work) {
    return work->params.alpha.surfaceIndex;
}

void sdfSetFlagListAlphaSurfaceIndex(SdfFlagListWork *work, u32 surfaceIndex) {
    work->params.alpha.surfaceIndex = surfaceIndex;
}

void itfCopyColorFields(SdfFlagListWork *dst, const SdfFlagListParams *src) {
    dst->params.color.initialColor = src->color.initialColor;
    dst->params.speed = src->speed;
    dst->params.alpha.alpha = src->alpha.alpha;
    dst->params.alpha.surfaceIndex = src->alpha.surfaceIndex;
}

void sdfEnableFlagListViewTargetRelativeMode(void) {
    D_003BD2C8 = 1;
}

void sdfDisableFlagListViewTargetRelativeMode(void) {
    D_003BD2C8 = 0;
}

void sdfCreateSemaphoreFromOptions(void) {
}

s32 sdfCreateSema(u32 initial, u32 option, u32 maximum) {
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

extern void *_gp;

/* Create a kernel thread from a zeroed descriptor that uses the caller's
 * workspace as its stack and the program's GP. */
s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority) {
    SdfKernelThreadParam param;

    memset(&param, 0, sizeof(param));
    param.entry = entryAddress;
    param.stack = workspace;
    param.stackSize = stackBytes;
    param.priority = priority;
    param.gp = &_gp;
    return CreateThread(&param);
}

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority) {
    void *workspace;

    workspace = sdfAllocAuto(stackBytes);
    return CreateThreadSimple((void *)entryAddress, workspace, stackBytes, priority);
}

INCLUDE_SDATA(const s32, "game/code_002CC750", sdfDebugLogAppendMode);

INCLUDE_SDATA(const s32, "game/code_002CC750", sdfDebugLogPairFormat);

INCLUDE_SDATA(const s32, "game/code_002CC750", D_003BD2C8);

