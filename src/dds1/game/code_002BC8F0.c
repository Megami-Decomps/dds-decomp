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


extern u32 effColorEditRgbMenuOpen;

extern u32 effColorEditSelectedKey;

extern u32 effColorEditPage;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 effColorEditSelectedAlphaKey;



extern s32 effEditorCurrentMenu;



extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern u32 effSharedScalyStripResource;

extern u32 effWindTextureHandle;

extern struct sdfMemHandle_t *D_003BC984;

extern struct sdfMemHandle_t *D_003BC994;



extern s32 effSharedRibbonReferenceCount;

extern u32 D_003BC990;

extern struct SdfTextureFileHeader *effFlashTextureHandles;

extern struct SdfTextureFileHeader *D_003BC96C;

extern u32 D_003BC950;

extern u32 D_003BC954;

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

extern EffectObjectNode *effFloorModelListHead;

extern u32 effPartsListScroll;

extern s32 D_003BC970[2];

extern RefObj *D_003BC978[2];



extern EffRecordBucket D_0038FD88[];


extern u32 effSharedTextureReferenceCount;

extern u32 D_003BC9B0[2];

extern u32 D_003BC9B8[2];

extern u8 D_003BC9C0[2];

extern u8 D_003BC9C8[2];

extern u8 D_003BD088[];

extern u8 D_003BD090[];

extern u8 *D_003BD074;

extern u8 D_003BCA40[];

extern u8 D_003BD050[];

extern u8 D_003BD000[];

extern u8 D_003BD108;

extern u8 D_003BD109;

extern u8 D_003BD10A;


/* Relative offsets in an effect resource's table and its eight-byte entries. */
typedef struct EffectResourceTable {
    u8 pad00[0xC];
    u32 entryOffset;
    u8 pad10[4];
    u16 textureCount;
    u16 slotCount;
} EffectResourceTable;

typedef struct EffectResourceEntry {
    u32 unknown;
    u32 resourceOffset;
} EffectResourceEntry;


extern u16 D_003BC944;

extern u8 *D_003BC958;
extern struct sdfMemHandle_t *D_003BC960[2];

extern s8 D_003BC9AC;

extern f32 D_003BC9A8;

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

extern char D_003BD080[];

extern s32 sprintf(char *, const char *, ...);

extern u8 D_003BD078[];

extern u8 D_003BCF30[];

extern s32 sprintf(char *, const char *, ...);

extern u32 D_003BD064;



extern char D_003BD198[];

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
#define EFF_SLOT_WORK_BYTES 0xA0
#define EFF_SLOT_PHASE_ZERO 0
#define EFF_SLOT_PHASE_ONE 0x10000
#define EFF_PAYLOAD_RECORD_BYTES 0x6C
#define EFF_RESOURCE_TABLE_ENTRY_BYTES 8

/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, u32 keepAllocation) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    struct sdfMemHandle_t *allocation;
    EffectSlotSet *instance;

    sprintf(path, D_003BD198, base, name);
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

extern s32 sprintf(char *, const char *, ...);

/* Clear the output first; only mode one selects the retained-allocation completion path. */
void effRequestResourceByMode(const char *base, const char *name, u32 mode, u32 *outInstance) {
    char path[EFF_RESOURCE_PATH_BYTES];

    sprintf(path, D_003BD198, base, name);
    *outInstance = 0;
    if (mode == EFF_RESOURCE_KEEP_ALLOCATION) {
        fileLoadEx(path, 0, (u32)effCompleteRetainedResourceJob, (u32)outInstance);
    } else {
        fileLoadEx(path, 0, (u32)effCompleteTransientResourceJob, (u32)outInstance);
    }
}

/* Build mapped records from the retained source address, then release the original file allocation. */
EffMappedResource *effLoadMappedResource(const char *base, const char *name) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    EffMappedResource *mappedResource;
    struct sdfMemHandle_t *allocation;

    sprintf(path, D_003BD198, base, name);
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

    sprintf(path, D_003BD198, base, name);
    *outMappedResource = 0;
    fileLoadEx(path, 0, (u32)effCompleteMappedResourceJob, (u32)outMappedResource);
}

