#include "common.h"

extern s32 D_00435E38;

extern s32 dds3FindEntry();

extern u32 D_00435E88;

extern s32 D_00435DEC;

extern s32 D_00435DD0;

/* Item quantities occupy byte slots in the save-state block. */
typedef struct SaveItemCounts {
    u8 pad00[0x1340];
    u8 counts[0x100];
} SaveItemCounts;

typedef struct Entry1A4 {
    u16 flags; /* 0x0 */
    u8 pad2[2]; /* 0x2 */
    u16 rosterIndex; /* 0x4 */
    u16 unk6; /* 0x6 */
    u8 pad8[6]; /* 0x8 */
    u16 unkE; /* 0xE */
    u8 pad10[4]; /* 0x10 */
    u16 level; /* 0x14: clamped at level 99 by dds3Clamp99 */
    u8 pad16[0x3C]; /* 0x16 */
    u16 tableValue; /* 0x52 */
    u8 pad54[0x150]; /* through 0x1A4 */
} Entry1A4;

typedef struct TableEntry32 {
    u16 unk0; /* 0x0 */
    u16 unk2; /* 0x2 */
} TableEntry32;

extern TableEntry32 D_00386248[];

typedef struct Entry4 {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 pad3; /* 0x3 */
} Entry4;

extern Entry4 D_003862C8[];

/* Script-visible records: only the accessed fields are identified. */
typedef struct EventStatRecord {
    u8 pad00[0x11];
    u8 stat11;
    u8 pad12[6];
    s16 stat18;
    u8 pad1A[2];
    s16 stat1C;
    u8 pad1E[7];
    u8 stat25;
    u8 pad26[7];
    u8 stat2D;
    u8 pad2E[6];
    s16 stat34;
    s16 stat36;
} EventStatRecord; /* stride 0x38 */

typedef struct EventRosterStat {
    s16 base;             /* 0x00 */
    u8 alternateA;        /* 0x02 */
    u8 alternateB;        /* 0x03 */
    f32 multiplier;       /* 0x04 */
    u8 pad08[0x0C];
} EventRosterStat; /* stride 0x14 */

typedef struct EventRosterRecord {
    u8 pad00[4];
    u8 flaggedValue;      /* 0x04 */
    u8 pad05[0x23];
    s32 panelValue;       /* 0x28 */
    u8 pad2C[0x20];
} EventRosterRecord; /* stride 0x4C */

typedef struct EventIndexRecord {
    u8 pad00[2];
    u16 index;            /* 0x02 */
    u8 pad04[4];
} EventIndexRecord; /* stride 0x08 */

/* Both active script-entry pointers expose these same word offsets. */
typedef struct EventScriptEntry {
    u16 flags;           /* 0x00 */
    u8 pad02[2];
    u16 rosterIndex;     /* 0x04 */
    u16 value6;          /* 0x06 */
    u16 value8;          /* 0x08 */
    u8 pad0A[0x0A];
    u16 index14;         /* 0x14 */
} EventScriptEntry;

typedef struct EventModeSlot {
    u8 pad0;
    s8 kind;             /* 0x01 */
} EventModeSlot;

extern void func_0011A118(s32 arg0, s32 arg1);

extern void func_0011CA88(Entry1A4 *entry);

extern s32 D_0043E5C8[];

extern s32 scrSetIntegerReturnValue();

extern s32 D_0043E5CC[];

extern s32 scrReadIntParameter(s32 idx);

extern s32 func_00119A78(s32 arg0, s32 arg1);

extern s32 D_00435E20;

extern s32 D_00435E1C;

extern s32 D_00435DE8;

extern s32 evtGetMirroredSolarPhase(void);

extern s32 D_0043E5C4[];

extern s32 D_0043E5C0[];

extern s32 D_0043E5D0[];

extern void scrSetFloatReturnValue(f32 value);

extern s32 effMiscRandMod(u32 stream, u32 modulus);

extern s32 datMoveCursorX(void *, s32);

extern s32 datMoveCursorY(void *, s32);

extern u8 btlIsRuntimeAllocated(void);

extern f32 func_001AD978(void);

extern u32 func_001ADA10(void);

extern s32 D_00435E8C;

extern s32 mdlFlagTest(s32 flagIndex);

