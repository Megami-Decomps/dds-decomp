/* Keeps its own declaration of the functions below (see sdk_libc.h note). */
#define SDK_SKIP_memset
#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "prf_requirement.h"
#include "prf_requirement_fallback.h"
#include "prf_fallback_item_requirement.h"
#include "sdf_packet_list.h"
#include "dsp_name.h"
#include "common.h"
#include "pty_profile_record_value.h"
#include "sdf_vu0.h"
#include "eff_random.h"
#include "sdf_resource.h"
#include "sdf_vu_matrix.h"
#include "file.h"
#include "sdf.h"
#include "eff_curve.h"
#include "sdf_projection.h"
#include "mdl_flag_api.h"
/* Declared before first use (were implicit int). */
extern s32 btlAccessRecordedLinkageSkillFlag(s32 id, s32 operation);
extern u16 mnuPickPairedTableValue(s32 boundWordIndex, s32 alternate);
extern void *memset(void *dst, s32 value, u32 size);

#include "fpu.h"
#include "pcp_vu0.h"
#include "itf.h"
#include "fr_font.h"
#include "dat_state.h"
#include "pty_skill_nibble_state.h"
#include "mnu_mantra.h"
#include "mnu_mantra_position_api.h"
#include "prf_profile_cap.h"
#include "script_flag_slot.h"
#include "pty_preset_record.h"
#include "prf_current_record_view.h"
#include "prf_profile_parameters.h"


#define SCR_FLAG_ID_MASK 0xFFFF
#define SCR_FLAG_SLOT_INDEX_MASK 7
#define SCR_FLAG_WORD_INDEX_SHIFT 3
#define SCR_FLAG_SLOT_BIT_SHIFT 2
#define SCR_FLAG_PRIMARY_BIT 1U
#define SCR_SKILL_STATE_TWO_BIT 2U
#define SCR_FLAG_SECONDARY_BIT 4U
#define SCR_SECONDARY_FLAG_COUNT 0x2A0
#define SCR_GLOBAL_FLAG_FIRST_ID 0x1AB
#define SCR_GLOBAL_FLAG_END_ID 0x220
/* Adding this bias in a u16 subtracts the first ID by wrapping. */
#define SCR_GLOBAL_FLAG_WRAP_BIAS 0xFE55
#define SCR_GLOBAL_FLAG_WORD_SHIFT 5
#define SCR_GLOBAL_FLAG_BIT_MASK 31
#define PTY_SKILL_SLOT_COUNT 24
#define PRF_SKILL_LIST_FLAGGED 4

#define PTY_PRESET_SKILL_SLOT_COUNT 8
#define PTY_PRESET_POOL_SKILL_COUNT 40
/* Observed gate for the post-mark call; its gameplay meaning is not established. */
#define PTY_PRESET_POOL_EXTRA_GATE 0xBA0
#define PTY_PROFILE_UNIT_LAST_INDEX 4
#define PTY_PROFILE_UNIT_ID_COUNT 0x10
#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1
/* Stride in u16 entries, not bytes; only the first eight are skill IDs. */
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


extern DspMantraName *D_00435E50;


extern DatProfileRecord *ptyGetProfileRecordPointer(const datUnitWork_t *, u16);

extern PrfFallbackItemRequirement D_00404A90[];

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18

extern Entry24W D_00404AA4[];

extern FrFontTextBank frFontColoredGlyphResource;


extern s32 ptyTestPrimaryProfileFlag(datUnitWork_t *, u16);

extern PtyPresetRecord D_004052A8[];
extern u8 scrSelectScriptEntryAndInitialize(datUnitWork_t *, u32);
extern void ptySetSecondaryProfileFlag(datUnitWork_t *, u16);
extern PtyProfileRecordValue ptyAddProfileRecordValueClamped(datUnitWork_t *, u32);

extern u16 D_004052E8[][80];

extern u16 D_004052F8[][80];

extern void ptyRefreshEntryFromSavedTemplate(datUnitWork_t *);



extern s32 ptyGetCurrentProfileId(const datUnitWork_t *);

extern s32 scrSetFlag(datUnitWork_t *, u16);
extern s32 scrFindSlot(datUnitWork_t *, u16);
extern s32 scrRemoveSlot(datUnitWork_t *, u16);

extern ScriptFlagSlot D_00404AA8[];

extern u32 scrGetEntryRequirementFlags(u16);







extern u8 D_0045C828[];












extern s32 fprintf(SdkFileStream *, const char *, ...);


extern char sdfDebugLogAppendMode[];

extern char sdfDebugLogPairFormat[];


void ptyRebuildProfileSkills(s32 useCurrentProfile, datUnitWork_t *unit);

extern _FRQ *itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, FrFontTextBank *, s32);


extern s32 frFontDrawGlyphChain(_FRQ *, s8, u32);


void sdfAppendFormattedDebugLogPair(s32 left, s32 right) {
    SdkFileStream *file = fopen("debug.log", sdfDebugLogAppendMode);
    if (file != NULL) {
        fprintf(file, sdfDebugLogPairFormat, left, right);
        fclose(file);
    }
}

/* Clear the complete per-unit profile-record table in game state. */
void ptyClearProfileRecords(void) {
    memset(DDS3_GBWK->profileBanks, 0, sizeof(DDS3_GBWK->profileBanks));
}

