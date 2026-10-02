#include "common.h"

extern u64 func_00325BB0(u64, u32);

extern u64 func_00325AB8(u64, u32);

extern u64 func_00320AE8(u64, u64, u32 *);

extern u64 func_00325790(u64, u32);

extern void (*sdfTickCallback)(void);

extern void dds3DestroyCallbackNodeAfterLastNotification(u32);
extern u32 func_0035A828(s32 bytes);
extern void func_003211F0(void);

#define DDS_NAMED_RECORD_NAME_BYTES 0x40

typedef struct DdsNamedRecord {
    char *name;       /* 0x00: writable 0x40-byte name buffer */
    u32 value;        /* 0x04: packed offset or resolved address */
} DdsNamedRecord;

typedef struct DdsNamedNode {
    u8 pad00[8];
    struct DdsNamedNode *next; /* 0x08 */
    u8 pad0C[4];
    DdsNamedRecord *record;    /* 0x10 */
} DdsNamedNode;

typedef struct DdsNamedList {
    u8 pad00[4];
    DdsNamedNode *first; /* 0x04 */
} DdsNamedList;

typedef struct DdsPackedObject {
    u32 maxPackedOffset; /* 0x00 */
    u32 segments; /* 0x04 */
    u32 namedReferences; /* 0x08 */
    u32 pendingReferences; /* 0x0C */
} DdsPackedObject;

typedef struct DdsCallbackNode {
    u8 pad00[0xC];
    u32 userData; /* 0x0C */
    s32 onFirst; /* 0x10 */
    s32 onLast; /* 0x14 */
} DdsCallbackNode;

typedef struct DdsCallbackCollection {
    u8 pad00[4];
    u32 callbacks[3]; /* 0x04, 0x08, 0x0C */
} DdsCallbackCollection;

/* Allocation block: bytes used so far and its 0x10000-byte buffer
   (func_0031F138 frees both). */
typedef struct DdsAllocBlock {
    u32 used;   /* 0x00 */
    u32 buffer; /* 0x04 */
} DdsAllocBlock;

DdsAllocBlock *dds3AllocateEmptyPackedValueBuffer(void) {
    DdsAllocBlock *block = (DdsAllocBlock *)func_0035A828(8);
    u32 buffer;

    memset(block, 0, 8);
    buffer = func_0035A828(0x10000);
    block->used = 0;
    block->buffer = buffer;
    return block;
}

void func_0031F138(u32 node) {
    func_0035A880(*(u32 *)(node + 4));
    func_0035A880(node);
}


INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F168);

void func_0031F1B8(u32 node) {
    func_0035A880(*(u32 *)node);
    func_0035A880(node);
}


void func_0031F1E8(u32 unused, u32 node) {
    func_0031F138(node);
}


void func_0031F208(u32 unused, u32 node) {
    func_0031F1B8(node);
}


DdsAllocBlock *dds3AllocateAndAttachPackedValueBuffer(s32 object) {
    DdsAllocBlock *record;

    record = dds3AllocateEmptyPackedValueBuffer();
    func_00320CE0(*(u32 *)(object + 4), 0, record);
    return record;
}

