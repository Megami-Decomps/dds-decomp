#include "common.h"
#include "sdf_chip.h"
#include "evt_viewer.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "mdl_motion_api.h"
#include "sdf_motion.h"
#include "itf_mes_window.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "pcp_vu0.h"
#include "evt_unit.h"
#include "eff_transform.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "dat_state.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_object.h"
#include "mdl.h"
#include "evt_polygon_movie.h"
#include "kwln_task_lifecycle.h"
#include "file_request_api.h"

extern s32 strcmp(const char *a, const char *b);
extern char *strcpy(char *dst, const char *src);
extern void *dds3GetWorldObject(void);
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



void evtUnlinkListNode(EvtRuntimeGroup *entry, EvtRuntimeChild *node);
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

void func_002467A0(void) {
    evtCreateViewerTimelineKey();
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
    entry->entryHeader = -1;
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

void evtViewerSetMinimumFromCurrent(EvtRuntime *range) {
    s32 current;

    current = range->curFrame;
    range->headerFirst = current;
    if (range->frameRange.word < current) {
        range->frameRange.word = current;
    }
}

void evtViewerSetMaximumFromCurrent(EvtRuntime *range) {
    s32 current;

    current = range->curFrame;
    range->frameRange.word = current;
    if (current < range->headerFirst) {
        range->headerFirst = current;
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
extern void *func_00115500(void *resource, void *position, void *scale);
extern void effObjReplaceActiveEventNode(void *obj, u32 entryId);
struct EffNodeDescriptor;
extern struct EffectObj *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, void *secondVector);
extern s32 func_00115318(s32 arg, f32 *vec0, f32 *vec1);
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
            handle = func_00115318((s32)cmd->resourceData, vec0, vec1);
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
    evtViewerBindNamedOwner((EffWorldNode *)effect, value, type, word, viewer);
    return effect;
}

void evtEventViewerFreeBuffer(EvtRuntimeChild *node) {
    if (node->payload != NULL) {
        dds3RemoveWorldObjectNode(node->payload);
    }
    node->payload = NULL;
}

extern f32 dds3GetCameraFieldOfView(EffWorldNode *camera);
extern void sdfSetViewFieldOfView(f32);
extern s32 evtViewerHasUpdateFlag(EvtRuntime *);

typedef struct CampDisplayDefaults CampDisplayDefaults;
extern void func_0025E460(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);
extern void mnuDrawCampScaledTexture(SdfTex *texture, CampDisplayDefaults *display);

/* Interpolates parameter keys at the viewer's current frame, accounting for
 * the track offset. DDS2 subtracts 35 from the second output word before applying it. */
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
        func_0025E460((EvtRuntimeChild *)from, (EvtRuntimeChild *)to, (CampDisplayDefaults *)out, ratio);
        *(s32 *)(out + 4) -= 35;
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
extern u128 *D_0037F770[];
extern u128 kwlnDefaultColorVector[];
extern f32 D_00438A48;
extern f32 D_00438A4C;
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
extern void func_0025EE00(EvtRuntime *);

void func_002476B8(EvtRuntime *viewer) {
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
    func_0025EE00(viewer);
    evtViewerApplyParameterKeyTracks(viewer);
}

/* Select the active entry (or fallback) and sync world selection and camera. */
void evtViewerApplySelectedEntry(EvtRuntime *viewer) {
    EffWorldNode *unit;
    EffWorldNode *first = viewer->activeEntryIndex;

    if (first != 0) {
        unit = first;
    } else {
        unit = (EffWorldNode *)viewer->fallbackEntry;
    }
    if (unit != 0) {
        dds3SetWorldCameraObject(dds3GetWorldObject(), unit);
        sdfSetViewFieldOfView(dds3GetCameraFieldOfView(unit));
    }
}

extern u16 D_004372B0;
extern s32 evtViewerTestIndexedCondition(u32 encodedId);
extern void mnuUnpackNibbleFields(EvtRuntimeChild *, s32 *, s32 *);
extern s32 evtSetBgmVolumePan(s32, s32);
extern s32 evtPlayBgm(s32, s32);
extern s32 evtFadeInBgm(s32, s32);
extern s32 evtQueueValidatedBgmSoundCode(s32, s32);
extern void evtTransitionBgm(s32, s32);
extern void fldRegisterCampSceneId(EvtRuntime *viewer, s32 identifier);
extern void func_003425B0(void);
extern void func_003425D8(void);
extern void func_00341CA8(void);
extern void kwlnCancelConfiguredFadeFrames(void);
extern void kwlnFadeSetupFrames(s32 mode, s32 frames);
extern void kwlnPadStartMotor(u32 motor, u8 level, s32 duration);
extern void func_002A7F98(s32 value);
extern s32 fldTitleIsActive(void);
extern void func_0014E668(s32 enabled);
extern void func_0014E698(void);
extern void kwlnFadeSetMode(s32 mode);
extern void kwlnFadeBackgroundStartOut(s32 duration);
extern void kwlnFadeBackgroundStartIn(s32 duration);
extern s32 mnuQueryTitleSoundBusy(void);
extern u32 itfMesGetWindowEntryItems(s32 window, s32 entry);
extern s32 itfMesSelectTableItemText(s32 window, s32 entry, s32 unused);
extern s32 itfMesStartEntry(s32 window, s32 entry, s32 unused);
extern void itfMesBuildOptionList(s32 window, s32 entry);
extern void mnuStopTitleVoicePlayback(void);
extern void evtPrintDeveloperConsoleMessage(const char *format, ...);
extern char D_004224A8[];

/* Apply one timeline key to its effect, audio, message, or fade channel. */
INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004224A8);

