#include "prf_requirement.h"
#include "dsp_name.h"
#include "common.h"
#include "sdf.h"
extern void memset();

#include "fpu.h"
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
#define SCR_SECONDARY_FLAG_COUNT 0x2A0
#define SCR_GLOBAL_FLAG_FIRST_ID 0x1AB
#define SCR_GLOBAL_FLAG_END_ID 0x220
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
#define PTY_PRESET_POOL_EXTRA_GATE 0xBA0
#define PTY_PROFILE_UNIT_LAST_INDEX 4
#define PTY_PROFILE_UNIT_ID_COUNT 0x10
#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1
/* Stride in u16 entries, not bytes; only the first eight are skill IDs. */
#define PRF_SKILL_TABLE_STRIDE 18
#define PRF_PROFILE_PARAM_BYTES 36
#define PRF_PROFILE_COUNT 0xB0
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

extern s32 sdfReleaseResourceAllocation(u32);

extern s32 fileResolvePrimaryBuffer();

extern void effCreateSelectionFlagListFromWork();

extern DspMantraName *D_00435E50;


extern u32 ptyGetProfileRecordCap(u16);

extern DatProfileRecord *ptyGetProfileRecordPointer(DatPartyRecord *, u16);

/* 24-byte requirement table entries: an active byte, two (stat id, minimum) pairs and a flag id. */
typedef struct Entry24B {
    u8 active;         // 0x00
    u8 pad_0x01[3];    // 0x01
    struct {
        s32 id;        // 0x00
        s32 minimum;   // 0x04
    } requirement[2];  // 0x04
    u32 flagId;        // 0x14
} Entry24B; // 0x18

extern Entry24B D_00404A90[];

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18

extern Entry24W D_00404AA4[];

extern u8 frFontColoredGlyphResource[];


extern s32 func_00314990(DatPartyRecord *, u16);

/* The first four pairs in each 0xA0-byte row select script/value initialization
 * for the active mantra model flag state. */
typedef struct PtyPresetScriptChoice {
    u16 scriptId;
    u16 initialValue;
} PtyPresetScriptChoice;

typedef struct PtyPresetRecord {
    PtyPresetScriptChoice scriptChoices[4];
    u8 pad10[0x90];
} PtyPresetRecord;

extern PtyPresetRecord D_004052A8[];
extern s32 mnuGetActiveMantraModelFlagState(void);
extern u8 scrSelectScriptEntryAndInitialize(DatPartyRecord *, u32);
extern void func_00314A80(DatPartyRecord *, u16);
extern u32 ptyAddProfileRecordValueClamped(DatPartyRecord *, u32);

extern u16 D_004052E8[][80];

extern u16 D_004052F8[][80];

extern void func_0011CA88(DatPartyRecord *);

extern u32 D_00401328[][9];

extern s32 func_00314C10(DatPartyRecord *);

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern s32 scrSetFlag(DatPartyRecord *, u16);
extern u32 ptyGetSkillNibbleState(DatPartyRecord *, u16);
extern s32 scrFindSlot(DatPartyRecord *, u16);
extern s32 scrRemoveSlot(DatPartyRecord *, u16);

typedef struct ScriptFlagSlot {
    u8 id;             /* 0x00 */
    u8 pad01[3];
    u32 flag;          /* 0x04 */
} ScriptFlagSlot;

extern ScriptFlagSlot D_00404AA8[];

extern u32 scrGetEntryRequirementFlags(u16);

extern u8 D_00401324[];

extern u8 D_00401325[];

extern u16 D_00401326[];

extern u8 D_0045C828[];


typedef struct MantraNodePos {
    u32 header;
    s16 x;
    s16 y;
    struct MantraNodePos *adjacent[6];
} MantraNodePos;

extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);

extern u8 D_00401320[][36];

extern u8 D_0040132C[];

extern u16 D_00401332[];


extern s32 func_00359A98(const char *, const char *);

extern void func_00359AC0(s32, const char *, s32, s32);

extern void func_003594A8(s32);

extern char sdfDebugLogAppendMode[];

extern char sdfDebugLogPairFormat[];


void func_003140C8(s32 useCurrentProfile, DatPartyRecord *unit);

