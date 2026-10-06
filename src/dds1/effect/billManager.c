#include "common.h"
#include "eff.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "sdf.h"

extern BillDispatch D_0034E060[];
extern BillDispatch D_0034E068[];

void *sdfAllocSizeClassBlock(s32 size);
void *sdfReadNamedResource(s32 arg0, u32 *arg1, s32 arg2);
void sdfReleaseResourceAllocation(void *arg);
void sdfReleaseChipBlock(void *arg);
void effReleaseSharedTextureRecord(void *arg);
void func_001502B0(void *arg0, void *arg1);
void billReleaseSharedEntryBlock(void *arg);
void *func_00150148(void *arg);
void billSetAnimationEntry(BillObj *arg0, s32 arg1);
void *func_00151A88(void *arg);

INCLUDE_ASM(const s32, "effect/billManager", func_001502B0);

typedef struct BillDeferredDescriptor {
    u8 pad00[0x10];
    void (*dispatch)(void *descriptor, u32 value);
} BillDeferredDescriptor;

typedef struct BillManagerNode BillManagerNode;

struct BillManagerNode {
    u8 pad00[0x2C];
    SdfListHead *pendingLists[5];
    u16 packetListIndex;
    u8 pad42[6];
    void *work;
    BillManagerNode *next;
};

typedef struct BillPacketWork {
    u8 pad00[0x3FC];
    s32 count;
} BillPacketWork;

extern BillManagerNode *D_003BD7F4;
extern BillDeferredDescriptor *D_0034E030[5];
extern void sdfAppendPacket(SdfListHead *list, u32 packet);
extern u32 sdfBuildCompactVertexVifPacket(void *work, void *arg1, void *arg2, void *arg3, s32 count, s32 callback);

void func_00150750(void) {
    BillManagerNode *node;

    node = D_003BD7F4;
    if (node != NULL) {
        do {
            BillPacketWork *work = node->work;
            s32 count = work->count;
            s32 i;

            if (count > 0) {
                u32 packet = sdfBuildCompactVertexVifPacket(work, (u8 *)work + 0xF0, (u8 *)work + 0x12C,
                                           (u8 *)work + 0x21C, count, 0);
                sdfAppendPacket(node->pendingLists[node->packetListIndex], packet);
                work->count = 0;
            }

            for (i = 0; i < 5; i++) {
                SdfListHead *value = node->pendingLists[i];

                if (value != NULL) {
                    D_0034E030[i]->dispatch(D_0034E030[i], (u32)value);
                    node->pendingLists[i] = 0;
                }
            }

            {
                BillManagerNode *next = node->next;
                node->next = node;
                node = next;
            }
        } while (node != NULL);
    }
    D_003BD7F4 = NULL;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00150840);

typedef struct BillDrawNode {
    BillManagerNode *first;
    BillManagerNode *second;
    u8 pad08[0xC];
    SdfListHead *packetList;
    struct BillDrawNode *next;
} BillDrawNode;
INCLUDE_ASM(const s32, "effect/billManager", func_00150EB0);


typedef struct BillDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct BillDrawSurface *, SdfListHead *);
} BillDrawSurface;

typedef struct BillStatePacket {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
    u64 test;
    u64 testRegister;
    u64 alpha;
    u64 alphaRegister;
} BillStatePacket;

extern BillDrawNode *D_003BD7F8;
extern BillDrawSurface D_00325228;
extern u8 kwlnFrameDrawPacketRecords[];
extern u32 kwlnGetDrawBufferIndex(void);
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendDmaTagToList(SdfListHead *, u32);
extern void func_002D4CC8(const void *, void *, s32);
extern void func_00150EB0(BillDrawNode *);

void func_00151010(void) {
    BillDrawNode *node = D_003BD7F8;
    SdfListHead *list;
    void *texture;
    BillStatePacket *packet;

    if (node != NULL) {
        do {
            BillPacketWork *work = node->second->work;
            if (work->count != 0) {
                func_00150EB0(node);
            }
            D_00325228.submit(&D_00325228, node->packetList);
            node->packetList = NULL;
            node = node->next;
        } while (node != NULL);
    }
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4CC8(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40, texture, 0);
    sdfAppendDmaTagToList(list, (u32)texture);
    packet = sdfAllocPacketAligned(0x40);
    packet->dmaTag = 3;
    packet->vifCommands = 0x5000000310000000ULL;
    packet->gifTag = 0x1000000000008002ULL;
    packet->gifRegisters = 0xE;
    packet->test = 0x71801;
    packet->testRegister = 0x47;
    packet->alpha = 0x48;
    packet->alphaRegister = 0x42;
    sdfAppendPacket(list, (u32)packet);
    D_00325228.submit(&D_00325228, list);
    D_003BD7F8 = NULL;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151178);

