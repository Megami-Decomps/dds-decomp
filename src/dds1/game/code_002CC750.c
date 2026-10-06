#include "common.h"
#include "pcp_vu0.h"

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
#define SCR_GLOBAL_FLAG_TABLE_OFFSET 0x2e9d0
#define SCR_GLOBAL_FLAG_WORD_BYTES 4
#define PTY_SKILL_SLOT_COUNT 24
#define PRF_SKILL_LIST_ENTRY_COUNT 8
#define PRF_SKILL_LIST_FLAGGED 4

#define PTY_PRESET_SKILL_SLOT_COUNT 8
#define PTY_PRESET_POOL_SKILL_COUNT 40
/* Observed gate for the post-mark call; its gameplay meaning is not established. */
#define PTY_PRESET_POOL_EXTRA_GATE 0xB90
#define PTY_PROFILE_UNIT_LAST_INDEX 4
#define PTY_PROFILE_UNIT_TABLE_OFFSET 0xA60
#define PTY_PROFILE_UNIT_STRIDE 0x1A4
#define PTY_PROFILE_UNIT_ID_COUNT 0x10
#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1
#define PTY_PROFILE_RECORD_TABLE_OFFSET 0x2EBB0
#define PTY_PROFILE_RECORD_TABLE_BYTES 0x3000
#define PTY_PROFILE_RECORD_BANK_BYTES 0x300
#define PRF_PROFILE_RECORD_BYTES 8
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
#define SDF_FLAG_LIST_MARK_WORDS 2
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
extern void ptyMergeStockSkills(u8 *);

extern void (*sdfTickCallback)(void);

extern u64 sdfAllocateBlockBySizeThreshold(u64);

extern u32 D_003BD2C8;

extern u32 prfGetCapValue(u16);

extern u32 prfIsRequirementExcluded(u16);

extern u32 ptyGetProfileRecord(u32, u16);

extern s32 datGameState;

/* Operand block used by the script VM helpers near ptyGetProfileRecord (layout inferred from field accesses). */
typedef struct ScrVmOperand {
    u8 pad_0x00[0x04]; // 0x00
    u16 h04;           // 0x04
    u8 pad_0x06[0x0E]; // 0x06
    u16 h14;           // 0x14
    u8 pad_0x16[0x3A]; // 0x16
    float f50;         // 0x50
    u8 unk54;          // 0x54
    s8 selectedIndex;  // 0x55: active operand chosen by scrSelectOperandIndex
    u8 pad_0x56[2];    // 0x56
    u32 flags[0x4C];   // 0x58: eight 4-bit flag slots per word
} ScrVmOperand;

/* Profile prerequisite operands reuse +0x08 as the required threshold. */
typedef struct PrfRequirementOperand {
    u8 pad00[8];
    u32 requiredValue;      /* 0x08 */
    u8 profileIds[8];       /* 0x0C */
} PrfRequirementOperand;

typedef struct PtyGameCounter {
    u8 pad00[0x3C];
    u32 currency;           /* 0x3C */
} PtyGameCounter;

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

/* 0xAARRGGBB color split into RGB and alpha fields. */
typedef struct RgbAlpha {
    u8 pad_0x00[0x18]; // 0x00
    u32 rgb;           // 0x18: low 24 bits of packed color
    u8 pad_0x1C[0x1C]; // 0x1C
    u32 alpha;         // 0x38: high byte of packed color
    u32 x3C;           // 0x3C
    u8 pad_0x40[0x10]; // 0x40
    float f50;         // 0x50
} RgbAlpha; // 0x54

extern s32 CreateSema(void *);

extern Entry24B D_00393220[];

extern Entry24W D_00393234[];

/* 84-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry84W {
    u32 v0;             // 0x00
    u8 pad_0x04[0x50]; // 0x04
} Entry84W; // 0x54

extern void ptySetProfileFlag1(void *, u16);

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

/* Copy source for itfCopyColorFields (layout inferred from field accesses). */
typedef struct CfSrc {
    u8 pad_0x00[0x04]; // 0x00
    u32 rgb;           // 0x04
    u8 pad_0x08[0x1C]; // 0x08
    u32 alpha;         // 0x24
    u32 x28;           // 0x28
    u8 pad_0x2C[0x10]; // 0x2C
    float f3C;         // 0x3C
} CfSrc; // 0x40

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
extern u32 fileResolvePrimaryBuffer(void);
extern void effCreateSelectionFlagListFromWork(u32);
extern s32 ptyTestProfileFlag0(s32, u16);
extern u16 D_003907BC[];
u32 ptyGetCurrentProfileRecord(ScrVmOperand *);
s8 ptyGetCurrentProfileId(ScrVmOperand *);

