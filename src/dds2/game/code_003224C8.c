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
extern u8 *mnuGetResourceProgressStepState(void);
s32 dds3MeasureRecordBlock(DdsCountedPayload *entries, s32 count);


s32 mnuAdvanceTimedStateRecord(MenuStateRecord *record);
extern MenuRuntimeRecord *func_00321A30(MenuStateRecord *record,
                                        MenuRuntimeList *runtimeList,
                                        s32 x, s32 y, f32 angle);

/* Exported timed-state/runtime-list providers from the preceding unit. */
extern MenuRuntimeList *func_00321EC8(void);
extern MenuRuntimeList *func_00321ED8(void);
extern void func_00321688(MnuCallbackList *, MenuRuntimeList *, u32, u32, f32);
extern void func_00321798(MnuCallbackList *, MenuRuntimeList *, s32, s32, s32, s32, f32);
extern void mnuDeactivateListRecord(MenuRuntimeList *, MenuRuntimeRecord *);
extern void func_00322418(void);

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
        if (record->kind == MENU_SHORT_RECORD_KIND_FIXED) {
            return record;
        }
    }
    return NULL;
}

MenuShortRecord *func_003225C0(MenuShortRecordList *list) {
    s32 i;
    MenuShortRecord *record = list->records;
    for (i = 0; i < list->count; i++, record++) {
        if (record->kind == MENU_SHORT_RECORD_KIND_FIXED) {
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

MenuShortRecord *mnuFindMarkedShortListRecord(MenuShortRecordList *list) {
    s32 recordIndex;
    MenuShortRecord *record;

    if (list != NULL) {
        record = list->records;
        if (record == NULL) {
            return NULL;
        }
        for (recordIndex = 0; recordIndex < list->count; recordIndex++) {
            if ((record->kind & MENU_REPEAT_DIRECTIVE_KIND_MASK) ==
                MENU_REPEAT_DIRECTIVE_KIND_TAG) {
                return record;
            }
            record++;
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_003224C8", func_003226D8);

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
        return entry->currentY +
            (f32)((s32)*(u16 *)(progressState + 2) - resourceRecord->progress);
    }
    return entry->currentY;
}

extern void func_00321528(MnuCallbackList *, MenuRegistryRecord *);
extern void func_003232A0(MenuWorkEntry *, MenuShortRecordList *);

void mnuInitializeRegistryWorkEntry(MenuWorkEntry *entry, u32 tag,
                                    s32 resourceRecordIndex, s32 x, s32 y,
                                    f32 progress) {
    MenuRegistry *registry;
    MenuRegistryTable *table;
    MenuRegistryRecord *record;

    memset(entry, 0, sizeof(*entry));
    entry->flags |= MNU_WORK_ACTIVE;
    entry->resourceRecordIndex = resourceRecordIndex;
    tag |= MNU_WORK_TAG_REGISTRY_TABLE;
    entry->tag = tag;
    entry->currentX = entry->segmentStartX = (f32)x;
    entry->currentY = entry->segmentStartY = (f32)y;
    entry->currentAngleRadians = entry->segmentStartAngleRadians = progress;

    registry = mnuGetMenuRecordRegistryEntry(tag);
    table = registry->table;
    entry->remaining = registry->initialRemainingCount;
    record = table->recordBase;
    func_003232A0(entry, record->lists);
    entry->callback = mnuCreateReleaseCallbackNode();
    func_00321528(entry->callback, record);
}

void mnuDeactivateWorkEntry(MenuWorkEntry *entry) {
    entry->flags = entry->flags & 0xfffffffe;
    if (entry->callback != 0) {
        dds3DestroyCallbackNodeAfterLastNotification(entry->callback);
        entry->callback = 0;
    }
}

extern void func_003226D8(MenuWorkEntry *, MenuShortRecordList *, MenuShortRecord *);
extern s32 func_003230A0(MenuWorkEntry *, MenuRegistryTable *, MenuRegistryRecord *, MenuShortRecord *);

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

s32 func_003230A0(MenuWorkEntry *entry, MenuRegistryTable *table,
                  MenuRegistryRecord *row, MenuShortRecord *record) {
    s32 action = 0;

    entry->frameCounter++;
    if (entry->frameCounter > record->parameters[0]) {
        entry->frameCounter = 0;
        entry->shortListIndex++;
        entry->segmentStartX = entry->currentX;
        entry->segmentStartY = entry->currentY;
        entry->segmentStartAngleRadians = entry->currentAngleRadians;
        if (entry->shortListIndex >= row->firstCount) {
            if (entry->control.bits.loopMode == MENU_REPEAT_LOOP_MODE_RESTART_OR_ADVANCE) {
                switch (entry->control.bits.repeatMode) {
                case MENU_REPEAT_POLICY_DECREMENT_COUNT:
                    if (entry->repeatCount > 0) {
                        entry->shortListIndex = 0;
                        entry->repeatCount--;
                    } else {
                        action = 1;
                    }
                    break;
                case MENU_REPEAT_POLICY_REPEAT_COUNT_LT_REMAINING:
                    if (entry->remaining > entry->repeatCount) {
                        entry->shortListIndex = 0;
                    } else {
                        action = 1;
                    }
                    break;
                case MENU_REPEAT_POLICY_REMAINING_LT_REPEAT_COUNT:
                    if (entry->remaining < entry->repeatCount) {
                        entry->shortListIndex = 0;
                    } else {
                        action = 1;
                    }
                    break;
                }
            } else if (entry->control.bits.loopMode == MENU_REPEAT_LOOP_MODE_TARGET_CAPABLE) {
                switch (entry->control.bits.repeatMode) {
                case MENU_REPEAT_POLICY_DECREMENT_COUNT:
                    if (entry->repeatCount > 0) {
                        entry->shortListIndex = 0;
                        entry->repeatCount--;
                    } else {
                        action = 2;
                    }
                    break;
                case MENU_REPEAT_POLICY_REPEAT_COUNT_LT_REMAINING:
                    action = entry->remaining > entry->repeatCount ? 2 : 1;
                    break;
                case MENU_REPEAT_POLICY_REMAINING_LT_REPEAT_COUNT:
                    action = entry->remaining < entry->repeatCount ? 2 : 1;
                    break;
                }
            } else {
                action = 1;
            }
            if (action != 0) {
                entry->shortListIndex = 0;
                entry->elapsed = 0;
                if (action == 1) {
                    entry->recordIndex++;
                } else if (action == 2) {
                    entry->recordIndex = entry->repeatTargetRecordIndex;
                }
                if (table->recordCount <= entry->recordIndex) {
                    return 2;
                }
                row = &table->recordBase[entry->recordIndex];
                func_003232A0(entry, row->lists);
                return 1;
            }
        }
    }
    return 0;
}

void func_003232A0(MenuWorkEntry *entry, MenuShortRecordList *list) {
    MenuShortRecord *record;

    entry->control.bits.loopMode = MENU_REPEAT_LOOP_MODE_NONE;
    record = mnuFindMarkedShortListRecord(list);
    if (record != NULL) {
        switch (record->kind) {
        case MENU_REPEAT_DIRECTIVE_KIND_REPEAT:
            entry->control.bits.loopMode = MENU_REPEAT_LOOP_MODE_RESTART_OR_ADVANCE;
            if (record->parameters[1] ==
                MENU_REPEAT_SELECTOR_DECREMENT_COUNT) {
                entry->control.bits.repeatMode =
                    MENU_REPEAT_POLICY_DECREMENT_COUNT;
                entry->repeatCount = record->parameters[0];
            } else if (record->parameters[1] ==
                       MENU_REPEAT_SELECTOR_REPEAT_COUNT_LT_REMAINING) {
                entry->control.bits.repeatMode =
                    MENU_REPEAT_POLICY_REPEAT_COUNT_LT_REMAINING;
                entry->repeatCount = record->parameters[0];
            } else if (record->parameters[1] ==
                       MENU_REPEAT_SELECTOR_REMAINING_LT_REPEAT_COUNT) {
                entry->control.bits.repeatMode =
                    MENU_REPEAT_POLICY_REMAINING_LT_REPEAT_COUNT;
                entry->repeatCount = record->parameters[0];
            }
            break;
        case MENU_REPEAT_DIRECTIVE_KIND_REPEAT_TO_TARGET:
            entry->control.bits.loopMode = MENU_REPEAT_LOOP_MODE_TARGET_CAPABLE;
            if (record->parameters[1] ==
                MENU_REPEAT_SELECTOR_DECREMENT_COUNT) {
                entry->control.bits.repeatMode =
                    MENU_REPEAT_POLICY_DECREMENT_COUNT;
                entry->repeatCount = record->parameters[0];
                entry->repeatTargetRecordIndex = record->parameters[2];
            } else if (record->parameters[1] ==
                       MENU_REPEAT_SELECTOR_REPEAT_COUNT_LT_REMAINING) {
                entry->control.bits.repeatMode =
                    MENU_REPEAT_POLICY_REPEAT_COUNT_LT_REMAINING;
                entry->repeatCount = record->parameters[0];
                entry->repeatTargetRecordIndex = record->parameters[2];
            } else if (record->parameters[1] ==
                       MENU_REPEAT_SELECTOR_REMAINING_LT_REPEAT_COUNT) {
                entry->control.bits.repeatMode =
                    MENU_REPEAT_POLICY_REMAINING_LT_REPEAT_COUNT;
                entry->repeatCount = record->parameters[0];
                entry->repeatTargetRecordIndex = record->parameters[2];
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
            u8 movementEffectScale;

            memset(vector, 0, sizeof(vector));
            /* Save X across advancement, then reuse the scalar for step length. */
            value = entry->currentX;
            previousY = mnuEvaluateTimedValue(entry);
            if (mnuAdvanceRegistryWorkEntry(entry) != 0) {
                continue;
            }

            currentX = entry->currentX;
            vector[0] = currentX;
            currentY = (f32)(s32)mnuEvaluateTimedValue(entry);
            currentYInteger = (s32)currentY;
            vector[0] -= value;
            vector[1] = currentY - previousY;
            value = sdfVectorLength(vector);
            if (value * 10.0f >= 255.0f) {
                movementEffectScale = 255;
            } else {
                movementEffectScale = (u8)((u8)value * 10.0f);
            }
            /* Store the clamped movement step scale in the second byte. */
            entry->movementEffectScale = movementEffectScale;

            if (entry->currentX < -50.0f || (f32)parameters->width + 50.0f < entry->currentX ||
                (f32)currentYInteger < -200.0f ||
                (f32)parameters->height + 64.0f < (f32)currentYInteger) {
                entry->flagsBits.pendingDeactivate = 1;
            }

            if (context == 0) {
                func_00321688(entry->callback, runtimeList,
                              (s32)entry->currentX, currentYInteger,
                              entry->currentAngleRadians);
            }

            if (entry->flagsBits.halfRemainingCountReached) {
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
            mnuActiveWorkVisitorCallback(entry, (struct MnuShootingWork *)context);
        }
    }
    mnuActiveWorkVisitorCallback(mnuGetActiveEffectWorkEntry(), (struct MnuShootingWork *)context);
}

void func_00323748(MenuWorkEntry *entry, struct MnuShootingWork *context) {
    MenuRegistry *registry;

    if (entry->flagsBits.pendingStart) {
        mnuWorkEntryStartCallback(entry, context);
        entry->flagsBits.pendingStart = 0;
    }
    if (entry->flagsBits.updated) {
        entry->flagsBits.updated = 0;
        if (!entry->flagsBits.halfRemainingCountReached) {
            if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_REGISTRY_TABLE) {
                registry = mnuGetMenuRecordRegistryEntry(entry->tag);
                if (entry->remaining <= (registry->initialRemainingCount >> 1)) {
                    entry->flagsBits.halfRemainingCountReached = 1;
                }
            }
        }
    }
    if (entry->flagsBits.finished) {
        if ((entry->tag & MNU_WORK_TAG_CLASS_MASK) == MNU_WORK_TAG_MOVEMENT_TABLE) {
            entry->flags |= 0x4000;
            mnuWorkEntryFinishOrDeactivateCallback(entry, context);
        } else {
            entry->flagsBits.pendingDeactivate = 1;
        }
    }
    if (entry->flagsBits.pendingDeactivate) {
        mnuWorkEntryFinishOrDeactivateCallback(entry, context);
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
    mnuWorkEntryStartCallback = callback;
}

void mnuSetWorkEntryFinishOrDeactivateCallback(MenuWorkCallback callback) {
    mnuWorkEntryFinishOrDeactivateCallback = callback;
}

void mnuSetActiveWorkVisitor(MenuWorkCallback callback) {
    mnuActiveWorkVisitorCallback = callback;
}

void mnuSetRuntimeWorkHitCallback(MenuRuntimeWorkCallback callback) {
    mnuRuntimeWorkHitCallback = callback;
}

void mnuSetRuntimeRecordPairCallback(MenuRuntimePairCallback callback) {
    mnuRuntimeRecordPairCallback = callback;
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
    left = (s32)(work->currentX + (f32)parameters->hitOffsetX);
    top = (s32)(work->currentY + (f32)parameters->hitOffsetY);
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
                mnuRuntimeWorkHitCallback(record, work, context);
                return record;
            }
            if (work->inputCountdown == 0 && work->remaining != 0) {
                mnuRuntimeWorkHitCallback(record, work, context);
                return record;
            }
        }
    }
    return NULL;
}

s32 func_00323BB8(struct MnuShootingWork *context) {
    MenuRuntimeList *list = func_00321ED8();
    MenuRuntimeList *targets = func_00321EC8();
    MenuRuntimeRecord *record = list->records;
    MenuRuntimeRecord *target;
    s32 i;
    s32 j;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;

    for (i = 0; i < list->capacity; i++, record++) {
        if (record->state.word & MNU_WORK_ACTIVE) {
            if ((record->state.kind & 0xF) == 4) {
                f32 x = (record->rotatedOffsetX + record->displacementX) + record->baseX;
                f32 radiusX = (f32)D_0040B248[4][0];
                f32 y = (record->rotatedOffsetY + record->displacementY) + record->baseY;
                f32 radiusY = (f32)D_0040B248[4][1];

                left = (s32)(x - radiusX);
                right = (s32)(x + radiusX);
                top = (s32)(y - radiusY);
                bottom = (s32)(y + radiusY);
                target = targets->records;
                for (j = 0; j < targets->capacity; j++, target++) {
                    if ((target->state.word & 0xF01) == 0x101) {
                        s32 tx = (s32)((target->rotatedOffsetX + target->displacementX) + target->baseX);
                        s32 ty = (s32)((target->rotatedOffsetY + target->displacementY) + target->baseY);
                        s32 kind = target->state.kind & 0xF;
                        s32 targetRadiusX = D_0040B248[kind][0];
                        s32 targetRadiusY;

                        if (right < tx - targetRadiusX || tx + targetRadiusX < left) {
                            continue;
                        }
                        targetRadiusY = D_0040B248[kind][1];
                        if (bottom < ty - targetRadiusY || ty + targetRadiusY < top) {
                            continue;
                        }
                        mnuDeactivateListRecord(targets, target);
                        mnuRuntimeRecordPairCallback(record, target, context);
                    }
                }
            }
        }
    }
    return 0;
}

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
            entryX = (s32)entry->currentX;
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
                                mnuRuntimeWorkHitCallback(record, entry, context);
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

        entryX = (s32)entry->currentX;
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
        inputLeft = (s32)(input->currentX + (f32)parameters->hitOffsetX);
        inputTop = (s32)(input->currentY + (f32)parameters->hitOffsetY);
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

extern MenuInputActionSnapshotStorage D_0045C890;

/* Copy the twelve-byte input-action snapshot into its backing storage. */
void mnuSetInputActionSnapshot(const MenuInputActionSnapshot *snapshot) {
    memcpy(&D_0045C890.snapshot, snapshot, sizeof(*snapshot));
}

MenuWorkEntry *mnuGetActiveEffectWorkEntry(void) {
    return mnuActiveEffectEntry;
}

s32 func_003242D0(MenuWorkEntry *entry, u32 mode);

void mnuInitializeActiveEffectWorkEntry(MenuWorkEntry *entry) {
    memset(entry, 0, 0x48);
    func_003242D0(entry, 0);
    entry->flags |= 0x4010;
    entry->tag = MNU_WORK_TAG_MOVEMENT_TABLE;
    entry->remaining = 1;
    entry->currentAngleRadians = 1.5707963f;
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

    oldList = entry->callback;
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
    func_00320CE0(entry->callback, 0, (u32)record);

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
            func_00320CE0(entry->callback, 0, (u32)record);
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
            func_00320CE0(entry->callback, 0, (u32)record);

            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -20;
            record->offsetY = 5;
            record->flags.bits.direction = 4;
            record->flags.bits.hasRange = 1;
            func_00320CE0(entry->callback, 0, (u32)record);
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
            func_00320CE0(entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 10;
            tag.index = 40;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = 30;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 3;
            func_00320CE0(entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 10;
            tag.index = 40;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -30;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 4;
            func_00320CE0(entry->callback, 0, (u32)record);
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
            func_00320CE0(entry->callback, 0, (u32)record);

            tag.flags = 0x40;
            tag.group = -90;
            tag.duration = 5;
            tag.index = 30;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -20;
            record->offsetY = 5;
            record->flags.bits.direction = 4;
            record->flags.bits.hasRange = 1;
            func_00320CE0(entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 7;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = 40;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 3;
            func_00320CE0(entry->callback, 0, (u32)record);

            tag.flags = 0x45;
            tag.group = -90;
            tag.duration = 7;
            tag.index = 20;
            record = mnuCreateNamedRecord(&tag);
            record->offsetX = -40;
            record->offsetY = 5;
            record->flags.bits.hasRange = 1;
            record->flags.bits.direction = 4;
            func_00320CE0(entry->callback, 0, (u32)record);
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
    MenuWorkEntry *work = mnuActiveEffectEntry;
    u32 kindMask = 0;
    MenuInputActionSnapshot *input = &D_0045C890.snapshot;
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
        if (input->verticalNegative != 0) {
            movement[1] = -1.0f;
        } else if (input->verticalPositive != 0) {
            movement[1] = 1.0f;
        }
        if (input->horizontalNegative != 0) {
            movement[0] = -1.0f;
        } else if (input->horizontalPositive != 0) {
            movement[0] = 1.0f;
        }

        length = sdfVec3Normalize(movement);
        if (length != 0.0f) {
            sdfVectorScale((f32)progress->movementScale, movement);
            work->currentX += movement[0];
            work->currentY += movement[1];
            if (work->currentX < 16.0f) {
                work->currentX = 16.0f;
            }
            if (work->currentY < 16.0f) {
                work->currentY = 16.0f;
            }
            if (work->currentX > (f32)(parameters->width - 16)) {
                work->currentX = (f32)(parameters->width - 16);
            }
            if (work->currentY > (f32)(parameters->height - 16)) {
                work->currentY = (f32)(parameters->height - 16);
            }
        }

        kindMask = 0;
        if (input->actionSlot0 != 0) {
            kindMask = 0x21;
        }
        if (input->actionSlot1 != 0) {
            kindMask |= 0x10;
        }
    }

    func_00321798(work->callback,
                  func_00321ED8(), kindMask,
                  (s32)work->currentX, (s32)work->currentY, kindMask,
                  work->currentAngleRadians);

    func_00322418();

    {
        MenuWorkFlags flags;
        flags.word = work->flags;
        if (flags.bits.halfRemainingCountReached) {
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
    func_00320CE0(context->callback, 0,
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
        entry->currentX = -200.0f;
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
            entry->currentX = 120.0f;
            positionY = 400.0f;
            entry->currentY = positionY;
            D_004389B8 = 0;
        }
        break;
    case 3:
        positionY = entry->currentY;
        positionY -= 2.0f;
        if (++D_004389B8 >= 31) {
            phase = 1;
        }
        D_004389B4 = phase;
        entry->currentY = positionY;
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


INCLUDE_ASM(const s32, "game/code_003224C8", func_00324D50);

void mnuReleaseEffectPairAndNode(u32 node) {
    if (node != 0) {
        dds3DestroyCallbackNodeAfterLastNotification((MnuCallbackList *)*(u32 *)node);
        dds3DestroyCallbackNodeAfterLastNotification((MnuCallbackList *)*(u32 *)(node + 4));
        func_0035A880(node);
    }
}

INCLUDE_SDATA(const s32, "game/code_003224C8", mnuWorkEntryStartCallback);

INCLUDE_SDATA(const s32, "game/code_003224C8", mnuWorkEntryFinishOrDeactivateCallback);

INCLUDE_SDATA(const s32, "game/code_003224C8", mnuActiveWorkVisitorCallback);

INCLUDE_SDATA(const s32, "game/code_003224C8", mnuRuntimeWorkHitCallback);

INCLUDE_SDATA(const s32, "game/code_003224C8", mnuRuntimeRecordPairCallback);

INCLUDE_SDATA(const s32, "game/code_003224C8", D_004389B4);

INCLUDE_SDATA(const s32, "game/code_003224C8", D_004389B8);

