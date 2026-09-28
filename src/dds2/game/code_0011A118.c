#include "common.h"

extern s32 D_00435E38;

extern s32 dds3FindEntry(void);

extern u32 D_00435E88;

extern s32 D_00435DEC;

extern s32 D_00435DD0;

typedef struct Entry1A4 {
    u16 flags; /* 0x0 */
    u8 pad2[2]; /* 0x2 */
    u16 rosterIndex; /* 0x4 */
    u16 unk6; /* 0x6 */
    u8 pad8[6]; /* 0x8 */
    u16 unkE; /* 0xE */
    u8 pad10[4]; /* 0x10 */
    u16 unk14; /* 0x14 */
    u8 pad16[398]; /* 0x16 */
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

extern void func_0011A118(s32 arg0, s32 arg1);

extern void func_0011CA88(Entry1A4 *arg0);

extern s32 D_0043E5C8[];

extern s32 func_0010D818(s32 arg0);

extern s32 D_0043E5CC[];

extern s32 func_0010D650(s32 idx);

extern s32 func_00119A78(s32 arg0, s32 arg1);

extern s32 D_00435E20;

extern s32 D_0043E5C4[];

extern s32 D_0043E5C0[];

extern s32 D_0043E5D0[];

extern void func_0010D830(f32 arg0);

extern s32 effMiscRandMod(u32 arg0, u32 arg1);

extern u8 func_001AA308(void);

extern f32 func_001AD978(void);

extern u32 func_001ADA10(void);

extern s32 D_00435E8C;

extern s32 mdlFlagTest(s32 arg0);

extern s32 D_00435E3C;

extern s32 D_00386288[];

extern void func_002C5588(Entry1A4 *, s32);

extern s32 func_0011AEE0(s32);

extern void func_00314CE8(s32, s32);

extern s32 D_0043E5C0[];

extern void func_0011A808(Entry1A4 *, s32);

extern s32 func_00119AF8(Entry1A4 *, s32);

extern void func_0010C058(u32, s32);

extern void func_0010C2D8(u32);

extern s32 func_001B3918(u32 arg0);

extern s32 func_001B3830(u32 arg0);

extern s32 func_001B3A00(u32 arg0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A118);

INCLUDE_ASM(const s32, "game/code_0011A118", eventCheckValueThreshold);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A220);

u8 func_0011A288(s32 arg0) {
    if ((*(u16 *)arg0 & 0x20) == 0) {
        return 0;
    }
    return *(u8 *)(D_00435DEC + *(u16 *)(arg0 + 4) * 76 + 4);
}

INCLUDE_ASM(const s32, "game/code_0011A118", dds3FindEntryIndex);

s8 func_0011A318(s32 arg0) {
    return *(s8 *)(arg0 + D_00435DD0 + 0xa76);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A328);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A510);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A700);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A788);

void func_0011A7F8(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 0x10) = *(s32 *)(arg0 + 0x10) + arg1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011A808);

INCLUDE_ASM(const s32, "game/code_0011A118", eventUpdateFlaggedStats);

INCLUDE_ASM(const s32, "game/code_0011A118", eventHasMatchingFlaggedEntry);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AA58);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AB38);

u16 func_0011AE60(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_00435E38 + 2);
}

u16 dds3Clamp99(s32 arg0) {
    s32 temp = *(u16 *)(arg0 + 0x14);

    return temp < 100 ? temp : 99;
}

INCLUDE_ASM(const s32, "game/code_0011A118", dds3FindEntry);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011AEE0);