u32 func_0031F270(s32 object) {
    return *(u32 *)(*(s32 *)(*(s32 *)(object + 4) + 8) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F280);

void dds3ReleaseCallbackCollectionAndNodes(u32 node) {
    dds3DestroyCallbackNodeAfterLastNotification(((DdsCallbackCollection *)node)->callbacks[0]);
    dds3DestroyCallbackNodeAfterLastNotification(((DdsCallbackCollection *)node)->callbacks[1]);
    dds3DestroyCallbackNodeAfterLastNotification(((DdsCallbackCollection *)node)->callbacks[2]);
    func_0035A880(node);
}


/* Append `bytes` copies of the source data into the object's 0x10000-byte allocation blocks, moving on to a fresh block when one fills up. */
void func_0031F340(DdsPackedObject *object, const void *source, u32 bytes) {
    DdsAllocBlock *block = (DdsAllocBlock *)func_0031F270((s32)object);

    if (bytes != 0) {
        do {
            u32 remaining = 0x10000 - block->used;
            u32 count = remaining < bytes ? remaining : bytes;

            memcpy((void *)(block->buffer + block->used), source, count);
            bytes -= count;
            block->used += count;
            object->maxPackedOffset += count;
            if (block->used > 0xffff) {
                block = dds3AllocateAndAttachPackedValueBuffer((s32)object);
            }
        } while (bytes != 0);
    }
}

void dds3WritePackedValue(destination, datum, size)
    u32 destination;
    u32 datum;
    u32 size;
{
    u32 value[4];
    value[0] = datum;
    func_0031F340((DdsPackedObject *)destination, value, size);
}

extern u32 func_0031F168(void);
extern char D_00438960[];
extern void func_0035C860(char *dst, const char *format, ...);

void func_0031F430(DdsPackedObject *object, const char *name) {
    DdsNamedNode *node = ((DdsNamedList *)object->namedReferences)->first;
    DdsNamedRecord *record;

    while (node != NULL) {
        record = node->record;
        if (strncmp(record->name, name, DDS_NAMED_RECORD_NAME_BYTES) == 0) {
            return;
        }
        node = node->next;
    }
    record = (DdsNamedRecord *)func_0031F168();
    func_0035C860(record->name, D_00438960, name);
    func_00320CE0(object->namedReferences, 0, record);
    record->value = object->maxPackedOffset;
}

void dds3RecordNamedReference(u32 context, const char *name) {
    u32 record = func_0031F168();
    strncpy((char *)((DdsNamedRecord *)record)->name, name, DDS_NAMED_RECORD_NAME_BYTES);
    func_00320CE0(((DdsPackedObject *)context)->pendingReferences, 0, record);
    ((DdsNamedRecord *)record)->value = ((DdsPackedObject *)context)->maxPackedOffset;
    dds3WritePackedValue(context, -1, 4);
}


/* Walk the named-record list; names occupy at most 0x40 bytes. */
u32 dds3FindNamedRecord(u32 context, const char *name) {
    DdsNamedNode *node = ((DdsNamedList *)context)->first;
    while (node) {
        DdsNamedRecord *record = node->record;
        if (strncmp(record->name, name, DDS_NAMED_RECORD_NAME_BYTES) == 0) {
            return (u32)record;
        }
        node = node->next;
    }
    return 0;
}

/* Split a packed segment:offset reference and resolve it to a live address. */
u32 dds3ResolvePackedOffset(u32 *object, u32 packedOffset) {
    u32 segmentIndex = packedOffset >> 16;
    u32 offset = packedOffset - (segmentIndex << 16);
    u32 segment;
    if (packedOffset > ((DdsPackedObject *)object)->maxPackedOffset) {
        return 0;
    }
    segment = mnuFindResourceNodeById(((DdsPackedObject *)object)->segments, segmentIndex);
    return *(u32 *)(*(u32 *)(segment + 0x10) + 4) + offset;
}

/* Replace recorded pointer slots with their matching named addresses. */
s32 dds3ApplyNamedRelocations(u32 *object) {
    DdsNamedNode *node = ((DdsNamedList *)((DdsPackedObject *)object)->pendingReferences)->first;
    if (node != NULL) {
        do {
            DdsNamedRecord *record = node->record;
            u32 found = dds3FindNamedRecord(((DdsPackedObject *)object)->namedReferences, record->name);
            u32 address;
            u32 replacement;
            if (found == 0) {
                return 0;
            }
            address = dds3ResolvePackedOffset(object, record->value);
            replacement = ((DdsNamedRecord *)found)->value;
            node = node->next;
            *(u32 *)address = replacement;
        } while (node != NULL);
    }
    return 1;
}

u32 dds3WritePendingNamedReferenceValues(u32 object) {
    DdsNamedNode *node = ((DdsNamedList *)((DdsPackedObject *)object)->pendingReferences)->first;
    u32 destination;
    if (node == NULL) {
        return 0;
    }
    destination = func_0031F280(object);
    do {
        dds3WritePackedValue(destination, node->record->value, 4);
        node = node->next;
    } while (node != NULL);
    return destination;
}

u32 dds3RegisterPendingNamedReferences(u32 *object, u32 extra) {
    DdsNamedNode *node = ((DdsNamedList *)object[1])->first;
    while (node) {
        DdsNamedRecord *record = node->record;
        func_0035A648(record->value, (u32)record->name, 1, extra);
        node = node->next;
    }
    return object[0];
}

extern s32 func_00359A98(const char *name, const char *path);
extern void func_0035A648(u32 *data, s32 size, s32 flag, s32 handle);
extern void func_003594A8(s32 handle);
extern const char D_00438968[];
extern const char D_00438970[];

s32 func_0031F778(u32 *object, s32 arg1, s32 arg2) {
    char buffer[0x100];
    u32 record[2];
    s32 handle;
    u32 *pending;

    func_0035C860(buffer, D_00438968, arg1, arg2);
    handle = func_00359A98(buffer, D_00438970);
    if (handle == 0) {
        return 0;
    }
    dds3ApplyNamedRelocations(object);
    pending = (u32 *)dds3WritePendingNamedReferenceValues((u32)object);
    record[0] = object[0];
    record[1] = pending[0];
    func_0035A648(record, 8, 1, handle);
    dds3RegisterPendingNamedReferences(object, handle);
    dds3RegisterPendingNamedReferences(pending, handle);
    func_003594A8(handle);
    return 1;
}

void dds3ApplyRelocationOffsets(u8 *base, u32 adjustment, u32 *offsets, u32 size) {
    s32 count = size >> 2;
    while (count > 0) {
        u32 *slot = (u32 *)(base + *offsets++);
        *slot += adjustment;
        count--;
    }
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F878);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031FA60);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320020);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_003201A0);

