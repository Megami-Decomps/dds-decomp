#include "common.h"
#include "mnu_work.h"

#define MNU_WORK_ACTIVE   1

#define MNU_STATE_COMPLETED    1
#define MNU_STATE_VALUE_GATED  2
#define MNU_STATE_WAIT_PENDING 8
#define MNU_REGISTRY_TAG_PREFIX 0x02010000


extern u32 mnuActiveEffectEntry;

extern MenuRuntimePairCallback D_004389B0;

extern MenuRuntimeWorkCallback D_004389AC;

extern MenuWorkCallback D_004389A8;

extern MenuWorkCallback D_004389A4;

extern MenuWorkCallback D_004389A0;

extern u32 mnuWorkEntryPool;

extern s32 mnuWorkEntryPoolCount;

extern MenuRegistryParameters *D_004390E4;

extern u32 D_004390E8;

extern u32 D_004390DC;

extern u32 D_004390E0;

extern MenuRegistry *D_004390D0;

extern u32 D_004390D4;

extern void (*sdfTickCallback)(void);

extern u8 D_0045C870[];

extern u8 D_0045C880[];

extern void dds3DestroyCallbackNodeAfterLastNotification(u32);

extern void mnuFreeOptionalBlock(u32);
extern u32 func_0035A828(s32 bytes);
extern u8 *mnuGetResourceProgressStepState(void);
extern u8 *mnuGetResourceRecordByIndex(s32 index);
extern MenuWorkEntry *mnuFindUnusedWorkEntry(void);
extern void func_00322E18(u32 node, u32 context, s32 mode, s32 x, s32 y,
                          f32 progress);
extern u32 mnuGetActiveEffectWorkEntry(void);

typedef struct ShortRecord {
    u8 kind;
    u8 pad01;
    s16 parameters[3];
} ShortRecord;

typedef struct ShortRecordList {
    s32 count;
    ShortRecord *records;
} ShortRecordList;

typedef struct MenuRegistryRecord {
    u8 pad00[4];
    u16 listCount; /* 0x04: short lists in this record */
    u8 pad06[2];
    ShortRecordList *lists;
    u8 pad0C[4];
} MenuRegistryRecord;


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

typedef struct MenuRuntimeList {
    MenuRuntimeRecord *records;
    s32 capacity;
    u32 activeCount;
    u32 unk0C;
} MenuRuntimeList;



struct MenuRegistryTable {
    u32 flags;
    u8 pad04[0xA];
    u16 recordCount; /* 0x0E */
    MenuRegistryRecord *recordBase; /* 0x10 */
};

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
u8 *mnuCreateNamedRecord(u8 *tagData) {
    u8 *record;
    if (tagData == 0) {
        return 0;
    }
    record = (u8 *)func_0035A828(0x22);
    memset(record, 0, 0x22);
    memcpy(record + 0xa, tagData, 8);
    return record;
}

void mnuFreeOptionalBlock(u32 ptr) {
    if (ptr != 0) {
        func_0035A880(ptr);
    }
}


typedef struct MenuStateRecord {
    union {
        u16 word;
        struct {
            u16 completed : 1;
        } bits;
    } flags;
    u8 pad02[2];
    u16 waitCount; /* 0x04 */
    s16 elapsedCount; /* 0x06: advances toward duration, then resets */
    s16 value;     /* 0x08 */
    u8 pad0A[4];
    s16 duration;  /* 0x0E */
    u8 pad10[4];
    s16 waitLimit; /* 0x14 */
    s16 limit;     /* 0x16 */
} MenuStateRecord;

