#include "common.h"

extern s32 D_003BAA00;
extern s32 D_003BAA18;
extern s32 D_003BAA1C;
extern s32 D_003BAA50;
extern s32 D_003BAA68;
extern s32 D_003BAA6C;
extern s32 D_003BAAB8;

typedef struct TableEntry32 {
    u16 unk0; /* 0x0 */
    u16 unk2; /* 0x2 */
} TableEntry32;

typedef struct Entry4 {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 pad3; /* 0x3 */
} Entry4;

typedef struct Entry1A4 {
    u16 unk0; /* 0x0 */
    u8 pad2[2]; /* 0x2 */
    u16 unk4; /* 0x4 */
    u8 pad6[14]; /* 0x6 */
    u16 unk14; /* 0x14 */
    u8 pad16[398]; /* 0x16 */
} Entry1A4;

extern TableEntry32 D_0032AEA8[];
extern Entry4 D_0032AEE8[];

extern u32 D_003BAAB4;

extern s32 D_003C2E70[];
extern s32 D_003C2E74[];
extern s32 D_003C2E78[];
extern s32 D_003C2E7C[];
extern s32 D_003C2E80[];

extern s32 func_0010BCA0(void);
extern s32 func_0010D428(s32 idx);
extern s32 func_0010D5F0(s32 arg0);
extern void func_0010D608(f32 arg0);
extern Entry1A4 *func_0011A598(s32 arg0);
extern void func_00119900(s32 arg0, s32 arg1);
extern void func_0011B528(Entry1A4 *arg0);
extern s32 func_00119368(s32 arg0, s32 arg1);
extern u8 func_001A1438(void);
extern f32 func_001A4598(void);
extern u32 func_001A4630(void);
extern void func_001A92D0(u32 arg0);
extern void func_001A93B8(u32 arg0);
extern void func_001A94A0(u32 arg0);
extern void *memset(void *dst, s32 c, u32 n);
extern s32 func_0021F600(s32 arg0);
extern s32 func_002E83F8(u32 arg0, u32 arg1);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119900);

INCLUDE_ASM(const s32, "game/code_00119900", func_001199B0);

INCLUDE_ASM(const s32, "game/code_00119900", func_00119A00);

u8 func_00119A68(s32 arg0) {
    if ((*(u16 *)arg0 & 0x20) == 0) {
        return 0;
    }
    return *(u8 *)(D_003BAA1C + *(u16 *)(arg0 + 4) * 76 + 4);
}

s32 func_00119AA8(s32 arg0) {
    Entry1A4 *p = (Entry1A4 *)(D_003BAA00 + 0xa60);
    s32 n = 0;

    do {
        if (p->unk0 & 1) {
            if (p->unk4 == arg0) {
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

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A038);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A0C0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A158);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A238);

u16 func_0011A568(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_003BAA68 + 2);
}

u16 func_0011A580(s32 arg0) {
    s32 temp = *(u16 *)(arg0 + 0x14);

    return temp < 100 ? temp : 99;
}

Entry1A4 *func_0011A598(s32 arg0) {
    Entry1A4 *p = (Entry1A4 *)(D_003BAA00 + 0xa60);
    s32 n = 0;

    do {
        if (p->unk4 != arg0) {
            n++;
        } else {
            if (p->unk0 & 1) {
                return p;
            }
            n++;
        }
        p++;
    } while (n < 5);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011A5E8);

u8 func_0011A968(s32 arg0) {
    return func_0011A598(arg0) != 0;
}

s32 func_0011A988(void) {
    Entry1A4 *p = (Entry1A4 *)(D_003BAA00 + 0xa60);
    s32 best = 0;
    s32 n = 4;

    do {
        if (p->unk0 & 1) {
            if (best < p->unk14) {
                best = p->unk14;
            }
        }
        p++;
        n--;
    } while (n >= 0);
    return best;
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
    *(u16 *)(arg0 + 0x52) = D_0032AEA8[*(u16 *)(arg0 + 4)].unk0;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B438);

void func_0011B4C8(void) {
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

void func_0011B640(void) {
    s32 off = 0;
    s32 n = 4;

    do {
        Entry1A4 *p = (Entry1A4 *)(D_003BAA00 + off + 0xa60);

        if (p->unk0 & 1) {
            func_0011B528(p);
        }
        off += 0x1a4;
        n--;
    } while (n >= 0);
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B6A8);

void func_0011B7B8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_002E83F8(0, 4);
    *(s32 *)(arg0 + 0x194) = 0x12 - temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B7F0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011B878);

void func_0011B908(void) {
    D_003BAAB4 = func_0010BCA0();
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

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BD78);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BE00);

s32 func_0011BE90(void) {
    func_0010D5F0(*(u8 *)(D_003BAA50 + D_003C2E74[0] * 56 + 0x2d));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BED0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BF50);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011BFE8);

s32 func_0011C070(void) {
    func_0010D5F0(*(s16 *)(D_003BAA50 + D_003C2E74[0] * 56 + 0x36));
    return 1;
}

s32 func_0011C0B0(void) {
    u16 bits = *(u16 *)D_003C2E70 | 1;

    *(u16 *)D_003C2E70 = bits;
    D_003C2E70[4] = func_0010D428(0);
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
    s32 val = func_002E83F8(0, v0 * 2);

    func_0010D608((f32)(val - v0 + 100) / 100.0f);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C1B8);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C208);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C258);

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

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C3F0);

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

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C580);

void func_0011C5D8(void) {
    func_0010D5F0(*(s16 *)(D_003BAA18 + *(u16 *)(D_003C2E78[0] + 4) * 20));
}

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C610);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C700);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C790);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011C990);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CAB0);

INCLUDE_ASM(const s32, "game/code_00119900", func_0011CB90);

void func_0011CE18(void) {
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
