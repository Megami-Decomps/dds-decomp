#include "common.h"

#define MNU_WORK_ACTIVE   1
#define MNU_WORK_UPDATED  2
#define MNU_WORK_FINISHED 4

typedef struct MenuWorkEntry {
    u8 pad00[4];
    u32 tag;        /* 0x04 */
    s32 unk08;      /* 0x08 */
    u8 pad0C[4];
    f32 x0;         /* 0x10 */
    f32 y0;         /* 0x14 */
    f32 scale0;     /* 0x18 */
    u8 pad1C[4];
    f32 x1;         /* 0x20 */
    f32 y1;         /* 0x24 */
    f32 scale1;     /* 0x28 */
    u8 pad2C[8];
    u16 unk34;      /* 0x34 */
    u16 remaining;  /* 0x36: decreased until the completion flag is set */
    u8 pad38[4];
    u32 callback;   /* 0x3C */
    u32 flags;      /* 0x40 */
    u8 pad44[4];
} MenuWorkEntry; /* 0x48 */

extern u32 D_004390D8;

extern u8 *D_004389B0;

extern u8 *D_004389AC;

extern u8 *D_004389A8;

extern u8 *D_004389A4;

extern u8 *D_004389A0;

extern u32 D_004390C8;

extern s32 D_004390CC;

extern u32 D_004390E4;

extern u32 D_004390E8;

extern u32 D_004390DC;

extern u32 D_004390E0;

extern u32 D_004390D0;

extern u32 D_004390D4;

extern void (*D_004389C4)(void);

extern u8 D_0045C870[];

extern u8 D_0045C880[];

extern void dds3DestroyCallbackNodeAfterLastNotification(u32);

extern void func_00321908(u32);
extern u32 func_0035A828(s32 bytes);
extern u8 *func_00321238(void);
extern u8 *mnuGetResourceRecordByIndex(s32 index);
extern MenuWorkEntry *mnuFindUnusedWorkEntry(void);
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

typedef struct MenuCallbackNode {
    u8 pad00[0x10];
    void (*callback)(u32, s32);
} MenuCallbackNode;

typedef struct MenuWordPair {
    u32 first;
    u32 second;
    u8 pad08[8];
} MenuWordPair;

typedef struct MenuTaggedRecord {
    u8 pad00[4];
    u32 tag;          /* 0x04 */
    u8 pad08[0x24];
    s16 recordIndex;  /* 0x2C: indexes 16-byte records */
} MenuTaggedRecord;

typedef struct MenuRegistry {
    u8 pad00[0xC];
    u32 *table; /* 0x0C */
} MenuRegistry;

typedef struct MenuRegistryTable {
    u32 flags;
    u8 pad04[0xC];
    u32 recordBase; /* 0x10 */
} MenuRegistryTable;

void func_003214D0(u32, s32);
s32 dds3MeasureRecordBlock(s32 *entries, s32 count);

u32 mnuCreateReleaseCallbackNode(void) {
    MenuCallbackNode *node = (MenuCallbackNode *)mnuCreateCallbackNode(0);
    node->callback = func_003214D0;
    return (u32)node;
}
INCLUDE_ASM(const s32, "game/code_00321500", func_00321528);

void func_00321688(u32 left, u32 right, u32 value, u32 count) {
    func_003216A8(left, right, value, count, 1);
}


INCLUDE_ASM(const s32, "game/code_00321500", func_003216A8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321798);

/* Allocate a zeroed 0x22-byte record with an eight-byte tag at offset 0xA. */
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

typedef struct MenuStateRecord {
    u16 flags;      /* 0x00 */
    u8 pad02[4];
    s16 busy;       /* 0x06 */
    s16 value;      /* 0x08 */
    u8 pad0A[0xC];
    s16 limit;      /* 0x16 */
} MenuStateRecord;

