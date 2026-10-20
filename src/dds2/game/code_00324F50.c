#include "common.h"
#include "dds_nested_resource.h"
#include "mnu_callback_list.h"
#include "sdf_sprite_record.h"
#include "sdf_word_array_record.h"
#include "sdf_resource.h"

extern f32 sdfVec3Normalize(f32 *);

extern void *sdfAllocHeapMemory(u32);


extern void (*sdfTickCallback)(void);

extern f32 sdfVec3DotNormalized(void *, void *);

extern f64 cos(f64);

extern f64 sin(f64);

extern f64 sqrt(f64);

/* Keep all packed bytes opaque while the clone copies the complete header. */
typedef struct SdfResourceVectorRecord1C {
    u8 opaque00[0x1C];
} SdfResourceVectorRecord1C;


typedef char SdfResourceVectorRecord1CSizeCheck[(sizeof(SdfResourceVectorRecord1C) == 0x1C) ? 1 : -1];

extern void *memcpy(void *, const void *, u32);

/* Append a value to the first resource list in a two-list owner. */ void dds3AppendFirstResourceListValue(MnuCallbackListPair *lists, u32 option);

/* Allocate bytes, track the handle in the second owner list, and return it. */
void *sdfAllocateOwnedResourceBuffer(MnuCallbackListPair *owner, u32 bytes) {
    void *handle;

    handle = sdfAllocHeapMemory(bytes);
    dds3AppendIndexedListNode(owner->second, 0, (u32)handle);
    return handle;
}

/* Unlink the owner list node that carries the handle and notify its callbacks. */
void mnuRemoveLinkedResourceByHandle(MnuCallbackListPair *owner, u32 handle) {
    SdfListNode *node = mnuFindResourceNodeByHandle(owner->second, handle);

    dds3RemoveListNodeAndNotify(owner->second, node);
}

extern void *memset(void *, int, unsigned int);

DdsNestedHeader *sdfCloneNestedResourceRecord(MnuCallbackListPair *owner, MnuCallbackList **list) {
    SdfListNode *node;
    DdsNestedHeader *buffer;
    DdsNestedGroup *group;
    DdsCountedPayload *entries;
    u8 *cursor;
    s32 totalSize;
    s32 i;
    s32 j;

    node = (*list)->head;
    if (node == NULL) {
        return NULL;
    }
    totalSize = 0;
    do {
        totalSize += dds3MeasureMenuRecord((DdsNestedGroup *)node->value);
        node = node->next;
    } while (node != NULL);
    buffer = sdfAllocateOwnedResourceBuffer(owner, totalSize + sizeof(*buffer));
    memset(buffer, 0, totalSize);
    buffer->groupCount = (*list)->count;
    buffer->groups = (DdsNestedGroup *)(buffer + 1);
    cursor = (u8 *)buffer->groups;
    node = (*list)->head;
    if (node != NULL) {
        do {
            group = (DdsNestedGroup *)node->value;
            memcpy(cursor, group, sizeof(*group));
            cursor += sizeof(*group);
            node = node->next;
        } while (node != NULL);
    }
    group = buffer->groups;
    for (i = 0; i < buffer->groupCount; i++, group++) {
        memcpy(cursor, group->first, group->firstCount * sizeof(*entries));
        group->first = (DdsCountedPayload *)cursor;
        entries = group->first;
        cursor += group->firstCount * sizeof(*entries);
        for (j = 0; j < group->firstCount; j++, entries++) {
            memcpy(cursor, entries->data, entries->count * 8);
            entries->data = cursor;
            cursor += entries->count * 8;
        }
        memcpy(cursor, group->second, group->secondCount * sizeof(*entries));
        group->second = (DdsCountedPayload *)cursor;
        entries = group->second;
        cursor += group->secondCount * sizeof(*entries);
        for (j = 0; j < group->secondCount; j++, entries++) {
            memcpy(cursor, entries->data, entries->count * 8);
            entries->data = cursor;
            cursor += entries->count * 8;
        }
    }
    return buffer;
}