extern u32 itfDrawBankTextWithLayoutFlags(s32, s32, u32, u16, u32, u32);

extern void frFontSetChildColors(u32, u32);

extern void func_0019D550(u32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(u32);

void sdfAppendFormattedDebugLogPair(s32 left, s32 right) {
    s32 file = func_00359A98("debug.log", sdfDebugLogAppendMode);
    if (file != 0) {
        func_00359AC0(file, sdfDebugLogPairFormat, left, right);
        func_003594A8(file);
    }
}

/* Clear the complete per-unit profile-record table in game state. */
void ptyClearProfileRecords(void) {
    memset(datGameState->profileRecords, 0, sizeof(datGameState->profileRecords));
}

void func_00313C70(DatPartyRecord *work) {
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
        func_00314A80(work, scriptId);
        ptyAddProfileRecordValueClamped(work, amount);
    }
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00313D80);

/* Load nonzero preset IDs into their original slots; zero IDs leave slots unchanged. */
void ptyLoadPresetSkillSlots(DatPartyRecord *unit) {
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
void ptyMarkPresetSkillPool(DatPartyRecord *unit) {
    u16 *presetSkills = D_004052F8[unit->unitId];
    u32 presetIndex = 0;
    do {
        u16 skillId = *presetSkills++;
        if (skillId != 0) {
            scrSetFlag(unit, skillId);
        }
        presetIndex++;
    } while (presetIndex < PTY_PRESET_POOL_SKILL_COUNT);
    if (mdlFlagTest(PTY_PRESET_POOL_EXTRA_GATE)) {
        func_0011CA88(unit);
    }
}

extern void func_00313D80(s32, DatPartyRecord *);
void ptyRecomputeMaxHpMp(DatPartyRecord *);
extern void func_00314868(DatPartyRecord *, u16);
s32 func_00315388(u16, PrfSkillList *);
void scrClearAllSecondaryScriptFlags(DatPartyRecord *);
void scrSetSecondaryScriptFlag(DatPartyRecord *, u16);

void func_003140C8(s32 mode, DatPartyRecord *unit) {
    PrfSkillList list;
    s32 profile;
    u16 profileId;
    u32 i;

    func_00313D80(mode, unit);
    switch (mode) {
    case 0:
        ptyLoadPresetSkillSlots(unit);
        ptyMarkPresetSkillPool(unit);
        ptyRecomputeMaxHpMp(unit);
        unit->hp = unit->maxHp;
        unit->mp = unit->maxMp;
        break;
    case 1:
        profile = func_00314C10(unit);
        profileId = profile;
        if (profile != 0 && func_00314990(unit, profileId) == 0) {
            func_00314868(unit, profileId);
            memset(&list, 0, sizeof(list));
            func_00315388(profileId, &list);
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
        DatPartyRecord *unit = &datGameState->party[unitIndex];
        if ((unit->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
            s32 candidateUnitId;
            for (candidateUnitId = 0; candidateUnitId < PTY_PROFILE_UNIT_ID_COUNT; candidateUnitId++) {
                if (unit->unitId == candidateUnitId) {
                    func_003140C8(0, unit);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314298);

void ptyRecomputeMaxHpMp(DatPartyRecord *unit) {
    func_00314298(unit, 0);
}

extern u32 strlen(const char *text);
extern char *strcpy(char *destination, const char *source);
extern void *memcpy(void *destination, const void *source, u32 size);

void func_00314500(u16 scriptId, s32 mode, char *destination) {
    u8 *name = D_00435E50[scriptId].encodedText;
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


u32 scrGetIndexedRecordAddress(u32 scriptId, s32 *record) {
    *record = (s32)&D_00435E50[scriptId & 0xffff];
    return 1;
}

/* Read the configured capacity for an unchecked profile ID. */
u32 ptyGetProfileRecordCap(u16 profileId) {
    return D_00401328[profileId][0];
}

void ptySetProfileRecordValue(DatPartyRecord *work, u16 scriptId, u32 value) {
    DatProfileRecord *record;

    record = ptyGetProfileRecordPointer(work, scriptId);
    record->value = value;
}

/* Store the configured capacity in an unchecked profile record. */
void ptySetProfileRecordToCap(DatPartyRecord *unitAddress, u16 profileId) {
    DatProfileRecord *record;
    u32 recordCap;

    record = ptyGetProfileRecordPointer(unitAddress, profileId);
    recordCap = ptyGetProfileRecordCap(profileId);
    record->value = recordCap;
}

/* Add to the selected record and cap the unsigned result; no selection returns zero. */
u32 ptyAddProfileRecordValueClamped(DatPartyRecord *unit, u32 increment) {
    DatProfileRecord *record;
    u32 recordCap;
    u8 selectedProfile;
    if (func_00314C10(unit) == 0) return 0;
    record = ptyGetCurrentProfileRecord(unit);
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
u32 sdfSetFlagBySlotId(DatPartyRecord *unit, u32 slotId) {
    ScriptFlagSlot *slotCursor = (ScriptFlagSlot *)((u8 *)D_00404AA8 + (unit->unitId << SDF_FLAG_SLOT_TABLE_SHIFT));
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

void scrClearProfileFlagsTable(void) {
    memset(datGameState->mantraBits, 0, sizeof(datGameState->mantraBits));
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314868);

s32 func_00314990(DatPartyRecord *work, u16 id) {
    u32 word, shift;
    prfDecodeFlagPair(id, &word, &shift);
    return (datGameState->mantraBits[work->unitId].words[word] & (1 << shift)) != 0;
}

/* Require the primary profile flag for every row not marked as excluded. */
s32 prfAreAllRequiredProfileFlagsSet(DatPartyRecord *unit) {
    u32 profileIndex;
    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        u16 profileId = profileIndex;
        if (!(scrGetEntryRequirementFlags(profileId) & PRF_REQUIREMENT_EXCLUSION_BIT) && !func_00314990(unit, profileId)) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314A80);

s32 func_00314B00(DatPartyRecord *work, u16 id) {
    u32 word, shift;
    prfDecodeFlagPair(id, &word, &shift);
    return (datGameState->mantraBits[work->unitId].words[word] & (1 << (shift + 1))) != 0;
}

u32 scrGetSelectedScriptEntryId(DatPartyRecord *work) {
    return work->profileId;
}

/* Address an unchecked profile record within the unit's record bank. */
DatProfileRecord *ptyGetProfileRecordPointer(DatPartyRecord *unit, u16 profileId) {
    return &datGameState->profileRecords[unit->unitId][profileId];
}

u32 ptyGetProfileRecordValue(DatPartyRecord *work, u16 scriptId) {
    DatProfileRecord *record;

    record = ptyGetProfileRecordPointer(work, scriptId);
    return record->value;
}

/* Resolve the unit's stored script-entry selection without an extra mask. */
DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *unit) {
    return ptyGetProfileRecordPointer(unit, scrGetSelectedScriptEntryId(unit));
}

s32 func_00314C10(DatPartyRecord *work) {
    return work->profileId;
}

/* Store the narrowed selection, initialize its profile state, and return the stored ID. */
u8 scrSelectScriptEntryAndInitialize(DatPartyRecord *unit, u32 profileId) {
    unit->profileId = profileId;
    func_00314A80(unit, (u16)profileId);
    return unit->profileId;
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

void scrClearPackedScriptFlags(DatPartyRecord *work) {
    memset(work->skillFlags, 0, 0x154);
}

/* Set bit 0 of the selected nibble; the return value is always 1. */
s32 scrSetFlag(DatPartyRecord *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex(work, flagId, &wordIndex, &bitShift);
    work->skillFlags[wordIndex] |= SCR_FLAG_PRIMARY_BIT << bitShift;
    return 1;
}

void scrRemoveAvailableSkillFlagAndSlot(DatPartyRecord *work, u16 id) {
    u32 word, shift;
    u32 status = ptyGetSkillNibbleState(work, id);
    if (status == 1) {
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

    memset(datGameState->scriptFlags, 0, sizeof(datGameState->scriptFlags));
    first = mnuPickPairedTableValue(0x10, 0);
    last = mnuPickPairedTableValue(0x10, 1);
    for (; (s32)first < last; first = first + 1) {
        func_001B7940(first & 0xffff, 1);
    }
}

/* Set the global bit for an ID in the supported half-open range. */
void scrSetGlobalBitFlag(u32 flagId) {
    u16 bitIndex;
    flagId &= SCR_FLAG_ID_MASK;
    if (flagId < SCR_GLOBAL_FLAG_FIRST_ID) return;
    if (flagId >= SCR_GLOBAL_FLAG_END_ID) return;
    bitIndex = flagId + SCR_GLOBAL_FLAG_WRAP_BIAS;
    datGameState->scriptFlags[bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT] |= SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK);
}

/* Return the global bit's raw word mask, not a normalized boolean. */
u32 scrTestGlobalBitFlag(u16 bitIndex) {
    if (bitIndex < SCR_GLOBAL_FLAG_FIRST_ID) return 0;
    if (bitIndex >= SCR_GLOBAL_FLAG_END_ID) return 0;
    bitIndex += SCR_GLOBAL_FLAG_WRAP_BIAS;
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
void scrClearAllSecondaryScriptFlags(DatPartyRecord *work) {
    s32 flagId = 0;
    do {
        scrClearSecondaryScriptFlag(work, (u16)flagId);
        flagId++;
    } while (flagId < SCR_SECONDARY_FLAG_COUNT);
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
u8 func_003151D0(u16 profileId) {
    return D_00401324[profileId * PRF_PROFILE_PARAM_BYTES];
}

/* Read the threshold used by the profile-level check. */
u16 prfGetRequiredProfileLevel(u16 profileId) {
    return D_00401326[profileId * (PRF_PROFILE_PARAM_BYTES / sizeof(D_00401326[0]))];
}

/* Read a byte parameter from the unchecked profile row. */
u8 func_00315220(u16 profileId) {
    return D_00401325[profileId * PRF_PROFILE_PARAM_BYTES];
}

/* Read an unchecked byte offset within the profile parameter row. */
u32 prfGetIndexedProfileByte(u16 profileId, s32 byteIndex) {
    return D_0040132C[byteIndex + profileId * PRF_PROFILE_PARAM_BYTES];
}

/* Read the selected profile's indexed byte when a selection is present.
 * Retain both selection reads and the no-selection fall-through. */
u32 scrCallIfOperandReady(DatPartyRecord *operand, s32 byteIndex) {
    s32 selectedProfileId = func_00314C10(operand);

    if (func_00314C10(operand)) {
        return prfGetIndexedProfileByte((u16)selectedProfileId, byteIndex);
    }
}

/* Read one of the profile row's eight skills; an unsigned out-of-range slot returns zero. */
u16 prfGetSkillAtIndex(u16 profileId, u32 skillIndex) {
    if (skillIndex >= PRF_SKILL_LIST_ENTRY_COUNT) {
        return 0;
    }
    return D_00401332[profileId * PRF_SKILL_TABLE_STRIDE + skillIndex];
}

u32 func_00315318(void) {
    return 0;
}

s32 ptyCheckLevelAtLeastProfileParam7b6(DatPartyRecord *work, u16 scriptId) {
    s32 current = work->level;
    if (current < (s32)prfGetRequiredProfileLevel(scriptId)) return 0;
    return 1;
}

/* Clear the paired output words; the first two arguments are unused. */
void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, SCR_PAIRED_OUTPUT_BYTES);
}

u8 scrIsSelectedScriptEntryId(DatPartyRecord *work, u16 id) {
    return work->profileId == id;
}


s32 func_00315388(u16 profile, PrfSkillList *output) {
    PrfSkillList list;
    u32 i;
    u16 *skills;
    u16 skill;

    memset(&list, 0, sizeof(PrfSkillList));
    list.count = 0;
    skills = &D_00401332[profile * 18];
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
        profileSkillCursor = &D_00401332[selectedProfileId * PRF_SKILL_TABLE_STRIDE];
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

/* Return the selected profile's skill count without flagged skills; the second argument is unused. */
s32 prfBuildSkillListState0(unit, unusedProfile, output)
DatPartyRecord *unit;
DatProfileRecord *unusedProfile;
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
s32 ptyHasAllReqProfiles(DatPartyRecord *unit, PrfRequirementOperand *operand) {
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand->profileIds;
        do {
            if (func_00314990(unit, profileIds[profileIndex++]) == 0) {
                return 0;
            }
        } while (profileIndex < profileCount);
    }
    return 1;
}

/* Compare the number of flagged IDs in the list with its minimum count. */
s32 ptyReqProfileCountAtLeast(DatPartyRecord *unit, PrfRequirementOperand *operand) {
    u32 matchedCount = 0;
    s32 profileIndex = 0;
    s32 profileCount = prfCountProfileList(operand);
    if (profileCount > 0) {
        u8 *profileIds = operand->profileIds;
        do {
            if (func_00314990(unit, profileIds[profileIndex++]) != 0) {
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
s32 ptyProfileCountAtLeast(DatPartyRecord *unit, PrfRequirementOperand *operand) {
    u32 profileIndex;
    u32 matchedCount = 0;

    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        if (func_00314990(unit, (u16)profileIndex)) {
            if (D_00401320[profileIndex][5] >= operand->profileThreshold) {
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
                DatPartyRecord *partyUnit = &datGameState->party[unitIndex];
                if ((partyUnit->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
                    if (func_00314990(partyUnit, (u16)profileId) != 0) {
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
s32 prfReqCheckUnitLevel(DatPartyRecord *unit, PrfRequirementOperand *operand) {
    if (unit->level < operand->levelThreshold) {
        return 0;
    }
    return 1;
}

/* Test the global counter against the prerequisite's minimum. */
s32 prfReqCheckGlobalCounter(PrfRequirementOperand *operand) {
    if (datGameState->header.currency < operand->minimum) {
        return 0;
    }
    return 1;
}

/* The word-sized ID API narrows only when calling the node-record lookup. */
s32 func_00315950(s32 id, DatPartyRecord *context, s32 mode) {
    s32 result = 1;
    MantraNodePos *record = mnuGetMantraNodePositionRecord(id);
    MantraNodePos **adjacent;
    u32 i;

    if (record == NULL) {
        return 0;
    }
    if (mode == 0x40) {
        adjacent = record->adjacent;
        i = 0;
        do {
            if (*adjacent != NULL &&
                func_00314990(context, (*adjacent)->header >> 16) == 0 &&
                ((*adjacent)->header & 0xF) != 2) {
                result = 0;
            }
            i++;
            adjacent++;
        } while (i < 6);
        return result;
    }
    adjacent = record->adjacent;
    i = 0;
    do {
        if (*adjacent != NULL &&
            func_00314990(context, (*adjacent)->header >> 16) != 0 &&
            ((*adjacent)->header & 0x100) == 0) {
            return 1;
        }
        i++;
        adjacent++;
    } while (i < 6);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00315A50);

s32 mnuIsResourceCategoryAvailable(s32 id) {
    MantraNodePos *info = mnuGetMantraNodePositionRecord(id);
    if (info == 0) {
        return 0;
    }
    return D_0045C828[(s32)(info->header << 24) >> 28] != 0;
}

s32 func_00315C40(u32 index) {
    if (index >= 17) {
        return 0;
    }
    return D_0045C828[index] != 0;
}

/* The evaluator masks its incoming requirement ID to the lower sixteen bits. */

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00315C68);

s32 func_00315FA0(u32 mode, DatPartyRecord *unit, u16 id) {
    return func_00315C68(mode, 0, unit, id, 0);
}

/* Read requirement flags for an unchecked profile ID. */
u32 scrGetEntryRequirementFlags(u16 requirementId) {
    return *(u32 *)D_00401320[requirementId];
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
s32 func_00316020(DatPartyRecord *operand, u16 requirementId) {
    u32 ruleResult;
    u32 groupIndex;
    u32 flagIndex;

    requirementId &= SCR_FLAG_ID_MASK;
    if ((scrGetEntryState(requirementId) & PRF_REQUIREMENT_RULES_BIT) != 0) {
        if (func_00315C68(0, 0, operand, requirementId, &ruleResult) == 0) {
            return ruleResult != 0;
        }
    }
    for (groupIndex = 0; groupIndex < PRF_FALLBACK_GROUP_COUNT; groupIndex++) {
        if (D_00404A20[groupIndex].id == requirementId) {
            for (flagIndex = 0; flagIndex < PRF_FALLBACK_FLAG_COUNT; flagIndex++) {
                u32 flagId = D_00404A20[groupIndex].flags[flagIndex];

                if (flagId != 0 && mdlFlagTest(flagId) == 0) {
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
    return D_00404A90[entryIndex].active;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", prfReqGetPair);

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
                mdlFlagSet(flagId);
            }
        }
    } while (entryIndex++ >= 0);
}

PrfRequirementRecord *scrGetEntryDescriptor(u16 id) {
    return &D_00402BE0[id];
}

void scrSetEntryFlag(DatPartyRecord *context, u16 entryId, u32 bit) {
    DatProfileRecord *entry;
    if (bit < 16) {
        entry = ptyGetProfileRecordPointer(context, entryId);
        entry->flags |= 1 << bit;
    }
}

void scrClearEntryFlag(DatPartyRecord *context, u16 entryId, u32 bit) {
    DatProfileRecord *entry;
    if (bit < 16) {
        entry = ptyGetProfileRecordPointer(context, entryId);
        entry->flags &= ~(1 << bit);
    }
}

s32 scrTestEntryFlag(DatPartyRecord *context, u16 entryId, u32 bit) {
    if (bit >= 16) {
        return 0;
    }
    return (ptyGetProfileRecordPointer(context, entryId)->flags & (1 << bit)) != 0;
}

void scrSetEntryLowFlags(DatPartyRecord *context, u16 entryId, u16 lowFlags) {
    DatProfileRecord *entry = ptyGetProfileRecordPointer(context, entryId);
    entry->flags = (entry->flags & 0xFFFF0000) | lowFlags;
}

u16 scrGetEntryLowFlags(DatPartyRecord *context, u16 entryId) {
    return ptyGetProfileRecordPointer(context, entryId)->flags;
}

void frFontQueueColoredGlyph(s32 x, s32 y, u32 first, u16 width, u32 second, s32 option) {
    u32 handle = itfDrawBankTextWithLayoutFlags(x, y, first, width, (u32)frFontColoredGlyphResource, 0);
    frFontSetChildColors(handle, second);
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
}

u8 *frFontGetColoredGlyphResource(void) {
    return frFontColoredGlyphResource;
}

/* Per-entry state: frame counter (-1 = free) and the alpha chosen at spawn. */
typedef struct SdfFlagListMark {
    s32 timer;
    u8 alpha;
    u8 pad05[3];
} SdfFlagListMark;

typedef struct SdfFlagListWork {
    s32 frame;
    u8 pad04[4];
    SdfFlagListMark *marks;   /* 0x08 */
    f32 (*vertices)[4];
    u32 *colors;
    u8 unk14[0x24];           /* 0x14: colour curve read by func_002D7458 */
    u8 unk38[4];
    s32 surfaceIndex;
    u8 pad40[8];
    s32 maxFrames;
    u32 count;         /* 0x4C */
    f32 speed;         /* 0x50 */
    u32 resource;
} SdfFlagListWork;

/* Reset each entry's timer to free, then clear both colour words per entry. */
void sdfResetFlagListEntries(s32 workAddress) {
    u32 entryCount;
    SdfFlagListMark *markCursor;
    u32 entryIndex;

    entryIndex = 0;
    entryCount = ((SdfFlagListWork *)workAddress)->count;
    markCursor = ((SdfFlagListWork *)workAddress)->marks;
    if (entryCount != 0) {
        do {
            entryIndex = entryIndex + 1;
            markCursor->timer = SDF_FLAG_LIST_RESET_MARK;
            markCursor++;
        } while (entryIndex < entryCount);
    }
    memset(((SdfFlagListWork *)workAddress)->colors, 0, entryCount << SDF_FLAG_LIST_VALUE_SHIFT);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316528);

void sdfInitializeFlagListFromResource(void) {
    effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer());
}

void sdfReleaseFlagListResource(s32 work) {
    sdfReleaseResourceAllocation(((SdfFlagListWork *)work)->resource);
}

extern f32 sdfViewTargetVector[4];
extern u32 D_00438918;
extern void vuBuildLookAtBasis(void);
extern u32 func_002D7458(u8 *, u8 *, s32, s32);
extern f32 sdfAtan2Poly(f32 ratio);
extern f32 effMiscRandUnitFloat(void *state);
extern u32 effMiscRand(void *state);
extern u8 effSharedRandomState[];

/* Advance the drop list: respawn a random number of free drops around the camera, move live drops along their
 * own direction, and fade them over their last ten frames. */
void func_00316680(SdfFlagListWork *work) {
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

    maxFrames = work->maxFrames;
    speed = work->speed;
    frame = work->frame;
    if (maxFrames > 0 && frame >= maxFrames) {
        return;
    }
    spawn = 1;
    vuBuildLookAtBasis();
    color = func_002D7458(work->unk14, work->unk38, frame, maxFrames);
    count = work->count;
    spawnRange = count >> 4;
    halfFov = sdfSceneProjectionParameters.fov * 0.5f;
    mark = work->marks;
    vertices = work->vertices;
    colors = work->colors;
    spreadX = sdfAtan2Poly(halfFov * 1.5f * 512.0f / 448.0f);
    spreadY = sdfAtan2Poly(halfFov);
    /* Retail evaluates these two as well and never uses the results. */
    sdfAtan2Poly(sdfSceneProjectionParameters.fov);
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
                    if (D_00438918 != 0) {
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
                    if (D_00438918 != 0) {
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

typedef struct GsSurface {
    u8 pad00[0x10];
    void (*submit)(struct GsSurface *, void *);
} GsSurface;

extern GsSurface *D_0040A958[];
extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfMemoryGetBlockAddress(s32);
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void *func_00348158(f32 (*)[4], u32 *, u32, s32);

/* Copy vertex pairs, optionally add the view target, and submit the packet list.
 * count is reused first as an entry count and then as a vertex count. */
void func_00316C88(SdfFlagListWork *work) {
    f32 (*sourceVertices)[4];
    f32 (*copiedVertices)[4];
    u32 count;
    u32 vertexIndex;
    s32 vertexAllocation;
    void *packetList;
    GsSurface *surface;

    if (work->maxFrames != 0 && work->frame >= work->maxFrames) {
        return;
    }
    count = work->count;
    sourceVertices = work->vertices;
    vertexAllocation = sdfAllocGeneralBlock(count * SDF_FLAG_LIST_ENTRY_VERTEX_BYTES);
    count *= SDF_FLAG_LIST_VERTICES_PER_ENTRY;
    copiedVertices = sdfMemoryGetBlockAddress(vertexAllocation);
    for (vertexIndex = 0; vertexIndex < count; vertexIndex++) {
        VU0_LOAD_VF(vf10, &sourceVertices[vertexIndex]);
        if (D_00438918 != 0) {
            VU0_LOAD_VF(vf11, sdfViewTargetVector);
            VU0_ADD(vf10, vf10, vf11);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, &copiedVertices[vertexIndex]);
    }
    packetList = sdfAllocPacketAligned(SDF_FLAG_LIST_PACKET_BYTES);
    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, func_00348158(copiedVertices, work->colors, work->count * SDF_FLAG_LIST_VERTICES_PER_ENTRY, 0x40));
    surface = D_0040A958[work->surfaceIndex];
    surface->submit(surface, packetList);
    sdfReleaseResourceAllocation(vertexAllocation);
}

/* Read the camera color effect's stored float. */
float scrGetOperandFloatValue(RgbAlpha *operand) {
    return operand->f50;
}

/* Replace the camera color effect's stored float without validation. */
void scrSetOperandFloatValue(RgbAlpha *operand, float value) {
    operand->f50 = value;
}

void func_00316DE0(SdfFlagListWork *work) {
    func_002D7458(work->unk14, work->unk38, 0, 0);
}

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogAppendMode);

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogPairFormat);

INCLUDE_SDATA(const s32, "game/code_00313BB8", D_00438918);

