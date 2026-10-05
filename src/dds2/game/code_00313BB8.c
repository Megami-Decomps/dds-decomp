#include "common.h"
extern void memset();

#include "fpu.h"
#include "pcp_vu0.h"

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
#define SCR_GLOBAL_FLAG_TABLE_OFFSET 0x16ef0
#define SCR_GLOBAL_FLAG_WORD_BYTES 4
#define PTY_SKILL_SLOT_COUNT 24
#define PRF_SKILL_LIST_ENTRY_COUNT 8
#define PRF_SKILL_LIST_FLAGGED 4

#define PTY_PRESET_SKILL_SLOT_COUNT 8
#define PTY_PRESET_POOL_SKILL_COUNT 40
/* Observed gate for the post-mark call; its gameplay meaning is not established. */
#define PTY_PRESET_POOL_EXTRA_GATE 0xBA0
#define PTY_PROFILE_UNIT_LAST_INDEX 4
#define PTY_PROFILE_UNIT_TABLE_OFFSET 0xA60
#define PTY_PROFILE_UNIT_STRIDE 0x1C4
#define PTY_PROFILE_UNIT_ID_COUNT 0x10
#define PTY_PROFILE_UNIT_OCCUPIED_BIT 1
#define PTY_PROFILE_RECORD_TABLE_OFFSET 0x17210
#define PTY_PROFILE_RECORD_TABLE_BYTES 0x5800
#define PTY_PROFILE_RECORD_BANK_BYTES 0x580
#define PRF_PROFILE_RECORD_SHIFT 3
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
#define SDF_FLAG_LIST_MARK_WORDS 2
#define SDF_FLAG_LIST_RESET_MARK 0xFFFFFFFF
#define SDF_FLAG_LIST_ENTRY_VERTEX_BYTES 32
#define SDF_FLAG_LIST_VERTICES_PER_ENTRY 2
#define SDF_FLAG_LIST_PACKET_BYTES 0x20
#define SDF_FLAG_SLOT_TABLE_SHIFT 7
#define SDF_FLAG_SLOT_COUNT 16

extern s32 sdfReleaseResourceAllocation(u32);

extern s32 fileResolvePrimaryBuffer();

extern void effCreateSelectionFlagListFromWork();

extern s32 D_00435E50;

extern s32 datGameState;

extern u32 ptyGetProfileRecordCap(u16);

extern u32 ptyGetProfileRecordPointer(u32, u16);

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

/* Operand block used by the script VM helpers near ptyGetProfileRecord (layout inferred from field accesses). */
typedef struct ScrVmOperand {
    u8 pad_0x00[0x04]; // 0x00
    u16 h04;           // 0x04
    u8 pad_0x06[0x0E]; // 0x06
    u16 level;         // 0x14: compared with profile level requirements
    u8 pad_0x16[0x3A]; // 0x16
    float f50;         // 0x50
    u8 unk54;          // 0x54
    s8 selectedIndex;  // 0x55: active operand chosen by scrSelectOperandIndex
    u8 pad_0x56[2];    // 0x56
    u32 flags[0x4C];   // 0x58: eight 4-bit flag slots per word
} ScrVmOperand;

/* Script flag storage overlaps the other VM operand view near 0x50. */
typedef struct ScriptFlagWork {
    u8 pad00[0x22];
    u16 slotIds[24];       /* 0x22 */
    u8 pad52[3];
    u8 scriptId;           /* 0x55 */
    u8 pad56[2];
    u32 flagWords[0x55];   /* 0x58, three bits per packed flag */
} ScriptFlagWork;

typedef struct ScriptFlagEntry {
    u32 unknown;
    u32 flags;
} ScriptFlagEntry;

extern s32 func_00314990(s32, u16);

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
extern u8 scrSelectScriptEntryAndInitialize(u8 *, u32);
extern void func_00314A80(u8 *, u16);
extern u32 ptyAddProfileRecordValueClamped(u8 *, u32);

extern u16 D_004052E8[][80];

extern u16 D_004052F8[][80];

extern void func_0011CA88(u8 *);

extern u32 D_00401328[][9];

extern u8 func_00314C10(s32);

extern u32 *ptyGetCurrentProfileRecord(s32);

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