/* Clone each nested group into its own owned buffer, relocating the payload
 * pointers of its two counted lists into the copy. Return the resource-list owner. */
MnuCallbackListPair *func_003251C0(const DdsNestedHeader *header) {
    MnuCallbackListPair *owner = mnuCreateCallbackPair();
    DdsNestedGroup *group = header->groups;
    s32 i;

    for (i = 0; i < header->groupCount; i++, group++) {
        s32 size = dds3MeasureMenuRecord(group);
        u8 *cursor = sdfAllocateOwnedResourceBuffer(owner, size);
        DdsNestedGroup *copy;
        DdsCountedPayload *entry;
        s32 j;

        memset(cursor, 0, size);
        copy = (DdsNestedGroup *)cursor;
        dds3AppendFirstResourceListValue(owner, (u32)cursor);
        memcpy(cursor, group, sizeof(*group));
        cursor += sizeof(*group);
        memcpy(cursor, group->first, group->firstCount * sizeof(*entry));
        copy->first = (DdsCountedPayload *)cursor;
        cursor += group->firstCount * sizeof(*entry);
        entry = copy->first;
        for (j = 0; j < copy->firstCount; j++, entry++) {
            memcpy(cursor, entry->data, entry->count * 8);
            entry->data = cursor;
            cursor += entry->count * 8;
        }
        memcpy(cursor, group->second, group->secondCount * sizeof(*entry));
        copy->second = (DdsCountedPayload *)cursor;
        cursor += group->secondCount * sizeof(*entry);
        entry = copy->second;
        for (j = 0; j < group->secondCount; j++, entry++) {
            memcpy(cursor, entry->data, entry->count * 8);
            entry->data = cursor;
            cursor += entry->count * 8;
        }
    }
    return owner;
}

/* Pack both callback lists into separate header and eight-byte payload regions. */
DdsNestedGroup *sdfPackCountedPayloadLists(MnuCallbackListPair *owner, MnuCallbackList **firstList,
                             MnuCallbackList **secondList) {
    u32 firstBytes = 0;
    u32 secondBytes;
    u32 totalBytes;
    SdfListNode *node;
    DdsCountedPayload *source;
    DdsCountedPayload *destination;
    DdsNestedGroup *group;
    u8 *payload;

    node = (*firstList)->head;
    while (node != NULL) {
        source = node->value;
        firstBytes += source->count * 8 + sizeof(*source);
        node = node->next;
    }
    secondBytes = 0;
    node = (*secondList)->head;
    while (node != NULL) {
        source = node->value;
        secondBytes += source->count * 8 + sizeof(*source);
        node = node->next;
    }
    totalBytes = firstBytes + secondBytes + sizeof(*group);
    group = sdfAllocateOwnedResourceBuffer(owner, totalBytes);
    memset(group, 0, totalBytes);
    group->firstCount = (*firstList)->count;
    group->secondCount = (*secondList)->count;
    group->first = (DdsCountedPayload *)(group + 1);
    group->second = (DdsCountedPayload *)((u8 *)group->first + firstBytes);

    destination = group->first;
    node = (*firstList)->head;
    while (node != NULL) {
        source = node->value;
        memcpy(destination, source, sizeof(*source));
        node = node->next;
        destination++;
    }
    payload = (u8 *)destination;
    destination = group->first;
    node = (*firstList)->head;
    while (node != NULL) {
        source = node->value;
        memcpy(payload, source->data, source->count * 8);
        destination->data = payload;
        payload += source->count * 8;
        destination++;
        node = node->next;
    }

    destination = group->second;
    node = (*secondList)->head;
    while (node != NULL) {
        source = node->value;
        memcpy(destination, source, sizeof(*source));
        node = node->next;
        destination++;
    }
    payload = (u8 *)destination;
    destination = group->second;
    node = (*secondList)->head;
    while (node != NULL) {
        source = node->value;
        memcpy(payload, source->data, source->count * 8);
        destination->data = payload;
        payload += source->count * 8;
        destination++;
        node = node->next;
    }
    return group;
}

