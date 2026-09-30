#include "common.h"

#include "fpu.h"

extern s32 func_003297C8(u32);

extern void func_003154A0();

extern s32 fileResolvePrimaryBuffer();

extern void func_002DEB80();

extern s32 D_00435E50;

extern s32 D_00435DD0;

extern u32 func_00314690(u16);

extern u32 func_00314B80(u32, u16);

/* 24-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry24B {
    u8 v0;            // 0x00
    u8 pad_0x01[0x17]; // 0x01
} Entry24B; // 0x18

extern Entry24B D_00404A90[];

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18

extern Entry24W D_00404AA4[];

extern u8 D_00405CA8[];

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

extern u32 func_00315FC8(u16);

extern u8 D_00401324[];

extern u8 D_00401325[];

extern u16 D_00401326[];

extern u8 D_0045C828[];

extern u32 *func_0026CF70(s16);

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

extern char D_00438908[];

extern char D_00438910[];

/* Party profile header; each party slot occupies 0x1A4 bytes in game state. */
typedef struct PtyProfileUnit {
    u16 flags;          /* 0x00: bit 0 indicates an occupied slot */
    u8 pad02[2];
    u16 unitId;         /* 0x04: profile preset table index */
    u8 pad06[0x1C];
    u16 skills[24];     /* 0x22 */
} PtyProfileUnit;

void func_003140C8(s32 useCurrentProfile, u8 *unit);

extern u32 func_0019FC38(s32, s32, u32, u16, u32, u32);

extern void frFontSetChildColors(u32, u32);

extern void func_0019D550(u32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(u32);

void func_00313BB8(s32 left, s32 right) {
    s32 file = func_00359A98("debug.log", D_00438908);
    if (file != 0) {
        func_00359AC0(file, D_00438910, left, right);
        func_003594A8(file);
    }
}

void ptyClearProfileRecords(void) {
    memset(D_00435DD0 + 0x17210, 0, 0x5800);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00313C70);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00313D80);

