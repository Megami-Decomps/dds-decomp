#include "common.h"
#include "sdf_packet_list.h"
#include "sdf_texture_draw_packet.h"
#include "itf_draw_grid.h"
#include "eff_ref_obj.h"
#include "sdf_resource.h"
#include "file_request_api.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "eff.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "eff_record_bucket.h"
#include "eff_owner_records.h"
#include "sdf.h"

extern SdfTex *sdfTexAcquireResourceTexture(void *resourceAddress);


extern u32 D_003BD11C;

extern u32 D_003BD120;

extern u32 D_003BD128;

extern u32 D_003BD158;

extern u32 D_003BD15C;

extern u32 D_003BD160;

extern u32 D_003BD124;

extern s32 effQueuedResourceNameRecord;

extern s32 effResourceBankEntries;

extern s32 effResourceBankDescriptor;

extern s32 D_003BD098;

extern s32 effQueuedFileObject;

extern s32 effTemporaryFileJob;

extern u32 effScalyTextureHandle;

extern s32 effSharedStripReferenceCount;

extern u32 effSharedScalyStripResource;

extern u32 effWindTextureHandle;

extern u32 D_003BC984;

extern u32 D_003BC994;

extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);

extern u32 effQueuedFileHandle;

extern u32 effCurrentRenderPacket;

extern s32 effSharedRibbonReferenceCount;

extern u32 D_003BC990;

extern u32 effFlashTextureHandles;

extern u32 D_003BC96C;

extern u32 D_003BC950;

extern u32 D_003BC954;

typedef struct EffectObjectFlag {
    u8 pad00[0xC];
    u32 flags;
} EffectObjectFlag;

typedef struct EffectObjectNode {
    EffectObjectFlag *object;
    struct EffectObjectNode *prev;
    struct EffectObjectNode *next;
} EffectObjectNode;

extern EffectObjectNode *effFloorModelListHead;

extern u32 D_003BD058;

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
} EffectResourceTable;

typedef struct EffectResourceEntry {
    u32 unknown;
    u32 resourceOffset;
} EffectResourceEntry;


extern u16 D_003BC944;

extern u8 *D_003BC958;

extern void sdfTexReleaseReference(SdfTex *texture);


extern u32 D_003BC960[2];

extern s8 D_003BC9AC;

extern f32 D_003BC9A8;

extern char D_003BD080[];

extern s32 func_003014F0(char *, const char *, ...);

extern u8 D_003BD078[];

extern u8 D_003BCF30[];

extern s32 func_003014F0(char *, const char *, ...);

extern u32 D_003BD064;

extern void *sdfAllocSizeClassBlock(u32);

extern void sdfReleaseChipBlock(void *);

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
#define EFF_BATCH_HEADER_BYTES 0xC
#define EFF_SLOT_WORK_BYTES 0xA0
#define EFF_PHASE_FULL 0x10000
#define EFF_PAYLOAD_RECORD_BYTES 0x6C
#define EFF_RESOURCE_TABLE_ENTRY_BYTES 8

/* Load and instantiate a resource; only a zero keepAllocation releases the source allocation. */
EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, u32 keepAllocation) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    u32 allocation;
    EffectSlotSet *instance;

    func_003014F0(path, D_003BD198, base, name);
    allocation = sdfReadNamedResource(path, &sourceAddress, 0);
    instance = effCreateResourceSlotSetFromAllocation((struct SdfMemBlock *)allocation, keepAllocation);
    if (keepAllocation == EFF_RESOURCE_TRANSIENT) {
        sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    }
    return instance;
}

/* Publish the instance, release its source allocation, then clean up the completed file job. */
void effCompleteTransientResourceJob(struct FileRequest *job, u32 *outInstance) {
    u32 allocation;
    EffectSlotSet *instance;

    allocation = fileGetResourceHandle(job);
    instance = effCreateResourceSlotSetFromAllocation((struct SdfMemBlock *)allocation, EFF_RESOURCE_TRANSIENT);
    *outInstance = (u32)instance;
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    filePollEntryCleanup(job);
}

/* Publish the instance without releasing its source allocation, then clean up the file job. */
void effCompleteRetainedResourceJob(struct FileRequest *job, u32 *outInstance) {
    u32 allocation;
    EffectSlotSet *instance;

    allocation = fileGetResourceHandle(job);
    instance = effCreateResourceSlotSetFromAllocation((struct SdfMemBlock *)allocation, EFF_RESOURCE_KEEP_ALLOCATION);
    *outInstance = (u32)instance;
    filePollEntryCleanup(job);
}

