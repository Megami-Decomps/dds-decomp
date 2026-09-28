#include "common.h"

/* Natural memset version exceeded 0x38-byte retail span; likely call-shape mismatch. */

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
extern u32 func_0035A828(s32 bytes);
extern u8 *func_00321238(void);
extern u8 *func_00321328(s32 index);
extern u32 func_00322D50(void);
extern void func_00322E18(u32 node, u32 context, s32 mode, s32 x, s32 y,
                          f32 progress);

typedef struct ShortRecord {
    u8 kind;
    u8 pad[7];
} ShortRecord;

typedef struct ShortRecordList {
    s32 count;
    ShortRecord *records;
} ShortRecordList;

typedef struct MenuInitialTag {
    u8 reserved;
    u8 flags;
    u16 group;
    u16 kind;
    u16 index;
} MenuInitialTag;

typedef struct MenuLengthData {
    u8 pad0[4];
    u16 firstCount;
    u16 secondCount;
    s32 *firstRecords;
    s32 *secondRecords;
} MenuLengthData;

void func_003214D0(u32 arg0, s32 arg1);
s32 dds3MeasureRecordBlock(s32 *entries, s32 count);

u32 func_00321500(void) {
    u32 node = mnuCreateCallbackNode(0);
    *(void (**)(u32, s32))(node + 0x10) = func_003214D0;
    return node;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00321528);

void func_00321688(u32 left, u32 right, u32 value, u32 count) {
    func_003216A8(left, right, value, count, 1);
}


INCLUDE_ASM(const s32, "game/code_00321500", func_003216A8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321798);

u8 *mnuCreateNamedRecord(u8 *name) {
    u8 *record;
    if (name == 0) {
        return 0;
    }
    record = (u8 *)func_0035A828(0x22);
    memset(record, 0, 0x22);
    memcpy(record + 0xa, name, 8);
    return record;
}

void func_00321908(u32 ptr) {
    if (ptr != 0) {
        func_0035A880(ptr);
    }
}


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

void func_00321EE8(u32 *record) {
    memset((void *)record[0], 0, record[1] * 36);
}


INCLUDE_ASM(const s32, "game/code_00321500", func_00321F18);

void mnuDeactivateListRecord(u32 *list, u32 *record) {
    u32 flags = record[0];
    u32 count = list[2];
    record[0] = flags & ~1u;
    list[2] = count - 1;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00321F98);

void func_003223F8(void) {
    func_00321F98(D_0045C870);
}


void func_00322418(void) {
    func_00321F98(D_0045C880);
}


s32 dds3MeasureMenuRecord(MenuLengthData *data) {
    s32 length = dds3MeasureRecordBlock(data->firstRecords, data->firstCount) + 0x10;
    return length + dds3MeasureRecordBlock(data->secondRecords, data->secondCount);
}

s32 dds3MeasureRecordBlock(s32 *records, s32 count) {
    s32 entryCount;
    s32 length;

    length = count << 3;
    if (0 < count) {
        do {
            entryCount = *records;
            records = records + 2;
            count = count - 1;
            length = length + entryCount * 8;
        } while (count != 0);
    }
    return length;
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

void func_00322D18(void) {
    if (D_004390C8 != 0) {
        memset((void *)D_004390C8, 0, D_004390CC * 72);
    }
}


INCLUDE_ASM(const s32, "game/code_00321500", func_00322D50);

u32 func_00322D98(void) {
    return D_004390C8;
}

f32 mnuEvaluateTimedValue(u8 *entry) {
    u8 *registry = func_003224F0(*(u32 *)(entry + 4));
    if ((**(u32 **)(registry + 0xc) & 1) != 0) {
        u8 *clock = func_00321238();
        u8 *segment = func_00321328(*(s32 *)(entry + 8));
        return *(f32 *)(entry + 0x14) +
            (f32)((s32)*(u16 *)(clock + 2) - *(s32 *)(segment + 0xc));
    }
    return *(f32 *)(entry + 0x14);
}

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

void mnuVisitActiveRecords(s32 arg) {
    s32 index = 0;
    if ((s32)D_004390CC > 0) {
        s32 offset = 0;
        do {
            u8 *record = (u8 *)(D_004390C8 + offset);
            if ((*(u32 *)(record + 0x40) & 1) != 0) {
                func_00323748(record, arg);
            }
            index++;
            offset += 0x48;
        } while (index < (s32)D_004390CC);
    }
}

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

void mnuInitializeEffectContext(u8 *context) {
    MenuInitialTag tag;
    /* Retail only initializes bytes 1 through 7 of this tag. */
    tag.flags = 0;
    tag.group = 0;
    tag.kind = 2;
    tag.index = 0;
    memset(context, 0, 0x48);
    *(u32 *)(context + 0x3c) = func_00321500();
    func_00320CE0(*(u32 *)(context + 0x3c), 0,
                   (u32)mnuCreateNamedRecord((u8 *)&tag));
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00324B28);

u32 mnuCreateAnimatedEffect(u32 context, f32 x, f32 y, f32 progress) {
    u32 node = func_00322D50();
    if (node != 0) {
        func_00322E18(node, context, 0, (s32)x, (s32)y, progress);
        *(u32 *)(node + 0x40) |= 0x10;
    }
    return node;
}

void func_00324D28(u32 unused, u32 ptr) {
    if (ptr != 0) {
        func_0035A880(ptr);
    }
}


INCLUDE_ASM(const s32, "game/code_00321500", func_00324D50);

void func_00324DB8(u32 node) {
    if (node != 0) {
        func_00320C88(*(u32 *)node);
        func_00320C88(*(u32 *)(node + 4));
        func_0035A880(node);
    }
}

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389A0);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389A4);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389A8);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389AC);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389B0);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389B4);

INCLUDE_SDATA(const s32, "game/code_00321500", D_004389B8);