/* Advance optional wait and active counters; completion stays latched. */
s32 mnuAdvanceTimedStateRecord(MenuStateRecord *record) {
    if (record->flags.bits.completed) {
        return 1;
    }
    if (record->flags.word & MNU_STATE_WAIT_PENDING) {
        /* A zero wait limit leaves the pending wait stalled. */
        if (record->waitLimit == 0) {
            return 0;
        }
        if (++record->waitCount < record->waitLimit) {
            return 0;
        }
        record->waitCount = 0;
        record->flags.word &= ~MNU_STATE_WAIT_PENDING;
        record->value = 0;
    }
    if (++record->elapsedCount >= record->duration) {
        record->elapsedCount = 0;
        if (!(record->flags.word & MNU_STATE_VALUE_GATED) || record->limit > record->value) {
            record->flags.word |= MNU_STATE_COMPLETED;
            return 1;
        }
        /* A failed value gate rearms the wait instead of marking completion. */
        record->waitCount = 0;
        record->flags.word |= MNU_STATE_WAIT_PENDING;
    }
    return 0;
}

/* Test the idle counter and value gate; the pending-wait bit is not checked. */
s32 mnuCanAdvanceIdleStateRecord(MenuStateRecord *record) {
    if (record->elapsedCount == 0 && (!(record->flags.word & MNU_STATE_VALUE_GATED) || record->limit > record->value)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00321A30);

extern MenuRuntimeRecord *func_00321F18(MenuRuntimeList *);
extern MenuRuntimeCallback D_0043899C;
extern f64 cos(f64);
extern f64 sin(f64);

MenuRuntimeRecord *func_00321C60(MenuRuntimeList *list, s32 x, s32 y, u8 kind,
                           s32 offsetX, s32 offsetY, s32 direction,
                           s16 speed, s16 remaining, f32 angle) {
    MenuRuntimeRecord *record = func_00321F18(list);

    if (record == NULL) {
        return NULL;
    }
    record->state.kind = kind;
    record->state.directionDegrees = direction;
    record->unk18 = 0.0f;
    record->unk1C = 0.0f;
    record->unk04 = x;
    record->unk08 = y;
    record->unk0C = offsetX * cos(angle + 1.5707963f) + offsetY * sin(angle + 1.5707963f);
    record->unk10 = offsetY * cos(angle + 1.5707963f) - offsetX * sin(angle + 1.5707963f);
    record->angle = angle;
    record->speed = speed;
    record->remaining = remaining;
    D_0043899C(record);
    return record;
}


void func_00321E18(MenuRuntimeRecord *records, s32 capacity) {
    memset(D_0045C870, 0, sizeof(MenuRuntimeList));
    ((MenuRuntimeList *)D_0045C870)->records = records;
    ((MenuRuntimeList *)D_0045C870)->capacity = capacity;
}

void func_00321E70(MenuRuntimeRecord *records, s32 capacity) {
    memset(D_0045C880, 0, sizeof(MenuRuntimeList));
    ((MenuRuntimeList *)D_0045C880)->records = records;
    ((MenuRuntimeList *)D_0045C880)->capacity = capacity;
}

MenuRuntimeList *func_00321EC8(void) {
    return (MenuRuntimeList *)D_0045C870;
}

MenuRuntimeList *func_00321ED8(void) {
    return (MenuRuntimeList *)D_0045C880;
}

void mnuClearPackedMenuRecordBlock(MenuRuntimeList *list) {
    memset(list->records, 0, list->capacity * sizeof(*list->records));
}


INCLUDE_ASM(const s32, "game/code_00321500", func_00321F18);

void mnuDeactivateListRecord(MenuRuntimeList *list, MenuRuntimeRecord *record) {
    u32 flags = record->state.word;
    u32 count = list->activeCount;
    record->state.word = flags & ~1u;
    list->activeCount = count - 1;
}

extern f32 mnuEvaluateTimedValue(MenuWorkEntry *);
extern f64 cos(f64);
extern f64 sin(f64);
extern s32 func_0035C200(void);

void func_00321F98(MenuRuntimeList *list) {
    MenuProgressParameters *parameters;
    MenuRuntimeRecord *record;
    MenuWorkEntry *work;
    s32 i;
    s32 x;
    s32 y;
    f32 angle;
    f32 pi;

    parameters = mnuGetResourceProgressParameters();
    record = list->records;
    for (i = 0; i < list->capacity; i++, record++) {
        if (record->state.word & MNU_WORK_ACTIVE) {
            if ((record->state.kind & 0xF) >= 4) {
                work = (MenuWorkEntry *)mnuGetActiveEffectWorkEntry();
                record->unk04 = work->x0;
                record->unk08 = mnuEvaluateTimedValue(work);
            }
            pi = 3.1415926f;
            angle = record->state.directionDegrees * pi / 180.0f +
                    record->angle + 1.5707963f;
            record->unk18 += record->speed * cos(angle);
            record->unk1C -= record->speed * sin(angle);
            x = record->unk18 + record->unk0C + record->unk04;
            y = record->unk1C + record->unk10 + record->unk08;
            if ((record->state.kind & 0xF) == 3) {
                if (x < 0.0f) {
                    x = 0;
                }
                if (x > parameters->width) {
                    x = parameters->width;
                }
                if (y < 0.0f) {
                    y = 0;
                }
                if (y > parameters->height) {
                    y = parameters->height;
                }
                if (x <= 0.0f) {
                    record->angle = (-135.0f + (func_0035C200() % 100) *
                                     90.0f / 100.0f) * pi / 180.0f;
                } else if (x >= parameters->width) {
                    record->angle = (135.0f - (func_0035C200() % 100) *
                                     90.0f / 100.0f) * pi / 180.0f;
                } else if (y <= 0.0f) {
                    record->angle = ((func_0035C200() % 100) *
                                     90.0f / 100.0f + 135.0f) * pi / 180.0f;
                } else if (y >= parameters->height) {
                    record->angle = (-45.0f + (func_0035C200() % 100) *
                                     90.0f / 100.0f) * pi / 180.0f;
                }
            } else if (x < -50.0f || x > parameters->width + 50.0f ||
                       y < -150.0f || y > parameters->height + 50.0f) {
                mnuDeactivateListRecord(list, record);
            }
            if (record->remaining > 0) {
                record->remaining--;
                if (record->remaining == 0) {
                    mnuDeactivateListRecord(list, record);
                }
            }
        }
    }
}

void func_003223F8(void) {
    func_00321F98((MenuRuntimeList *)D_0045C870);
}


void func_00322418(void) {
    func_00321F98((MenuRuntimeList *)D_0045C880);
}


/* Include the 0x10-byte header and both variable-length record blocks. */
s32 dds3MeasureMenuRecord(MenuLengthData *data) {
    s32 byteSize = dds3MeasureRecordBlock(data->firstRecords, data->firstCount) + 0x10;
    return byteSize + dds3MeasureRecordBlock(data->secondRecords, data->secondCount);
}

/* Each entry has an eight-byte header followed by its eight-byte subentries. */
s32 dds3MeasureRecordBlock(s32 *records, s32 count) {
    s32 subentryCount;
    s32 byteSize;

    byteSize = count << 3;
    if (0 < count) {
        do {
            subentryCount = *records;
            records = records + 2;
            count = count - 1;
            byteSize = byteSize + subentryCount * 8;
        } while (count != 0);
    }
    return byteSize;
}

void func_003224B0(MenuWorkEntry *entry, struct MnuShootingWork *context) {
}

void func_003224B8(MenuRuntimeRecord *record, MenuWorkEntry *entry, struct MnuShootingWork *context) {
}

void func_003224C0(MenuRuntimeRecord *record, MenuRuntimeRecord *other, struct MnuShootingWork *context) {
}

extern u16 D_0040B248[];

u16 *func_003224C8(s32 index) {
    return &D_0040B248[index];
}

void mnuBindMenuRecordRegistry(MenuRegistry *records, u32 count) {
    D_004390D0 = records;
    D_004390D4 = count;
}

MenuRegistry *mnuGetMenuRecordRegistryEntry(u32 taggedIndex) {
    u16 index = taggedIndex;
    return &D_004390D0[index];
}

void func_00322510(u32 records, u32 count) {
    D_004390DC = records;
    D_004390E0 = count;
}

u8 *func_00322520(u16 index) {
    return (u8 *)D_004390DC + index * 24;
}

void func_00322540(MenuRegistryParameters *records, u32 count) {
    D_004390E4 = records;
    D_004390E8 = count;
}

MenuRegistryParameters *func_00322550(u32 taggedIndex) {
    u8 index = taggedIndex;
    return &D_004390E4[index];
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

u32 mnuResolveTaggedRegistryRecord(u32 taggedRecord) {
    MenuRegistry *registryEntry;
    MenuRegistryTable *registryTable;
    if ((((MenuWorkEntry *)taggedRecord)->tag & 0xffff0000) != MNU_REGISTRY_TAG_PREFIX) {
        return 0;
    }
    registryEntry = mnuGetMenuRecordRegistryEntry(((MenuWorkEntry *)taggedRecord)->tag);
    if (registryEntry == 0) {
        return 0;
    }
    registryTable = registryEntry->table;
    return (u32)&registryTable->recordBase[((MenuWorkEntry *)taggedRecord)->recordIndex];
}

typedef struct MenuByteRecordList {
    s32 count;         /* 0x00 */
    u8 *records;       /* 0x04 */
} MenuByteRecordList;

u8 *mnuFindMarkedShortListRecord(MenuByteRecordList *list) {
    s32 recordIndex;
    u8 *record;

    if (list != NULL) {
        record = list->records;
        if (record == NULL) {
            return NULL;
        }
        for (recordIndex = 0; recordIndex < list->count; recordIndex++) {
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
    mnuWorkEntryPool = entries;
    mnuWorkEntryPoolCount = count;
}

void mnuResetWorkEntryPool(void) {
    if (mnuWorkEntryPool != 0) {
        memset((void *)mnuWorkEntryPool, 0, mnuWorkEntryPoolCount * 72);
    }
}


/* An entry is reusable whenever its active bit is clear, regardless of other flags. */
MenuWorkEntry *mnuFindUnusedWorkEntry(void) {
    s32 entryIndex = 0;
    MenuWorkEntry *entry;

    if (mnuWorkEntryPoolCount > 0) {
        entry = (MenuWorkEntry *)mnuWorkEntryPool;
        do {
            if (!(entry->flags & MNU_WORK_ACTIVE)) {
                return entry;
            }
            entry++;
            entryIndex++;
        } while (entryIndex < mnuWorkEntryPoolCount);
    }
    return NULL;
}

u32 mnuGetWorkEntryPool(void) {
    return mnuWorkEntryPool;
}

/* A flagged registry entry offsets its base value by accumulated resource progress. */
f32 mnuEvaluateTimedValue(MenuWorkEntry *entry) {
    MenuRegistry *registry = mnuGetMenuRecordRegistryEntry(entry->tag);
    if ((registry->table->flags & 1) != 0) {
        u8 *progressState = mnuGetResourceProgressStepState();
        u8 *resourceRecord = mnuGetResourceRecordByIndex(entry->unk08);
        return entry->y0 +
            (f32)((s32)*(u16 *)(progressState + 2) - *(s32 *)(resourceRecord + 0xc));
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

extern void func_003226D8(MenuWorkEntry *, ShortRecordList *, ShortRecord *);
extern s32 func_003230A0(MenuWorkEntry *, MenuRegistryTable *, MenuRegistryRecord *, ShortRecord *);
extern void func_00321528(u32, MenuRegistryRecord *);

/* Tick the packed countdown and dispatch the row's fixed-kind record. */
s32 mnuAdvanceRegistryWorkEntry(MenuWorkEntry *entry) {
    MenuRegistryTable *table;
    MenuRegistryRecord *row;
    ShortRecordList *list;
    ShortRecord *record;
    ShortRecord empty;

    table = mnuGetMenuRecordRegistryEntry(entry->tag)->table;
    row = &table->recordBase[entry->recordIndex];
    list = &row->lists[entry->shortListIndex];
    if (entry->control.bits.countdownEnabled) {
        if (entry->control.bits.countdown > 0) {
            entry->control.bits.countdown--;
            if (entry->control.bits.countdown == 0) {
                entry->control.bits.countdownEnabled = 0;
            }
        }
    }
    record = mnuFindFirstFixedKindShortRecord(list);
    if (record == NULL) {
        memset(&empty, 0, sizeof(empty));
        record = &empty;
    }
    func_003226D8(entry, list, record);
    entry->elapsed++;
    switch (func_003230A0(entry, table, row, record)) {
    case 1:
        func_00321528(entry->callback, &table->recordBase[entry->recordIndex]);
        return 0;
    case 2:
        entry->flags |= 0x8;
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00321500", func_003230A0);

void func_003232A0(MenuWorkEntry *entry, MenuByteRecordList *list) {
    ShortRecord *record;

    entry->control.bits.loopMode = 0;
    record = (ShortRecord *)mnuFindMarkedShortListRecord(list);
    if (record != NULL) {
        switch (record->kind) {
        case 0x11:
            entry->control.bits.loopMode = 1;
            if (record->parameters[1] == 0) {
                entry->control.bits.repeatMode = 1;
                entry->repeatCount = record->parameters[0];
            } else if (record->parameters[1] == 1) {
                entry->control.bits.repeatMode = 2;
                entry->repeatCount = record->parameters[0];
            } else if (record->parameters[1] == 2) {
                entry->control.bits.repeatMode = 3;
                entry->repeatCount = record->parameters[0];
            }
            break;
        case 0x12:
            entry->control.bits.loopMode = 2;
            if (record->parameters[1] == 0) {
                entry->control.bits.repeatMode = 1;
                entry->repeatCount = record->parameters[0];
                entry->unk34 = record->parameters[2];
            } else if (record->parameters[1] == 1) {
                entry->control.bits.repeatMode = 2;
                entry->repeatCount = record->parameters[0];
                entry->unk34 = record->parameters[2];
            } else if (record->parameters[1] == 2) {
                entry->control.bits.repeatMode = 3;
                entry->repeatCount = record->parameters[0];
                entry->unk34 = record->parameters[2];
            }
            break;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00321500", func_003233E8);

void mnuVisitActiveWorkAndEffectEntry(s32 context) {
    s32 entryIndex;

    for (entryIndex = 0; entryIndex < mnuWorkEntryPoolCount; entryIndex++) {
        MenuWorkEntry *entry = &((MenuWorkEntry *)mnuWorkEntryPool)[entryIndex];

        if (entry->flags & MNU_WORK_ACTIVE) {
            D_004389A8(entry, (struct MnuShootingWork *)context);
        }
    }
    D_004389A8((MenuWorkEntry *)mnuGetActiveEffectWorkEntry(), (struct MnuShootingWork *)context);
}

void func_00323748(MenuWorkEntry *entry, struct MnuShootingWork *context) {
    MenuRegistry *registry;

    if (entry->flagsBits.pendingStart) {
        D_004389A0(entry, context);
        entry->flagsBits.pendingStart = 0;
    }
    if (entry->flagsBits.updated) {
        entry->flagsBits.updated = 0;
        if (!entry->flagsBits.unk5) {
            if ((entry->tag & 0xFFFF0000) == 0x02010000) {
                registry = mnuGetMenuRecordRegistryEntry(entry->tag);
                if (entry->remaining <= (registry->unk08 >> 1)) {
                    entry->flagsBits.unk5 = 1;
                }
            }
        }
    }
    if (entry->flagsBits.finished) {
        if ((entry->tag & 0xFFFF0000) == 0x01000000) {
            entry->flags |= 0x4000;
            D_004389A4(entry, context);
        } else {
            entry->flagsBits.pendingDeactivate = 1;
        }
    }
    if (entry->flagsBits.pendingDeactivate) {
        D_004389A4(entry, context);
        entry->flagsBits.finished = 0;
        mnuDeactivateWorkEntry(entry);
    }
}


/* Walk allocated entries; only entries carrying the active bit are visited. */
void mnuVisitActiveRecords(s32 context) {
    s32 entryIndex = 0;
    if ((s32)mnuWorkEntryPoolCount > 0) {
        s32 byteOffset = 0;
        do {
            MenuWorkEntry *entry = (MenuWorkEntry *)(mnuWorkEntryPool + byteOffset);
            if ((entry->flags & MNU_WORK_ACTIVE) != 0) {
                func_00323748(entry, (struct MnuShootingWork *)context);
            }
            entryIndex++;
            byteOffset += 0x48;
        } while (entryIndex < (s32)mnuWorkEntryPoolCount);
    }
}

void func_00323918(MenuWorkCallback records) {
    D_004389A0 = records;
}

void func_00323920(MenuWorkCallback records) {
    D_004389A4 = records;
}

void mnuSetActiveWorkVisitor(MenuWorkCallback callback) {
    D_004389A8 = callback;
}

void func_00323930(MenuRuntimeWorkCallback records) {
    D_004389AC = records;
}

void func_00323938(MenuRuntimePairCallback records) {
    D_004389B0 = records;
}

/* Remaining is interpreted as signed 16-bit; updated/finished flags stay latched. */
u32 mnuAdvanceWorkEntry(MenuWorkEntry *entry, s32 elapsed) {
    if (entry->remaining - elapsed < 1) {
        entry->remaining = 0;
        entry->flagsBits.finished = 1;
        return 1;
    }
    entry->remaining = entry->remaining - (s16)elapsed;
    entry->flagsBits.updated = 1;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00323988);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323BB8);

INCLUDE_ASM(const s32, "game/code_00321500", func_00323DF0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324070);

extern char D_0045C890[12];

/* Copy the 12-byte resource progress parameter block. */
void mnuSetInputActionSnapshot(u8 *src) {
    memcpy(D_0045C890, src, sizeof(D_0045C890));
}

u32 mnuGetActiveEffectWorkEntry(void) {
    return mnuActiveEffectEntry;
}

void mnuInitializeActiveEffectWorkEntry(u32 entry) {
    memset((void *)entry, 0, 0x48);
    func_003242D0(entry, 0);
    ((MenuWorkEntry *)entry)->flags |= 0x4010;
    ((MenuWorkEntry *)entry)->tag = 0x1000000;
    ((MenuWorkEntry *)entry)->remaining = 1;
    ((MenuWorkEntry *)entry)->scale0 = 1.5707963f;
    mnuActiveEffectEntry = entry;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_003242D0);

INCLUDE_ASM(const s32, "game/code_00321500", func_00324840);

void mnuInitializeEffectContext(MenuWorkEntry *context) {
    MenuInitialTag initialTag;
    /* Retail only initializes bytes 1 through 7 of this tag. */
    initialTag.flags = 0;
    initialTag.group = 0;
    initialTag.kind = 2;
    initialTag.index = 0;
    memset(context, 0, 0x48);
    context->callback = mnuCreateReleaseCallbackNode();
    func_00320CE0(context->callback, 0,
                   (u32)mnuCreateNamedRecord((u8 *)&initialTag));
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00324B28);


u32 mnuCreateAnimatedEffect(u32 context, f32 x, f32 y, f32 progress) {
    u32 entry = (u32)mnuFindUnusedWorkEntry();
    if (entry != 0) {
        func_00322E18(entry, context, 0, (s32)x, (s32)y, progress);
        ((MenuWorkEntry *)entry)->flags |= 0x10;
    }
    return entry;
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

