#include "common.h"
#include "dds3obj.h"
#include "evt_world.h"

extern s32 strcmp(const char *a, const char *b);
extern char *strcpy(char *dst, const char *src);
extern s32 dds3GetWorldObject(void);
extern s32 dds3FindIndexedObjectChainNodeByName(s32 world, s32 type, const char *name);
extern void *sdfAllocSizeClassBlock(s32 size);
extern void *memset(void *dst, s32 value, u32 size);

/* Node queued on an entry, linked through +0x30/+0x34, owning a buffer. */
typedef struct EvtEvNode {
    u8 pad00[8];
    s8 slot;                  /* 0x08: signed world-object slot, negative when unused */
    u8 pad09[0x23];
    void *buf;                 /* 0x2C */
    struct EvtEvNode *next;    /* 0x30 */
    struct EvtEvNode *prev;    /* 0x34 */
} EvtEvNode;

/* Event-viewer entries are linked at +0x7C/+0x80 and keyed by id. */
typedef struct EvtEvEntry {
    s32 id;                  /* 0x00 */
    u8 pad4[4];
    s32 unk8;                /* 0x08 */
    s32 unkC;                /* 0x0C */
    u8 pad10[0x14];
    s32 tex;                 /* 0x24 */
    u8 pad28[0x28];
    s32 nodeCount;           /* 0x50 */
    EvtEvNode *firstNode;    /* 0x54 */
    EvtEvNode *lastNode;     /* 0x58 */
    u8 pad5C[0x20];
    struct EvtEvEntry *next; /* 0x7C */
    struct EvtEvEntry *prev; /* 0x80 */
} EvtEvEntry;

/* Table of 0x20-byte (group, type) records at +0x34, counted at +0x38. */
typedef struct EvtGroupRec {
    u8 pad00[8];
    s32 group;               /* 0x08 */
    s32 type;                /* 0x0C */
    u8 pad10[0x10];
} EvtGroupRec;

typedef struct EvtGroupTable {
    u8 pad00[0x34];
    EvtGroupRec *recs;       /* 0x34 */
    s32 count;               /* 0x38 */
} EvtGroupTable;

/* The name table and entry list have the same offsets in both games. */
typedef struct EvtViewer {
    s32 unk00;           /* 0x00 */
    u32 flags;           /* 0x04 */
    EvtGroupTable *groupTable; /* 0x08 */
    s32 unk0C;           /* 0x0C */
    u8 pad10[4];
    s32 unk14;           /* 0x14 */
    s32 frame;            /* 0x18: end frame for movie preview */
    u8 pad1C[4];
    s32 nameCount;       /* 0x20 */
    char names[256][32]; /* 0x24 */
    u8 pad2024[0xC];
    s32 entryCount;      /* 0x2030 */
    EvtEvEntry *head;    /* 0x2034 */
    EvtEvEntry *tail;    /* 0x2038 */
    void *slots[0x7F];   /* 0x203C */
    s32 unk2238;         /* 0x2238 */
    u8 pad223C[0xC4];
    s32 queuedA;         /* 0x2300 */
    s32 queuedB;         /* 0x2304 */
    EvtEvEntry *queue;   /* 0x2308 */
    u8 pad230C[0x104];
    s32 unk2410;         /* 0x2410 */
    u8 pad2414[0xA8];
} EvtViewer;

/* Bounds are reset from the currently observed value. */
typedef struct EvtRange {
    u8 pad00[0x10];
    s32 min;
    s32 max;
    s32 value;
} EvtRange;

void evtUnlinkListNode(EvtEvEntry *entry, EvtEvNode *node);
void sdfReleaseChipBlock(void *ptr);
void sdfTexReleaseReference(s32 tex);
s32 sdfCheckPendingWorkWithInterrupts();
void effInitCh71Id(void);
void effInitCh72Id(void);
void effInitCh76Id(void);
void effInitCh75Id(void);
void evtPolygonMovieFreeWork(EvtGroupTable *table);
void btlRemoveCurrentGroupedEntity(s32 group, s32 type);
void kwlnPadResetMotorLevelsAndOutput(void);
EvtEvNode *evtEventViewerGetPendingNode(EvtViewer *viewer);
void func_00246878(EvtViewer *viewer);
void evtEventViewerFreeSlot(s32 index, s32 viewerAddress);
void evtEventViewerFreeBuffer(EvtEvNode *node);
struct WorldListNode;
void dds3RemoveWorldObjectNode(struct WorldListNode *ptr);

void func_002467A0(void) {
    func_00246108();
}

