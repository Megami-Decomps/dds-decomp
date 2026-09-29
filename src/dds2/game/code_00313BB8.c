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

/* Operand block used by the script VM helpers near func_002CD730 (layout inferred from field accesses). */
typedef struct ScrVmOperand {
    u8 pad_0x00[0x04]; // 0x00
    u8 b04;            // 0x04
    u8 pad_0x05[0x0F]; // 0x05
    u16 h14;           // 0x14
    u8 pad_0x16[0x3A]; // 0x16
    float f50;         // 0x50
    u8 unk54;          // 0x54
    s8 s55;            // 0x55
} ScrVmOperand; // 0x56

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

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00313BB8);

void ptyClearProfileRecords(void) {
    memset(D_00435DD0 + 0x17210, 0, 0x5800);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00313C70);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00313D80);

void func_00313F88(u8 *work) {
    u16 *source = D_004052E8[*(u16 *)(work + 4)];
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
    u16 *source = D_004052F8[*(u16 *)(work + 4)];
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

INCLUDE_ASM(const s32, "game/code_00313BB8", ptyRebuildAllProfiles);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314298);

void ptyRecomputeMaxHpMp(u32 arg0) {
    func_00314298(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314500);

u32 func_00314668(u32 arg0, s32 *arg1) {
    *arg1 = D_00435E50 + (arg0 & 0xffff) * 0x13;
    return 1;
}

u32 func_00314690(u16 scriptId) {
    return D_00401328[scriptId][0];
}

void func_003146B8(u32 arg0, u16 arg1, u32 arg2) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    *puVar1 = arg2;
}

void ptySetProfileRecordToCap(u32 arg0, u16 arg1) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    temp_v0 = func_00314690(arg1);
    *puVar1 = temp_v0;
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
    ScriptFlagSlot *slot = (ScriptFlagSlot *)((u8 *)D_00404AA8 + (*(u16 *)(work + 4) << 7));
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

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314990);

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

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314B00);

u8 func_00314B78(s32 arg0) {
    return ((ScriptFlagWork *)arg0)->scriptId;
}

u32 func_00314B80(u32 ref, u16 index) {
    u32 entry = D_00435DD0 + *(u16 *)(ref + 4) * 0x580;

    return entry + (index << 3) + 0x17210;
}

u32 ptyGetProfileRecordValue(u32 arg0, u16 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00314B80(arg0, arg1);
    return *puVar1;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", ptyGetCurrentProfileRecord);

u8 func_00314C10(s32 arg0) {
    return ((ScriptFlagWork *)arg0)->scriptId;
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

void func_00314C68(s32 arg0) {
    memset(((ScriptFlagWork *)arg0)->flagWords, 0, 0x154);
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

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00314E80);

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

s32 ptyHasSkill(s32 arg0, s32 arg1) {
    u32 key = arg1 & 0xFFFF;
    u16 *p = ((ScriptFlagWork *)arg0)->slotIds;
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

u16 func_00315118(u8 *work, u32 index) {
    if (index >= 24) {
        return 0;
    }
    return ((ScriptFlagWork *)work)->slotIds[index];
}

u32 func_00315138(u8 *work) {
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

u16 func_00315170(s32 arg0, s32 arg1, u16 arg2) {
    u16 temp_v0;
    u16 *puVar2;

    /* Required to match: offset-first address calculation for slotIds[arg1]. */
    puVar2 = (u16 *)(arg1 * 2 + arg0 + 0x22);
    temp_v0 = *puVar2;
    *puVar2 = arg2;
    return temp_v0;
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

u8 func_00315248(u16 id, s32 sub) {
    return D_0040132C[sub + id * 36];
}

INCLUDE_ASM(const s32, "game/code_00313BB8", scrCallIfOperandReady);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_003152D8);

u32 func_00315318(void) {
    return 0;
}

s32 ptyCheckLevelAtLeastProfileParam7b6(u8 *work, u16 scriptId) {
    s32 current = *(u16 *)(work + 0x14);
    if (current < (s32)func_003151F8(scriptId)) return 0;
    return 1;
}

void func_00315350(u32 arg0, u32 arg1, u32 arg2) {
    memset(arg2, 0, 8);
}

u8 func_00315370(s32 arg0, u16 arg1) {
    return *(u8 *)(arg0 + 0x55) == arg1;
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
    if (matched < *(u32 *)(operand + 8)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_003157A0);

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
    if (*(u16 *)(a + 0x14) < b[4]) {
        return 0;
    }
    return 1;
}

s32 prfReqCheckGlobalCounter(u8 *a) {
    if (*(u32 *)(D_00435DD0 + 0x3C) < *(u32 *)(a + 8)) {
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

void func_00315FA0(u32 arg0, u32 arg1, u16 arg2) {
    func_00315C68(arg0, 0, arg1, arg2, 0);
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

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316450);

u8 *func_003164C0(void) {
    return D_00405CA8;
}

void func_003164D0(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(u32 *)(arg0 + 0x4c);
    puVar2 = *(u32 **)(arg0 + 8);
    if (temp_v0 != 0) {
        do {
            temp_v1 = temp_v1 + 1;
            *puVar2 = 0xffffffff;
            puVar2 = puVar2 + 2;
        } while (temp_v1 < temp_v0);
    }
    memset(*(u32 *)(arg0 + 0x10), 0, temp_v0 << 3);
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316528);

void func_00316648(void) {
    func_002DEB80(fileResolvePrimaryBuffer());
}

void func_00316668(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316680);

INCLUDE_ASM(const s32, "game/code_00313BB8", func_00316C88);

float func_00316DD0(ScrVmOperand *op) {
    return op->f50;
}

void func_00316DD8(ScrVmOperand *op, float value) {
    op->f50 = value;
}

void func_00316DE0(s32 arg0) {
    func_002D7458(arg0 + 0x14, arg0 + 0x38, 0, 0);
}
