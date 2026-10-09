#include "common.h"
#include "mnu_callback_list.h"
#include "mnu_work.h"

#define MNU_WORK_ACTIVE   1

#define MNU_STATE_COMPLETED    1
#define MNU_STATE_VALUE_GATED  2
#define MNU_STATE_WAIT_PENDING 8


extern u32 mnuActiveEffectEntry;

extern MenuRuntimePairCallback D_004389B0;

extern MenuRuntimeWorkCallback D_004389AC;

extern MenuWorkCallback D_004389A8;

extern MenuWorkCallback D_004389A4;

extern MenuWorkCallback D_004389A0;

extern u32 mnuWorkEntryPool;

extern s32 mnuWorkEntryPoolCount;

extern MenuRegistryParameters *mnuMenuRegistryParametersBase;

extern u32 mnuMenuRegistryParametersSetupCount;

extern MenuMovementRecord18 *mnuMovementRecordTableBase;

extern u32 mnuMovementRecordTableSetupCount;

extern MenuRegistry *mnuMenuRecordRegistryBase;

extern u32 mnuMenuRecordRegistrySetupCount;

extern void (*sdfTickCallback)(void);

extern MenuRuntimeList D_0045C870;

extern MenuRuntimeList D_0045C880;

extern void dds3DestroyCallbackNodeAfterLastNotification(MnuCallbackList *);

extern void mnuFreeOptionalBlock(u32);
extern void *func_0035A828(u32 bytes);
extern u8 *mnuGetResourceProgressStepState(void);
extern u32 mnuGetActiveEffectWorkEntry(void);

typedef struct MenuLengthData {
    u8 pad0[4];
    u16 firstCount;
    u16 secondCount;
    s32 *firstRecords;
    s32 *secondRecords;
} MenuLengthData;

void func_003214D0(u32, s32);
s32 dds3MeasureRecordBlock(s32 *entries, s32 count);


s32 mnuAdvanceTimedStateRecord(MenuStateRecord *record);
extern MenuRuntimeRecord *func_00321A30(MenuStateRecord *record,
                                        MenuRuntimeList *runtimeList,
                                        s32 x, s32 y, f32 angle);

u32 mnuCreateReleaseCallbackNode(void) {
    MnuCallbackList *node = mnuCreateCallbackNode(0);
    node->onRemove = func_003214D0;
    return (u32)node;
}
INCLUDE_ASM(const s32, "game/code_00321500", func_00321528);

void func_003216A8(MnuCallbackList *, MenuRuntimeList *, s32, s32, s32, f32);

void func_00321688(u32 left, u32 right, u32 value, u32 count, f32 angle) {
    func_003216A8((MnuCallbackList *)left, (MenuRuntimeList *)right,
                  value, count, 1, angle);
}


void func_003216A8(MnuCallbackList *list, MenuRuntimeList *runtimeList,
                   s32 x, s32 y, s32 enabled, f32 angle) {
    SdfListNode *node = list->head;
    MenuStateRecord *record;

    if (node != NULL) {
        do {
            record = (MenuStateRecord *)node->value;
            if (mnuAdvanceTimedStateRecord(record) != 0 && enabled != 0) {
                record->flags.word &= 0xFFFE;
                /* The native dispatcher retains separate kind paths even
                 * though both currently invoke the same spawn provider. */
                if ((record->tag.flags & 0xF) >= 2) {
                    func_00321A30(record, runtimeList, x, y, angle);
                } else {
                    func_00321A30(record, runtimeList, x, y, angle);
                }
            }
            node = node->next;
        } while (node != NULL);
    }
}

void func_00321798(MnuCallbackList *list, MenuRuntimeList *runtimeList,
                   s32 kindMask, s32 x, s32 y, s32 enabled, f32 angle) {
    SdfListNode *node = list->head;
    MenuStateRecord *record;

    if (node != NULL) {
        do {
            record = (MenuStateRecord *)node->value;
            if (mnuAdvanceTimedStateRecord(record) != 0 && enabled != 0) {
                u32 kind = record->tag.flags & 0xF;
                if ((kindMask >> kind) & 1) {
                    record->flags.word &= 0xFFFE;
                    /* Retain the two native kind paths, as in the unfiltered
                     * updater, though both use the same spawn provider. */
                    if (kind >= 2) {
                        func_00321A30(record, runtimeList, x, y, angle);
                    } else {
                        func_00321A30(record, runtimeList, x, y, angle);
                    }
                }
            }
            node = node->next;
        } while (node != NULL);
    }
}