/* Return the pending node at position (count A + count B) in the queue entry, or NULL. */
EvtEvNode *evtEventViewerGetPendingNode(EvtViewer *viewer) {
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
void evtEventViewerReleaseNode(EvtEvEntry *entry, EvtEvNode *node) {
    evtUnlinkListNode(entry, node);
    if (node->buf != NULL) {
        if (entry->id != 0x12) {
            sdfReleaseChipBlock(node->buf);
        }
        node->buf = NULL;
    }
    sdfReleaseChipBlock(node);
}

/* Release the pending node and consume its position in the viewer queue. */
void func_00246878(EvtViewer *viewer) {
    EvtEvEntry *entry;
    EvtEvNode *node;

    entry = viewer->queue;
    node = evtEventViewerGetPendingNode(viewer);
    switch (entry->id) {
    case 3:
    case 20:
    case 21:
    case 26:
        if (node->slot >= 0) {
            evtEventViewerFreeSlot(node->slot, (s32)viewer);
        }
        break;
    case 18:
        evtEventViewerFreeBuffer(node);
        break;
    }
    evtEventViewerReleaseNode(entry, node);
    if (viewer->queuedB == 0) {
        if (viewer->queuedA > 0) {
            viewer->queuedA--;
        }
    } else {
        if (viewer->queuedA > 0) {
            viewer->queuedA--;
        } else {
            viewer->queuedB--;
        }
    }
}

/* Release a supplied queued node, with the same resource and counter handling. */
void func_00246950(EvtViewer *viewer, EvtEvEntry *entry, EvtEvNode *node) {
    switch (entry->id) {
    case 3:
    case 20:
    case 21:
    case 26:
        if (node->slot >= 0) {
            evtEventViewerFreeSlot(node->slot, (s32)viewer);
        }
        break;
    case 18:
        evtEventViewerFreeBuffer(node);
        break;
    }
    evtEventViewerReleaseNode(entry, node);
    if (viewer->queuedB == 0) {
        if (viewer->queuedA > 0) {
            viewer->queuedA--;
        }
    } else {
        if (viewer->queuedA > 0) {
            viewer->queuedA--;
        } else {
            viewer->queuedB--;
        }
    }
}

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(EvtViewer *viewer) {
    while (evtEventViewerGetPendingNode(viewer) != 0) {
        func_00246878(viewer);
    }
}

/* Insert an entry into the viewer's list, ordered by id. */
void evtEventViewerInsertEntry(EvtEvEntry *entry, EvtViewer *viewer) {
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

void evtEventViewerUnlinkEntry(EvtEvEntry *entry, EvtViewer *viewer) {
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
EvtEvEntry *evtEventViewerCreateEntry(s32 id, EvtViewer *viewer) {
    EvtEvEntry *entry;

    entry = sdfAllocSizeClassBlock(0x84);
    if (entry == NULL) {
        return NULL;
    }
    memset(entry, 0, 0x84);
    entry->id = id;
    entry->unk8 = -1;
    entry->unkC = -1;
    evtEventViewerInsertEntry(entry, viewer);
    return entry;
}

s32 evtEventViewerCountEntriesById(s32 id, EvtViewer *viewer) {
    EvtEvEntry *node;
    s32 currentId;
    s32 count;

    node = viewer->head;
    count = 0;
    while (node != NULL) {
        currentId = node->id;
        node = node->next;
        if (currentId == id) {
            count++;
        }
    }
    return count;
}

s32 evtEventViewerCountEntries(EvtViewer *viewer) {
    EvtEvEntry *node;
    s32 count;

    count = 0;
    for (node = viewer->head; node != NULL; node = node->next) {
        count++;
    }
    return count;
}

/* Sum nodeCount over entries: mode 2 skips ids 5/0x13, mode 3 takes only those. */
s32 evtEventViewerSumNodeCounts(s32 mode, EvtViewer *viewer) {
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
void evtEventViewerDestroyEntry(EvtEvEntry *entry, EvtViewer *viewer) {
    while (entry->firstNode != NULL) {
        evtEventViewerReleaseNode(entry, entry->firstNode);
    }
    if (entry->id == 0x18) {
        sdfTexReleaseReference(entry->tex);
        entry->tex = 0;
        while (sdfCheckPendingWorkWithInterrupts() != 0) {
        }
    }
    evtEventViewerUnlinkEntry(entry, viewer);
    sdfReleaseChipBlock(entry);
}

void evtViewerSetMinimumFromCurrent(EvtRange *range) {
    s32 current;

    current = range->value;
    range->min = current;
    if (range->max < current) {
        range->max = current;
    }
}

void evtViewerSetMaximumFromCurrent(EvtRange *range) {
    s32 current;

    current = range->value;
    range->max = current;
    if (current < range->min) {
        range->min = current;
    }
}

/* Reset the viewer (0x24BC bytes) but keep its first and 0x2410 fields. */
void evtEventViewerReset(EvtViewer *viewer) {
    s32 keepA;
    s32 keepB;

    keepA = viewer->unk00;
    keepB = viewer->unk2410;
    memset(viewer, 0, 0x24BC);
    viewer->unk00 = keepA;
    viewer->unk2410 = keepB;
    viewer->flags |= 1;
    viewer->unk0C = 0x21C;
    viewer->unk14 = 0x21B;
    viewer->unk2238 = 1;
}

/* Shut the viewer down: reset effect channels, destroy entries, release the handle. */
void evtEventViewerShutdown(EvtViewer *viewer) {
    effInitCh71Id();
    effInitCh72Id();
    effInitCh76Id();
    effInitCh75Id();
    while (viewer->head != NULL) {
        evtEventViewerDestroyEntry(viewer->head, viewer);
    }
    if (viewer->groupTable != 0) {
        evtPolygonMovieFreeWork(viewer->groupTable);
        viewer->groupTable = 0;
    }
    kwlnPadResetMotorLevelsAndOutput();
}

/* Destroy every grouped entity listed in the viewer's table. */
void evtEventViewerReleaseGroups(EvtViewer *viewer) {
    s32 i;

    if (viewer->groupTable != NULL) {
        for (i = 0; i < viewer->groupTable->count; i++) {
            btlRemoveCurrentGroupedEntity(viewer->groupTable->recs[i].group, viewer->groupTable->recs[i].type);
        }
    }
}

/* Search the fixed-width (0x20-byte) event-name records. */
s32 evtEventViewerFindNameIndex(const char *name, EvtViewer *viewer) {
    const char *nameEntry;
    s32 index;

    index = 0;
    if (0 < viewer->nameCount) {
        nameEntry = viewer->names[0];
        do {
            if (strcmp(name, nameEntry) == 0) {
                return index;
            }
            index = index + 1;
            nameEntry = nameEntry + 0x20;
        } while (index < viewer->nameCount);
    }
    return -1;
}

/* Return the index of a name in the table, appending it when missing. */
s32 evtEventViewerAddName(const char *name, EvtViewer *viewer) {
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
s32 evtEventViewerGetNameObject(s32 index, EvtViewer *viewer) {
    if (index < 0) {
        return 0;
    }
    return dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(), 7, viewer->names[index]);
}

struct EffectObj;
struct PolyMovieObject;
extern u32 *dds3FindObjectChainNodeByName(EvtWorldObject *, const u8 *);
extern s32 effObjBindOwnerBillEntry(struct EffectObj *, struct EffectObj *, s32);
extern s32 effObjBindValidatedOwner(struct EffectObj *, struct EffectObj *);
extern s32 evtStageRelinkOwnedNodeResource(void *, void *);
extern s32 evtPolygonMovieScaleByProgress(struct PolyMovieObject *, s32, s32, s32);

/* Billboard entries and polygon movies use distinct owner attachment paths. */
void evtViewerBindNamedOwner(s32 obj, s32 value, s32 type, u32 word, EvtViewer *viewer) {
    ObjData *owner;
    struct PolyMovieObject *movie;
    s32 frame;

    if (obj == 0) {
        return;
    }
    frame = viewer->frame;
    if (value < 0) {
        return;
    }
    owner = (ObjData *)dds3FindObjectChainNodeByName(
        (EvtWorldObject *)dds3GetWorldObject(),
        (const u8 *)viewer->names[value]);
    if (owner == NULL) {
        return;
    }
    switch (owner->kind) {
    case 5:
        if (type >= 0) {
            effObjBindOwnerBillEntry((struct EffectObj *)obj,
                                    (struct EffectObj *)owner, type);
            break;
        }
        /* A negative entry selects the normal owner link instead. */
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 17:
        effObjBindValidatedOwner((struct EffectObj *)obj, (struct EffectObj *)owner);
        break;
    case 16:
        evtStageRelinkOwnedNodeResource(owner, (void *)obj);
        movie = dds3GetObjectOwnedHandle(obj)->slots[1];
        if (viewer->flags & 1) {
            evtPolygonMovieScaleByProgress(movie, 0, word, frame);
        } else {
            evtPolygonMovieScaleByProgress(movie, 1, word, frame);
        }
        break;
    }
}

typedef struct EvtViewCmd {
    s32 kind;           /* 0x0 */
    u8 pad04[0x10];
    s32 arg;            /* 0x14 */
    u8 pad18[6];
    s8 plain;           /* 0x1E */
} EvtViewCmd;

typedef struct EvtViewParams {
    u16 word;           /* 0x0 */
    u8 pad02[7];
    s8 unk9;            /* 0x9 */
    u8 pad0A[2];
    union {
        s8 names[4];    /* 0xC */
        struct {
            s8 flag;    /* 0xC */
            s8 type;    /* 0xD */
            s16 value;  /* 0xE */
        } a;
    } u;
} EvtViewParams;

extern void effObjSetFlags(s32 obj, s32 flags);
extern void *func_00115500(void *resource, void *position, void *scale);
extern void effObjReplaceActiveEventNode(void *obj, u32 entryId);
struct EffNodeDescriptor;
extern struct EffectObj *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, s32 secondVectorAddress);
extern s32 func_00115318(s32 arg, f32 *vec0, f32 *vec1);
extern s32 func_00115AA8(s32 mode, s32 arg);
extern void effObjDispatchReadyState(s32 obj);
extern s32 effObjCopyMagatuhiSourceParameters(s32 obj, s32 a, s32 b, s32 c, s32 d);

/* Create the viewer object for a command in the first free slot; returns the slot, or -1 when full. */
s32 evtViewerCreateObjectInFreeSlot(s32 unused, EvtViewCmd *cmd, EvtViewParams *params, EvtViewer *viewer) {
    f32 vec0[4];
    f32 vec1[4];
    s32 handle = 0;
    s32 slot;
    s32 n0;
    s32 n1;
    s32 n2;

    memset(vec0, 0, 0x10);
    memset(vec1, 0, 0x10);
    vec1[3] = 1.0f;
    for (slot = 0; slot < 0x7F; slot++) {
        if (viewer->slots[slot] == NULL) {
            break;
        }
    }
    if (slot == 0x7F) {
        return -1;
    }
    switch (cmd->kind) {
    case 3:
    case 0x1A:
        if (cmd->kind == 3) {
            handle = (s32)effObjSpawnDescriptorBoundEffect((struct EffNodeDescriptor *)cmd->arg, vec0, (s32)vec1);
        } else {
            handle = func_00115318(cmd->arg, vec0, vec1);
        }
        if (params->u.a.flag != 0) {
            effObjDispatchReadyState(handle);
        }
        if (cmd->plain == 0) {
            evtViewerBindNamedOwner(handle, params->u.a.value, params->u.a.type, params->word, viewer);
        } else {
            evtViewerBindNamedOwner(handle, params->u.a.value, params->u.a.type, 0, viewer);
        }
        break;
    case 0x14:
    case 0x15:
        switch (cmd->kind) {
        case 0x14:
            handle = func_00115AA8(1, cmd->arg);
            break;
        case 0x15:
            handle = func_00115AA8(2, cmd->arg);
            break;
        }
        n0 = evtEventViewerGetNameObject(params->u.names[0], viewer);
        n1 = evtEventViewerGetNameObject(params->u.names[1], viewer);
        n2 = evtEventViewerGetNameObject(params->u.names[2], viewer);
        effObjCopyMagatuhiSourceParameters(handle, n0, n1, n2, evtEventViewerGetNameObject(params->u.names[3], viewer));
        if (params->unk9 != 0) {
            effObjDispatchReadyState(handle);
        }
        break;
    }
    viewer->slots[slot] = (void *)handle;
    if (handle != 0) {
        effObjSetFlags(handle, 1);
    }
    return slot;
}

void evtEventViewerFreeSlot(s32 index, s32 viewerAddress) {
    s32 resource;
    s32 *slot;

    slot = (s32 *)(index * 4 + viewerAddress + 0x203c);
    resource = *slot;
    if (resource != 0) {
        dds3RemoveWorldObjectNode((struct WorldListNode *)resource);
        *slot = 0;
    }
}

/* Create an event-viewer effect at the origin and attach its active event node. */
void *func_00247400(void *resource, u32 entryId, s32 value, s32 type, u32 word,
                    EvtViewer *viewer) {
    f32 position[4];
    f32 scale[4];
    void *effect;

    memset(position, 0, 0x10);
    memset(scale, 0, 0x10);
    scale[3] = 1.0f;
    effect = func_00115500(resource, position, scale);
    effObjSetFlags((s32)effect, 1);
    effObjReplaceActiveEventNode(effect, entryId);
    evtViewerBindNamedOwner((s32)effect, value, type, word, viewer);
    return effect;
}

void evtEventViewerFreeBuffer(EvtEvNode *node) {
    if (node->buf != NULL) {
        dds3RemoveWorldObjectNode(node->buf);
    }
    node->buf = NULL;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004224A8);

