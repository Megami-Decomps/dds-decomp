#include "sdf_gs_alpha_register_api.h"
#include "common.h"
#include "sdf_packet_header.h"
#include "eff_object_node.h"
#include "sdf_trig.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_texture_draw_packet.h"
#include "itf_draw_grid.h"
#include "eff_ref_obj.h"
#include "sdf_heap_api.h"
#include "ps2asm.h"
#include "eff.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "eff_record_bucket.h"
#include "eff_owner_records.h"
#include "sdf.h"
#include "sdf_texture_file.h"
#include "btl.h"
#include "file_request_api.h"
/* Declared before first use (were implicit int). */
extern void * memset(void *, s32, u32);
extern sdfGrOt_t sdfGrOtTable[];
extern EffRecordBucket D_00400508[];

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

extern char D_004387E8[];
extern s32 sprintf(char *, const char *, ...);

#define EFF_RESOURCE_PATH_BYTES 0x80
#define EFF_RESOURCE_KEEP_ALLOCATION 1
#define EFF_RESOURCE_TRANSIENT 0
#define EFF_OWNER_LIST_BYTES 0x44
#define EFF_RECORD_BUCKETS 16
#define EFF_RECORD_LAST_BUCKET 15
#define EFF_STATUS_RECORD_BYTES 0x24
#define EFF_PACKED_STATUS_HEADER_BYTES 0x20
#define EFF_STATUS_PARAM_STRIDE 0x18
#define EFF_STATUS_PARAM_HEADER_BYTES 4
#define EFF_BATCH_HEADER_BYTES 0xC
#define EFF_SLOT_WORK_BYTES 0xA0
#define EFF_SLOT_PHASE_ZERO 0
#define EFF_SLOT_PHASE_ONE 0x10000
#define EFF_PAYLOAD_RECORD_BYTES 0x6C
#define EFF_RESOURCE_TABLE_ENTRY_BYTES 8

/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, s32 keepAllocation) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    struct sdfMemHandle_t *allocation;
    EffectSlotSet *instance;
    sprintf(path, D_004387E8, base, name);
    allocation = sdfDevLoadFileEx(path, &sourceAddress, 0);
    instance = effCreateResourceSlotSetFromAllocation(allocation, keepAllocation);
    if (keepAllocation == EFF_RESOURCE_TRANSIENT) {
        sdfMemFreeDirect(allocation);
    }
    return instance;
}

/* Publish the instance, release its source allocation, then clean up the completed file job. */
void effCompleteTransientResourceJob(struct FileRequest *job, u32 *outInstance) {
    struct sdfMemHandle_t *allocation;
    EffectSlotSet *instance;

    allocation = fileGetMemHandle(job);
    instance = effCreateResourceSlotSetFromAllocation(allocation, EFF_RESOURCE_TRANSIENT);
    *outInstance = (u32)instance;
    sdfMemFreeDirect(allocation);
    fileHandleFree(job);
}

/* Publish the instance without releasing its source allocation, then clean up the file job. */
void effCompleteRetainedResourceJob(struct FileRequest *job, u32 *outInstance) {
    struct sdfMemHandle_t *allocation;
    EffectSlotSet *instance;

    allocation = fileGetMemHandle(job);
    instance = effCreateResourceSlotSetFromAllocation(allocation, EFF_RESOURCE_KEEP_ALLOCATION);
    *outInstance = (u32)instance;
    fileHandleFree(job);
}

/* Clear the output first; only mode one selects the retained-allocation completion path. */
void effRequestResourceByMode(const char *prefix, const char *name, s32 mode, u32 *outInstance) {
    char path[EFF_RESOURCE_PATH_BYTES];
    sprintf(path, D_004387E8, prefix, name);
    *outInstance = 0;
    if (mode == EFF_RESOURCE_KEEP_ALLOCATION) {
        fileLoadEx(path, 0, (s32)effCompleteRetainedResourceJob, (s32)outInstance);
    } else {
        fileLoadEx(path, 0, (s32)effCompleteTransientResourceJob, (s32)outInstance);
    }
}

/* Build mapped records from the retained source address, then release the original file allocation. */
EffMappedResource *effLoadMappedResource(const char *base, const char *name) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    struct sdfMemHandle_t *allocation;
    EffMappedResource *mappedResource;
    sprintf(path, D_004387E8, base, name);
    allocation = sdfDevLoadFileEx(path, &sourceAddress, 0);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    sdfMemFreeDirect(allocation);
    return mappedResource;
}

/* Publish mapped records before releasing their source allocation and completing the file job. */
void effCompleteMappedResourceJob(struct FileRequest *job,
                                  EffMappedResource **outMappedResource) {
    struct sdfMemHandle_t *allocation;
    u32 sourceAddress;
    EffMappedResource *mappedResource;

    allocation = fileGetMemHandle(job);
    sourceAddress = sdfMemLock(allocation);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    *outMappedResource = mappedResource;
    sdfMemFreeDirect(allocation);
    fileHandleFree(job);
}

/* Initialize the output to zero and schedule mapped-record completion. */
void effRequestMappedResource(const char *base, const char *name,
                              EffMappedResource **outMappedResource) {
    char path[EFF_RESOURCE_PATH_BYTES];
    sprintf(path, D_004387E8, base, name);
    *outMappedResource = 0;
    fileLoadEx(path, 0, (s32)effCompleteMappedResourceJob, (s32)outMappedResource);
}

/* Create an owner list with sixteen initially empty record buckets. */
EffectOwnerRecord *effCreateOwnerRecordList(EffectSlotSet *owner) {
    EffectOwnerRecord *list = (EffectOwnerRecord *)sdfChipAllocDirect(EFF_OWNER_LIST_BYTES);
    memset(list, 0, EFF_OWNER_LIST_BYTES);
    list->owner = owner;
    return list;
}

