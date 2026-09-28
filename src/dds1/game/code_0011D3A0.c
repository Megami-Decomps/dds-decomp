#include "common.h"
#include "fpu.h"

extern u32 D_003BABE0;

extern s32 D_003BABD8;

extern u32 D_003BAC00;

extern u32 D_003BABF8;
extern s32 D_003BABF0;

extern u32 D_003BABD0;

extern u64 func_0010FDC0(void);
extern s64 func_00110400(u64);
extern u64 func_00110458(u64);
extern s64 func_001104B0(u64);
extern u64 func_00110AB0(u64, u64);
extern s64 func_00113E30(u64);

extern s32 D_003BAA00;

extern u64 func_002D3FD0(u64);
extern u32 D_003BABDC;
extern s32 D_003BAB08;
extern s32 D_003BAB0C;
extern s32 D_003BAB10;
extern s32 D_003BAB14;
extern s32 D_003BAB18;
extern s32 D_003BAB1C;
extern s32 D_003BAB20;
extern u32 D_0032E3B0[];
extern s32 D_0032E4DC[];
extern u8 D_00324F88[];
extern u8 D_003257F8[];
extern u8 D_00324530[];
extern char D_0039FA00[];
extern char D_0039FCA0[];
extern char D_0039FCB0[];
extern char D_0039FCC8[];
extern void func_0011D998(f32 *arg0, u8 *arg1);
extern void func_0011DAC0(u32, u32, u32, u32, u32, u32, u8 *);
extern void func_0011DC70(u32 *arg0, s32 arg1, u8 *arg2);
extern void func_0011E540(void);
extern void func_0012E6F0(void);
extern void func_0013E5A8(u32 arg0);
extern u32 func_001462D0(void);
extern s32 func_0010BED8(const char *arg0);
extern void func_002D8C88(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern u32 D_003BAB34;
extern u32 D_003BAB54;
extern u32 D_0032E498[];
extern u32 D_003BAB58;
extern u32 D_0032E3D0[];
extern u32 D_003BABE8;
extern u32 D_003BAB38;
extern void func_0010F770(u32);
extern void func_0010F788(u32);
extern void func_001278E8(s32, s32);
extern s32 func_00127B98(void);
extern void func_00127B30(void);
extern s32 func_00127B88(void);
extern void func_00127AC0(void);
extern s32 func_002D3EE8(void);
extern s32 func_00102A60(void);
extern s32 func_00102AB0(void);
extern void func_0013F100(s32, u32);
extern u32 D_0032E4EC[];
extern void func_002D0A10(u32 arg0);
extern s32 D_003BABEC;
extern u8 D_0034C8F0[];
extern void func_00148C98(s32);
extern void func_0021F580(s32);
extern void func_0011B150(s32);
void func_001260A8(void);
extern u32 D_0032F1A0[];
extern s16 D_0032DDB0[];
extern u8 D_0033F068[];
extern u8 D_00342868[];
extern void *func_0010FA80(void);
extern void *kwlnTaskGetTaskByName(const char *);
extern void func_0011CE18(void);
extern char D_0039FBC0[]; /* "fldProcSequence" */
extern void *func_00113CE8(void *, u32 *, u32 *);
extern void func_0010FA98(void *, const char *);
extern char D_0039FD50[]; /* "FLD_DMY_MATTER" */
extern u8 D_003BAB3C;
extern u8 D_0032C9A0[];
extern s16 D_0032C9B0[];
extern u8 D_00346068[];
extern u32 func_00272228(void);
void func_001244D0(void);
void func_001246C8(void);
void func_00124788(void);
void func_00124850(void);
void func_00125DE0(u32 arg0);
extern u32 D_003BAB64;
extern u32 D_003BAB60;
extern u32 D_003BAB5C;
extern u32 func_002D03F8(u32);
extern void *func_002D03F0(u32);
extern void func_00218B48(s32, s32, s32, void *, u32);
void func_001238B8(void);
u8 func_00125DF8(u32 arg0);

u32 *func_00123DD0(void);

void func_0011D3A0(u32 *arg0, u32 arg1, u32 arg2) {
    arg0[4] = arg1;
    arg0[5] = arg2;
}

u64 func_0011D3B0(void) {
    u64 temp_v0;

    temp_v0 = func_002D3FD0(0x20);
    func_002D4010(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D3E8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D570);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D6B0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011D998);

void func_0011DAA0(f32 *arg0) {
    func_0011D998(arg0, D_00324530);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DAC0);

void func_0011DC50(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    func_0011DAC0(a, b, c, d, e, f, D_00324530);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DC70);

void func_0011DE00(u32 *arg0, s32 arg1) {
    func_0011DC70(arg0, arg1, D_00324530);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DE20);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011DEB0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E080);