void func_00247858(EvtRuntimeGroup *track, EvtRuntimeChild *key, s32 position, s32 unused,
                   EvtRuntime *viewer) {
    s32 nibbles[2];

    nibbles[0] = 0;
    nibbles[1] = 0;

    switch (track->type) {
    case 0:
    case 1:
    case 2:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 22:
    case 23:
    case 24:
    case 25:
    case 27:
    case 31:
        break;
    case 3:
    case 20:
    case 21:
    case 26: {
        EffWorldNode *object;

        if (position >= key->frame && key->p08.sb[0] < 0) {
            key->p08.sb[0] = evtViewerCreateObjectInFreeSlot(
                position, track, key, viewer);
        }
        if (key->p08.sb[0] < 0) {
            break;
        }
        object = viewer->objects[key->p08.sb[0]];
        if (object == NULL || track->type != 21) {
            break;
        }
        {
            struct EffectObj *first = evtEventViewerGetNameObject(key->p0C.sb[0], viewer);
            struct EffectObj *second = evtEventViewerGetNameObject(key->p0C.sb[1], viewer);
            struct EffectObj *third = evtEventViewerGetNameObject(key->p0C.sb[2], viewer);
            struct EffectObj *fourth = evtEventViewerGetNameObject(key->p0C.sb[3], viewer);

            effObjCopyMagatuhiSourceParameters((struct EffectObj *)object, first, second, third, fourth);
        }
        break;
    }
    case 18:
        if (position == key->frame) {
            evtEventViewerFreeBuffer(key);
            if (track->metadata.extra1 == 0) {
                key->payload = func_00247400((void *)track->resourceData,
                    (s32)key->p0C.sb[0], key->p08.sh[1], -1, key->frame, viewer);
            } else {
                key->payload = func_00247400((void *)track->resourceData,
                    (s32)key->p0C.sb[0], key->p08.sh[1], -1, 0, viewer);
            }
        }
        break;
    case 5:
        if (evtViewerHasUpdateFlag(viewer) == 1 ||
            evtViewerTestIndexedCondition((u32)(s32)key->p08.sh[1]) == 0) {
            break;
        }
        evtSetBgmVolumePan(D_004372B0, key->p08.sh[0]);
        fldRegisterCampSceneId(viewer, key->p08.sh[0]);
        break;
    case 19:
        if (evtViewerHasUpdateFlag(viewer) == 1 ||
            evtViewerTestIndexedCondition((u32)(s32)key->p0C.sh[0]) == 0) {
            break;
        }
        switch (key->p08.sh[0]) {
        case 0:
            evtPlayBgm(D_004372B0, key->p08.sh[1]);
            fldRegisterCampSceneId(viewer, key->p08.sh[1]);
            break;
        case 1:
            evtFadeInBgm(D_004372B0, key->p08.sh[1]);
            fldRegisterCampSceneId(viewer, key->p08.sh[1]);
            break;
        case 2:
            evtQueueValidatedBgmSoundCode(D_004372B0, key->p08.sh[1]);
            break;
        case 3:
            evtTransitionBgm(D_004372B0, key->p08.sh[1]);
            break;
        case 4:
            func_003425B0();
            break;
        case 5:
            func_003425D8();
            break;
        case 6:
            func_00341CA8();
            break;
        }
        break;
    case 7:
        if (key->duration == 0 && key->p08.sh[0] == 0) {
            viewer->flags &= ~2;
            kwlnCancelConfiguredFadeFrames();
        } else if (position == key->frame) {
            viewer->flags |= 2;
            kwlnFadeSetupFrames(key->duration, key->p08.sh[0]);
        }
        break;
    case 4: {
        s32 selectedText;

        if (viewer->windowContext->handle == -1 || evtViewerHasUpdateFlag(viewer) != 0) {
            break;
        }
        mnuUnpackNibbleFields(key, &nibbles[0], &nibbles[1]);
        if (key->frame != position) {
            break;
        }
        if (itfMesGetWindowEntryItems(viewer->windowContext->handle, nibbles[0]) == 0) {
            if (evtViewerTestIndexedCondition((u32)(s32)key->p08.sh[1]) == 0) {
                break;
            }
            if (mnuQueryTitleSoundBusy() != 0) {
                selectedText = itfMesSelectTableItemText(viewer->windowContext->handle, nibbles[0], 0);
                if (selectedText == 1) {
                    evtPrintDeveloperConsoleMessage(D_004224A8, nibbles[0]);
                    mnuStopTitleVoicePlayback();
                    viewer->voicePending = selectedText;
                    viewer->voiceMessage = nibbles[0];
                    viewer->voiceFrame = viewer->curFrame;
                    break;
                }
            }
            itfMesStartEntry(viewer->windowContext->handle, nibbles[0], 0);
        } else {
            viewer->unk23C6 = (s16)(nibbles[1] + 0xC7);
            itfMesBuildOptionList(viewer->windowContext->handle, nibbles[0]);
            viewer->flags |= 1;
            evtViewerDispatchFlagMode(viewer);
        }
        break;
    }
    case 28:
        if (position == (s32)key->frame + track->metadata.value) {
            kwlnPadStartMotor(key->p08.sh[0], key->p08.b[2], key->duration);
        }
        break;
    case 29:
        if (evtViewerHasUpdateFlag(viewer) == 1 || position != key->frame) {
            break;
        }
        func_002A7F98(key->p08.sh[0]);
        viewer->timedStart = key->frame;
        if (key->duration == 0) {
            if (key->p0C.sb[0] == 0) {
                viewer->timedEnd = -1;
            } else {
                viewer->timedEnd = -2;
            }
        } else {
            viewer->timedEnd = key->frame + key->duration;
        }
        viewer->timedActive = 1;
        break;
    case 30:
        if (position == key->frame && fldTitleIsActive() == 1) {
            if (key->p08.sh[1] != 0) {
                func_0014E668(1);
            } else {
                func_0014E668(0);
            }
            func_0014E698();
        }
        break;
    case 32:
        if (position == key->frame) {
            if (key->p08.sh[0] == 0) {
                kwlnFadeSetMode(1);
                kwlnFadeBackgroundStartOut(key->duration);
            } else {
                kwlnFadeSetMode(1);
                kwlnFadeBackgroundStartIn(key->duration);
            }
        }
        break;
    }
}

