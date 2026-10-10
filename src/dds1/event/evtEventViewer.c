#include "common.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "itf_mes_window.h"
#include "sdf_resource.h"
#include "evt_viewer.h"
#include "kwln.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "pcp_vu0.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_object.h"
#include "mdl.h"
#include "evt_polygon_movie.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"

struct EvtRuntime;
struct EvtRuntime;
struct EvtRuntime;

extern char evtViewerTaskName[]; /* "EventViewer" */
s32 evtViewerHasUpdateFlag(struct EvtRuntime *viewer);
s32 evtViewerUpdateFrame(KwlnTask *task);
void fldInitializeCameraColorResource(void);
void func_00101A80(s32 arg0, s32 arg1);
s32 evtCreateFrameVariableTask(void);
void *evtViewerScheduleFrameVariableTask(s32 arg0);
extern void func_00232E20(PolyMovieWork *work);
extern s32 mnuPollTitleStreamStateLocked(void);
extern void mnuMarkTitleStreamResetPending(void);

/* Effect-channel assignments driven by the viewer's timeline tracks. */

extern void effInitCh71Id(void);
extern void effInitCh72Id(void);
extern void effInitCh75Id(void);
extern void effInitCh76Id(void);
extern void effSetCh71Id(SdfTex *texture);
extern void effSetCh72Id(SdfTex *sourceHandle);
extern void effSetCh75Id(SdfTex *texture);
extern void effSetCh76Id(SdfTex *sourceHandle);
extern void *mnuCampFindEntryByName(void *scene, const char *name);

extern u32 mnuCampGetPrimaryOption(void *scene);
extern const char *D_00368410[];
extern const char *D_00368418[];
extern const char *D_00368420[];
extern const char *D_00368430[];
extern s32 D_003BBE94;

extern u32 kwlnDrawControlFlags;
extern s8 D_0036876A[];
extern u8 D_003BBE88[3];
extern u32 kwlnGetDrawBufferIndex(void);
extern void kwlnFadeSetColor(s32 red, s32 green, s32 blue, s32 alpha);
void evtApplyViewerTimelineFrame(s32 time, EvtRuntime *viewer);
void evtViewerPushCommandHistory(s32 arg0, s32 arg1, s32 arg2, EvtRuntime *arg3);
void *dds3GetWorldObject(void);
f32 dds3GetCameraFieldOfView(EffWorldNode *camera);
void sdfSetViewFieldOfView(f32 arg0);
void mnuStopMovieDrawTask(void);
void mnuCheckMovieDecoderStatus(void);

/* Handles retained by the viewer and by its owning task context. */



/* Script-command parameter slots have byte, halfword, word and float views. */


u16 evtViewerPopHistory(EvtRuntime *viewer);

/* Timeline key shared by several track kinds, not a rendered font glyph.
 * The selector/channel widths depend on the enclosing track and key kind. */


/* Linked timeline track. Saved vectors and the attachment latch are used by
 * kind-1 world-object tracks; the vectors keep their original embedded layout. */


extern void evtReorderListNodes(EvtRuntimeGroup *track);

typedef struct CampDisplayDefaults CampDisplayDefaults;
extern void func_00243048(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);
extern void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display);


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


/* Interpolates parameter keys at the viewer's current frame, accounting for
 * the track offset. A missing next key leaves the interpolation ratio at zero. */
void evtViewerApplyInterpolatedNodeKey(EvtRuntime *viewer, EvtRuntimeGroup *node, u16 *from, u16 *to) {
    u8 out[0x20];
    f32 ratio = 0.0f;

    if (from != NULL) {
        if (to != NULL) {
            s32 start = *from;
            f32 span = *to - start;
            f32 elapsed = viewer->curFrame - (start + node->metadata.value);

            if (span != 0.0f) {
                ratio = elapsed / span;
            }
        }
        func_00243048((EvtRuntimeChild *)from, (EvtRuntimeChild *)to, (CampDisplayDefaults *)out, ratio);
        mnuDrawCampScaledTexture(node->texture, (CampDisplayDefaults *)out);
    }
}