extern s32 func_003014F0(char *, const char *, ...);

/* Clear the output first; only mode one selects the retained-allocation completion path. */
void effRequestResourceByMode(const char *base, const char *name, u32 mode, u32 *outInstance) {
    char path[EFF_RESOURCE_PATH_BYTES];

    func_003014F0(path, D_003BD198, base, name);
    *outInstance = 0;
    if (mode == EFF_RESOURCE_KEEP_ALLOCATION) {
        fileCreateCallbackRequest(path, 0, (u32)effCompleteRetainedResourceJob, (u32)outInstance);
    } else {
        fileCreateCallbackRequest(path, 0, (u32)effCompleteTransientResourceJob, (u32)outInstance);
    }
}

/* Build mapped records from the retained source address, then release the original file allocation. */
EffMappedResource *effLoadMappedResource(const char *base, const char *name) {
    char path[EFF_RESOURCE_PATH_BYTES];
    u32 sourceAddress;
    EffMappedResource *mappedResource;
    u32 allocation;

    func_003014F0(path, D_003BD198, base, name);
    allocation = sdfReadNamedResource(path, &sourceAddress, 0);
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    return mappedResource;
}

/* Publish mapped records before releasing their source allocation and completing the file job. */
void effCompleteMappedResourceJob(struct FileRequest *job, u32 *outMappedResource) {
    u32 allocation;
    u32 sourceAddress;
    EffMappedResource *mappedResource;

    allocation = fileGetResourceHandle(job);
    sourceAddress = sdfResourceRetainAddress((struct SdfMemBlock *)(allocation));
    mappedResource = effCreateMappedResource((const u8 *)sourceAddress);
    *outMappedResource = (u32)mappedResource;
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(allocation));
    filePollEntryCleanup(job);
}

/* Initialize the output to zero and schedule mapped-record completion. */
void effRequestMappedResource(const char *base, const char *name, u32 *outMappedResource) {
    char path[EFF_RESOURCE_PATH_BYTES];

    func_003014F0(path, D_003BD198, base, name);
    *outMappedResource = 0;
    fileCreateCallbackRequest(path, 0, (u32)effCompleteMappedResourceJob, (u32)outMappedResource);
}

/* Create an owner list with sixteen initially empty record buckets. */
EffectOwnerRecord *effCreateOwnerRecordList(void *owner) {
    EffectOwnerRecord *list = sdfAllocSizeClassBlock(EFF_OWNER_LIST_BYTES);
    memset(list, 0, EFF_OWNER_LIST_BYTES);
    list->owner = owner;
    return list;
}

/* Per-slot effect data (0x80 bytes each); only the bucket index is known. */
typedef struct EffectSlot {
    u8 pad_0x00[0x28]; // 0x00
    s32 bucket;        // 0x28
    u8 pad_0x2C[0x54]; // 0x2C
} EffectSlot; // 0x80

typedef struct EffectSlotOwner {
    u8 pad_0x00[0x10];  // 0x00
    EffectSlot *slots;  // 0x10
} EffectSlotOwner;

/* Clamp only the upper bucket bound and prepend; the former head's prev link is not rewritten. */
void effInsertSlotRecord(void *owner, EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucket;
    EffectRecord *record;

    if (bucketIndex >= EFF_RECORD_BUCKETS) {
        bucketIndex = EFF_RECORD_LAST_BUCKET;
    }
    record = sdfAllocSizeClassBlock(sizeof(*record));
    record->prev = 0;
    record->owner = owner;
    record->slot = slotIndex;
    record->next = list->entries[bucketIndex];
    list->entries[bucketIndex] = record;
}

/* Remove the first matching slot from the raw bucket index; unlike insertion, this does not clamp it. */
s32 effRemoveSlotRecord(EffectOwnerRecord *list, EffectSlotOwner *work, s32 slotIndex) {
    EffectSlot *slotData = &work->slots[slotIndex];
    s32 bucketIndex = slotData->bucket;
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
            sdfReleaseChipBlock(record);
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
            sdfReleaseChipBlock(record);
            record = record->next;
        }
        ++bucketHead;
    } while (--bucketCountdown >= 0);
}

extern void itfGridLookupValueOrDefault(void *, s32);