/* Party profile header; each party slot occupies 0x1A4 bytes in game state. */
typedef struct PtyProfileUnit {
    u16 flags;          /* 0x00: bit 0 indicates an occupied slot */
    u8 pad02[2];
    u16 unitId;         /* 0x04: profile preset table index */
    u8 pad06[0x1C];
    u16 skills[24];     /* 0x22 */
} PtyProfileUnit;

extern void ptyRecomputeMaxVitals(PtyProfileUnit *, const s32 *);

void sdfAppendFormattedDebugLogPair(s32 left, s32 right) {
    s32 file = func_002FE950("debug.log", sdfDebugLogAppendMode);
    if (file != 0) {
        func_002FE978(file, sdfDebugLogPairFormat, left, right);
        func_002FE360(file);
    }
}

/* Clear the complete per-unit profile-record table in game state. */
void ptyClearProfileRecords(void) {
    memset(datGameState + PTY_PROFILE_RECORD_TABLE_OFFSET, 0, PTY_PROFILE_RECORD_TABLE_BYTES);
}

extern void ptySelectProfileStage(PtyProfileUnit *);
INCLUDE_ASM(const s32, "game/code_002CC750", ptySelectProfileStage);

extern void ptyApplyProfilePreset(s32, PtyProfileUnit *);
INCLUDE_ASM(const s32, "game/code_002CC750", ptyApplyProfilePreset);