BillObj *billAllocChild(void *resourceData) {
    BillObj *obj;

    obj = sdfAllocSizeClassBlock(0x34);
    obj->entryList = NULL;
    if (resourceData != NULL) {
        obj->entryList = func_00150148(resourceData);
    }
    return obj;
}

void billReleaseChild(BillObj *obj) {
    if (obj->entryList != NULL) {
        effReleaseSharedTextureRecord(obj->entryList);
    }
    sdfReleaseChipBlock(obj);
}

void billProcessChild(BillObj *obj) {
    func_001502B0(obj, obj->entryList);
}

BillObj *billAllocList(void *resourceData) {
    BillData *data;
    BillObj *newobj;
    s32 n;

    data = NULL;
    if (resourceData != NULL) {
        data = func_00151A88(resourceData);
    }
    n = data->entryCount;
    newobj = sdfAllocSizeClassBlock(n * 20 + 0x6C);
    newobj->entryList = data;
    newobj->unk60 = (u8 *)newobj + 0x6C;
    newobj->unk50 = 1;
    newobj->unk48 = 0;
    newobj->unk4C = 0;
    newobj->pair.unk8 = 0;
    billSetAnimationEntry(newobj, 0);
    return newobj;
}

BillObj *billCloneList(BillObj *obj) {
    BillData *data;
    s32 n;
    BillObj *newobj;

    data = obj->entryList;
    n = data->entryCount;
    data->listRefCount = data->listRefCount + 1;
    newobj = sdfAllocSizeClassBlock(n * 20 + 0x6C);
    newobj->entryList = data;
    newobj->unk60 = (u8 *)newobj + 0x6C;
    newobj->unk50 = 1;
    newobj->unk48 = 0;
    newobj->unk4C = 0;
    billSetAnimationEntry(newobj, 0);
    return newobj;
}

