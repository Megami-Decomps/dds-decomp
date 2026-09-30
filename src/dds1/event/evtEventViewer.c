#include "common.h"

/* Node queued on an entry, linked through +0x30/+0x34, owning a buffer. */
typedef struct EvtEvNode {
    u8 pad00[0x2C];
    void *buf;                 /* 0x2C */
    struct EvtEvNode *next;    /* 0x30 */
    struct EvtEvNode *prev;    /* 0x34 */
} EvtEvNode;

/* Event-viewer entry: doubly linked through next/prev, keyed by id. */
typedef struct EvtEvEntry {
    s32 id;                    /* 0x0 */
    u8 pad4[4];                /* 0x4 */
    s32 unk8;                  /* 0x8 */
    s32 unkC;                  /* 0xC */
    u8 pad10[0x14];            /* 0x10 */
    s32 tex;                   /* 0x24 */
    u8 pad28[0x28];            /* 0x28 */
    s32 nodeCount;             /* 0x50 */
    EvtEvNode *firstNode;      /* 0x54 */
    EvtEvNode *lastNode;       /* 0x58 */
    u8 pad5C[0x20];            /* 0x5C */
    struct EvtEvEntry *next;   /* 0x7c */
    struct EvtEvEntry *prev;   /* 0x80 */
} EvtEvEntry;

/* Table of 0x20-byte (group, type) records at +0x34, counted at +0x38. */
typedef struct EvtGroupRec {
    u8 pad00[8];
    s32 group;                 /* 0x8 */
    s32 type;                  /* 0xC */
    u8 pad10[0x10];
} EvtGroupRec;

typedef struct EvtGroupTable {
    u8 pad00[0x34];
    EvtGroupRec *recs;         /* 0x34 */
    s32 count;                 /* 0x38 */
} EvtGroupTable;

/* Event viewer: name table at 0x20, entry list at 0x2030. */
typedef struct EvtViewer {
    s32 unk00;           /* 0x0 */
    u32 flags;           /* 0x4 */
    EvtGroupTable *unk08; /* 0x8 */
    s32 unk0C;           /* 0xC */
    u8 pad10[4];
    s32 unk14;           /* 0x14 */
    u8 pad18[8];
    s32 nameCount;       /* 0x20 */
    char names[256][32]; /* 0x24: fixed-width names */
    u8 pad2024[0xC];
    s32 entryCount;      /* 0x2030 */
    EvtEvEntry *head;    /* 0x2034 */
    EvtEvEntry *tail;    /* 0x2038 */
    void *slots[0x7F];   /* 0x203c */
    s32 unk2238;         /* 0x2238 */
    u8 pad223C[0xC4];
    s32 queuedA;         /* 0x2300 */
    s32 queuedB;         /* 0x2304 */
    EvtEvEntry *queue;   /* 0x2308 */
    u8 pad230C[0x104];
    s32 unk2410;         /* 0x2410 */
    u8 pad2414[0x7C];
} EvtViewer;

/* Min/max tracker fed from a live value. */
typedef struct EvtRange {
    u8 unk00[0x10];   /* 0x0 */
    s32 min;          /* 0x10 */
    s32 max;          /* 0x14 */
    s32 value;        /* 0x18 */
} EvtRange;

typedef struct EvtViewBuf {
    u8 unk00[0x2c];   /* 0x0 */
    void *buf;        /* 0x2c */
} EvtViewBuf;

void evtUnlinkListNode(EvtEvEntry *entry, EvtEvNode *node);
void func_002CFF98(void *ptr);
void sdfTexReleaseReference(s32 tex);
s32 sdfCheckPendingWorkWithInterrupts();

void func_0022B7A0(void);
void effInitCh71Id(void);
void effInitCh72Id(void);
void effInitCh76Id(void);
void effInitCh75Id(void);
void func_00234A30(EvtGroupTable *table);
void func_00216A70(s32 group, s32 type);
void func_00104130(void);
EvtEvNode *func_0022BE40(EvtViewer *viewer);
void func_0022BF00(EvtViewer *viewer);
void func_00110928(void *ptr);
void *func_002CFEB8(s32 size);
void *memset(void *dst, s32 value, u32 size);
s32 dds3GetWorldObject(void);
s32 func_00110ED0(s32 world, s32 type, const char *name);
s32 strcmp(const char *a, const char *b);
char *strcpy(char *dst, const char *src);