typedef struct SdfFilterRecord {
    u8 enabled;
    u8 data[7];
} SdfFilterRecord;

typedef struct SdfFilteredRecords {
    u32 count;
    SdfFilterRecord *records;
} SdfFilteredRecords;

/* Copy enabled eight-byte records into owner-managed contiguous storage.
 * Return a count/pointer header containing only nonzero-enabled records. */
SdfFilteredRecords *sdfCloneEnabledFilterRecords(MnuCallbackListPair *owner, SdfFilterRecord *records, u32 count) {
    u32 enabled = 0;
    u32 i;
    s32 size;
    SdfFilteredRecords *result;
    SdfFilterRecord *out;

    for (i = 0; i < count; i++) {
        if (records[i].enabled != 0) {
            enabled++;
        }
    }
    size = sizeof(*result) + enabled * sizeof(*records);
    result = sdfAllocateOwnedResourceBuffer(owner, size);
    memset(result, 0, size);
    out = (SdfFilterRecord *)(result + 1);
    result->count = enabled;
    result->records = out;
    for (i = 0; i < count; i++) {
        if (records[i].enabled != 0) {
            *out++ = records[i];
        }
    }
    return result;
}

MnuCallbackListPair *sdfCloneOwnedResourceRecords(const SdfWordArrayRecord *source, s32 count) {
    MnuCallbackListPair *owner = mnuCreateCallbackPair();

    if (count != 0) {
        do {
            SdfWordArrayRecord *copy = (SdfWordArrayRecord *)sdfAllocateOwnedResourceBuffer(owner, sizeof(*copy));

            *copy = *source;
            copy->items = (u32 *)sdfAllocateOwnedResourceBuffer(owner, copy->itemCount * sizeof(*copy->items));
            memcpy(copy->items, source->items, copy->itemCount * sizeof(*copy->items));
            copy->info = (SdfWordArrayInfo *)sdfAllocateOwnedResourceBuffer(owner, sizeof(*copy->info));
            *copy->info = *source->info;
            dds3AppendFirstResourceListValue(owner, (u32)copy);
            source++;
            count--;
        } while (count != 0);
    }
    return owner;
}

typedef struct SdfSpriteListRecord {
    u8 unk00[0xB];
    u8 itemCount;              /* 0x0B */
    DdsNestedHeader *nested;   /* 0x0C */
    u32 *items;                /* 0x10 */
    SdfVec4 *vector;           /* 0x14 */
    u8 unk18[4];
} SdfSpriteListRecord;

/* Deep-copy count sprite records, each into one owned buffer: the header, its
 * item words, its vector and, when present, the nested groups with payloads. */