/* Clamp only the upper bucket bound and prepend; the former head's prev link is not rewritten. */
void effInsertSlotRecord(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucketIndex;
    EffectRecord *record;

    if (bucketIndex >= EFF_RECORD_BUCKETS) {
        bucketIndex = EFF_RECORD_LAST_BUCKET;
    }
    record = sdfChipAllocDirect(sizeof(*record));
    record->prev = 0;
    record->owner = owner;
    record->slot = slotIndex;
    record->next = list->entries[bucketIndex];
    list->entries[bucketIndex] = record;
}

/* Remove the first matching slot from the raw bucket index; unlike insertion, this does not clamp it. */
s32 effRemoveSlotRecord(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucketIndex;
    EffectRecord *record = list->entries[bucketIndex];

    while (record != 0) {
        if (record->slot == slotIndex) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == list->entries[bucketIndex]) {
                list->entries[bucketIndex] = record->next;
            }
            sdfChipFreeDirect(record);
            return 1;
        }
        record = record->next;
    }
    return 0;
}

/* Release all sixteen buckets; retain the original traversal that reads next after releasing a record. */
void effReleaseRecordBuckets(EffectOwnerRecord *list) {
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown;
    for (bucketCountdown = EFF_RECORD_LAST_BUCKET; bucketCountdown >= 0; bucketCountdown--, bucketHead++) {
        EffectRecord *record = *bucketHead;
        while (record != 0) {
            if (record->prev != 0) {
                record->prev->next = record->next;
            }
            if (record->next != 0) {
                record->next->prev = record->prev;
            }
            if (record == *bucketHead) {
                *bucketHead = record->next;
            }
            sdfChipFreeDirect(record);
            record = record->next;
        }
    }
}

extern void itfGridLookupValueOrDefault(void *object, s32 key);

/* Dispatch every bucket record and optionally refresh its owner/slot lookup. */
s32 effDispatchRecordBuckets(s32 refresh, EffectOwnerRecord *list, s32 drawOption) {
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown;
    for (bucketCountdown = EFF_RECORD_LAST_BUCKET; bucketCountdown >= 0; bucketCountdown--, bucketHead++) {
        EffectRecord *record = *bucketHead;
        while (record != 0) {
            itfDrawGridWithResolvedSlot(0, 0, 0, 0, list->owner, record->slot, drawOption);
            if (refresh != 0) {
                itfGridLookupValueOrDefault(list->owner, record->slot);
            }
            record = record->next;
        }
    }
    return 1;
}

/* Release bucket records before freeing the owner list. */
u32 effDestroyOwnerRecordList(u32 buckets) {
    effReleaseRecordBuckets((EffectOwnerRecord *)buckets);
    sdfChipFreeDirect((void *)buckets);
    return 1;
}

/* Convert a 4-byte effect parameter between its int and float kinds. Kind 0
 * converts (float to int, or int to float when toFloat is set); kind 2 copies
 * a float through. Returns the converted size, or 0 for other kinds. */
u32 effConvertParamValue(u32 *kind, void *out, void *in, void *toFloat) {
    s32 intValue;
    f32 floatValue;

    if (toFloat == NULL) {
        switch (*kind) {
        case 0:
            if (in != NULL) {
                intValue = *(f32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &intValue, 4);
            }
            return 4;
        case 2:
            if (in != NULL) {
                floatValue = *(f32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &floatValue, 4);
            }
            return 4;
        }
    } else {
        switch (*kind) {
        case 0:
            if (in != NULL) {
                floatValue = *(s32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &floatValue, 4);
            }
            return 4;
        case 2:
            if (in != NULL) {
                floatValue = *(f32 *)in;
            }
            if (out != NULL) {
                memcpy(out, &floatValue, 4);
            }
            return 4;
        }
    }
    return 0;
}


/* Sum the status-storage byte requirements for the selected record category. */
u32 effSumRecordStatuses(const EffMappedRecord *record) {
    EffRecordBucket *group = &D_00400508[record->category];
    u32 statusBytes = 0;
    u32 recordIndex;

    for (recordIndex = 0; recordIndex < group->count; recordIndex++) {
        statusBytes += effConvertParamValue(group->records + recordIndex * EFF_STATUS_PARAM_STRIDE + EFF_STATUS_PARAM_HEADER_BYTES, 0, 0, 0);
    }
    return statusBytes;
}

/* Serialized file header; this is distinct from the live batch header below. */
typedef struct EffMappedHeader {
    u8 pad_00[0x14];
    u32 count;        // 0x14
    u8 pad_18[8];
} EffMappedHeader;    // 0x20

/* Copy packed headers and status data into live records, zero-filling extra status capacity.
 * The required-size calculation uses the first record, not the current row.
 * Returns the record-allocation handle, not its retained address.
 */
struct sdfMemHandle_t *effLoadMappedStatusRecords(const u8 *source, EffMappedHeader *headerOut) {
    EffMappedHeader header;
    struct sdfMemHandle_t *allocation;
    EffMappedRecord *records;
    u32 recordIndex = 0;
    u32 statusBytes;

    memcpy(&header, source, sizeof(header));
    source += sizeof(header);
    allocation = sdfMemAllocDirect(header.count * EFF_STATUS_RECORD_BYTES);
    records = (EffMappedRecord *)sdfMemLock(allocation);
    for (; recordIndex < header.count; recordIndex++) {
        EffMappedRecord *record = &records[recordIndex];

        memcpy(record, source, EFF_PACKED_STATUS_HEADER_BYTES);
        source += EFF_PACKED_STATUS_HEADER_BYTES;
        statusBytes = effSumRecordStatuses(records);
        if (statusBytes < record->statusBytes) {
            statusBytes = record->statusBytes;
        }
        record->status = sdfChipAllocDirect(statusBytes);
        memset(record->status, 0, statusBytes);
        memcpy(record->status, source, record->statusBytes);
        source += record->statusBytes;
        if (record->statusBytes < statusBytes) {
            record->statusBytes = statusBytes;
        }
    }
    if (headerOut != 0) {
        memcpy(headerOut, &header, sizeof(header));
    }
    return allocation;
}