typedef union MantraNodeHeader {
    u32 word;
    struct {
        u16 flags;
        u16 id;
    } parts;
} MantraNodeHeader;

typedef struct MantraNodePos {
    MantraNodeHeader header;
    s16 x;
    s16 y;
    struct MantraNodePos *adjacent[6];
} MantraNodePos;

extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);

extern u8 D_00401320[][36];

typedef struct ScriptEntry44 {
    u32 state;
    u8 unknown[40];
} ScriptEntry44;

extern ScriptEntry44 D_00402BE0[];

extern u8 D_0040132C[];

extern u16 D_00401332[];

typedef struct SdfScriptRef {
    u8 pad00[4];
    u16 script;     /* 0x04 */
} SdfScriptRef;

extern s32 func_00359A98(const char *, const char *);

extern void func_00359AC0(s32, const char *, s32, s32);

extern void func_003594A8(s32);

extern char sdfDebugLogAppendMode[];

extern char sdfDebugLogPairFormat[];

/* Party profile header; each party slot occupies 0x1A4 bytes in game state. */
typedef struct PtyProfileUnit {
    u16 flags;          /* 0x00: bit 0 indicates an occupied slot */
    u8 pad02[2];
    u16 unitId;         /* 0x04: profile preset table index */
    u8 pad06[0x1C];
    u16 skills[24];     /* 0x22 */
} PtyProfileUnit;

void func_003140C8(s32 useCurrentProfile, u8 *unit);

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
    memset(datGameState + PTY_PROFILE_RECORD_TABLE_OFFSET, 0, PTY_PROFILE_RECORD_TABLE_BYTES);
}

void func_00313C70(u8 *work) {
    s32 i;
    s32 selected = 0;
    u16 preset;
    u16 scriptId;
    u16 amount;
    PtyPresetScriptChoice *entry;

    preset = ((ScrVmOperand *)work)->h04;
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
void ptyLoadPresetSkillSlots(u8 *unit) {
    u16 *presetSkills = D_004052E8[((ScrVmOperand *)unit)->h04];
    u16 *skillCursor = ((ScriptFlagWork *)unit)->slotIds;
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
    u16 *presetSkills = D_004052F8[((ScrVmOperand *)unit)->h04];
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

INCLUDE_ASM(const s32, "game/code_00313BB8", func_003140C8);

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
                    func_003140C8(0, unit);
                }
            }
        }
        unitOffset += PTY_PROFILE_UNIT_STRIDE;
    }
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314298);

void ptyRecomputeMaxHpMp(u32 unit) {
    func_00314298(unit, 0);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314500);

u32 scrGetIndexedRecordAddress(u32 scriptId, s32 *record) {
    *record = D_00435E50 + (scriptId & 0xffff) * 0x13;
    return 1;
}

/* Read the configured capacity for an unchecked profile ID. */
u32 ptyGetProfileRecordCap(u16 profileId) {
    return D_00401328[profileId][0];
}

void ptySetProfileRecordValue(u32 work, u16 scriptId, u32 value) {
    u32 *record;

    record = (u32 *)ptyGetProfileRecordPointer(work, scriptId);
    *record = value;
}

/* Store the configured capacity in an unchecked profile record. */
void ptySetProfileRecordToCap(u32 unitAddress, u16 profileId) {
    u32 *recordAddress;
    u32 recordCap;

    recordAddress = (u32 *)ptyGetProfileRecordPointer(unitAddress, profileId);
    recordCap = ptyGetProfileRecordCap(profileId);
    *recordAddress = recordCap;
}

