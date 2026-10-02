#include "common.h"
extern u16 ptyPresetSkillSlots[][96];
extern u16 ptyPresetPoolSkills[][96];
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

extern u8 D_00394680[];

/* 24-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry24B {
    u8 v0;            // 0x00
    u8 pad_0x01[0x17]; // 0x01
} Entry24B; // 0x18

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

extern void ptySetProfileFlag1(void *, s32);

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
extern u32 func_00197C40(s32, s32, u32, u16, u32, u32);
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

void sdfAppendFormattedDebugLogPair(s32 left, s32 right) {
    s32 file = func_002FE950("debug.log", sdfDebugLogAppendMode);
    if (file != 0) {
        func_002FE978(file, sdfDebugLogPairFormat, left, right);
        func_002FE360(file);
    }
}

void ptyClearProfileRecords(void) {
    memset(datGameState + 0x2ebb0, 0, 0x3000);
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptySelectProfileStage);

INCLUDE_ASM(const s32, "game/code_002CC750", ptyApplyProfilePreset);

void ptyLoadPresetSkillSlots(u8 *work) {
    u16 *source = ptyPresetSkillSlots[((PtyProfileUnit *)work)->unitId];
    u16 *slots = ((PtyProfileUnit *)work)->skills;
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
    u16 *source = ptyPresetPoolSkills[((PtyProfileUnit *)work)->unitId];
    u32 index = 0;
    do {
        u16 id = *source++;
        if (id != 0) {
            scrSetFlag(work, id);
        }
        index++;
    } while (index < 40);
    if (mdlFlagTest(0xB90)) {
        ptyMergeStockSkills(work);
    }
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyRebuildProfileSkills);

void ptyRebuildProfileSkills(s32 useCurrentProfile, u8 *unit);

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
                    ptyRebuildProfileSkills(0, unit);
                }
            }
        }
        offset += 0x1A4;
    }
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyRecomputeMaxVitals);

void ptyRecomputeMaxHpMp(u32 unit) {
    ptyRecomputeMaxVitals(unit, 0);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD0D8);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD240);

u32 prfGetCapValue(u16 i) {
    return D_003907B8[i].v0;
}

/* Clamp one profile record to its configured capacity. */
void ptySetProfileRecordToCap(u32 unit, u16 profileId) {
    u32 *record;
    u32 cap;

    record = (u32 *)ptyGetProfileRecord(unit, profileId);
    cap = prfGetCapValue(profileId);
    *record = cap;
}

u32 ptyAddProfilePoints(ScrVmOperand *work, s32 increment) {
    u32 *position;
    u32 limit;
    u32 result;
    s32 selectedIndex;
    if (ptyGetCurrentProfileId(work) == 0) {
        return 0;
    }
    position = (u32 *)ptyGetCurrentProfileRecord(work);
    selectedIndex = work->selectedIndex;
    *position += increment;
    limit = prfGetCapValue(selectedIndex & 0xffff);
    result = *position;
    if (limit < result) {
        *position = limit;
        result = limit;
    }
    return result;
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

u32 sdfSetFlagBySlotId(u8 *work, u32 id) {
    ScriptFlagSlot *slot = (ScriptFlagSlot *)((u8 *)D_00393280 + (*(u16 *)(work + 4) << 7));
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

INCLUDE_ASM(const s32, "game/code_002CC750", ptyApplyProfile);

s32 ptyTestProfileFlag0(s32 work, u16 id) {
    u32 word;
    u32 shift;
    u32 *flags;

    prfDecodeFlagPair(id, &word, &shift);
    flags = (u32 *)datGameState;
    return (flags[0x2e9f0 / 4 + word + ((ScrVmOperand *)work)->h04 * 7] & (1 << shift)) != 0;
}

s32 scrCheckStateBits(ScrVmOperand *work) {
    u32 index = 0;
    do {
        u16 id = index;
        index++;
        if ((prfIsRequirementExcluded(id) & 1) == 0 &&
            !ptyTestProfileFlag0(work, id)) {
            return 0;
        }
    } while (index < 0x60);
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

u32 ptyGetProfileRecord(u32 work, u16 index) {
    u32 record = datGameState + *(u16 *)(work + 4) * 0x300;

    return record + index * 8 + 0x2EBB0;
}

u32 ptyGetProfileRecordValue(u32 unit, u16 profileId) {
    u32 *record;

    record = (u32 *)ptyGetProfileRecord(unit, profileId);
    return *record;
}

u32 ptyGetCurrentProfileRecord(ScrVmOperand *p) {
    return ptyGetProfileRecord((u32)p, scrGetSelectedOperandIndex(p) & 0xFFFF);
}

s8 ptyGetCurrentProfileId(ScrVmOperand *op) {
    return op->selectedIndex;
}

s8 scrSelectOperandIndex(ScrVmOperand *p, s32 v) {
    p->selectedIndex = v;
    ptySetProfileFlag1(p, v & 0xFFFF);
    return p->selectedIndex;
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

void scrDecodePackedFlagIndex(s32 unused, u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 7;
    v >>= 3;
    *a = v;
    *b = lo << 2;
}

s32 scrSetFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    work->flags[word] |= 1U << shift;
    return 1;
}

void scrSetGlobalSeenBit(u32 id) {
    u16 bit;
    u32 *word;
    s32 offset;
    id &= 0xffff;
    if (id < 0x1ab) return;
    if (id >= 0x200) return;
    bit = id + 0xfe55;
    offset = 0x2e9d0 + (bit >> 5) * 4;
    word = (u32 *)(datGameState + offset);
    *word |= 1U << (bit & 31);
}

u32 scrTestGlobalSeenBit(u32 arg) {
    u16 id = arg;
    u32 *word;
    s32 offset;
    if (id < 0x1ab) return 0;
    if (id >= 0x200) return 0;
    id = id + 0xfe55;
    offset = 0x2e9d0 + (id >> 5) * 4;
    word = (u32 *)(datGameState + offset);
    return *word & (1U << (id & 31));
}

void scrSetSecondaryScriptFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    work->flags[word] |= 4U << shift;
}

void scrClearSecondaryScriptFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    work->flags[word] &= ~(4U << shift);
}

void scrClearFlags(ScrVmOperand *work) {
    s32 index;
    for (index = 0; index < 0x260; index++) {
        scrClearSecondaryScriptFlag(work, index);
    }
}

u32 scrGetSecondaryScriptFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    return work->flags[word] & (4U << shift);
}