/* Allocate a zeroed 0x22-byte record with an eight-byte tag at offset 0xA. */
MenuStateRecord *mnuCreateNamedRecord(const void *tagData) {
    MenuStateRecord *record;
    if (tagData == 0) {
        return 0;
    }
    record = func_0035A828(sizeof(*record));
    memset(record, 0, sizeof(*record));
    memcpy(&record->tag, tagData, sizeof(record->tag));
    return record;
}

void mnuFreeOptionalBlock(u32 ptr) {
    if (ptr != 0) {
        func_0035A880(ptr);
    }
}


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
    if (++record->elapsedCount >= record->tag.duration) {
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

extern MenuRuntimeRecord *mnuAcquireRuntimeRecordSlot(MenuRuntimeList *);
extern MenuRuntimeCallback D_0043899C;
extern f64 cos(f64);
extern f64 sin(f64);

MenuRuntimeRecord *mnuCreateRuntimeRecord(MenuRuntimeList *list, s32 x, s32 y, u8 kind,
                           s32 offsetX, s32 offsetY, s32 direction,
                           s16 speed, s16 remaining, f32 angle) {
    MenuRuntimeRecord *record = mnuAcquireRuntimeRecordSlot(list);

    if (record == NULL) {
        return NULL;
    }
    record->state.kind = kind;
    record->state.directionDegrees = direction;
    record->displacementX = 0.0f;
    record->displacementY = 0.0f;
    record->baseX = x;
    record->baseY = y;
    record->rotatedOffsetX = offsetX * cos(angle + 1.5707963f) + offsetY * sin(angle + 1.5707963f);
    record->rotatedOffsetY = offsetY * cos(angle + 1.5707963f) - offsetX * sin(angle + 1.5707963f);
    record->angle = angle;
    record->speed = speed;
    record->remaining = remaining;
    D_0043899C(record);
    return record;
}


void func_00321E18(MenuRuntimeRecord *records, s32 capacity) {
    memset(&D_0045C870, 0, sizeof(D_0045C870));
    D_0045C870.records = records;
    D_0045C870.capacity = capacity;
}

void func_00321E70(MenuRuntimeRecord *records, s32 capacity) {
    memset(&D_0045C880, 0, sizeof(D_0045C880));
    D_0045C880.records = records;
    D_0045C880.capacity = capacity;
}

MenuRuntimeList *func_00321EC8(void) {
    return &D_0045C870;
}

MenuRuntimeList *func_00321ED8(void) {
    return &D_0045C880;
}

/* Clears only record storage; the list header and activeCount are unchanged. */
void mnuClearRuntimeRecordStorage(MenuRuntimeList *list) {
    memset(list->records, 0, list->capacity * sizeof(*list->records));
}


MenuRuntimeRecord *mnuAcquireRuntimeRecordSlot(MenuRuntimeList *list) {
    MenuRuntimeRecord *record;
    s32 i;
    u32 count;

    record = list->records;
    for (i = 0; i < list->capacity; i++, record++) {
        if (!(record->state.word & MNU_WORK_ACTIVE)) {
            count = list->activeCount;
            record->state.word |= MNU_WORK_ACTIVE;
            list->activeCount = count + 1;
            return record;
        }
    }
    return NULL;
}

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

void mnuAdvanceMovingRuntimeRecords(MenuRuntimeList *list) {
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
                record->baseX = work->x0;
                record->baseY = mnuEvaluateTimedValue(work);
            }
            pi = 3.1415926f;
            angle = record->state.directionDegrees * pi / 180.0f +
                    record->angle + 1.5707963f;
            record->displacementX += record->speed * cos(angle);
            record->displacementY -= record->speed * sin(angle);
            x = record->displacementX + record->rotatedOffsetX + record->baseX;
            y = record->displacementY + record->rotatedOffsetY + record->baseY;
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
    mnuAdvanceMovingRuntimeRecords(&D_0045C870);
}