/* Dispatch every bucket record and optionally refresh its owner/slot lookup. */
u32 effDispatchRecordBuckets(u32 refresh, EffectOwnerRecord *list, s32 drawOption) {
    EffectRecord **bucketHead = list->entries;
    s32 bucketCountdown = EFF_RECORD_LAST_BUCKET;

    do {
        EffectRecord *record = *bucketHead;
        while (record != 0) {
            itfDrawGridWithResolvedSlot(0, 0, 0, 0, (EffectSlotSet *)list->owner, record->slot, drawOption);
            if (refresh != 0) {
                itfGridLookupValueOrDefault(list->owner, record->slot);
            }
            record = record->next;
        }
        ++bucketHead;
    } while (--bucketCountdown >= 0);
    return 1;
}

/* Release bucket records before the owner list; preserve the existing short-arity K&R call. */
u32 effDestroyOwnerRecordList(u32 list) {
    effReleaseRecordBuckets();
    sdfReleaseChipBlock(list);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", effConvertParamValue);

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
struct SdfMemBlock *effLoadMappedStatusRecords(const u8 *source, EffMappedHeader *headerOut) {
    EffMappedHeader header;
    struct SdfMemBlock *allocation;
    EffMappedRecord *records;
    u32 recordIndex = 0;
    u32 statusBytes;

    memcpy(&header, source, sizeof(header));
    source += sizeof(header);
    allocation = sdfAllocGeneralBlock(header.count * EFF_STATUS_RECORD_BYTES);
    records = (EffMappedRecord *)sdfResourceRetainAddress(allocation);
    for (; recordIndex < header.count; recordIndex++) {
        EffMappedRecord *record = &records[recordIndex];

        memcpy(record, source, EFF_PACKED_STATUS_HEADER_BYTES);
        source += EFF_PACKED_STATUS_HEADER_BYTES;
        statusBytes = effSumRecordStatuses(records);
        if (statusBytes < record->statusBytes) {
            statusBytes = record->statusBytes;
        }
        record->status = sdfAllocSizeClassBlock(statusBytes);
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
    EffMappedResource *mappedResource = (EffMappedResource *)sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    EffMappedHeader header;

    mappedResource->allocation = effLoadMappedStatusRecords(source, &header);
    mappedResource->records = (EffMappedRecord *)sdfResourceRetainAddress(mappedResource->allocation);
    mappedResource->count = header.count;
    return mappedResource;
}

/* Build one zeroed status record and allocate the category's required status storage. */
EffMappedResource *effCreateStatusBatch(u32 category) {
    EffMappedResource *batch = sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    struct SdfMemBlock *allocation;
    u32 recordAddress;
    u32 statusBytes;
    void *statuses;

    batch->count = 1;
    allocation = sdfAllocGeneralBlock(EFF_STATUS_RECORD_BYTES);
    batch->allocation = allocation;
    recordAddress = sdfResourceRetainAddress(allocation);
    batch->records = (EffMappedRecord *)recordAddress;
    memset((void *)recordAddress, 0, EFF_STATUS_RECORD_BYTES);
    {
        EffMappedRecord *record = batch->records;
        record->category = category;
        statusBytes = effSumRecordStatuses(record);
    }
    statuses = sdfAllocSizeClassBlock(statusBytes);
    batch->records->status = statuses;
    memset(statuses, 0, statusBytes);
    batch->records->statusBytes = statusBytes;
    return batch;
}


/* Release each record's status storage, then the record allocation and batch header. */
u32 effDestroyPackedBatch(EffMappedResource *batch) {
    s32 recordIndex;
    for (recordIndex = 0; recordIndex < batch->count; recordIndex++) {
        sdfReleaseChipBlock(batch->records[recordIndex].status);
    }
    sdfReleaseResourceAllocation(batch->allocation);
    sdfReleaseChipBlock(batch);
    return 1;
}

u32 effReleaseSlotWorkAllocation(EffectSlotSet *owner) {
    sdfReleaseResourceAllocation(owner->workAllocation);
    return 1;
}

/* Return the normal slot address unless its stored alternate address is nonzero. */
void *effGetSlotWorkOrOverride(EffectSlotSet *work, s32 slotIndex) {
    s32 alternateAddress;
    s32 entryAddress;

    entryAddress = slotIndex * EFF_SLOT_WORK_BYTES + (s32)work->workEntries;
    alternateAddress = ((BdWork *)entryAddress)->alternate.address;
    if (alternateAddress != 0) {
        entryAddress = alternateAddress;
    }
    return (void *)entryAddress;
}

/* Start at zero when moving forward, otherwise at the full 16.16 endpoint. */
void effInitializeSlotPhase(EffTimedState *effect) {
    if (effect->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
        effect->value = 0;
    } else {
        effect->value = EFF_PHASE_FULL;
    }
}

INCLUDE_ASM(const s32, "game/code_002BC8F0", effInitializeSlotWorkFromDescription);

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
u32 effResolveResourceSlots(EffectSlotSet *set, u8 *resourceBytes, s32 clearAllSlots, s32 selectedSlot) {
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
                        set->textureReferences[slotIndex] = sdfTexAcquireResourceTexture(resourceData);
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
    return (u32)entryBytes;
}

void effResolveAndReleaseResource(EffectSlotSet *owner) {
    if (owner->sourceAllocation != 0) {
        u8 *data = (u8 *)sdfResourceRetainAddress(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, -1);
        sdfDecrementAllocationReferenceCount(owner->sourceAllocation);
    }
}

void effResolveAndReleaseSelectedResource(EffectSlotSet *owner, s32 slot) {
    if (owner->sourceAllocation != 0) {
        u8 *data = (u8 *)sdfResourceRetainAddress(owner->sourceAllocation);
        effResolveResourceSlots(owner, data, 0, slot);
        sdfDecrementAllocationReferenceCount(owner->sourceAllocation);
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

u8 effHasFirstTextureHandle(EffectSlotSet *set) {
    return set->textureReferences[0] != NULL;
}

/* Allocate and clear count 0x6C-byte records; retain the existing allocation/count/address header order. */
EffPayload *effCreatePayload(u32 recordCount) {
    u32 recordBytes = recordCount * EFF_PAYLOAD_RECORD_BYTES;
    EffPayload *header = sdfAllocSizeClassBlock(EFF_BATCH_HEADER_BYTES);
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(recordBytes);
    u8 *records;

    header->count = recordCount;
    header->allocation = allocation;
    records = (u8 *)sdfResourceRetainAddress(allocation);
    header->records = records;
    memset(records, 0, recordBytes);
    return header;
}

/* Release the record allocation before freeing its small header. */
u32 effDestroyPayload(EffPayload *payload) {
    sdfReleaseResourceAllocation(payload->allocation);
    sdfReleaseChipBlock(payload);
    return 1;
}

EffectSlotSet *effCreateResourceSlotSetFromAllocation(struct SdfMemBlock *resourceAllocation, u32 keepAllocation) {
    EffectSlotSet *set;
    u8 *resource;
    u32 *entries;
    u32 index;
    u32 sourceOffset;

    set = sdfAllocSizeClassBlock(0x30);
    memset(set, 0, 0x30);
    set->sharesTextureReferences = 0;
    set->sourceAllocation = keepAllocation != 0 ? resourceAllocation : 0;
    resource = (u8 *)sdfResourceRetainAddress(resourceAllocation);
    set->textureCount = *(u16 *)(resource + 0x14);
    set->textureAllocation =
        sdfAllocGeneralBlock(set->textureCount * 4);
    set->textureReferences = (SdfTex **)sdfResourceRetainAddress(set->textureAllocation);
    memset(set->textureReferences, 0, set->textureCount * 4);
    entries = (u32 *)effResolveResourceSlots(set, resource,
        keepAllocation, -1);

    set->count = *(u16 *)(resource + 0x16);
    set->descriptionAllocation =
        sdfAllocGeneralBlock(set->count * 0x80);
    set->descriptions = (EffectSlotDescription *)sdfResourceRetainAddress(set->descriptionAllocation);
    set->workAllocation = sdfAllocGeneralBlock(set->count * 0xA0);
    set->workEntries = (BdWork *)sdfResourceRetainAddress(set->workAllocation);
    for (index = 0; index < set->count; index++) {
        sourceOffset = entries[1];
        memcpy(&set->descriptions[index],
            resource + sourceOffset, 0x80);
        effResetSlotWork(set, index);
        entries += 2;
    }
    return set;
}

EffectSlotSet *effCreateResourceSlotSet(EffectSlotSet *source, u32 slot, u32 count) {
    EffectSlotSet *effect = (EffectSlotSet *)sdfAllocSizeClassBlock(0x30);
    u32 index = 0;
    effect->sharesTextureReferences = 1;
    {
        u32 mode = source->textureCount;
        SdfTex **textureReferences = source->textureReferences;
        effect->textureCount = mode;
        effect->textureReferences = textureReferences;
    }
    effect->sourceAllocation = 0;
    effect->textureAllocation = 0;
    effect->count = count;
    effect->descriptionAllocation = sdfAllocGeneralBlock(count * 0x80);
    effect->descriptions = (EffectSlotDescription *)sdfResourceRetainAddress(effect->descriptionAllocation);
    effect->workAllocation = sdfAllocGeneralBlock(effect->count * 0xA0);
    effect->workEntries = (BdWork *)sdfResourceRetainAddress(effect->workAllocation);
    if (effect->count != 0) {
        do {
            memcpy(&effect->descriptions[index], &source->descriptions[slot], 0x80);
            effResetSlotWork(effect, index);
            index++;
        } while (index < effect->count);
    }
    return effect;
}

u32 effDestroyResourceSlotSet(EffectSlotSet *set) {
    if (set->sourceAllocation != 0) {
        sdfReleaseResourceAllocation(set->sourceAllocation);
    }
    if (set->sharesTextureReferences == 0) {
        effReleaseTextureHandlesAndResetSlots(set);
        sdfReleaseResourceAllocation(set->textureAllocation);
    }
    sdfReleaseResourceAllocation(set->descriptionAllocation);
    effReleaseSlotWorkAllocation(set);
    sdfReleaseChipBlock(set);
    return 1;
}



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
    if (effect->value > 0x10000) {
        s32 flags = effect->flags;
        effect->value = 0x10000;
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
    if (effect->value < 0) {
        s32 flags = effect->flags;
        effect->value = 0;
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
            s32 step = group->step(record, entry, state);

            if (state->delay > 0) {
                step = 0;
                state->delay -= 1;
            }
            if (step >= 0) {
                if (state->flags & EFF_TIMED_STATE_DIRECTION_FORWARD) {
                    if (state->value != 0x10000) {
                        state->value += step;
                        idle = 0;
                        if (effClampSlotPhaseAtEnd(effect, slot, state) == 0) {
                            state->delay = state->delayMax;
                            return 0;
                        }
                    }
                } else if (state->value != 0) {
                    state->value -= step;
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
    if (owner->workEntries[index].alternate.address == 0) {
        effAttachSlotWorkOwner(owner, index, payload);
    }
    owner->workEntries[index].alternate.address = (s32)payload;
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
    ((EffectSlotSet *)work)->workEntries[index].alternate.address = 0;
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


extern SdfPoolNode kwlnDrawSurfaces[];
typedef struct SdfTex SdfTex;
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

extern s32 sdfAllocPacketAligned(s32);
extern void sdfTexSetPrimaryBufferModeBits(SdfTex *, s32, s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void *sdfConsInitPacketHeader(SdfDrawPacket *, s32, s32, s64, s32);
extern s32 sdfConsMeasurePacketWithHeader(s32);
extern void effSelectPresetByKind(u32, u32);
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);

void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height,
                   const EffSpriteUV *uvRect, const EffSpriteColor *color, u32 flipFlags,
                   u32 blendKind, s32 mode, SdfTex *texture, s32 surfaceId) {
    u32 textureCoordinates[4];
    s32 packet;
    SdfListHead *packetList;
    u64 *packetWords;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 savedCoordinate;
    SdfPoolNode *drawSurface;

    if (mode == 0) {
        sdfTexSetPrimaryBufferModeBits(texture, 0, 1);
    } else {
        sdfTexSetPrimaryBufferModeBits(texture, 1, 1);
    }
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, 1));
    sdfConsInitPacketHeader((SdfDrawPacket *)packet, 0x156, 5, 0x43431, 1);
    /* The SDK size helper also skips the packet's two header quadwords. */
    packetWords = (u64 *)sdfConsMeasurePacketWithHeader(packet);
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
    packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packetList);
    sdfConsCreateDrawPacket(packetList, texture, 0);
    sdfAppendPacket(packetList, packet);
    drawSurface = &kwlnDrawSurfaces[surfaceId];
    drawSurface->append((SdfListHead *)drawSurface, packetList);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surfaceId);
}

extern f32 sdfSinPoly(f32);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32);

void func_002BE728(s32 *outX, s32 *outY, s32 x, s32 y, s32 centerX, s32 centerY, f32 angle) {
    f32 aspect = 1.75f;
    f32 radians = angle * 0.017453293f;
    f32 rotatedX;
    f32 rotatedY;

    y = (s32)(y * aspect);
    rotatedX = sdfEvaluateCosineViaSinePhaseShift(radians) * x - sdfSinPoly(radians) * y;
    rotatedY = sdfSinPoly(radians) * x + sdfEvaluateCosineViaSinePhaseShift(radians) * y;
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


INCLUDE_ASM(const s32, "game/code_002BC8F0", func_002BE8A8);

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void effSelectPresetAndDispatch(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 presetMode, u32 presetValue) {
    effSelectPresetByKind(presetMode, presetValue);
    uiDrawGradientColorRect(arg0, arg1, arg2, arg3, arg4, (const u32 *)arg5, presetValue);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, presetValue);
}