MnuCallbackListPair *sdfCloneOwnedSpriteRecordList(const SdfSpriteListRecord *source, s32 count) {
    MnuCallbackListPair *owner = mnuCreateCallbackPair();

    while (count > 0) {
        DdsNestedHeader *nested = source->nested;
        SdfSpriteListRecord *copy;
        DdsNestedGroup *group;
        DdsNestedGroup *groupCopy;
        DdsCountedPayload *entry;
        DdsCountedPayload *entryCopy;
        u8 *cursor;
        s32 size;
        s32 i;
        s32 j;

        size = sizeof(*copy) + source->itemCount * sizeof(*copy->items) + sizeof(*copy->vector);
        if (nested != NULL) {
            size += sizeof(*nested);
            group = nested->groups;
            for (i = 0; i < nested->groupCount; i++, group++) {
                size += dds3MeasureMenuRecord(group);
            }
        }
        cursor = sdfAllocateOwnedResourceBuffer(owner, size);
        dds3AppendFirstResourceListValue(owner, (u32)cursor);
        memset(cursor, 0, size);
        copy = (SdfSpriteListRecord *)cursor;
        memcpy(cursor, source, sizeof(*copy));
        cursor += sizeof(*copy);
        memcpy(cursor, source->items, source->itemCount * sizeof(*copy->items));
        copy->items = (u32 *)cursor;
        cursor += source->itemCount * sizeof(*copy->items);
        memcpy(cursor, source->vector, sizeof(*copy->vector));
        copy->vector = (SdfVec4 *)cursor;
        cursor += sizeof(*copy->vector);
        if (nested == NULL) {
            count--;
            source++;
            continue;
        }
        memcpy(cursor, source->nested, sizeof(*nested));
        copy->nested = (DdsNestedHeader *)cursor;
        cursor += sizeof(*nested);
        nested = source->nested;
        memcpy(cursor, nested->groups, nested->groupCount * sizeof(*group));
        copy->nested->groups = (DdsNestedGroup *)cursor;
        cursor += nested->groupCount * sizeof(*group);
        group = source->nested->groups;
        groupCopy = copy->nested->groups;
        for (i = 0; i < nested->groupCount; i++, group++, groupCopy++) {
            memcpy(cursor, group->first, group->firstCount * sizeof(*entry));
            groupCopy->first = (DdsCountedPayload *)cursor;
            cursor += group->firstCount * sizeof(*entry);
            entry = group->first;
            entryCopy = groupCopy->first;
            for (j = 0; j < group->firstCount; j++, entry++, entryCopy++) {
                memcpy(cursor, entry->data, entry->count * 8);
                entryCopy->data = cursor;
                cursor += entry->count * 8;
            }
            memcpy(cursor, group->second, group->secondCount * sizeof(*entry));
            groupCopy->second = (DdsCountedPayload *)cursor;
            cursor += group->secondCount * sizeof(*entry);
            entry = group->second;
            entryCopy = groupCopy->second;
            for (j = 0; j < group->secondCount; j++, entry++, entryCopy++) {
                memcpy(cursor, entry->data, entry->count * 8);
                entryCopy->data = cursor;
                cursor += entry->count * 8;
            }
        }
        count--;
        source++;
    }
    return owner;
}

/* Deep-copy count basic sprite headers and their sixteen-byte vectors.
 * Return the resource-list owner; each copied header points to its own vector. */
MnuCallbackListPair *sdfCloneSpriteRecords(const SdfResourceVectorRecord1C *source, s32 count) {
    MnuCallbackListPair *owner = mnuCreateCallbackPair();

    while (count != 0) {
        SdfResourceVectorRecord1C *copy =
            (SdfResourceVectorRecord1C *)sdfAllocateOwnedResourceBuffer(owner, sizeof(*copy) + sizeof(SdfVec4));
        SdfVec4 *vector;

        memset(copy, 0, sizeof(*copy) + sizeof(*vector));
        *copy = *source;
        vector = (SdfVec4 *)(copy + 1);
        ((SdfSpriteVectorRecord *)copy)->vector = vector;
        *vector = *((SdfSpriteVectorRecord *)source)->vector;
        dds3AppendFirstResourceListValue(owner, (u32)copy);
        source++;
        count--;
    }
    return owner;
}

/* Deep-copy count extended sprite headers and their sixteen-byte vectors.
 * Return the resource-list owner with each copied vector stored after its header. */
MnuCallbackListPair *sdfCloneExtendedSpriteRecords(const SdfSpriteExtendedRecord *source, s32 count) {
    MnuCallbackListPair *owner = mnuCreateCallbackPair();

    while (count != 0) {
        SdfSpriteExtendedRecord *copy =
            (SdfSpriteExtendedRecord *)sdfAllocateOwnedResourceBuffer(owner, sizeof(*copy) + sizeof(SdfVec4));
        SdfVec4 *vector;

        memset(copy, 0, sizeof(*copy) + sizeof(*vector));
        *copy = *source;
        vector = (SdfVec4 *)(copy + 1);
        copy->vector = vector;
        *vector = *source->vector;
        dds3AppendFirstResourceListValue(owner, (u32)copy);
        source++;
        count--;
    }
    return owner;
}