void func_00322418(void) {
    mnuAdvanceMovingRuntimeRecords(&D_0045C880);
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

/* Each packed table word contains signed X and Y hit radii. */
extern s8 D_0040B248[][2];

u16 *func_003224C8(s32 index) {
    return (u16 *)&D_0040B248[index];
}

void mnuBindMenuRecordRegistry(MenuRegistry *records, u32 count) {
    mnuMenuRecordRegistryBase = records;
    mnuMenuRecordRegistrySetupCount = count;
}

MenuRegistry *mnuGetMenuRecordRegistryEntry(u32 taggedIndex) {
    u16 index = taggedIndex;
    return &mnuMenuRecordRegistryBase[index];
}

void mnuBindMovementRecordTable(MenuMovementRecord18 *records, u32 count) {
    mnuMovementRecordTableBase = records;
    mnuMovementRecordTableSetupCount = count;
}

MenuMovementRecord18 *mnuGetMovementRecordByIndex(u32 movementRecordIndex) {
    u16 index = movementRecordIndex;
    return &mnuMovementRecordTableBase[index];
}

void mnuBindMenuRegistryParameters(MenuRegistryParameters *records, u32 count) {
    mnuMenuRegistryParametersBase = records;
    mnuMenuRegistryParametersSetupCount = count;
}

MenuRegistryParameters *mnuGetMenuRegistryParametersByIndex(u32 parameterIndex) {
    u8 index = parameterIndex;
    return &mnuMenuRegistryParametersBase[index];
}

MenuShortRecord *mnuFindFirstFixedKindShortRecord(MenuShortRecordList *list) {
    s32 i;
    MenuShortRecord *record = list->records;
    for (i = 0; i < list->count; i++, record++) {
        if (record->kind == 0x40) {
            return record;
        }
    }
    return NULL;
}

MenuShortRecord *func_003225C0(MenuShortRecordList *list) {
    s32 i;
    MenuShortRecord *record = list->records;
    for (i = 0; i < list->count; i++, record++) {
        if (record->kind == 0x40) {
            return record;
        }
    }
    return NULL;
}

MenuRegistryRecord *mnuResolveTaggedRegistryRecord(MenuWorkEntry *work) {
    MenuRegistry *registryEntry;
    MenuRegistryTable *registryTable;
    if ((work->tag & MNU_WORK_TAG_CLASS_MASK) !=
        MNU_WORK_TAG_REGISTRY_TABLE) {
        return 0;
    }
    registryEntry = mnuGetMenuRecordRegistryEntry(work->tag);
    if (registryEntry == 0) {
        return 0;
    }
    registryTable = registryEntry->table;
    return &registryTable->recordBase[work->recordIndex];
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
        MenuResourceRecord *resourceRecord = mnuGetResourceRecordByIndex(entry->resourceRecordIndex);
        return entry->y0 +
            (f32)((s32)*(u16 *)(progressState + 2) - resourceRecord->progress);
    }
    return entry->y0;
}

INCLUDE_ASM(const s32, "game/code_00321500", mnuInitializeRegistryWorkEntry);

void mnuDeactivateWorkEntry(MenuWorkEntry *entry) {
    entry->flags = entry->flags & 0xfffffffe;
    if (entry->callback != 0) {
        dds3DestroyCallbackNodeAfterLastNotification((MnuCallbackList *)entry->callback);
        entry->callback = 0;
    }
}

extern void func_003226D8(MenuWorkEntry *, MenuShortRecordList *, MenuShortRecord *);
extern s32 func_003230A0(MenuWorkEntry *, MenuRegistryTable *, MenuRegistryRecord *, MenuShortRecord *);
extern void func_00321528(u32, MenuRegistryRecord *);