extern s32 D_00435E3C;

extern s32 D_00386288[];

extern void mnuSetPartyEntryCurrentId(Entry1A4 *, s32);

extern s32 func_0011AEE0(s32);

extern void func_00314CE8(s32, s32);

extern s32 D_0043E5C0[];

extern void func_0011A808(Entry1A4 *, s32);

extern s32 func_00119AF8(Entry1A4 *, s32);

extern void func_0010C058(u32, s32);

extern void bfStepContext(u32);

extern s32 func_001B3918(u32 arg0);

extern s32 func_001B3830(u32 arg0);

extern s32 func_001B3A00(u32 arg0);

extern s32 scrCreateTaskWithDefaultOption(void);

extern void *memset(void *dst, s32 c, u32 n);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A118);

s32 evtCheckValueThreshold(s32 index, s32 limit) {
    if ((u32)(index - 0x80) < 0x20) {
        return mdlFlagTest(index + 0x980) != 0;
    }
    if (((SaveItemCounts *)D_00435DD0)->counts[index] < limit) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A220);

u8 evtGetFlaggedRosterValue(Entry1A4 *entry) {
    if ((entry->flags & 0x20) == 0) {
        return 0;
    }
    return ((EventRosterRecord *)D_00435DEC)[entry->rosterIndex].flaggedValue;
}

s32 dds3FindEntryIndex(rosterIndex)
    s32 rosterIndex;
{
    s32 index = 0;
    s32 entry = D_00435DD0 + 0xA60;
    do {
        if (((Entry1A4 *)entry)->flags & 1) {
            if (((Entry1A4 *)entry)->rosterIndex == rosterIndex) {
                return index;
            }
        }
        index++;
        entry += 0x1C4;
    } while (index < 5);
    return -1;
}

s8 func_0011A318(s32 index) {
    return *(s8 *)(index + D_00435DD0 + 0xa76);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A510);

void ptyRecoverAllUnits(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_00435DD0 + offset + 0xA60);
        offset += 0x1C4;
        if (entry->flags & 1) {
            datMoveCursorX(entry, 9999);
            datMoveCursorY(entry, 9999);
            entry->unkE &= 0x8000;
        }
        remaining--;
    } while (remaining >= 0);
}

s32 ptyAnyUnitFlagMatch(u32 mask, s32 mode) {
    s32 index = 0;
    s32 entry = D_00435DD0 + 0xA60;
    do {
        if (((Entry1A4 *)entry)->flags & 1) {
            if (((Entry1A4 *)entry)->unk6 != 0) {
                if (mode != 1 || (((Entry1A4 *)entry)->flags & 2)) {
                    if (((Entry1A4 *)entry)->unkE & mask) {
                        return 1;
                    }
                }
            }
        }
        index++;
        entry += 0x1C4;
    } while (index < 5);
    return 0;
}

void evtAdvanceCounterValue(s32 counterAddress, s32 increment) {
    *(s32 *)(counterAddress + 0x10) = *(s32 *)(counterAddress + 0x10) + increment;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A808);

void evtUpdateFlaggedStats(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_00435DD0 + offset + 0xA60);
        offset += 0x1C4;
        if (entry->flags & 1) {
            if (entry->flags & 2) {
                func_0011A808(entry, 0x24C);
                func_0011A808(entry, 0x270);
            }
        }
        remaining--;
    } while (remaining >= 0);
}

s32 evtHasMatchingFlaggedEntry(s32 mask) {
    s32 index = 0;
    s32 offset = 0;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_00435DD0 + offset + 0xA60);
        offset += 0x1C4;
        if ((entry->flags & 1) && (entry->flags & 2)) {
            if (func_00119AF8(entry, mask)) {
                return 1;
            }
        }
        index++;
    } while (index < 5);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AA58);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AB38);

u16 evtGetIndexedEventRecordId(s32 tableIndex) {
    return ((EventIndexRecord *)D_00435E38)[tableIndex].index;
}

u16 dds3Clamp99(s32 unit) {
    s32 level = ((Entry1A4 *)unit)->level;

    return level < 100 ? level : 99;
}