typedef struct SdfRelocatedResource {
    u32 unk00;
    u16 firstCount;
    u16 secondCount;
    void *data;
    DdsCountedPayload *groups;
    u32 unk10;
} SdfRelocatedResource;

extern u32 *dds3PackNestedResourceRecords(MnuCallbackList **);

extern void dds3ApplyNamedRelocations(u32 *);

extern u32 *dds3WritePendingNamedReferenceValues(u32 *);

extern void dds3ApplyRelocationOffsets(void *, void *, void *, u32);

extern void sdfFreeHeapMemory(void *);

SdfRelocatedResource *sdfCloneRelocatedResourceGroups(MnuCallbackListPair *owner, MnuCallbackList **source,
                                  MnuCallbackList **groups) {
    SdfRelocatedResource *result;
    SdfListNode *node;
    DdsCountedPayload *payload;
    DdsCountedPayload *record;
    u32 *sourceHeader;
    u32 *relocationHeader;
    void *data;
    void *relocations;
    u8 *cursor;
    u32 size;

    if (source == NULL || groups == NULL) {
        return NULL;
    }
    size = 0;
    result = sdfAllocateOwnedResourceBuffer(owner, sizeof(*result));
    memset(result, 0, sizeof(*result));
    sourceHeader = dds3PackNestedResourceRecords(source);
    dds3ApplyNamedRelocations(sourceHeader);
    relocationHeader = dds3WritePendingNamedReferenceValues(sourceHeader);
    data = sdfAllocateOwnedResourceBuffer(owner, *sourceHeader);
    memset(data, 0, *sourceHeader);
    relocations = sdfAllocateOwnedResourceBuffer(owner, *relocationHeader);
    memset(relocations, 0, *relocationHeader);
    dds3ApplyRelocationOffsets(data, data, relocations, *relocationHeader);
    sdfFreeHeapMemory(relocations);
    result->data = data;
    result->firstCount = (*source)->count;
    for (node = (*groups)->head; node != NULL; node = node->next) {
        payload = (DdsCountedPayload *)node->value;
        size += payload->count * 8 + sizeof(*payload);
    }
    result->groups = sdfAllocateOwnedResourceBuffer(owner, size);
    memset(result->groups, 0, size);
    result->secondCount = (*groups)->count;
    record = result->groups;
    for (node = (*groups)->head; node != NULL; node = node->next) {
        payload = (DdsCountedPayload *)node->value;
        memcpy(record, payload, sizeof(*record));
        record++;
    }
    cursor = (u8 *)record;
    record = result->groups;
    for (node = (*groups)->head; node != NULL; node = node->next) {
        payload = (DdsCountedPayload *)node->value;
        memcpy(cursor, payload->data, payload->count * 8);
        record->data = cursor;
        record++;
        cursor += payload->count * 8;
    }
    return result;
}

/* Rotate y/z about the X axis by angle radians; leave x and any fourth lane.
 * Preserve the scalar double-precision libm calls and coordinate temporary. */
void sdfVectorRotateX(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[1] = vector[1] * cos(angle) + vector[2] * sin(angle);
    rotated[2] = vector[1] * -sin(angle) + vector[2] * cos(angle);
    vector[1] = rotated[1];
    vector[2] = rotated[2];
}

/* Rotate x/z about the Y axis by angle radians; leave y and any fourth lane. */
void sdfVectorRotateY(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[0] = vector[0] * cos(angle) - vector[2] * sin(angle);
    rotated[2] = vector[0] * sin(angle) + vector[2] * cos(angle);
    vector[0] = rotated[0];
    vector[2] = rotated[2];
}

/* Rotate x/y about the Z axis by angle radians; leave z and any fourth lane. */
void sdfVectorRotateZ(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[0] = vector[0] * cos(angle) + vector[1] * sin(angle);
    rotated[1] = vector[0] * -sin(angle) + vector[1] * cos(angle);
    vector[0] = rotated[0];
    vector[1] = rotated[1];
}

