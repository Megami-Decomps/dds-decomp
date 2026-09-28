#include "common.h"

extern u32 D_004390D8;

extern u8 *D_004389B0;

extern u8 *D_004389AC;

extern u8 *D_004389A8;

extern u8 *D_004389A4;

extern u8 *D_004389A0;

extern u32 D_004390C8;

extern u32 D_004390CC;

extern u32 D_004390E4;

extern u32 D_004390E8;

extern u32 D_004390DC;

extern u32 D_004390E0;

extern u32 D_004390D0;

extern u32 D_004390D4;

extern void (*D_004389C4)(void);

extern u8 D_0045C870[];

extern u8 D_0045C880[];

extern void func_00320C88(u32);

extern void func_00321908(u32);

typedef struct ShortRecord {
    u8 kind;
    u8 pad[7];
} ShortRecord;

typedef struct ShortRecordList {
    s32 count;
    ShortRecord *records;
} ShortRecordList;

void func_003214D0(u32 arg0, s32 arg1);

u32 func_00321500(void) {
    u32 node = func_00320C28(0);
    *(void (**)(u32, s32))(node + 0x10) = func_003214D0;
    return node;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00321528);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321688);

INCLUDE_ASM(const s32, "game/code_00321500", func_003216A8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321798);

INCLUDE_ASM(const s32, "game/code_00321500", func_003218A0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321908);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321928);

INCLUDE_ASM(const s32, "game/code_00321500", func_003219F0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321A30);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321C60);

void func_00321E18(u32 first, u32 second) {
    memset(D_0045C870, 0, 16);
    *(u32 *)(D_0045C870 + 0) = first;
    *(u32 *)(D_0045C870 + 4) = second;
}

void func_00321E70(u32 first, u32 second) {
    memset(D_0045C880, 0, 16);
    *(u32 *)(D_0045C880 + 0) = first;
    *(u32 *)(D_0045C880 + 4) = second;
}

u8 *func_00321EC8(void) {
    return D_0045C870;
}

u8 *func_00321ED8(void) {
    return D_0045C880;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00321EE8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321F18);

void func_00321F78(u32 *list, u32 *record) {
    u32 flags = record[0];
    u32 count = list[2];
    record[0] = flags & ~1u;
    list[2] = count - 1;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00321F98);

INCLUDE_ASM(const s32, "game/code_00321500", func_003223F8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00322418);

INCLUDE_ASM(const s32, "game/code_00321500", func_00322438);

s32 func_00322480(s32 *arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 << 3;
    if (0 < arg1) {
        do {
            temp_v0 = *arg0;
            arg0 = arg0 + 2;
            arg1 = arg1 - 1;
            temp_v1 = temp_v1 + temp_v0 * 8;
        } while (arg1 != 0);
    }
    return temp_v1;
}

void func_003224B0(void) {
}

void func_003224B8(void) {
}

void func_003224C0(void) {
}

INCLUDE_ASM(const s32, "game/code_00321500", func_003224C8);

void func_003224E0(u32 arg0, u32 arg1) {
    D_004390D0 = arg0;
    D_004390D4 = arg1;
}

u8 *func_003224F0(u32 taggedIndex) {
    u16 index = taggedIndex;
    return (u8 *)D_004390D0 + index * 28;
}

void func_00322510(u32 arg0, u32 arg1) {
    D_004390DC = arg0;
    D_004390E0 = arg1;
}

u8 *func_00322520(u16 index) {
    return (u8 *)D_004390DC + index * 24;
}

void func_00322540(u32 arg0, u32 arg1) {
    D_004390E4 = arg0;
    D_004390E8 = arg1;
}

u8 *func_00322550(u8 index) {
    return (u8 *)D_004390E4 + index * 48;
}

ShortRecord *func_00322570(ShortRecordList *list) {
    s32 i;
    ShortRecord *record = list->records;
    for (i = 0; i < list->count; i++, record++) {
        if (record->kind == 0x40) {
            return record;
        }
    }
    return NULL;
}

ShortRecord *func_003225C0(ShortRecordList *list) {
    s32 i;
    ShortRecord *record = list->records;
    for (i = 0; i < list->count; i++, record++) {
        if (record->kind == 0x40) {
            return record;
        }
    }
    return NULL;
}

u32 func_00322610(u32 record) {
    u32 registry;
    u32 table;
    if ((*(u32 *)(record + 4) & 0xffff0000) != 0x2010000) {
        return 0;
    }
    registry = (u32)func_003224F0(*(u32 *)(record + 4));
    if (registry == 0) {
        return 0;
    }
    table = *(u32 *)(registry + 0xc);
    return *(u32 *)(table + 0x10) + *(s16 *)(record + 0x2c) * 16;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00322670);

INCLUDE_ASM(const s32, "game/code_00321500", func_003226D8);

void func_00322D08(u32 arg0, u32 arg1) {
    D_004390C8 = arg0;
    D_004390CC = arg1;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00322D18);

INCLUDE_ASM(const s32, "game/code_00321500", func_00322D50);

u32 func_00322D98(void) {
    return D_004390C8;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00322DA0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00322E18);

void func_00322F00(s32 arg0) {
    *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) & 0xfffffffe;
    if (*(s32 *)(arg0 + 0x3c) != 0) {
        func_00320C88(*(s32 *)(arg0 + 0x3c));
        *(u32 *)(arg0 + 0x3c) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00322F48);

INCLUDE_ASM(const s32, "game/code_00321500", func_003230A0);

INCLUDE_ASM(const s32, "game/code_00321500", func_003232A0);

INCLUDE_ASM(const s32, "game/code_00321500", func_003233E8);

INCLUDE_ASM(const s32, "game/code_00321500", func_003236B0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323748);

INCLUDE_ASM(const s32, "game/code_00321500", func_003238A0);

void func_00323918(u8 *arg0) {
    D_004389A0 = arg0;
}

void func_00323920(u8 *arg0) {
    D_004389A4 = arg0;
}

void func_00323928(u8 *arg0) {
    D_004389A8 = arg0;
}

void func_00323930(u8 *arg0) {
    D_004389AC = arg0;
}

void func_00323938(u8 *arg0) {
    D_004389B0 = arg0;
}

u32 func_00323940(s32 arg0, s32 arg1) {
    if (*(s16 *)(arg0 + 0x36) - arg1 < 1) {
        *(u16 *)(arg0 + 0x36) = 0;
        *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) | 4;
        return 1;
    }
    *(s16 *)(arg0 + 0x36) = *(s16 *)(arg0 + 0x36) - (s16)arg1;
    *(u32 *)(arg0 + 0x40) = *(u32 *)(arg0 + 0x40) | 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00323988);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323BB8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323DF0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324070);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324238);

u32 func_00324268(void) {
    return D_004390D8;
}

void func_00324270(u32 node) {
    memset((void *)node, 0, 0x48);
    func_003242D0(node, 0);
    *(u32 *)(node + 0x40) |= 0x4010;
    *(u32 *)(node + 4) = 0x1000000;
    *(u16 *)(node + 0x36) = 1;
    *(f32 *)(node + 0x18) = 1.5707963f;
    D_004390D8 = node;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_003242D0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324840);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324AC0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324B28);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324C98);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324D28);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324D50);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324DB8);
INCLUDE_SDATA(const s32, "game/code_00321500", D_004389A0);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389A4);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389A8);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389AC);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389B0);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389B4);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389B8);