/* Select the highest eligible preset script stage and initialize its profile. */
void ptySelectProfileStage(datUnitWork_t *work) {
    s32 i;
    s32 selected = 0;
    u16 preset;
    u16 scriptId;
    u16 amount;
    PtyPresetScriptChoice *entry;

    preset = work->unitId;
    entry = &D_004052A8[preset].scriptChoices[1];
    for (i = 1; i < 4; i++, entry++) {
        if (entry->scriptId != 0 && mnuGetActiveMantraModelFlagState() >= i) {
            selected = i;
        }
    }
    scriptId = D_004052A8[preset].scriptChoices[selected].scriptId;
    amount = D_004052A8[preset].scriptChoices[selected].initialValue;
    if (scriptId != 0) {
        scrSelectScriptEntryAndInitialize(work, scriptId);
        ptySetSecondaryProfileFlag(work, scriptId);
        ptyAddProfileRecordValueClamped(work, amount);
    }
}

extern void ptyApplyProfile(datUnitWork_t *, u16);
extern void mtrMantraSetBit(datUnitWork_t *);
extern void mtrRecomputeMantraFlagsForPartyUnit(datUnitWork_t *);

/* Choose the least-conflicting group, install its nonzero profile IDs,
 * then complete the native current/preset profile update. */
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
            profileId = D_004052A8[presetIndex].profileIds[groupIndex][profileIndex];
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
    for (i = 0; i < sizeof(D_004052A8[presetIndex].profileIds[selectedGroup]); i++) {
        profileId = D_004052A8[presetIndex].profileIds[selectedGroup][i];
        if (profileId != 0) {
            selectedProfileId = profileId;
            ptyApplyProfile(unit, selectedProfileId);
            ptySetSecondaryProfileFlag(unit, selectedProfileId);
        }
    }
    if (useCurrentProfile == 0) {
        mtrMantraSetBit(unit);
    } else {
        mtrRecomputeMantraFlagsForPartyUnit(unit);
    }
}

/* Load nonzero preset IDs into their original slots; zero IDs leave slots unchanged. */
void ptyLoadPresetSkillSlots(datUnitWork_t *unit) {
    u16 *presetSkills = D_004052E8[unit->unitId];
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
    u16 *presetSkills = D_004052F8[unit->unitId];
    u32 presetIndex = 0;
    do {
        u16 skillId = *presetSkills++;
        if (skillId != 0) {
            scrSetFlag(unit, skillId);
        }
        presetIndex++;
    } while (presetIndex < PTY_PRESET_POOL_SKILL_COUNT);
    if (evtBitCheck(PTY_PRESET_POOL_EXTRA_GATE)) {
        ptyRefreshEntryFromSavedTemplate(unit);
    }
}

void ptyRecomputeMaxHpMp(datUnitWork_t *);
s32 prfBuildRawSkillList(u16, PrfSkillList *);
void scrClearAllSecondaryScriptFlags(datUnitWork_t *);
void scrSetSecondaryScriptFlag(datUnitWork_t *, u16);

