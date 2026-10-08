#include "common.h"
#include "evt_viewer.h"
#include "dds3obj.h"
#include "evt_world.h"

extern s32 strcmp(const char *a, const char *b);
extern char *strcpy(char *dst, const char *src);
extern void *dds3GetWorldObject(void);
extern EffWorldNode *dds3FindIndexedObjectChainNodeByName(EffWorldNode *world, s32 type, const u8 *name);
extern void *sdfAllocSizeClassBlock(s32 size);
extern void *memset(void *dst, s32 value, u32 size);

/* Node queued on an entry, linked through +0x30/+0x34, owning a buffer. */


/* Event-viewer entries are linked at +0x7C/+0x80 and keyed by id. */


/* Table of 0x20-byte (group, type) records at +0x34, counted at +0x38. */
typedef struct EvtGroupRec {
    u8 pad00[8];
    s32 group;               /* 0x08 */
    s32 type;                /* 0x0C */
    u8 pad10[0x10];
} EvtGroupRec;



/* The name table and entry list have the same offsets in both games. */


/* Bounds are reset from the currently observed value. */
typedef struct EvtRange {
    u8 pad00[0x10];
    s32 min;
    s32 max;
    s32 value;
} EvtRange;

void evtUnlinkListNode(EvtRuntimeGroup *entry, EvtRuntimeChild *node);
void sdfReleaseChipBlock(void *ptr);
void sdfTexReleaseReference(struct SdfTex *tex);
s32 sdfCheckPendingWorkWithInterrupts();
void effInitCh71Id(void);
void effInitCh72Id(void);
void effInitCh76Id(void);
void effInitCh75Id(void);
void evtPolygonMovieFreeWork(PolyMovieWork *table);
void btlRemoveCurrentGroupedEntity(s32 group, s32 type);
void kwlnPadResetMotorLevelsAndOutput(void);
EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *viewer);
void func_00246878(EvtRuntime *viewer);
void evtEventViewerFreeSlot(s32 index, EvtRuntime *viewer);
void evtEventViewerFreeBuffer(EvtRuntimeChild *node);
void dds3RemoveWorldObjectNode(struct EffWorldNode *ptr);

void func_002467A0(void) {
    func_00246108();
}

/* Return the pending node at position (count A + count B) in the queue entry, or NULL. */
EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *viewer) {
    EvtRuntimeGroup *queue;
    EvtRuntimeChild *node;
    s32 i;

    queue = viewer->frameGroup;
    if (queue == NULL) {
        return NULL;
    }
    node = queue->children;
    for (i = 0; i < viewer->frameCursor + viewer->frameFirst && node != NULL; i++) {
        node = node->next;
    }
    return node;
}

/* Release a queued node from its entry, freeing its buffer unless the entry id is 0x12. */
void evtEventViewerReleaseNode(EvtRuntimeGroup *entry, EvtRuntimeChild *node) {
    evtUnlinkListNode(entry, node);
    if (node->payload != NULL) {
        if (entry->type != 0x12) {
            sdfReleaseChipBlock(node->payload);
        }
        node->payload = NULL;
    }
    sdfReleaseChipBlock(node);
}

/* Release the pending node and consume its position in the viewer queue. */
void func_00246878(EvtRuntime *viewer) {
    EvtRuntimeGroup *entry;
    EvtRuntimeChild *node;

    entry = viewer->frameGroup;
    node = evtEventViewerGetPendingNode(viewer);
    switch (entry->type) {
    case 3:
    case 20:
    case 21:
    case 26:
        if (node->p08.sb[0] >= 0) {
            evtEventViewerFreeSlot(node->p08.sb[0], viewer);
        }
        break;
    case 18:
        evtEventViewerFreeBuffer(node);
        break;
    }
    evtEventViewerReleaseNode(entry, node);
    if (viewer->frameCursor == 0) {
        if (viewer->frameFirst > 0) {
            viewer->frameFirst--;
        }
    } else {
        if (viewer->frameFirst > 0) {
            viewer->frameFirst--;
        } else {
            viewer->frameCursor--;
        }
    }
}