void func_0022BE28(void)
{
    func_0022B7A0();
}

/* Return the pending node at position (count A + count B) in the queue entry, or NULL. */
EvtEvNode *func_0022BE40(EvtViewer *viewer)
{
    EvtEvEntry *queue;
    EvtEvNode *node;
    s32 i;

    queue = viewer->queue;
    if (queue == NULL) {
        return NULL;
    }
    node = queue->firstNode;
    for (i = 0; i < viewer->queuedB + viewer->queuedA && node != NULL; i++) {
        node = node->next;
    }
    return node;
}

/* Release a queued node from its entry, freeing its buffer unless the entry id is 0x12. */
void func_0022BEA8(EvtEvEntry *entry, EvtEvNode *node)
{
    evtUnlinkListNode(entry, node);
    if (node->buf != NULL) {
        if (entry->id != 0x12) {
            func_002CFF98(node->buf);
        }
        node->buf = NULL;
    }
    func_002CFF98(node);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BF00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BFD8);

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(EvtViewer *viewer)
{
    while (func_0022BE40(viewer) != 0) {
        func_0022BF00(viewer);
    }
}

/* Insert an entry into the viewer's list, ordered by id. */
void func_0022C0E8(EvtEvEntry *entry, EvtViewer *viewer)
{
    EvtEvEntry *node;

    node = viewer->head;
    if (node == NULL) {
        viewer->head = entry;
        viewer->tail = entry;
        entry->next = NULL;
        entry->prev = NULL;
    } else {
        while (node != NULL) {
            if (entry->id < node->id) {
                if (node->prev == NULL) {
                    viewer->head = entry;
                    node->prev = entry;
                    entry->next = node;
                    entry->prev = NULL;
                } else {
                    node->prev->next = entry;
                    entry->prev = node->prev;
                    entry->next = node;
                    node->prev = entry;
                }
                break;
            }
            node = node->next;
        }
        if (node == NULL) {
            viewer->tail->next = entry;
            entry->prev = viewer->tail;
            entry->next = NULL;
            viewer->tail = entry;
        }
    }
    viewer->entryCount++;
}

void evtEventViewerUnlinkEntry(EvtEvEntry *entry, EvtViewer *viewer)
{
    if (entry->prev == NULL) {
        viewer->head = entry->next;
    } else {
        entry->prev->next = entry->next;
    }
    if (entry->next == NULL) {
        viewer->tail = entry->prev;
    } else {
        entry->next->prev = entry->prev;
    }
    entry->prev = NULL;
    entry->next = NULL;
    viewer->entryCount--;
}

/* Allocate a zeroed 0x84-byte entry for an id and queue it in the viewer. */
EvtEvEntry *func_0022C1D8(s32 id, EvtViewer *viewer)
{
    EvtEvEntry *entry;

    entry = func_002CFEB8(0x84);
    if (entry == NULL) {
        return NULL;
    }
    memset(entry, 0, 0x84);
    entry->id = id;
    entry->unk8 = -1;
    entry->unkC = -1;
    func_0022C0E8(entry, viewer);
    return entry;
}

s32 evtEventViewerCountEntriesById(s32 id, EvtViewer *viewer)
{
    EvtEvEntry *entry;
    s32 count;

    count = 0;
    entry = viewer->head;
    while (entry != NULL) {
        if (entry->id == id) {
            count = count + 1;
        }
        entry = entry->next;
    }
    return count;
}

s32 evtEventViewerCountEntries(EvtViewer *viewer)
{
    EvtEvEntry *entry;
    s32 count;

    count = 0;
    entry = viewer->head;
    while (entry != NULL) {
        count = count + 1;
        entry = entry->next;
    }
    return count;
}

/* Sum nodeCount over entries: mode 2 skips ids 5/0x13, mode 3 takes only those. */
s32 func_0022C2C0(s32 mode, EvtViewer *viewer)
{
    EvtEvEntry *entry;
    s32 total;

    total = 0;
    for (entry = viewer->head; entry != NULL; entry = entry->next) {
        if (mode == 2) {
            if (entry->id == 5 || entry->id == 0x13) {
                continue;
            }
        } else if (mode == 3) {
            if (entry->id != 5 && entry->id != 0x13) {
                continue;
            }
        } else {
            continue;
        }
        total += entry->nodeCount;
    }
    return total;
}

