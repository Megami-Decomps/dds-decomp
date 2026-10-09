#include "common.h"
#include "sdf_chip.h"
#include "evt_viewer.h"
#include "dds3obj.h"
#include "evt_world.h"

/* Node queued on an entry, linked through +0x30/+0x34, owning a buffer. */


/* Event-viewer entry: doubly linked through next/prev, keyed by id. */


/* Table of 0x20-byte (group, type) records at +0x34, counted at +0x38. */
typedef struct EvtGroupRec {
    u8 pad00[8];
    s32 group;                 /* 0x8 */
    s32 type;                  /* 0xC */
    u8 pad10[0x10];
} EvtGroupRec;



/* Event viewer: name table at 0x20, entry list at 0x2030. */




void evtUnlinkListNode(EvtRuntimeGroup *entry, EvtRuntimeChild *node);
void sdfTexReleaseReference(struct SdfTex *tex);
s32 sdfCheckPendingWorkWithInterrupts();

void func_0022B7A0(void);
void effInitCh71Id(void);
void effInitCh72Id(void);
void effInitCh76Id(void);
void effInitCh75Id(void);
void evtPolygonMovieFreeWork(PolyMovieWork *table);
void btlRemoveCurrentGroupedEntity(s32 group, s32 type);
void kwlnPadResetMotorLevelsAndOutput(void);
EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *viewer);
void func_0022BF00(EvtRuntime *viewer);
void evtEventViewerFreeSlot(s32 index, EvtRuntime *viewer);
void evtEventViewerFreeBuffer(EvtRuntimeChild *node);
void *memset(void *dst, s32 value, u32 size);
void *dds3GetWorldObject(void);
s32 strcmp(const char *a, const char *b);
char *strcpy(char *dst, const char *src);

void func_0022BE28(void)
{
    func_0022B7A0();
}