s32 mnuCanAdvanceIdleStateRecord(MenuStateRecord *record) {
    if (record->busy == 0 && (!(record->flags & 2) || record->limit > record->value)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00321A30);

INCLUDE_ASM(const s32, "game/code_00321500", func_00321C60);

void func_00321E18(u32 first, u32 second) {
    memset(D_0045C870, 0, 16);
    ((MenuWordPair *)D_0045C870)->first = first;
    ((MenuWordPair *)D_0045C870)->second = second;
}

void func_00321E70(u32 first, u32 second) {
    memset(D_0045C880, 0, 16);
    ((MenuWordPair *)D_0045C880)->first = first;
    ((MenuWordPair *)D_0045C880)->second = second;
}

u8 *func_00321EC8(void) {
    return D_0045C870;
}

u8 *func_00321ED8(void) {
    return D_0045C880;
}

void mnuClearPackedMenuRecordBlock(u32 *record) {
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


/* Include the 0x10-byte header and both variable-length record blocks. */
s32 dds3MeasureMenuRecord(MenuLengthData *data) {
    s32 length = dds3MeasureRecordBlock(data->firstRecords, data->firstCount) + 0x10;
    return length + dds3MeasureRecordBlock(data->secondRecords, data->secondCount);
}

/* Each entry has an eight-byte header followed by its eight-byte subentries. */
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

extern u16 D_0040B248[];

u16 *func_003224C8(s32 index) {
    return &D_0040B248[index];
}

void mnuBindMenuRecordRegistry(u32 records, u32 count) {
    D_004390D0 = records;
    D_004390D4 = count;
}

u8 *mnuGetMenuRecordRegistryEntry(u32 taggedIndex) {
    u16 index = taggedIndex;
    return (u8 *)D_004390D0 + index * 28;
}

void func_00322510(u32 records, u32 count) {
    D_004390DC = records;
    D_004390E0 = count;
}

u8 *func_00322520(u16 index) {
    return (u8 *)D_004390DC + index * 24;
}

void func_00322540(u32 records, u32 count) {
    D_004390E4 = records;
    D_004390E8 = count;
}

u8 *func_00322550(u8 index) {
    return (u8 *)D_004390E4 + index * 48;
}

ShortRecord *mnuFindFirstFixedKindShortRecord(ShortRecordList *list) {
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

u32 mnuResolveTaggedRegistryRecord(u32 record) {
    u32 registry;
    u32 table;
    if ((((MenuTaggedRecord *)record)->tag & 0xffff0000) != 0x2010000) {
        return 0;
    }
    registry = (u32)mnuGetMenuRecordRegistryEntry(((MenuTaggedRecord *)record)->tag);
    if (registry == 0) {
        return 0;
    }
    table = (u32)((MenuRegistry *)registry)->table;
    return ((MenuRegistryTable *)table)->recordBase + ((MenuTaggedRecord *)record)->recordIndex * 16;
}

typedef struct ShortRecordList2 {
    s32 count;         /* 0x00 */
    u8 *records;       /* 0x04 */
} ShortRecordList2;

u8 *mnuFindMarkedShortListRecord(ShortRecordList2 *list) {
    s32 i;
    u8 *record;

    if (list != NULL) {
        record = list->records;
        if (record == NULL) {
            return NULL;
        }
        for (i = 0; i < list->count; i++) {
            if ((*record & 0xF0) == 0x10) {
                return record;
            }
            record += 8;
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_003226D8);

void mnuRegisterWorkEntryPool(u32 entries, u32 count) {
    D_004390C8 = entries;
    D_004390CC = count;
}

void mnuResetWorkEntryPool(void) {
    if (D_004390C8 != 0) {
        memset((void *)D_004390C8, 0, D_004390CC * 72);
    }
}


MenuWorkEntry *mnuFindUnusedWorkEntry(void) {
    s32 i = 0;
    MenuWorkEntry *entry;

    if (D_004390CC > 0) {
        entry = (MenuWorkEntry *)D_004390C8;
        do {
            if (!(entry->flags & MNU_WORK_ACTIVE)) {
                return entry;
            }
            entry++;
            i++;
        } while (i < D_004390CC);
    }
    return NULL;
}

u32 mnuGetWorkEntryPool(void) {
    return D_004390C8;
}

/* A flagged registry entry offsets its base value by the running clock. */
f32 mnuEvaluateTimedValue(MenuWorkEntry *entry) {
    u8 *registry = mnuGetMenuRecordRegistryEntry(entry->tag);
    if ((((MenuRegistryTable *)((MenuRegistry *)registry)->table)->flags & 1) != 0) {
        u8 *clock = func_00321238();
        u8 *segment = mnuGetResourceRecordByIndex(entry->unk08);
        return entry->y0 +
            (f32)((s32)*(u16 *)(clock + 2) - *(s32 *)(segment + 0xc));
    }
    return entry->y0;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00322E18);

void mnuDeactivateWorkEntry(MenuWorkEntry *entry) {
    entry->flags = entry->flags & 0xfffffffe;
    if (entry->callback != 0) {
        dds3DestroyCallbackNodeAfterLastNotification(entry->callback);
        entry->callback = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00322F48);

INCLUDE_ASM(const s32, "game/code_00321500", func_003230A0);

INCLUDE_ASM(const s32, "game/code_00321500", func_003232A0);

INCLUDE_ASM(const s32, "game/code_00321500", func_003233E8);

INCLUDE_ASM(const s32, "game/code_00321500", func_003236B0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323748);

/* Walk allocated entries; only entries carrying the active bit are visited. */
void mnuVisitActiveRecords(s32 context) {
    s32 index = 0;
    if ((s32)D_004390CC > 0) {
        s32 offset = 0;
        do {
            MenuWorkEntry *entry = (MenuWorkEntry *)(D_004390C8 + offset);
            if ((entry->flags & MNU_WORK_ACTIVE) != 0) {
                func_00323748(entry, context);
            }
            index++;
            offset += 0x48;
        } while (index < (s32)D_004390CC);
    }
}

void func_00323918(u8 *records) {
    D_004389A0 = records;
}

void func_00323920(u8 *records) {
    D_004389A4 = records;
}

void func_00323928(u8 *records) {
    D_004389A8 = records;
}

void func_00323930(u8 *records) {
    D_004389AC = records;
}

void func_00323938(u8 *records) {
    D_004389B0 = records;
}

u32 mnuAdvanceWorkEntry(MenuWorkEntry *entry, s32 elapsed) {
    if ((s16)entry->remaining - elapsed < 1) {
        entry->remaining = 0;
        entry->flags = entry->flags | MNU_WORK_FINISHED;
        return 1;
    }
    entry->remaining = (s16)entry->remaining - (s16)elapsed;
    entry->flags = entry->flags | MNU_WORK_UPDATED;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00323988);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323BB8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323DF0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324070);

extern char D_0045C890[12];

/* Copy the 12-byte resource progress parameter block. */
void func_00324238(u8 *src) {
    memcpy(D_0045C890, src, sizeof(D_0045C890));
}

u32 mnuGetActiveEffectWorkEntry(void) {
    return D_004390D8;
}

void mnuInitializeActiveEffectWorkEntry(u32 node) {
    memset((void *)node, 0, 0x48);
    func_003242D0(node, 0);
    ((MenuWorkEntry *)node)->flags |= 0x4010;
    ((MenuWorkEntry *)node)->tag = 0x1000000;
    ((MenuWorkEntry *)node)->remaining = 1;
    ((MenuWorkEntry *)node)->scale0 = 1.5707963f;
    D_004390D8 = node;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_003242D0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324840);

void mnuInitializeEffectContext(MenuWorkEntry *context) {
    MenuInitialTag tag;
    /* Retail only initializes bytes 1 through 7 of this tag. */
    tag.flags = 0;
    tag.group = 0;
    tag.kind = 2;
    tag.index = 0;
    memset(context, 0, 0x48);
    context->callback = mnuCreateReleaseCallbackNode();
    func_00320CE0(context->callback, 0,
                   (u32)mnuCreateNamedRecord((u8 *)&tag));
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00324B28);

u32 mnuCreateAnimatedEffect(u32 context, f32 x, f32 y, f32 progress) {
    u32 node = (u32)mnuFindUnusedWorkEntry();
    if (node != 0) {
        func_00322E18(node, context, 0, (s32)x, (s32)y, progress);
        ((MenuWorkEntry *)node)->flags |= 0x10;
    }
    return node;
}

void func_00324D28(u32 unused, u32 ptr) {
    if (ptr != 0) {
        func_0035A880(ptr);
    }
}


INCLUDE_ASM(const s32, "game/code_00321500", func_00324D50);

void mnuReleaseEffectPairAndNode(u32 node) {
    if (node != 0) {
        dds3DestroyCallbackNodeAfterLastNotification(*(u32 *)node);
        dds3DestroyCallbackNodeAfterLastNotification(*(u32 *)(node + 4));
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