/* Applies kind-24 parameter tracks using bracketing keys, or the pending key
 * when keyMode is 1. Traversal uses the shared timeline-key record. */
void evtViewerApplyParameterKeyTracks(EvtRuntime *viewer) {
    EvtRuntimeGroup *node = viewer->groups;
    s32 position = viewer->curFrame;

    while (node != NULL) {
        if (node->type == 24) {
            if (node->unk28 == 1) {
                u16 *key = (u16 *)evtEventViewerGetPendingNode(viewer);
                evtViewerApplyInterpolatedNodeKey(viewer, node, key, NULL);
            } else {
                EvtRuntimeChild *glyph = node->children;
                EvtRuntimeChild *from;

                while (glyph != NULL && position >= glyph->frame + node->metadata.value) {
                    glyph = glyph->next;
                }
                if (glyph != NULL) {
                    from = glyph->prev;
                } else {
                    from = node->lastChild;
                }
                evtViewerApplyInterpolatedNodeKey(viewer, node, (u16 *)from, (u16 *)glyph);
            }
        }
        node = node->next;
    }
}


extern KwlnDrawVectorParams kwlnDrawVector;
extern u128 *D_00324770[];
extern u128 kwlnDefaultColorVector[];
extern f32 D_003BD358;
extern f32 D_003BD35C;
extern EffBlurTemplateBody *effEventGetBlurTemplateSetupParams(void);
extern EffBlurScatterParams *effEventGetScatterBlurSetupParams(void);
extern EffBlurScaleParams *effEventGetScaleBlurSetupParams(void);
extern EffResourceRectParams *effEventGetResourceTemplateSetupParams(void);
extern void *memcpy(void *destination, const void *source, u32 size);

extern void effEnableTexturedBlur(void);
extern void effDisableTexturedBlur(void);
extern void effEnableTexturedSquare(void);
extern void effDisableTexturedSquare(void);
extern void effEnableFilterBlur(void);
extern void effDisableFilterBlur(void);
extern void effEnableStaggeredBlur(void);
extern void effDisableStaggeredBlur(void);
extern void effEnableFramebufferQuad(void);
extern void effDisableFramebufferQuad(void);
extern void effEnableColorRectangle(void);
extern void effDisableColorRectangle(void);
extern void func_00243A18();

void func_0022CD30(EvtRuntime *viewer) {
    if (viewer->blurRectangleEnabled != 0) {
        effDrawBlurRectangle(&effGetLoadDescA()->source);
    }
    if (viewer->texturedBlurEnabled != 0) {
        effEnableTexturedBlur();
    } else {
        effDisableTexturedBlur();
    }
    if (viewer->texturedSquareEnabled != 0) {
        effEnableTexturedSquare();
    } else {
        effDisableTexturedSquare();
    }
    if (viewer->filterBlurEnabled != 0) {
        effEnableFilterBlur();
    } else {
        effDisableFilterBlur();
    }
    if (viewer->staggeredBlurEnabled != 0) {
        effEnableStaggeredBlur();
    } else {
        effDisableStaggeredBlur();
    }
    if (viewer->framebufferQuadEnabled != 0) {
        effEnableFramebufferQuad();
    } else {
        effDisableFramebufferQuad();
    }
    if (viewer->colorRectangleEnabled != 0) {
        effEnableColorRectangle();
    } else {
        effDisableColorRectangle();
    }
    func_00243A18(viewer);
    evtViewerApplyParameterKeyTracks(viewer);
}

void evtViewerApplySelectedEntry(EvtRuntime *viewer) {
    s32 entry;
    s32 selected;

    selected = viewer->activeEntryIndex;
    if (selected != 0) {
        entry = selected;
    } else {
        entry = viewer->fallbackEntry;
    }
    if (entry == 0) {
        return;
    }
    dds3SetWorldCameraObject(dds3GetWorldObject(), (EffWorldNode *)entry);
    sdfSetViewFieldOfView(dds3GetCameraFieldOfView((EffWorldNode *)entry));
}