/* Destroy an entry: free its nodes, drop a type-0x18 texture, unlink it, free it. */
void func_0022C340(EvtEvEntry *entry, EvtViewer *viewer)
{
    while (entry->firstNode != NULL) {
        func_0022BEA8(entry, entry->firstNode);
    }
    if (entry->id == 0x18) {
        sdfTexReleaseReference(entry->tex);
        entry->tex = 0;
        while (sdfCheckPendingWorkWithInterrupts() != 0) {
        }
    }
    evtEventViewerUnlinkEntry(entry, viewer);
    func_002CFF98(entry);
}

void evtSetRangeMinimumFromCurrent(EvtRange *range)
{
    s32 value;

    value = range->value;
    range->min = value;
    if (range->max < value) {
        range->max = value;
    }
}

void evtSetRangeMaximumFromCurrent(EvtRange *range)
{
    s32 value;

    value = range->value;
    range->max = value;
    if (value < range->min) {
        range->min = value;
    }
}

/* Reset the viewer (0x2490 bytes) but keep its first and 0x2410 fields. */
void func_0022C408(EvtViewer *viewer)
{
    s32 keepA;
    s32 keepB;

    keepA = viewer->unk00;
    keepB = viewer->unk2410;
    memset(viewer, 0, 0x2490);
    viewer->unk00 = keepA;
    viewer->unk2410 = keepB;
    viewer->flags |= 1;
    viewer->unk0C = 0x21C;
    viewer->unk14 = 0x21B;
    viewer->unk2238 = 1;
}

/* Shut the viewer down: reset effect channels, destroy entries, release the handle. */
void func_0022C478(EvtViewer *viewer)
{
    effInitCh71Id();
    effInitCh72Id();
    effInitCh76Id();
    effInitCh75Id();
    while (viewer->head != NULL) {
        func_0022C340(viewer->head, viewer);
    }
    if (viewer->unk08 != 0) {
        func_00234A30(viewer->unk08);
        viewer->unk08 = 0;
    }
    func_00104130();
}

/* Destroy every grouped entity listed in the viewer's table. */
void func_0022C4F0(EvtViewer *viewer)
{
    s32 i;

    if (viewer->unk08 != NULL) {
        for (i = 0; i < viewer->unk08->count; i++) {
            func_00216A70(viewer->unk08->recs[i].group, viewer->unk08->recs[i].type);
        }
    }
}

/* Search the fixed-width (0x20-byte) event-name records. */
s32 evtEventViewerFindNameIndex(const char *name, EvtViewer *viewer)
{
    const char *slot;
    s32 index;

    index = 0;
    if (0 < viewer->nameCount) {
        slot = viewer->names[0];
        do {
            if (strcmp(name, slot) == 0) {
                return index;
            }
            index = index + 1;
            slot = slot + 0x20;
        } while (index < viewer->nameCount);
    }
    return -1;
}

/* Return the index of a name in the table, appending it when missing. */
s32 func_0022C5E8(const char *name, EvtViewer *viewer)
{
    s32 found;
    s32 index;

    found = evtEventViewerFindNameIndex(name, viewer);
    if (found >= 0) {
        return found;
    }
    index = viewer->nameCount;
    strcpy(viewer->names[index], name);
    viewer->nameCount++;
    return index;
}

/* Look up a name record by index in the world object (type 7). */
s32 func_0022C648(s32 index, EvtViewer *viewer)
{
    if (index < 0) {
        return 0;
    }
    return func_00110ED0(dds3GetWorldObject(), 7, viewer->names[index]);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C6B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C7F0);

void evtEventViewerFreeSlot(s32 index, EvtViewer *viewer)
{
    void **slot;

    slot = (void **)(index * 4 + (s32)viewer + 0x203c);
    if (*slot != NULL) {
        func_00110928(*slot);
        *slot = NULL;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022CA88);

void evtEventViewerFreeBuffer(EvtViewBuf *work)
{
    if (work->buf != NULL) {
        func_00110928(work->buf);
    }
    work->buf = NULL;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003ACFE8);