void billReleaseList(BillObj *obj) {
    billReleaseSharedEntryBlock(obj->entryList);
    sdfReleaseChipBlock(obj);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151398);

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 effBillModulateColors(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

INCLUDE_ASM(const s32, "effect/billManager", func_001515E8);

/* Resolves an indexed billboard record and caches its signed +0x12 value. */
void billResolveEntry(BillData *table, s32 index, BillOut *out) {
    u8 *base;
    BillAnimationEntry *entry;
    s32 offset;
    s16 value;

    base = table->base;
    entry = table->entries + index;
    offset = entry->offset;
    out->entry = entry;
    base = base + offset;
    out->frameIndex = 0;
    value = ((BillRecord *)base)->value;
    out->record = (BillRecord *)base;
    out->framesRemaining = value;
}

extern s32 func_003003F0(const char *, ...);

/* Select an animation and initialize the plural records' signed start delays. */
void billSetAnimationEntry(BillObj *obj, s32 index) {
    BillData *data = obj->entryList;
    BillAnimationEntry *entry = data->entries + index;

    if (entry->frameCount == 0) {
        obj->unk50 = 0;
        return;
    }
    if (entry->flags & 0x10000000) {
        BillRecord *records;
        u32 i = 0;

        func_003003F0("billAnim..PLURAL SET\n");
        obj->modeFlags = 0x10000000;
        obj->unk58 = index;
        obj->entryCount = entry->frameCount;
        records = (BillRecord *)(data->base + entry->offset);
        for (; i < entry->frameCount; i++) {
            billResolveEntry(data, records[i].entryIndex, (BillOut *)obj->unk60 + i);
            ((BillOut *)obj->unk60)[i].frameIndex = -records[i].delay;
        }
    } else if (entry->unk8 & 0xC0) {
        func_003003F0("billAnim..(A)MTEX SET\n");
        obj->modeFlags = entry->unk8;
        obj->unk58 = index;
        obj->entryCount = 2;
        billResolveEntry(data, index, obj->unk60);
        billResolveEntry(data, index + 1, (BillOut *)obj->unk60 + 1);
    } else {
        obj->modeFlags = 0;
        obj->entryCount = 1;
        obj->unk58 = index;
        billResolveEntry(data, index, obj->unk60);
    }
    if (entry->unk8 & 0x100) {
        func_003003F0("billAnim..P2A POLYGON\n");
    }
    obj->unk50 = 1;
}

extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfResourceRetainAddress(SdfMemBlock *allocation);
extern s32 func_003003F0(const char *format, ...);

void *func_00151A88(void *resource) {
    u8 *source = resource;
    s32 *header = resource;
    s32 *childOffsets = (s32 *)(source + header[0]);
    s32 childCount = *childOffsets++;
    SdfMemBlock *allocation;
    BillData *data;
    u8 *base;
    s32 count;
    s32 i;

    allocation = sdfAllocGeneralBlock(header[0] + childCount * 0x50 + sizeof(*data));
    data = (BillData *)sdfResourceRetainAddress(allocation);
    base = (u8 *)(data + 1);
    data->allocation = allocation;
    memcpy(base, source, header[0]);
    data->base = base;
    data->childCount = childCount;
    data->entries = (BillAnimationEntry *)(base + 8);
    data->children = (BillChildPayload **)(base + *(s32 *)base + 8);
    for (i = 0; i < childCount; i++) {
        data->children[i] = func_00150148(source + *childOffsets++);
    }
    data->entryCount = data->listRefCount = 1;
    count = ((s32 *)data->base)[1];
    for (i = 0; i < count; i++) {
        BillAnimationEntry *entry = &data->entries[i];
        if (entry->flags & 0x10000000) {
            data->entryCount = entry->frameCount;
            func_003003F0("billAnim no[%d][%d]...PLURAL\n", i, data->entryCount);
        } else {
            if (entry->unk8 & 0x40) {
                data->entryCount = 2;
                func_003003F0("billAnim no[%d][%d]...MTEX\n", i, 2);
            } else if (entry->unk8 & 0x80) {
                data->entryCount = 2;
                func_003003F0("billAnim no[%d][%d]...AMTEX\n", i, 2);
            }
        }
    }
    return data;
}


/* Drop one reference; the last one releases every entry and the block itself. */
void billReleaseSharedEntryBlock(void *arg) {
    BillData *block = arg;
    s32 i;

    block->listRefCount--;
    if (block->listRefCount == 0) {
        i = 0;
        while (i < block->childCount) {
            effReleaseSharedTextureRecord(block->children[i]);
            i++;
        }
        sdfReleaseResourceAllocation(block->allocation);
    }
}


typedef struct BillSnapshot {
    f32 x;              /* 0x00 */
    f32 y;              /* 0x04 */
    f32 halfWidth;      /* 0x08 */
    f32 halfHeight;     /* 0x0C */
    u32 unk10;          /* 0x10 */
    BillTextureQuad uv; /* 0x14 */
} BillSnapshot;

extern void *memcpy(void *, const void *, u32);
extern BillChildPayload *func_00151398(BillObj *obj, BillOut *entries);

/* Copy the billboard's current source record (by kind) into a snapshot. */
void billCopyCurrentRecordToSnapshot(BillObj *obj, BillSnapshot *snapshot) {
    BillChildPayload *record;

    if (obj->kind == 1) {
        record = func_00151398(obj, obj->unk60);
    } else if (obj->kind == 0 || obj->kind == 3) {
        record = obj->entryList;
    } else {
        return;
    }
    snapshot->x = record->x;
    snapshot->unk10 = record->value;
    snapshot->y = record->y;
    snapshot->halfWidth = record->halfWidth;
    snapshot->halfHeight = record->halfHeight;
    memcpy(&snapshot->uv, &record->uv, sizeof(snapshot->uv));
}

BillObj *billCreateIndexed(s32 index, u32 data) {
    BillObj *newobj;

    newobj = D_0034E060[index].func(data);
    func_00151178(newobj);
    newobj->kind = index;
    newobj->callback = D_0034E060[index].callback;
    return newobj;
}

void *billCreateFromResource(s32 kind, s32 resource) {
    void *allocation;
    void *billboard;
    u32 header[4];

    allocation = sdfReadNamedResource(resource, header, 0);
    billboard = billCreateIndexed(kind, header[0]);
    sdfReleaseResourceAllocation(allocation);
    return billboard;
}

extern void func_00151178(BillObj *obj);

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
BillObj *billCloneObjectRetainingSharedData(BillObj *source) {
    BillObj *copy;
    BillChildPayload *data;

    if (source->kind == 1) {
        copy = billCloneList(source);
        func_00151178(copy);
        copy->kind = source->kind;
        copy->callback = source->callback;
    } else {
        copy = billAllocChild(NULL);
        func_00151178(copy);
        copy->kind = source->kind;
        copy->callback = source->callback;
        data = source->entryList;
        data->refCount = data->refCount + 1;
        copy->entryList = data;
    }
    return copy;
}

void billDispatchByKind(BillObj *obj) {
    D_0034E068[obj->kind].func();
}

void billInvokeCallback(BillObj *obj) {
    obj->callback();
}
