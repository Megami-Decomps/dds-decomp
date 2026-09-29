#include "common.h"

extern s32 D_003BAA4C;
extern s32 D_003BAA00;
extern s32 D_003BAA18;
extern s32 D_003BAA1C;
extern s32 D_003BAA50;
extern s32 D_003BAA68;
extern s32 D_003BAA6C;
extern s32 D_003BAAB8;

typedef struct TableEntry32 {
    u16 value; /* 0x0: copied to active roster entry */
    u16 unk2; /* 0x2 */
} TableEntry32;

typedef struct Entry4 {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 pad3; /* 0x3 */
} Entry4;

typedef struct Entry1A4 {
    u16 flags; /* 0x0: active and flagged-entry bits */
    u8 pad2[2]; /* 0x2 */
    u16 rosterIndex; /* 0x4: entry identifier */
    u16 unk6; /* 0x6 */
    u8 pad8[6]; /* 0x8 */
    u16 unkE; /* 0xE */
    u8 pad10[4]; /* 0x10 */
    u16 unk14; /* 0x14 */
    u8 pad16[0x3C];      /* 0x016 */
    u16 tableValue;       /* 0x052 */
    u8 pad54[0x140];
    s32 randomizedValue;  /* 0x194 */
    u8 pad198[0xC];
} Entry1A4;

typedef struct EvtScriptContext {
    u16 stateFlags;      /* 0x00 */
    u16 pad02;
    s32 third;           /* 0x04 */
    s32 first;           /* 0x08 */
    s32 second;          /* 0x0C */
    s32 result;          /* 0x10 */
    u16 options;         /* 0x14 */
    u8 pad16[2];
} EvtScriptContext;

extern TableEntry32 D_0032AEA8[];
extern Entry4 D_0032AEE8[];

extern u32 D_003BAAB4;

extern s32 D_003C2E70[];
extern s32 D_003C2E74[];
extern s32 D_003C2E78[];
extern s32 D_003C2E7C[];
extern s32 D_003C2E80[];

extern s32 scrCreateTaskWithDefaultOption(void);
extern s32 func_0010D428(s32 idx);
extern s32 func_0010D5F0(s32 arg0);
extern void func_0010D608(f32 arg0);
extern Entry1A4 *dds3FindEntry(s32 arg0);
extern void func_00119900(s32 arg0, s32 arg1);
extern void func_0011B528(Entry1A4 *arg0);
extern s32 func_00119368(s32 arg0, s32 arg1);
extern u8 func_001A1438(void);
extern f32 func_001A4598(void);
extern u32 func_001A4630(void);
extern s32 func_001A92D0(u32 arg0);
extern s32 func_001A93B8(u32 arg0);
extern s32 func_001A94A0(u32 arg0);
extern void *memset(void *dst, s32 c, u32 n);
extern s32 mdlFlagTest(s32 arg0);
extern s32 effMiscRandMod(u32 arg0, u32 arg1);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119900);

s32 evtCheckValueThreshold(s32 index, s32 limit) {
    if ((u32)(index - 0x80) < 0x20) {
        return mdlFlagTest(index + 0x980) != 0;
    }
    if (*(u8 *)(index + D_003BAA00 + 0x12A0) < limit) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_00119A00);

u8 evtGetFlaggedRosterValue(s32 arg0) {
    if ((*(u16 *)arg0 & 0x20) == 0) {
        return 0;
    }
    return *(u8 *)(D_003BAA1C + *(u16 *)(arg0 + 4) * 76 + 4);
}

s32 dds3FindEntryIndex(s32 arg0) {
    Entry1A4 *p = (Entry1A4 *)(D_003BAA00 + 0xa60);
    s32 n = 0;

    do {
        if (p->flags & 1) {
            if (p->rosterIndex == arg0) {
                return n;
            }
        }
        n++;
        p++;
    } while (n < 5);
    return -1;
}