/* Build the live batch header from the serialized count and owned record array. */
EffMappedResource *effCreateMappedResource(const u8 *source) {
    EffMappedResource *mappedResource = (EffMappedResource *)sdfChipAllocDirect(EFF_BATCH_HEADER_BYTES);
    EffMappedHeader header;

    mappedResource->allocation = effLoadMappedStatusRecords(source, &header);
    mappedResource->records = (EffMappedRecord *)sdfMemLock(mappedResource->allocation);
    mappedResource->count = header.count;
    return mappedResource;
}

/* Build one zeroed status record and allocate the category's required status storage. */
EffMappedResource *effCreateStatusBatch(u32 category) {
    EffMappedResource *batch = (EffMappedResource *)sdfChipAllocDirect(EFF_BATCH_HEADER_BYTES);
    struct sdfMemHandle_t *allocation;
    u32 statusBytes;
    u8 *statuses;

    batch->count = 1;
    allocation = sdfMemAllocDirect(EFF_STATUS_RECORD_BYTES);
    batch->allocation = allocation;
    batch->records = (EffMappedRecord *)sdfMemLock(allocation);
    memset(batch->records, 0, EFF_STATUS_RECORD_BYTES);
    {
        EffMappedRecord *record = batch->records;
        record->category = category;
        statusBytes = effSumRecordStatuses(record);
    }
    statuses = (u8 *)sdfChipAllocDirect(statusBytes);
    batch->records->status = statuses;
    memset(statuses, 0, statusBytes);
    batch->records->statusBytes = statusBytes;
    return batch;
}

#include "eff_mapped_resource_destroy_result.h"

/* Release each record's status storage, then the record allocation and batch header. */
EffMappedResourceDestroyResult effDestroyPackedBatch(EffMappedResource *batch) {
    s32 recordIndex;

    for (recordIndex = 0; recordIndex < batch->count; recordIndex++) {
        sdfChipFreeDirect(batch->records[recordIndex].status);
    }
    sdfMemFreeDirect(batch->allocation);
    sdfChipFreeDirect(batch);
    return EFF_MAPPED_RESOURCE_DESTROYED;
}


typedef struct EffPackedResourceHeader {
    u8 pad00[0xC];
    u32 entriesOffset;
    u8 pad10[4];
    u16 textureCount;
    u16 slotCount;
} EffPackedResourceHeader;

typedef struct EffPackedResourceEntry {
    u32 unk00;
    u32 resourceOffset;
} EffPackedResourceEntry;



u32 effReleaseSlotWorkAllocation(EffectSlotSet *owner) {
    sdfMemFreeDirect(owner->workAllocation);
    return 1;
}

/* Return the normal slot address unless its stored alternate address is nonzero. */
void *effGetSlotWorkOrOverride(EffectSlotSet *owner, s32 slotIndex) {
    void *alternatePayload;
    s32 entryAddress;

    entryAddress = slotIndex * EFF_SLOT_WORK_BYTES + (s32)owner->workEntries;
    alternatePayload = ((BdWork *)entryAddress)->alternate.payload;
    if (alternatePayload != NULL) {
        entryAddress = (s32)alternatePayload;
    }
    return (void *)entryAddress;
}

/* Start at zero when moving forward, otherwise at the full 16.16 endpoint. */
void effInitializeSlotPhase(EffTimedState *state) {
    if (state->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
        state->value = EFF_SLOT_PHASE_ZERO;
    } else {
        state->value = EFF_SLOT_PHASE_ONE;
    }
}

extern s8 D_004002F0[];

EffectSlotSet *effUpdateTimedStates(EffectSlotSet *effect, u32 slot, void *entryData);

/* Rebuild a slot's live work from its source description, then prime the timed states. */
void effInitializeSlotWorkFromDescription(EffectSlotSet *owner, s32 slotIndex, void *payload) {
    BdWork *entry = (BdWork *)payload;
    EffectSlotDescription *description = &owner->descriptions[slotIndex];
    BdWork *work = &owner->workEntries[slotIndex];
    EffectSlotGeometry *geometry = &entry->geometry;
    s32 i;

    entry->geometry.bounds[1] = 0;
    geometry->bounds[0] = 0;
    for (i = 0; i < 4; i++) {
        work->bounds.grid.quantizedBounds[i] = description->bounds[i];
    }
    for (i = 0; i < 4; i++) {
        s32 color = description->cornerColors[D_004002F0[i]];
        geometry->cornerColors[i] = color;
        work->savedColors[i] = color;
    }
    for (i = 0; i < 4; i++) {
        entry->bounds.grid.pad68[i] = 0x80;
    }
    if (description->widthOverride != 0) {
        work->sourceWidth = description->widthOverride;
    } else {
        work->sourceWidth = description->colors[2] - description->colors[0];
    }
    if (description->heightOverride != 0) {
        work->sourceHeight = description->heightOverride;
    } else {
        work->sourceHeight = description->colors[3] - description->colors[1];
    }
    entry->geometry.angleDegrees = 0;
    entry->geometry.bounds[2] = work->sourceWidth << 4;
    entry->geometry.bounds[3] = work->sourceHeight << 3;
    work->remainingDescriptionUpdates = description->descriptionUpdateDelay;
    work->slotOffset = 0;
    for (i = 0; i < sizeof(entry->states) / sizeof(entry->states[0]); i++) {
        EffTimedState *state = &entry->states[i];

        state->delay = description->initialDelay;
        effInitializeSlotPhase(state);
        state->delay += 1;
    }
    effUpdateTimedStates(owner, slotIndex, payload);
}

