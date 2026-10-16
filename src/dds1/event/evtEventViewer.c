#include "common.h"
#include "sdf_dma_tag.h"
#include "evt_camera_color_track_api.h"
#include "kwln_background_fade_api.h"
#include "evt_scaled_value.h"
#include "evt_viewer_playback.h"
#include "kwln_task_add_child.h"
#include "mnu_title_stream.h"
#include "mnu_camp_font_api.h"
#include "kwln_draw_buffer.h"
#include "mdl_motion_api.h"
#include "mdl_world_object_source_vector_api.h"
#include "sdf_motion.h"
#include "itf_mes_window.h"
#include "sdf_resource.h"
#include "evt_viewer.h"
#include "evt_reload_viewer_api.h"
#include "evt_reset_unit_vector_slots_api.h"
#include "kwln.h"
#include "kwln_fade.h"
#include "kwln_fade_timing.h"
#include "kwln_task_create.h"
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
#include "eff_object_dispatch_api.h"
#include "mdl.h"
#include "evt_polygon_movie.h"
#include "evt_viewer_group_record.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"

#include "eff_dependency.h"
#include "eff_node.h"
struct EvtRuntime;
struct EvtRuntime;
struct EvtRuntime;

extern char evtViewerTaskName[]; /* "EventViewer" */
s32 evtViewerHasUpdateFlag(struct EvtRuntime *viewer);
s32 evtViewerUpdateFrame(KwlnTask *task);
void fldInitializeCameraColorResource(void);
s32 evtCreateFrameVariableTask(void);
void *evtViewerScheduleFrameVariableTask(s32 task);
extern void evtEventViewerPollMovieResources(PolyMovieWork *assets);

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
extern void kwlnFadeSetColor(s32 red, s32 green, s32 blue, s32 alpha);
void evtApplyViewerTimelineFrame(s32 time, EvtRuntime *viewer);
void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, EvtRuntime *viewer);
EffWorldNode *dds3GetWorldObject(void);
f32 dds3GetCameraFieldOfView(EffWorldNode *camera);
void sdfSetViewFieldOfView(f32 fieldOfView);
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
extern void mnuBlendCampDisplayFields(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);
extern void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display);


#include "common.h"
#include "sdf_chip.h"
#include "evt_viewer.h"
#include "dds3obj.h"
#include "evt_world.h"

/* Node queued on an entry, linked through +0x30/+0x34, owning a buffer. */


/* Event-viewer entry: doubly linked through next/prev, keyed by id. */


/* Table of 0x20-byte (group, type) records at +0x34, counted at +0x38. */
/* Event viewer: name table at 0x20, entry list at 0x2030. */




void evtUnlinkListNode(EvtRuntimeGroup *entry, EvtRuntimeChild *node);
s32 sdfCheckPendingWorkWithInterrupts();

EvtRuntimeChild *evtCreateTrackKeyAtFrame(EvtRuntimeGroup *group, s32 frame, EvtRuntime *viewer);
void effInitCh71Id(void);
void effInitCh72Id(void);
void effInitCh76Id(void);
void effInitCh75Id(void);
void evtPolygonMovieFreeWork(PolyMovieWork *table);
void btlRemoveCurrentGroupedEntity(s32 group, s32 type);
void kwlnPadResetMotorLevelsAndOutput(void);
EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *viewer);
void evtEventViewerReleasePendingNode(EvtRuntime *viewer);
void evtEventViewerFreeSlot(s32 index, EvtRuntime *viewer);
void evtEventViewerFreeBuffer(EvtRuntimeChild *node);
void *memset(void *dst, s32 value, u32 size);
EffWorldNode *dds3GetWorldObject(void);
s32 strcmp(const char *a, const char *b);
char *strcpy(char *dst, const char *src);