s8 func_00119AF8(s32 arg0) {
    return *(s8 *)(arg0 + D_003BAA00 + 0xa76);
}

INCLUDE_ASM(const s32, "game/code_00119900", func_00119B08);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119CF0);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119E00);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119E88);

void func_00119EF8(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 0x10) = *(s32 *)(arg0 + 0x10) + arg1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_00119F08);

extern void func_00119F08(Entry1A4 *, s32);

void evtUpdateFlaggedStats(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_003BAA00 + offset + 0xA60);
        offset += 0x1A4;
        if (entry->flags & 1) {
            if (entry->flags & 2) {
                func_00119F08(entry, 0x22C);
                func_00119F08(entry, 0x250);
            }
        }
        remaining--;
    } while (remaining >= 0);
}

extern s32 func_001193A0(Entry1A4 *, s32);

s32 evtHasMatchingFlaggedEntry(s32 mask) {
    s32 index = 0;
    s32 offset = 0;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_003BAA00 + offset + 0xA60);
        offset += 0x1A4;
        if ((entry->flags & 1) && (entry->flags & 2)) {
            if (func_001193A0(entry, mask)) {
                return 1;
            }
        }
        index++;
    } while (index < 5);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A158);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A238);

u16 func_0011A568(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_003BAA68 + 2);
}

u16 dds3Clamp99(s32 arg0) {
    s32 temp = *(u16 *)(arg0 + 0x14);

    return temp < 100 ? temp : 99;
}

Entry1A4 *dds3FindEntry(s32 rosterIndex) {
    Entry1A4 *entry = (Entry1A4 *)(D_003BAA00 + 0xa60);
    s32 index = 0;

    do {
        if (entry->rosterIndex != rosterIndex) {
            index++;
        } else {
            if (entry->flags & 1) {
                return entry;
            }
            index++;
        }
        entry++;
    } while (index < 5);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A5E8);

u8 func_0011A968(s32 arg0) {
    return dds3FindEntry(arg0) != 0;
}

s32 dds3EntryMax(void) {
    Entry1A4 *entry = (Entry1A4 *)(D_003BAA00 + 0xa60);
    s32 maximum = 0;
    s32 remaining = 4;

    do {
        if (entry->flags & 1) {
            if (maximum < entry->unk14) {
                maximum = entry->unk14;
            }
        }
        entry++;
        remaining--;
    } while (remaining >= 0);
    return maximum;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A9C8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011AA28);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011AE78);

u32 func_0011B140(void) {
    return 0;
}

u32 func_0011B148(void) {
    return 1;
}

u32 func_0011B150(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B158);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B308);

void func_0011B418(s32 arg0) {
    ((Entry1A4 *)arg0)->tableValue = D_0032AEA8[((Entry1A4 *)arg0)->rosterIndex].value;
}

void evtUpdateFlaggedEntries(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_003BAA00 + offset + 0xA60);
        if (entry->flags & 1) {
            s32 id = 0;
            do {
                if (entry->rosterIndex == id) {
                    func_0011B418((s32)entry);
                }
                id++;
            } while (id < 16);
        }
        remaining--;
        offset += 0x1A4;
    } while (remaining >= 0);
}

void dds3ForEachEntry(void) {
    Entry4 *p = D_0032AEE8;
    u32 i = 0;

    do {
        u16 a0 = p->unk0;
        u8 a1 = p->unk2;

        p++;
        if (a0 != 0) {
            func_00119900(a0, a1);
        }
        i++;
    } while (i < 2);
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B528);

void dds3ForEachFlagged(void) {
    s32 off = 0;
    s32 n = 4;

    do {
        Entry1A4 *p = (Entry1A4 *)(D_003BAA00 + off + 0xa60);

        if (p->flags & 1) {
            func_0011B528(p);
        }
        off += 0x1a4;
        n--;
    } while (n >= 0);
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B6A8);

void func_0011B7B8(s32 arg0) {
    s32 randomOffset;

    randomOffset = effMiscRandMod(0, 4);
    ((Entry1A4 *)arg0)->randomizedValue = 0x12 - randomOffset;
}