s32 dds3FindEntry(rosterIndex)
    s32 rosterIndex;
{
    s32 index = 0;
    s32 entry = D_00435DD0 + 0xA60;
    do {
        if (((Entry1A4 *)entry)->rosterIndex == rosterIndex && (((Entry1A4 *)entry)->flags & 1)) {
            return entry;
        }
        index++;
        entry += 0x1C4;
    } while (index < 5);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AEE0);

u8 func_0011B260(void) {
    s64 entry;

    entry = dds3FindEntry();
    return entry != 0;
}

s32 dds3EntryMax(void) {
    s32 max = 0;
    s32 remaining = 4;
    s32 entry = D_00435DD0 + 0xA60;
    do {
        remaining--;
        if (((Entry1A4 *)entry)->flags & 1) {
            s32 level = ((Entry1A4 *)entry)->level;
            if (max < level) {
                max = level;
            }
        }
        entry += 0x1C4;
    } while (remaining >= 0);
    return max;
}

s32 func_0011B2C0(void) {
    s32 sum = 0;
    s32 count = 0;
    s32 remaining = 4;
    s32 entry = D_00435DD0 + 0xA60;
    do {
        remaining--;
        if ((((Entry1A4 *)entry)->flags & 1) != 0) {
            count++;
            sum += ((Entry1A4 *)entry)->level;
        }
        entry += 0x1C4;
    } while (remaining >= 0);
    if (count == 0) {
        return 0;
    }
    return (sum + count - 1) / count;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B4B0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B6F0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011B9A0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011BC80);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C0B0);

void func_0011C328(u32 arg0) {
    func_0011C0B0(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C340);

void func_0011C680(u32 arg0) {
    func_0011C340(arg0, 0);
}

u32 func_0011C698(void) {
    return 0;
}

u32 func_0011C6A0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C6A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011C868);

void evtCopyRosterTableValue(s32 entryAddress) {
    ((Entry1A4 *)entryAddress)->tableValue = D_00386248[((Entry1A4 *)entryAddress)->rosterIndex].unk0;
}

void evtUpdateFlaggedEntries(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_00435DD0 + offset + 0xA60);
        if (entry->flags & 1) {
            s32 index = 0;
            do {
                if (entry->rosterIndex == index) {
                    evtCopyRosterTableValue(entry);
                }
                index++;
            } while (index < 16);
        }
        remaining--;
        offset += 0x1C4;
    } while (remaining >= 0);
}

void dds3ForEachEntry(void) {
    Entry4 *p = D_003862C8;
    u32 i = 0;

    do {
        u16 a0 = p->unk0;
        u8 a1 = p->unk2;

        p++;
        if (a0 != 0) {
            func_0011A118(a0, a1);
        }
        i++;
    } while (i < 2);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011CA88);

void dds3ForEachFlagged(void) {
    s32 off = 0;
    s32 n = 4;

    do {
        Entry1A4 *p = (Entry1A4 *)(D_00435DD0 + off + 0xa60);

        if (p->flags & 1) {
            func_0011CA88(p);
        }
        off += 0x1C4;
        n--;
    } while (n >= 0);
}

void func_0011D050(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        s32 entry = D_00435DD0 + offset + 0xA60;
        offset += 0x1C4;
        if ((((Entry1A4 *)entry)->flags & 1) != 0) {
            func_00314CE8(entry, 0x5B);
            func_00314CE8(entry, 0x5C);
            func_00314CE8(entry, 0x5D);
        }
        remaining--;
    } while (remaining >= 0);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D0D8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D130);

void evtRandomizeEntryValue(s32 unit) {
    s32 randomOffset;

    randomOffset = effMiscRandMod(0, 4);
    *(s32 *)(unit + 0x1B4) = 0x12 - randomOffset;
}

/* Clear a subset of per-unit status flags on occupied qualifying entries,
 * gated by the event RNG; report whether any flags were cleared. */
s32 evtClearRandomStatusFlags(void) {
    s32 changed = 0;
    s32 remaining;
    s32 entry;
    if (effMiscRandMod(0, 100) >= 51) {
        return 0;
    }
    remaining = 4;
    entry = D_00435DD0 + 0xA60;
    do {
        if ((((Entry1A4 *)entry)->flags & 1) != 0 && ((Entry1A4 *)entry)->unk6 != 0) {
            u16 flags = ((Entry1A4 *)entry)->unkE;
            if ((flags & 0x5D0) != 0) {
                ((Entry1A4 *)entry)->unkE = flags & ~0x5D0;
                changed = 1;
            }
        }
        remaining--;
        entry += 0x1C4;
    } while (remaining >= 0);
    return changed;
}