EvtRuntimeChild *evtEventViewerCreateTrackKeyAtFrame(EvtRuntimeGroup *group, s32 frame, EvtRuntime *viewer)
{
    return evtCreateTrackKeyAtFrame(group, frame, viewer);
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
void evtEventViewerReleasePendingNode(EvtRuntime *viewer)
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
void evtEventViewerReleaseQueuedNode(EvtRuntime *viewer, EvtRuntimeGroup *entry, EvtRuntimeChild *node)
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
        evtEventViewerReleasePendingNode(viewer);
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

/* Allocate a zeroed 0x84-byte entry for a type and queue it in the viewer. */
EvtRuntimeGroup *evtEventViewerCreateEntry(s32 entryType, EvtRuntime *viewer)
{
    EvtRuntimeGroup *entry;

    entry = sdfAllocSizeClassBlock(0x84);
    if (entry == NULL) {
        return NULL;
    }
    memset(entry, 0, 0x84);
    entry->type = entryType;
    entry->entryHeader = -1;
    entry->argument0C = -1;
    evtEventViewerInsertEntry(entry, viewer);
    return entry;
}

s32 evtEventViewerCountEntriesById(s32 entryType, EvtRuntime *viewer)
{
    EvtRuntimeGroup *entry;
    s32 matchingEntryCount;

    matchingEntryCount = 0;
    entry = viewer->groups;
    while (entry != NULL) {
        if (entry->type == entryType) {
            matchingEntryCount = matchingEntryCount + 1;
        }
        entry = entry->next;
    }
    return matchingEntryCount;
}

s32 evtEventViewerCountEntries(EvtRuntime *viewer)
{
    EvtRuntimeGroup *entry;
    s32 entryCount;

    entryCount = 0;
    entry = viewer->groups;
    while (entry != NULL) {
        entryCount = entryCount + 1;
        entry = entry->next;
    }
    return entryCount;
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
    s32 currentFrame;

    currentFrame = range->curFrame;
    range->headerFirst = currentFrame;
    if (range->frameRange.word < currentFrame) {
        range->frameRange.word = currentFrame;
    }
}

void evtViewerSetMaximumFromCurrent(EvtRuntime *range)
{
    s32 currentFrame;

    currentFrame = range->curFrame;
    range->frameRange.word = currentFrame;
    if (currentFrame < range->headerFirst) {
        range->headerFirst = currentFrame;
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
        for (i = 0; i < (s32)viewer->windowContext->mainEntry3Count; i++) {
            btlRemoveCurrentGroupedEntity(((EvtViewerGroupRecord *)viewer->windowContext->mainEntry3Data)[i].group, ((EvtViewerGroupRecord *)viewer->windowContext->mainEntry3Data)[i].type);
        }
    }
}

/* Search the fixed-width (0x20-byte) event-name records. */
s32 evtEventViewerFindNameIndex(const char *name, EvtRuntime *viewer)
{
    const char *nameEntry;
    s32 nameIndex;

    nameIndex = 0;
    if (0 < viewer->entryTotal) {
        nameEntry = viewer->entryName[0];
        do {
            if (strcmp(name, nameEntry) == 0) {
                return nameIndex;
            }
            nameIndex = nameIndex + 1;
            nameEntry = nameEntry + 0x20;
        } while (nameIndex < viewer->entryTotal);
    }
    return -1;
}

/* Return the index of a name in the table, appending it when missing. */
s32 evtEventViewerAddName(const char *name, EvtRuntime *viewer)
{
    s32 existingNameIndex;
    s32 newNameIndex;

    existingNameIndex = evtEventViewerFindNameIndex(name, viewer);
    if (existingNameIndex >= 0) {
        return existingNameIndex;
    }
    newNameIndex = viewer->entryTotal;
    strcpy(viewer->entryName[newNameIndex], name);
    viewer->entryTotal++;
    return newNameIndex;
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

extern s32 effObjBindOwnerBillEntry(struct EffectObj *, struct EffectObj *, s32);
extern s32 effObjBindValidatedOwner(struct EffectObj *, struct EffectObj *);
extern s32 evtStageRelinkOwnedNodeResource(void *object, void *owner);

/* Billboard entries and polygon movies use distinct owner attachment paths. */
void evtViewerBindNamedOwner(EffWorldNode *effectObject, s32 ownerNameIndex, s32 billboardEntry, u32 startFrame, EvtRuntime *viewer) {
    EffWorldNode *owner;
    struct PolyMovieObject *movieObject;
    s32 currentFrame;

    if (effectObject == 0) {
        return;
    }
    currentFrame = viewer->curFrame;
    if (ownerNameIndex < 0) {
        return;
    }
    owner = dds3FindObjectChainNodeByName(
        (EffWorldNode *)dds3GetWorldObject(),
        (const u8 *)viewer->entryName[ownerNameIndex]);
    if (owner == NULL) {
        return;
    }
    switch (owner->kindTag >> 24) {
    case EFF_WORLD_KIND_FOLLOW_MODEL:
        if (billboardEntry >= 0) {
            effObjBindOwnerBillEntry((struct EffectObj *)effectObject,
                                    (struct EffectObj *)owner, billboardEntry);
            break;
        }
        /* A negative entry selects the normal owner link instead. */
    case EFF_WORLD_KIND_CAMERA:
    case EFF_WORLD_KIND_EFFECT_TRANSFORM:
    case EFF_WORLD_KIND_EFFECT_OBJECT:
    case EFF_WORLD_KIND_RESOURCE_OWNER:
    case EFF_WORLD_KIND_LIGHT:
    case EFF_WORLD_KIND_TRANSFORM_SOURCE:
        effObjBindValidatedOwner((struct EffectObj *)effectObject, (struct EffectObj *)owner);
        break;
    case EFF_WORLD_KIND_ACTION_10:
        evtStageRelinkOwnedNodeResource(owner, (void *)effectObject);
        movieObject = dds3GetObjectOwnedHandle(effectObject)->slots[1];
        if (viewer->flags & 1) {
            evtPolygonMovieScaleByProgress(movieObject, 0, startFrame, currentFrame);
        } else {
            evtPolygonMovieScaleByProgress(movieObject, 1, startFrame, currentFrame);
        }
        break;
    }
}


extern void effObjSetFlags(s32 obj, s32 flags);
extern void *effObjCreateEventNode(void *resource, void *position, void *scale);
extern void effObjReplaceActiveEventNode(void *obj, u32 entryId);
struct EffNodeDescriptor;
extern struct EffectObj *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, void *secondVector);
extern s32 effObjCreateBillboardFromFactory(s32 arg, f32 *vec0, f32 *vec1);
extern struct EffectObj *effForwardMagatuhiDescriptor(s32 kind, struct EffNodeDescriptor *descriptor);
extern s32 effObjCopyMagatuhiSourceParameters(struct EffectObj *obj, struct EffectObj *first,
                                               struct EffectObj *second, struct EffectObj *third,
                                               struct EffectObj *fourth);

/* Create the viewer object for a command in the first free slot; returns the slot, or -1 when full. */
s32 evtViewerCreateObjectInFreeSlot(s32 unused, EvtRuntimeGroup *track, EvtRuntimeChild *key, EvtRuntime *viewer) {
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
    switch (track->type) {
    case 3:
    case 0x1A:
        if (track->type == 3) {
            handle = (s32)effObjSpawnDescriptorBoundEffect((struct EffNodeDescriptor *)track->resourceData, vec0, vec1);
        } else {
            handle = effObjCreateBillboardFromFactory((s32)track->resourceData, vec0, vec1);
        }
        if (key->p0C.sb[0] != 0) {
            effObjDispatchReadyState((EffWorldNode *)handle);
        }
        if (track->metadata.extra1 == 0) {
            evtViewerBindNamedOwner((EffWorldNode *)handle, key->p0C.sh[1], key->p0C.sb[1], key->frame, viewer);
        } else {
            evtViewerBindNamedOwner((EffWorldNode *)handle, key->p0C.sh[1], key->p0C.sb[1], 0, viewer);
        }
        break;
    case 0x14:
    case 0x15:
        switch (track->type) {
        case 0x14:
            handle = (s32)effForwardMagatuhiDescriptor(1, (struct EffNodeDescriptor *)track->resourceData);
            break;
        case 0x15:
            handle = (s32)effForwardMagatuhiDescriptor(2, (struct EffNodeDescriptor *)track->resourceData);
            break;
        }
        n0 = evtEventViewerGetNameObject(key->p0C.sb[0], viewer);
        n1 = evtEventViewerGetNameObject(key->p0C.sb[1], viewer);
        n2 = evtEventViewerGetNameObject(key->p0C.sb[2], viewer);
        effObjCopyMagatuhiSourceParameters((struct EffectObj *)handle, n0, n1, n2,
                                           evtEventViewerGetNameObject(key->p0C.sb[3], viewer));
        if (key->p08.sb[1] != 0) {
            effObjDispatchReadyState((EffWorldNode *)handle);
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
void *evtEventViewerCreateOriginEffect(void *resource, u32 entryId, s32 ownerNameIndex, s32 billboardEntry, u32 startFrame,
                    EvtRuntime *viewer) {
    f32 position[4];
    f32 scale[4];
    void *effect;

    memset(position, 0, 0x10);
    memset(scale, 0, 0x10);
    scale[3] = 1.0f;
    effect = effObjCreateEventNode(resource, position, scale);
    effObjSetFlags((s32)effect, 1);
    effObjReplaceActiveEventNode(effect, entryId);
    evtViewerBindNamedOwner((EffWorldNode *)effect, ownerNameIndex, billboardEntry, startFrame, viewer);
    return effect;
}

void evtEventViewerFreeBuffer(EvtRuntimeChild *node)
{
    if (node->payload != NULL) {
        dds3RemoveWorldObjectNode(node->payload);
    }
    node->payload = NULL;
}


/* Interpolates parameter keys at the viewer's current frame, accounting for
 * the track offset. A missing next key leaves the interpolation ratio at zero. */
void evtViewerApplyInterpolatedNodeKey(EvtRuntime *viewer, EvtRuntimeGroup *track, u16 *from, u16 *to) {
    u8 out[0x20];
    f32 ratio = 0.0f;

    if (from != NULL) {
        if (to != NULL) {
            s32 start = *from;
            f32 span = *to - start;
            f32 elapsed = viewer->curFrame - (start + track->metadata.value);

            if (span != 0.0f) {
                ratio = elapsed / span;
            }
        }
        mnuBlendCampDisplayFields((EvtRuntimeChild *)from, (EvtRuntimeChild *)to, (CampDisplayDefaults *)out, ratio);
        mnuDrawCampScaledTexture(track->texture, (CampDisplayDefaults *)out);
    }
}

/* Applies kind-24 parameter tracks using bracketing keys, or the pending key
 * when keyMode is 1. Traversal uses the shared timeline-key record. */
void evtViewerApplyParameterKeyTracks(EvtRuntime *viewer) {
    EvtRuntimeGroup *track = viewer->groups;
    s32 position = viewer->curFrame;

    while (track != NULL) {
        if (track->type == 24) {
            if (track->usePendingKey == 1) {
                u16 *key = (u16 *)evtEventViewerGetPendingNode(viewer);
                evtViewerApplyInterpolatedNodeKey(viewer, track, key, NULL);
            } else {
                EvtRuntimeChild *glyph = track->children;
                EvtRuntimeChild *from;

                while (glyph != NULL && position >= glyph->frame + track->metadata.value) {
                    glyph = glyph->next;
                }
                if (glyph != NULL) {
                    from = glyph->prev;
                } else {
                    from = track->lastChild;
                }
                evtViewerApplyInterpolatedNodeKey(viewer, track, (u16 *)from, (u16 *)glyph);
            }
        }
        track = track->next;
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

void evtEventViewerApplyEffectToggles(EvtRuntime *viewer) {
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
    evtUpdateCameraColorTrackEffect(viewer);
    evtViewerApplyParameterKeyTracks(viewer);
}

void evtViewerApplySelectedEntry(EvtRuntime *viewer) {
    s32 selectedEntry;
    s32 activeEntry;

    activeEntry = viewer->activeEntryIndex;
    if (activeEntry != 0) {
        selectedEntry = activeEntry;
    } else {
        selectedEntry = viewer->fallbackEntry;
    }
    if (selectedEntry == 0) {
        return;
    }
    dds3SetWorldCameraObject(dds3GetWorldObject(), (EffWorldNode *)selectedEntry);
    sdfSetViewFieldOfView(dds3GetCameraFieldOfView((EffWorldNode *)selectedEntry));
}

extern s32 evtViewerCreateObjectInFreeSlot(s32, EvtRuntimeGroup *, EvtRuntimeChild *, EvtRuntime *);
extern struct EffectObj *evtEventViewerGetNameObject(s32, EvtRuntime *);
extern s32 effObjCopyMagatuhiSourceParameters(struct EffectObj *, struct EffectObj *, struct EffectObj *, struct EffectObj *, struct EffectObj *);
extern void evtEventViewerFreeBuffer(EvtRuntimeChild *);
extern void *evtEventViewerCreateOriginEffect(void *, u32, s32, s32, u32, EvtRuntime *);
extern s32 evtSetBgmVolumePan(s32 id, s32 fade);
extern void mnuShopRegisterSceneObject(EvtRuntime *, s32);
extern s32 evtPlayBgm(s32 id, s32 fade);
extern s32 evtFadeInBgm(s32 id, s32 fade);
extern s32 evtQueueValidatedBgmSoundCode(s32 id, s32 fade);
extern void evtTransitionBgm(s32 id, s32 fade);
extern void sndStartReducedBgm(void);
extern void sndStopReducedBgm(void);
extern void sndQueueNoPayloadCommand1A0(void);
extern s32 itfMesSelectTableItemText(s32, s32, s32);
extern void itfMesBuildOptionList(s32, s32);
extern void kwlnPadStartMotor(u32, u8, s32);
extern void mnuStopMovieDrawTask(void);
extern void mnuRequestIndexedMovieResource(s32);
extern void kwlnFadeBackgroundStartIn(s32);
extern u16 D_003BBE78;
extern char D_003ACFE8[];
extern void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut);
extern s32 evtViewerTestIndexedCondition(u32 condition);
extern s32 mnuQueryTitleSoundBusy(void);
extern void mnuStopTitleVoicePlayback(void);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void fldSetTitleDrawMode(u32);
extern void fldBeginTitleAnimation(void);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003ACFE8);

void evtEventViewerApplyTimelineKey(EvtRuntimeGroup *track, EvtRuntimeChild *key, s32 currentFrame, s32 unused, EvtRuntime *viewer) {
    s32 nibbles[2];
    struct EffectObj *object;
    s32 selectedText;

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
        if (currentFrame >= key->frame && key->p08.sb[0] < 0) {
            key->p08.sb[0] = evtViewerCreateObjectInFreeSlot(currentFrame, track, key, viewer);
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
        if (currentFrame != key->frame) {
            break;
        }
        evtEventViewerFreeBuffer(key);
        if (track->metadata.extra1 == 0) {
            key->payload = evtEventViewerCreateOriginEffect(track->resourceData, key->p0C.sb[0], key->p08.sh[1], -1, key->frame, viewer);
        } else {
            key->payload = evtEventViewerCreateOriginEffect(track->resourceData, key->p0C.sb[0], key->p08.sh[1], -1, 0, viewer);
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
            sndStartReducedBgm();
            break;
        case 5:
            sndStopReducedBgm();
            break;
        case 6:
            sndQueueNoPayloadCommand1A0();
            break;
        }
        break;
    case 7:
        if (key->duration == 0 && key->p08.sh[0] == 0) {
            viewer->flags &= ~2;
            kwlnCancelConfiguredFadeFrames();
        } else if (currentFrame == key->frame) {
            viewer->flags |= 2;
            kwlnFadeSetupFrames(key->duration, key->p08.sh[0]);
        }
        break;
    case 4:
        if (viewer->windowContext->handle == -1 || evtViewerHasUpdateFlag(viewer) != 0) {
            break;
        }
        mnuUnpackNibbleFields(key, &nibbles[0], &nibbles[1]);
        if (key->frame != currentFrame) {
            break;
        }
        if (itfMesGetWindowEntryItems(viewer->windowContext->handle, nibbles[0]) == 0) {
            if (!evtViewerTestIndexedCondition(key->p08.sh[1])) {
                break;
            }
            if (mnuQueryTitleSoundBusy()) {
                selectedText = itfMesSelectTableItemText(viewer->windowContext->handle, nibbles[0], 0);
                if (selectedText == 1) {
                    evtPrintDeveloperConsoleMessage(D_003ACFE8, nibbles[0]);
                    mnuStopTitleVoicePlayback();
                    viewer->voicePending = selectedText;
                    viewer->voiceMessage = nibbles[0];
                    viewer->voiceFrame = viewer->curFrame;
                    break;
                }
            }
            itfMesStartEntry(viewer->windowContext->handle, nibbles[0], 0);
        } else {
            viewer->windowResultVariableIndex = nibbles[1] + 0xC7;
            itfMesBuildOptionList(viewer->windowContext->handle, nibbles[0]);
            viewer->flags |= 1;
            evtViewerDispatchFlagMode(viewer);
        }
        break;
    case 0x1C:
        if (currentFrame == key->frame + track->metadata.value) {
            kwlnPadStartMotor(key->p08.sh[0], key->p08.b[2], key->duration);
        }
        break;
    case 0x1D:
        if (evtViewerHasUpdateFlag(viewer) == 1 || currentFrame != key->frame) {
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
        if (currentFrame != key->frame || fldTitleIsActive() != 1) {
            break;
        }
        if (key->p08.sh[1] != 0) {
            fldSetTitleDrawMode(1);
        } else {
            fldSetTitleDrawMode(0);
        }
        fldBeginTitleAnimation();
        break;
    case 0x20:
        if (currentFrame != key->frame) {
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





struct EvtRuntime;
struct EvtRuntime;
struct EvtRuntime;

extern char evtViewerTaskName[]; /* "EventViewer" */
s32 evtViewerHasUpdateFlag(struct EvtRuntime *viewer);
s32 evtViewerUpdateFrame(KwlnTask *task);
void fldInitializeCameraColorResource(void);
s32 evtCreateFrameVariableTask(void);
void *evtViewerScheduleFrameVariableTask(s32 task);
extern void evtEventViewerPollMovieResources(PolyMovieWork *assets);

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
extern void kwlnFadeSetColor(s32 red, s32 green, s32 blue, s32 alpha);
void evtApplyViewerTimelineFrame(s32 time, EvtRuntime *viewer);
void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, EvtRuntime *viewer);
EffWorldNode *dds3GetWorldObject(void);
f32 dds3GetCameraFieldOfView(EffWorldNode *camera);
void sdfSetViewFieldOfView(f32 fieldOfView);
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
extern void mnuBlendCampDisplayFields(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);
extern void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display);


/* Timeline payloads store fogFar before fogNear. */
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

extern void evtEventViewerFreeSlot(s32 index, EvtRuntime *viewer);
extern void evtEventViewerFreeBuffer(EvtRuntimeChild *key);

void evtEventViewerFadeEffectSlot(EvtRuntimeGroup *group, EvtRuntimeChild *key, s32 time,
                   s32 interval, EvtRuntime *viewer) {
    s32 slot;
    s32 start;
    s32 currentFrame;
    s32 duration;
    s32 end;
    s32 alpha;

    switch (group->type) {
    case 3:
    case 20:
    case 21:
    case 26:
        slot = key->p08.sb[0];
        if (slot >= 0) {
            start = key->frame;
            currentFrame = viewer->curFrame;
            duration = key->duration;
            if (currentFrame < start) {
                evtEventViewerFreeSlot(slot, viewer);
                key->p08.sb[0] = -1;
                break;
            }
            end = start + interval + duration;
            if (currentFrame >= end) {
                evtEventViewerFreeSlot(slot, viewer);
                key->p08.sb[0] = -1;
                break;
            }
            alpha = (s32)((128.0f / (f32)duration) * (f32)(end - currentFrame));
            return effSetNodeParameterValue(
                ((EffectDependencyState *)viewer->objects[slot]->data)->handle,
                ((u32)alpha << 24) | 0x00808080);
        }
        break;
    case 18:
        evtEventViewerFreeBuffer(key);
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerUpdateTimelineGroupState);


extern void evtEndUnitValueTransition(EvtUnit *unit, s32 duration);

/* At timelineTime, selects each named unit's latest kind-9 transition key and starts,
 * ends or leaves its value transition according to the selected key flags. */
void evtEventViewerUpdateUnitTransitions(s32 timelineTime, EvtRuntime *viewer) {
    EffWorldNode *worldObject;
    EvtRuntimeGroup *track;
    EvtRuntimeChild *transitionKey;
    EvtRuntimeChild *selectedKey;
    EvtUnit *eventUnit;
    s32 transitionValue;
    s32 latestTransitionTime;

    if (dds3GetWorldObject() != NULL) {
        worldObject = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head;
        while (worldObject != NULL) {
            if (((u8 *)worldObject->value) != NULL) {
                selectedKey = NULL;
                transitionValue = 0;
                latestTransitionTime = -1;
                track = viewer->groups;
                while (track != NULL) {
                    if (track->type == 9) {
                        transitionKey = track->children;
                        while (transitionKey != NULL) {
                            if (timelineTime >= transitionKey->frame + track->metadata.value && transitionKey->p08.sh[0] >= 0 &&
                                worldObject == dds3FindIndexedObjectChainNodeByName(dds3GetWorldObject(),
                                    EVT_WORLD_SLOT_UNIT, viewer->entryName[transitionKey->p08.sh[0]])) {
                                if (latestTransitionTime < transitionKey->frame + track->metadata.value) {
                                    transitionValue = (s32)track->info;
                                    latestTransitionTime = transitionKey->frame + track->metadata.value;
                                    selectedKey = transitionKey;
                                }
                            }
                            transitionKey = transitionKey->next;
                        }
                    }
                    track = track->next;
                }
                eventUnit = evtUnitGetNestedValue(worldObject);
                if (selectedKey != NULL) {
                    if (selectedKey->p08.sh[1] != 0) {
                        if (eventUnit->currentTransitionValue != transitionValue || !(eventUnit->flags & 0x40000)) {
                            evtSetUnitValueTransition(eventUnit, (EffWorldNode *)transitionValue, selectedKey->duration);
                        }
                    } else {
                        if (eventUnit->flags & 0x40000) {
                            if (!(eventUnit->flags & 0x100000)) {
                                evtEndUnitValueTransition(eventUnit, selectedKey->duration);
                            }
                        }
                    }
                } else {
                    if (eventUnit->currentTransitionValue != 0) {
                        evtEndUnitValueTransition(eventUnit, 0);
                    }
                }
            }
            worldObject = worldObject->next;
        }
    }
}

extern f32 evtMovieInterpolateFloatIfEnabled(s32 enable, f32 t, f32 a, f32 b);
extern s32 evtMovieInterpolateIntIfEnabled(s32 enable, f32 t, s32 a, s32 b);

/* At time, applies the track's latest enabled mode keys, then interpolates the
 * unit's byte and float parameters from the active kind-3/4 key to the next
 * enabled key of the same kind. */
void evtViewApplyMovieModeKeys(EvtRuntimeGroup *track, s32 time) {
    u32 objectMode = 0;
    u32 unitMode = 2;
    s32 pass;
    s32 setFlags = 0;
    s32 clearFlags = 0;
    EvtUnit *unit = NULL;
    EvtRuntimeChild *key;

    key = track->children;
    if (key != NULL) {
        do {
            if (time < key->frame) {
                break;
            }
            switch (key->p08.sb[0]) {
            case 0:
            case 1:
                if (evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                    objectMode = key->p08.sb[0];
                }
                break;
            case 2:
            case 3:
            case 4:
                if (evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                    unitMode = key->p08.sb[0];
                    unit = evtUnitGetNestedValue(track->info);
                }
                break;
            }
            key = key->next;
        } while (key != NULL);
    }

    for (pass = 0; pass < 1; pass++) {
        EvtRuntimeChild *current = NULL;
        EvtRuntimeChild *next;
        s8 kind;
        f32 interpolationFactor;
        s32 startFrame;
        s32 endFrame;

        for (key = track->children; key != NULL; key = key->next) {
            if ((key->p08.sb[0] == 3 || key->p08.sb[0] == 4) &&
                evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                if (key->frame <= time) {
                    current = key;
                } else {
                    break;
                }
            }
        }
        if (current == NULL) {
            break;
        }
        kind = current->p08.sb[0];
        next = NULL;
        for (key = track->children; key != NULL; key = key->next) {
            if (key->p08.sb[0] == kind && evtViewerTestIndexedCondition(key->p10.sh[0]) == 1 &&
                time < key->frame) {
                next = key;
                break;
            }
        }
        if (next == NULL) {
            unit->shadowLevel = current->p0C.b[1];
            unit->shadowY = current->p14.f;
        } else {
            startFrame = current->frame;
            endFrame = next->frame;
            if (endFrame != startFrame) {
                interpolationFactor = (f32)(time - startFrame) / (f32)(endFrame - startFrame);
            } else {
                interpolationFactor = 0.0f;
            }
            if (next->p0C.sb[0] == 0 || next->p0C.sb[0] == 2) {
                interpolationFactor = 0.0f;
            }
            unit->shadowLevel = evtMovieInterpolateIntIfEnabled(0, interpolationFactor, current->p0C.b[1], next->p0C.b[1]);
            if (next->p0C.b[0] < 2) {
                interpolationFactor = 0.0f;
            }
            unit->shadowY = evtMovieInterpolateFloatIfEnabled(0, interpolationFactor, current->p14.f, next->p14.f);
        }
        if (current->p0C.sb[0] == 1 || current->p0C.sb[0] == 3) {
            setFlags |= 0x4000;
        } else {
            clearFlags |= 0x4000;
        }
        switch (current->p0C.sb[0]) {
        case 2:
        case 3:
            setFlags |= 0x8000;
            break;
        default:
            clearFlags |= 0x8000;
            break;
        }
    }
    evtPolygonMovieSetObjectMode((struct PolyMovieObject *)track->info, objectMode, 0, 0);
    evtPolygonMovieSetObjectMode((struct PolyMovieObject *)track->info, unitMode, setFlags, clearFlags);
}

extern void evtViewerClampMovieTimes(s32 endTime, EvtRuntime *viewer);
extern void evtViewerSyncWorldGroups(s32 position, EvtRuntime *viewer);
extern void evtViewerApplyGlyphLodChannel(s32 position, EvtRuntime *viewer);
extern void evtEventViewerAttachIndexedObject(s32 position, EvtRuntime *viewer);
extern void evtViewerActivateWindowForGlyphEntry(s32 position, EvtRuntime *viewer);
extern void evtViewerCountFlaggedUpdates(EvtRuntime *viewer);
extern void evtEventViewerApplyEffectChannels(s32 frame, EvtRuntime *viewer);
extern s32 evtViewerUpdateTimedAction(EvtRuntime *viewer);
extern void evtViewerUpdateShopFlagTrack(EvtRuntime *viewer);
extern s32 evtViewerTestIndexedCondition(u32 condition);
extern void evtEventViewerApplyTimelineKey(EvtRuntimeGroup *track, EvtRuntimeChild *key, s32 currentFrame,
                                           s32, EvtRuntime *viewer);
extern void evtEventViewerFadeEffectSlot(EvtRuntimeGroup *group, EvtRuntimeChild *key, s32 time,
                                        s32 interval, EvtRuntime *viewer);
extern void evtViewerUpdateTimelineGroupState(EvtRuntimeGroup *, EvtRuntimeChild *, EvtRuntimeChild *, s32, s32 *, EvtRuntime *, s32);
extern u8 kwlnDrawOverlayEnabled;
extern EvtRuntimeGroup *D_003BBE80;

void evtApplyViewerTimelineFrame(s32 time, EvtRuntime *viewer) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    EvtRuntimeChild *other;
    EvtRuntimeChild *selected;
    s32 difference;
    s32 bestDistance;
    s32 conditionIndex;
    s16 duration;
    s16 offset;

    viewer->blurRectangleEnabled = 0;
    viewer->texturedBlurEnabled = 0;
    viewer->flags &= ~2;
    viewer->colorRectangleEnabled = 0;
    viewer->flags &= ~4;
    conditionIndex = 0;
    viewer->texturedSquareEnabled = 0;
    viewer->filterBlurEnabled = 0;
    viewer->staggeredBlurEnabled = 0;
    viewer->framebufferQuadEnabled = 0;
    evtResetUnitVectorSlots();
    evtViewerClampMovieTimes(time, viewer);
    evtViewerSyncWorldGroups(time, viewer);
    evtViewerApplyGlyphLodChannel(time, viewer);
    evtEventViewerAttachIndexedObject(time, viewer);
    evtViewerActivateWindowForGlyphEntry(time, viewer);
    evtViewerCountFlaggedUpdates(viewer);
    evtEventViewerApplyEffectChannels(time, viewer);
    evtViewerUpdateTimedAction(viewer);
    evtViewerUpdateShopFlagTrack(viewer);

    for (group = viewer->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            evtViewApplyMovieModeKeys(group, time);
            continue;
        }
        for (child = group->children; child != NULL; child = child->next) {
            switch (group->type) {
            case 3:
            case 20:
            case 21:
            case 26:
                duration = child->p08.sh[1];
                if (duration == 0) {
                    difference = viewer->headerThird - child->frame;
                } else {
                    difference = duration - child->frame;
                    if (difference < 0) {
                        difference = 0;
                    }
                }
                break;
            case 18:
                duration = child->p08.sh[0];
                if (duration == 0) {
                    difference = viewer->headerThird - child->frame;
                } else {
                    difference = duration - child->frame;
                    if (difference < 0) {
                        difference = 0;
                    }
                }
                break;
            case 9:
                other = child->next;
                while (other != NULL && other->p08.sh[0] != child->p08.sh[0]) {
                    other = other->next;
                }
                if (other != NULL) {
                    difference = other->frame - child->frame;
                } else {
                    difference = viewer->headerThird - child->frame;
                }
                break;
            default:
                difference = child->duration;
                break;
            }

            if ((difference == 0 && time == child->frame) ||
                (difference > 0 && time >= child->frame &&
                 time < child->frame + difference)) {
                evtEventViewerApplyTimelineKey(group, child, time, difference, viewer);
            } else {
                evtEventViewerFadeEffectSlot(group, child, time, difference, viewer);
            }
        }
    }

    bestDistance = 10000;
    D_003BBE80 = NULL;
    for (group = viewer->groups; group != NULL; group = group->next) {
        offset = group->metadata.value;
        child = group->children;
        for (; child != NULL; child = child->next) {
            if (group->type == 2) {
                if (evtViewerTestIndexedCondition(child->p0C.sh[0]) == 0) {
                    continue;
                }
            } else {
                if (group->type == 16 || group->type == 17 || group->type == 12) {
                    if (evtViewerTestIndexedCondition(child->p14.sh[0]) == 0) {
                        continue;
                    }
                }
            }
            if (time < child->frame + offset) {
                break;
            }
        }

        selected = child != NULL ? child->prev : group->lastChild;
        if (group->type == 2 || group->type == 16 ||
            group->type == 17 || group->type == 12) {
            switch (group->type) {
            case 2:
                conditionIndex = 1;
                break;
            case 12:
            case 16:
            case 17:
                conditionIndex = 3;
                break;
            }
            while (selected != NULL) {
                s16 condition;

                condition = (s16)selected->body.words[conditionIndex];
                if (evtViewerTestIndexedCondition(condition) == 1) {
                    break;
                }
                selected = selected->prev;
            }
        }
        evtViewerUpdateTimelineGroupState(group, child, selected, time, &bestDistance, viewer, offset);
    }

    if (D_003BBE80 != NULL) {
        viewer->fallbackEntry = (s32)D_003BBE80->info;
        evtViewerApplySelectedEntry(viewer);
    }
    evtEventViewerUpdateUnitTransitions(time, viewer);
    if (viewer->flags & 1) {
        if (!(viewer->flags & 2)) {
            kwlnCancelConfiguredFadeFrames();
        }
        if (!(viewer->flags & 4)) {
            kwlnDrawOverlayEnabled = 0;
        }
    }
}



/* Clamps each movie object's playback interval using its linked track's first
 * key frame (or track offset) and the supplied endTime. */
void evtViewerClampMovieTimes(s32 endTime, EvtRuntime *viewer) {
    s32 worldTable;
    s32 worldSlots;
    s32 movieObject;
    EvtRuntimeGroup *trackEntry;
    s32 startFrame;

    if (dds3GetWorldObject() != 0) {
        worldTable = (s32)((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data);
        if (worldTable != 0) {
            worldSlots = (s32)((EvtWorldTable *)worldTable)->slots;
            if (worldSlots != 0) {
                movieObject = (s32)((EvtWorldSlot *)worldSlots)[EVT_WORLD_SLOT_MOVIE].head;
                if (movieObject != 0) {
                    do {
                        trackEntry = viewer->groups;
                        while (trackEntry != NULL) {
                            if (trackEntry->info != NULL && (void *)movieObject == dds3GetSlot(trackEntry->info, 1)) {
                                if (trackEntry->type == 2) {
                                    startFrame = 0;
                                    if (trackEntry->childCount != 0) {
                                        startFrame = trackEntry->children->frame;
                                    }
                                } else {
                                    startFrame = trackEntry->metadata.value;
                                }
                                evtPolygonMovieClampTime((PolyMovieObject *)movieObject, 0, startFrame, endTime);
                                break;
                            }
                            trackEntry = trackEntry->next;
                        }
                        movieObject = (s32)((EffWorldNode *)movieObject)->next;
                    } while (movieObject != 0);
                }
            }
        }
    }
}

extern Motion *mdlFindNodeById(MdlCtx *ctx, s32 id);
extern EvtRuntimeChild *evtViewerFindLatestMatchingMotionKey(EvtRuntimeGroup *group, s32 frame, s32 channel);

/* Synchronize four motion channels, retaining their last applied timeline key. */
void evtViewerSyncModelMotionChannels(s32 frame, EffWorldNode *object, EvtRuntimeGroup *track,
                   EvtRuntime *viewer, s32 mode) {
    EffectObjectData *data = object->data;
    MdlCtx *model = (MdlCtx *)data->modelHolder->resourceHandle;
    s32 channel;

    if (model->first == NULL) {
        return;
    }
    for (channel = 0; channel < 4; channel++) {
        Motion *motion = mdlFindNodeById(model, channel);
        EvtRuntimeChild *key;
        s32 sampleFrame;

        if (motion == NULL) {
            continue;
        }
        key = evtViewerFindLatestMatchingMotionKey(track, frame, channel);
        if (key == NULL) {
            if (mode == 0 && track->motionKeyCache[channel] == key) {
                continue;
            }
            motion = mdlFindNodeById(model, channel);
            sdfMotionInitialize(motion, 0, 0, 0.0f, 0.0f);
            if (mdlGetNodeFrameCount(model, channel) != 0) {
                if ((s32)mdlGetNodeFrameCount(model, channel) - 1 < frame) {
                    sampleFrame = (s32)mdlGetNodeFrameCount(model, channel) - 1;
                } else {
                    sampleFrame = frame;
                }
            } else {
                sampleFrame = 0;
            }
            if (channel == 0) {
                motion = mdlFindNodeById(model, 0);
                sdfMotionSampleAtFrame(motion, (f32)sampleFrame);
            } else {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSampleAtFrame(motion, 0.0f);
            }
            if ((viewer->flags & 1) != 0) {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSuspend(motion);
            } else {
                motion = mdlFindNodeById(model, channel);
                sdfMotionResume(motion);
            }
            track->motionKeyCache[channel] = NULL;
            if (channel != 0) {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSuspend(motion);
            }
            continue;
        }

        {
            s32 loopEnabled;
            s32 nodeFrameCount;

            if (mode == 0 && track->motionKeyCache[channel] == key) {
                continue;
            }
            loopEnabled = key->p0C.sb[2] == 0;
            motion = mdlFindNodeById(model, channel);
            sdfMotionInitialize(motion, key->p0C.sb[1], loopEnabled,
                                0.0f, (f32)key->p0C.sb[3]);
            if (mode == 1) {
                if (loopEnabled == mode) {
                    nodeFrameCount = (s32)mdlGetNodeFrameCount(model, channel);
                    if (nodeFrameCount != 0) {
                        nodeFrameCount = (s32)mdlGetNodeFrameCount(model, channel);
                        sampleFrame = (frame - key->frame) % nodeFrameCount;
                    } else {
                        sampleFrame = 0;
                    }
                } else {
                    s32 relativeFrame = frame - key->frame;
                    nodeFrameCount = (s32)mdlGetNodeFrameCount(model, channel);
                    if (nodeFrameCount < relativeFrame) {
                        sampleFrame = (s32)mdlGetNodeFrameCount(model, channel);
                    } else {
                        sampleFrame = frame - key->frame;
                    }
                }
                motion = mdlFindNodeById(model, channel);
                sdfMotionSampleAtFrame(motion, (f32)sampleFrame);
            }
            if ((viewer->flags & 1) != 0) {
                motion = mdlFindNodeById(model, channel);
                sdfMotionSuspend(motion);
            } else {
                motion = mdlFindNodeById(model, channel);
                sdfMotionResume(motion);
            }
            track->motionKeyCache[channel] = key;
        }
    }
}


/* Updates world units at position using the first track whose owner word
 * matches that unit's object-chain node. */
void evtViewerSyncWorldGroups(s32 position, EvtRuntime *viewer) {
    s32 unitObject;
    EvtRuntimeGroup *node;
    EvtRuntimeGroup *found;

    if (dds3GetWorldObject() != 0) {
        unitObject = (s32)((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data)->slots[EVT_WORLD_SLOT_UNIT].head;
        if (unitObject != 0) {
            do {
                node = viewer->groups;
                found = NULL;
                while (node != NULL) {
                    if ((s32)node->info == unitObject) {
                        found = node;
                        break;
                    }
                    node = node->next;
                }
                if (found != NULL) {
                    evtViewerSyncModelMotionChannels(position, (EffWorldNode *)unitObject, node, viewer, 0);
                }
                unitObject = (s32)((EffWorldNode *)unitObject)->next;
            } while (unitObject != 0);
        }
    }
}

extern u32 sdfGetLodChunkValue(SdfModel *model);

/* Applies the latest kind-6 key at/before position to a kind-1 track's LOD
 * byte, provided the requested signed-byte level is supported by its chunk. */
void evtViewerApplyGlyphLodChannel(s32 position, EvtRuntime *viewer) {
    EvtRuntimeGroup *node = viewer->groups;
    EvtRuntimeChild *glyph;
    EvtRuntimeChild *best;
    s32 bestFrame;
    SdfModel *lod;
    s8 level;

    while (node != NULL) {
        if (node->type == 1) {
            glyph = node->children;
            bestFrame = -1;
            best = NULL;
            if (glyph != NULL) {
                do {
                    if (position >= glyph->frame && bestFrame < glyph->frame && glyph->p08.sb[0] == 6) {
                        bestFrame = glyph->frame;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            lod = ((MdlCtx *)((EffectObjectData *)node->info->data)->modelHolder->resourceHandle)->inner;
            if (best == NULL) {
                lod->lodIndex = 0;
            } else {
                level = best->p0C.sb[0];
                if ((s32)sdfGetLodChunkValue(lod) >= level) {
                    lod->lodIndex = best->p0C.sb[0];
                }
            }
        }
        node = node->next;
    }
}



/* At an exact kind-7 key frame, attaches the indexed world object. Index -1
 * restores the saved vectors; losing the active key restores them once too. */
void evtEventViewerAttachIndexedObject(s32 position, EvtRuntime *viewer) {
    EvtRuntimeGroup *node = viewer->groups;
    EvtRuntimeChild *glyph;
    EvtRuntimeChild *best;
    s32 bestFrame;

    while (node != NULL) {
        if (node->type == 1) {
            glyph = node->children;
            bestFrame = -1;
            best = NULL;
            if (glyph != NULL) {
                do {
                    if (glyph->p08.sb[0] == 7 && position >= glyph->frame &&
                        bestFrame < glyph->frame) {
                        bestFrame = glyph->frame;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            if (best == NULL) {
                if (node->objectAttached == 1) {
                    effObjSetInnerPosition(node->info,
                        (u128 *)node->savedPosition);
                    effObjSetInnerRotation(node->info,
                        (u128 *)node->savedRotation);
                    effObjFetchInnerPosition(node->info);
                    VU0_STORE_VF(vf10, &node->info->inner->smoothedPosition);
                    node->objectAttached = 0;
                }
            } else if (best->frame == position) {
                s16 channel = best->p0C.sh[0];

                if (channel == -1) {
                    effObjSetInnerPosition(node->info,
                        (u128 *)node->savedPosition);
                    effObjSetInnerRotation(node->info,
                        (u128 *)node->savedRotation);
                    effObjFetchInnerPosition(node->info);
                    VU0_STORE_VF(vf10, &node->info->inner->smoothedPosition);
                    node->objectAttached = 0;
                } else {
                    EffWorldNode *object = dds3FindObjectChainNodeByName(
                        dds3GetWorldObject(), viewer->entryName[channel]);

                    mdlAttachWorldObjectToSourceVector(
                        node->info->key, object->key);
                    node->objectAttached = 1;
                }
            }
        }
        node = node->next;
    }
}

extern void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut);
void evtViewerMarkWindowActive(EvtRuntime *viewer);

/* Activates the message window thirty frames before a kind-4 key when its
 * unpacked entry is ready. */
void evtViewerActivateWindowForGlyphEntry(s32 timelinePosition, EvtRuntime *viewer) {
    s32 entryIndex;
    s32 entryMode;
    EvtRuntimeGroup *node;
    EvtRuntimeChild *glyph;

    if (viewer->windowContext != 0) {
        if (viewer->windowContext->handle != -1) {
            for (node = viewer->groups; node != NULL; node = node->next) {
                if (node->type != 4) {
                    continue;
                }
                for (glyph = node->children; glyph != NULL; glyph = glyph->next) {
                    if (glyph->frame - 30 != timelinePosition) {
                        continue;
                    }
                    mnuUnpackNibbleFields(glyph, &entryIndex, &entryMode);
                    if (itfMesGetWindowEntryItems(
                            viewer->windowContext->handle, entryIndex) != 1) {
                        continue;
                    }
                    evtViewerMarkWindowActive(viewer);
                    break;
                }
                break;
            }
        }
    }
}

void evtViewerCountFlaggedUpdates(EvtRuntime *viewer) {
    s64 active;

    active = evtViewerHasUpdateFlag(viewer);
    if (active != 0) {
        viewer->updateCount = viewer->updateCount + 1;
    }
}

extern s32 campAnyPackedFlagSet(void *scene);
extern s32 scrCommandIsProcessControlFlagClear(void);
extern u32 mnuCampGetSecondaryOption(void *scene);
extern void mnuReleaseCampSceneRegisteredIds(EvtRuntime *viewer);
extern s32 mnuQueryTitleSoundBusy(void);
extern void mnuStopTitleVoicePlayback(void);
extern void sdfSoundSetChannelCount(u32 channels);
extern s32 fldTitleIsActive(void);
void evtViewerCleanupMessageWindow(EvtRuntime *viewer);

void evtEventViewerStartEndCountdown(EvtRuntime *viewer) {
    if (campAnyPackedFlagSet(viewer) == 1 || scrCommandIsProcessControlFlagClear() == 0) {
        if (viewer->glyphTickCount == 0) {
            viewer->glyphTickCount = 1;
        }
        return;
    }
    if (viewer->curFrame < viewer->frameRange.word - 30) {
        if (viewer->headerFirst + 20 >= viewer->curFrame) {
            return;
        }
    } else {
        return;
    }

    viewer->flags |= 0x10;
    if (mnuCampGetSecondaryOption(viewer) == 2) {
        viewer->updateCount = 0x17;
    } else {
        viewer->updateCount = 0;
    }
    mnuReleaseCampSceneRegisteredIds(viewer);
    if (mnuQueryTitleSoundBusy() == 1) {
        mnuStopTitleVoicePlayback();
    } else if (viewer->timedActive == 1) {
        sdfSoundSetChannelCount(10);
    }
    if (fldTitleIsActive() == 1) {
        fldSetTitleDrawMode(0);
    }
    evtViewerCleanupMessageWindow(viewer);
}

/* Test the viewer's update flag. */
s32 evtViewerHasUpdateFlag(EvtRuntime *viewer) {
    return ((s32)viewer->flags & 0x10) > 0;
}

/* Fades the viewer's background colour over its update countdown. */
void evtEventViewerFadeBackground(EvtRuntime *viewer) {
    s32 fade = 0;

    kwlnGetDrawBufferIndex();
    if (viewer->updateCount != 0x19) {
        fade = (s32)(128.0f - ((f32)viewer->backgroundFadeStartAlpha +
                               ((128.0f - (f32)viewer->backgroundFadeStartAlpha) / 25.0f) * (f32)viewer->updateCount));
    }
    if (fade < 0) {
        fade = 0;
    }
    if (fade < 0x80 && evtViewerHasUpdateFlag(viewer) != 0) {
        s32 option = (s32)mnuCampGetSecondaryOption(viewer);

        switch (option) {
        case 0:
            D_003BBE88[0] = D_003BBE88[1] = D_003BBE88[2] = 0;
            break;
        case 1:
            D_003BBE88[0] = D_003BBE88[1] = D_003BBE88[2] = 0xFF;
            break;
        case 2:
            return;
        }
        kwlnFadeSetColor(D_003BBE88[0], D_003BBE88[1], D_003BBE88[2], 0x80 - fade);
    }
}

/* Applies the effect-channel assignments driven by the viewer's tracks. */
void evtEventViewerApplyEffectChannels(s32 frame, EvtRuntime *viewer) {
    EvtRuntimeGroup *track = viewer->groups;

    while (track != NULL) {
        if ((u32)(track->type - 0xE) < 2 || track->type == 0x17 || track->type == 0x11) {
            EvtRuntimeChild *key = track->children;
            SdfTex *value = 0;

            while (key != NULL) {
                if (track->type != 0x11 || evtViewerTestIndexedCondition(key->p14.sh[0]) != 0) {
                    if (frame < key->frame + track->metadata.value) {
                        break;
                    }
                    if (key->p10.sh[0] != 0) {
                        value = 0;
                        if (key->p10.sh[0] != 1) {
                            value = ((EvtRuntimeGroup *)mnuCampFindEntryByName(
                                         viewer, (char *)viewer->entryName[key->p10.sh[0] - 2]))->texture;
                        }
                    }
                }
                key = key->next;
            }

            switch (track->type) {
            case 0xE:
                if (viewer->ch71 != value) {
                    viewer->ch71 = value;
                    if (value == 0) {
                        effInitCh71Id();
                    } else {
                        effSetCh71Id(value);
                    }
                }
                break;
            case 0xF:
                if (viewer->ch72 != value) {
                    viewer->ch72 = value;
                    if (value == 0) {
                        effInitCh72Id();
                    } else {
                        effSetCh72Id(value);
                    }
                }
                break;
            case 0x17:
                if (viewer->ch76 != value) {
                    viewer->ch76 = value;
                    if (value == 0) {
                        effInitCh76Id();
                    } else {
                        effSetCh76Id(value);
                    }
                }
                break;
            case 0x11:
                if (viewer->ch75 != value) {
                    viewer->ch75 = value;
                    if (value == 0) {
                        effInitCh75Id();
                    } else {
                        effSetCh75Id(value);
                    }
                }
                break;
            }
        }
        track = track->next;
    }
}

/* Advance or stop the timed viewer action according to the current position. */
s32 evtViewerUpdateTimedAction(EvtRuntime *viewer) {
    if (viewer->timedActive == 1) {
        if (viewer->curFrame < viewer->timedStart) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        } else if (viewer->timedEnd == -1) {
            mnuCheckMovieDecoderStatus();
        } else if (viewer->curFrame >= viewer->timedEnd) {
            mnuStopMovieDrawTask();
            viewer->timedActive = 0;
            viewer->timedStart = 0;
            viewer->timedEnd = 0;
        }
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerUpdateShopFlagTrack);

void evtViewerPreGlyphTickHookNoOp(EvtRuntime *viewer) {
}

/* Tick the current glyph while text is advancing; wrap after thirty ticks. */
void evtViewerAdvanceGlyphTick(EvtRuntime *viewer) {
    s32 nextTick;

    if ((viewer->curFrame < viewer->frameRange.word - 3) && (0 < viewer->glyphTickCount))
    {
        frFontDrawGlyphInDefaultMode(viewer->glyph);
        nextTick = viewer->glyphTickCount + 1;
        viewer->glyphTickCount = nextTick;
        if (0x1d < nextTick) {
            viewer->glyphTickCount = 0;
        }
    }
}

void evtViewerFlag8ClearFrameHookNoOp(EvtRuntime *viewer) {
}

s32 evtViewerTestIndexedCondition(u32 condition);

/* Returns the latest eligible kind-5 motion key at/before frame for channel,
 * or NULL. Its condition must pass; equal-frame ties retain the first key. */
EvtRuntimeChild *evtViewerFindLatestMatchingMotionKey(EvtRuntimeGroup *group, s32 frame, s32 channel) {
    s32 bestFrame = -1;
    EvtRuntimeChild *best = NULL;
    EvtRuntimeChild *key = group->children;

    if (key != NULL) {
        do {
            if (frame >= key->frame && bestFrame < key->frame && key->p08.sb[0] == 5 &&
                key->p0C.sb[0] == channel && evtViewerTestIndexedCondition(key->p10.sh[0]) == 1) {
                bestFrame = key->frame;
                best = key;
            }
            key = key->next;
        } while (key != NULL);
    }
    return best;
}


/* Apply the viewer playback mode to unit, motion and movie-object tracks. */
void evtEventViewerApplyPlaybackMode(s32 mode, u32 frame, EvtRuntime *viewer) {
    EvtWorldTable *table;
    EffWorldNode *object;
    EvtRuntimeGroup *track;
    EvtRuntimeGroup *found;
    EvtRuntimeChild *key;
    Motion *motion;
    struct EvtScaledValue *counter;
    void *movie;
    s32 useTrackTime;

    if (dds3GetWorldObject() == NULL) {
        return;
    }
    table = ((EvtWorldTable *)((EffWorldNode *)dds3GetWorldObject())->data);
    object = table->slots[5].head;
    while (object != NULL) {
        track = viewer->groups;
        found = NULL;
        while (track != NULL) {
            if (track->info == object) {
                found = track;
                break;
            }
            track = track->next;
        }
        if (found != NULL) {
            evtViewerSyncModelMotionChannels(frame, object, track, viewer, 1);
        }
        object = object->next;
    }
    object = table->slots[6].head;
    while (object != NULL) {
        motion = ((EvtViewerPlaybackData *)object->data)->object->motion;
        if (motion != NULL) {
            if (mode == 0) {
                sdfMotionSampleAtFrame(motion, (f32)frame);
                sdfMotionSuspend(motion);
            } else {
                sdfMotionResume(motion);
            }
        }
        object = object->next;
    }
    object = table->slots[3].head;
    while (object != NULL) {
        counter = ((EvtViewerPlaybackData *)object->data)->counter;
        if (counter != NULL) {
            if (mode == 0) {
                sdfFreezeFloatCounter(counter);
            } else {
                sdfUnfreezeFloatCounter(counter);
            }
        }
        object = object->next;
    }
    track = viewer->groups;
    while (track != NULL) {
        switch (track->type) {
        case 3: case 18: case 20: case 21: case 26:
            key = track->children;
            useTrackTime = 0;
            while (key != NULL) {
                switch (track->type) {
                case 3: case 26:
                    useTrackTime = track->metadata.extra1;
                    /* These track kinds use the same indexed movie lookup. */
                case 20: case 21:
                    if (key->p08.sb[0] < 0) {
                        break;
                    }
                    object = viewer->objects[key->p08.sb[0]];
                    goto updateMovie;
                case 18:
                    object = key->payload;
                    useTrackTime = track->metadata.extra1;
updateMovie:
                    if (object != NULL) {
                        movie = dds3GetObjectOwnedHandle(object)->slots[1];
                        if (movie != NULL) {
                            if (useTrackTime == 0) {
                                evtPolygonMovieScaleByProgress(movie, mode, key->frame, frame);
                            } else {
                                evtPolygonMovieScaleByProgress(movie, mode, 0, frame);
                            }
                        }
                    }
                    break;
                }
                key = key->next;
            }
            break;
        }
        track = track->next;
    }
}


/* Dispatch one of two viewer modes based on its lowest flag bit. */
void evtViewerDispatchFlagMode(EvtRuntime *viewer) {
    if ((viewer->flags & 1) != 0) {
        evtEventViewerApplyPlaybackMode(0, viewer->curFrame, viewer);
        return;
    }
    evtEventViewerApplyPlaybackMode(1, viewer->curFrame, viewer);
}

/* Returns the address word of the nearest kind-2 key strictly after the current
 * frame, or zero. Equal-distance ties retain the first key visited. */
s32 evtViewFindNextGlyph(EvtRuntime *viewer) {
    EvtRuntimeChild *selectedGlyph = NULL;
    s32 bestFrameDistance = 99999;
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtRuntimeChild *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 glyphFrame = glyph->frame;

                    if (viewer->curFrame < glyphFrame) {
                        s32 frameDistance = glyphFrame - viewer->curFrame;

                        if (frameDistance < bestFrameDistance) {
                            bestFrameDistance = frameDistance;
                            selectedGlyph = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return (s32)selectedGlyph;
}

/* Returns the address word of the nearest kind-2 key strictly before the current
 * frame, or zero. Equal-distance ties retain the first key visited. */
s32 evtViewFindPrevGlyph(EvtRuntime *viewer) {
    EvtRuntimeChild *selectedGlyph = NULL;
    s32 bestFrameDistance = 99999;
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtRuntimeChild *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 glyphFrame = glyph->frame;

                    if (glyphFrame < viewer->curFrame) {
                        s32 frameDistance = viewer->curFrame - glyphFrame;

                        if (frameDistance < bestFrameDistance) {
                            bestFrameDistance = frameDistance;
                            selectedGlyph = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return (s32)selectedGlyph;
}

void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, EvtRuntime *viewer) {
    s32 historyIndex = viewer->historyCount + 1;

    viewer->actionMode = actionMode;
    viewer->historyCount = historyIndex;
    viewer->history[historyIndex].id = actionMode;
    viewer->history[historyIndex].a = first;
    viewer->history[historyIndex].b = second;
    if (actionMode > 0) {
        if (actionMode >= 3) {
            if (actionMode == 3) {
                viewer->charCol = 0;
                viewer->charRow = 0;
                viewer->nameStorage[0] = 0;
                viewer->nameStorage[0xC] = 0;
            }
        }
    }
}

u16 evtViewerPopHistory(EvtRuntime *viewer) {
    u16 historyId;
    s32 historyIndex;

    historyIndex = viewer->historyCount - 1;
    if (viewer->historyCount == 0) {
        viewer->actionMode = 0;
        return 0;
    }
    viewer->historyCount = historyIndex;
    historyId = viewer->history[historyIndex].id;
    viewer->actionMode = (u32)historyId;
    return historyId;
}

extern s16 itfPanelGetPairFirst(s32 windowIndex);
extern void itfPanelSetPairFirst(s32 windowIndex, s16 status);
extern void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock);
extern void itfMesResetWindow(s32 window);
void evtViewerMarkWindowInactive(EvtRuntime *);
void evtViewerUpdateWindowShadeFade(EvtRuntime *);

void evtUpdateViewerMessageWindow(EvtRuntime *viewer) {
    if (viewer->windowContext == 0) {
        return;
    }
    if (viewer->windowContext->handle == -1) {
        return;
    }
    if (itfMesGetWindowFlags(viewer->windowContext->handle) & 4) {
        if ((viewer->flags & 1) == 0) {
            itfMesCleanupWindow(viewer->windowContext->handle, 1);
            itfMesFinishWindowAndClearStatus(viewer->windowContext->handle);
        }
    }
    if (itfPanelGetPairFirst(viewer->windowContext->handle) < 0) {
        itfPanelSetPairFirst(viewer->windowContext->handle, 0);
        datGameState->script.ints[viewer->windowResultVariableIndex] = itfMesGetWindowClearBitCount(viewer->windowContext->handle);
        itfMesResetWindow(viewer->windowContext->handle);
        evtViewerMarkWindowInactive(viewer);
        viewer->flags ^= 1;
        evtViewerDispatchFlagMode(viewer);
    }
    evtViewerUpdateWindowShadeFade(viewer);
}


void evtViewerCleanupMessageWindow(EvtRuntime *viewer) {
    PolyMovieWork *windowContext;
    s32 window;

    windowContext = viewer->windowContext;
    if (windowContext == 0) {
        return;
    }
    window = windowContext->handle;
    if (window == -1) {
        return;
    }
    itfMesCleanupWindow(window, 1);
    windowContext = viewer->windowContext;
    itfMesFinishWindowAndClearStatus(windowContext->handle);
    windowContext = viewer->windowContext;
    itfPanelSetPairFirst(windowContext->handle, 0);
    windowContext = viewer->windowContext;
    itfMesResetWindow(windowContext->handle);
    viewer->windowActive = 0;
    viewer->windowShadeFade = 0;
}

void evtViewerMarkWindowActive(EvtRuntime *viewer) {
    viewer->windowActive = 1;
}

void evtViewerMarkWindowInactive(EvtRuntime *viewer) {
    viewer->windowActive = 0;
}

/* One A+D register write (value, then the GS register id). */
typedef struct EvtShadeRegister {
    u64 value;
    u64 registerId;
} EvtShadeRegister;

/* Per-draw-buffer window shade packet: setup registers, then a two-vertex
 * sprite covering the message window. */
typedef struct EvtWindowShadePacket {
    SdfDmaTagHeader tag;
    EvtShadeRegister regs[10];
} EvtWindowShadePacket;

extern EvtWindowShadePacket evtWindowShadePackets[];
extern u8 D_003BBE90[4];
extern SdfPoolNode D_00325448;

/* Ramp the message-window shade toward its target and, while visible, queue
 * the translucent shade sprite for the current draw buffer. */
void evtViewerUpdateWindowShadeFade(EvtRuntime *viewer) {
    u32 buffer;
    s32 alpha;

    if (viewer->windowActive == 0) {
        if (viewer->windowShadeFade - 9 > 0) {
            viewer->windowShadeFade -= 9;
        } else {
            viewer->windowShadeFade = 0;
        }
    } else {
        if (viewer->windowShadeFade + 3 < 0x5E) {
            viewer->windowShadeFade += 3;
        } else {
            viewer->windowShadeFade = 0x5E;
        }
    }
    buffer = kwlnGetDrawBufferIndex();
    alpha = 0x80 - viewer->windowShadeFade;
    if (alpha < 0x80) {
        u64 red;
        u64 blueQ;
        u64 color;
        SdfListHead *list;

        evtWindowShadePackets[buffer].tag.quadwordCount = 10;
        evtWindowShadePackets[buffer].tag.reservedByte = 0;
        evtWindowShadePackets[buffer].tag.control = 0x70;
        evtWindowShadePackets[buffer].tag.address = 0;
        evtWindowShadePackets[buffer].tag.firstVifCode = 0;
        evtWindowShadePackets[buffer].tag.secondVifCode = 0x5000000A;
        evtWindowShadePackets[buffer].regs[0].value = 0x1000000000000004ULL;
        evtWindowShadePackets[buffer].regs[0].registerId = 0xE;
        evtWindowShadePackets[buffer].regs[1].registerId = 0x42;
        evtWindowShadePackets[buffer].regs[1].value = 1;
        evtWindowShadePackets[buffer].regs[2].registerId = 0x49;
        evtWindowShadePackets[buffer].regs[2].value = 0;
        evtWindowShadePackets[buffer].regs[3].registerId = 0x4A;
        evtWindowShadePackets[buffer].regs[3].value = 0;
        evtWindowShadePackets[buffer].regs[4].registerId = 0x47;
        evtWindowShadePackets[buffer].regs[4].value = 0x31003;
        evtWindowShadePackets[buffer].regs[5].value = 0x1023400000008004ULL;
        evtWindowShadePackets[buffer].regs[5].registerId = 0xE;
        red = (alpha << 24) | D_003BBE90[0];
        blueQ = ((u64)D_003BBE90[2] << 16) | 0x3F80000000000000ULL;
        color = red | blueQ | ((u64)D_003BBE90[1] << 8);
        evtWindowShadePackets[buffer].regs[6].registerId = 1;
        evtWindowShadePackets[buffer].regs[6].value = color;
        evtWindowShadePackets[buffer].regs[7].registerId = 5;
        evtWindowShadePackets[buffer].regs[7].value = 0x00FEFFFF79007000ULL;
        evtWindowShadePackets[buffer].regs[8].registerId = 1;
        evtWindowShadePackets[buffer].regs[8].value = color;
        evtWindowShadePackets[buffer].regs[9].registerId = 5;
        evtWindowShadePackets[buffer].regs[9].value = 0x00FEFFFF87009000ULL;
        list = sdfCreateResetPacketList();
        sdfAppendPacket(list, (u32)&evtWindowShadePackets[buffer]);
        D_00325448.append(&D_00325448, list);
    }
}

s32 evtViewerTestIndexedCondition(u32 encodedCondition) {
    u32 conditionIndex;
    u32 expectedValue;

    conditionIndex = (encodedCondition << 16) >> 28;
    expectedValue = encodedCondition & 0xfff;
    if (conditionIndex == 0) {
        return 1;
    }
    return (datGameState->script.ints[199 + conditionIndex] ^ expectedValue) == 0;
}

u32 func_00230470(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD2B0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD2C0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD2D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD2E0);

s32 evtEventViewerOpenInputMenu(s32 arg0, s32 arg1, EvtRuntime *viewer) {
    switch ((u32)viewer->inputA) {
    case 0:
    case 1:
    case 2:
        viewer->charCol = 0;
        viewer->charRow = 0;
        evtViewerPushCommandHistory(9, 0xE4, 0x3C, viewer);
        break;
    case 5:
        viewer->value = viewer->headerThird - 1;
        viewer->valueMin = viewer->frameRange.word;
        viewer->valueMax = 10000;
        evtViewerPushCommandHistory(7, 0xB4, 0x78, viewer);
        break;
    case 3:
    case 4:
        viewer->cursor = 0;
        viewer->itemCount = 2;
        viewer->title = "FRAME SET.OK? ";
        viewer->itemNames = D_00368410;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    case 6:
        viewer->commandStart = 1;
        viewer->flags |= 1;
        evtViewerDispatchFlagMode(viewer);
        viewer->actionMode = 0;
        viewer->historyCount = 0;
        viewer->commandResetId = 0;
        D_003BBE94 = 0;
        return 1;
    case 7:
        viewer->cursor = mnuCampGetPrimaryOption(viewer);
        viewer->itemCount = 2;
        viewer->title = "START FRAME SELECT";
        viewer->itemNames = D_00368430;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    case 8:
        viewer->value = 0;
        viewer->valueMin = -5000;
        viewer->valueMax = 5000;
        evtViewerPushCommandHistory(7, 0xB4, 0x78, viewer);
        break;
    case 9:
        viewer->cursor = mnuCampGetPrimaryOption(viewer);
        viewer->itemCount = 2;
        viewer->title = "SET BISTAMODE";
        viewer->itemNames = D_00368418;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    case 10:
        viewer->cursor = mnuCampGetSecondaryOption(viewer);
        viewer->itemCount = 3;
        viewer->title = "SET SKIPMODE";
        viewer->itemNames = D_00368420;
        evtViewerPushCommandHistory(2, 0xE4, 0x3C, viewer);
        break;
    }
    return 0;
}


/* Commit viewer menu selections and apply typed edits to the current timeline key. */
s32 evtViewerApplyCommandSelection(s32 unused0, s32 mode, EvtRuntime *viewer) {
    s32 changedRange = 0;
    EvtRuntimeChild *key;

    switch (mode) {
    case 1:
        switch (viewer->inputA) {
        case 3:
            if (viewer->cursor == 0) {
                changedRange = 1;
                evtViewerSetMinimumFromCurrent(viewer);
            } else {
                evtViewerPopHistory(viewer);
            }
            break;
        case 4:
            if (viewer->cursor == 0) {
                changedRange = 1;
                evtViewerSetMaximumFromCurrent(viewer);
            } else {
                evtViewerPopHistory(viewer);
            }
            break;
        case 9:
            mnuCampSetPrimaryOption(viewer, viewer->cursor);
            evtViewerPopHistory(viewer);
            break;
        case 10:
            mnuCampSetSecondaryOption(viewer, viewer->cursor);
            evtViewerPopHistory(viewer);
            break;
        case 7:
            if (viewer->cursor == 0) {
                viewer->optionFrameBase = viewer->headerFirst;
            } else {
                viewer->optionFrameBase = viewer->curFrame;
            }
            viewer->valueMin = 1;
            viewer->value = 0x3C;
            viewer->valueMax = viewer->frameRange.word - viewer->optionFrameBase + 1;
            evtViewerPushCommandHistory(7, 0xB4, 0x78, viewer);
            break;
        }
        break;
    case 15:
        if (mnuGetSelectedTableValue(viewer) == 2) {
            viewer->frameGroup->metadata.extra1 = (s8)viewer->cursor;
            evtViewerPopHistory(viewer);
        }
        break;
    case 5:
        key = evtEventViewerGetPendingNode(viewer);
        switch (viewer->commandCategory) {
        case -1:
            if (viewer->cursor == 0) {
                evtEventViewerReleasePendingNode(viewer);
            } else if (viewer->cursor == 2) {
                evtEventViewerProcessPending(viewer);
            }
            break;
        case 5:
            switch (viewer->frameGroup->type) {
            case 10:
                key->serializedValue = viewer->cursor;
                break;
            case 8:
            case 25:
                key->p0C.h[0] = (u16)viewer->cursor;
                break;
            case 11: case 13: case 14: case 15: case 16: case 17: case 23: case 27:
                key->p08.h[0] = (u16)viewer->cursor;
                break;
            case 24:
                key->p08.sb[0] = (s8)viewer->cursor;
                break;
            }
            break;
        case 11:
            mode = viewer->frameGroup->type;
            switch (mode) {
            case 22:
                key->p08.h[0] = (u16)viewer->cursor;
                break;
            case 25:
                key->p08.h[0] = (u16)viewer->cursor;
                break;
            case 9:
                key->p08.h[1] = (u16)viewer->cursor;
                break;
            }
            break;
        case 16:
            mode = viewer->frameGroup->type;
            switch (mode) {
            case 20: case 21:
                key->p08.sb[1] = (s8)viewer->cursor;
                break;
            case 3: case 26:
                key->p0C.sb[0] = (s8)viewer->cursor;
                break;
            }
            break;
        case 12:
            switch (viewer->frameGroup->type) {
            case 12:
                key->p08.h[0] = (u16)viewer->cursor;
                viewer->unk2288 = 0;
                break;
            case 10:
                key->p14.h[0] = (u16)viewer->cursor;
                break;
            case 30:
                key->p08.h[1] = (u16)viewer->cursor;
                break;
            case 6: case 19: case 28: case 31: case 32:
                key->p08.h[0] = (u16)viewer->cursor;
                break;
            case 1:
                if (key->p08.sb[0] != viewer->cursor) {
                    switch (viewer->cursor) {
                    case 5:
                        key->p0C.sb[0] = 0;
                        key->p0C.sb[2] = 1;
                        key->p0C.sb[1] = 0;
                        key->p0C.sb[3] = 0;
                        key->p10.i = 0;
                        break;
                    case 7:
                        key->p10.i = 0;
                        key->p0C.h[0] = 0xFFFF;
                        break;
                    default:
                        key->p0C.i = 0;
                        key->p10.i = 0;
                        break;
                    }
                    key->p14.i = 0;
                }
                key->p08.sb[0] = (s8)viewer->cursor;
                break;
            }
            break;
        }
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
        evtViewerPopHistory(viewer);
        break;
    }
    return changedRange != 0 ? -1 : 0;
}


u32 evtViewerPushMode5CommandHistory(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    evtViewerPushCommandHistory(5, 0x90, 0x48, viewer);
    return 0;
}

/* Menu labels and callbacks used by the command editor. */
extern char *D_00368438[];
extern char *D_00368448[];
extern char *D_00368450[];
extern char *D_00368460[];
extern char *D_00368470[];
extern char *D_00368490[];
extern char *D_003684B0[];
extern char *D_003684B8[];
extern char *D_003684C0[];
extern char *D_003684C8[];
extern char *D_003684D0[];
extern char D_003BBF60[];
extern char D_003BBF68[];
extern char D_003BBF70[];
extern u8 D_003BD344;
extern s32 evtEventViewerEditDrawVectorTargetTask(KwlnTask *task);
extern s32 evtPollRuntimeControlReady(KwlnTask *task);
extern s32 evtSynchronizeSelectedEntry(KwlnTask *task);
extern void evtSetDrawVectorTarget(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD4B0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD4C0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD4D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD4E0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD4F0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD500);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD510);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD520);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD530);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD540);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD550);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD560);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003AD570);

s32 evtEventViewerHandleKeyEditCommand(s32 command, s32 unused, EvtRuntime *viewer) {
    EvtRuntimeChild *key;

    if (command == -4) {
        if (viewer->frameColumn == 0) {
            viewer->tableColumn = 0;
        } else {
            viewer->tableColumn = 1;
        }
        evtViewerPushCommandHistory(15, 144, 336, viewer);
        return 0;
    }
    {
        if (viewer->frameCursor + viewer->frameFirst < viewer->frameGroup->childCount) {
            if (command == -2) {
                viewer->commandCategory = -1;
                viewer->itemCount = 3;
                viewer->title = "DELETE FRAME.OK? ";
                viewer->itemNames = D_00368438;
                viewer->cursor = 0;
                evtViewerPushCommandHistory(2, 216, 132, viewer);
                return 0;
            }
            if (command != -3) {
                viewer->commandCategory = D_0036876A[viewer->frameColumn + viewer->frameGroup->type * 10];
                key = evtEventViewerGetPendingNode(viewer);
                switch ((u32)viewer->commandCategory) {
                case 0:
                    viewer->value = key->frame + viewer->frameGroup->metadata.value;
                    viewer->valueMin = viewer->frameGroup->metadata.value;
                    viewer->valueMax = viewer->headerThird - 1;
                    evtViewerPushCommandHistory(6, 180, 120, viewer);
                    break;
                case 9:
                    viewer->valueMin = 0;
                    viewer->valueMax = 10000;
                    switch (viewer->frameGroup->type) {
                    case 3:
                    case 20:
                    case 21:
                    case 26:
                        viewer->value = key->p08.sh[1];
                        evtViewerPushCommandHistory(6, 180, 120, viewer);
                        break;
                    case 18:
                        viewer->value = key->p08.sh[0];
                        evtViewerPushCommandHistory(6, 180, 120, viewer);
                        break;
                    }
                    break;
                case 15:
                    viewer->value = key->duration;
                    viewer->valueMin = 0;
                    viewer->valueMax = 10000;
                    evtViewerPushCommandHistory(6, 180, 120, viewer);
                    break;
                case 1:
                    viewer->value = key->duration;
                    viewer->valueMin = 0;
                    viewer->valueMax = 10000;
                    if (viewer->frameGroup->type == 10 && viewer->frameGroup->metadata.extra1 != 3) {
                        break;
                    }
                    evtViewerPushCommandHistory(7, 180, 120, viewer);
                    break;
                case 2:
                    if (viewer->frameGroup->type == 7) {
                        viewer->value = key->p08.sh[0];
                        viewer->valueMin = 0;
                        viewer->valueMax = 3000;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                    }
                    break;
                case 3: {
                    switch (viewer->frameGroup->type) {
                    case 8:
                        viewer->value = key->p08.sh[0];
                        viewer->valueMin = 0;
                        viewer->valueMax = 255;
                        break;
                    case 12:
                        viewer->value = key->p08.sh[1];
                        viewer->valueMin = -255;
                        viewer->valueMax = 255;
                        break;
                    case 31:
                        viewer->value = key->p08.sh[1];
                        viewer->valueMin = 0;
                        viewer->valueMax = 255;
                        break;
                    }
                    if (viewer->frameGroup->type == 31 && key->p08.sh[0] == 0) {
                        break;
                    }
                    evtViewerPushCommandHistory(7, 180, 120, viewer);
                    break;
                }
                case 4:
                    viewer->value = key->p08.sh[1];
                    viewer->valueMin = -255;
                    viewer->valueMax = 255;
                    evtViewerPushCommandHistory(7, 180, 120, viewer);
                    break;
                case 5:
                    switch (viewer->frameGroup->type) {
                    case 10: viewer->cursor = key->serializedValue; break;
                    case 11: case 13: case 14: case 15: case 16: case 17: case 23:
                        viewer->cursor = key->p08.sh[0]; break;
                    case 24: viewer->cursor = key->p08.sb[0]; break;
                    case 8: case 25: viewer->cursor = key->p0C.sh[0]; break;
                    }
                    viewer->title = "HOKAN MODE   ";
                    viewer->itemCount = 2;
                    viewer->itemNames = D_00368448;
                    evtViewerPushCommandHistory(2, 216, 120, viewer);
                    break;
                case 11:
                    switch (viewer->frameGroup->type) {
                    case 9:
                        viewer->cursor = key->p08.sh[1];
                        viewer->itemCount = 2;
                        viewer->title = "ON/OFF MODE ";
                        viewer->itemNames = D_00368448;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 22:
                        viewer->cursor = key->p08.sh[0];
                        viewer->itemCount = 2;
                        viewer->title = "ON/OFF MODE ";
                        viewer->itemNames = D_00368448;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 25:
                        viewer->cursor = key->p08.sh[0];
                        viewer->itemCount = 2;
                        viewer->title = "ON/OFF MODE ";
                        viewer->itemNames = D_00368448;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    }
                    break;
                case 16:
                    switch (viewer->frameGroup->type) {
                    case 20: case 21: viewer->cursor = key->p08.sb[1]; break;
                    case 3: case 26: viewer->cursor = key->p0C.sb[0]; break;
                    }
                    viewer->title = "ON/OFF MODE ";
                    viewer->itemCount = 2;
                    viewer->itemNames = D_00368448;
                    evtViewerPushCommandHistory(2, 216, 120, viewer);
                    break;
                case 12:
                    switch (viewer->frameGroup->type) {
                    case 12:
                        viewer->cursor = key->p08.sh[0];
                        viewer->itemCount = 3;
                        viewer->title = "TRANS MODE ";
                        viewer->itemNames = D_00368450;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 6:
                        viewer->cursor = key->p08.sh[0];
                        viewer->itemCount = 4;
                        viewer->title = "FADE TYPE  ";
                        viewer->itemNames = D_00368460;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 19:
                        viewer->cursor = key->p08.sh[0];
                        viewer->itemCount = 7;
                        viewer->title = "CONTROL TYPE ";
                        viewer->itemNames = D_00368470;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 1:
                        viewer->cursor = key->p08.sb[0];
                        viewer->itemCount = 8;
                        viewer->title = "UNIT CONTROL TYPE ";
                        viewer->itemNames = D_00368490;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 28:
                        viewer->cursor = key->p08.sb[0];
                        viewer->itemCount = 2;
                        viewer->title = "PADACT TYPE";
                        viewer->itemNames = D_003684B0;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 10:
                        viewer->cursor = key->p14.sh[0];
                        viewer->itemCount = 2;
                        viewer->title = "CONFIG DRAW STOP";
                        viewer->itemNames = D_003684B8;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 30:
                        viewer->cursor = key->p08.sh[1];
                        viewer->itemCount = 2;
                        viewer->title = "TIMEI FADE DOWN/UP";
                        viewer->itemNames = D_003684C0;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 31:
                        viewer->cursor = key->p08.sh[0];
                        viewer->itemCount = 2;
                        viewer->title = "SELECT MODE";
                        viewer->itemNames = D_003684C8;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    case 32:
                        viewer->cursor = key->p08.sh[0];
                        viewer->itemCount = 2;
                        viewer->title = "BISTA MODE";
                        viewer->itemNames = D_003684D0;
                        evtViewerPushCommandHistory(2, 216, 120, viewer);
                        break;
                    }
                    break;
                case 6:
                    viewer->floatValue = key->p08.f * 57.2957763671875f;
                    viewer->floatMin = 1.0f;
                    viewer->floatMax = 180.0f;
                    evtViewerPushCommandHistory(8, 180, 120, viewer);
                    break;
                case 8:
                    switch (viewer->frameGroup->type) {
                    case 5:
                        viewer->value = key->p08.sh[0];
                        viewer->valueMin = 0;
                        viewer->valueMax = 255;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 19:
                        viewer->value = key->p08.sh[1];
                        if (key->p08.sh[0] == 3) {
                            viewer->valueMin = -1;
                        } else {
                            viewer->valueMin = 0;
                        }
                        viewer->valueMax = 255;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 3:
                    case 26:
                        viewer->value = key->p0C.sb[1];
                        viewer->valueMin = -1;
                        viewer->valueMax = 127;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 12:
                        viewer->value = key->p0C.i;
                        viewer->valueMin = 0;
                        viewer->valueMax = 255;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 18:
                        viewer->value = key->p0C.sb[0];
                        viewer->valueMin = 0;
                        viewer->valueMax = 2;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 4:
                        viewer->value = key->p08.sh[0];
                        viewer->valueMin = 0;
                        viewer->valueMax = 500;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 28:
                        viewer->value = key->p08.sh[1];
                        viewer->valueMin = 0;
                        viewer->valueMax = 300;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 29:
                        viewer->value = key->p08.sh[0];
                        viewer->valueMin = 0;
                        viewer->valueMax = 80;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    case 30:
                        viewer->value = key->p08.sh[0];
                        viewer->valueMin = 0;
                        viewer->valueMax = 60;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    }
                    break;
                case 7:
                    viewer->value = key->p08.sh[1];
                    viewer->valueMin = 0;
                    viewer->valueMax = 127;
                    evtViewerPushCommandHistory(7, 180, 120, viewer);
                    break;
                case 13: {
                    switch (viewer->frameGroup->type) {
                    case 20:
                    case 21:
                        viewer->inputB = 0;
                        evtViewerPushCommandHistory(12, 120, 84, viewer);
                        break;
                    case 10: {
                        f32 *parameters = key->payload;
                        s32 i;
                        viewer->unk2328 = 0;
                        viewer->unk232C = 0;
                        for (i = 0; i < 4; i++) {
                            viewer->commandMatrix[i] = parameters[i];
                            viewer->commandMatrix[i + 4] = parameters[i + 4];
                            viewer->commandMatrix[i + 8] = parameters[i + 8];
                        }
                        for (i = 0; i < 4; i++) {
                            viewer->savedCommandMatrix[i] = parameters[i];
                            viewer->savedCommandMatrix[i + 4] = parameters[i + 4];
                            viewer->savedCommandMatrix[i + 8] = parameters[i + 8];
                        }
                        evtViewerPushCommandHistory(18, 348, 0, viewer);
                        break;
                    }
                    case 11: {
                        EvtViewerDrawPayload *parameters = key->payload;
                        evtSetDrawVectorTarget(0, parameters->y, parameters->x, parameters->z, parameters->w);
                        kwlnDrawVector.fogColor = parameters->mode;
                        viewer->controlState = kwlnTaskCreate(D_003BBF60, 0x2AF9, 1, 1, evtEventViewerEditDrawVectorTargetTask, NULL, 1);
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    }
                    case 13:
                        effSetLoadDescA(key->payload);
                        viewer->blurRectangleEnabled = 0;
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 14:
                        effEventSetBlurTemplateParameters(key->payload);
                        viewer->texturedBlurEnabled = 0;
                        campResolvePendingValue(viewer, key);
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 15:
                        effEventSetScatterBlurParameters(key->payload);
                        viewer->filterBlurEnabled = 0;
                        campResolvePendingValue(viewer, key);
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 23:
                        effEventSetScaleBlurParameters(key->payload);
                        viewer->staggeredBlurEnabled = 0;
                        campResolvePendingValue(viewer, key);
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 27:
                        effSetLoadDescD(key->payload);
                        viewer->framebufferQuadEnabled = 0;
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 16:
                        effEventSetSolidRectangleParameters(key->payload);
                        viewer->colorRectangleEnabled = 0;
                        viewer->controlState = kwlnTaskCreate(D_003BBF68, 0x2AF9, 1, 1, evtPollRuntimeControlReady, NULL, (u32)viewer);
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 17:
                        effEventSetResourceTemplateParameters(key->payload);
                        viewer->texturedSquareEnabled = 0;
                        campResolvePendingValue(viewer, key);
                        viewer->controlState = kwlnTaskCreate(D_003BBF70, 0x2AF9, 1, 1, evtSynchronizeSelectedEntry, NULL, (u32)viewer);
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 24:
                        viewer->editField = 0;
                        viewer->frameGroup->usePendingKey = 1;
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    case 25:
                        viewer->unk2404 = 0;
                        viewer->colorEditorActive = 1;
                        viewer->colorSelection = 0;
                        evtViewerPushCommandHistory(11, 0, 0, viewer);
                        break;
                    }
                    break;
                }
                case 14:
                    viewer->floatSelection = 0;
                    viewer->floatEditMode = 0;
                    viewer->floatEditX = key->p08.f;
                    viewer->floatEditY = key->p0C.f;
                    viewer->savedFloatEditX = key->p08.f;
                    viewer->savedFloatEditY = key->p0C.f;
                    viewer->savedOverlayFlag = D_003BD344;
                    evtViewerPushCommandHistory(19, 336, -12, viewer);
                    break;
                case 17: {
                    s32 value = key->p08.sh[0];
                    viewer->messageField = 0;
                    viewer->value = value;
                    viewer->valueMin = 0;
                    viewer->valueMax = itfMesGetEntryCount(viewer->windowContext->handle) - 1;
                    evtViewerPushCommandHistory(13, 180, 120, viewer);
                    break;
                }
                case 18: {
                    s32 unavailable = 0;
                    switch (viewer->frameGroup->type) {
                    case 4:
                        viewer->value = key->p08.sh[1];
                        if ((key->p08.h[0] >> 12) != 0) {
                            unavailable = 1;
                        }
                        break;
                    case 2:
                        viewer->value = key->p0C.sh[0];
                        break;
                    case 1:
                        if (key->p08.sb[0] >= 6) {
                            unavailable = 1;
                        } else if (key->p08.sb[0] < 0) {
                            unavailable = 1;
                        } else {
                            viewer->value = key->p10.sh[0];
                        }
                        break;
                    case 12: case 16: case 17:
                        viewer->value = key->p14.sh[0];
                        break;
                    default:
                        unavailable = 1;
                        break;
                    }
                    if (!unavailable) {
                        viewer->compareField = 0;
                        viewer->valueMax = 15;
                        viewer->valueMin = 0;
                        evtViewerPushCommandHistory(14, 180, 120, viewer);
                    }
                    break;
                }
                case 10:
                    viewer->groupFirst = 0;
                    viewer->groupCursor = 0;
                    evtViewerPushCommandHistory(10, 156, 84, viewer);
                    break;
                case 19:
                    switch (key->p08.sb[0]) {
                    case 5:
                        viewer->value = key->p0C.i;
                        viewer->valueMax = 1000;
                        viewer->fieldIndex = 0;
                        viewer->valueMin = 0;
                        evtViewerPushCommandHistory(16, 180, 120, viewer);
                        break;
                    case 6: {
                        u32 maximum = sdfGetLodChunkValue(((MdlCtx *)((EffectObjectData *)viewer->frameGroup->info->data)->modelHolder->resourceHandle)->inner);
                        s32 value = key->p0C.sb[0];
                        viewer->valueMax = maximum;
                        viewer->value = value;
                        viewer->valueMin = 0;
                        evtViewerPushCommandHistory(7, 180, 120, viewer);
                        break;
                    }
                    case 7:
                        viewer->groupFirst = 0;
                        viewer->groupCursor = 0;
                        evtViewerPushCommandHistory(10, 156, 84, viewer);
                        break;
                    case 3:
                        viewer->groupFirst = 0;
                        viewer->groupCursor = 0;
                        viewer->shadowMode = key->p0C.b[0];
                        viewer->shadowAlpha = key->p0C.b[1];
                        viewer->shadowY = key->p14.f;
                        evtViewerPushCommandHistory(22, 216, 180, viewer);
                        break;
                    case 4:
                        viewer->groupFirst = 0;
                        viewer->groupCursor = 0;
                        viewer->shadowMode = key->p0C.b[0];
                        viewer->shadowAlpha = key->p0C.b[1];
                        evtViewerPushCommandHistory(22, 216, 180, viewer);
                        break;
                    }
                    break;
                case 20:
                    viewer->groupFirst = 0;
                    viewer->groupCursor = 0;
                    evtViewerPushCommandHistory(21, 180, 120, viewer);
                    break;
                case 21:
                    break;
                }
            }
        } else if (command == 1) {
            evtEventViewerCreateTrackKeyAtFrame(viewer->frameGroup, viewer->curFrame, viewer);
        }
    }
    return 0;
}

/* Store the edited timing or selector halfword in the pending timeline key. */
s32 evtViewerStoreKeyTimingOrSelector(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *key = evtEventViewerGetPendingNode(viewer);
    EvtRuntimeGroup *track;
    s32 kind;
    s8 category;

    if (key != NULL) {
        track = viewer->frameGroup;
        kind = track->type;
        category = D_0036876A[viewer->frameColumn + kind * 10];
        switch (category) {
        case 0:
            key->frame = viewer->value - track->metadata.value;
            evtReorderListNodes(track);
            evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
            break;
        case 9:
            switch (kind) {
            case 3:
            case 20:
            case 21:
            case 26:
                key->p08.sh[1] = viewer->value;
                break;
            case 18:
                key->p08.sh[0] = viewer->value;
                break;
            }
            break;
        case 15:
            key->duration = viewer->value;
            break;
        }
        evtViewerPopHistory(viewer);
    }
    return 0;
}

extern void mnuFxWorldScrollDelta();
extern void mnuAdjustTimelineDurations(EvtRuntime *world, s32 threshold, s32 delta);

/* Store the edited menu value: the option/frame commands commit directly, and
 * otherwise the value goes into the pending key field chosen by the column's
 * command category and the track kind. */
s32 evtViewerStoreMenuValueInPendingKey(s32 unused, s32 command, EvtRuntime *viewer) {
    s32 value = viewer->value;
    f32 floatValue = viewer->floatValue;
    EvtRuntimeChild *key;

    if (viewer->commandResetId == 0) {
        switch (viewer->inputA) {
        case 5:
            viewer->headerThird = value + 1;
            evtViewerPopHistory(viewer);
            return 0;
        case 8:
            mnuFxWorldScrollDelta(viewer, value, viewer->curFrame);
            mnuAdjustTimelineDurations(viewer, viewer->curFrame, value);
            evtViewerPopHistory(viewer);
            return 0;
        case 7:
            viewer->commandStart = value;
            viewer->flags |= 1;
            viewer->curFrame = viewer->optionFrameBase;
            evtViewerDispatchFlagMode(viewer);
            viewer->actionMode = 0;
            viewer->historyCount = 0;
            viewer->commandResetId = 0;
            D_003BBE94 = 0;
            return 1;
        }
    } else if (viewer->commandResetId == 1 && command == 0xF) {
        viewer->frameGroup->metadata.value = value;
        evtViewerPopHistory(viewer);
        return 0;
    }
    key = evtEventViewerGetPendingNode(viewer);
    if (key == NULL) {
        return 0;
    }
    switch (D_0036876A[viewer->frameColumn + viewer->frameGroup->type * 10]) {
    case 1:
        key->duration = value;
        break;
    case 2:
        if (viewer->frameGroup->type == 7) {
            key->p08.sh[0] = value;
        }
        break;
    case 3:
        switch (viewer->frameGroup->type) {
        case 8:
            key->p08.sh[0] = value;
            break;
        case 12:
            key->p08.sh[1] = value;
            break;
        case 31:
            key->p08.sh[1] = value;
            break;
        }
        break;
    case 4:
        key->p08.sh[1] = value;
        break;
    case 7:
        key->p08.sh[1] = value;
        break;
    case 6:
        key->p08.f = floatValue * 0x1.1df46ap-6f;
        break;
    case 8:
        switch (viewer->frameGroup->type) {
        case 3:
            key->p0C.sb[1] = value;
            break;
        case 26:
            key->p0C.sb[1] = value;
            break;
        case 4:
            key->p08.sh[0] = value;
            break;
        case 5:
            key->p08.sh[0] = value;
            break;
        case 29:
            key->p08.sh[0] = value;
            break;
        case 30:
            key->p08.sh[0] = value;
            break;
        case 12:
            key->p0C.i = value;
            break;
        case 18:
            key->p0C.sb[0] = value;
            break;
        case 19:
            key->p08.sh[1] = value;
            break;
        case 28:
            key->p08.sh[1] = value;
            break;
        }
        break;
    case 19:
        if (key->p08.sb[0] == 6) {
            key->p0C.sb[0] = value;
        }
        break;
    case 20:
    case 21:
        break;
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    s32 value = viewer->value;
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

    if (entry == NULL) {
        return 0;
    }
    entry->p08.h[0] = value;
    if (((u32)(value << 16) >> 28) != 0) {
        entry->p08.h[1] = 0;
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Store the command value in the selected halfword of a viewer entry. */
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry;
    u32 value;
    s32 slot;

    value = viewer->value;
    entry = evtEventViewerGetPendingNode(viewer);
    if (entry != 0) {
        slot = viewer->frameGroup->type - 1;
        if ((u32)slot < 0x11u) {
            switch (slot) {
            case 3:
                entry->p08.h[1] = value;
                break;
            case 1:
                entry->p0C.h[0] = value;
                break;
            case 0:
                entry->p10.h[0] = value;
                break;
            case 11:
            case 15:
            case 16:
                entry->p14.h[0] = value;
                break;
            }
        }
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

/* Registered viewer-command handler with no action; return 0. */
u32 evtViewCmdNoOp(void) {
    return 0;
}

/* Copy the current command word into the selected script entry. */
u32 evtViewerStoreCommandInEntryWord(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry;

    entry = evtEventViewerGetPendingNode(viewer);
    if (entry != 0) {
        entry->p0C.i = viewer->value;
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
}

s32 evtEventViewerCommitGroupSelection(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeGroup *group;
    s32 selection;
    s32 targetGroupIndex;
    s32 groupIndex;
    s32 nameIndex;

    selection = viewer->groupFirst + viewer->groupCursor;
    if (selection == 0) {
        viewer->frameGroup->metadata.extra1 = 0;
    } else if (selection == 1) {
        viewer->frameGroup->metadata.extra1 = 1;
    } else if (selection == 2) {
        viewer->frameGroup->metadata.extra1 = 2;
    } else {
        viewer->frameGroup->metadata.extra1 = 3;
        targetGroupIndex = viewer->groupFirst + viewer->groupCursor - 3;
        groupIndex = 0;
        group = viewer->groups;
        for (; group != NULL; group = group->next) {
            if (group->type == 1) {
                if (groupIndex == targetGroupIndex) {
                    nameIndex = evtEventViewerAddName(group->info->value, viewer);
                    viewer->frameGroup->metadata.extra2 = nameIndex;
                    break;
                }
                groupIndex++;
            }
        }
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

u32 kwlnBattleCopyMatrix(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *record = evtEventViewerGetPendingNode(viewer);
    if (record != NULL) {
        f32 *dst = record->payload;
        f32 *src = viewer->commandMatrix + 8;
        s32 index = 3;
        do {
            index--;
            dst[0] = src[-8];
            dst[4] = src[-4];
            dst[8] = src[0];
            src++;
            dst++;
        } while (index >= 0);
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
    return (u32)record;
}

/* Transfer a selected two-component viewer position to the command entry. */
s32 evtViewCmdSetPosition(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

    if (entry == NULL) {
        return 0;
    }
    if (viewer->frameGroup == 0) {
        return 0;
    }
    entry->p08.f = viewer->floatEditX;
    entry->p0C.f = viewer->floatEditY;
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, u32 viewerAddr) {
    evtViewerPopHistory((EvtRuntime *)viewerAddr);
    return 0;
}
s32 evtViewCmdResolveSlot(s32 operation, void *argument, EvtRuntime *viewer) {
    s32 count = 0;
    EvtRuntimeChild *key;
    EvtRuntimeGroup *group;
    s32 selection;

    key = evtEventViewerGetPendingNode(viewer);
    selection = viewer->groupFirst + viewer->groupCursor;
    if (selection == 0) {
        key->p10.h[0] = 0;
    } else if (selection == 1) {
        key->p10.h[0] = 1;
    } else {
        s32 ordinal = selection - 2;

        group = viewer->groups;
        while (group != NULL) {
            if (group->type == 0x18) {
                if (count == ordinal) {
                    u16 entry = group->entryHeader;
                    key->p10.h[0] = entry + 2;
                    break;
                }
                count++;
            }
            group = group->next;
        }
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}


/* Copy the selected slot descriptor and numeric value into the script entry. */
s32 evtViewCmdSetSlot(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

    entry->p0C.b[0] = viewer->shadowMode;
    entry->p0C.b[1] = viewer->shadowAlpha;
    entry->p14.f = viewer->shadowY;
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

extern char D_003ADA98[]; /* "E%3d_%03d" */
extern u16 D_003BBE78;
extern u16 D_003BBE7A;
extern s32 sprintf(char *buffer, const char *format, ...);
extern s32 evtViewerSaveTrackFiles(s32 mode, EvtRuntime *runtime);

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    s32 applied = 0;
    s32 mode = viewer->inputA;

    if (mode < 3) {
        if (mode >= 0) {
            sprintf(viewer->eventName, D_003ADA98, D_003BBE78, D_003BBE7A);
            if (viewer->inputA == 0) {
                evtViewerSaveTrackFiles(0, viewer);
                evtViewerSaveTrackFiles(1, viewer);
            } else if (viewer->inputA == 1) {
                evtReloadEventViewer(0, viewer);
            } else if (viewer->inputA == 2) {
                evtReloadEventViewer(1, viewer);
            }
            applied = 1;
        }
    }
    return applied ? -1 : 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdSetObjectTarget);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, EvtRuntime *viewer) {
    if (evtEventViewerGetPendingNode(viewer) != 0) {
        viewer->groupFirst = 0;
        viewer->groupCursor = 0;
        evtViewerPushCommandHistory(0xa, 0x9c, 0x54, viewer);
        return 0;
    }
}

/* Restore default effect parameters for the selected timeline key. */
INCLUDE_RODATA(const s32, "event/evtEventViewer", D_003ADA98);

s32 evtEventViewerCopyEffectSetupPayload(s32 unused0, s32 unused1, EvtRuntime *viewer) {
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);
    u128 *destination;
    u128 *source;
    EvtViewerDrawPayload *draw;

    switch (viewer->frameGroup->type) {
    case 10:
        switch (viewer->commandCategory) {
        case 13:
            destination = entry->payload;
            source = D_00324770[0];
            PCP_COPY_VECTOR(destination, source);
            PCP_COPY_VECTOR(destination + 1, source + 1);
            PCP_COPY_VECTOR(destination + 2, kwlnDefaultColorVector);
            break;
        case 14:
            entry->p08.f = D_003BD358;
            entry->p0C.f = D_003BD35C;
            break;
        }
        break;
    case 11:
        draw = entry->payload;
        draw->x = kwlnDrawVector.fogFar;
        draw->mode = kwlnDrawVector.fogColor;
        draw->y = kwlnDrawVector.fogNear;
        draw->z = kwlnDrawVector.depthNear;
        draw->w = kwlnDrawVector.depthFar;
        break;
    case 13:
        memcpy(entry->payload, effGetLoadDescA(), 0x28);
        break;
    case 14:
        memcpy(entry->payload, effEventGetBlurTemplateSetupParams(), 0x2C);
        break;
    case 15:
        memcpy(entry->payload, effEventGetScatterBlurSetupParams(), 0x2C);
        break;
    case 23:
        memcpy(entry->payload, effEventGetScaleBlurSetupParams(), 0x2C);
        break;
    case 27:
        memcpy(entry->payload, effGetLoadDescD(), 0x28);
        break;
    case 16:
        memcpy(entry->payload, effEventGetSolidRectangleSetupParams(), 0x18);
        break;
    case 17:
        memcpy(entry->payload, effEventGetResourceTemplateSetupParams(), 0x24);
        break;
    }
    evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}


/* Hand control back to the frame update whether or not the task still owns a viewer. */
s32 evtViewerPickNextHandler(KwlnTask *task) {
    if (kwlnTaskGetUserValue(task) != 0) {
        return (s32)evtViewerUpdateFrame;
    }
    return (s32)evtViewerUpdateFrame;
}

extern s16 D_003BBE7C;
extern s8 D_00324510[];
extern s32 itfMesStartEntry(s32 window, s32 entryIndex, s32 itemIndex);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void mnuAdvanceShopMenuState(struct EvtRuntime *);
extern s32 mnuGetMoviePlaybackMode(void);
extern s32 evtViewerPickNextHandler(KwlnTask *task);

/* Advance the viewer timeline, deferred voice and task-update handoff. */
s32 evtViewerUpdateFrame(KwlnTask *task) {
    EvtRuntime *viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    s32 flags;

    D_003BBE7C = 0;
    if (viewer->unk2238 == 1) {
        evtViewerPreGlyphTickHookNoOp(viewer);
    }
    evtViewerAdvanceGlyphTick(viewer);
    evtEventViewerApplyEffectToggles(viewer);
    flags = viewer->flags;
    if (!(flags & 8)) {
        if (D_00324510[0x22] < 0) {
            viewer->flags = flags | 1;
            evtViewerDispatchFlagMode(viewer);
            viewer->actionMode = 0;
            viewer->historyCount = 0;
            viewer->commandResetId = 0;
            evtViewerPushCommandHistory(1, 0x24, 0x18, viewer);
            return (s32)evtViewerPickNextHandler;
        }
        evtViewerFlag8ClearFrameHookNoOp(viewer);
    } else {
        if (viewer->curFrame == viewer->headerFirst) {
            viewer->flags = flags & ~1;
            evtViewerDispatchFlagMode(viewer);
        }
        if (D_00324510[0x22] >= 0 && D_00324510[0x2C] < 0 &&
            evtViewerHasUpdateFlag(viewer) == 0) {
            evtEventViewerStartEndCountdown(viewer);
        }
    }
    if (viewer->voicePending == 1 && evtViewerHasUpdateFlag(viewer) == 0 &&
        mnuQueryTitleSoundBusy() == 0) {
        itfMesStartEntry(viewer->windowContext->handle,
            viewer->voiceMessage, 0);
        viewer->voicePending = 0;
        evtPrintDeveloperConsoleMessage("[conflict voice play      ] mesno= %d\n",
            viewer->voiceMessage);
    }
    if (viewer->curFrame != viewer->previousGlyphPosition) {
        evtApplyViewerTimelineFrame(viewer->curFrame, viewer);
    }
    viewer->previousGlyphPosition = viewer->curFrame;
    if (viewer->flags & 8) {
        evtViewerApplySelectedEntry(viewer);
    }
    evtUpdateViewerMessageWindow(viewer);
    evtEventViewerFadeBackground(viewer);
    mnuAdvanceShopMenuState((struct EvtRuntime *)viewer);
    if (viewer->flags & 8) {
        if (viewer->curFrame >= viewer->frameRange.word) {
            return -1;
        }
        if (evtViewerHasUpdateFlag(viewer) == 1 && viewer->updateCount >= 25) {
            return -1;
        }
    }
    if (!(viewer->flags & 1)) {
        if (viewer->curFrame >= viewer->frameRange.word) {
            if (!(viewer->flags & 8)) {
                viewer->flags ^= 1;
                evtViewerDispatchFlagMode(viewer);
            }
        } else if (viewer->timedActive == 1) {
            if (mnuGetMoviePlaybackMode() == 2) {
                viewer->curFrame++;
            }
        } else {
            viewer->curFrame++;
        }
    }
    return 0;
}

/* Update the active viewer, then switch to its frame-variable task. */
void *evtViewerScheduleFrameVariableTask(s32 task) {
    void *viewer;

    viewer = (void *)kwlnTaskGetUserValue((KwlnTask *)task);
    evtApplyViewerTimelineFrame(((EvtRuntime *)viewer)->curFrame, viewer);
    kwlnTaskAddChild(task, (KwlnTask *)evtCreateFrameVariableTask());
    kwlnDrawControlFlags |= 0x2000000;
    return (void *)evtViewerUpdateFrame;
}

/* Initialize the active viewer and schedule its next update callback. */
s32 evtViewerInitializeUpdateSequence(KwlnTask *task) {
    struct EvtRuntime *viewer;

    viewer = (struct EvtRuntime *)kwlnTaskGetUserValue(task);
    fldInitializeCameraColorResource();
    evtEventViewerReset(viewer);
    kwlnDrawControlFlags |= 0x2000000;
    return (s32)(u32)evtViewerScheduleFrameVariableTask;
}

/* Advance the viewer update: tick the timed action or hand over to the next task. */
void *evtViewerAdvanceUpdate(KwlnTask *task) {
    EvtRuntime *viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    PolyMovieWork *window;
    s32 windowFlags;

    evtEventViewerPollMovieResources(viewer->windowContext);
    window = viewer->windowContext;
    windowFlags = window->flags;
    if ((windowFlags & 8) == 0) {
        kwlnDrawControlFlags |= 0x2000000;
        return 0;
    } else {
        if ((windowFlags & 1) != 0) {
            kwlnDrawControlFlags |= 0x2000000;
            return 0;
        }
        if ((u32)(mnuPollTitleStreamStateLocked() - 3) < 2) {
            if (viewer->titleStreamWaitFrames == 0x78) {
                mnuMarkTitleStreamResetPending();
            }
            viewer->titleStreamWaitFrames++;
            kwlnDrawControlFlags |= 0x2000000;
            return 0;
        }
        evtPopulateViewerTimelineFromMovie(viewer->windowContext, viewer);
        kwlnDrawControlFlags |= 0x2000000;
        return (void *)evtViewerScheduleFrameVariableTask;
    }
}

s32 evtViewerStartUpdate(KwlnTask *task) {
    EvtRuntime *viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    PolyMovieWork *context;
    u16 eventId;
    u16 sceneId;

    fldInitializeCameraColorResource();
    context = viewer->windowContext;
    eventId = context->eventId;
    sceneId = context->sceneId;
    D_003BBE78 = eventId;
    D_003BBE7A = sceneId;
    sprintf(viewer->eventName, D_003ADA98, D_003BBE78, D_003BBE7A);
    viewer->flags = 1;
    evtViewerDispatchFlagMode(viewer);
    viewer->unk2238 = 0;
    viewer->flags |= 8;
    kwlnDrawControlFlags |= 0x2000000;
    return (s32)(u32)evtViewerAdvanceUpdate;
}

extern f32 D_00324590[];
extern void mnuReleaseCampSceneRegisteredIds(EvtRuntime *viewer);
extern void fldSetTitleDrawMode();
extern void mnuStopMovieDrawTask();
extern s32 sdfCheckPendingWorkWithInterrupts();
extern void evtDestroySecondaryWorldNode();
extern void kwlnTextureReleaseHeldReference();
extern void fldReleaseCameraColorEffect();
void evtViewerCleanupMessageWindow(EvtRuntime *viewer);

void evtViewerReleaseResources(EvtRuntime *viewer) {
    mnuReleaseCampSceneRegisteredIds(viewer);
    evtResetUnitVectorSlots();
    evtViewerCleanupMessageWindow(viewer);
    mnuCampLinkFontGlyph(viewer);
    fldSetTitleDrawMode(0);
    kwlnCancelConfiguredFadeFrames();
    D_00324590[0] = D_00324590[1] = D_00324590[2] = D_00324590[3] = 0.0f;
    if (viewer->timedActive == 1) {
        mnuStopMovieDrawTask();
        viewer->timedActive = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    evtDestroySecondaryWorldNode();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (viewer->pendingResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)viewer->pendingResource);
        viewer->pendingResource = 0;
        viewer->pendingWork = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnTextureReleaseHeldReference();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    evtEventViewerReleaseGroups(viewer);
    evtEventViewerShutdown(viewer);
    sdfReleaseResourceAllocation(viewer->resourceHandle);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    fldReleaseCameraColorEffect();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnFadeSetMode(0);
    kwlnDrawControlFlags |= 0x2000000;
}

/* Destroy the currently active event viewer. */
void evtEventViewerDestroyActive(KwlnTask *task) {
    EvtRuntime *viewer;

    viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    evtViewerReleaseResources(viewer);
}

/* Alternate destroy callback for the same active viewer. */
void evtEventViewerDestroyActiveAlternate(KwlnTask *task) {
    EvtRuntime *viewer;

    viewer = (EvtRuntime *)kwlnTaskGetUserValue(task);
    evtViewerReleaseResources(viewer);
}

extern void mnuCampInitFontResource(EvtRuntime *viewer);

void evtEventViewerInitCampFontResource(EvtRuntime *viewer) {
    mnuCampInitFontResource(viewer);
}

extern u32 D_003BA8EC;
extern void *memset(void *dst, s32 value, u32 size);
extern s32 evtCreateSkyTask(void);

void evtViewerCreateTaskWithSky(void) {
    SdfMemBlock *viewerHandle;
    EvtRuntime *viewer;
    void *viewerTask;

    D_003BA8EC = 0x80000000;
    viewerHandle = sdfAllocGeneralBlock(0x2490);
    viewer = (EvtRuntime *)sdfResourceRetainAddress(viewerHandle);
    memset(viewer, 0, 0x2490);
    viewer->resourceHandle = viewerHandle;
    viewerTask = kwlnTaskCreate(evtViewerTaskName, 0x3EB, 1, 1, evtViewerInitializeUpdateSequence, evtEventViewerDestroyActive, (u32)viewer);
    kwlnTaskAddChild(viewerTask, (KwlnTask *)evtCreateSkyTask());
    evtEventViewerInitCampFontResource(viewer);
}

void evtEventViewerDestroyTask(void) {
    kwlnTaskDestroyWithHierarchyByName(evtViewerTaskName, 1);
}



void evtEventViewerPollMovieResources(PolyMovieWork *assets) {

    if (assets->flags & 8) {
        return;
    }
    if (assets->flags & 2) {
        if (assets->mainResource.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)assets->mainResource.request) == 0) {
            return;
        }
        assets->mainResource.handle = fileGetResourceHandle((struct FileRequest *)assets->mainResource.request);
        assets->mainResource.address = (PmdHeader *)sdfResourceRetainAddress(assets->mainResource.handle);
        filePollEntryCleanup(assets->mainResource.request);
        assets->mainResource.request = 0;
        assets->flags &= ~2;
    } else if (assets->flags & 4) {
        if (assets->secondaryResource.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)assets->secondaryResource.request) == 0) {
            return;
        }
        assets->secondaryResource.handle = fileGetResourceHandle((struct FileRequest *)assets->secondaryResource.request);
        assets->secondaryResource.address = (PmdHeader *)sdfResourceRetainAddress(assets->secondaryResource.handle);
        filePollEntryCleanup(assets->secondaryResource.request);
        assets->secondaryResource.request = 0;
        assets->flags &= ~4;
    } else if (assets->flags & 0x10) {
        if (assets->tertiaryResource.request == 0) {
            return;
        }
        if (fileIsRequestReadyInCurrentMode((struct FileRequest *)assets->tertiaryResource.request) == 0) {
            return;
        }
        assets->tertiaryResource.handle = fileGetResourceHandle((struct FileRequest *)assets->tertiaryResource.request);
        assets->tertiaryResource.address = (PmdHeader *)sdfResourceRetainAddress(assets->tertiaryResource.handle);
        filePollEntryCleanup(assets->tertiaryResource.request);
        assets->tertiaryResource.request = 0;
        assets->flags &= ~0x10;
    } else if (assets->mainResource.address != NULL && assets->secondaryResource.address != NULL) {
        evtPolygonMovieInitWork(assets, assets->mainResource.address, assets->secondaryResource.address, assets->tertiaryResource.address);
        assets->flags |= 8;
    }
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", evtViewerTaskName);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE78);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE7A);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE7C);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE80);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE88);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE90);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE94);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBE98);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEA0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEA8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEB0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEB8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEC0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEC8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBED0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBED8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEE0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEE8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEF0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBEF8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF00);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF08);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF10);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF18);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF20);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF28);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF30);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF38);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF40);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF48);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF50);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF58);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF60);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF68);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_003BBF70);