/* Release a supplied queued node, with the same resource and counter handling. */
void func_00246950(EvtRuntime *viewer, EvtRuntimeGroup *entry, EvtRuntimeChild *node) {
    switch (entry->type) {
    case 3:
    case 20:
    case 21:
    case 26:
        if (node->p08.sb[0] >= 0) {
            evtEventViewerFreeSlot(node->p08.sb[0], viewer);
        }
        break;
    case 18:
        evtEventViewerFreeBuffer(node);
        break;
    }
    evtEventViewerReleaseNode(entry, node);
    if (viewer->frameCursor == 0) {
        if (viewer->frameFirst > 0) {
            viewer->frameFirst--;
        }
    } else {
        if (viewer->frameFirst > 0) {
            viewer->frameFirst--;
        } else {
            viewer->frameCursor--;
        }
    }
}

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(EvtRuntime *viewer) {
    while (evtEventViewerGetPendingNode(viewer) != 0) {
        func_00246878(viewer);
    }
}

/* Insert an entry into the viewer's list, ordered by id. */
void evtEventViewerInsertEntry(EvtRuntimeGroup *entry, EvtRuntime *viewer) {
    EvtRuntimeGroup *node;

    node = viewer->groups;
    if (node == NULL) {
        viewer->groups = entry;
        viewer->lastGroup = entry;
        entry->next = NULL;
        entry->prev = NULL;
    } else {
        while (node != NULL) {
            if (entry->type < node->type) {
                if (node->prev == NULL) {
                    viewer->groups = entry;
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
            viewer->lastGroup->next = entry;
            entry->prev = viewer->lastGroup;
            entry->next = NULL;
            viewer->lastGroup = entry;
        }
    }
    viewer->entryCount++;
}

void evtEventViewerUnlinkEntry(EvtRuntimeGroup *entry, EvtRuntime *viewer) {
    if (entry->prev == NULL) {
        viewer->groups = entry->next;
    } else {
        entry->prev->next = entry->next;
    }
    if (entry->next == NULL) {
        viewer->lastGroup = entry->prev;
    } else {
        entry->next->prev = entry->prev;
    }
    entry->prev = NULL;
    entry->next = NULL;
    viewer->entryCount--;
}

/* Allocate a zeroed 0x84-byte entry for an id and queue it in the viewer. */
EvtRuntimeGroup *evtEventViewerCreateEntry(s32 id, EvtRuntime *viewer) {
    EvtRuntimeGroup *entry;

    entry = sdfAllocSizeClassBlock(0x84);
    if (entry == NULL) {
        return NULL;
    }
    memset(entry, 0, 0x84);
    entry->type = id;
    entry->entryHeader.word = -1;
    entry->argument0C = -1;
    evtEventViewerInsertEntry(entry, viewer);
    return entry;
}

s32 evtEventViewerCountEntriesById(s32 id, EvtRuntime *viewer) {
    EvtRuntimeGroup *node;
    s32 currentId;
    s32 count;

    node = viewer->groups;
    count = 0;
    while (node != NULL) {
        currentId = node->type;
        node = node->next;
        if (currentId == id) {
            count++;
        }
    }
    return count;
}

s32 evtEventViewerCountEntries(EvtRuntime *viewer) {
    EvtRuntimeGroup *node;
    s32 count;

    count = 0;
    for (node = viewer->groups; node != NULL; node = node->next) {
        count++;
    }
    return count;
}

/* Sum nodeCount over entries: mode 2 skips ids 5/0x13, mode 3 takes only those. */
s32 evtEventViewerSumNodeCounts(s32 mode, EvtRuntime *viewer) {
    EvtRuntimeGroup *entry;
    s32 total;

    total = 0;
    for (entry = viewer->groups; entry != NULL; entry = entry->next) {
        if (mode == 2) {
            if (entry->type == 5 || entry->type == 0x13) {
                continue;
            }
        } else if (mode == 3) {
            if (entry->type != 5 && entry->type != 0x13) {
                continue;
            }
        } else {
            continue;
        }
        total += entry->childCount;
    }
    return total;
}

/* Destroy an entry: free its nodes, drop a type-0x18 texture, unlink it, free it. */
void evtEventViewerDestroyEntry(EvtRuntimeGroup *entry, EvtRuntime *viewer) {
    while (entry->children != NULL) {
        evtEventViewerReleaseNode(entry, entry->children);
    }
    if (entry->type == 0x18) {
        sdfTexReleaseReference(entry->texture);
        entry->texture = 0;
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
void evtEventViewerReset(EvtRuntime *viewer) {
    SdfMemBlock *keepA;
    s32 keepB;

    keepA = viewer->resourceHandle;
    keepB = viewer->glyph;
    memset(viewer, 0, 0x24BC);
    viewer->resourceHandle = keepA;
    viewer->glyph = keepB;
    viewer->flags |= 1;
    viewer->headerThird = 0x21C;
    viewer->frameRange.word = 0x21B;
    viewer->unk2238 = 1;
}

/* Shut the viewer down: reset effect channels, destroy entries, release the handle. */
void evtEventViewerShutdown(EvtRuntime *viewer) {
    effInitCh71Id();
    effInitCh72Id();
    effInitCh76Id();
    effInitCh75Id();
    while (viewer->groups != NULL) {
        evtEventViewerDestroyEntry(viewer->groups, viewer);
    }
    if (viewer->windowContext != 0) {
        evtPolygonMovieFreeWork(viewer->windowContext);
        viewer->windowContext = 0;
    }
    kwlnPadResetMotorLevelsAndOutput();
}

/* Destroy every grouped entity listed in the viewer's table. */
void evtEventViewerReleaseGroups(EvtRuntime *viewer) {
    s32 i;

    if (viewer->windowContext != NULL) {
        for (i = 0; i < (s32)viewer->windowContext->unk_38; i++) {
            btlRemoveCurrentGroupedEntity(((EvtGroupRec *)viewer->windowContext->mainEntry3Data)[i].group, ((EvtGroupRec *)viewer->windowContext->mainEntry3Data)[i].type);
        }
    }
}

/* Search the fixed-width (0x20-byte) event-name records. */
s32 evtEventViewerFindNameIndex(const char *name, EvtRuntime *viewer) {
    const char *nameEntry;
    s32 index;

    index = 0;
    if (0 < viewer->entryTotal) {
        nameEntry = viewer->entryName[0];
        do {
            if (strcmp(name, nameEntry) == 0) {
                return index;
            }
            index = index + 1;
            nameEntry = nameEntry + 0x20;
        } while (index < viewer->entryTotal);
    }
    return -1;
}

/* Return the index of a name in the table, appending it when missing. */
s32 evtEventViewerAddName(const char *name, EvtRuntime *viewer) {
    s32 found;
    s32 index;

    found = evtEventViewerFindNameIndex(name, viewer);
    if (found >= 0) {
        return found;
    }
    index = viewer->entryTotal;
    strcpy(viewer->entryName[index], name);
    viewer->entryTotal++;
    return index;
}

struct EffectObj;

/* Look up a named kind-7 effect object by index. */
struct EffectObj *evtEventViewerGetNameObject(s32 index, EvtRuntime *viewer) {
    if (index < 0) {
        return NULL;
    }
    return (struct EffectObj *)dds3FindIndexedObjectChainNodeByName(
        (EffWorldNode *)dds3GetWorldObject(), 7, (const u8 *)viewer->entryName[index]);
}

struct PolyMovieObject;
extern EffWorldNode *dds3FindObjectChainNodeByName(EffWorldNode *, const u8 *);
extern s32 effObjBindOwnerBillEntry(struct EffectObj *, struct EffectObj *, s32);
extern s32 effObjBindValidatedOwner(struct EffectObj *, struct EffectObj *);
extern s32 evtStageRelinkOwnedNodeResource(void *, void *);
extern s32 evtPolygonMovieScaleByProgress(struct PolyMovieObject *, s32, s32, s32);

/* Billboard entries and polygon movies use distinct owner attachment paths. */
void evtViewerBindNamedOwner(s32 obj, s32 value, s32 type, u32 word, EvtRuntime *viewer) {
    ObjData *owner;
    struct PolyMovieObject *movie;
    s32 frame;

    if (obj == 0) {
        return;
    }
    frame = viewer->curFrame;
    if (value < 0) {
        return;
    }
    owner = (ObjData *)dds3FindObjectChainNodeByName(
        (EffWorldNode *)dds3GetWorldObject(),
        (const u8 *)viewer->entryName[value]);
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
extern struct EffectObj *func_00115AA8(s32 kind, struct EffNodeDescriptor *descriptor);
extern void effObjDispatchReadyState(s32 obj);
extern s32 effObjCopyMagatuhiSourceParameters(struct EffectObj *obj, struct EffectObj *first,
                                               struct EffectObj *second, struct EffectObj *third,
                                               struct EffectObj *fourth);

/* Create the viewer object for a command in the first free slot; returns the slot, or -1 when full. */
s32 evtViewerCreateObjectInFreeSlot(s32 unused, EvtViewCmd *cmd, EvtViewParams *params, EvtRuntime *viewer) {
    f32 vec0[4];
    f32 vec1[4];
    s32 handle = 0;
    s32 slot;
    struct EffectObj *n0;
    struct EffectObj *n1;
    struct EffectObj *n2;

    memset(vec0, 0, 0x10);
    memset(vec1, 0, 0x10);
    vec1[3] = 1.0f;
    for (slot = 0; slot < 0x7F; slot++) {
        if (viewer->objects[slot] == NULL) {
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
            handle = (s32)func_00115AA8(1, (struct EffNodeDescriptor *)cmd->arg);
            break;
        case 0x15:
            handle = (s32)func_00115AA8(2, (struct EffNodeDescriptor *)cmd->arg);
            break;
        }
        n0 = evtEventViewerGetNameObject(params->u.names[0], viewer);
        n1 = evtEventViewerGetNameObject(params->u.names[1], viewer);
        n2 = evtEventViewerGetNameObject(params->u.names[2], viewer);
        effObjCopyMagatuhiSourceParameters((struct EffectObj *)handle, n0, n1, n2,
                                           evtEventViewerGetNameObject(params->u.names[3], viewer));
        if (params->unk9 != 0) {
            effObjDispatchReadyState(handle);
        }
        break;
    }
    viewer->objects[slot] = (void *)handle;
    if (handle != 0) {
        effObjSetFlags(handle, 1);
    }
    return slot;
}

void evtEventViewerFreeSlot(s32 index, EvtRuntime *viewer) {
    s32 resource;
    s32 *slot;

    slot = (s32 *)(index * 4 + (s32)viewer + 0x203c);
    resource = *slot;
    if (resource != 0) {
        dds3RemoveWorldObjectNode((struct EffWorldNode *)resource);
        *slot = 0;
    }
}

/* Create an event-viewer effect at the origin and attach its active event node. */
void *func_00247400(void *resource, u32 entryId, s32 value, s32 type, u32 word,
                    EvtRuntime *viewer) {
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

void evtEventViewerFreeBuffer(EvtRuntimeChild *node) {
    if (node->payload != NULL) {
        dds3RemoveWorldObjectNode(node->payload);
    }
    node->payload = NULL;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004224A8);

