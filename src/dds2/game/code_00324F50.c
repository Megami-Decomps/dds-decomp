#include "common.h"
#include "dds_nested_resource.h"
#include "mnu_callback_list.h"
#include "sdf_resource.h"

typedef struct SdfVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} SdfVec4;

extern f32 sdfVec3Normalize(f32 *);

extern void *func_0035A828(u32);

extern u64 func_00325790(u64, u32);

extern void (*sdfTickCallback)(void);

extern f32 sdfVec3DotNormalized(void *, void *);

extern f64 cos(f64);

extern f64 sin(f64);

extern f64 func_003532A0(f64);

typedef struct SdfResourceInfo {
    u32 word[4];
} SdfResourceInfo;

typedef struct SdfResourceRecord {
    u8 unk00[0xE];
    u16 count;
    u32 *items;
    SdfResourceInfo *info;
} SdfResourceRecord;

/* Serialized 0x1C-byte header; its vector-pointer word is at byte 0x18. */
typedef struct SdfResourceVectorRecord1C {
    u8 data[0x1C];
} SdfResourceVectorRecord1C;

typedef struct SdfResourceVectorRecord30 {
    u8 data[0x2C];
    SdfVec4 *vector;
} SdfResourceVectorRecord30;

extern u32 *func_00324D50(void);

extern void *memcpy(void *, const void *, u32);

/* Reset the first resource list in a two-list owner. */ void func_00324DF8(u32 *lists, u32 option);

void *func_00324F50(u32 *owner, u32 resource) {
    void *handle;

    handle = func_0035A828(resource);
    func_00320CE0((MnuCallbackList *)owner[1], 0, (u32)handle);
    return handle;
}

INCLUDE_ASM(const s32, "game/code_00324F50", mnuRemoveLinkedResourceByHandle);

DdsNestedHeader *sdfCloneNestedResourceRecord(u32 *owner, MnuCallbackList **list) {
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
    buffer = func_00324F50(owner, totalSize + sizeof(*buffer));
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

INCLUDE_ASM(const s32, "game/code_00324F50", func_003251C0);

/* Pack both callback lists into separate header and eight-byte payload regions. */
DdsNestedGroup *func_00325398(u32 *owner, MnuCallbackList **firstList,
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
    group = func_00324F50(owner, totalBytes);
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

SdfFilteredRecords *func_003255A0(u32 *owner, SdfFilterRecord *records, u32 count) {
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
    result = func_00324F50(owner, size);
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

u32 *sdfCloneOwnedResourceRecords(const SdfResourceRecord *source, s32 count) {
    u32 *owner = func_00324D50();

    if (count != 0) {
        do {
            SdfResourceRecord *copy = (SdfResourceRecord *)func_00324F50(owner, sizeof(*copy));

            *copy = *source;
            copy->items = (u32 *)func_00324F50(owner, copy->count * sizeof(*copy->items));
            memcpy(copy->items, source->items, copy->count * sizeof(*copy->items));
            copy->info = (SdfResourceInfo *)func_00324F50(owner, sizeof(*copy->info));
            *copy->info = *source->info;
            func_00324DF8(owner, (u32)copy);
            source++;
            count--;
        } while (count != 0);
    }
    return owner;
}

INCLUDE_ASM(const s32, "game/code_00324F50", func_00325790);

u32 *func_00325AB8(const SdfResourceVectorRecord1C *source, s32 count) {
    u32 *owner = func_00324D50();

    while (count != 0) {
        SdfResourceVectorRecord1C *copy =
            (SdfResourceVectorRecord1C *)func_00324F50(owner, sizeof(*copy) + sizeof(SdfVec4));
        SdfVec4 *vector;

        memset(copy, 0, sizeof(*copy) + sizeof(*vector));
        *copy = *source;
        vector = (SdfVec4 *)(copy + 1);
        *(SdfVec4 **)&copy->data[0x18] = vector;
        *vector = **(SdfVec4 * const *)&source->data[0x18];
        func_00324DF8(owner, (u32)copy);
        source++;
        count--;
    }
    return owner;
}

u32 *func_00325BB0(const SdfResourceVectorRecord30 *source, s32 count) {
    u32 *owner = func_00324D50();

    while (count != 0) {
        SdfResourceVectorRecord30 *copy =
            (SdfResourceVectorRecord30 *)func_00324F50(owner, sizeof(*copy) + sizeof(SdfVec4));
        SdfVec4 *vector;

        memset(copy, 0, sizeof(*copy) + sizeof(*vector));
        *copy = *source;
        vector = (SdfVec4 *)(copy + 1);
        copy->vector = vector;
        *vector = *source->vector;
        func_00324DF8(owner, (u32)copy);
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

extern u32 *func_0031FA60(MnuCallbackList **);

extern void dds3ApplyNamedRelocations(u32 *);

extern u32 *dds3WritePendingNamedReferenceValues(u32 *);

extern void dds3ApplyRelocationOffsets(void *, void *, void *, u32);

extern void func_0035A880(void *);

SdfRelocatedResource *sdfCloneRelocatedResourceGroups(u32 *owner, MnuCallbackList **source,
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
    result = func_00324F50(owner, sizeof(*result));
    memset(result, 0, sizeof(*result));
    sourceHeader = func_0031FA60(source);
    dds3ApplyNamedRelocations(sourceHeader);
    relocationHeader = dds3WritePendingNamedReferenceValues(sourceHeader);
    data = func_00324F50(owner, *sourceHeader);
    memset(data, 0, *sourceHeader);
    relocations = func_00324F50(owner, *relocationHeader);
    memset(relocations, 0, *relocationHeader);
    dds3ApplyRelocationOffsets(data, data, relocations, *relocationHeader);
    func_0035A880(relocations);
    result->data = data;
    result->firstCount = (*source)->count;
    for (node = (*groups)->head; node != NULL; node = node->next) {
        payload = (DdsCountedPayload *)node->value;
        size += payload->count * 8 + sizeof(*payload);
    }
    result->groups = func_00324F50(owner, size);
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

void func_00325EC8(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[1] = vector[1] * cos(angle) + vector[2] * sin(angle);
    rotated[2] = vector[1] * -sin(angle) + vector[2] * cos(angle);
    vector[1] = rotated[1];
    vector[2] = rotated[2];
}

void func_00326018(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[0] = vector[0] * cos(angle) - vector[2] * sin(angle);
    rotated[2] = vector[0] * sin(angle) + vector[2] * cos(angle);
    vector[0] = rotated[0];
    vector[2] = rotated[2];
}

void func_00326158(f32 *vector, f32 angle) {
    f32 rotated[4];

    rotated[0] = vector[0] * cos(angle) + vector[1] * sin(angle);
    rotated[1] = vector[0] * -sin(angle) + vector[1] * cos(angle);
    vector[0] = rotated[0];
    vector[1] = rotated[1];
}

/* Rotate a vector about a normalized axis using an axis-angle matrix. */
void func_003262A8(f32 *vector, f32 *axis, f32 angle) {
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
    return func_003532A0(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

/* Dot product of two normalized 3D directions (w is ignored). */
f32 sdfVec3DotNormalized(void *first, void *second) {
    SdfVec4 firstNormalized = *(SdfVec4 *)first;
    SdfVec4 secondNormalized = *(SdfVec4 *)second;

    sdfVec3Normalize(&firstNormalized.x);
    sdfVec3Normalize(&secondNormalized.x);
    return firstNormalized.x * secondNormalized.x + firstNormalized.y * secondNormalized.y + firstNormalized.z * secondNormalized.z;
}