u8 func_0011B260(void) {
    s64 temp_v0;

    temp_v0 = dds3FindEntry();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0011A118", dds3EntryMax);

s32 func_0011B2C0(void) {
    s32 sum = 0;
    s32 count = 0;
    s32 remaining = 4;
    s32 entry = D_00435DD0 + 0xA60;
    do {
        remaining--;
        if ((*(u16 *)entry & 1) != 0) {
            count++;
            sum += *(u16 *)(entry + 0x14);
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

void func_0011C978(s32 arg0) {
    *(u16 *)(arg0 + 0x52) = D_00386248[*(u16 *)(arg0 + 4)].unk0;
}

void eventUpdateFlaggedEntries(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        Entry1A4 *entry = (Entry1A4 *)(D_00435DD0 + offset + 0xA60);
        if (entry->flags & 1) {
            s32 index = 0;
            do {
                if (entry->rosterIndex == index) {
                    func_0011C978(entry);
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

INCLUDE_ASM(const s32, "game/code_0011A118", dds3ForEachFlagged);

void func_0011D050(void) {
    s32 offset = 0;
    s32 remaining = 4;
    do {
        s32 entry = D_00435DD0 + offset + 0xA60;
        offset += 0x1C4;
        if ((*(u16 *)entry & 1) != 0) {
            func_00314CE8(entry, 0x5B);
            func_00314CE8(entry, 0x5C);
            func_00314CE8(entry, 0x5D);
        }
        remaining--;
    } while (remaining >= 0);
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D0D8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D130);

void func_0011D2A0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = effMiscRandMod(0, 4);
    *(s32 *)(arg0 + 0x1b4) = 0x12 - temp_v0;
}

s32 func_0011D2D8(void) {
    s32 changed = 0;
    s32 remaining;
    s32 entry;
    if (effMiscRandMod(0, 100) >= 51) {
        return 0;
    }
    remaining = 4;
    entry = D_00435DD0 + 0xA60;
    do {
        if ((*(u16 *)entry & 1) != 0 && *(u16 *)(entry + 6) != 0) {
            u16 flags = *(u16 *)(entry + 0xE);
            if ((flags & 0x5D0) != 0) {
                *(u16 *)(entry + 0xE) = flags & ~0x5D0;
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
    func_002C5588(entry, value);
    if (value != 0) {
        *(u8 *)(value + D_00435DD0 + 0x1340) = 1;
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

s32 eventRunContext(s32 script, s32 first, s32 second, s32 third, u16 flags) {
    func_0010C058(D_00435E88, script);
    D_0043E5C0[1] = third;
    D_0043E5C0[2] = first;
    D_0043E5C0[3] = second;
    *(u16 *)((u8 *)D_0043E5C0 + 0x14) = flags;
    *(u16 *)D_0043E5C0 &= 0xFFFE;
    func_0010C2D8(D_00435E88);
    return D_0043E5C0[4];
}

INCLUDE_ASM(const s32, "game/code_0011A118", dds3WorkInit);

u32 func_0011D588(void) {
    return D_00435E88;
}

void func_0011D590(void) {
    func_0010BF48(D_00435E88);
    D_00435E88 = 0;
}

s32 func_0011D5B8(void) {
    func_0010D818(*(u16 *)(D_0043E5C8[0] + 0x14));
    return 1;
}

s32 func_0011D5E0(void) {
    func_0010D818(*(u16 *)(D_0043E5CC[0] + 0x14));
    return 1;
}

s32 func_0011D608(void) {
    func_0010D818(*(u16 *)(D_0043E5C8[0] + 6));
    return 1;
}

s32 func_0011D630(void) {
    func_0010D818(*(u16 *)(D_0043E5CC[0] + 6));
    return 1;
}

s32 func_0011D658(void) {
    func_0010D818(*(u16 *)(D_0043E5C8[0] + 8));
    return 1;
}

s32 func_0011D680(void) {
    func_0010D818(*(u16 *)(D_0043E5CC[0] + 8));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D6A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D758);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D808);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D8B0);

s32 func_0011D958(void) {
    s32 val = func_0010D650(0);

    func_0010D818(func_00119A78(D_0043E5C8[0], val));
    return 1;
}

s32 func_0011D990(void) {
    s32 val = func_0010D650(0);

    func_0010D818(func_00119A78(D_0043E5CC[0], val));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011D9C8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DA50);

s32 func_0011DAE0(void) {
    func_0010D818(*(u8 *)(D_00435E20 + D_0043E5C4[0] * 56 + 0x2d));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DB20);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DBA0);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DC38);

s32 func_0011DCC0(void) {
    func_0010D818(*(s16 *)(D_00435E20 + D_0043E5C4[0] * 56 + 0x36));
    return 1;
}

s32 func_0011DD00(void) {
    u16 bits = *(u16 *)D_0043E5C0 | 1;

    *(u16 *)D_0043E5C0 = bits;
    D_0043E5C0[4] = func_0010D650(0);
    return 1;
}

s32 func_0011DD40(void) {
    func_0010D818(D_0043E5D0[0]);
    return 1;
}

s32 func_0011DD68(void) {
    func_0010D818((((*(u16 *)D_0043E5C8[0]) >> 5) ^ 1) & 1);
    return 1;
}

s32 func_0011DDA0(void) {
    s32 v0 = func_0010D650(0);
    s32 val = effMiscRandMod(0, v0 * 2);

    func_0010D830((f32)(val - v0 + 100) / 100.0f);
    return 1;
}

s32 func_0011DE08(void) {
    s32 available = func_001AA308();
    s32 value;
    if (available) {
        s32 choice = func_0010D650(0);
        value = func_001B3918(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    func_0010D818(value);
    return 1;
}

s32 func_0011DE58(void) {
    s32 available = func_001AA308();
    s32 value;
    if (available) {
        s32 choice = func_0010D650(0);
        value = func_001B3830(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    func_0010D818(value);
    return 1;
}

s32 func_0011DEA8(void) {
    s32 available = func_001AA308();
    s32 value;
    if (available) {
        s32 choice = func_0010D650(0);
        value = func_001B3A00(choice ? 0x20 : 4);
    }
    else {
        value = 0;
    }
    func_0010D818(value);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011DEF8);

s32 func_0011DF60(void) {
    s32 v0 = func_001AA308();
    f32 val = 0.0f;

    if (v0 != 0) {
        val = func_001AD978();
    }
    func_0010D830(val);
    return 1;
}

s32 func_0011DFA0(void) {
    s32 v0 = func_001AA308();
    s32 val = 0;

    if (v0 != 0) {
        val = func_001ADA10();
    }
    func_0010D818(val);
    return 1;
}

extern s32 D_00435E44;

s32 func_0011DFE0(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = *(u16 *)(entry + 0x14);
    func_0010D830(*(f32 *)((D_00435E44 - 4) + index * 4));
    return 1;
}

s32 func_0011E018(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = *(u16 *)(entry + 0x14);
    func_0010D830(*(f32 *)(D_00435E44 + index * 4 + 0x188));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E050);

s32 func_0011E128(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = *(u16 *)(entry + 0x14);
    func_0010D830(*(f32 *)(D_00435E44 + index * 4 + 0x360));
    return 1;
}

s32 func_0011E160(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = *(u16 *)(entry + 0x14);
    func_0010D830(*(f32 *)(D_00435E44 + index * 4 + 0x4EC));
    return 1;
}

s32 func_0011E198(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = *(u16 *)(entry + 0x14);
    func_0010D830(*(f32 *)(D_00435E44 + index * 4 + 0x678));
    return 1;
}

s32 func_0011E1D0(void) {
    s32 entry = D_0043E5C8[0];
    u32 index = *(u16 *)(entry + 0x14);
    func_0010D830(*(f32 *)(D_00435E44 + index * 4 + 0x678));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E208);

extern s32 D_00435DE8;

s32 func_0011E268(void) {
    u16 index = *(u16 *)(D_0043E5C8[0] + 4);
    func_0010D818(*(s16 *)(D_00435DE8 + index * 20));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E2A8);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E3A0);

s32 func_0011E430(void) {
    s32 entry = D_0043E5C8[0];
    s32 result;
    if (!(*(u16 *)entry & 0x20)) {
        result = *(s32 *)(D_00435DD0 + 0x3C);
    } else {
        s32 index = *(u16 *)(entry + 4);
        result = *(s32 *)(D_00435DEC + index * 76 + 0x28);
    }
    func_0010D818(result);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E498);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E528);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E728);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E848);

INCLUDE_ASM(const s32, "game/code_0011A118", func_0011E930);

INCLUDE_ASM(const s32, "game/code_0011A118", dds3WorkClear);

void func_0011EBE0(void) {
}

void func_0011EBE8(void) {
}

void func_0011EBF0(void) {
}

void func_0011EBF8(void) {
}

s32 func_0011EC00(void) {
    s32 value = func_0010D650(0);
    s32 state;
    if (func_0010D650(1) == 0) {
        state = func_0011B260();
    } else {
        state = func_0011AEE0(value);
    }
    func_0010D818(state == 1);
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