/* Tick the packed countdown and dispatch the row's fixed-kind record. */
s32 mnuAdvanceRegistryWorkEntry(MenuWorkEntry *entry) {
    MenuRegistryTable *table;
    MenuRegistryRecord *row;
    MenuShortRecordList *list;
    MenuShortRecord *record;
    MenuShortRecord empty;

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
    MenuShortRecord *record;

    entry->control.bits.loopMode = 0;
    record = (MenuShortRecord *)mnuFindMarkedShortListRecord(list);
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

extern f32 sdfVectorLength(const f32 *vector);

s32 func_003233E8(s32 context) {
    MenuRuntimeList *runtimeList = func_00321EC8();
    MenuProgressParameters *parameters = mnuGetResourceProgressParameters();
    s32 entryIndex;

    for (entryIndex = 0; entryIndex < mnuWorkEntryPoolCount; entryIndex++) {
        MenuWorkEntry *entry = (MenuWorkEntry *)mnuWorkEntryPool + entryIndex;

        if (entry->flags & MNU_WORK_ACTIVE) {
            f32 vector[4];
            f32 value;
            f32 previousY;
            f32 currentX;
            f32 currentY;
            s32 currentYInteger;
            u8 alpha;

            memset(vector, 0, sizeof(vector));
            /* Save X across advancement, then reuse the scalar for step length. */
            value = entry->x0;
            previousY = mnuEvaluateTimedValue(entry);
            if (mnuAdvanceRegistryWorkEntry(entry) != 0) {
                continue;
            }

            currentX = entry->x0;
            vector[0] = currentX;
            currentY = (f32)(s32)mnuEvaluateTimedValue(entry);
            currentYInteger = (s32)currentY;
            vector[0] -= value;
            vector[1] = currentY - previousY;
            value = sdfVectorLength(vector);
            if (value * 10.0f >= 255.0f) {
                alpha = 255;
            } else {
                alpha = (u8)((u8)value * 10.0f);
            }
            /* The high byte stores the movement alpha. */
            ((u8 *)&entry->unk38)[1] = alpha;

            if (entry->x0 < -50.0f || (f32)parameters->width + 50.0f < entry->x0 ||
                (f32)currentYInteger < -200.0f ||
                (f32)parameters->height + 64.0f < (f32)currentYInteger) {
                entry->flagsBits.pendingDeactivate = 1;
            }

            if (context == 0) {
                func_00321688(entry->callback, (u32)runtimeList,
                              (s32)entry->x0, currentYInteger, entry->scale0);
            }

            if (entry->flagsBits.unk5) {
                entry->flagsBits.unk6++;
                if (entry->flagsBits.unk6 >= 11) {
                    entry->flagsBits.unk6 = 0;
                }
            }
        }
    }
    return 0;
}

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
            if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_REGISTRY_TABLE) {
                registry = mnuGetMenuRecordRegistryEntry(entry->tag);
                if (entry->remaining <= (registry->unk08 >> 1)) {
                    entry->flagsBits.unk5 = 1;
                }
            }
        }
    }
    if (entry->flagsBits.finished) {
        if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_MOVEMENT_TABLE) {
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

void mnuSetWorkEntryStartCallback(MenuWorkCallback callback) {
    D_004389A0 = callback;
}

void mnuSetWorkEntryFinishOrDeactivateCallback(MenuWorkCallback callback) {
    D_004389A4 = callback;
}

void mnuSetActiveWorkVisitor(MenuWorkCallback callback) {
    D_004389A8 = callback;
}

void mnuSetRuntimeWorkHitCallback(MenuRuntimeWorkCallback callback) {
    D_004389AC = callback;
}

void mnuSetRuntimeRecordPairCallback(MenuRuntimePairCallback callback) {
    D_004389B0 = callback;
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

/* Find the first active runtime record overlapping the work entry's hit rectangle. */
MenuRuntimeRecord *func_00323988(MenuWorkEntry *work, struct MnuShootingWork *context) {
    MenuRuntimeList *list = func_00321EC8();
    MenuRegistryParameters *parameters = NULL;
    MenuRuntimeRecord *record;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 i;
    u32 tag = work->tag;

    switch (tag & MNU_WORK_TAG_CLASS_MASK) {
    case MNU_WORK_TAG_MOVEMENT_TABLE: {
        MenuMovementRecord18 *fixed = mnuGetMovementRecordByIndex(tag);
        parameters = mnuGetMenuRegistryParametersByIndex(fixed->parameterTag);
        break;
    }
    case MNU_WORK_TAG_REGISTRY_TABLE: {
        MenuRegistry *registry;
        if (work->remaining == 0) {
            return NULL;
        }
        if (work->control.bits.countdownEnabled) {
            return NULL;
        }
        registry = mnuGetMenuRecordRegistryEntry(tag);
        parameters = mnuGetMenuRegistryParametersByIndex(registry->parameterIndex);
        break;
    }
    }
    if (parameters->hitWidth == 0) {
        return NULL;
    }
    left = (s32)(work->x0 + (f32)parameters->hitOffsetX);
    top = (s32)(work->y0 + (f32)parameters->hitOffsetY);
    right = left + parameters->hitWidth;
    bottom = top + parameters->hitHeight;
    record = list->records;
    for (i = 0; i < list->capacity; i++, record++) {
        if (record->state.word & MNU_WORK_ACTIVE) {
            s32 x = (s32)((record->rotatedOffsetX + record->displacementX) + record->baseX);
            s32 y = (s32)((record->rotatedOffsetY + record->displacementY) + record->baseY);
            s32 kind = record->state.kind & 0xF;
            s32 radiusX = D_0040B248[kind][0];
            s32 radiusY;

            if (right < x - radiusX || x + radiusX < left) {
                continue;
            }
            radiusY = D_0040B248[kind][1];
            if (bottom < y - radiusY || y + radiusY < top) {
                continue;
            }
            if ((work->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_MOVEMENT_TABLE && kind == 3) {
                D_004389AC(record, work, context);
                return record;
            }
            if (work->inputCountdown == 0 && work->remaining != 0) {
                D_004389AC(record, work, context);
                return record;
            }
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_00321500", func_00323BB8);

/* Test active runtime records against the fixed work pool and its hit bounds. */
s32 func_00323DF0(MenuRuntimeList *list, struct MnuShootingWork *context) {
    s32 entryIndex;

    for (entryIndex = 0; entryIndex < 100; entryIndex++) {
        MenuWorkEntry *entry = (MenuWorkEntry *)(entryIndex * sizeof(MenuWorkEntry) + mnuWorkEntryPool);

        if (entry->flagsBits.active) {
            s32 entryX;
            s32 entryY;
            MenuRegistry *registry;
            MenuRegistryParameters *parameters;

            if (entry->flagsBits.finished || entry->control.bits.countdownEnabled) {
                continue;
            }
            entryX = (s32)entry->x0;
            entryY = (s32)mnuEvaluateTimedValue(entry);
            registry = mnuGetMenuRecordRegistryEntry(entry->tag);
            parameters = mnuGetMenuRegistryParametersByIndex(registry->parameterIndex);

            if (parameters->hitWidth != 0) {
                s32 left = entryX + parameters->hitOffsetX;
                s32 top = entryY + parameters->hitOffsetY;
                s32 right = left + parameters->hitWidth;
                s32 bottom = top + parameters->hitHeight;
                MenuRuntimeRecord *record = list->records;
                s32 recordCount = list->capacity;
                s32 recordIndex = 0;

                while (recordIndex < recordCount) {
                    if (record->state.word & MNU_WORK_ACTIVE) {
                        s32 kind = record->state.kind & 0xF;
                        s32 radiusX = D_0040B248[kind][0];
                        s32 centerX = (s32)((record->rotatedOffsetX + record->displacementX) + record->baseX);
                        s32 centerY = (s32)((record->rotatedOffsetY + record->displacementY) + record->baseY);

                        if (right >= centerX - radiusX && centerX + radiusX >= left) {
                            s32 radiusY = D_0040B248[kind][1];

                            if (bottom >= centerY - radiusY && centerY + radiusY >= top) {
                                s32 advanceMode;
                                s32 status = 0;

                                switch (kind) {
                                case 1:
                                case 2:
                                case 3:
                                    advanceMode = 1;
                                    mnuDeactivateListRecord(list, record);
                                    break;
                                case 4:
                                    advanceMode = 2;
                                    break;
                                default:
                                    advanceMode = 1;
                                    mnuDeactivateListRecord(list, record);
                                    break;
                                }
                                if (entry->remaining == 0) {
                                    status = 2;
                                } else {
                                    status = mnuAdvanceWorkEntry(entry, advanceMode) != 0;
                                }
                                D_004389AC(record, entry, context);
                                if (status != 0) {
                                    break;
                                }
                                recordCount = list->capacity;
                            }
                        }
                    }
                    record++;
                    recordIndex++;
                }
            }
        }
    }
    return 0;
}

/* Advance work entries whose hit rectangles overlap the input. */
s32 func_00324070(MenuWorkEntry *input) {
    s32 entryIndex;

    if (input->remaining == 0) {
        return 0;
    }
    for (entryIndex = 0; entryIndex < 100; entryIndex++) {
        MenuWorkEntry *entry =
            (MenuWorkEntry *)(entryIndex * sizeof(MenuWorkEntry) + mnuWorkEntryPool);
        MenuRegistry *registry;
        MenuRegistryParameters *parameters;
        MenuMovementRecord18 *inputRecord;
        s32 entryX;
        s32 entryY;
        s32 left;
        s32 top;
        s32 right;
        s32 bottom;
        s32 inputLeft;
        s32 inputTop;
        s32 inputRight;
        s32 inputBottom;

        if ((entry->flags & MNU_WORK_ACTIVE) == 0) {
            continue;
        }
        /* Finished entries and pending deactivations do not take hits. */
        if ((entry->flags & 0xC) != 0) {
            continue;
        }

        entryX = (s32)entry->x0;
        entryY = (s32)mnuEvaluateTimedValue(entry);
        registry = mnuGetMenuRecordRegistryEntry(entry->tag);
        parameters = mnuGetMenuRegistryParametersByIndex(registry->parameterIndex);
        if (parameters->hitWidth == 0) {
            continue;
        }
        left = entryX + parameters->hitOffsetX;
        top = entryY + parameters->hitOffsetY;

        right = left + parameters->hitWidth;
        bottom = top + parameters->hitHeight;
        inputRecord = mnuGetMovementRecordByIndex(input->tag);
        parameters = mnuGetMenuRegistryParametersByIndex(inputRecord->parameterTag);
        inputLeft = (s32)(input->x0 + (f32)parameters->hitOffsetX);
        inputTop = (s32)(input->y0 + (f32)parameters->hitOffsetY);
        inputRight = inputLeft + parameters->hitWidth;
        inputBottom = inputTop + parameters->hitHeight;

        if (right < inputLeft || inputRight < left ||
            bottom < inputTop || inputBottom < top) {
            continue;
        }

        if (entry->remaining != 0 && mnuAdvanceWorkEntry(entry, 3) != 0) {
            entry->flags |= 0x100000;
        }
        if (input->remaining != 0 && mnuAdvanceWorkEntry(input, 1) != 0) {
            return 0;
        }
    }
    return 0;
}

extern char D_0045C890[12];

/* Copy the 12-byte resource progress parameter block. */
void mnuSetInputActionSnapshot(u8 *src) {
    memcpy(D_0045C890, src, sizeof(D_0045C890));
}

u32 mnuGetActiveEffectWorkEntry(void) {
    return mnuActiveEffectEntry;
}

s32 func_003242D0(MenuWorkEntry *entry, u32 mode);

void mnuInitializeActiveEffectWorkEntry(u32 entry) {
    memset((void *)entry, 0, 0x48);
    func_003242D0((MenuWorkEntry *)entry, 0);
    ((MenuWorkEntry *)entry)->flags |= 0x4010;
    ((MenuWorkEntry *)entry)->tag = MNU_WORK_TAG_MOVEMENT_TABLE;
    ((MenuWorkEntry *)entry)->remaining = 1;
    ((MenuWorkEntry *)entry)->scale0 = 1.5707963f;
    mnuActiveEffectEntry = entry;
}

s32 func_003242D0(MenuWorkEntry *entry, u32 mode) {
    MenuStateRecord savedRecord;
    MenuInitialTag tag;
    MenuStateRecord *record;
    MnuCallbackList *oldList;
    SdfListNode *node;
    u32 currentMode;
    s32 haveSavedRecord = 0;

    if (mode != 0) {
        entry->flagsBits.mode = (mode - 1) & 0xF;
    } else if (entry->flagsBits.mode == 4) {
        return 0;
    }

    oldList = (MnuCallbackList *)entry->callback;
    if (oldList != NULL) {
        for (node = oldList->head; node != NULL; node = node->next) {
            record = (MenuStateRecord *)node->value;
            if (record->tag.flags == 0x44) {
                memcpy(&savedRecord, record, sizeof(savedRecord));
                haveSavedRecord = 1;
                break;
            }
        }
        dds3DestroyCallbackNodeAfterLastNotification(oldList);
    }

    entry->callback = mnuCreateReleaseCallbackNode();

    tag.flags = 0x44;
    tag.group = -80;
    tag.duration = 60;
    tag.index = 5;
    record = mnuCreateNamedRecord(&tag);
    if (haveSavedRecord) {
        memcpy(record, &savedRecord, sizeof(*record));
    } else {
        record->flags.bits.completed = 1;
        record->flags.bits.hasRange = 1;
        record->flags.bits.direction = 1;
        record->unk18 = 20;
        record->offsetX = -20;
        record->offsetY = -20;
    }
    func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);

    currentMode = entry->flagsBits.mode;
    if (currentMode < 5) {
        switch (currentMode) {
        case 0:
            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetY = 15;
            record->offsetX = 0;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 2;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);
            entry->flagsBits.mode++;
            break;
        case 1:
            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = 20;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 3;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);

            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -20;
            record->offsetY = 5;
            record->flags.bits.direction = 4;
            record->flags.bits.hasRange = 1;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);
            entry->flagsBits.mode++;
            break;
        case 2:
            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetY = 15;
            record->offsetX = 0;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 2;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 10;
            tag.index = 40;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = 30;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 3;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 10;
            tag.index = 40;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -30;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 4;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);
            entry->flagsBits.mode++;
            break;
        case 3:
            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 30;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = 20;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 3;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);

            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 30;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -20;
            record->offsetY = 5;
            record->flags.bits.direction = 4;
            record->flags.bits.hasRange = 1;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 7;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = 40;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 3;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 7;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -40;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 4;
            func_00320CE0((MnuCallbackList *)entry->callback, 0, (u32)record);
            entry->flagsBits.mode++;
            break;
        case 4:
        default:
            break;
        }
    }
    return 1;
}

