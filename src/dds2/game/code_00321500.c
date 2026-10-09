#include "common.h"
#include "mnu_callback_list.h"
#include "mnu_work.h"
#include "dds_nested_resource.h"

#define MNU_WORK_ACTIVE   1

#define MNU_STATE_COMPLETED    1
#define MNU_STATE_VALUE_GATED  2
#define MNU_STATE_WAIT_PENDING 8


extern MenuRuntimePairCallback mnuRuntimeRecordPairCallback;

extern MenuRuntimeWorkCallback mnuRuntimeWorkHitCallback;

extern MenuWorkCallback mnuActiveWorkVisitorCallback;

extern MenuWorkCallback mnuWorkEntryFinishOrDeactivateCallback;

extern MenuWorkCallback mnuWorkEntryStartCallback;

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
void func_003214D0(u32, u32);
s32 dds3MeasureRecordBlock(DdsCountedPayload *entries, s32 count);


s32 mnuAdvanceTimedStateRecord(MenuStateRecord *record);
extern MenuRuntimeRecord *mnuCreateRuntimeAnimationRecord(MenuStateRecord *record,
                                        MenuRuntimeList *runtimeList,
                                        s32 x, s32 y, f32 angle);

MnuCallbackList *mnuCreateReleaseCallbackNode(void) {
    MnuCallbackList *node = mnuCreateCallbackNode(0);
    node->onRemove = func_003214D0;
    return node;
}
extern void mnuClearResourceList(MnuCallbackList *list);

/* Rebuild the row's named states and attach its tagged parameter groups. */
void mnuBuildNamedResourceList(MnuCallbackList *callbackList, MenuRegistryRecord *row) {
    s32 groupIndex;
    s32 recordIndex;
    MenuShortRecordList *group;
    MenuShortRecord *tag;
    MenuStateRecord *state;

    if (callbackList != 0) {
        mnuClearResourceList(callbackList);
        group = row->secondLists;
        for (groupIndex = 0; groupIndex < row->secondCount; groupIndex++, group++) {
            state = NULL;
            tag = group->records;
            for (recordIndex = 0; recordIndex < group->count; recordIndex++, tag++) {
                if (tag->kind == 0x20) {
                    state = mnuCreateNamedRecord(tag);
                    func_00320CE0(callbackList, 0, (u32)state);
                } else if (tag->kind == 0x10) {
                    if (state != NULL) {
                        memcpy(&state->pad12, tag, sizeof(*tag));
                        state->flags.bits.valueGated = 1;
                    }
                } else if (tag->kind == 0x30) {
                    if (state != NULL) {
                        memcpy(&state->pad1A, tag, sizeof(*tag));
                        state->flags.bits.hasRange = 1;
                    }
                }
            }
        }
    }
}

void mnuUpdateLinkedTimedStateList(MnuCallbackList *, MenuRuntimeList *, s32, s32, s32, f32);

void func_00321688(MnuCallbackList *left, MenuRuntimeList *right,
                   u32 value, u32 count, f32 angle) {
    mnuUpdateLinkedTimedStateList(left, right, value, count, 1, angle);
}


void mnuUpdateLinkedTimedStateList(MnuCallbackList *list, MenuRuntimeList *runtimeList,
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
                    mnuCreateRuntimeAnimationRecord(record, runtimeList, x, y, angle);
                } else {
                    mnuCreateRuntimeAnimationRecord(record, runtimeList, x, y, angle);
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
                        mnuCreateRuntimeAnimationRecord(record, runtimeList, x, y, angle);
                    } else {
                        mnuCreateRuntimeAnimationRecord(record, runtimeList, x, y, angle);
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

extern MenuRuntimeRecord *mnuAcquireRuntimeRecordSlot(MenuRuntimeList *);
extern MenuRuntimeCallback mnuRuntimeRecordInitializationCallback;
extern f64 cos(f64);
extern f64 sin(f64);

MenuRuntimeRecord *mnuCreateRuntimeAnimationRecord(MenuStateRecord *state, MenuRuntimeList *runtimeList,
                                  s32 x, s32 y, f32 angle) {
    s32 offsetX = 0;
    s32 offsetY;
    s32 effect;
    MenuRuntimeRecord *record;
    u32 flags;
    u8 kind;
    f32 baseX;
    f32 baseY;

    if (state->flags.bits.hasRange) {
        offsetX = state->offsetX;
        offsetY = state->offsetY;
    } else {
        offsetY = 0;
    }

    effect = state->effect;
    if (effect > 0) {
        mnuCreateAnimatedEffect((u32)(effect - 1), (f32)(x + offsetX),
                                (f32)(y + offsetY), angle);
        state->value++;
        return NULL;
    }

    record = mnuAcquireRuntimeRecordSlot(runtimeList);
    if (record == NULL) {
        return NULL;
    }

    kind = state->tag.flags;
    baseX = (f32)x;
    baseY = (f32)y;
    record->displacementX = 0.0f;
    record->state.kind = kind;
    record->displacementY = 0.0f;
    record->state.directionDegrees = state->tag.group;
    record->baseX = baseX;
    record->baseY = baseY;
    record->rotatedOffsetX = offsetX * cos(angle + 1.5707963f) +
                    offsetY * sin(angle + 1.5707963f);
    record->rotatedOffsetY = offsetY * cos(angle + 1.5707963f) -
                    offsetX * sin(angle + 1.5707963f);
    record->angle = angle;
    record->speed = state->tag.index;
    flags = record->state.word & ~0x1Eu;
    record->remaining = state->unk18;
    record->state.word = flags |
                         (state->flags.bits.direction << 1);
    state->value++;
    mnuRuntimeRecordInitializationCallback(record);
    return record;
}


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
    mnuRuntimeRecordInitializationCallback(record);
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
                work = mnuGetActiveEffectWorkEntry();
                record->baseX = work->currentX;
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
s32 dds3MeasureMenuRecord(DdsNestedGroup *group) {
    s32 byteSize = dds3MeasureRecordBlock(group->first, group->firstCount) + 0x10;
    return byteSize + dds3MeasureRecordBlock(group->second, group->secondCount);
}

/* Each entry has an eight-byte header followed by its eight-byte subentries. */
s32 dds3MeasureRecordBlock(DdsCountedPayload *records, s32 count) {
    s32 subentryCount;
    s32 byteSize;

    byteSize = count << 3;
    if (0 < count) {
        do {
            subentryCount = records->count;
            records++;
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