/* Rebuild preset skills/vitals in mode0, or current-profile skill flags in mode1. */
void ptyRebuildProfileSkills(s32 useCurrentProfile, datUnitWork_t *unit) {
    PrfSkillList list;
    s32 profile;
    u16 profileId;
    u32 i;

    ptyApplyProfilePreset(useCurrentProfile, unit);
    switch (useCurrentProfile) {
    case 0:
        ptyLoadPresetSkillSlots(unit);
        ptyMarkPresetSkillPool(unit);
        ptyRecomputeMaxHpMp(unit);
        unit->hp = unit->maxhp;
        unit->mp = unit->maxMp;
        break;
    case 1:
        profile = ptyGetCurrentProfileId(unit);
        profileId = profile;
        if (profile != 0 && ptyTestPrimaryProfileFlag(unit, profileId) == 0) {
            ptyApplyProfile(unit, profileId);
            memset(&list, 0, sizeof(list));
            prfBuildRawSkillList(profileId, &list);
            if (list.count != 0) {
                scrClearAllSecondaryScriptFlags(unit);
                for (i = 0; i < list.count; i++) {
                    u16 skillId = list.skills[i];
                    scrSetFlag(unit, skillId);
                    scrSetSecondaryScriptFlag(unit, skillId);
                }
            }
        }
        break;
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

void ptyRecomputeMaxVitals(datUnitWork_t *unit, const s32 *statGains) {
    datUnitWork_t adjusted = *unit;
    u16 oldMaxHp;
    u16 oldMaxMp;
    s32 newMaxHp;
    s32 newMaxMp;
    f32 maximum;

    if (statGains != 0) {
        s32 i;

        for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
            adjusted.baseStats[i] += statGains[i];
        }
    }

    oldMaxHp = unit->maxhp;
    oldMaxMp = unit->maxMp;
    newMaxHp = datComputeSkillBoostedMaxHp(&adjusted);
    newMaxMp = datComputeSkillBoostedMaxMp(&adjusted);

    if (oldMaxHp != newMaxHp) {
        u32 adjustedHp;

        unit->maxhp = newMaxHp;
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
    if (unit->hp > unit->maxhp) {
        unit->hp = unit->maxhp;
    }
    if (unit->mp > unit->maxMp) {
        unit->mp = unit->maxMp;
    }
}

void ptyRecomputeMaxHpMp(datUnitWork_t *unit) {
    ptyRecomputeMaxVitals(unit, 0);
}


void prfCopyMantraNamePart(u16 scriptId, s32 namePartMode, char *destination) {
    u8 *name = D_00435E50[scriptId].encodedText;
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


u32 scrGetIndexedRecordAddress(u32 scriptId, const u8 **text) {
    *text = D_00435E50[scriptId & 0xffff].encodedText;
    return 1;
}

/* Read the configured capacity for an unchecked profile ID. */
u32 ptyGetProfileRecordCap(u16 profileId) {
    return D_00401320[profileId].capacity;
}

void ptySetProfileRecordValue(datUnitWork_t *work, u16 scriptId, u32 value) {
    DatProfileRecord *record;

    record = ptyGetProfileRecordPointer(work, scriptId);
    record->value = value;
}

/* Store the configured capacity in an unchecked profile record. */
void ptySetProfileRecordToCap(datUnitWork_t *unitAddress, u16 profileId) {
    DatProfileRecord *record;
    u32 recordCap;

    record = ptyGetProfileRecordPointer(unitAddress, profileId);
    recordCap = ptyGetProfileRecordCap(profileId);
    record->value = recordCap;
}

/* Add to the selected record and cap the unsigned result; no selection returns zero. */
PtyProfileRecordValue ptyAddProfileRecordValueClamped(datUnitWork_t *unit, u32 increment) {
    DatProfileRecord *record;
    u32 recordCap;
    u8 selectedProfile;
    if (ptyGetCurrentProfileId(unit) == 0) return 0;
    /* This provider owns the bank mutation; callers get the read-only view. */
    record = (DatProfileRecord *)ptyGetCurrentProfileRecord(unit);
    selectedProfile = unit->profileId;
    record->value += increment;
    recordCap = ptyGetProfileRecordCap(selectedProfile);
    if (recordCap < record->value) {
        record->value = recordCap;
    }
    return record->value;
}

void prfDecodeFlagPair(u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 0xF;
    v >>= 4;
    *a = v;
    *b = lo << 1;
}

/* Search the selected 16-entry table for an exact nonzero byte ID.
 * Set and return its global flag ID; return zero if no entry matches. */
u32 ptySetMappedGlobalFlagForSlot(datUnitWork_t *unit, u32 slotId) {
    ScriptFlagSlot *slotCursor = (ScriptFlagSlot *)((u8 *)D_00404AA8 + (unit->unitId << SDF_FLAG_SLOT_TABLE_SHIFT));
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

void scrClearProfileFlagsTable(void) {
    memset(DDS3_GBWK->mantraBits, 0, sizeof(DDS3_GBWK->mantraBits));
}

#ifdef NON_MATCHING
#include "common.h"
#include "dat_state.h"
#include "mdl_flag_api.h"
#include "prf_profile_parameters.h"

extern s32 D_00404A50[]; /* each unit's flag for holding every profile, or 0 */

extern void prfDecodeFlagPair(u32 v, u32 *a, u32 *b);
extern s32 scrSetFlag(datUnitWork_t *, u16);
extern void ptySetProfileRecordToCap(datUnitWork_t *unitAddress, u16 profileId);
extern u32 ptySetMappedGlobalFlagForSlot(datUnitWork_t *unit, u32 slotId);
extern s32 prfAreAllRequiredProfileFlagsSet(datUnitWork_t *unit);

/* Mark the profile's primary flag, grant its skills, cap its record and set
 * its mapped flag; once the unit holds every profile it needs, set the
 * unit's flag, where it has one. DDS1's ptyApplyProfile on DDS2's layouts.
 * Upstream: INCLUDE_ASM in game/code_00313BB8; the same loads and stores as
 * the retail routine. */
void ptyApplyProfile(datUnitWork_t *unit, u16 profileId) {
    u32 word;
    u32 shift;
    u16 *skills;
    u32 skillIndex;

    prfDecodeFlagPair(profileId, &word, &shift);
    DDS3_GBWK->mantraBits[unit->unitId].words[word] |= 1 << shift;
    skills = D_00401320[profileId].skills;
    skillIndex = 0;
    do {
        u16 skillId = *skills++;
        if (skillId != 0) {
            scrSetFlag(unit, skillId);
        }
        skillIndex++;
    } while (skillIndex < 8);
    ptySetProfileRecordToCap(unit, profileId);
    ptySetMappedGlobalFlagForSlot(unit, profileId);
    if (prfAreAllRequiredProfileFlagsSet(unit) && D_00404A50[unit->unitId] != 0) {
        evtBitOn(D_00404A50[unit->unitId]);
    }
}
#else
INCLUDE_ASM(const s32, "game/code_00313BB8", ptyApplyProfile);
#endif

/* Test the first bit of the decoded profile flag pair for this unit. */
s32 ptyTestPrimaryProfileFlag(datUnitWork_t *partyRecord, u16 id) {
    u32 word, shift;
    prfDecodeFlagPair(id, &word, &shift);
    return (DDS3_GBWK->mantraBits[partyRecord->unitId].words[word] & (1 << shift)) != 0;
}

/* Require the primary profile flag for every row not marked as excluded. */
s32 prfAreAllRequiredProfileFlagsSet(datUnitWork_t *unit) {
    u32 profileIndex;
    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        u16 profileId = profileIndex;
        if (!(scrGetEntryRequirementFlags(profileId) & PRF_REQUIREMENT_EXCLUSION_BIT) && !ptyTestPrimaryProfileFlag(unit, profileId)) {
            return 0;
        }
    }
    return 1;
}

/* Mark the second bit of the decoded mantra pair for this unit. */
void ptySetSecondaryProfileFlag(datUnitWork_t *unit, u16 id) {
    u32 word;
    u32 shift;

    prfDecodeFlagPair(id, &word, &shift);
    DDS3_GBWK->mantraBits[unit->unitId].words[word] |= 1U << (shift + 1);
}

/* Test the second bit of the decoded profile flag pair for this unit. */
s32 ptyTestSecondaryProfileFlag(datUnitWork_t *partyRecord, u16 id) {
    u32 word, shift;
    prfDecodeFlagPair(id, &word, &shift);
    return (DDS3_GBWK->mantraBits[partyRecord->unitId].words[word] & (1 << (shift + 1))) != 0;
}

u32 scrGetSelectedScriptEntryId(datUnitWork_t *work) {
    return work->profileId;
}

/* The party record supplies only the bank index; the returned profile record is writable. */
DatProfileRecord *ptyGetProfileRecordPointer(const datUnitWork_t *unit, u16 profileId) {
    return &DDS3_GBWK->profileBanks[unit->unitId].records[profileId];
}

u32 ptyGetProfileRecordValue(const datUnitWork_t *work, u16 scriptId) {
    const DatProfileRecord *record;

    record = ptyGetProfileRecordPointer(work, scriptId);
    return record->value;
}

/* Resolve the unit's stored script-entry selection without an extra mask. */
const DatProfileRecord *ptyGetCurrentProfileRecord(datUnitWork_t *unit) {
    return ptyGetProfileRecordPointer(unit, scrGetSelectedScriptEntryId(unit));
}

s32 ptyGetCurrentProfileId(const datUnitWork_t *work) {
    return work->profileId;
}

/* Store the narrowed selection, initialize its profile state, and return the stored ID. */
u8 scrSelectScriptEntryAndInitialize(datUnitWork_t *unit, u32 profileId) {
    unit->profileId = profileId;
    ptySetSecondaryProfileFlag(unit, (u16)profileId);
    return unit->profileId;
}

/* Decode the low 16 bits: eight four-bit flag slots per word.
 * The unused first argument remains part of the existing calling convention. */
void scrDecodePackedFlagIndex(const datUnitWork_t *unused, u32 flagId, u32 *wordIndex, u32 *bitShift) {
    u32 slotIndex;

    flagId &= SCR_FLAG_ID_MASK;
    slotIndex = flagId & SCR_FLAG_SLOT_INDEX_MASK;
    flagId >>= SCR_FLAG_WORD_INDEX_SHIFT;
    *wordIndex = flagId;
    *bitShift = slotIndex << SCR_FLAG_SLOT_BIT_SHIFT;
}

void scrClearPackedScriptFlags(datUnitWork_t *work) {
    memset(work->skillFlags, 0, 0x154);
}

/* Set bit 0 of the selected nibble; the return value is always 1. */
s32 scrSetFlag(datUnitWork_t *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    work->skillFlags[wordIndex] |= SCR_FLAG_PRIMARY_BIT << bitShift;
    return 1;
}

void scrRemoveAvailableSkillFlagAndSlot(datUnitWork_t *work, u16 id) {
    u32 word, shift;
    PtySkillNibbleState status = ptyGetSkillNibbleState(work, id);
    if (status == PTY_SKILL_NIBBLE_STATE_PRIMARY_BIT_SET) {
        scrDecodePackedFlagIndex(work, id, &word, &shift);
        work->skillFlags[word] &= ~(status << shift);
        if (scrFindSlot(work, id) >= 0) {
            scrRemoveSlot(work, id);
        }
    }
}

void scrResetAndSetPairedGlobalFlags(void) {
    u32 first;
    s32 last;

    memset(DDS3_GBWK->scriptFlags, 0, sizeof(DDS3_GBWK->scriptFlags));
    first = mnuPickPairedTableValue(0x10, 0);
    last = mnuPickPairedTableValue(0x10, 1);
    for (; (s32)first < last; first = first + 1) {
        btlAccessRecordedLinkageSkillFlag(first & 0xffff, 1);
    }
}

/* Set the global bit for an ID in the supported half-open range. */
void scrSetGlobalBitFlag(u32 flagId) {
    u16 bitIndex;
    flagId &= SCR_FLAG_ID_MASK;
    if (flagId < SCR_GLOBAL_FLAG_FIRST_ID) return;
    if (flagId >= SCR_GLOBAL_FLAG_END_ID) return;
    bitIndex = flagId + SCR_GLOBAL_FLAG_WRAP_BIAS;
    DDS3_GBWK->scriptFlags[bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT] |= SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK);
}

/* Return the global bit's raw word mask, not a normalized boolean. */
u32 scrTestGlobalBitFlag(u16 bitIndex) {
    if (bitIndex < SCR_GLOBAL_FLAG_FIRST_ID) return 0;
    if (bitIndex >= SCR_GLOBAL_FLAG_END_ID) return 0;
    bitIndex += SCR_GLOBAL_FLAG_WRAP_BIAS;
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
void scrClearAllSecondaryScriptFlags(datUnitWork_t *work) {
    s32 flagId = 0;
    do {
        scrClearSecondaryScriptFlag(work, (u16)flagId);
        flagId++;
    } while (flagId < SCR_SECONDARY_FLAG_COUNT);
}

/* Return bit 2's raw word mask, not a normalized boolean. */
u32 scrGetSecondaryScriptFlag(datUnitWork_t *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    return work->skillFlags[wordIndex] & (SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Bit 1 takes precedence and returns state 2; otherwise return boolean bit 0.
 * The secondary bit does not affect this state. */
PtySkillNibbleState ptyGetSkillNibbleState(const datUnitWork_t *partyRecord, u16 flagId) {
    u32 wordIndex, bitShift;
    u32 flagWord;
    scrDecodePackedFlagIndex(partyRecord, flagId, &wordIndex, &bitShift);
    flagWord = partyRecord->skillFlags[wordIndex];
    if (flagWord & (SCR_SKILL_STATE_TWO_BIT << bitShift)) {
        return PTY_SKILL_NIBBLE_STATE_BIT_1_SET;
    }
    return (flagWord & (SCR_FLAG_PRIMARY_BIT << bitShift)) != 0
               ? PTY_SKILL_NIBBLE_STATE_PRIMARY_BIT_SET
               : PTY_SKILL_NIBBLE_STATE_CLEAR;
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
    u16 *skillSlots = unit->effectData;
    u32 occupiedCount = 0;
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_SKILL_SLOT_COUNT; slotIndex++) {
        if (skillSlots[slotIndex] != 0) {
            occupiedCount++;
        }
    }
    return occupiedCount;
}

/* Store a new skill ID at the requested index and return the previous ID. */
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

/* Read the unchecked profile's display byte, used as a marker count in DDS2. */
u8 prfGetMantraDisplayParameter(u16 profileId) {
    return D_00401320[profileId].mantraDisplayParameter;
}

/* Read the threshold used by the profile-level check. */
u16 prfGetRequiredProfileLevel(u16 profileId) {
    return D_00401320[profileId].requiredLevel;
}

/* Read byte 5 from the unchecked profile parameter row. */
u8 prfGetProfileParameterByte5(u16 profileId) {
    return D_00401320[profileId].value05;
}

/* Read an unchecked byte offset within the profile parameter row. */
u32 prfGetIndexedProfileByte(u16 profileId, s32 byteIndex) {
    return D_00401320[profileId].parameters[byteIndex];
}

/* Read the selected profile's indexed byte when a selection is present.
 * Retain both selection reads and the no-selection fall-through. */
u32 scrCallIfOperandReady(datUnitWork_t *operand, s32 byteIndex) {
    s32 selectedProfileId = ptyGetCurrentProfileId(operand);

    if (ptyGetCurrentProfileId(operand)) {
        return prfGetIndexedProfileByte((u16)selectedProfileId, byteIndex);
    }
}

/* Read one of the profile row's eight skills; an unsigned out-of-range slot returns zero. */
u16 prfGetSkillAtIndex(u16 profileId, u32 skillIndex) {
    if (skillIndex >= PRF_SKILL_LIST_ENTRY_COUNT) {
        return 0;
    }
    return D_00401320[profileId].skills[skillIndex];
}

u32 func_00315318(void) {
    return 0;
}

/* Test whether a unit meets the profile's configured level requirement. */
u8 ptyMeetsProfileLevelRequirement(const datUnitWork_t *unit, u16 profileId) {
    s32 unitLevel = unit->level;
    if (unitLevel < (s32)prfGetRequiredProfileLevel(profileId)) return 0;
    return 1;
}

/* Clear the paired output words; the first two arguments are unused. */
void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, SCR_PAIRED_OUTPUT_BYTES);
}

u8 scrIsSelectedScriptEntryId(datUnitWork_t *work, u16 id) {
    return work->profileId == id;
}


/* Compact eight nonzero profile skills with cleared flags; optionally copy and return count. */
s32 prfBuildRawSkillList(u16 profile, PrfSkillList *output) {
    PrfSkillList list;
    u32 i;
    u16 skill;

    memset(&list, 0, sizeof(PrfSkillList));
    list.count = 0;
    for (i = 0; i < 8; i++) {
        skill = D_00401320[profile].skills[i];
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
s32 prfBuildSkillList(datUnitWork_t *unit, const DatProfileRecord *unusedProfile, PrfSkillList *output, s32 includeFlagged) {
    PrfSkillList compactedList;
    u32 sourceIndex;
    u16 *profileSkillCursor;
    u16 skillId;
    PtySkillNibbleState skillState;
    s32 selectedProfileId;

    memset(&compactedList, 0, sizeof(PrfSkillList));
    selectedProfileId = unit->profileId;
    compactedList.count = 0;
    if (selectedProfileId != 0) {
        profileSkillCursor = D_00401320[selectedProfileId].skills;
        for (sourceIndex = 0; sourceIndex < PRF_SKILL_LIST_ENTRY_COUNT; sourceIndex++) {
            skillId = *profileSkillCursor++;
            if (skillId != 0) {
                skillState = ptyGetSkillNibbleState(unit, skillId);
                if (skillState != PTY_SKILL_NIBBLE_STATE_CLEAR && includeFlagged == 0) {
                    continue;
                }
                compactedList.flags[compactedList.count] = 0;
                if (skillState != PTY_SKILL_NIBBLE_STATE_CLEAR) {
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

/* Return the selected profile's skill count without flagged skills; the second argument is unused. */
s32 prfBuildSkillListState0(unit, unusedProfile, output)
datUnitWork_t *unit;
const DatProfileRecord *unusedProfile;
PrfSkillList *output;
{
    return prfBuildSkillList(unit, unusedProfile, output, 0);
}


/* Count leading profile IDs up to the first zero or the eight-entry bound. */
u32 prfCountProfileList(PrfRequirementOperand *operand) {
    u8 *profileIds = operand->profileIds;
    u32 profileIndex = 0;
    while (profileIndex < PRF_REQUIRED_PROFILE_COUNT) {
        if (profileIds[profileIndex] == 0) return profileIndex;
        profileIndex++;
    }
    return PRF_REQUIRED_PROFILE_COUNT;
}

/* Require each listed profile's primary flag; an empty list succeeds. */
s32 ptyHasAllReqProfiles(datUnitWork_t *unit, PrfRequirementOperand *operand) {
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
s32 ptyReqProfileCountAtLeast(datUnitWork_t *unit, PrfRequirementOperand *operand) {
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
    if (matchedCount < operand->minimum) {
        return 0;
    }
    return 1;
}

/* Count flagged profile rows whose byte-five value reaches the operand's threshold. */
s32 ptyProfileCountAtLeast(datUnitWork_t *unit, PrfRequirementOperand *operand) {
    u32 profileIndex;
    u32 matchedCount = 0;

    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        if (ptyTestPrimaryProfileFlag(unit, (u16)profileIndex)) {
            if (D_00401320[profileIndex].value05 >= operand->profileThreshold) {
                matchedCount++;
            }
        }
    }
    if (matchedCount < operand->minimum) {
        return 0;
    }
    return 1;
}

/* Each listed profile must be flagged in at least one occupied party slot. */
s32 ptyAreReqProfilesInParty(PrfRequirementOperand *operand) {
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

/* Compare the unit's level byte with the prerequisite's byte threshold. */
u8 prfReqCheckUnitLevel(datUnitWork_t *unit, PrfRequirementOperand *operand) {
    if (unit->level < operand->levelThreshold) {
        return 0;
    }
    return 1;
}

/* Test the global counter against the prerequisite's minimum. */
u8 prfReqCheckGlobalCounter(PrfRequirementOperand *operand) {
    if (DDS3_GBWK->header.currency < operand->minimum) {
        return 0;
    }
    return 1;
}

/* The word-sized ID API narrows only when calling the node-record lookup. */
/* Check six neighboring mantra flags: mode0x40 requires all relevant neighbors, otherwise any. */
s32 prfCheckAdjacentMantraRequirements(s32 requirementId, datUnitWork_t *context,
                                      s32 requirementMode) {
    s32 result = 1;
    MantraNodePos *record = mnuGetMantraNodePositionRecord(requirementId);
    MantraNodePos **adjacent;
    u32 i;

    if (record == NULL) {
        return 0;
    }
    if (requirementMode == 0x40) {
        adjacent = record->neighbors;
        i = 0;
        do {
            if (*adjacent != NULL &&
                ptyTestPrimaryProfileFlag(context, (*adjacent)->selector.packed >> 16) == 0 &&
                ((*adjacent)->selector.packed & 0xF) != 2) {
                result = 0;
            }
            i++;
            adjacent++;
        } while (i < 6);
        return result;
    }
    adjacent = record->neighbors;
    i = 0;
    do {
        if (*adjacent != NULL &&
            ptyTestPrimaryProfileFlag(context, (*adjacent)->selector.packed >> 16) != 0 &&
            ((*adjacent)->selector.packed & 0x100) == 0) {
            return 1;
        }
        i++;
        adjacent++;
    } while (i < 6);
    return 0;
}

/* Enable the starting categories, then unlock the next category when an occupied
 * party member owns a profile whose node connects to that category. The roster
 * countdown still visits the five party slots in ascending order. */
void mnuRebuildMantraCategoryAvailability(void) {
    u32 index;
    s32 partyRemaining;

    memset(D_0045C828, 0, 0x11);
    D_0045C828[0] = 1;
    for (index = mnuGetActiveMantraModelFlagState(); (s32)index >= 0; index--) {
        D_0045C828[index] = 1;
    }
    for (partyRemaining = 4; partyRemaining >= 0; partyRemaining--) {
        datUnitWork_t *partyUnit = &DDS3_GBWK->party[4 - partyRemaining];

        if (!(partyUnit->flags & 1)) {
            continue;
        }
        for (index = 1; index < 0xB0; index++) {
            MantraNodePos *record = mnuGetMantraNodePositionRecord(index);
            MnuMantraModelFlagState modelFlagState;
            s32 category;
            s32 neighborIndex;

            if (record == NULL) {
                continue;
            }
            modelFlagState = record->modelFlagState;
            category = modelFlagState + 1;
            if (D_0045C828[category] != 0) {
                continue;
            }
            for (neighborIndex = 0; neighborIndex < 6; neighborIndex++) {
                MantraNodePos *neighbor = record->neighbors[neighborIndex];

                if (neighbor != NULL) {
                    s32 neighborCategory = neighbor->modelFlagState;

                    if (neighborCategory == modelFlagState + 1 && ptyTestPrimaryProfileFlag(partyUnit, index)) {
                        D_0045C828[neighborCategory] = 1;
                    }
                }
            }
        }
    }
}


s32 mnuIsResourceCategoryAvailable(s32 requirementId) {
    MantraNodePos *info = mnuGetMantraNodePositionRecord(requirementId);
    if (info == 0) {
        return 0;
    }
    return D_0045C828[(s32)(info->selector.packed << 24) >> 28] != 0;
}

/* Test the shared mantra-category enable table; out-of-range indices are disabled. */
s32 prfIsMantraCategoryEnabled(u32 categoryIndex) {
    if (categoryIndex >= 17) {
        return 0;
    }
    return D_0045C828[categoryIndex] != 0;
}

/* The evaluator masks its incoming requirement ID to the lower sixteen bits. */

s32 prfReqEvaluateRules(u32 state, u32 flags, datUnitWork_t *unit, u32 requirementId, u32 *mismatchCount) {
    PrfRequirementOperand *rules = NULL;
    u32 expected = 0;
    u32 count = 0;
    u32 result = 1;
    u32 i;

    requirementId &= 0xFFFF;
    switch (state) {
    case 0: {
        MantraNodePos *node = mnuGetMantraNodePositionRecord((s16)requirementId);

        if (node == NULL) {
            return 0;
        }
        if ((node->selector.packed & 0xF) != 3) {
            return mnuIsResourceCategoryAvailable(requirementId);
        }
        if (mnuIsResourceCategoryAvailable(requirementId) == 0) {
            return 0;
        }
    }
    /* Kind 3 also uses the serialized requirements. */
    case 1:
        rules = D_00402BE0[requirementId].rules;
        if (D_00402BE0[requirementId].state & 0x10) {
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
        case PRF_REQUIREMENT_OP_ALL_PROFILES:
            if (flags == 0 || (flags & 4)) {
                if (ptyHasAllReqProfiles(unit, &rules[i]) == expected) {
                    result = expected;
                } else {
                    count++;
                }
            }
            break;
        case PRF_REQUIREMENT_OP_PROFILE_COUNT:
            if (flags == 0 || (flags & 4)) {
                if (ptyReqProfileCountAtLeast(unit, &rules[i]) == expected) {
                    result = expected;
                } else {
                    count++;
                }
            }
            break;
        case PRF_REQUIREMENT_OP_PROFILE_VALUE_COUNT:
            if (flags == 0 || (flags & 4)) {
                if (ptyProfileCountAtLeast(unit, &rules[i]) == expected) {
                    result = expected;
                } else {
                    count++;
                }
            }
            break;
        case PRF_REQUIREMENT_OP_PROFILES_IN_PARTY:
            if (flags == 0 || (flags & 4)) {
                if (ptyAreReqProfilesInParty(&rules[i]) == expected) {
                    result = expected;
                } else {
                    count++;
                }
            }
            break;
        case PRF_REQUIREMENT_OP_UNIT_LEVEL:
            if (flags == 0) {
                if (prfReqCheckUnitLevel(unit, &rules[i]) == expected) {
                    result = expected;
                } else {
                    count++;
                }
            }
            break;
        case PRF_REQUIREMENT_OP_GLOBAL_MINIMUM:
            if (flags == 0 || (flags & 1)) {
                if (prfReqCheckGlobalCounter(&rules[i]) == expected) {
                    result = expected;
                } else {
                    count++;
                }
            }
            break;
        }
    }
    if (flags == 0) {
        if ((D_00402BE0[requirementId].state & 0x20) == 0) {
            if (prfCheckAdjacentMantraRequirements(requirementId, unit, D_00402BE0[requirementId].state) == 0) {
                result = 0;
            }
        }
    } else if (flags & 2) {
        result = 1;
        if ((D_00402BE0[requirementId].state & 0x20) == 0) {
            result = prfCheckAdjacentMantraRequirements(requirementId, unit, D_00402BE0[requirementId].state) != 0;
        }
    }
    if (mismatchCount != NULL) {
        *mismatchCount = count;
    }
    return result;
}

/* Evaluate requirement rules with zero extra flags and no mismatch-count output. */
s32 prfCheckRequirementRules(u32 mode, datUnitWork_t *unit, u16 id) {
    return prfReqEvaluateRules(mode, 0, unit, id, 0);
}

/* Read requirement flags for an unchecked profile ID. */
u32 scrGetEntryRequirementFlags(u16 requirementId) {
    return D_00401320[requirementId].flags;
}

/* Read the rule-state word for an unchecked requirement ID. */
u32 scrGetEntryState(u16 requirementId) {
    return D_00402BE0[requirementId].state;
}

typedef struct RequirementFallbackEntry {
    u8 id;
    u8 pad01[3];
    u32 flags[2];
} RequirementFallbackEntry;

extern RequirementFallbackEntry D_00404A20[];

/* A zero evaluator return selects its result; otherwise test the fallback flags.
 * Missing fallback IDs succeed, as do entries with no nonzero flag requirements. */
s32 prfReqCheckWithFallback(datUnitWork_t *operand, u16 requirementId) {
    u32 ruleResult;
    u32 groupIndex;
    u32 flagIndex;

    requirementId &= SCR_FLAG_ID_MASK;
    if ((scrGetEntryState(requirementId) & PRF_REQUIREMENT_RULES_BIT) != 0) {
        if (prfReqEvaluateRules(0, 0, operand, requirementId, &ruleResult) == 0) {
            return ruleResult != 0;
        }
    }
    for (groupIndex = 0; groupIndex < PRF_FALLBACK_GROUP_COUNT; groupIndex++) {
        if (D_00404A20[groupIndex].id == requirementId) {
            for (flagIndex = 0; flagIndex < PRF_FALLBACK_FLAG_COUNT; flagIndex++) {
                u32 flagId = D_00404A20[groupIndex].flags[flagIndex];

                if (flagId != 0 && evtBitCheck(flagId) == 0) {
                    return 0;
                }
            }
            return 1;
        }
    }
    return 1;
}

/* Index of the first active table entry whose requirements are met and whose flag is not yet set; -1 when there is none. Only entry 0 is ever visited: the loop bound is one. */
s32 sdfFindUnmetRequirementIndex(u32 start) {
    u32 i;

    for (i = start; i < 1; i++) {
        if (D_00404A90[i].active != 0) {
            s32 met = 1;
            u32 j;
            u32 flagId = D_00404A90[i].flagId;

            for (j = 0; j < 2; j++) {
                s32 id = D_00404A90[i].requirement[j].id;
                s32 minimum = D_00404A90[i].requirement[j].minimum;

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
    return D_00404A90[entryIndex].active;
}

s32 prfReqGetPair(s32 entryIndex, s32 requirementIndex, s32 *minimum) {
    if (minimum != NULL) {
        *minimum = D_00404A90[entryIndex].requirement[requirementIndex].minimum;
    }
    return D_00404A90[entryIndex].requirement[requirementIndex].id;
}

/* Read the flag ID in an unchecked prerequisite-table entry. */
u32 prfReq18GetWord3234(s32 entryIndex) {
    return D_00404AA4[entryIndex].v0;
}

/* Set flags returned by the native selector until it yields a negative index. */
void sdfSetAllFlagsFromTable(void) {
    s32 entryIndex = 0;

    do {
        entryIndex = sdfFindUnmetRequirementIndex(entryIndex);
        if (entryIndex >= 0) {
            s32 flagId = prfReq18GetWord3234(entryIndex);
            if (flagId != 0) {
                evtBitOn(flagId);
            }
        }
    } while (entryIndex++ >= 0);
}

PrfRequirementRecord *scrGetEntryDescriptor(u16 id) {
    return &D_00402BE0[id];
}

void scrSetEntryFlag(datUnitWork_t *context, u16 entryId, u32 bit) {
    DatProfileRecord *entry;
    if (bit < 16) {
        entry = ptyGetProfileRecordPointer(context, entryId);
        entry->flags |= 1 << bit;
    }
}

void scrClearEntryFlag(datUnitWork_t *context, u16 entryId, u32 bit) {
    DatProfileRecord *entry;
    if (bit < 16) {
        entry = ptyGetProfileRecordPointer(context, entryId);
        entry->flags &= ~(1 << bit);
    }
}

s32 scrTestEntryFlag(datUnitWork_t *context, u16 entryId, u32 bit) {
    if (bit >= 16) {
        return 0;
    }
    return (ptyGetProfileRecordPointer(context, entryId)->flags & (1 << bit)) != 0;
}

void scrSetEntryLowFlags(datUnitWork_t *context, u16 entryId, u16 lowFlags) {
    DatProfileRecord *entry = ptyGetProfileRecordPointer(context, entryId);
    entry->flags = (entry->flags & 0xFFFF0000) | lowFlags;
}

u16 scrGetEntryLowFlags(datUnitWork_t *context, u16 entryId) {
    return ptyGetProfileRecordPointer(context, entryId)->flags;
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
extern u32 D_00438918;
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
                    if (D_00438918 != 0) {
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
                    if (D_00438918 != 0) {
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


extern sdfGrOt_t *D_0040A958[];
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
    sdfGrOt_t *surface;

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
        if (D_00438918 != 0) {
            SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_STOREVECTOR_NOMEM(&copiedVertices[vertexIndex]);
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(SDF_FLAG_LIST_PACKET_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)effDebugDrawWireModel(copiedVertices, work->colors, work->params.count * SDF_FLAG_LIST_VERTICES_PER_ENTRY, 0x40));
    surface = D_0040A958[work->params.alpha.surfaceIndex];
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

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogAppendMode);

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogPairFormat);

INCLUDE_SDATA(const s32, "game/code_00313BB8", D_00438918);