u32 func_0011E278(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E280);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E540);

void func_0011E668(void) {
    kwlnTaskCreate((s32)D_0039FA00, 0x2AF8, 0, 0, (s32)func_0011E540, 0, 0);
}

void func_0011E6A0(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0039FA00, 1);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E6C0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E810);

void func_0011E960(void) {
    D_003BAB20 = 0;
    D_003BAB08 = -999;
    D_003BAB0C = -999;
    D_003BAB10 = -999;
    D_003BAB14 = -999;
    D_003BAB18 = -999;
    D_003BAB1C = -999;
    func_0012E6F0();
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011E998);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA00);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011EA10);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011EBC8);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FA28);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_0011ECC8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120968);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120AE8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120C08);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120EC8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00120FA0);

u8 func_00121048(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1370);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

void func_001210A0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1372);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1372);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121148(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1372);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

void func_001211A0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1374);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1374);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121248(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1374);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

void func_001212A0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1376);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1376);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121348(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1376);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

void func_001213A0(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1378);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x1378);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_00121448(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x1378);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001214A0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001214F0);

void func_00121550(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138A);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138A);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001215F8(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x138A);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

void func_00121650(s32 map, s32 slot, s32 bit, s32 enabled) {
    if (map < 40) {
        s32 index = map % 100;
        if (enabled != 0) {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138C);
            *flags |= 1 << bit;
        } else {
            u16 *flags = (u16 *)(slot * 30 + index * 1920 + D_003BAA00 + 0x138C);
            *flags &= ~(1 << bit);
        }
    }
}

u8 func_001216F8(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    u8 *temp_v1;
    u32 temp_v2;

    if (arg0 < 0x28) {
        temp_v0 = arg0 % 100;
        temp_v1 = (u8 *)(arg1 * 30 + temp_v0 * 1920);
        temp_v1 += D_003BAA00;
        temp_v2 = *(u16 *)(temp_v1 + 0x138C);
        return (temp_v2 >> arg2) & 1;
    }
    return 0;
}

void func_00121750(s32 flagIndex) {
    s32 byteOffset = (flagIndex >> 3) + 0x15970;
    u8 *byte = (u8 *)(D_003BAA00 + byteOffset);
    u8 *entry;
    *byte |= 1 << (flagIndex & 7);
    func_00148C98(flagIndex);
    if ((u32)(flagIndex - 0xF0) < 16) {
        func_0021F580(flagIndex + 0x610);
    }
    entry = (u8 *)(flagIndex * 16 + (s32)D_0034C8F0);
    if (*(s32 *)entry == 2) {
        func_0011B150(*(s16 *)(entry + 4));
    }
}

u8 func_001217F0(u32 arg0) {
    return (*(u8 *)(((s32)arg0 >> 3) + D_003BAA00 + 0x15970) >> (arg0 & 7)) & 1;
}

s32 func_00121818(s32 x, s32 y) {
    u8 *records = D_0033F068;
    u8 *second = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < 512);
    return 0;
}

s32 func_00121870(s32 x, s32 y) {
    u8 *records = D_00342868;
    u8 *second = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < 512);
    return 0;
}

s32 func_001218C8(s32 x, s32 y) {
    u8 *records = D_00346068;
    u8 *second = records + 2;
    s32 index = 1;
    s32 offset = 28;
    do {
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return index;
        }
        index++;
        offset += 28;
    } while (index < 256);
    return 0;
}

s16 *func_00121920(s32 x, s32 y) {
    u8 *records = (u8 *)D_0032C9B0;
    u8 *second = records + 2;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return (s16 *)(offset + (s32)records);
        }
    } while (index < 640);
    return D_0032C9B0;
}

s32 func_00121970(s32 x, s32 y) {
    u8 *records = (u8 *)D_0032C9B0;
    u8 *second = records + 2;
    u8 *result = records + 6;
    s32 index = 1;
    do {
        s32 offset = index * 8;
        index++;
        if (x == *(s16 *)(offset + (s32)records) && y == *(s16 *)(offset + (s32)second)) {
            return *(s16 *)(offset + (s32)result);
        }
    } while (index < 640);
    return 0;
}

u32 func_001219C8(s32 x, s32 y) {
    s16 *entry = D_0032DDB0;
    s32 index = 0;
    s32 checked = 0;

    do {
        if (entry[0] == x && entry[1] == y) {
            return index;
        }
        index++;
        checked++;
        entry += 8;
    } while (checked < 0x280);
    return index;
}