/* Initialize the normal slot entry at the unchanged 0xA0-byte stride. */
void effInitializeSlotWork(EffectSlotSet *owner, s32 slotIndex) {
    effInitializeSlotWorkFromDescription(owner, slotIndex, &owner->workEntries[slotIndex]);
}

/* Reset every normal work slot in source order. */
void effInitializeAllSlotWork(EffectSlotSet *owner) {
    u32 slotIndex = 0;
    if (owner->count != 0) {
        do {
            effResetSlotWork(owner, slotIndex++);
        } while (slotIndex < owner->count);
    }
}

/* Store the owner and slot index before invoking the work initializer. */
void effAttachSlotWorkOwner(EffectSlotSet *owner, s32 slotIndex, void *payload) {
    /* Alternate records share these owner/index fields without a full work allocation. */
    BdWork *entry = (BdWork *)payload;
    entry->owner = owner;
    entry->slotIndex = slotIndex;
    effInitializeSlotWorkFromDescription(owner, slotIndex, payload);
}

/* Clear the complete normal slot, then restore owner/index and initialize it. */
void effResetSlotWork(EffectSlotSet *owner, u32 slotIndex) {
    BdWork *entry;

    entry = &owner->workEntries[slotIndex];
    memset(entry, 0, EFF_SLOT_WORK_BYTES);
    effAttachSlotWorkOwner(owner, slotIndex, entry);
}


/* Acquire missing selected handles or clear slots; clearAllSlots does not release textures.
 * Return the address following the processed resource-table entries.
 */
u8 *effResolveResourceSlots(EffectSlotSet *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot) {
    u32 slotIndex = 0;
    u8 *entryBytes = resourceBytes;

    entryBytes += ((EffPackedResourceHeader *)entryBytes)->entriesOffset;

    if (set->textureCount != 0) {
        do {
            s32 *resourceData = (s32 *)(resourceBytes + ((EffPackedResourceEntry *)entryBytes)->resourceOffset);

            entryBytes += EFF_RESOURCE_TABLE_ENTRY_BYTES;
            if (clearAllSlots == 0) {
                if (selectedSlot == -1 || selectedSlot == slotIndex) {
                    if (set->textureReferences[slotIndex] == 0) {
                        set->textureReferences[slotIndex] = sdfTexSet((SdfTextureFileHeader *)(resourceData));
                    }
                } else {
                    set->textureReferences[slotIndex] = 0;
                }
            } else {
                set->textureReferences[slotIndex] = 0;
            }
            slotIndex++;
        } while (slotIndex < set->textureCount);
    }
    return entryBytes;
}

void effResolveAndReleaseResource(EffectSlotSet *owner) {
    if (owner->sourceAllocation != 0) {
        struct sdfMemHandle_t *resource = owner->sourceAllocation;
        u8 *mapped = (u8 *)sdfMemLock(resource);
        effResolveResourceSlots(owner, mapped, 0, -1);
        sdfMemUnlock(owner->sourceAllocation);
    }
}

void effResolveAndReleaseSelectedResource(EffectSlotSet *owner, s32 mapping) {
    if (owner->sourceAllocation != 0) {
        struct sdfMemHandle_t *resource = owner->sourceAllocation;
        u8 *mapped = (u8 *)sdfMemLock(resource);
        effResolveResourceSlots(owner, mapped, 0, mapping);
        sdfMemUnlock(owner->sourceAllocation);
    }
}
void effReleaseSlotTextureReferencesAndResetWork(EffectSlotSet *owner, s32 preserveWork) {
    u32 i = 0;
    u32 count = owner->textureCount;
    sdfTexHandle_t **resources;

    if (count != 0) {
        resources = owner->textureReferences;
        do {
            if (resources[i] != NULL) {
                sdfTexHandle_t **current;
                sdfTexReleaseReference(resources[i]);
                current = owner->textureReferences;
                count = owner->textureCount;
                resources = current;
                current[i] = NULL;
            }
            i++;
        } while (i < count);
    }
    if (!preserveWork) {
        effInitializeAllSlotWork(owner);
    }
}

void effReleaseTextureHandlesAndResetSlots(EffectSlotSet *owner) {
    effReleaseSlotTextureReferencesAndResetWork(owner, 0);
}

u8 effHasFirstTextureHandle(const EffectSlotSet *owner) {
    return owner->textureReferences[0] != NULL;
}

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
EffPayload *effCreatePayload(u32 recordCount) {
    u32 recordBytes = recordCount * EFF_PAYLOAD_RECORD_BYTES;
    EffPayload *payload = (EffPayload *)sdfChipAllocDirect(EFF_BATCH_HEADER_BYTES);
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(recordBytes);
    payload->count = recordCount;
    payload->allocation = allocation;
    payload->records = (u8 *)sdfMemLock(allocation);
    memset(payload->records, 0, recordBytes);
    return payload;
}

/* Release the record allocation before freeing its small header. */
u32 effDestroyPayload(EffPayload *payload) {
    sdfMemFreeDirect(payload->allocation);
    sdfChipFreeDirect(payload);
    return 1;
}

/* A slot set either owns its texture-reference table or borrows its parent's. */
#define EFF_SLOT_TEXTURE_REFERENCES_OWNED 0
#define EFF_SLOT_TEXTURE_REFERENCES_BORROWED 1