extern s32 D_003BAA00;

extern s32 effMiscRandMod(u32 arg0, u32 arg1);

s32 func_0011B7F0(void) {
    s32 changed = 0;
    s32 remaining;
    s32 entry;
    if (effMiscRandMod(0, 100) >= 51) {
        return 0;
    }
    remaining = 4;
    entry = D_003BAA00 + 0xA60;
    do {
        if ((*(u16 *)entry & 1) != 0 && *(u16 *)(entry + 6) != 0) {
            u16 flags = *(u16 *)(entry + 0xE);
            if ((flags & 0x5D0) != 0) {
                *(u16 *)(entry + 0xE) = flags & ~0x5D0;
                changed = 1;
            }
        }
        remaining--;
        entry += 0x1A4;
    } while (remaining >= 0);
    return changed;
}

extern void func_0010BE30(u32, s32);
extern void func_0010C0B0(u32);

s32 evtRunContext(s32 script, s32 first, s32 second, s32 third, u16 flags) {
    func_0010BE30(D_003BAAB4, script);
    ((EvtScriptContext *)D_003C2E70)->third = third;
    ((EvtScriptContext *)D_003C2E70)->first = first;
    ((EvtScriptContext *)D_003C2E70)->second = second;
    ((EvtScriptContext *)D_003C2E70)->options = flags;
    ((EvtScriptContext *)D_003C2E70)->stateFlags &= 0xFFFE;
    func_0010C0B0(D_003BAAB4);
    return ((EvtScriptContext *)D_003C2E70)->result;
}

void dds3WorkInit(void) {
    D_003BAAB4 = scrCreateTaskWithDefaultOption();
    memset(D_003C2E70, 0, 0x18);
}

u32 func_0011B938(void) {
    return D_003BAAB4;
}

void func_0011B940(void) {
    func_0010BD20(D_003BAAB4);
    D_003BAAB4 = 0;
}

s32 func_0011B968(void) {
    func_0010D5F0(*(u16 *)(D_003C2E78[0] + 0x14));
    return 1;
}

s32 func_0011B990(void) {
    func_0010D5F0(*(u16 *)(D_003C2E7C[0] + 0x14));
    return 1;
}

s32 func_0011B9B8(void) {
    func_0010D5F0(*(u16 *)(D_003C2E78[0] + 6));
    return 1;
}

s32 func_0011B9E0(void) {
    func_0010D5F0(*(u16 *)(D_003C2E7C[0] + 6));
    return 1;
}

s32 func_0011BA08(void) {
    func_0010D5F0(*(u16 *)(D_003C2E78[0] + 8));
    return 1;
}

s32 func_0011BA30(void) {
    func_0010D5F0(*(u16 *)(D_003C2E7C[0] + 8));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BA58);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BB08);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BBB8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BC60);

s32 func_0011BD08(void) {
    s32 val = func_0010D428(0);

    func_0010D5F0(func_00119368(D_003C2E78[0], val));
    return 1;
}

s32 func_0011BD40(void) {
    s32 val = func_0010D428(0);

    func_0010D5F0(func_00119368(D_003C2E7C[0], val));
    return 1;
}

s32 func_0011BD78(void) {
    EvtScriptContext *work = (EvtScriptContext *)D_003C2E70;
    s32 index = work->third;
    s32 value;
    if (*(s8 *)(D_003BAA4C + index * 2 + 1) == 5) {
        value = *(u8 *)(D_003BAA18 + ((Entry1A4 *)work->first)->rosterIndex * 20 + 2);
    } else {
        value = *(u8 *)(D_003BAA50 + index * 56 + 0x11);
    }
    func_0010D5F0(value);
    return 1;
}