extern f32 sdfVec3Normalize(f32 *vector);
extern void sdfVectorScale(f32 factor, f32 *vector);



s32 func_00324840(void) {
    MenuWorkEntry *work = (MenuWorkEntry *)mnuActiveEffectEntry;
    u32 kindMask = 0;
    s8 *input = (s8 *)D_0045C890;
    MenuProgressParameters *parameters = mnuGetResourceProgressParameters();
    MenuMovementRecord18 *progress = mnuGetMovementRecordByIndex((u16)work->tag);

    if (work->inputCountdown > 0) {
        work->inputCountdown--;
        if (work->inputCountdown <= 0) {
            work->inputCountdown = 0;
            work->remaining = 1;
        }
    }

    if (((work->flags >> 14) & 1) == 0) {
        f32 movement[4];
        f32 length;

        memset(movement, 0, sizeof(movement));
        if (input[0] != 0) {
            movement[1] = -1.0f;
        } else if (input[1] != 0) {
            movement[1] = 1.0f;
        }
        if (input[2] != 0) {
            movement[0] = -1.0f;
        } else if (input[3] != 0) {
            movement[0] = 1.0f;
        }

        length = sdfVec3Normalize(movement);
        if (length != 0.0f) {
            sdfVectorScale((f32)progress->movementScale, movement);
            work->x0 += movement[0];
            work->y0 += movement[1];
            if (work->x0 < 16.0f) {
                work->x0 = 16.0f;
            }
            if (work->y0 < 16.0f) {
                work->y0 = 16.0f;
            }
            if (work->x0 > (f32)(parameters->width - 16)) {
                work->x0 = (f32)(parameters->width - 16);
            }
            if (work->y0 > (f32)(parameters->height - 16)) {
                work->y0 = (f32)(parameters->height - 16);
            }
        }

        kindMask = 0;
        if (input[4] != 0) {
            kindMask = 0x21;
        }
        if (input[5] != 0) {
            kindMask |= 0x10;
        }
    }

    func_00321798((MnuCallbackList *)work->callback,
                  func_00321ED8(), kindMask,
                  (s32)work->x0, (s32)work->y0, kindMask, work->scale0);

    func_00322418();

    {
        MenuWorkFlags flags;
        flags.word = work->flags;
        if (flags.bits.unk5) {
            MenuWorkFlags updated = flags;
            updated.bits.unk6++;
            work->flags = updated.word;
            if (updated.bits.unk6 >= 11) {
                MenuWorkFlags cleared = updated;
                cleared.bits.unk6 = 0;
                work->flags = cleared.word;
                flags = cleared;
            } else {
                flags = updated;
            }
        }
        if (flags.bits.inputDisabled) {
            return 0;
        }
        return 1;
    }
}