/* Normalize axis and rotate vector's first three lanes by angle radians.
 * The axis-angle matrix, scalar libm calls and normalization behavior are retained. */
void sdfVectorRotateAxis(f32 *vector, f32 *axis, f32 angle) {
    SdfVec4 normalized;
    SdfVec4 source;
    SdfVec4 temporary;
    f32 matrix[9];

    memset(&source, 0, sizeof(source));
    source.x = axis[0];
    source.y = axis[1];
    source.z = axis[2];
    normalized = source;
    memset(&temporary, 0, sizeof(temporary));
    temporary.x = vector[0];
    temporary.y = vector[1];
    temporary.z = vector[2];
    source = temporary;
    sdfVec3Normalize(&normalized.x);
    matrix[0] = normalized.x * normalized.x * (1.0f - cos(angle)) + cos(angle);
    matrix[1] = normalized.x * normalized.y * (1.0f - cos(angle)) - normalized.z * sin(angle);
    matrix[2] = normalized.x * normalized.z * (1.0f - cos(angle)) + normalized.y * sin(angle);
    matrix[3] = normalized.y * normalized.x * (1.0f - cos(angle)) + normalized.z * sin(angle);
    matrix[4] = normalized.y * normalized.y * (1.0f - cos(angle)) + cos(angle);
    matrix[5] = normalized.y * normalized.z * (1.0f - cos(angle)) - normalized.x * sin(angle);
    matrix[6] = normalized.z * normalized.x * (1.0f - cos(angle)) - normalized.y * sin(angle);
    matrix[7] = normalized.z * normalized.y * (1.0f - cos(angle)) + normalized.x * sin(angle);
    matrix[8] = normalized.z * normalized.z * (1.0f - cos(angle)) + cos(angle);
    vector[0] = source.x * matrix[0] + source.y * matrix[3] + source.z * matrix[6];
    vector[1] = source.x * matrix[1] + source.y * matrix[4] + source.z * matrix[7];
    vector[2] = source.x * matrix[2] + source.y * matrix[5] + source.z * matrix[8];
}

void sdfVectorAdd(float *vector, float *delta) {
    *vector = *vector + *delta;
    vector[1] = vector[1] + delta[1];
    vector[2] = vector[2] + delta[2];
}

void sdfVectorSubtract(float *vector, float *delta) {
    *vector = *vector - *delta;
    vector[1] = vector[1] - delta[1];
    vector[2] = vector[2] - delta[2];
}

void sdfVectorAddComponents(float *vector, float x, float y, float z) {
    vector[0] += x;
    vector[1] += y;
    vector[2] += z;
}

void sdfVec3SetComponents(float *vector, float x, float y, float z) {
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
}

void sdfVectorScale(float factor, float *vector) {
    *vector = *vector * factor;
    vector[1] = vector[1] * factor;
    vector[2] = vector[2] * factor;
}

extern f32 sdfVectorLength(const f32 *);

/* Normalize the first three components; a zero-length vector stays unchanged. */
f32 sdfVec3Normalize(f32 *vector) {
    f32 length = sdfVectorLength(vector);

    if (length == 0.0f) {
        return 0.0f;
    }
    vector[0] = vector[0] / length;
    vector[1] = vector[1] / length;
    vector[2] = vector[2] / length;
    return length;
}

f32 sdfVectorLength(const f32 *vector) {
    return sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

/* Dot product of two normalized 3D directions (w is ignored). */
f32 sdfVec3DotNormalized(void *first, void *second) {
    SdfVec4 firstNormalized = *(SdfVec4 *)first;
    SdfVec4 secondNormalized = *(SdfVec4 *)second;

    sdfVec3Normalize(&firstNormalized.x);
    sdfVec3Normalize(&secondNormalized.x);
    return firstNormalized.x * secondNormalized.x + firstNormalized.y * secondNormalized.y + firstNormalized.z * secondNormalized.z;
}