s32 func_0011BE00(void) {
    EvtScriptContext *work = (EvtScriptContext *)D_003C2E70;
    s32 index = work->third;
    s32 value = *(u8 *)(D_003BAA50 + index * 56 + 0x25);
    if (*(s8 *)(D_003BAA4C + index * 2 + 1) == 5) {
        f32 scale = *(f32 *)(D_003BAA18 + ((Entry1A4 *)work->first)->rosterIndex * 20 + 4);
        value = (s32)((f32)value * scale);
    }
    func_0010D5F0(value);
    return 1;
}

s32 func_0011BE90(void) {
    func_0010D5F0(*(u8 *)(D_003BAA50 + D_003C2E74[0] * 56 + 0x2d));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BED0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BF50);

s32 func_0011BFE8(void) {
    EvtScriptContext *work = (EvtScriptContext *)D_003C2E70;
    s32 index = work->third;
    s32 value;
    if (*(s8 *)(D_003BAA4C + index * 2 + 1) == 5) {
        value = *(u8 *)(D_003BAA18 + ((Entry1A4 *)work->first)->rosterIndex * 20 + 3);
    } else {
        value = *(s16 *)(D_003BAA50 + index * 56 + 0x34);
    }
    func_0010D5F0(value);
    return 1;
}

s32 func_0011C070(void) {
    func_0010D5F0(*(s16 *)(D_003BAA50 + D_003C2E74[0] * 56 + 0x36));
    return 1;
}

s32 func_0011C0B0(void) {
    u16 bits = ((EvtScriptContext *)D_003C2E70)->stateFlags | 1;

    ((EvtScriptContext *)D_003C2E70)->stateFlags = bits;
    ((EvtScriptContext *)D_003C2E70)->result = func_0010D428(0);
    return 1;
}

s32 func_0011C0F0(void) {
    func_0010D5F0(D_003C2E80[0]);
    return 1;
}

s32 func_0011C118(void) {
    func_0010D5F0((((*(u16 *)D_003C2E78[0]) >> 5) ^ 1) & 1);
    return 1;
}

s32 func_0011C150(void) {
    s32 v0 = func_0010D428(0);
    s32 val = effMiscRandMod(0, v0 * 2);

    func_0010D608((f32)(val - v0 + 100) / 100.0f);
    return 1;
}