EffectSlotSet *effCreateResourceSlotSetFromAllocation(struct sdfMemHandle_t *resourceAllocation, u32 keepAllocation) {
    EffectSlotSet *set;
    u8 *resource;
    u8 *entries;
    u32 index;
    u32 sourceOffset;

    set = (EffectSlotSet *)sdfChipAllocDirect(0x30);
    memset(set, 0, 0x30);
    set->sharesTextureReferences = EFF_SLOT_TEXTURE_REFERENCES_OWNED;
    set->sourceAllocation = keepAllocation != 0 ? resourceAllocation : 0;
    resource = (u8 *)sdfMemLock(resourceAllocation);
    set->textureCount = ((EffPackedResourceHeader *)resource)->textureCount;
    set->textureAllocation = sdfMemAllocDirect(
        set->textureCount * 4);
    set->textureReferences = (sdfTexHandle_t **)sdfMemLock(set->textureAllocation);
    memset(set->textureReferences, 0, set->textureCount * 4);
    entries = effResolveResourceSlots(set, resource,
        keepAllocation, -1);

    set->count = ((EffPackedResourceHeader *)resource)->slotCount;
    set->descriptionAllocation =
        sdfMemAllocDirect(set->count * 0x80);
    set->descriptions = (EffectSlotDescription *)sdfMemLock(set->descriptionAllocation);
    set->workAllocation = sdfMemAllocDirect(set->count * 0xA0);
    set->workEntries = (BdWork *)sdfMemLock(set->workAllocation);
    for (index = 0; index < set->count; index++) {
        sourceOffset = *(u32 *)(entries + 4);
        memcpy(&set->descriptions[index],
            resource + sourceOffset, 0x80);
        effResetSlotWork(set, index);
        entries += 8;
    }
    return set;
}

EffectSlotSet *effCreateResourceSlotSet(EffectSlotSet *borrowedSource, u32 slot, u32 count) {
    EffectSlotSet *effect = (EffectSlotSet *)sdfChipAllocDirect(0x30);
    u32 index = 0;
    effect->sharesTextureReferences = EFF_SLOT_TEXTURE_REFERENCES_BORROWED;
    {
        u32 mode = borrowedSource->textureCount;
        sdfTexHandle_t **textureReferences = borrowedSource->textureReferences;
        effect->textureCount = mode;
        effect->textureReferences = textureReferences;
    }
    effect->sourceAllocation = 0;
    effect->textureAllocation = 0;
    effect->count = count;
    effect->descriptionAllocation = sdfMemAllocDirect(count * 0x80);
    effect->descriptions = (EffectSlotDescription *)sdfMemLock(effect->descriptionAllocation);
    effect->workAllocation = sdfMemAllocDirect(effect->count * 0xA0);
    effect->workEntries = (BdWork *)sdfMemLock(effect->workAllocation);
    if (effect->count != 0) {
        do {
            memcpy(&effect->descriptions[index], &borrowedSource->descriptions[slot], 0x80);
            effResetSlotWork(effect, index);
            index++;
        } while (index < effect->count);
    }
    return effect;
}

u32 effDestroyResourceSlotSet(EffectSlotSet *set) {
    if (set->sourceAllocation != 0) {
        sdfMemFreeDirect(set->sourceAllocation);
    }
    if (set->sharesTextureReferences == EFF_SLOT_TEXTURE_REFERENCES_OWNED) {
        effReleaseTextureHandlesAndResetSlots(set);
        sdfMemFreeDirect(set->textureAllocation);
    }
    sdfMemFreeDirect(set->descriptionAllocation);
    effReleaseSlotWorkAllocation(set);
    sdfChipFreeDirect(set);
    return 1;
}

#undef EFF_SLOT_TEXTURE_REFERENCES_OWNED
#undef EFF_SLOT_TEXTURE_REFERENCES_BORROWED

u32 effSetSlotResourceAndFlags(EffTimedState *effect, EffMappedRecord *resource, u32 flags) {
    effect->flags = flags;
    effect->source = resource;
    if ((flags & EFF_TIMED_STATE_INITIALIZE_PHASE_ON_BIND) != 0) {
        effInitializeSlotPhase(effect);
    }
    effect->delay = effect->delay + 1;
    return 1;
}

u32 effSetSlotIndexedResource(EffTimedState *target, EffMappedResource *resources, s32 index,
                              u32 flags) {
    effSetSlotResourceAndFlags(target, &resources->records[index], flags);
    return 1;
}


u32 effSlotTransitionClearTarget(EffTimedState *work, u32 unused) {
    work->source = NULL;
    return 1;
}

s32 effClampSlotPhaseAtEnd(EffectSlotSet *owner, u32 slot, EffTimedState *state) {
    if (EFF_SLOT_PHASE_ONE < state->value) {
        u32 flags = state->flags;
        state->value = EFF_SLOT_PHASE_ONE;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                state->flags = flags & ~EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, slot);
            }
            return 0;
        }
    }
    return 1;
}

s32 effClampSlotPhaseAtStart(EffectSlotSet *owner, u32 slot, EffTimedState *state) {
    if (state->value < EFF_SLOT_PHASE_ZERO) {
        u32 flags = state->flags;
        state->value = EFF_SLOT_PHASE_ZERO;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                state->flags = flags | EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, slot);
            }
            return 0;
        }
    }
    return 1;
}

extern u32 effResetRecordRun(u8 *, u32, u32);