/* Add to the selected record and cap the unsigned result; no selection returns zero. */
u32 ptyAddProfileRecordValueClamped(u8 *unit, u32 increment) {
    u32 *record;
    u32 recordCap;
    u8 selectedProfile;
    if (func_00314C10((s32)unit) == 0) return 0;
    record = ptyGetCurrentProfileRecord((s32)unit);
    selectedProfile = ((ScriptFlagWork *)unit)->scriptId;
    *record += increment;
    recordCap = ptyGetProfileRecordCap(selectedProfile);
    if (recordCap < *record) {
        *record = recordCap;
    }
    return *record;
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
u32 sdfSetFlagBySlotId(u8 *unit, u32 slotId) {
    ScriptFlagSlot *slotCursor = (ScriptFlagSlot *)((u8 *)D_00404AA8 + (((ScrVmOperand *)unit)->h04 << SDF_FLAG_SLOT_TABLE_SHIFT));
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
    memset(datGameState + 0x16f10, 0, 0x300);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314868);

s32 func_00314990(s32 work, u16 id) {
    u32 word, shift;
    u32 *flags;
    prfDecodeFlagPair(id, &word, &shift);
    flags = (u32 *)datGameState;
    return (flags[0x16f10 / 4 + ((ScrVmOperand *)work)->h04 * 12 + word] & (1 << shift)) != 0;
}

/* Require the primary profile flag for every row not marked as excluded. */
s32 prfAreAllRequiredProfileFlagsSet(u8 *unit) {
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

s32 func_00314B00(s32 work, u16 id) {
    u32 word, shift;
    u32 *flags;
    prfDecodeFlagPair(id, &word, &shift);
    flags = (u32 *)datGameState;
    return (flags[0x16f10 / 4 + ((ScrVmOperand *)work)->h04 * 12 + word] & (1 << (shift + 1))) != 0;
}

u32 scrGetSelectedScriptEntryId(s32 work) {
    return ((ScriptFlagWork *)work)->scriptId;
}

/* Address an unchecked profile record within the unit's record bank. */
u32 ptyGetProfileRecordPointer(u32 unit, u16 profileId) {
    u32 unitRecordsBase = datGameState + ((ScrVmOperand *)unit)->h04 * PTY_PROFILE_RECORD_BANK_BYTES;

    return unitRecordsBase + (profileId << PRF_PROFILE_RECORD_SHIFT) + PTY_PROFILE_RECORD_TABLE_OFFSET;
}

u32 ptyGetProfileRecordValue(u32 work, u16 scriptId) {
    u32 *record;

    record = (u32 *)ptyGetProfileRecordPointer(work, scriptId);
    return *record;
}

/* Resolve the unit's stored script-entry selection without an extra mask. */
u32 *ptyGetCurrentProfileRecord(s32 unit) {
    return (u32 *)ptyGetProfileRecordPointer(unit, scrGetSelectedScriptEntryId(unit));
}

u8 func_00314C10(s32 work) {
    return ((ScriptFlagWork *)work)->scriptId;
}

/* Store the narrowed selection, initialize its profile state, and return the stored ID. */
u8 scrSelectScriptEntryAndInitialize(u8 *unit, u32 profileId) {
    ((ScriptFlagWork *)unit)->scriptId = profileId;
    func_00314A80(unit, (u16)profileId);
    return ((ScriptFlagWork *)unit)->scriptId;
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

void scrClearPackedScriptFlags(s32 work) {
    memset(((ScriptFlagWork *)work)->flagWords, 0, 0x154);
}

/* Set bit 0 of the selected nibble; the return value is always 1. */
s32 scrSetFlag(u8 *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    ((ScriptFlagWork *)work)->flagWords[wordIndex] |= SCR_FLAG_PRIMARY_BIT << bitShift;
    return 1;
}

void scrRemoveAvailableSkillFlagAndSlot(u8 *work, u16 id) {
    u32 word, shift;
    u32 status = ptyGetSkillNibbleState(work, id);
    if (status == 1) {
        scrDecodePackedFlagIndex((s32)work, id, &word, &shift);
        ((ScriptFlagWork *)work)->flagWords[word] &= ~(status << shift);
        if (scrFindSlot(work, id) >= 0) {
            scrRemoveSlot(work, id);
        }
    }
}

void scrResetAndSetPairedGlobalFlags(void) {
    u32 first;
    s32 last;

    memset(datGameState + 0x16ef0, 0, 0x10);
    first = mnuPickPairedTableValue(0x10, 0);
    last = mnuPickPairedTableValue(0x10, 1);
    for (; (s32)first < last; first = first + 1) {
        func_001B7940(first & 0xffff, 1);
    }
}

/* Set the global bit for an ID in the supported half-open range. */
void scrSetGlobalBitFlag(u32 flagId) {
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
u32 scrTestGlobalBitFlag(u16 bitIndex) {
    if (bitIndex < SCR_GLOBAL_FLAG_FIRST_ID) return 0;
    if (bitIndex >= SCR_GLOBAL_FLAG_END_ID) return 0;
    bitIndex += SCR_GLOBAL_FLAG_WRAP_BIAS;
    return *(u32 *)(datGameState + SCR_GLOBAL_FLAG_TABLE_OFFSET + (bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT) * SCR_GLOBAL_FLAG_WORD_BYTES) & (SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK));
}

/* Set only bit 2 of the selected flag nibble. */
void scrSetSecondaryScriptFlag(u8 *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    ((ScriptFlagWork *)work)->flagWords[wordIndex] |= SCR_FLAG_SECONDARY_BIT << bitShift;
}

/* Clear only bit 2 of the selected flag nibble. */
void scrClearSecondaryScriptFlag(u8 *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    ((ScriptFlagWork *)work)->flagWords[wordIndex] &= ~(SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Clear secondary bits over the fixed ID range, preserving other nibble bits. */
void scrClearAllSecondaryScriptFlags(u8 *work) {
    s32 flagId = 0;
    do {
        scrClearSecondaryScriptFlag(work, (u16)flagId);
        flagId++;
    } while (flagId < SCR_SECONDARY_FLAG_COUNT);
}

/* Return bit 2's raw word mask, not a normalized boolean. */
u32 scrGetSecondaryScriptFlag(u8 *work, u16 flagId) {
    u32 wordIndex, bitShift;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    return ((ScriptFlagWork *)work)->flagWords[wordIndex] & (SCR_FLAG_SECONDARY_BIT << bitShift);
}

/* Bit 1 takes precedence and returns state 2; otherwise return boolean bit 0.
 * The secondary bit does not affect this state. */
u32 ptyGetSkillNibbleState(u8 *work, u16 flagId) {
    u32 wordIndex, bitShift;
    u32 flagWord;
    scrDecodePackedFlagIndex((s32)work, flagId, &wordIndex, &bitShift);
    flagWord = ((ScriptFlagWork *)work)->flagWords[wordIndex];
    if (flagWord & (SCR_SKILL_STATE_TWO_BIT << bitShift)) {
        return 2;
    }
    return (flagWord & (SCR_FLAG_PRIMARY_BIT << bitShift)) != 0;
}

/* Search all slots for the low 16-bit ID; zero can match an empty slot. */
s32 ptyHasSkill(s32 unit, s32 skillId) {
    u32 maskedSkillId = skillId & SCR_FLAG_ID_MASK;
    u16 *skillSlot = ((ScriptFlagWork *)unit)->slotIds;
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
s32 scrFindSlot(u8 *unit, u16 skillId) {
    u32 slotIndex;
    u16 *skillSlots = ((ScriptFlagWork *)unit)->slotIds;
    for (slotIndex = 0; slotIndex < PTY_SKILL_SLOT_COUNT; slotIndex++) {
        if (skillSlots[slotIndex] == skillId) {
            return slotIndex;
        }
    }
    return -1;
}

/* Read a slot's ID, returning zero for an out-of-range unsigned index. */
u16 scrGetSlot(u8 *unit, u32 slotIndex) {
    if (slotIndex >= PTY_SKILL_SLOT_COUNT) {
        return 0;
    }
    return ((ScriptFlagWork *)unit)->slotIds[slotIndex];
}

/* Count nonzero skill IDs across every slot. */
u32 scrCountSlots(u8 *unit) {
    u16 *skillSlots = ((ScriptFlagWork *)unit)->slotIds;
    u32 occupiedCount = 0;
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_SKILL_SLOT_COUNT; slotIndex++) {
        if (skillSlots[slotIndex] != 0) {
            occupiedCount++;
        }
    }
    return occupiedCount;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", scrSetSlot);

/* Clear the first exact-ID match and report whether a slot was found. */
s32 scrRemoveSlot(u8 *unit, u16 skillId) {
    s32 slotIndex = scrFindSlot(unit, skillId);
    if (slotIndex >= 0) {
        ((ScriptFlagWork *)unit)->slotIds[slotIndex] = 0;
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
u32 scrCallIfOperandReady(u8 *operand, s32 byteIndex) {
    s32 selectedProfileId = func_00314C10((s32)operand);

    if (func_00314C10((s32)operand)) {
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

s32 ptyCheckLevelAtLeastProfileParam7b6(u8 *work, u16 scriptId) {
    s32 current = ((ScrVmOperand *)work)->level;
    if (current < (s32)prfGetRequiredProfileLevel(scriptId)) return 0;
    return 1;
}

/* Clear the paired output words; the first two arguments are unused. */
void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, SCR_PAIRED_OUTPUT_BYTES);
}

u8 scrIsSelectedScriptEntryId(s32 work, u16 id) {
    return ((ScriptFlagWork *)work)->scriptId == id;
}

typedef struct PrfSkillList {
    u32 flags[8];
    s32 count;
    u16 skills[8];
} PrfSkillList;

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
s32 prfBuildSkillList(ScriptFlagWork *unit, u32 unusedProfile, PrfSkillList *output, s32 includeFlagged) {
    PrfSkillList compactedList;
    u32 sourceIndex;
    u16 *profileSkillCursor;
    u16 skillId;
    u32 skillState;
    s32 selectedProfileId;

    memset(&compactedList, 0, sizeof(PrfSkillList));
    selectedProfileId = unit->scriptId;
    compactedList.count = 0;
    if (selectedProfileId != 0) {
        profileSkillCursor = &D_00401332[selectedProfileId * PRF_SKILL_TABLE_STRIDE];
        for (sourceIndex = 0; sourceIndex < PRF_SKILL_LIST_ENTRY_COUNT; sourceIndex++) {
            skillId = *profileSkillCursor++;
            if (skillId != 0) {
                skillState = ptyGetSkillNibbleState((u8 *)unit, skillId);
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

/* Return the selected profile's skill count without flagged skills; the explicit ID is ignored. */
s32 prfBuildSkillListState0(unit, unusedProfile, output)
s32 unit;
s32 unusedProfile;
s32 output;
{
    return prfBuildSkillList((ScriptFlagWork *)unit, unusedProfile, (PrfSkillList *)output, 0);
}

/* The +8 threshold is compared with counts or a global counter. */
typedef struct PtyReqCount {
    u8 pad00[8];
    u32 minimumCount; /* 0x08 */
} PtyReqCount;

/* Count leading profile IDs up to the first zero or the eight-entry bound. */
u32 prfCountProfileList(u8 *operand) {
    u8 *profileIds = operand + PRF_REQUIRED_PROFILE_IDS_OFFSET;
    u32 profileIndex = 0;
    while (profileIndex < PRF_REQUIRED_PROFILE_COUNT) {
        if (profileIds[profileIndex] == 0) return profileIndex;
        profileIndex++;
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
            if (func_00314990(unit, profileIds[profileIndex++]) == 0) {
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
            if (func_00314990(unit, profileIds[profileIndex++]) != 0) {
                matchedCount++;
            }
        } while (profileIndex < profileCount);
    }
    if (matchedCount < ((PtyReqCount *)operand)->minimumCount) {
        return 0;
    }
    return 1;
}

/* Count flagged profile rows whose byte-five value reaches the operand's threshold. */
s32 ptyProfileCountAtLeast(u8 *unit, u8 *operand) {
    u32 profileIndex;
    u32 matchedCount = 0;

    for (profileIndex = 0; profileIndex < PRF_PROFILE_COUNT; profileIndex++) {
        if (func_00314990(unit, (u16)profileIndex)) {
            if (D_00401320[profileIndex][5] >= operand[5]) {
                matchedCount++;
            }
        }
    }
    if (matchedCount < ((PtyReqCount *)operand)->minimumCount) {
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
                if ((*(u16 *)partyUnit & PTY_PROFILE_UNIT_OCCUPIED_BIT) != 0) {
                    if (func_00314990((s32)partyUnit, (u16)profileId) != 0) {
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

/* Compare the unit's level byte with the prerequisite's byte threshold. */
s32 prfReqCheckUnitLevel(u8 *unit, u8 *operand) {
    if (((ScrVmOperand *)unit)->level < operand[4]) {
        return 0;
    }
    return 1;
}

/* Test the global counter against the prerequisite's minimum. */
s32 prfReqCheckGlobalCounter(u8 *operand) {
    if (*(u32 *)(datGameState + 0x3C) < ((PtyReqCount *)operand)->minimumCount) {
        return 0;
    }
    return 1;
}

s32 func_00315950(s16 id, s32 context, s32 mode) {
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
                func_00314990(context, (*adjacent)->header.parts.id) == 0 &&
                ((*adjacent)->header.word & 0xF) != 2) {
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
            func_00314990(context, (*adjacent)->header.parts.id) != 0 &&
            ((*adjacent)->header.word & 0x100) == 0) {
            return 1;
        }
        i++;
        adjacent++;
    } while (i < 6);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00315A50);

s32 mnuIsResourceCategoryAvailable(s16 id) {
    MantraNodePos *info = mnuGetMantraNodePositionRecord(id);
    if (info == 0) {
        return 0;
    }
    return D_0045C828[(s32)(info->header.word << 24) >> 28] != 0;
}

s32 func_00315C40(u32 index) {
    if (index >= 17) {
        return 0;
    }
    return D_0045C828[index] != 0;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00315C68);

void func_00315FA0(u32 context, u32 value, u16 id) {
    func_00315C68(context, 0, value, id, 0);
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
s32 func_00316020(void *operand, u16 requirementId) {
    u32 ruleResult;
    u32 groupIndex;
    u32 flagIndex;

    requirementId &= SCR_FLAG_ID_MASK;
    if ((scrGetEntryState(requirementId) & PRF_REQUIREMENT_RULES_BIT) != 0) {
        if (func_00315C68(0, 0, (u32)operand, requirementId, &ruleResult) == 0) {
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
                    if (*(u8 *)(id + datGameState + 0x1340) < minimum) {
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

ScriptEntry44 *scrGetEntryDescriptor(u16 id) {
    return &D_00402BE0[id];
}

void scrSetEntryFlag(u32 context, u16 entryId, u32 bit) {
    ScriptFlagEntry *entry;
    if (bit < 16) {
        entry = (ScriptFlagEntry *)ptyGetProfileRecordPointer(context, entryId);
        entry->flags |= 1 << bit;
    }
}

void scrClearEntryFlag(u32 context, u16 entryId, u32 bit) {
    ScriptFlagEntry *entry;
    if (bit < 16) {
        entry = (ScriptFlagEntry *)ptyGetProfileRecordPointer(context, entryId);
        entry->flags &= ~(1 << bit);
    }
}

s32 scrTestEntryFlag(u32 context, u16 entryId, u32 bit) {
    if (bit >= 16) {
        return 0;
    }
    return (((ScriptFlagEntry *)ptyGetProfileRecordPointer(context, entryId))->flags & (1 << bit)) != 0;
}

void scrSetEntryLowFlags(u32 context, u16 entryId, u16 lowFlags) {
    ScriptFlagEntry *entry = (ScriptFlagEntry *)ptyGetProfileRecordPointer(context, entryId);
    entry->flags = (entry->flags & 0xFFFF0000) | lowFlags;
}

u16 scrGetEntryLowFlags(u32 context, u16 entryId) {
    return *(u16 *)&((ScriptFlagEntry *)ptyGetProfileRecordPointer(context, entryId))->flags;
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

/* Each list entry occupies two words; only its first word is reset here. */
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
    u32 count;         /* 0x4C */
    u8 pad50[4];
    u32 resource;
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

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316528);

void sdfInitializeFlagListFromResource(void) {
    effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer());
}

void sdfReleaseFlagListResource(s32 work) {
    sdfReleaseResourceAllocation(((SdfFlagListWork *)work)->resource);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316680);

typedef struct GsSurface {
    u8 pad00[0x10];
    void (*submit)(struct GsSurface *, void *);
} GsSurface;

extern GsSurface *D_0040A958[];
extern f32 sdfViewTargetVector[4];
extern u32 D_00438918;
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

/* Read the operand's stored float; its gameplay meaning remains unknown. */
float scrGetOperandFloatValue(ScrVmOperand *operand) {
    return operand->f50;
}

/* Replace the operand's stored float without validation. */
void scrSetOperandFloatValue(ScrVmOperand *operand, float value) {
    operand->f50 = value;
}

void func_00316DE0(s32 work) {
    func_002D7458(work + 0x14, work + 0x38, 0, 0);
}

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogAppendMode);

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogPairFormat);

INCLUDE_SDATA(const s32, "game/code_00313BB8", D_00438918);