s32 func_0011C1B8(void) {
    s32 available = func_001A1438();
    s32 value;
    if (available) {
        s32 choice = func_0010D428(0);
        value = func_001A93B8(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    func_0010D5F0(value);
    return 1;
}

s32 func_0011C208(void) {
    s32 available = func_001A1438();
    s32 value;
    if (available) {
        s32 choice = func_0010D428(0);
        value = func_001A92D0(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    func_0010D5F0(value);
    return 1;
}

s32 func_0011C258(void) {
    s32 available = func_001A1438();
    s32 value;
    if (available) {
        s32 choice = func_0010D428(0);
        value = func_001A94A0(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    func_0010D5F0(value);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C2A8);

s32 func_0011C310(void) {
    s32 v0 = func_001A1438();
    f32 val = 0.0f;

    if (v0 != 0) {
        val = func_001A4598();
    }
    func_0010D608(val);
    return 1;
}

s32 func_0011C350(void) {
    s32 v0 = func_001A1438();
    s32 val = 0;

    if (v0 != 0) {
        val = func_001A4630();
    }
    func_0010D5F0(val);
    return 1;
}

void func_0011C390(void) {
    func_0010D608(*(f32 *)(D_003BAA6C + *(u16 *)(D_003C2E78[0] + 0x14) * 4 - 4));
}

void func_0011C3C0(void) {
    func_0010D608(*(f32 *)(D_003BAA6C + *(u16 *)(D_003C2E78[0] + 0x14) * 4 + 0x188));
}

extern s32 func_001190B0(s32);

void evtSelectStatGrade(void) {
    s32 total = func_001190B0(((EvtScriptContext *)D_003C2E70)->second);
    s32 current = *(u16 *)(((EvtScriptContext *)D_003C2E70)->second + 6);
    s32 percent = (s32)((f32)current / (f32)total * 100.0f);
    s32 grade = 0;

    if (percent != 100) {
        grade = 1;
        if (percent < 80) {
            grade = 2;
            if (percent < 60) {
                grade = 3;
                if (percent < 40) {
                    grade = 4;
                    if (percent < 30) {
                        grade = 5;
                        if (percent < 20) {
                            grade = percent >= 10 ? 6 : 7;
                        }
                    }
                }
            }
        }
    }
    func_0010D608(*(f32 *)(D_003BAA6C + grade * 4 + 0x318));
}

void func_0011C4C0(void) {
    func_0010D608(*(f32 *)(D_003BAA6C + *(u16 *)(D_003C2E78[0] + 0x14) * 4 + 0x360));
}

void func_0011C4F0(void) {
    func_0010D608(*(f32 *)(D_003BAA6C + *(u16 *)(D_003C2E78[0] + 0x14) * 4 + 0x4ec));
}

void func_0011C520(void) {
    func_0010D608(*(f32 *)(D_003BAA6C + *(u16 *)(D_003C2E78[0] + 0x14) * 4 + 0x678));
}

void func_0011C550(void) {
    func_0010D608(*(f32 *)(D_003BAA6C + *(u16 *)(D_003C2E78[0] + 0x14) * 4 + 0x678));
}

void evtScriptSelectRandomValue(void) {
    s32 value;
    s32 roll;

    if ((*(u16 *)D_003C2E7C[0] & 0x20) == 0) {
        roll = effMiscRandMod(0, 0x20);
        value = roll != 0 ? 10 : 0x80;
    } else {
        roll = effMiscRandMod(0, 0x30);
        value = roll != 0 ? 0 : 0x80;
    }
    func_0010D5F0(value);
}

void func_0011C5D8(void) {
    func_0010D5F0(*(s16 *)(D_003BAA18 + *(u16 *)(D_003C2E78[0] + 4) * 20));
}

void evtSelectFineStatGrade(void) {
    s32 total = func_001190B0(((EvtScriptContext *)D_003C2E70)->second);
    s32 current = *(u16 *)(((EvtScriptContext *)D_003C2E70)->second + 6);
    s32 percent = (s32)((f32)current / (f32)total * 100.0f);
    s32 grade = 0;

    if (percent != 100) {
        grade = 1;
        if (percent < 90) {
            grade = 2;
            if (percent < 80) {
                grade = 3;
                if (percent < 70) {
                    grade = 4;
                    if (percent < 60) {
                        grade = 5;
                        if (percent < 50) {
                            grade = 6;
                            if (percent < 40) {
                                grade = 7;
                                if (percent < 30) {
                                    grade = 8;
                                    if (percent < 20) {
                                        grade = percent >= 10 ? 9 : 10;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    func_0010D608(*(f32 *)(D_003BAA6C + grade * 4 + 0x338));
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C700);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C790);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C990);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CAB0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CB90);

void dds3WorkClear(void) {
    s32 base = D_003BAA00;

    *(s32 *)(base + 0x1360) = 0;
    *(s16 *)(base + 0x1364) = 0;
    D_003BAAB8 = 0;
}

void func_0011CE30(void) {
}

void func_0011CE38(void) {
}

void func_0011CE40(void) {
}

void func_0011CE48(void) {
}

s32 func_0011CE50(void) {
    s32 val = func_0010D428(0);

    func_0010D5F0(func_0011A968(val) == 1);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA34);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA38);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA3C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA40);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA44);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA48);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA4C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA50);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA54);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA58);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA5C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA60);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA64);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA68);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA6C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA70);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA74);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA78);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA7C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA80);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA84);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA88);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA8C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA90);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA94);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA98);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAA9C);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAA0);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAA4);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAA8);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAAC);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAB4);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAB8);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAABC);

INCLUDE_SDATA(const s32, "game/code_00119900", D_003BAAC0);