EffectSlotSet *effUpdateTimedStates(EffectSlotSet *effect, u32 slot, void *entryData) {
    /* Alternate payloads may be only 0x6C bytes; direct access here is limited to timed states.
     * Bucket callbacks retain their existing kind-specific pointer contract.
     */
    BdWork *entry = (BdWork *)entryData;
    EffTimedState *states = entry->states;
    BdWork *record = &effect->workEntries[slot];
    s32 idle = 1;
    u32 i;

    for (i = 0; i < 2; i++) {
        EffTimedState *state = &states[i];
        EffMappedRecord *source = state->source;

        if (source != 0 && source->category != 0) {
            EffRecordBucket *bucket = &D_00400508[source->category];
            s32 phaseStep = bucket->step(record, entry, state);

            if (state->delay > 0) {
                phaseStep = 0;
                state->delay -= 1;
            }
            if (phaseStep >= 0) {
                if (state->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
                    if (state->value != EFF_SLOT_PHASE_ONE) {
                        state->value += phaseStep;
                        idle = 0;
                        if (effClampSlotPhaseAtEnd(effect, slot, state) == 0) {
                            state->delay = state->delayMax;
                            return 0;
                        }
                    }
                } else if (state->value != EFF_SLOT_PHASE_ZERO) {
                    state->value -= phaseStep;
                    idle = 0;
                    if (effClampSlotPhaseAtStart(effect, slot, state) == 0) {
                        state->delay = state->delayMax;
                        return 0;
                    }
                }
            }
        }
    }
    if (idle != 0) {
        effResetRecordRun((u8 *)effect, slot, -1);
        return 0;
    }
    return effect;
}

u32 effSetSlotOverrideWork(EffectSlotSet *owner, s32 slotIndex, void *payload) {
    if (owner->workEntries[slotIndex].alternate.payload == NULL) {
        effAttachSlotWorkOwner(owner, slotIndex, payload);
    }
    owner->workEntries[slotIndex].alternate.payload = payload;
    return 1;
}

u32 effSetMaterialSlots(EffectSlotSet *owner, s32 index, u32 materialFlags, void *payload) {
    u32 i;
    EffTimedState *states;
    if (owner->workEntries[index].alternate.payload == NULL) {
        effAttachSlotWorkOwner(owner, index, payload);
    }
    owner->workEntries[index].alternate.payload = payload;
    /* Both timed states lie within the shared 0x6C-byte payload region. */
    states = ((BdWork *)payload)->states;
    for (i = 0; i < 2; i++) {
        states[i].materialFlags = materialFlags;
    }
    return 1;
}

u32 effClearSlotOverrideWork(s32 effect, s32 slot) {
    ((EffectSlotSet *)effect)->workEntries[slot].alternate.payload = NULL;
    return 1;
}

s32 effConfigureSlotResource(u8 *effect, u32 slot, u32 resource, u32 flags) {
    BdWork *entry = &((EffectSlotSet *)effect)->workEntries[slot];
    effSetSlotResourceAndFlags(&entry->states[0], (EffMappedRecord *)(u32)resource, flags);
    effUpdateTimedStates((EffectSlotSet *)effect, slot, entry);
    return 1;
}

s32 effConfigureIndexedSlotResource(EffectSlotSet *effect, u32 slot,
                                    EffMappedResource *resources, u32 index, u32 flags) {
    BdWork *entry = &effect->workEntries[slot];
    effSetSlotResourceAndFlags(&entry->states[0], &resources->records[index], flags);
    effUpdateTimedStates(effect, slot, entry);
    return 1;
}

s32 effConfigureIndexedSlotMaterial(EffectSlotSet *effect, u32 slot,
                  EffMappedResource *resources, u32 index,
                  u32 materialFlags, u32 materialValue, u32 stateFlags) {
    BdWork *entry = &effect->workEntries[slot];
    effSetSlotResourceAndFlags(&entry->states[0], &resources->records[index], stateFlags);
    effUpdateTimedStates(effect, slot, entry);
    entry->states[0].materialFlags = materialFlags;
    entry->states[0].materialValue = materialValue;
    return 1;
}

u32 effConfigureWithDefaultSetting(EffectSlotSet *effect, u32 slot,
                                   EffMappedResource *resources, u32 item,
                                   u32 materialFlags, u32 stateFlags) {
    effConfigureIndexedSlotMaterial(effect, slot, resources, item,
                                    materialFlags, 0, stateFlags);
    return 1;
}

u32 effResetRecordRun(u8 *work, u32 first, u32 unused) {
    u32 i = 0;
    u32 index;

    do {
        effSlotTransitionClearTarget((EffTimedState *)((first + i) * 0xA0 +
                                    (s32)((EffectSlotSet *)work)->workEntries + 0x28), unused);
        i++;
        index = first + i;
    } while (index < ((EffectSlotSet *)work)->count && (((EffectSlotSet *)work)->descriptions[index].flags & EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION));
    return 1;
}

void effSelectPresetByKind(u32 mode, u32 value) {
    switch (mode) {
    case 0:
        sdfSubmitGsAlphaOneRegisterPacket(0x44, value);
        return;
    case 1:
        sdfSubmitGsAlphaOneRegisterPacket(0x48, value);
        return;
    case 2:
        sdfSubmitGsAlphaOneRegisterPacket(0x42, value);
        break;
    }
}

typedef struct sdfTexHandle_t sdfTexHandle_t;
typedef struct SdfDrawPacket SdfDrawPacket;

typedef struct EffSpriteUV {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} EffSpriteUV;

typedef union EffSpriteColor {
    u32 rgba;
    struct {
        u32 alpha : 8;
        u32 blue : 8;
        u32 green : 8;
        u32 red : 8;
    } channels;
} EffSpriteColor;

#include "sdf_packet_alloc.h"