s32 func_0011D360(s32 id, s32 slot) {
    s32 index = id - 0xC0;
    s32 tableOffset = index * 6;
    s32 flagOffset = slot + index * 5;
    if (index <= 0) {
        return 0;
    }
    return *(s8 *)(tableOffset + D_00435E3C + slot) +
           *(u8 *)(flagOffset + D_00435DD0 + 0x1E670);
}

void func_0011D3B8(s32 base, s32 offset, s32 amount) {
    s8 *value = (s8 *)(offset + base + 0x16);
    s32 updated = *value + amount;
    if (updated < 0) {
        updated = 0;
    }
    if (updated >= 100) {
        updated = 99;
    }
    *value = updated;
}

void func_0011D3E8(Entry1A4 *entry) {
    s32 value = D_00386288[entry->rosterIndex];
    mnuSetPartyEntryCurrentId(entry, value);
    if (value != 0) {
        ((SaveItemCounts *)D_00435DD0)->counts[value] = 1;
    }
}

void func_0011D438(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_00435DD0 + offset + 0xA60);
        if (entry->flags & 1) {
            s32 index = 0;
            do {
                if (entry->rosterIndex == index) {
                    func_0011D3E8(entry);
                }
                index++;
            } while (index < 16);
        }
        remaining--;
        offset += 0x1C4;
    } while (remaining >= 0);
}

s32 evtRunContext(s32 script, s32 first, s32 second, s32 third, u16 flags) {
    func_0010C058(D_00435E88, script);
    D_0043E5C0[1] = third;
    D_0043E5C0[2] = first;
    D_0043E5C0[3] = second;
    ((EventScriptEntry *)D_0043E5C0)->index14 = flags;
    ((EventScriptEntry *)D_0043E5C0)->flags &= 0xFFFE;
    bfStepContext(D_00435E88);
    return D_0043E5C0[4];
}

void dds3WorkInit(void) {
    D_00435E88 = scrCreateTaskWithDefaultOption();
    memset(D_0043E5C0, 0, 0x18);
}

u32 scrGetWorkTaskHandle(void) {
    return D_00435E88;
}

void scrDestroyWorkTask(void) {
    scrProcDestroyTask(D_00435E88);
    D_00435E88 = 0;
}

s32 func_0011D5B8(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5C8[0])->index14);
    return 1;
}

s32 func_0011D5E0(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5CC[0])->index14);
    return 1;
}

s32 func_0011D608(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5C8[0])->value6);
    return 1;
}

s32 func_0011D630(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5CC[0])->value6);
    return 1;
}

s32 func_0011D658(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5C8[0])->value8);
    return 1;
}

s32 func_0011D680(void) {
    scrSetIntegerReturnValue(((EventScriptEntry *)D_0043E5CC[0])->value8);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D6A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D758);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D808);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D8B0);

s32 func_0011D958(void) {
    s32 val = scrReadIntParameter(0);

    scrSetIntegerReturnValue(func_00119A78(D_0043E5C8[0], val));
    return 1;
}

s32 func_0011D990(void) {
    s32 val = scrReadIntParameter(0);

    scrSetIntegerReturnValue(func_00119A78(D_0043E5CC[0], val));
    return 1;
}