extern s32 evtViewerCreateObjectInFreeSlot(s32, EvtRuntimeGroup *, EvtRuntimeChild *, EvtRuntime *);
extern struct EffectObj *evtEventViewerGetNameObject(s32, EvtRuntime *);
extern s32 effObjCopyMagatuhiSourceParameters(struct EffectObj *, struct EffectObj *, struct EffectObj *, struct EffectObj *, struct EffectObj *);
extern void evtEventViewerFreeBuffer(EvtRuntimeChild *);
extern void *func_0022CA88(void *, u32, s32, s32, u32, EvtRuntime *);
extern s32 evtSetBgmVolumePan(s32, s32);
extern void mnuShopRegisterSceneObject(EvtRuntime *, s32);
extern s32 evtPlayBgm(s32, s32);
extern s32 evtFadeInBgm(s32, s32);
extern s32 evtQueueValidatedBgmSoundCode(s32, s32);
extern void evtTransitionBgm(s32, s32);
extern void func_002E9708(void);
extern void func_002E9730(void);
extern void func_002E8E00(void);
extern void kwlnFadeSetupFrames(s32, s32);
extern s32 itfMesSelectTableItemText(s32, s32, s32);
extern void itfMesBuildOptionList(s32, s32);
extern void kwlnPadStartMotor(u32, u8, s32);
extern void mnuStopMovieDrawTask(void);
extern void mnuRequestIndexedMovieResource(s32);
extern void kwlnFadeSetMode(s32);
extern void kwlnFadeBackgroundStartOut(s32);
extern void kwlnFadeBackgroundStartIn(s32);
extern u16 D_003BBE78;
extern char D_003ACFE8[];
extern void mnuUnpackNibbleFields(EvtRuntimeChild *, s32 *, s32 *);
extern s32 evtViewerTestIndexedCondition(u32);
extern s32 mnuQueryTitleSoundBusy(void);
extern void mnuStopTitleVoicePlayback(void);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void func_0014A298(u32);
extern void func_0014A2C8(void);
extern void kwlnCancelConfiguredFadeFrames(void);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003ACFE8);