/* Return the pending node at position (count A + count B) in the queue entry, or NULL. */
EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *viewer)
{
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
void evtEventViewerReleaseNode(EvtRuntimeGroup *entry, EvtRuntimeChild *node)
{
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
void func_0022BF00(EvtRuntime *viewer)
{
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
void func_0022BFD8(EvtRuntime *viewer, EvtRuntimeGroup *entry, EvtRuntimeChild *node)
{
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
void evtEventViewerProcessPending(EvtRuntime *viewer)
{
    while (evtEventViewerGetPendingNode(viewer) != 0) {
        func_0022BF00(viewer);
    }
}

/* Insert an entry into the viewer's list, ordered by id. */
void evtEventViewerInsertEntry(EvtRuntimeGroup *entry, EvtRuntime *viewer)
{
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

void evtEventViewerUnlinkEntry(EvtRuntimeGroup *entry, EvtRuntime *viewer)
{
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
EvtRuntimeGroup *evtEventViewerCreateEntry(s32 id, EvtRuntime *viewer)
{
    EvtRuntimeGroup *entry;

    entry = sdfAllocSizeClassBlock(0x84);
    if (entry == NULL) {
        return NULL;
    }
    memset(entry, 0, 0x84);
    entry->type = id;
    entry->entryHeader = -1;
    entry->argument0C = -1;
    evtEventViewerInsertEntry(entry, viewer);
    return entry;
}

s32 evtEventViewerCountEntriesById(s32 id, EvtRuntime *viewer)
{
    EvtRuntimeGroup *entry;
    s32 count;

    count = 0;
    entry = viewer->groups;
    while (entry != NULL) {
        if (entry->type == id) {
            count = count + 1;
        }
        entry = entry->next;
    }
    return count;
}

s32 evtEventViewerCountEntries(EvtRuntime *viewer)
{
    EvtRuntimeGroup *entry;
    s32 count;

    count = 0;
    entry = viewer->groups;
    while (entry != NULL) {
        count = count + 1;
        entry = entry->next;
    }
    return count;
}

/* Sum nodeCount over entries: mode 2 skips ids 5/0x13, mode 3 takes only those. */
s32 evtEventViewerSumNodeCounts(s32 mode, EvtRuntime *viewer)
{
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
void evtEventViewerDestroyEntry(EvtRuntimeGroup *entry, EvtRuntime *viewer)
{
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

void evtViewerSetMinimumFromCurrent(EvtRuntime *range)
{
    s32 value;

    value = range->curFrame;
    range->headerFirst = value;
    if (range->frameRange.word < value) {
        range->frameRange.word = value;
    }
}

void evtViewerSetMaximumFromCurrent(EvtRuntime *range)
{
    s32 value;

    value = range->curFrame;
    range->frameRange.word = value;
    if (value < range->headerFirst) {
        range->headerFirst = value;
    }
}

/* Reset the viewer (0x2490 bytes) but keep its first and 0x2410 fields. */
void evtEventViewerReset(EvtRuntime *viewer)
{
    SdfMemBlock *keepA;
    s32 keepB;

    keepA = viewer->resourceHandle;
    keepB = viewer->glyph;
    memset(viewer, 0, 0x2490);
    viewer->resourceHandle = keepA;
    viewer->glyph = keepB;
    viewer->flags |= 1;
    viewer->headerThird = 0x21C;
    viewer->frameRange.word = 0x21B;
    viewer->unk2238 = 1;
}

/* Shut the viewer down: reset effect channels, destroy entries, release the handle. */
void evtEventViewerShutdown(EvtRuntime *viewer)
{
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
void evtEventViewerReleaseGroups(EvtRuntime *viewer)
{
    s32 i;

    if (viewer->windowContext != NULL) {
        for (i = 0; i < (s32)viewer->windowContext->unk_38; i++) {
            btlRemoveCurrentGroupedEntity(((EvtGroupRec *)viewer->windowContext->mainEntry3Data)[i].group, ((EvtGroupRec *)viewer->windowContext->mainEntry3Data)[i].type);
        }
    }
}

/* Search the fixed-width (0x20-byte) event-name records. */
s32 evtEventViewerFindNameIndex(const char *name, EvtRuntime *viewer)
{
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
s32 evtEventViewerAddName(const char *name, EvtRuntime *viewer)
{
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
struct EffectObj *evtEventViewerGetNameObject(s32 index, EvtRuntime *viewer)
{
    if (index < 0) {
        return NULL;
    }
    return (struct EffectObj *)dds3FindIndexedObjectChainNodeByName(
        (EffWorldNode *)dds3GetWorldObject(),
        EFF_WORLD_KIND_EFFECT_OBJECT,
        (const u8 *)viewer->entryName[index]);
}

struct PolyMovieObject;
extern s32 effObjBindOwnerBillEntry(struct EffectObj *, struct EffectObj *, s32);
extern s32 effObjBindValidatedOwner(struct EffectObj *, struct EffectObj *);
extern s32 evtStageRelinkOwnedNodeResource(void *, void *);
extern s32 evtPolygonMovieScaleByProgress(struct PolyMovieObject *, s32, s32, s32);

/* Billboard entries and polygon movies use distinct owner attachment paths. */
void evtViewerBindNamedOwner(EffWorldNode *obj, s32 value, s32 type, u32 word, EvtRuntime *viewer) {
    EffWorldNode *owner;
    struct PolyMovieObject *movie;
    s32 frame;

    if (obj == 0) {
        return;
    }
    frame = viewer->curFrame;
    if (value < 0) {
        return;
    }
    owner = dds3FindObjectChainNodeByName(
        (EffWorldNode *)dds3GetWorldObject(),
        (const u8 *)viewer->entryName[value]);
    if (owner == NULL) {
        return;
    }
    switch (owner->kindTag >> 24) {
    case EFF_WORLD_KIND_FOLLOW_MODEL:
        if (type >= 0) {
            effObjBindOwnerBillEntry((struct EffectObj *)obj,
                                    (struct EffectObj *)owner, type);
            break;
        }
        /* A negative entry selects the normal owner link instead. */
    case EFF_WORLD_KIND_CAMERA:
    case EFF_WORLD_KIND_EFFECT_TRANSFORM:
    case EFF_WORLD_KIND_EFFECT_OBJECT:
    case EFF_WORLD_KIND_RESOURCE_OWNER:
    case EFF_WORLD_KIND_LIGHT:
    case EFF_WORLD_KIND_TRANSFORM_SOURCE:
        effObjBindValidatedOwner((struct EffectObj *)obj, (struct EffectObj *)owner);
        break;
    case EFF_WORLD_KIND_ACTION_10:
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


extern void effObjSetFlags(s32 obj, s32 flags);
extern void *func_00115298(void *resource, void *position, void *scale);
extern void effObjReplaceActiveEventNode(void *obj, u32 entryId);
struct EffNodeDescriptor;
extern struct EffectObj *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, void *secondVector);
extern s32 func_001150B0(s32 arg, f32 *vec0, f32 *vec1);
extern struct EffectObj *effForwardMagatuhiDescriptor(s32 kind, struct EffNodeDescriptor *descriptor);
extern void effObjDispatchReadyState(s32 obj);
extern s32 effObjCopyMagatuhiSourceParameters(struct EffectObj *obj, struct EffectObj *first,
                                               struct EffectObj *second, struct EffectObj *third,
                                               struct EffectObj *fourth);

/* Create the viewer object for a command in the first free slot; returns the slot, or -1 when full. */
s32 evtViewerCreateObjectInFreeSlot(s32 unused, EvtRuntimeGroup *cmd, EvtRuntimeChild *params, EvtRuntime *viewer) {
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
    switch (cmd->type) {
    case 3:
    case 0x1A:
        if (cmd->type == 3) {
            handle = (s32)effObjSpawnDescriptorBoundEffect((struct EffNodeDescriptor *)cmd->resourceData, vec0, vec1);
        } else {
            handle = func_001150B0((s32)cmd->resourceData, vec0, vec1);
        }
        if (params->p0C.sb[0] != 0) {
            effObjDispatchReadyState(handle);
        }
        if (cmd->metadata.extra1 == 0) {
            evtViewerBindNamedOwner((EffWorldNode *)handle, params->p0C.sh[1], params->p0C.sb[1], params->frame, viewer);
        } else {
            evtViewerBindNamedOwner((EffWorldNode *)handle, params->p0C.sh[1], params->p0C.sb[1], 0, viewer);
        }
        break;
    case 0x14:
    case 0x15:
        switch (cmd->type) {
        case 0x14:
            handle = (s32)effForwardMagatuhiDescriptor(1, (struct EffNodeDescriptor *)cmd->resourceData);
            break;
        case 0x15:
            handle = (s32)effForwardMagatuhiDescriptor(2, (struct EffNodeDescriptor *)cmd->resourceData);
            break;
        }
        n0 = evtEventViewerGetNameObject(params->p0C.sb[0], viewer);
        n1 = evtEventViewerGetNameObject(params->p0C.sb[1], viewer);
        n2 = evtEventViewerGetNameObject(params->p0C.sb[2], viewer);
        effObjCopyMagatuhiSourceParameters((struct EffectObj *)handle, n0, n1, n2,
                                           evtEventViewerGetNameObject(params->p0C.sb[3], viewer));
        if (params->p08.sb[1] != 0) {
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

void evtEventViewerFreeSlot(s32 index, EvtRuntime *viewer)
{
    void **slot;

    slot = (void **)(index * 4 + (s32)viewer + 0x203c);
    if (*slot != NULL) {
        dds3RemoveWorldObjectNode(*slot);
        *slot = NULL;
    }
}

/* Create an event-viewer effect at the origin and attach its active event node. */
void *func_0022CA88(void *resource, u32 entryId, s32 value, s32 type, u32 word,
                    EvtRuntime *viewer) {
    f32 position[4];
    f32 scale[4];
    void *effect;

    memset(position, 0, 0x10);
    memset(scale, 0, 0x10);
    scale[3] = 1.0f;
    effect = func_00115298(resource, position, scale);
    effObjSetFlags((s32)effect, 1);
    effObjReplaceActiveEventNode(effect, entryId);
    evtViewerBindNamedOwner((EffWorldNode *)effect, value, type, word, viewer);
    return effect;
}

void evtEventViewerFreeBuffer(EvtRuntimeChild *work)
{
    if (work->payload != NULL) {
        dds3RemoveWorldObjectNode(work->payload);
    }
    work->payload = NULL;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003ACFE8);