extern void sdfTexSetFilter(sdfTexHandle_t *, s32, s32);
extern s32 sdf2DGetPrimPacketSize(s32, s32);
extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);
extern void effSelectPresetByKind(u32, u32);
void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *color, u32 flipFlags,
                   u32 blendKind, s32 mode, sdfTexHandle_t *texture, s32 surfaceId) {
    u32 textureCoordinates[4];
    void *packet;
    sdfDrawTag_t *packetList;
    u64 *packetWords;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 savedCoordinate;

    if (mode == 0) {
        sdfTexSetFilter(texture, 0, 1);
    } else {
        sdfTexSetFilter(texture, 1, 1);
    }
    packet = (void *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x43431, 1);
    /* The SDK size helper also skips the packet's two header quadwords. */
    packetWords = (u64 *)sdf2DGetPrimPtr(packet);
    left = x + 0x7000;
    top = y + 0x7900;
    textureCoordinates[0] = uvRect->u0 * 16;
    textureCoordinates[2] = uvRect->u1 * 16;
    textureCoordinates[1] = uvRect->v0 * 16;
    textureCoordinates[3] = uvRect->v1 * 16;
    right = left + width;
    bottom = top + height;
    if (flipFlags & 1) {
        savedCoordinate = left;
        left = right;
        right = savedCoordinate;
    }
    if (flipFlags & 2) {
        savedCoordinate = top;
        top = bottom;
        bottom = savedCoordinate;
    }
    if (color == NULL) {
        packetWords[0] = ((u64)0x80 << 32) | 0x80;
        packetWords[1] = ((u64)0x80 << 32) | 0x80;
    } else {
        u32 rgba = color->rgba;

        packetWords[0] = color->channels.red | ((u64)color->channels.green << 32);
        packetWords[1] = ((rgba >> 8) & 0xFF) | ((u64)(rgba & 0xFF) << 32);
    }
    packetWords[2] = textureCoordinates[0] | ((u64)textureCoordinates[1] << 32);
    packetWords[4] = (u64)(u32)left | ((u64)top << 32);
    packetWords[6] = textureCoordinates[2] | ((u64)textureCoordinates[3] << 32);
    packetWords[8] = (u64)(u32)right | ((u64)bottom << 32);
    packetWords[5] = depth;
    packetWords[9] = depth;
    effSelectPresetByKind(blendKind, surfaceId);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdf2DLinkTexPacket(packetList, texture, SDF_GS_CONTEXT_1);
    sdfDrawTagLink(packetList, (u32)packet);
    effSubmitSurfacePacket(&sdfGrOtTable[surfaceId], packetList);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceId);
}


/* Rotate an offset in degrees and add its center; mode selects the screen aspect correction. */
void itfRotateScreenOffset(s32 *outX, s32 *outY, s32 x, s32 y, s32 centerX, s32 centerY, f32 angle, s32 mode) {
    f32 aspect = 1.75f;
    f32 radians = angle * 0.017453293f;
    f32 adjustedY;
    f32 rotatedX;
    f32 rotatedY;
    if (mode == 0) {
        aspect = 2.0f;
    }
    adjustedY = (s32)(y * aspect);
    rotatedX = sdfCos(radians) * x - sdfSin(radians) * adjustedY;
    rotatedY = sdfSin(radians) * x + sdfCos(radians) * adjustedY;
    if (rotatedX > 0.0f) {
        rotatedX += 0.1f;
    } else {
        rotatedX -= 0.1f;
    }
    if (rotatedY > 0.0f) {
        rotatedY += 0.1f;
    } else {
        rotatedY -= 0.1f;
    }
    *outX = (s32)rotatedX;
    *outY = (s32)rotatedY;
    *outX += centerX;
    *outY = (s32)(*outY / aspect + centerY);
}


/* NON_MATCHING: register allocation in this function depends on how many
 * types and functions the unit declares before it, so it matches only with
 * unused declarations added; waits for the unit's real boundaries. */
void sdfDrawTexturedQuad(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *cornerColors,
                   u32 flipFlags, f32 angle, u32 blendKind, s32 mode, s32 aspectMode,
                   sdfTexHandle_t *texture, s32 surfaceId);