s32 func_0011D9C8(void) {
    s32 value;
    s32 index = D_0043E5C0[1];
    if (((EventModeSlot *)D_00435E1C)[index].kind == 5) {
        u16 id = ((EventScriptEntry *)D_0043E5C0[2])->rosterIndex;
        value = ((EventRosterStat *)D_00435DE8)[id].alternateA;
    } else {
        value = ((EventStatRecord *)D_00435E20)[index].stat11;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushSelectedScaledStat(void) {
    s32 value;
    s32 index = D_0043E5C0[1];
    value = ((EventStatRecord *)D_00435E20)[index].stat25;
    if (((EventModeSlot *)D_00435E1C)[index].kind == 5) {
        u16 id = ((EventScriptEntry *)D_0043E5C0[2])->rosterIndex;
        value = (s32)((f32)value * ((EventRosterStat *)D_00435DE8)[id].multiplier);
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushSelectedStatGrade(void) {
    scrSetIntegerReturnValue(((EventStatRecord *)D_00435E20)[D_0043E5C4[0]].stat2D);
    return 1;
}

s32 evtSelectScriptStatValue(void) {
    s32 *work = D_0043E5C0;
    s32 value;
    u16 mode = ((EventScriptEntry *)work)->index14;
    switch (mode) {
    case 1:
        value = ((EventStatRecord *)D_00435E20)[work[1]].stat18;
        break;
    case 2:
        value = ((EventStatRecord *)D_00435E20)[work[1]].stat1C;
        break;
    default:
        value = 0;
        break;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushEntryIndexedStatOption(void) {
    s32 *work = D_0043E5C0;
    s32 value;
    u16 mode = ((EventScriptEntry *)work)->index14;
    u16 index = ((EventIndexRecord *)D_00435E38)[((Entry1A4 *)work[2])->tableValue].index;
    switch (mode) {
    case 1:
        value = ((EventStatRecord *)D_00435E20)[index].stat18;
        break;
    case 2:
        value = ((EventStatRecord *)D_00435E20)[index].stat1C;
        break;
    default:
        value = 0;
        break;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 func_0011DC38(void) {
    s32 value;
    s32 index = D_0043E5C0[1];
    if (((EventModeSlot *)D_00435E1C)[index].kind == 5) {
        u16 id = ((EventScriptEntry *)D_0043E5C0[2])->rosterIndex;
        value = ((EventRosterStat *)D_00435DE8)[id].alternateB;
    } else {
        value = ((EventStatRecord *)D_00435E20)[index].stat34;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushSelectedStatMaximum(void) {
    scrSetIntegerReturnValue(((EventStatRecord *)D_00435E20)[D_0043E5C4[0]].stat36);
    return 1;
}

s32 evtStoreScriptParameterResult(void) {
    u16 bits = ((EventScriptEntry *)D_0043E5C0)->flags | 1;

    ((EventScriptEntry *)D_0043E5C0)->flags = bits;
    D_0043E5C0[4] = scrReadIntParameter(0);
    return 1;
}

s32 func_0011DD40(void) {
    scrSetIntegerReturnValue(D_0043E5D0[0]);
    return 1;
}

s32 evtPushEntryFlagBitInverted(void) {
    scrSetIntegerReturnValue((((((EventScriptEntry *)D_0043E5C8[0])->flags) >> 5) ^ 1) & 1);
    return 1;
}

/* Apply a random percentage offset around 1.0 to the active script value. */
s32 evtRollRandomScale(void) {
    s32 range = scrReadIntParameter(0);
    s32 roll = effMiscRandMod(0, range * 2);

    scrSetFloatReturnValue((f32)(roll - range + 100) / 100.0f);
    return 1;
}

s32 func_0011DE08(void) {
    s32 available = btlIsRuntimeAllocated();
    s32 value;
    if (available) {
        s32 choice = scrReadIntParameter(0);
        value = func_001B3918(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 func_0011DE58(void) {
    s32 available = btlIsRuntimeAllocated();
    s32 value;
    if (available) {
        s32 choice = scrReadIntParameter(0);
        value = func_001B3830(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 func_0011DEA8(void) {
    s32 available = btlIsRuntimeAllocated();
    s32 value;
    if (available) {
        s32 choice = scrReadIntParameter(0);
        value = func_001B3A00(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushAvailableChoiceRatio(void) {
    s32 available = btlIsRuntimeAllocated();
    u64 value;
    if (available) {
        s32 choice = scrReadIntParameter(0);
        s32 divisor = scrReadIntParameter(1);
        s32 dividend = choice ? 0x20 : 4;
        value = (u64)dividend / divisor;
    }
    else {
        value = 0;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushAvailableFloatValue(void) {
    s32 v0 = btlIsRuntimeAllocated();
    f32 val = 0.0f;

    if (v0 != 0) {
        val = func_001AD978();
    }
    scrSetFloatReturnValue(val);
    return 1;
}

s32 evtPushAvailableIntegerValue(void) {
    s32 v0 = btlIsRuntimeAllocated();
    s32 val = 0;

    if (v0 != 0) {
        val = func_001ADA10();
    }
    scrSetIntegerReturnValue(val);
    return 1;
}

extern s32 D_00435E44;

s32 func_0011DFE0(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = ((EventScriptEntry *)entry)->index14;
    scrSetFloatReturnValue(*(f32 *)((D_00435E44 - 4) + index * 4));
    return 1;
}

s32 func_0011E018(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = ((EventScriptEntry *)entry)->index14;
    scrSetFloatReturnValue(*(f32 *)(D_00435E44 + index * 4 + 0x188));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E050);

s32 func_0011E128(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = ((EventScriptEntry *)entry)->index14;
    scrSetFloatReturnValue(*(f32 *)(D_00435E44 + index * 4 + 0x360));
    return 1;
}

s32 func_0011E160(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = ((EventScriptEntry *)entry)->index14;
    scrSetFloatReturnValue(*(f32 *)(D_00435E44 + index * 4 + 0x4EC));
    return 1;
}

s32 func_0011E198(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = ((EventScriptEntry *)entry)->index14;
    scrSetFloatReturnValue(*(f32 *)(D_00435E44 + index * 4 + 0x678));
    return 1;
}

s32 func_0011E1D0(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = ((EventScriptEntry *)entry)->index14;
    scrSetFloatReturnValue(*(f32 *)(D_00435E44 + index * 4 + 0x678));
    return 1;
}

s32 func_0011E208(void) {
    s32 value;
    if ((((EventScriptEntry *)D_0043E5CC[0])->flags & 0x20) == 0) {
        value = effMiscRandMod(0, 0x20) != 0 ? 0xA : 0x80;
    } else {
        value = effMiscRandMod(0, 0x30) != 0 ? 0 : 0x80;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushRosterBaseValue(void) {
    u16 index = ((EventScriptEntry *)D_0043E5C8[0])->rosterIndex;
    scrSetIntegerReturnValue(((EventRosterStat *)D_00435DE8)[index].base);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E2A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E3A0);

s32 func_0011E430(void) {
    s32 entry = D_0043E5C8[0];
    s32 result;
    if (!(((EventScriptEntry *)entry)->flags & 0x20)) {
        result = *(s32 *)(D_00435DD0 + 0x3C);
    } else {
        s32 index = ((EventScriptEntry *)entry)->rosterIndex;
        result = ((EventRosterRecord *)D_00435DEC)[index].panelValue;
    }
    scrSetIntegerReturnValue(result);
    return 1;
}

s32 evtTestSolarPhaseOrModelFlag(u32 flags) {
    u32 type = flags >> 16;
    switch (type) {
    case 0:
        break;
    case 1:
        if (flags & (1 << evtGetMirroredSolarPhase()) & 0xFFFF) {
            return 1;
        }
        break;
    case 2:
        if (mdlFlagTest(flags & 0xFFFF)) {
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E528);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E728);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E848);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E930);

void dds3WorkClear(void) {
    s32 base = D_00435DD0;

    *(s32 *)(base + 0x1440) = 0;
    *(s16 *)(base + 0x1444) = 0;
    D_00435E8C = 0;
}

void func_0011EBE0(void) {
}

void func_0011EBE8(void) {
}

void func_0011EBF0(void) {
}

void func_0011EBF8(void) {
}

s32 func_0011EC00(void) {
    s32 value = scrReadIntParameter(0);
    s32 state;
    if (scrReadIntParameter(1) == 0) {
        state = func_0011B260();
    } else {
        state = func_0011AEE0(value);
    }
    scrSetIntegerReturnValue(state == 1);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E04);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E08);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E0C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E10);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E14);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E18);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E1C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E20);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E24);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E28);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E2C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E30);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E34);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E38);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E3C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E40);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E44);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E48);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E4C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E50);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E54);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E58);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E5C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E60);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E64);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E68);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E6C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E70);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E74);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E78);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E7C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E80);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E88);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E8C);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E90);

INCLUDE_SDATA(const s32, "game/code_0011A118", D_00435E94);

