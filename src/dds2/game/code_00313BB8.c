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
#define PTY_SKILL_SLOT_COUNT 24
#define PRF_SKILL_LIST_ENTRY_COUNT 8
#define PRF_SKILL_LIST_FLAGGED 4

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

void ptyClearProfileRecords(void) {
    memset(datGameState + 0x17210, 0, 0x5800);
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

void ptyLoadPresetSkillSlots(u8 *work) {
    u16 *source = D_004052E8[((ScrVmOperand *)work)->h04];
    u16 *slots = ((ScriptFlagWork *)work)->slotIds;
    u32 index;
    index = 0;
    do {
        u16 id = *source++;
        if (id != 0) {
            scrSetFlag(work, id);
            *slots = id;
        }
        slots++;
        index++;
    } while (index < 8);
}

void ptyMarkPresetSkillPool(u8 *work) {
    u16 *source = D_004052F8[((ScrVmOperand *)work)->h04];
    u32 index = 0;
    do {
        u16 id = *source++;
        if (id != 0) {
            scrSetFlag(work, id);
        }
        index++;
    } while (index < 40);
    if (mdlFlagTest(0xBA0)) {
        func_0011CA88(work);
    }
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_003140C8);

/* Rebuild skill lists for the five occupied party slots. */
void ptyRebuildAllProfiles(void) {
    s32 i;
    s32 offset;

    for (offset = 0, i = 4; i >= 0; i--) {
        u8 *unit = (u8 *)datGameState + 0xA60 + offset;

        if ((((PtyProfileUnit *)unit)->flags & 1) != 0) {
            s32 j;

            for (j = 0; j < 0x10; j++) {
                if (((PtyProfileUnit *)unit)->unitId == j) {
                    func_003140C8(0, unit);
                }
            }
        }
        offset += 0x1C4;
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

u32 ptyGetProfileRecordCap(u16 scriptId) {
    return D_00401328[scriptId][0];
}

void ptySetProfileRecordValue(u32 work, u16 scriptId, u32 value) {
    u32 *record;

    record = (u32 *)ptyGetProfileRecordPointer(work, scriptId);
    *record = value;
}

void ptySetProfileRecordToCap(u32 work, u16 scriptId) {
    u32 *record;
    u32 cap;

    record = (u32 *)ptyGetProfileRecordPointer(work, scriptId);
    cap = ptyGetProfileRecordCap(scriptId);
    *record = cap;
}

u32 ptyAddProfileRecordValueClamped(u8 *work, u32 amount) {
    u32 *total;
    u32 limit;
    u8 scriptId;
    if (func_00314C10((s32)work) == 0) return 0;
    total = ptyGetCurrentProfileRecord((s32)work);
    scriptId = ((ScriptFlagWork *)work)->scriptId;
    *total += amount;
    limit = ptyGetProfileRecordCap(scriptId);
    if (limit < *total) {
        *total = limit;
    }
    return *total;
}

void prfDecodeFlagPair(u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 0xF;
    v >>= 4;
    *a = v;
    *b = lo << 1;
}

u32 sdfSetFlagBySlotId(u8 *work, u32 id) {
    ScriptFlagSlot *slot = (ScriptFlagSlot *)((u8 *)D_00404AA8 + (((ScrVmOperand *)work)->h04 << 7));
    u32 i;

    for (i = 0; i < 16; i++, slot++) {
        if (slot->id != 0 && slot->id == id) {
            u32 flag = slot->flag;
            mdlFlagSet(flag);
            return flag;
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

s32 prfAreAllRequiredProfileFlagsSet(u8 *work) {
    u32 index;
    for (index = 0; index < 0xB0; index++) {
        u16 id = index;
        if (!(scrGetEntryRequirementFlags(id) & 1) && !func_00314990(work, id)) {
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

u32 ptyGetProfileRecordPointer(u32 ref, u16 index) {
    u32 entry = datGameState + ((ScrVmOperand *)ref)->h04 * 0x580;

    return entry + (index << 3) + 0x17210;
}

u32 ptyGetProfileRecordValue(u32 work, u16 scriptId) {
    u32 *record;

    record = (u32 *)ptyGetProfileRecordPointer(work, scriptId);
    return *record;
}

u32 *ptyGetCurrentProfileRecord(s32 work) {
    return (u32 *)ptyGetProfileRecordPointer(work, scrGetSelectedScriptEntryId(work));
}

u8 func_00314C10(s32 work) {
    return ((ScriptFlagWork *)work)->scriptId;
}

u8 scrSelectScriptEntryAndInitialize(u8 *context, u32 entryId) {
    ((ScriptFlagWork *)context)->scriptId = entryId;
    func_00314A80(context, (u16)entryId);
    return ((ScriptFlagWork *)context)->scriptId;
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
    byteOffset = 0x16ef0 + (bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT) * 4;
    flagWord = (u32 *)(datGameState + byteOffset);
    *flagWord |= SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK);
}

/* Return the global bit's raw word mask, not a normalized boolean. */
u32 scrTestGlobalBitFlag(u16 bitIndex) {
    if (bitIndex < SCR_GLOBAL_FLAG_FIRST_ID) return 0;
    if (bitIndex >= SCR_GLOBAL_FLAG_END_ID) return 0;
    bitIndex += SCR_GLOBAL_FLAG_WRAP_BIAS;
    return *(u32 *)(datGameState + 0x16ef0 + (bitIndex >> SCR_GLOBAL_FLAG_WORD_SHIFT) * 4) & (SCR_FLAG_PRIMARY_BIT << (bitIndex & SCR_GLOBAL_FLAG_BIT_MASK));
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

/* Replace an unchecked skill-slot index and return its previous identifier. */
u16 scrSetSlot(s32 unit, s32 slotIndex, u16 skillId) {
    u16 previousSkillId;
    u16 *skillSlot;

    /* Required to match: offset-first address calculation for slotIds[slotIndex]. */
    skillSlot = (u16 *)(slotIndex * 2 + unit + 0x22);
    previousSkillId = *skillSlot;
    *skillSlot = skillId;
    return previousSkillId;
}

/* Clear the first exact-ID match and report whether a slot was found. */
s32 scrRemoveSlot(u8 *unit, u16 skillId) {
    s32 slotIndex = scrFindSlot(unit, skillId);
    if (slotIndex >= 0) {
        ((ScriptFlagWork *)unit)->slotIds[slotIndex] = 0;
        return 1;
    }
    return 0;
}

u8 func_003151D0(u16 scriptId) {
    return D_00401324[scriptId * 36];
}

u16 prfGetRequiredProfileLevel(u16 scriptId) {
    return *(u16 *)((u8 *)D_00401326 + scriptId * 36);
}

u8 func_00315220(u16 scriptId) {
    return D_00401325[scriptId * 36];
}

u32 prfGetIndexedProfileByte(u16 id, s32 sub) {
    return D_0040132C[sub + id * 36];
}

u32 scrCallIfOperandReady(u8 *operand, s32 value) {
    s32 selected = func_00314C10((s32)operand);

    if (func_00314C10((s32)operand)) {
        return prfGetIndexedProfileByte((u16)selected, value);
    }
}

u16 prfGetSkillAtIndex(u16 scriptId, u32 entry) {
    if (entry >= 8) {
        return 0;
    }
    return D_00401332[scriptId * 18 + entry];
}

u32 func_00315318(void) {
    return 0;
}

s32 ptyCheckLevelAtLeastProfileParam7b6(u8 *work, u16 scriptId) {
    s32 current = ((ScrVmOperand *)work)->level;
    if (current < (s32)prfGetRequiredProfileLevel(scriptId)) return 0;
    return 1;
}

void scrClearPairedEntryOutput(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, 8);
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
s32 prfBuildSkillList(ScriptFlagWork *unit, u32 profile, PrfSkillList *output, s32 includeFlagged) {
    PrfSkillList list;
    u32 entryIndex;
    u16 *profileSkills;
    u16 skillId;
    u32 skillState;
    s32 selectedProfile;

    memset(&list, 0, sizeof(PrfSkillList));
    selectedProfile = unit->scriptId;
    list.count = 0;
    if (selectedProfile != 0) {
        profileSkills = &D_00401332[selectedProfile * 18];
        for (entryIndex = 0; entryIndex < PRF_SKILL_LIST_ENTRY_COUNT; entryIndex++) {
            skillId = *profileSkills++;
            if (skillId != 0) {
                skillState = ptyGetSkillNibbleState((u8 *)unit, skillId);
                if (skillState != 0 && includeFlagged == 0) {
                    continue;
                }
                list.flags[list.count] = 0;
                if (skillState != 0) {
                    list.flags[list.count] |= PRF_SKILL_LIST_FLAGGED;
                }
                list.skills[list.count] = skillId;
                list.count++;
            }
        }
    }
    if (output != NULL) {
        *output = list;
    }
    return list.count;
}

void prfBuildSkillListState0(a, b, c)
s32 a;
s32 b;
s32 c;
{
    prfBuildSkillList((ScriptFlagWork *)a, b, (PrfSkillList *)c, 0);
}

/* The +8 threshold is compared with counts or a global counter. */
typedef struct PtyReqCount {
    u8 pad00[8];
    u32 minimumCount; /* 0x08 */
} PtyReqCount;

u32 prfCountProfileList(u8 *work) {
    u8 *flags = work + 0xC;
    u32 index = 0;
    while (index < 8) {
        if (flags[index] == 0) return index;
        index++;
    }
    return 8;
}

s32 ptyHasAllReqProfiles(s32 state, u8 *operand) {
    s32 index = 0;
    s32 count = prfCountProfileList(operand);
    if (count > 0) {
        u8 *slots = operand + 0xc;
        do {
            if (func_00314990(state, slots[index++]) == 0) {
                return 0;
            }
        } while (index < count);
    }
    return 1;
}

s32 ptyReqProfileCountAtLeast(s32 state, u8 *operand) {
    u32 matched = 0;
    s32 index = 0;
    s32 count = prfCountProfileList(operand);
    if (count > 0) {
        u8 *slots = operand + 0xc;
        do {
            if (func_00314990(state, slots[index++]) != 0) {
                matched++;
            }
        } while (index < count);
    }
    if (matched < ((PtyReqCount *)operand)->minimumCount) {
        return 0;
    }
    return 1;
}

s32 ptyProfileCountAtLeast(u8 *work, u8 *req) {
    u32 index;
    u32 count = 0;

    for (index = 0; index < 0xB0; index++) {
        if (func_00314990(work, (u16)index)) {
            if (D_00401320[index][5] >= req[5]) {
                count++;
            }
        }
    }
    if (count < ((PtyReqCount *)req)->minimumCount) {
        return 0;
    }
    return 1;
}

s32 ptyAreReqProfilesInParty(u8 *operand) {
    s32 index = 0;
    s32 count = prfCountProfileList(operand);
    if (count > 0) {
        u8 *slots = operand + 0xc;
        do {
            s32 present = 0;
            s32 selected = slots[index];
            s32 offset = 0;
            s32 remaining = 4;
            do {
                u8 *entry = (u8 *)datGameState + 0xa60 + offset;
                offset += 0x1c4;
                if ((*(u16 *)entry & 1) != 0) {
                    if (func_00314990((s32)entry, (u16)selected) != 0) {
                        present = 1;
                    }
                }
                remaining--;
            } while (remaining >= 0);
            if (present == 0) {
                return 0;
            }
            index++;
        } while (index < count);
    }
    return 1;
}

s32 prfReqCheckUnitLevel(u8 *a, u8 *b) {
    if (((ScrVmOperand *)a)->level < b[4]) {
        return 0;
    }
    return 1;
}

s32 prfReqCheckGlobalCounter(u8 *a) {
    if (*(u32 *)(datGameState + 0x3C) < ((PtyReqCount *)a)->minimumCount) {
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

u32 scrGetEntryRequirementFlags(u16 index) {
    return *(u32 *)D_00401320[index];
}

u32 scrGetEntryState(u16 index) {
    return D_00402BE0[index].state;
}

typedef struct RequirementFallbackEntry {
    u8 id;
    u8 pad01[3];
    u32 flags[2];
} RequirementFallbackEntry;

extern RequirementFallbackEntry D_00404A20[];

s32 func_00316020(void *operand, u16 id) {
    u32 result;
    u32 group;
    u32 i;

    id &= 0xFFFF;
    if ((scrGetEntryState(id) & 4) != 0) {
        if (func_00315C68(0, 0, (u32)operand, id, &result) == 0) {
            return result != 0;
        }
    }
    for (group = 0; group < 4; group++) {
        if (D_00404A20[group].id == id) {
            for (i = 0; i < 2; i++) {
                u32 flag = D_00404A20[group].flags[i];

                if (flag != 0 && mdlFlagTest(flag) == 0) {
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

u8 prfReq18GetWord3220(s32 i) {
    return D_00404A90[i].active;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", prfReqGetPair);

u32 prfReq18GetWord3234(s32 i) {
    return D_00404AA4[i].v0;
}

void sdfSetAllFlagsFromTable(void) {
    s32 index = 0;

    do {
        index = sdfFindUnmetRequirementIndex(index);
        if (index >= 0) {
            s32 flag = prfReq18GetWord3234(index);
            if (flag != 0) {
                mdlFlagSet(flag);
            }
        }
    } while (index++ >= 0);
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

void sdfResetFlagListEntries(s32 list) {
    u32 entryCount;
    u32 *entry;
    u32 index;

    index = 0;
    entryCount = ((SdfFlagListWork *)list)->count;
    entry = ((SdfFlagListWork *)list)->marks;
    if (entryCount != 0) {
        do {
            index = index + 1;
            *entry = 0xffffffff;
            entry = entry + 2;
        } while (index < entryCount);
    }
    memset(((SdfFlagListWork *)list)->colors, 0, entryCount << 3);
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

void func_00316C88(SdfFlagListWork *work) {
    f32 (*source)[4];
    f32 (*vertices)[4];
    u32 count;
    u32 i;
    s32 allocation;
    void *packet;
    GsSurface *surface;

    if (work->maxFrames != 0 && work->frame >= work->maxFrames) {
        return;
    }
    count = work->count;
    source = work->vertices;
    allocation = sdfAllocGeneralBlock(count * 32);
    count *= 2;
    vertices = sdfMemoryGetBlockAddress(allocation);
    for (i = 0; i < count; i++) {
        VU0_LOAD_VF(vf10, &source[i]);
        if (D_00438918 != 0) {
            VU0_LOAD_VF(vf11, sdfViewTargetVector);
            VU0_ADD(vf10, vf10, vf11);
        }
        VU0_STORE_VF_UNCLOBBERED(vf10, &vertices[i]);
    }
    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    sdfAppendPacket(packet, func_00348158(vertices, work->colors, work->count * 2, 0x40));
    surface = D_0040A958[work->surfaceIndex];
    surface->submit(surface, packet);
    sdfReleaseResourceAllocation(allocation);
}

float scrGetOperandFloatValue(ScrVmOperand *op) {
    return op->f50;
}

void scrSetOperandFloatValue(ScrVmOperand *op, float value) {
    op->f50 = value;
}

void func_00316DE0(s32 work) {
    func_002D7458(work + 0x14, work + 0x38, 0, 0);
}

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogAppendMode);

INCLUDE_SDATA(const s32, "game/code_00313BB8", sdfDebugLogPairFormat);

INCLUDE_SDATA(const s32, "game/code_00313BB8", D_00438918);

