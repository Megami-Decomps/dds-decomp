#include "common.h"

extern u64 func_00325BB0(u64, u32);

extern u64 func_00325AB8(u64, u32);

extern u64 func_00320AE8(u64, u64, u32 *);

extern u64 func_00325790(u64, u32);

extern void (*D_004389C4)(void);

extern void func_00320C88(u32);
extern u32 func_0035A828(s32 bytes);
extern void func_003211F0(void);

#define DDS_NAMED_RECORD_NAME_BYTES 0x40

typedef struct DdsNamedRecord {
    const char *name; /* 0x00: up to 0x40 bytes */
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

u64 func_0031F0E8(void) {
    DdsAllocBlock *block = (DdsAllocBlock *)func_0035A828(8);
    u32 buffer;

    memset(block, 0, 8);
    buffer = func_0035A828(0x10000);
    block->used = 0;
    block->buffer = buffer;
    return (u64)block;
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


u64 func_0031F228(s32 object) {
    u64 record;

    record = func_0031F0E8();
    func_00320CE0(*(u32 *)(object + 4), 0, record);
    return record;
}

u32 func_0031F270(s32 object) {
    return *(u32 *)(*(s32 *)(*(s32 *)(object + 4) + 8) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F280);

void func_0031F300(u32 node) {
    func_00320C88(((DdsCallbackCollection *)node)->callbacks[0]);
    func_00320C88(((DdsCallbackCollection *)node)->callbacks[1]);
    func_00320C88(((DdsCallbackCollection *)node)->callbacks[2]);
    func_0035A880(node);
}


INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F340);

void dds3WritePackedValue(destination, datum, size)
    u32 destination;
    u32 datum;
    u32 size;
{
    u32 value[4];
    value[0] = datum;
    func_0031F340((u32 *)destination, value, size);
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F430);

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

u32 func_0031F6A0(u32 object) {
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

u32 func_0031F708(u32 *object, u32 extra) {
    DdsNamedNode *node = ((DdsNamedList *)object[1])->first;
    while (node) {
        DdsNamedRecord *record = node->record;
        func_0035A648(record->value, (u32)record->name, 1, extra);
        node = node->next;
    }
    return object[0];
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_0031F778);

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

void func_00320C88(u32 node) {
    if (node != 0) {
        void (*callback)(s32, u32);
        func_00321018(node);
        callback = (void (*)(s32, u32))((DdsCallbackNode *)node)->onLast;
        callback(-1, ((DdsCallbackNode *)node)->userData);
        func_0035A880(node);
    }
}


void func_00320CD0(s32 callbackNode, s32 callbackAddress) {
    if (callbackAddress != 0) {
        ((DdsCallbackNode *)callbackNode)->onLast = (s32)callbackAddress;
    }
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320CE0);

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320D80);

void func_00320EA8(s32 callbackNode, s32 callbackAddress) {
    if (callbackAddress != 0) {
        ((DdsCallbackNode *)callbackNode)->onFirst = (s32)callbackAddress;
    }
}

INCLUDE_ASM(const s32, "game/code_0031F0E8", func_00320EB8);

u32 func_00320F68(u32 list, u32 node) {
    u32 remaining;
    if (node == 0) {
        return 0;
    }
    remaining = func_00320EB8(list, node);
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