s16 * func_00121A10(s32 arg0, s32 arg1) {
    s16 *temp_v0 = D_0032DDB0;
    s32 temp_v1 = 0;

    do {
        if (temp_v0[0] == arg0 && temp_v0[1] == arg1) {
            return temp_v0;
        }
        temp_v1++;
        temp_v0 += 8;
    } while (temp_v1 < 0x60);
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121A58);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121B88);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121DE0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00121ED8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122030);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122100);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FB18);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FB60);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122498);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122710);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001228D8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001229A8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122A00);

f32 func_00122BB0(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) {
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    return fsqrtf(dx * dx + dy * dy + dz * dz);
}

void func_00122BE0(s64 arg0) {
    u64 temp_v0;
    s64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_0010FDC0();
    temp_v0 = func_00110AB0(temp_v0, 6);
    temp_v1 = func_00110400(temp_v0);
    if (temp_v1 == 0) {
        return;
    }
    func_00110490(temp_v0);
    do {
        temp_v2 = func_00110458(temp_v0);
        temp_v1 = func_00113E30(temp_v2);
        if (temp_v1 == 4) {
            if (arg0 == 0) {
                func_00113E20(temp_v2, 3);
            }
            else {
                func_00113E20(temp_v2, 0);
            }
        }
        temp_v1 = func_001104B0(temp_v0);
    } while (temp_v1 != 0);
    func_0010FF80(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122CB8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122D60);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00122E08);

void func_00122ED0(void) {
    u32 *buffer = D_0032F1A0;
    __asm__ volatile(
        ".set noreorder\n"
        "sqc2 vf0, 0(%0)\n"
        ".set reorder"
        : : "r"(buffer) : "memory");
    buffer += 4;
    __asm__ volatile(
        ".set noreorder\n"
        "sqc2 vf0, 0(%0)\n"
        ".set reorder"
        : : "r"(buffer) : "memory");
    D_003BAB34 = 0;
    *func_00123DD0() = 0;
}

typedef struct FieldSequenceRecord {
    u8 unk_00[0x30];
    u32 unk_30;
    u32 unk_34;
    u32 unk_38;
    u32 unk_3c;
    char name[16];
    s32 stage;
    s32 kind;
    s32 enabled;
    s32 mode;
    u16 code;
    u16 unk_62;
    s32 link;
    u8 unk_68[8];
    char detail[16];
    char note[16];
    u32 unk_90;
} FieldSequenceRecord;