/* Create an owner list with sixteen initially empty record buckets. */
EffectOwnerRecord *effCreateOwnerRecordList(EffectSlotSet *owner) {
    EffectOwnerRecord *list = sdfChipAllocDirect(sizeof(EffectOwnerRecord));
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
void effReleaseRecordBuckets(list)
EffectOwnerRecord *list;
{
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown = EFF_RECORD_LAST_BUCKET;

    do {
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
        ++bucketHead;
    } while (--bucketCountdown >= 0);
}

extern void itfGridLookupValueOrDefault(void *object, s32 key);

/* Dispatch every bucket record and optionally refresh its owner/slot lookup. */
u32 effDispatchRecordBuckets(u32 refresh, EffectOwnerRecord *list, s32 drawOption) {
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown = EFF_RECORD_LAST_BUCKET;

    do {
        EffectRecord *record = *bucketHead;
        while (record != 0) {
            itfDrawGridWithResolvedSlot(0, 0, 0, 0, list->owner, record->slot, drawOption);
            if (refresh != 0) {
                itfGridLookupValueOrDefault(list->owner, record->slot);
            }
            record = record->next;
        }
        ++bucketHead;
    } while (--bucketCountdown >= 0);
    return 1;
}

/* Release bucket records before the owner list. */
u32 effDestroyOwnerRecordList(u32 list) {
    effReleaseRecordBuckets((EffectOwnerRecord *)list);
    sdfChipFreeDirect((void *)list);
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

extern u32 effConvertParamValue(u32 *, void *, void *, void *);

/* Sum the status-storage byte requirements for the selected record category. */
u32 effSumRecordStatuses(const EffMappedRecord *record) {
    EffRecordBucket *group = &D_0038FD88[record->category];
    u32 statusBytes = 0;
    u32 recordIndex;

    for (recordIndex = 0; recordIndex < group->count; recordIndex++) {
        statusBytes += effConvertParamValue((u32 *)(group->records + recordIndex * EFF_STATUS_PARAM_STRIDE + EFF_STATUS_PARAM_HEADER_BYTES), 0, 0, 0);
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
    EffMappedResource *mappedResource = (EffMappedResource *)sdfChipAllocDirect(sizeof(EffMappedResource));
    EffMappedHeader header;

    mappedResource->allocation = effLoadMappedStatusRecords(source, &header);
    mappedResource->records = (EffMappedRecord *)sdfMemLock(mappedResource->allocation);
    mappedResource->count = header.count;
    return mappedResource;
}

/* Build one zeroed status record and allocate the category's required status storage. */
EffMappedResource *effCreateStatusBatch(u32 category) {
    EffMappedResource *batch = sdfChipAllocDirect(sizeof(EffMappedResource));
    struct sdfMemHandle_t *allocation;
    u32 recordAddress;
    u32 statusBytes;
    void *statuses;

    batch->count = 1;
    allocation = sdfMemAllocDirect(EFF_STATUS_RECORD_BYTES);
    batch->allocation = allocation;
    recordAddress = sdfMemLock(allocation);
    batch->records = (EffMappedRecord *)recordAddress;
    memset((void *)recordAddress, 0, EFF_STATUS_RECORD_BYTES);
    {
        EffMappedRecord *record = batch->records;
        record->category = category;
        statusBytes = effSumRecordStatuses(record);
    }
    statuses = sdfChipAllocDirect(statusBytes);
    batch->records->status = statuses;
    memset(statuses, 0, statusBytes);
    batch->records->statusBytes = statusBytes;
    return batch;
}


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

u32 effReleaseSlotWorkAllocation(EffectSlotSet *owner) {
    sdfMemFreeDirect(owner->workAllocation);
    return 1;
}

/* Return the normal slot work unless the slot carries an alternate payload. */
void *effGetSlotWorkOrOverride(EffectSlotSet *work, s32 slotIndex) {
    void *alternatePayload;
    s32 entryAddress;

    entryAddress = slotIndex * EFF_SLOT_WORK_BYTES + (s32)work->workEntries;
    alternatePayload = ((BdWork *)entryAddress)->alternate.payload;
    if (alternatePayload != NULL) {
        entryAddress = (s32)alternatePayload;
    }
    return (void *)entryAddress;
}

/* Start at zero when moving forward, otherwise at the full 16.16 endpoint. */
void effInitializeSlotPhase(EffTimedState *effect) {
    if (effect->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
        effect->value = EFF_SLOT_PHASE_ZERO;
    } else {
        effect->value = EFF_SLOT_PHASE_ONE;
    }
}

extern s8 D_0038FB70[];

extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *effect, u32 slot, void *entryData);

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
        s32 color = description->cornerColors[D_0038FB70[i]];
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
void effInitializeAllSlotWork(EffectSlotSet *work) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < work->count; slotIndex++) {
        effResetSlotWork(work, slotIndex);
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
void effResetSlotWork(EffectSlotSet *work, u32 slotIndex) {
    BdWork *entry;

    entry = &work->workEntries[slotIndex];
    memset(entry, 0, EFF_SLOT_WORK_BYTES);
    effAttachSlotWorkOwner(work, slotIndex, entry);
}

/* Acquire missing selected handles or clear slots; clearAllSlots does not release textures.
 * Return the address following the processed resource-table entries.
 */
u8 *effResolveResourceSlots(EffectSlotSet *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot) {
    u32 slotIndex = 0;
    u8 *entryBytes = resourceBytes;

    entryBytes += ((EffectResourceTable *)entryBytes)->entryOffset;

    if (set->textureCount != 0) {
        do {
            s32 *resourceData = (s32 *)(resourceBytes + ((EffectResourceEntry *)entryBytes)->resourceOffset);

            entryBytes += EFF_RESOURCE_TABLE_ENTRY_BYTES;
            if (clearAllSlots == 0) {
                if (selectedSlot == -1 || selectedSlot == (s32)slotIndex) {
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
        u8 *data = (u8 *)sdfMemLock(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, -1);
        sdfMemUnlock(owner->sourceAllocation);
    }
}

void effResolveAndReleaseSelectedResource(EffectSlotSet *owner, s32 slot) {
    if (owner->sourceAllocation != 0) {
        u8 *data = (u8 *)sdfMemLock(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, slot);
        sdfMemUnlock(owner->sourceAllocation);
    }
}

void effReleaseTextureHandlesAndResetSlots(EffectSlotSet *set) {
    u32 i;

    for (i = 0; i < set->textureCount; i++) {
        if (set->textureReferences[i] != 0) {
            sdfTexReleaseReference(set->textureReferences[i]);
            set->textureReferences[i] = 0;
        }
    }
    effInitializeAllSlotWork(set);
}

u8 effHasFirstTextureHandle(const EffectSlotSet *set) {
    return set->textureReferences[0] != NULL;
}

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
EffPayload *effCreatePayload(u32 recordCount) {
    u32 recordBytes = recordCount * EFF_PAYLOAD_RECORD_BYTES;
    EffPayload *header = sdfChipAllocDirect(sizeof(EffPayload));
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(recordBytes);
    u8 *records;

    header->count = recordCount;
    header->allocation = allocation;
    records = (u8 *)sdfMemLock(allocation);
    header->records = records;
    memset(records, 0, recordBytes);
    return header;
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
    u32 *entries;
    u32 index;
    u32 sourceOffset;

    set = sdfChipAllocDirect(sizeof(EffectSlotSet));
    memset(set, 0, 0x30);
    set->sharesTextureReferences = EFF_SLOT_TEXTURE_REFERENCES_OWNED;
    set->sourceAllocation = keepAllocation != 0 ? resourceAllocation : 0;
    resource = (u8 *)sdfMemLock(resourceAllocation);
    set->textureCount = ((EffectResourceTable *)resource)->textureCount;
    set->textureAllocation =
        sdfMemAllocDirect(set->textureCount * 4);
    set->textureReferences = (sdfTexHandle_t **)sdfMemLock(set->textureAllocation);
    memset(set->textureReferences, 0, set->textureCount * 4);
    entries = (u32 *)effResolveResourceSlots(set, resource,
        keepAllocation, -1);

    set->count = ((EffectResourceTable *)resource)->slotCount;
    set->descriptionAllocation =
        sdfMemAllocDirect(set->count * 0x80);
    set->descriptions = (EffectSlotDescription *)sdfMemLock(set->descriptionAllocation);
    set->workAllocation = sdfMemAllocDirect(set->count * 0xA0);
    set->workEntries = (BdWork *)sdfMemLock(set->workAllocation);
    for (index = 0; index < set->count; index++) {
        sourceOffset = entries[1];
        memcpy(&set->descriptions[index],
            resource + sourceOffset, 0x80);
        effResetSlotWork(set, index);
        entries += 2;
    }
    return set;
}

EffectSlotSet *effCreateResourceSlotSet(EffectSlotSet *borrowedSource, u32 slot, u32 count) {
    EffectSlotSet *effect = (EffectSlotSet *)sdfChipAllocDirect(sizeof(EffectSlotSet));
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

u32 effSetSlotResourceAndFlags(EffTimedState *record, EffMappedRecord *entry, u32 flags) {
    record->flags = flags;
    record->source = entry;
    if ((flags & EFF_TIMED_STATE_INITIALIZE_PHASE_ON_BIND) != 0) {
        effInitializeSlotPhase(record);
    }
    record->delay = record->delay + 1;
    return 1;
}

u32 effSetSlotIndexedResource(EffTimedState *target, EffMappedResource *resources, s32 item,
                              u32 flags) {
    effSetSlotResourceAndFlags(target, &resources->records[item], flags);
    return 1;
}

u32 effSlotTransitionClearTarget(EffTimedState *record, u32 unused) {
    record->source = 0;
    return 1;
}

u32 effClampSlotPhaseAtEnd(EffectSlotSet *owner, s32 index, EffTimedState *effect) {
    if (effect->value > EFF_SLOT_PHASE_ONE) {
        s32 flags = effect->flags;
        effect->value = EFF_SLOT_PHASE_ONE;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                effect->flags = flags & ~EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, index);
            }
            return 0;
        }
    }
    return 1;
}

u32 effClampSlotPhaseAtStart(EffectSlotSet *owner, s32 index, EffTimedState *effect) {
    if (effect->value < EFF_SLOT_PHASE_ZERO) {
        s32 flags = effect->flags;
        effect->value = EFF_SLOT_PHASE_ZERO;
        if (flags & EFF_TIMED_STATE_RESTART_AT_ENDPOINT) {
            if (flags & EFF_TIMED_STATE_PING_PONG) {
                effect->flags = flags | EFF_TIMED_STATE_DIRECTION_FORWARD;
            } else {
                effInitializeSlotWork(owner, index);
            }
            return 0;
        }
    }
    return 1;
}

extern u32 effResetRecordRun(u8 *work, u32 first, u32 unused);

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
            EffRecordBucket *group = &D_0038FD88[source->category];
            s32 phaseStep = group->step(record, entry, state);

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

u32 effSetSlotOverrideWork(EffectSlotSet *owner, s32 index, void *payload) {
    if (owner->workEntries[index].alternate.payload == NULL) {
        effAttachSlotWorkOwner(owner, index, payload);
    }
    owner->workEntries[index].alternate.payload = payload;
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

u32 effClearSlotOverrideWork(s32 work, s32 index) {
    ((EffectSlotSet *)work)->workEntries[index].alternate.payload = NULL;
    return 1;
}

u32 effConfigureSlotResource(s32 work, s32 index, u32 value, u32 flags) {
    BdWork *effect = &((EffectSlotSet *)work)->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], (EffMappedRecord *)(u32)value, flags);
    effUpdateTimedStates((EffectSlotSet *)work, index, effect);
    return 1;
}

u32 effConfigureIndexedSlotResource(EffectSlotSet *work, s32 index,
                                    EffMappedResource *data, s32 item, u32 flags) {
    BdWork *effect = &work->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], &data->records[item], flags);
    effUpdateTimedStates(work, index, effect);
    return 1;
}

u32 effConfigureIndexedSlotMaterial(EffectSlotSet *work, s32 index,
                  EffMappedResource *data, s32 item,
                  u32 materialFlags, u32 materialValue, u32 stateFlags) {
    BdWork *effect = &work->workEntries[index];
    effSetSlotResourceAndFlags(&effect->states[0], &data->records[item], stateFlags);
    effUpdateTimedStates(work, index, effect);
    effect->states[0].materialFlags = materialFlags;
    effect->states[0].materialValue = materialValue;
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
        effSlotTransitionClearTarget((EffTimedState *)((first + i) * EFF_SLOT_WORK_BYTES +
                                    (s32)((EffectSlotSet *)work)->workEntries + 0x28), unused);
        i++;
        index = first + i;
    } while (index < ((EffectSlotSet *)work)->count && (((EffectSlotSet *)work)->descriptions[index].flags & EFF_SLOT_DESCRIPTION_SEQUENCE_CONTINUATION));
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD198);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1A0);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1A8);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1B0);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1B8);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1C0);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1C8);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1D0);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1D8);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1E0);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1E8);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1F0);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD1F8);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD200);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD208);

INCLUDE_SDATA(const s32, "game/code_002BC8F0", D_003BD210);