void mnuInitializeEffectContext(MenuWorkEntry *context) {
    MenuInitialTag initialTag;
    /* Retail only initializes bytes 1 through 7 of this tag. */
    initialTag.flags = 0;
    initialTag.group = 0;
    initialTag.duration = 2;
    initialTag.index = 0;
    memset(context, 0, 0x48);
    context->callback = mnuCreateReleaseCallbackNode();
    func_00320CE0((MnuCallbackList *)context->callback, 0,
                   (u32)mnuCreateNamedRecord(&initialTag));
}

/* Update the active-effect entrance animation once per frame. */
s32 func_00324B28(MenuWorkEntry *entry) {
    extern s32 D_004389B4;
    extern s32 D_004389B8;
    s32 phase = D_004389B4;
    s32 mode;
    s32 result = 0;
    MenuWorkFlags flags;
    f32 positionY;

    switch (phase) {
    case 0:
        flags.word = entry->flags;
        entry->x0 = -200.0f;
        D_004389B4 = 2;
        mode = flags.bits.mode;
        if (flags.bits.finished) {
            if (mode >= 2) {
                mode--;
                func_003242D0(entry, mode);
            }
        }
        entry->flags = ((mode & 0xF) << 15) | 0x4001;
        D_004389B8 = 0;
        break;
    case 2:
        if (++D_004389B8 >= 31) {
            D_004389B4 = 3;
            entry->x0 = 120.0f;
            positionY = 400.0f;
            entry->y0 = positionY;
            D_004389B8 = 0;
        }
        break;
    case 3:
        positionY = entry->y0;
        positionY -= 2.0f;
        if (++D_004389B8 >= 31) {
            phase = 1;
        }
        D_004389B4 = phase;
        entry->y0 = positionY;
        break;
    case 1: {
        u32 completionFlags = entry->flags;
        D_004389B4 = 0;
        entry->inputCountdown = 100;
        entry->flags = completionFlags & ~0x4000u;
        result = 1;
        break;
    }
    default:
        break;
    }
    return result;
}


MenuWorkEntry *mnuCreateAnimatedEffect(u32 context, f32 x, f32 y, f32 progress) {
    MenuWorkEntry *entry = mnuFindUnusedWorkEntry();
    if (entry != NULL) {
        mnuInitializeRegistryWorkEntry(entry, context, 0, (s32)x, (s32)y, progress);
        entry->flags |= 0x10;
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
        dds3DestroyCallbackNodeAfterLastNotification((MnuCallbackList *)*(u32 *)node);
        dds3DestroyCallbackNodeAfterLastNotification((MnuCallbackList *)*(u32 *)(node + 4));
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