void func_0022CED0(EvtRuntimeGroup *track, EvtRuntimeChild *key, s32 frame, s32 unused, EvtRuntime *viewer) {
    s32 nibbles[2];
    struct EffectObj *object;
    s32 result;

    nibbles[0] = 0;
    nibbles[1] = 0;
    switch (track->type) {
    /* Retail jtbl_003AD010 entries 0..2 at 003AD010..18 are no-op exits. */
    case 0:
        break;
    case 3:
    case 0x14:
    case 0x15:
    case 0x1A:
        if (frame >= key->frame && key->p08.sb[0] < 0) {
            key->p08.sb[0] = evtViewerCreateObjectInFreeSlot(frame, track, key, viewer);
        }
        if (key->p08.sb[0] < 0) {
            break;
        }
        object = (struct EffectObj *)viewer->objects[key->p08.sb[0]];
        if (object == 0) {
            break;
        }
        if (track->type == 0x15) {
            effObjCopyMagatuhiSourceParameters(object, evtEventViewerGetNameObject(key->p0C.sb[0], viewer),
                                               evtEventViewerGetNameObject(key->p0C.sb[1], viewer),
                                               evtEventViewerGetNameObject(key->p0C.sb[2], viewer),
                                               evtEventViewerGetNameObject(key->p0C.sb[3], viewer));
        }
        break;
    case 0x12:
        if (frame != key->frame) {
            break;
        }
        evtEventViewerFreeBuffer(key);
        if (track->metadata.extra1 == 0) {
            key->payload = func_0022CA88(track->resourceData, key->p0C.sb[0], key->p08.sh[1], -1, key->frame, viewer);
        } else {
            key->payload = func_0022CA88(track->resourceData, key->p0C.sb[0], key->p08.sh[1], -1, 0, viewer);
        }
        break;
    case 5:
        evtSetBgmVolumePan(D_003BBE78, key->p08.sh[0]);
        break;
    case 0x13:
        if (evtViewerHasUpdateFlag(viewer) == 1) {
            break;
        }
        switch (key->p08.sh[0]) {
        case 0:
            evtPlayBgm(D_003BBE78, key->p08.sh[1]);
            mnuShopRegisterSceneObject(viewer, key->p08.sh[1]);
            break;
        case 1:
            evtFadeInBgm(D_003BBE78, key->p08.sh[1]);
            mnuShopRegisterSceneObject(viewer, key->p08.sh[1]);
            break;
        case 2:
            evtQueueValidatedBgmSoundCode(D_003BBE78, key->p08.sh[1]);
            break;
        case 3:
            evtTransitionBgm(D_003BBE78, key->p08.sh[1]);
            break;
        case 4:
            func_002E9708();
            break;
        case 5:
            func_002E9730();
            break;
        case 6:
            func_002E8E00();
            break;
        }
        break;
    case 7:
        if (key->duration == 0 && key->p08.sh[0] == 0) {
            viewer->flags &= ~2;
            kwlnCancelConfiguredFadeFrames();
        } else if (frame == key->frame) {
            viewer->flags |= 2;
            kwlnFadeSetupFrames(key->duration, key->p08.sh[0]);
        }
        break;
    case 4:
        if (viewer->windowContext->handle == -1 || evtViewerHasUpdateFlag(viewer) != 0) {
            break;
        }
        mnuUnpackNibbleFields(key, &nibbles[0], &nibbles[1]);
        if (key->frame != frame) {
            break;
        }
        if (itfMesGetWindowEntryItems(viewer->windowContext->handle, nibbles[0]) == 0) {
            if (!evtViewerTestIndexedCondition(key->p08.sh[1])) {
                break;
            }
            if (mnuQueryTitleSoundBusy()) {
                result = itfMesSelectTableItemText(viewer->windowContext->handle, nibbles[0], 0);
                if (result == 1) {
                    evtPrintDeveloperConsoleMessage(D_003ACFE8, nibbles[0]);
                    mnuStopTitleVoicePlayback();
                    viewer->voicePending = result;
                    viewer->voiceMessage = nibbles[0];
                    viewer->voiceFrame = viewer->curFrame;
                    break;
                }
            }
            itfMesStartEntry(viewer->windowContext->handle, nibbles[0], 0);
        } else {
            viewer->unk23C6 = nibbles[1] + 0xC7;
            itfMesBuildOptionList(viewer->windowContext->handle, nibbles[0]);
            viewer->flags |= 1;
            evtViewerDispatchFlagMode(viewer);
        }
        break;
    case 0x1C:
        if (frame == key->frame + track->metadata.value) {
            kwlnPadStartMotor(key->p08.sh[0], key->p08.b[2], key->duration);
        }
        break;
    case 0x1D:
        if (evtViewerHasUpdateFlag(viewer) == 1 || frame != key->frame) {
            break;
        }
        if (viewer->timedActive != 0) {
            mnuStopMovieDrawTask();
        }
        mnuRequestIndexedMovieResource(key->p08.sh[0]);
        viewer->timedActive = 1;
        viewer->timedStart = key->frame;
        if (key->duration == 0) {
            viewer->timedEnd = -1;
        } else {
            viewer->timedEnd = key->frame + key->duration;
        }
        break;
    case 0x1E:
        if (frame != key->frame || fldTitleIsActive() != 1) {
            break;
        }
        if (key->p08.sh[1] != 0) {
            func_0014A298(1);
        } else {
            func_0014A298(0);
        }
        func_0014A2C8();
        break;
    case 0x20:
        if (frame != key->frame) {
            break;
        }
        if (key->p08.sh[0] == 0) {
            kwlnFadeSetMode(1);
            kwlnFadeBackgroundStartOut(key->duration);
        } else {
            kwlnFadeSetMode(1);
            kwlnFadeBackgroundStartIn(key->duration);
        }
        break;
    }
}