/* Load nonzero preset IDs into their original slots; zero IDs leave slots unchanged. */
void ptyLoadPresetSkillSlots(u8 *unit) {
    u16 *presetSkills = D_00393A80[((PtyProfileUnit *)unit)->unitId].skillSlots;
    u16 *skillCursor = ((PtyProfileUnit *)unit)->skills;
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
void ptyMarkPresetSkillPool(u8 *unit) {
    u16 *presetSkills = D_00393A80[((PtyProfileUnit *)unit)->unitId].poolSkills;
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

void ptyRebuildProfileSkills(s32 useCurrentProfile, u8 *unit);

/* Rebuild skill lists for the five occupied party slots. */
void ptyRebuildAllProfiles(void) {
    s32 unitCountdown;
    s32 unitOffset;

    for (unitOffset = 0, unitCountdown = PTY_PROFILE_UNIT_LAST_INDEX; unitCountdown >= 0; unitCountdown--) {
        u8 *unit = (u8 *)datGameState + PTY_PROFILE_UNIT_TABLE_OFFSET + unitOffset;

        if ((((PtyProfileUnit *)unit)->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
            s32 candidateUnitId;

            for (candidateUnitId = 0; candidateUnitId < PTY_PROFILE_UNIT_ID_COUNT; candidateUnitId++) {
                if (((PtyProfileUnit *)unit)->unitId == candidateUnitId) {
                    ptyRebuildProfileSkills(0, unit);
                }
            }
        }
        unitOffset += PTY_PROFILE_UNIT_STRIDE;
    }
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyRecomputeMaxVitals);

void ptyRecomputeMaxHpMp(u32 unit) {
    ptyRecomputeMaxVitals((PtyProfileUnit *)unit, NULL);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD0D8);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD240);

/* Read the configured capacity for an unchecked profile ID. */
u32 prfGetCapValue(u16 profileId) {
    return D_003907B8[profileId].v0;
}

/* Store the configured capacity in an unchecked profile record. */
void ptySetProfileRecordToCap(u32 unitAddress, u16 profileId) {
    u32 *recordAddress;
    u32 recordCap;

    recordAddress = (u32 *)ptyGetProfileRecord(unitAddress, profileId);
    recordCap = prfGetCapValue(profileId);
    *recordAddress = recordCap;
}

/* Add to the selected record and cap the unsigned result; no selection returns zero. */
u32 ptyAddProfilePoints(ScrVmOperand *unit, s32 increment) {
    u32 *record;
    u32 recordCap;
    u32 recordValue;
    s32 selectedProfile;
    if (ptyGetCurrentProfileId(unit) == 0) {
        return 0;
    }
    record = (u32 *)ptyGetCurrentProfileRecord(unit);
    selectedProfile = unit->selectedIndex;
    *record += increment;
    recordCap = prfGetCapValue(selectedProfile & SCR_FLAG_ID_MASK);
    recordValue = *record;
    if (recordCap < recordValue) {
        *record = recordCap;
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
u32 sdfSetFlagBySlotId(u8 *unit, u32 slotId) {
    ScriptFlagSlot *slotCursor = (ScriptFlagSlot *)((u8 *)D_00393280 + (*(u16 *)(unit + 4) << SDF_FLAG_SLOT_TABLE_SHIFT));
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

extern void ptyApplyProfile(PtyProfileUnit *, u16);
INCLUDE_ASM(const s32, "game/code_002CC750", ptyApplyProfile);

s32 ptyTestProfileFlag0(s32 work, u16 id) {
    u32 word;
    u32 shift;
    u32 *flags;

    prfDecodeFlagPair(id, &word, &shift);
    flags = (u32 *)datGameState;
    return (flags[0x2e9f0 / 4 + word + ((ScrVmOperand *)work)->h04 * 7] & (1 << shift)) != 0;
}

/* Require the primary profile flag for every row not marked as excluded. */
s32 scrCheckStateBits(ScrVmOperand *unit) {
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

s32 ptyTestProfileFlag1(ScrVmOperand *work, u16 id) {
    u32 word;
    u32 shift;
    u32 *flags;

    prfDecodeFlagPair(id, &word, &shift);
    flags = (u32 *)datGameState;
    shift++;
    return (flags[0x2e9f0 / 4 + word + work->h04 * 7] & (1 << shift)) != 0;
}

s8 scrGetSelectedOperandIndex(ScrVmOperand *op) {
    return op->selectedIndex;
}

/* Address an unchecked profile record within the unit's record bank. */
u32 ptyGetProfileRecord(u32 unit, u16 profileId) {
    u32 unitRecordsBase = datGameState + *(u16 *)(unit + 4) * PTY_PROFILE_RECORD_BANK_BYTES;

    return unitRecordsBase + profileId * PRF_PROFILE_RECORD_BYTES + PTY_PROFILE_RECORD_TABLE_OFFSET;
}

u32 ptyGetProfileRecordValue(u32 unit, u16 profileId) {
    u32 *record;

    record = (u32 *)ptyGetProfileRecord(unit, profileId);
    return *record;
}

/* Resolve the selected ID using the native low-16-bit conversion. */
u32 ptyGetCurrentProfileRecord(ScrVmOperand *unit) {
    return ptyGetProfileRecord((u32)unit, scrGetSelectedOperandIndex(unit) & SCR_FLAG_ID_MASK);
}

s8 ptyGetCurrentProfileId(ScrVmOperand *op) {
    return op->selectedIndex;
}

/* Store the narrowed selection, mark its paired profile flag, and return the stored ID. */
s8 scrSelectOperandIndex(ScrVmOperand *unit, s32 profileId) {
    unit->selectedIndex = profileId;
    ptySetProfileFlag1(unit, profileId & SCR_FLAG_ID_MASK);
    return unit->selectedIndex;
}

u32 func_002CD7F0(u32 arg0, u32 arg1) {
    return arg1;
}

u32 func_002CD7F8(void) {
    return 0;
}

u32 func_002CD800(void) {
    return 1;
}

/* Decode the low 16 bits: eight four-bit flag slots per word.
 * The unused first argument remains part of the existing calling convention. */
void scrDecodePackedFlagIndex(s32 unused, u32 value, u32 *wordIndex, u32 *bitShift) {
    u32 slotIndex;

    value &= SCR_FLAG_ID_MASK;
    slotIndex = value & SCR_FLAG_SLOT_INDEX_MASK;
    value >>= SCR_FLAG_WORD_INDEX_SHIFT;
    *wordIndex = value;
    *bitShift = slotIndex << SCR_FLAG_SLOT_BIT_SHIFT;
}

/* Set bit 0 of the selected nibble; the return value is always 1. */
s32 scrSetFlag(ScrVmOperand *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    work->flags[wordIndex] |= SCR_FLAG_PRIMARY_BIT << bitShift;
    return 1;
}

/* Set the global bit for an ID in the supported half-open range. */
void scrSetGlobalSeenBit(u32 flagId) {
    u16 bitIndex;
    u32 *flagWord;
    s32 byteOffset;
    flagId &= SCR_FLAG_ID_MASK;
    if (flagId < SCR_GLOBAL_FLAG_FIRST_ID) return;
    if (flagId >= SCR_GLOBAL_FLAG_END_ID) return;
    bitIndex = flagId + SCR_GLOBAL_FLAG_WRAP_BIAS;
    byteOffset = SCR_GLOBAL_FLAG_TABLE_OFFSET + (bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT) * SCR_GLOBAL_FLAG_WORD_BYTES;
    flagWord = (u32 *)(datGameState + byteOffset);
    *flagWord |= SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK);
}

/* Return the global bit's raw word mask, not a normalized boolean. */
u32 scrTestGlobalSeenBit(u32 flagId) {
    u16 bitIndex = flagId;
    u32 *flagWord;
    s32 byteOffset;
    if (bitIndex < SCR_GLOBAL_FLAG_FIRST_ID) return 0;
    if (bitIndex >= SCR_GLOBAL_FLAG_END_ID) return 0;
    bitIndex = bitIndex + SCR_GLOBAL_FLAG_WRAP_BIAS;
    byteOffset = SCR_GLOBAL_FLAG_TABLE_OFFSET + (bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT) * SCR_GLOBAL_FLAG_WORD_BYTES;
    flagWord = (u32 *)(datGameState + byteOffset);
    return *flagWord & (SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK));
}

/* Set only bit 2 of the selected flag nibble. */
void scrSetSecondaryScriptFlag(ScrVmOperand *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    work->flags[wordIndex] |= SCR_FLAG_SECONDARY_BIT << bitShift;
}

/* Clear only bit 2 of the selected flag nibble. */
void scrClearSecondaryScriptFlag(ScrVmOperand *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    work->flags[wordIndex] &= ~(SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Clear secondary bits over the fixed ID range, preserving other nibble bits. */
void scrClearFlags(ScrVmOperand *work) {
    s32 flagId;
    for (flagId = 0; flagId < SCR_SECONDARY_FLAG_COUNT; flagId++) {
        scrClearSecondaryScriptFlag(work, flagId);
    }
}

/* Return bit 2's raw word mask, not a normalized boolean. */
u32 scrGetSecondaryScriptFlag(ScrVmOperand *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    return work->flags[wordIndex] & (SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Bit 1 takes precedence and returns state 2; otherwise return boolean bit 0.
 * The secondary bit does not affect this state. */
u32 ptyGetSkillNibbleState(ScrVmOperand *work, u16 flagId) {
    u32 wordIndex, bitShift;
    u32 flagWord;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    flagWord = work->flags[wordIndex];
    if (flagWord & (SCR_SKILL_STATE_TWO_BIT << bitShift)) {
        return 2;
    }
    return (flagWord & (SCR_FLAG_PRIMARY_BIT << bitShift)) != 0;
}

/* Search all slots for the low 16-bit ID; zero can match an empty slot. */
s32 ptyHasSkill(s32 unit, s32 skillId) {
    u32 maskedSkillId = skillId & SCR_FLAG_ID_MASK;
    u16 *skillSlot = ((PtyProfileUnit *)unit)->skills;
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
s32 scrFindSlot(u8 *unit, u16 skillId) {
    u32 slotIndex;
    u16 *skillSlots = ((PtyProfileUnit *)unit)->skills;
    for (slotIndex = 0; slotIndex < PTY_SKILL_SLOT_COUNT; slotIndex++) {
        if (skillSlots[slotIndex] == skillId) {
            return slotIndex;
        }
    }
    return -1;
}

/* Read a slot's ID, returning zero for an out-of-range unsigned index. */
u16 scrGetSlot(PtyProfileUnit *unit, u32 slotIndex) {
    if (slotIndex >= PTY_SKILL_SLOT_COUNT) {
        return 0;
    }
    return unit->skills[slotIndex];
}

/* Count nonzero skill IDs across every slot. */
u32 scrCountSlots(PtyProfileUnit *unit) {
    u32 occupiedCount = 0;
    u32 slotIndex;

    for (slotIndex = 0; slotIndex < PTY_SKILL_SLOT_COUNT; slotIndex++) {
        if (unit->skills[slotIndex] != 0) {
            occupiedCount++;
        }
    }
    return occupiedCount;
}

/* Replace an unchecked skill-slot index and return its previous identifier. */
u16 scrSetSlot(PtyProfileUnit *unit, s32 slotIndex, u16 skillId) {
    u16 previousSkillId = unit->skills[slotIndex];

    unit->skills[slotIndex] = skillId;
    return previousSkillId;
}

/* Clear the first exact-ID match and report whether a slot was found. */
s32 scrRemoveSlot(u8 *unit, u16 skillId) {
    s32 slotIndex = scrFindSlot(unit, skillId);
    if (slotIndex >= 0) {
        ((PtyProfileUnit *)unit)->skills[slotIndex] = 0;
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

u32 func_002CDDB0() {
    return 0;
}

/* DDS2 reads an indexed profile byte here; DDS1's native callee returns zero.
 * Retain both selection reads and the no-selection fall-through. */
u32 scrCallIfOperandReady(ScrVmOperand *operand, u32 byteIndex) {
    s8 selectedProfileId = ptyGetCurrentProfileId(operand);

    if (ptyGetCurrentProfileId(operand)) {
        return func_002CDDB0((u16)selectedProfileId, byteIndex);
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

u32 ptyCheckLevelAtLeastProfileParam7b6(ScrVmOperand *operand, u16 index) {
    u16 value = operand->h14;
    if (value < prfGetParamWord7b6(index)) {
        return 0;
    }
    return 1;
}

/* Clear the paired output words; the first two arguments are unused. */
void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, SCR_PAIRED_OUTPUT_BYTES);
}

u8 ptyIsCurrentProfileId(s32 operand, u32 profileId) {
    return (s64)((ScrVmOperand *)operand)->selectedIndex == (profileId & 0xffff);
}

typedef struct PrfSkillList {
    u32 flags[8];
    s32 count;
    u16 skills[8];
} PrfSkillList;

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
s32 prfBuildSkillList(ScrVmOperand *unit, u32 unusedProfile, PrfSkillList *output, s32 includeFlagged) {
    PrfSkillList compactedList;
    u32 sourceIndex;
    u16 *profileSkillCursor;
    u16 skillId;
    u32 skillState;
    s32 selectedProfileId;

    memset(&compactedList, 0, sizeof(PrfSkillList));
    selectedProfileId = unit->selectedIndex;
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

/* Build the stored selection's list without flagged skills; the explicit ID is ignored. */
u32 prfBuildSkillListState0(u32 unit, u32 unusedProfile, u32 output) {
    return prfBuildSkillList((ScrVmOperand *)unit, unusedProfile, (PrfSkillList *)output, 0);
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
s32 ptyHasAllReqProfiles(s32 unit, u8 *operand) {
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
s32 ptyReqProfileCountAtLeast(s32 unit, u8 *operand) {
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
s32 ptyProfileCountAtLeast(u8 *unit, u8 *operand) {
    PtyReqEntry *profileTable = (PtyReqEntry *)D_003907B0;
    u32 profileIndex;
    u32 matchedCount = 0;

    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        if (ptyTestProfileFlag0((s32)unit, (u16)profileIndex)) {
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
            s32 unitOffset = 0;
            s32 unitCountdown = PTY_PROFILE_UNIT_LAST_INDEX;
            do {
                u8 *partyUnit = (u8 *)datGameState + PTY_PROFILE_UNIT_TABLE_OFFSET + unitOffset;
                unitOffset += PTY_PROFILE_UNIT_STRIDE;
                if ((((PtyProfileUnit *)partyUnit)->flags & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
                    if (ptyTestProfileFlag0((s32)partyUnit, (u16)profileId) != 0) {
                        profilePresent = 1;
                    }
                }
                unitCountdown--;
            } while (unitCountdown >= 0);
            if (profilePresent == 0) {
                return 0;
            }
            profileIndex++;
        } while (profileIndex < profileCount);
    }
    return 1;
}

/* Compare the unit's level word with the prerequisite's byte threshold. */
u32 prfReqCheckUnitLevel(ScrVmOperand *unit, u8 *operand) {
    if (unit->h14 < operand[4]) {
        return 0;
    }
    return 1;
}

/* Test the global currency counter against a prerequisite threshold. */
u32 prfReqCheckGlobalCounter(u8 *operand) {
    if (((PtyGameCounter *)datGameState)->currency < ((PrfRequirementOperand *)operand)->requiredValue) {
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
                    if (*(u8 *)(id + datGameState + 0x12A0) < minimum) {
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

/* Interleaved mark words and an eight-byte-per-entry value block. */
typedef struct SdfFlagListWork {
    s32 frame;
    u8 pad04[4];
    u32 *marks;               /* 0x08: first word of each pair */
    f32 (*vertices)[4];
    u32 *colors;
    u8 pad14[0x28];
    s32 surfaceIndex;
    u8 pad40[8];
    s32 maxFrames;
    u32 count;                /* 0x4C */
    u8 pad50[4];
    u32 resource;             /* 0x54 */
} SdfFlagListWork;

/* Reset only the first mark word of each pair, then clear both value words per entry. */
void sdfResetFlagListEntries(s32 workAddress) {
    u32 entryCount;
    u32 *markCursor;
    u32 entryIndex;

    entryIndex = 0;
    entryCount = ((SdfFlagListWork *)workAddress)->count;
    markCursor = ((SdfFlagListWork *)workAddress)->marks;
    if (entryCount != 0) {
        do {
            entryIndex = entryIndex + 1;
            *markCursor = SDF_FLAG_LIST_RESET_MARK;
            markCursor = markCursor + SDF_FLAG_LIST_MARK_WORDS;
        } while (entryIndex < entryCount);
    }
    memset(((SdfFlagListWork *)workAddress)->colors, 0, entryCount << SDF_FLAG_LIST_VALUE_SHIFT);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CEAE8);

void sdfInitializeFlagListFromResource(void) {
    effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer());
}

void sdfReleaseFlagListResource(s32 context) {
    sdfReleaseResourceAllocation(((SdfFlagListWork *)context)->resource);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CEC40);

typedef struct GsSurface {
    u8 pad00[0x10];
    void (*submit)(struct GsSurface *, void *);
} GsSurface;

extern GsSurface *D_00398098[];
extern f32 sdfViewTargetVector[4];
extern u32 D_003BD2C8;
extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfMemoryGetBlockAddress(s32);
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void *func_002EF2B0(f32 (*)[4], u32 *, u32, s32);

/* Copy vertex pairs, optionally add the view target, and submit the packet list.
 * count is reused first as an entry count and then as a vertex count. */
void func_002CF248(SdfFlagListWork *work) {
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
        if (D_003BD2C8 != 0) {
            VU0_LOAD_VF(vf11, sdfViewTargetVector);
            VU0_ADD(vf10, vf10, vf11);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, &copiedVertices[vertexIndex]);
    }
    packetList = sdfAllocPacketAligned(SDF_FLAG_LIST_PACKET_BYTES);
    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, func_002EF2B0(copiedVertices, work->colors, work->count * SDF_FLAG_LIST_VERTICES_PER_ENTRY, 0x40));
    surface = D_00398098[work->surfaceIndex];
    surface->submit(surface, packetList);
    sdfReleaseResourceAllocation(vertexAllocation);
}

/* Read the operand's stored float; its gameplay meaning remains unknown. */
float scrGetOperandFloatValue(ScrVmOperand *operand) {
    return operand->f50;
}

/* Replace the operand's stored float without validation. */
void scrSetOperandFloatValue(ScrVmOperand *operand, float value) {
    operand->f50 = value;
}

void func_002CF3A0(s32 arg0) {
    func_00296F58(arg0 + 0x14, arg0 + 0x38, 0, 0);
}

void itfSetPackedRgbAlpha(RgbAlpha *p, u32 color) {
    p->rgb = color & 0xFFFFFF;
    p->alpha = color >> 24;
}

u32 func_002CF3E8(s32 arg0) {
    return *(u32 *)(arg0 + 0x3c);
}

void func_002CF3F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void itfCopyColorFields(RgbAlpha *dst, CfSrc *src) {
    dst->rgb = src->rgb;
    dst->f50 = src->f3C;
    dst->alpha = src->alpha;
    dst->x3C = src->x28;
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