void func_00122F08(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (D_0032E3B0[4] == stage) {
            D_0032E3B0[8] = 1;
        } else {
            func_0011CE18();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->unk_90 = 0;
    D_003BAB3C = 0;
    D_0032C9A0[0] = 0;
}

void func_00122FF0(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (D_0032E3B0[4] == stage) {
            D_0032E3B0[8] = 1;
        } else {
            func_0011CE18();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 3;
    record->kind = kind;
    record->code = 0;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    memset(record->note, 0, sizeof(record->note));
    record->unk_90 = 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001230D0);

void func_001231D0(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, const char *subname) {
    if (kwlnTaskGetTaskByName(D_0039FBC0) != NULL) {
        if (D_0032E3B0[4] == stage) {
            D_0032E3B0[8] = 1;
        } else {
            func_0011CE18();
        }
    }
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->stage = stage;
    record->enabled = 1;
    record->mode = 2;
    record->kind = kind;
    record->code = code;
    record->unk_62 = 0;
    record->link = 0;
    memset(record->detail, 0, sizeof(record->detail));
    strcpy(record->note, subname);
    record->unk_90 = 0;
}

void func_001232C0(FieldSequenceRecord *record, s32 stage, s32 kind, const char *name,
                    s32 code, s32 link, const char *subname) {
    record->unk_30 = 0;
    record->unk_3c = 0;
    record->unk_38 = 0;
    strcpy(record->name, name);
    record->enabled = 1;
    record->stage = stage;
    record->kind = kind;
    record->code = code;
    record->link = link;
    record->mode = 2;
    record->unk_62 = 0;
    strcpy(record->detail, subname);
    memset(record->note, 0, sizeof(record->note));
    record->unk_90 = 0;
}

u32 func_00123378(void) {
    u32 *temp_v0 = D_0032F1A0;

    if (temp_v0[0xD] == 1) {
        return 1;
    }
    if (temp_v0[0x17] == 3) {
        return 3;
    }
    if (temp_v0[0x17] == 4) {
        return 4;
    }
    if (temp_v0[0x17] == 5) {
        return 5;
    }
    if (temp_v0[0x17] == 6) {
        return 2;
    }
    return temp_v0[0xD] != 0 ? 2 : 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001233D0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBC0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FBD0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001238B8);

void func_00123990(void) {
    if (D_003BAB54 != 0) {
        func_002D0A10(D_003BAB54);
        D_003BAB54 = 0;
        D_0032E4EC[0] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001239C8);

void func_00123C30(void) {
    void *source;
    void *buffer;
    func_001238B8();
    D_003BAB60 = D_003BAB64;
    D_003BAB58 = func_002D03F8(D_003BAB64);
    source = func_002D03F0(D_003BAB54);
    buffer = func_002D03F0(D_003BAB58);
    memcpy(buffer, source, D_003BAB60);
    D_003BAB5C = (u32)buffer;
    func_00218B48(2, 0, 0x101, buffer, D_003BAB60);
    func_002D0A10(D_003BAB58);
    D_003BAB58 = 0;
}

void func_00123CC0(void) {
    if (D_003BAB58 != 0) {
        func_002D0A10(D_003BAB58);
        D_003BAB58 = 0;
    }
    if (D_0032E3D0[0] == 0 && D_003BABE8 == 0) {
        func_00123990();
        func_001278E8(0, 0);
    }
}

void func_00123D20(void) {
    u32 *buffer;
    u64 active;
    if (D_003BAB34 != 0) {
        active = func_0010FDC0();
        buffer = D_0032F1A0;
        if (active != 0) {
            func_0010F770(D_003BAB34);
            __asm__ volatile(
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(buffer) : "memory");
            func_0010F788(D_003BAB34);
            __asm__ volatile(
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(buffer + 4) : "memory");
        }
        D_003BAB34 = 0;
        D_003BAB38 = 0;
        *func_00123DD0() = 0;
        func_00123CC0();
    }
}

void func_00123D98(void) {
    func_00138D88();
    func_0011E6A0();
    func_00123D20();
    func_0021FE38();
    func_00125F18();
}

u32 * func_00123DD0(void) {
    return &D_003BABD0;
}

u32 func_00123DE0(void) {
    return *func_00123DD0();
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123E00);

void func_00123EA8(void) {
    if (D_003BAB34 != 0) {
        func_00125DE0(0x40);
        func_001127C0(D_003BAB34, 0);
    }
    D_0032E498[0] = 4;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123EE8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00123FB8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001242B8);

u32 func_001243C0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001243C8);

void func_00124488(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 8) != 0) {
        func_001244D0();
        D_0032E3B0[3] |= 1;
    }
}

void func_001244D0(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~8;
    D_0032E3B0[3] &= ~1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124500);

s32 func_00124590(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 8) != 0) {
        return 1;
    }
    if (D_0032E4DC[0] != 0) {
        return 0;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001245C0);

void func_00124680(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 4) != 0) {
        func_001246C8();
        D_0032E3B0[3] |= 2;
    }
}

void func_001246C8(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~4;
    D_0032E3B0[3] &= ~2;
}

void func_001246F8(void) {
    *(s32 *)(D_003BAA00 + 0xA58) |= 4;
    D_0032E3B0[3] &= ~2;
}

u8 func_00124728(void) {
    s32 temp_v0 = *(s32 *)(D_003BAA00 + 0xA58);
    temp_v0 &= 4;
    return temp_v0 != 0;
}

void func_00124740(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 2) != 0) {
        func_00124788();
        D_0032E3B0[3] |= 4;
    }
}

void func_00124788(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~2;
    D_0032E3B0[3] &= ~4;
}

void func_001247B8(void) {
    *(s32 *)(D_003BAA00 + 0xA58) = (*(s32 *)(D_003BAA00 + 0xA58) | 2) & ~1;
    D_0032E3B0[3] &= ~4;
}

u8 func_001247F0(void) {
    s32 temp_v0 = *(s32 *)(D_003BAA00 + 0xA58);
    temp_v0 &= 2;
    return temp_v0 != 0;
}

void func_00124808(void) {
    if ((*(s32 *)(D_003BAA00 + 0xA58) & 1) != 0) {
        func_00124850();
        D_0032E3B0[3] |= 8;
    }
}

void func_00124850(void) {
    *(s32 *)(D_003BAA00 + 0xA58) &= ~1;
    D_0032E3B0[3] &= ~8;
}

void func_00124880(void) {
    *(s32 *)(D_003BAA00 + 0xA58) = (*(s32 *)(D_003BAA00 + 0xA58) | 1) & ~2;
    D_0032E3B0[3] &= ~8;
}