u32 ptyGetSkillNibbleState(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    u32 mask;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    mask = work->flags[word];
    if (mask & (2U << shift)) {
        return 2;
    }
    return (mask & (1U << shift)) != 0;
}

s32 ptyHasSkill(s32 unit, s32 skillId) {
    u32 key = skillId & 0xFFFF;
    u16 *skills = ((PtyProfileUnit *)unit)->skills;
    u32 i = 0;

    do {
        if (*skills++ == key) {
            return 1;
        }
        i++;
    } while (i < 0x18);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", ptyRemoveProfileSkills);

s32 scrFindSlot(u8 *work, u16 key) {
    u32 index;
    u16 *entries = ((PtyProfileUnit *)work)->skills;
    for (index = 0; index < 24; index++) {
        if (entries[index] == key) {
            return index;
        }
    }
    return -1;
}

u16 scrGetSlot(PtyProfileUnit *unit, u32 index) {
    if (index >= 24) {
        return 0;
    }
    return unit->skills[index];
}

u32 scrCountSlots(PtyProfileUnit *unit) {
    u32 count = 0;
    u32 index;

    for (index = 0; index < 24; index++) {
        if (unit->skills[index] != 0) {
            count++;
        }
    }
    return count;
}

/* Replace a skill slot and return its previous identifier. */
u16 scrSetSlot(PtyProfileUnit *unit, s32 index, u16 skill) {
    u16 previous = unit->skills[index];

    unit->skills[index] = skill;
    return previous;
}

s32 scrRemoveSlot(u8 *work, u16 key) {
    s32 index = scrFindSlot(work, key);
    if (index >= 0) {
        ((PtyProfileUnit *)work)->skills[index] = 0;
        return 1;
    }
    return 0;
}

u8 prfGetParamWord7b4(u16 i) {
    return D_003907B4[i].v0;
}

s32 prfGetParamWord7b6(u16 i) {
    return D_003907B6[i].v0;
}

u8 prfGetParamWord7b5(u16 i) {
    return D_003907B5[i].v0;
}

u32 func_002CDDB0() {
    return 0;
}

u32 scrCallIfOperandReady(ScrVmOperand *operand, u32 value) {
    s8 selected = ptyGetCurrentProfileId(operand);

    if (ptyGetCurrentProfileId(operand)) {
        return func_002CDDB0((u16)selected, value);
    }
}

u16 prfGetSkillAtIndex(u16 scriptId, u32 entry) {
    if (entry >= 8) {
        return 0;
    }
    return D_003907BC[scriptId * 14 + entry];
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

void scrClearPairedEntryOutput(u32 arg0, u32 arg1, u32 arg2) {
    memset(arg2, 0, 8);
}

u8 ptyIsCurrentProfileId(s32 operand, u32 profileId) {
    return (s64)((ScrVmOperand *)operand)->selectedIndex == (profileId & 0xffff);
}

INCLUDE_ASM(const s32, "game/code_002CC750", prfBuildRawSkillList);

INCLUDE_ASM(const s32, "game/code_002CC750", prfBuildSkillList);

u32 prfBuildSkillListState0(u32 unit, u32 profile, u32 output) {
    return prfBuildSkillList(unit, profile, output, 0);
}

u32 prfCountProfileList(u8 *work) {
    u8 *slots = work + 0xc;
    u32 index;
    for (index = 0; index < 8; index++) {
        if (slots[index] == 0) {
            return index;
        }
    }
    return 8;
}

s32 ptyHasAllReqProfiles(s32 state, u8 *operand) {
    s32 index = 0;
    s32 count = prfCountProfileList(operand);
    if (count > 0) {
        u8 *slots = operand + 0xc;
        do {
            if (ptyTestProfileFlag0(state, slots[index++]) == 0) {
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
            if (ptyTestProfileFlag0(state, slots[index++]) != 0) {
                matched++;
            }
        } while (index < count);
    }
    if (matched < ((PrfRequirementOperand *)operand)->requiredValue) {
        return 0;
    }
    return 1;
}

typedef struct PtyReqEntry {
    u8 unk0[5];
    u8 value5;
    u8 pad6[0x16];
} PtyReqEntry;

s32 ptyProfileCountAtLeast(u8 *work, u8 *req) {
    PtyReqEntry *entry = (PtyReqEntry *)D_003907B0;
    u32 index;
    u32 count = 0;

    for (index = 0; index < 0x60; index++) {
        if (ptyTestProfileFlag0((s32)work, (u16)index)) {
            if (entry[index].value5 >= req[5]) {
                count++;
            }
        }
    }
    if (count < ((PrfRequirementOperand *)req)->requiredValue) {
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
                offset += 0x1a4;
                if ((((PtyProfileUnit *)entry)->flags & 1) != 0) {
                    if (ptyTestProfileFlag0((s32)entry, (u16)selected) != 0) {
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

u32 prfReqCheckUnitLevel(ScrVmOperand *operand, u8 *value) {
    if (operand->h14 < value[4]) {
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

u32 prfIsRequirementExcluded(u16 i) {
    return D_003907B0[i].v0;
}

u32 prfReq54GetWord1230(u16 i) {
    return D_00391230[i].v0;
}

typedef struct PrfFallbackGroup {
    u8 id;
    u8 pad01[3];
    u32 flags[2];
} PrfFallbackGroup;

extern PrfFallbackGroup D_003931B0[];
s32 prfReqCheckWithFallback(void *operand, u16 id) {

    u32 result;
    u32 group;
    u32 i;

    id &= 0xFFFF;
    if ((prfReq54GetWord1230(id) & 4) != 0) {
        if (prfReqEvaluateRules(0, (u32)operand, id, &result) == 0) {
            return result != 0;
        }
    }
    for (group = 0; group < 4; group++) {
        if (D_003931B0[group].id == id) {
            for (i = 0; i < 2; i++) {
                u32 flag = D_003931B0[group].flags[i];

                if (flag != 0 && mdlFlagTest(flag) == 0) {
                    return 0;
                }
            }
            return 1;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002CC750", prfReqSelectGroup);
u8 prfReq18GetWord3220(s32 i) {
    return D_00393220[i].v0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", prfReqGetPair);
u32 prfReq18GetWord3234(s32 i) {
    return D_00393234[i].v0;
}

void sdfSetAllFlagsFromTable(void) {
    s32 index = 0;
    do {
        index = prfReqSelectGroup(index);
        if (index >= 0) {
            u32 name = prfReq18GetWord3234(index);
            if (name != 0) {
                mdlFlagSet(name);
            }
        }
    } while (index++ >= 0);
}

Entry84W *prfReqGetEntryRecord(u16 index) {
    return &D_00391230[index];
}

void frFontQueueColoredGlyph(s32 x, s32 y, u32 first, u16 width, u32 second, s32 option) {
    u32 handle = func_00197C40(x, y, first, width, (u32)D_00394680, 0);
    frFontSetChildColors(handle, second);
    func_001958A0(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
}

u8 *frFontGetColoredGlyphResource(void) {
    return D_00394680;
}

/* Interleaved mark words and an eight-byte-per-entry value block. */
typedef struct SdfFlagListWork {
    u8 pad00[8];
    u32 *marks;               /* 0x08: first word of each pair */
    u8 pad0C[4];
    u32 *values;              /* 0x10 */
    u8 pad14[0x38];
    u32 count;                /* 0x4C */
    u8 pad50[4];
    u32 resource;             /* 0x54 */
} SdfFlagListWork;

void sdfResetFlagListEntries(s32 context) {
    u32 count;
    u32 *mark;
    u32 index;

    index = 0;
    count = ((SdfFlagListWork *)context)->count;
    mark = ((SdfFlagListWork *)context)->marks;
    if (count != 0) {
        do {
            index = index + 1;
            *mark = 0xffffffff;
            mark = mark + 2;
        } while (index < count);
    }
    memset(((SdfFlagListWork *)context)->values, 0, count << 3);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CEAE8);

void func_002CEC08(void) {
    effCreateSelectionFlagListFromWork(fileResolvePrimaryBuffer());
}

void sdfReleaseFlagListResource(s32 context) {
    func_002D0918(((SdfFlagListWork *)context)->resource);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CEC40);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CF248);

float scrGetOperandFloatValue(ScrVmOperand *op) {
    return op->f50;
}

void scrSetOperandFloatValue(ScrVmOperand *op, float value) {
    op->f50 = value;
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