u32 func_00320380(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320388);

u64 func_00320510(u64 source, u64 request) {
    u64 buffer;
    u64 result;
    u32 metadata[4];

    buffer = func_00320AE8(source, request, metadata);
    result = func_00325790(buffer, metadata[0]);
    func_0035A880(buffer);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320560);

u64 func_003206E8(u64 source, u64 request) {
    u64 buffer;
    u64 result;
    u32 metadata[4];

    buffer = func_00320AE8(source, request, metadata);
    result = func_00325790(buffer, metadata[0]);
    func_0035A880(buffer);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320738);

u64 func_003208C0(u64 source, u64 request) {
    u64 buffer;
    u64 result;
    u32 metadata[4];

    buffer = func_00320AE8(source, request, metadata);
    result = func_00325AB8(buffer, metadata[0]);
    func_0035A880(buffer);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320910);

u64 func_00320A98(u64 source, u64 request) {
    u64 buffer;
    u64 result;
    u32 metadata[4];

    buffer = func_00320AE8(source, request, metadata);
    result = func_00325BB0(buffer, metadata[0]);
    func_0035A880(buffer);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320AE8);

u32 mnuCreateCallbackNode(u32 userData) {
    u32 *node = (u32 *)func_0035A828(0x18);
    memset(node, 0, 0x18);
    ((DdsCallbackNode *)node)->userData = userData;
    ((DdsCallbackNode *)node)->onFirst = (u32)func_003211F0;
    ((DdsCallbackNode *)node)->onLast = (u32)func_003211F0;
    return (u32)node;
}

void dds3DestroyCallbackNodeAfterLastNotification(u32 node) {
    if (node != 0) {
        void (*callback)(s32, u32);
        mnuClearResourceList(node);
        callback = (void (*)(s32, u32))((DdsCallbackNode *)node)->onLast;
        callback(-1, ((DdsCallbackNode *)node)->userData);
        func_0035A880(node);
    }
}


void dds3SetCallbackNodeLastListener(s32 callbackNode, s32 callbackAddress) {
    if (callbackAddress != 0) {
        ((DdsCallbackNode *)callbackNode)->onLast = (s32)callbackAddress;
    }
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320CE0);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320D80);

void dds3SetCallbackNodeFirstListener(s32 callbackNode, s32 callbackAddress) {
    if (callbackAddress != 0) {
        ((DdsCallbackNode *)callbackNode)->onFirst = (s32)callbackAddress;
    }
}

typedef struct SdfListNode {
    u32 index;                  /* 0x00 */
    s32 key;                    /* 0x04 */
    struct SdfListNode *next;   /* 0x08 */
    struct SdfListNode *prev;   /* 0x0C */
    void *value;                /* 0x10 */
} SdfListNode;

typedef struct SdfList {
    u32 count;                     /* 0x00 */
    SdfListNode *head;             /* 0x04 */
    SdfListNode *tail;             /* 0x08 */
    u32 pad0C;
    void (*onRemove)(u32, void *); /* 0x10 */
} SdfList;

SdfListNode *dds3DetachIndexedListNodeAndRenumber(SdfList *list, SdfListNode *node) {
    SdfListNode *it;

    if (node == NULL) {
        return NULL;
    }
    for (it = list->head; it != NULL; it = it->next) {
        if (node->index < it->index) {
            it->index = it->index - 1;
        }
    }
    list->count = list->count - 1;
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node == list->head) {
        list->head = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    if (node->next != NULL) {
        return node->next;
    }
    return node->prev;
}

u32 dds3RemoveListNodeAndNotify(u32 list, u32 node) {
    u32 remaining;
    if (node == 0) {
        return 0;
    }
    remaining = dds3DetachIndexedListNodeAndRenumber(list, node);
    (*(void (**)(u32, u32))(list + 0x10))(*(u32 *)node, *(u32 *)(node + 0x10));
    func_0035A880(node);
    return remaining;
}
INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438960);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438968);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438970);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438978);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438980);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438988);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438990);

INCLUDE_SDATA(const s32, "game/code_0031F0E8", D_00438998);