u8 func_001248B8(void) {
    s32 flags = *(s32 *)(D_003BAA00 + 0xA58);
    flags &= 1;
    if (flags == 0) return 0;
    return 1;
}

void func_001248D0(void) {
    func_002D8C88(D_003257F8, (s32)D_00324F88, (s32)(D_00324F88 + 0xC0), (s32)(D_00324F88 + 0x100), (s32)(D_00324F88 + 0xE0));
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124900);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_001249E0);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124E18);

u8 func_00124E90(void) {
    return func_0010BED8(D_0039FCA0) != 0;
}

u8 func_00124EB8(void) {
    return func_0010BED8(D_0039FCC8) != 0;
}

u8 func_00124EE0(void) {
    return func_0010BED8(D_0039FCB0) != 0;
}

u8 func_00124F08(void) {
    if (D_003BABEC > 0) {
        return 2;
    }
    if (func_00272228() != 0) {
        return 1;
    }
    return func_00125DF8(0x20) != 0 ? 0 : 3;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00124F58);

u8 func_00125140(void) {
    if (D_003BABF0 > 0) {
        return 2;
    }
    return func_001462D0() != 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125170);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125348);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC40);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC50);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC60);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC70);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC80);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FC90);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCA0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCB0);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FCC8);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldProcSequence);

INCLUDE_ASM(const s32, "game/code_0011D3A0", fldProcDraw);

void func_00125D90(u32 arg0) {
    u32 *flags = &D_003BABF8;
    *flags |= arg0;
}

void func_00125DA8(u32 arg0) {
    u32 *flags = &D_003BABF8;
    *flags &= ~arg0;
}

u8 func_00125DC0(u32 arg0) {
    return (D_003BABF8 & arg0) != 0;
}

void func_00125DD0(u32 arg0) {
    D_003BAC00 = D_003BAC00 | arg0;
}

void func_00125DE0(u32 arg0) {
    D_003BAC00 = D_003BAC00 & ~arg0;
}

u8 func_00125DF8(u32 arg0) {
    return (D_003BAC00 & arg0) != 0;
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125E08);

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00125F18);

u8 func_00125FD0(void) {
    if (D_0032E3D0[0] == 0 && func_00127B98() != 0) {
        func_00127B30();
        if (func_00127B88() == 1) return 0;
        func_00127AC0();
        return 0;
    }
    return func_002D3EE8() == 0;
}

void func_00126038(void) {
    func_0013EC10();
}

void func_00126050(void) {
    func_00141D18();
}

void func_00126068(void) {
    if (D_003BABD8 != 0) {
        func_001260A8();
        return;
    }
    func_0013EC68();
}

void func_00126098(u32 arg0, u32 arg1) {
    D_003BABD8 = arg0;
    D_003BABDC = arg1;
}

void func_001260A8(void) {
    if (D_003BABD8 == 0) return;
    if (func_00102A60() > 0) return;
    if (func_00102A60() < 0 && (func_00102AB0() & 1) != 0) return;
    func_0013F100(D_003BABD8, D_003BABDC);
    D_003BABD8 = 0;
}

void func_00126108(u32 arg0) {
    D_003BABE0 = arg0;
}

void func_00126110(void) {
    u32 temp_v0;

    temp_v0 = D_003BABE0;
    if (temp_v0 != 0) {
        func_0013E5A8(temp_v0);
        D_003BABE0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0011D3A0", func_00126140);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD30);

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD40);

void *func_00126198(void) {
    u32 args[8];
    void *matter;
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    args[4] = 0;
    args[5] = 0;
    args[6] = 0;
    args[7] = 0;
    matter = func_00113CE8(func_0010FA80(), args, args + 4);
    func_0010FA98(matter, D_0039FD50);
    return matter;
}

INCLUDE_RODATA(const s32, "game/code_0011D3A0", D_0039FD50);



INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAE0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAE8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAF0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAAF8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB00);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB08);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB0C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB10);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB14);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB18);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB1C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB20);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB24);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB28);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB2C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB30);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB34);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB38);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB3C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB40);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB50);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB54);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB58);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB5C);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB60);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB64);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB68);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB70);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB78);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB80);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB88);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB90);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAB98);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABA0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABA8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABB0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABB8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABC0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABD0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABD8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABDC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABE8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABEC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF0);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF4);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABF8);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BABFC);

INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAC00);


INCLUDE_SDATA(const s32, "game/code_0011D3A0", D_003BAC08);