void func_00313F88(u8 *work) {
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

void func_00314020(u8 *work) {
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
        u8 *unit = (u8 *)D_00435DD0 + 0xA60 + offset;

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

u32 func_00314668(u32 scriptId, s32 *record) {
    *record = D_00435E50 + (scriptId & 0xffff) * 0x13;
    return 1;
}

u32 func_00314690(u16 scriptId) {
    return D_00401328[scriptId][0];
}

void func_003146B8(u32 work, u16 scriptId, u32 value) {
    u32 *record;

    record = (u32 *)func_00314B80(work, scriptId);
    *record = value;
}

void ptySetProfileRecordToCap(u32 work, u16 scriptId) {
    u32 *record;
    u32 cap;

    record = (u32 *)func_00314B80(work, scriptId);
    cap = func_00314690(scriptId);
    *record = cap;
}

u32 func_00314728(u8 *work, u32 amount) {
    u32 *total;
    u32 limit;
    u8 scriptId;
    if (func_00314C10((s32)work) == 0) return 0;
    total = ptyGetCurrentProfileRecord((s32)work);
    scriptId = ((ScriptFlagWork *)work)->scriptId;
    *total += amount;
    limit = func_00314690(scriptId);
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

void func_00314838(void) {
    memset(D_00435DD0 + 0x16f10, 0, 0x300);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314868);

s32 func_00314990(s32 work, u16 id) {
    u32 word, shift;
    u32 *flags;
    prfDecodeFlagPair(id, &word, &shift);
    flags = (u32 *)D_00435DD0;
    return (flags[0x16f10 / 4 + ((ScrVmOperand *)work)->h04 * 12 + word] & (1 << shift)) != 0;
}

s32 func_00314A08(u8 *work) {
    u32 index;
    for (index = 0; index < 0xB0; index++) {
        u16 id = index;
        if (!(func_00315FC8(id) & 1) && !func_00314990(work, id)) {
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
    flags = (u32 *)D_00435DD0;
    return (flags[0x16f10 / 4 + ((ScrVmOperand *)work)->h04 * 12 + word] & (1 << (shift + 1))) != 0;
}

u32 func_00314B78(s32 work) {
    return ((ScriptFlagWork *)work)->scriptId;
}

u32 func_00314B80(u32 ref, u16 index) {
    u32 entry = D_00435DD0 + ((ScrVmOperand *)ref)->h04 * 0x580;

    return entry + (index << 3) + 0x17210;
}

u32 ptyGetProfileRecordValue(u32 work, u16 scriptId) {
    u32 *record;

    record = (u32 *)func_00314B80(work, scriptId);
    return *record;
}

u32 *ptyGetCurrentProfileRecord(s32 work) {
    return (u32 *)func_00314B80(work, func_00314B78(work));
}

u8 func_00314C10(s32 work) {
    return ((ScriptFlagWork *)work)->scriptId;
}

u8 func_00314C18(u8 *context, u32 entryId) {
    ((ScriptFlagWork *)context)->scriptId = entryId;
    func_00314A80(context, (u16)entryId);
    return ((ScriptFlagWork *)context)->scriptId;
}

void scrDecodePackedFlagIndex(s32 unused, u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 7;
    v >>= 3;
    *a = v;
    *b = lo << 2;
}

void func_00314C68(s32 work) {
    memset(((ScriptFlagWork *)work)->flagWords, 0, 0x154);
}

s32 scrSetFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    ((ScriptFlagWork *)work)->flagWords[word] |= 1U << shift;
    return 1;
}

void func_00314CE8(u8 *work, u16 id) {
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

void func_00314D90(void) {
    u32 temp_v0;
    s32 temp_v1;

    memset(D_00435DD0 + 0x16ef0, 0, 0x10);
    temp_v0 = mnuPickPairedTableValue(0x10, 0);
    temp_v1 = mnuPickPairedTableValue(0x10, 1);
    for (; (s32)temp_v0 < temp_v1; temp_v0 = temp_v0 + 1) {
        func_001B7940(temp_v0 & 0xffff, 1);
    }
}

void scrSetGlobalBitFlag(u32 id) {
    u16 bit;
    u32 *word;
    s32 offset;
    id &= 0xffff;
    if (id < 0x1ab) return;
    if (id >= 0x220) return;
    bit = id + 0xfe55;
    offset = 0x16ef0 + (bit >> 5) * 4;
    word = (u32 *)(D_00435DD0 + offset);
    *word |= 1U << (bit & 31);
}

/* Tests the global bit that scrSetGlobalBitFlag sets (ids 0x1AB..0x21F). */
u32 scrTestGlobalBitFlag(u16 id) {
    if (id < 0x1ab) return 0;
    if (id >= 0x220) return 0;
    id += 0xfe55;
    return *(u32 *)(D_00435DD0 + 0x16ef0 + (id >> 5) * 4) & (1U << (id & 31));
}

void scrSetSecondaryScriptFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    ((ScriptFlagWork *)work)->flagWords[word] |= 4U << shift;
}

void scrClearSecondaryScriptFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    ((ScriptFlagWork *)work)->flagWords[word] &= ~(4U << shift);
}

void func_00314F90(u8 *work) {
    s32 index = 0;
    do {
        scrClearSecondaryScriptFlag(work, (u16)index);
        index++;
    } while (index < 0x2A0);
}

u32 scrGetSecondaryScriptFlag(u8 *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    return ((ScriptFlagWork *)work)->flagWords[word] & (4U << shift);
}

u32 ptyGetSkillNibbleState(u8 *work, u16 index) {
    u32 word, shift;
    u32 mask;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    mask = ((ScriptFlagWork *)work)->flagWords[word];
    if (mask & (2U << shift)) {
        return 2;
    }
    return (mask & (1U << shift)) != 0;
}

s32 ptyHasSkill(s32 work, s32 id) {
    u32 key = id & 0xFFFF;
    u16 *p = ((ScriptFlagWork *)work)->slotIds;
    u32 i = 0;

    do {
        if (*p++ == key) {
            return 1;
        }
        i++;
    } while (i < 0x18);
    return 0;
}

s32 scrFindSlot(u8 *work, u16 key) {
    u32 index;
    u16 *entries = ((ScriptFlagWork *)work)->slotIds;
    for (index = 0; index < 24; index++) {
        if (entries[index] == key) {
            return index;
        }
    }
    return -1;
}

u16 scrGetSlot(u8 *work, u32 index) {
    if (index >= 24) {
        return 0;
    }
    return ((ScriptFlagWork *)work)->slotIds[index];
}

u32 scrCountSlots(u8 *work) {
    u16 *entries = ((ScriptFlagWork *)work)->slotIds;
    u32 count = 0;
    u32 index;
    for (index = 0; index < 24; index++) {
        if (entries[index] != 0) {
            count++;
        }
    }
    return count;
}

u16 scrSetSlot(s32 work, s32 index, u16 id) {
    u16 previous;
    u16 *slot;

    /* Required to match: offset-first address calculation for slotIds[index]. */
    slot = (u16 *)(index * 2 + work + 0x22);
    previous = *slot;
    *slot = id;
    return previous;
}

s32 scrRemoveSlot(u8 *work, u16 key) {
    s32 index = scrFindSlot(work, key);
    if (index >= 0) {
        ((ScriptFlagWork *)work)->slotIds[index] = 0;
        return 1;
    }
    return 0;
}

u8 func_003151D0(u16 scriptId) {
    return D_00401324[scriptId * 36];
}

u16 func_003151F8(u16 scriptId) {
    return *(u16 *)((u8 *)D_00401326 + scriptId * 36);
}

u8 func_00315220(u16 scriptId) {
    return D_00401325[scriptId * 36];
}

u32 func_00315248(u16 id, s32 sub) {
    return D_0040132C[sub + id * 36];
}

u32 scrCallIfOperandReady(u8 *operand, s32 value) {
    s32 selected = func_00314C10((s32)operand);

    if (func_00314C10((s32)operand)) {
        return func_00315248((u16)selected, value);
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
    if (current < (s32)func_003151F8(scriptId)) return 0;
    return 1;
}

void func_00315350(u32 unused0, u32 unused1, u32 output) {
    memset(output, 0, 8);
}

u8 func_00315370(s32 work, u16 id) {
    return ((ScriptFlagWork *)work)->scriptId == id;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00315388);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_003154A0);

void prfBuildSkillListState0(a, b, c)
s32 a;
s32 b;
s32 c;
{
    func_003154A0(a, b, c, 0);
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
                u8 *entry = (u8 *)D_00435DD0 + 0xa60 + offset;
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
    if (*(u32 *)(D_00435DD0 + 0x3C) < ((PtyReqCount *)a)->minimumCount) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00315950);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00315A50);

s32 func_00315BF8(s16 id) {
    u32 *info = func_0026CF70(id);
    if (info == 0) {
        return 0;
    }
    return D_0045C828[(s32)(*info << 24) >> 28] != 0;
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

u32 func_00315FC8(u16 index) {
    return *(u32 *)D_00401320[index];
}

u32 func_00315FF0(u16 index) {
    return D_00402BE0[index].state;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316020);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316118);

u8 prfReq18GetWord3220(s32 i) {
    return D_00404A90[i].v0;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", prfReqGetPair);

u32 prfReq18GetWord3234(s32 i) {
    return D_00404AA4[i].v0;
}

void sdfSetAllFlagsFromTable(void) {
    s32 index = 0;

    do {
        index = func_00316118(index);
        if (index >= 0) {
            s32 flag = prfReq18GetWord3234(index);
            if (flag != 0) {
                mdlFlagSet(flag);
            }
        }
    } while (index++ >= 0);
}

ScriptEntry44 *func_003162D8(u16 id) {
    return &D_00402BE0[id];
}

void scrSetEntryFlag(u32 context, u16 entryId, u32 bit) {
    ScriptFlagEntry *entry;
    if (bit < 16) {
        entry = (ScriptFlagEntry *)func_00314B80(context, entryId);
        entry->flags |= 1 << bit;
    }
}

void scrClearEntryFlag(u32 context, u16 entryId, u32 bit) {
    ScriptFlagEntry *entry;
    if (bit < 16) {
        entry = (ScriptFlagEntry *)func_00314B80(context, entryId);
        entry->flags &= ~(1 << bit);
    }
}

s32 scrTestEntryFlag(u32 context, u16 entryId, u32 bit) {
    if (bit >= 16) {
        return 0;
    }
    return (((ScriptFlagEntry *)func_00314B80(context, entryId))->flags & (1 << bit)) != 0;
}

void scrSetEntryLowFlags(u32 context, u16 entryId, u16 lowFlags) {
    ScriptFlagEntry *entry = (ScriptFlagEntry *)func_00314B80(context, entryId);
    entry->flags = (entry->flags & 0xFFFF0000) | lowFlags;
}

u16 scrGetEntryLowFlags(u32 context, u16 entryId) {
    return *(u16 *)&((ScriptFlagEntry *)func_00314B80(context, entryId))->flags;
}

void func_00316450(s32 x, s32 y, u32 first, u16 width, u32 second, s32 option) {
    u32 handle = func_0019FC38(x, y, first, width, (u32)D_00405CA8, 0);
    frFontSetChildColors(handle, second);
    func_0019D550(handle, 1, option);
    frFontQueueGlyphInSelectedSlot(handle);
}

u8 *func_003164C0(void) {
    return D_00405CA8;
}

/* Each list entry occupies two words; only its first word is reset here. */
typedef struct ScriptFlagList {
    u8 pad00[8];
    u32 *entries;      /* 0x08 */
    u8 pad0C[4];
    u32 clearBuffer;   /* 0x10 */
    u8 pad14[0x38];
    u32 count;         /* 0x4C */
} ScriptFlagList;

void sdfResetFlagListEntries(s32 list) {
    u32 entryCount;
    u32 *entry;
    u32 index;

    index = 0;
    entryCount = ((ScriptFlagList *)list)->count;
    entry = ((ScriptFlagList *)list)->entries;
    if (entryCount != 0) {
        do {
            index = index + 1;
            *entry = 0xffffffff;
            entry = entry + 2;
        } while (index < entryCount);
    }
    memset(((ScriptFlagList *)list)->clearBuffer, 0, entryCount << 3);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316528);

void func_00316648(void) {
    func_002DEB80(fileResolvePrimaryBuffer());
}

void func_00316668(s32 work) {
    func_003297C8(*(u32 *)(work + 0x54));
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316680);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316C88);

float func_00316DD0(ScrVmOperand *op) {
    return op->f50;
}

void func_00316DD8(ScrVmOperand *op, float value) {
    op->f50 = value;
}

void func_00316DE0(s32 work) {
    func_002D7458(work + 0x14, work + 0x38, 0, 0);
}
INCLUDE_SDATA(const s32, "game/code_00313BB8", D_00438908);

INCLUDE_SDATA(const s32, "game/code_00313BB8", D_00438910);

INCLUDE_SDATA(const s32, "game/code_00313BB8", D_00438918);