#ifdef NON_MATCHING
void sdfDrawTexturedQuad(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *cornerColors,
                   u32 flipFlags, f32 angle, u32 blendKind, s32 mode, s32 aspectMode,
                   sdfTexHandle_t *texture, s32 surfaceId) {
    u32 textureCoordinates[4];
    /* The four corners stay separate locals: itfRotateScreenOffset writes each
     * coordinate through its own pointer. */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;
    s32 x3;
    s32 y3;
    SdfDrawPacket *packet;
    sdfDrawTag_t *packetList;
    u64 *packetWords;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 halfWidth;
    s32 halfHeight;
    s32 centerX;
    s32 centerY;
    s32 savedCoordinate;

    if (mode == 0) {
        sdfTexSetFilter(texture, 0, 1);
    } else {
        sdfTexSetFilter(texture, 1, 1);
    }
    packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 4));
    sdf2DMakePrimPacketHeader(packet, 0x15C, 3, 0x531, 4);
    packetWords = (u64 *)sdf2DGetPrimPtr(packet);
    left = x + 0x7000;
    top = y + 0x7900;
    right = left + width - 8;
    bottom = top + height - 4;
    halfWidth = (right - left) / 2;
    halfHeight = (bottom - top) / 2;
    centerX = left + halfWidth;
    centerY = top + halfHeight;
    x0 = left;
    y0 = top;
    x3 = right;
    y3 = bottom;
    if (angle == 0.0f) {
        x1 = right;
        y1 = top;
        x2 = left;
        y2 = bottom;
    } else {
        itfRotateScreenOffset(&x0, &y0, -halfWidth, -halfHeight, centerX, centerY, angle, aspectMode);
        itfRotateScreenOffset(&x1, &y1, halfWidth, -halfHeight, centerX, centerY, angle, aspectMode);
        itfRotateScreenOffset(&x2, &y2, -halfWidth, halfHeight, centerX, centerY, angle, aspectMode);
        itfRotateScreenOffset(&x3, &y3, halfWidth, halfHeight, centerX, centerY, angle, aspectMode);
    }
    textureCoordinates[0] = uvRect->u0 * 16;
    textureCoordinates[1] = uvRect->v0 * 16;
    textureCoordinates[2] = uvRect->u1 * 16;
    textureCoordinates[3] = uvRect->v1 * 16;
    if (flipFlags & 1) {
        savedCoordinate = x0;
        x0 = x1;
        x1 = savedCoordinate;
        savedCoordinate = x2;
        x2 = x3;
        x3 = savedCoordinate;
    }
    if (flipFlags & 2) {
        savedCoordinate = y0;
        y0 = y2;
        y2 = savedCoordinate;
        savedCoordinate = y1;
        y1 = y3;
        y3 = savedCoordinate;
    }
    if (cornerColors == NULL) {
        packetWords[0] = ((u64)0x80 << 32) | 0x80;
        packetWords[1] = ((u64)0x80 << 32) | 0x80;
        packetWords[6] = ((u64)0x80 << 32) | 0x80;
        packetWords[7] = ((u64)0x80 << 32) | 0x80;
        packetWords[12] = ((u64)0x80 << 32) | 0x80;
        packetWords[13] = ((u64)0x80 << 32) | 0x80;
        packetWords[18] = ((u64)0x80 << 32) | 0x80;
        packetWords[19] = ((u64)0x80 << 32) | 0x80;
    } else {
        u32 rgba0 = cornerColors[0].rgba;
        u32 rgba1 = cornerColors[1].rgba;
        u32 rgba2 = cornerColors[2].rgba;
        u32 rgba3 = cornerColors[3].rgba;

        packetWords[0] = cornerColors[0].channels.red | ((u64)cornerColors[0].channels.green << 32);
        packetWords[1] = ((rgba0 >> 8) & 0xFF) | ((u64)(rgba0 & 0xFF) << 32);
        packetWords[6] = cornerColors[1].channels.red | ((u64)cornerColors[1].channels.green << 32);
        packetWords[7] = ((rgba1 >> 8) & 0xFF) | ((u64)(rgba1 & 0xFF) << 32);
        packetWords[12] = cornerColors[2].channels.red | ((u64)cornerColors[2].channels.green << 32);
        packetWords[13] = ((rgba2 >> 8) & 0xFF) | ((u64)(rgba2 & 0xFF) << 32);
        packetWords[18] = cornerColors[3].channels.red | ((u64)cornerColors[3].channels.green << 32);
        packetWords[19] = ((rgba3 >> 8) & 0xFF) | ((u64)(rgba3 & 0xFF) << 32);
    }
    /* Six u64 per vertex: RGBAQ, UV, then XYZ2 (the 0x531 register list). */
    packetWords[2] = textureCoordinates[0] | ((u64)textureCoordinates[1] << 32);
    packetWords[4] = (u64)(u32)x0 | ((u64)(u32)y0 << 32);
    packetWords[5] = depth >> 4;
    packetWords[8] = textureCoordinates[2] | ((u64)textureCoordinates[1] << 32);
    packetWords[10] = (u64)(u32)x1 | ((u64)(u32)y1 << 32);
    packetWords[11] = depth >> 4;
    packetWords[14] = textureCoordinates[0] | ((u64)textureCoordinates[3] << 32);
    packetWords[16] = (u64)(u32)x2 | ((u64)(u32)y2 << 32);
    packetWords[17] = depth >> 4;
    packetWords[20] = textureCoordinates[2] | ((u64)textureCoordinates[3] << 32);
    packetWords[22] = (u64)(u32)x3 | ((u64)(u32)y3 << 32);
    packetWords[23] = depth >> 4;
    effSelectPresetByKind(blendKind, surfaceId);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdf2DLinkTexPacket(packetList, texture, SDF_GS_CONTEXT_1);
    sdfDrawTagLink(packetList, (u32)packet);
    effSubmitSurfacePacket(&sdfGrOtTable[surfaceId], packetList);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceId);
}
#else
INCLUDE_ASM(void, "game/code_00304030", sdfDrawTexturedQuad);
#endif


void itfDrawRotatedTexturedRect(s32 x, s32 y, s32 depth, s32 width, s32 height,
                               const u32 *textureCoordinates, const u32 *cornerColors,
                               u32 flipFlags, f32 rotation, u32 blendKind, u32 textureMode,
                               sdfTexHandle_t *texture, u32 surfaceId) {
    sdfDrawTexturedQuad(x, y, depth, width, height, (const EffSpriteUV *)textureCoordinates,
                  (const EffSpriteColor *)cornerColors, flipFlags, rotation, blendKind, textureMode, 1,
                  texture, surfaceId);
}

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void effSelectPresetAndDispatch(u32 x, u32 y, u32 z, u32 width,
                                    u32 height, u32 vertexColors, u32 presetMode, u32 presetValue) {
    effSelectPresetByKind(presetMode, presetValue);
    uiDrawGradientColorRect(x, y, z, width, height, (const u32 *)vertexColors, presetValue);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, presetValue);
}

INCLUDE_SDATA(const s32, "game/code_00304030", D_004387E8);

INCLUDE_SDATA(const s32, "game/code_00304030", D_004387F0);

INCLUDE_SDATA(const s32, "game/code_00304030", D_004387F8);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438800);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438808);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438810);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438818);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438820);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438828);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438830);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438838);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438840);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438848);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438850);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438858);

INCLUDE_SDATA(const s32, "game/code_00304030", D_00438860);

