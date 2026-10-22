#include "sdk_ps2.h"
#include "mnu_camp_display.h"
#include "common.h"
#include "mdl_flag_api.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "evt_viewer.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "evt_solar.h"
#include "eff.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "eff_event_work_api.h"
#include "kwln_task_lifecycle.h"
#include "btl.h"
#include "sdf_packet_alloc.h"

/* Insert after existing equal keys, keeping ascending order and incrementing count. */
void LinkFrameInfo(EvtRuntimeGroup *list, EvtRuntimeChild *insertedNode) {
    EvtRuntimeChild *cursor = list->children;

    if (cursor == 0) {
        list->children = insertedNode;
        list->lastChild = insertedNode;
        insertedNode->next = 0;
        insertedNode->prev = 0;
    } else {
        while (cursor != 0) {
            if (insertedNode->frame < cursor->frame) {
                if (cursor->prev == 0) {
                    list->children = insertedNode;
                    cursor->prev = insertedNode;
                    insertedNode->next = cursor;
                    insertedNode->prev = 0;
                } else {
                    cursor->prev->next = insertedNode;
                    insertedNode->prev = cursor->prev;
                    insertedNode->next = cursor;
                    cursor->prev = insertedNode;
                }
                break;
            }
            cursor = cursor->next;
        }
        if (cursor == 0) {
            EvtRuntimeChild *tailNode = list->lastChild;
            tailNode->next = insertedNode;
            insertedNode->prev = list->lastChild;
            insertedNode->next = 0;
            list->lastChild = insertedNode;
        }
    }
    list->childCount++;
}

/* Unlink an attached node, clear its links, and decrement the owning list's count. */
void UnlinkFrameInfo(EvtRuntimeGroup *list, EvtRuntimeChild *removedNode) {
    EvtRuntimeChild *nextNode = removedNode->next;
    EvtRuntimeChild *previousNode = removedNode->prev;
    if (previousNode == 0) {
        list->children = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    {
        EvtRuntimeChild *previousNeighbor = removedNode->prev;
        EvtRuntimeChild *nextNeighbor = removedNode->next;
        if (nextNeighbor == 0) {
            list->lastChild = previousNeighbor;
        } else {
            nextNeighbor->prev = previousNeighbor;
        }
    }
    {
        s32 nodeCount = list->childCount;
        removedNode->prev = 0;
        removedNode->next = 0;
        list->childCount = nodeCount - 1;
    }
}

/* Walk the linked list and reinsert the first out-of-order successor. */
void SortFrameInfo(EvtRuntimeGroup *list) {
    if (list != 0) {
        EvtRuntimeChild *anchorNode = list->children;
        while (anchorNode != 0) {
            EvtRuntimeChild *resumeNode = anchorNode->next;
            EvtRuntimeChild *candidateNode = resumeNode;
            while (candidateNode != 0) {
                if (candidateNode->frame < anchorNode->frame) {
                    UnlinkFrameInfo(list, candidateNode);
                    LinkFrameInfo(list, candidateNode);
                    /* Resume from the relocated node's new successor, not its old one. */
                    resumeNode = candidateNode->next;
                    break;
                }
                candidateNode = candidateNode->next;
            }
            anchorNode = resumeNode;
        }
    }
}

extern s32 evtEventViewerAddName(const char *, EvtRuntime *);
extern void mnuSampleCampPanelVisualTrack(EvtRuntime *runtime, EvtRuntimeGroup *group, s32 frame,
                                          f32 (*outVectors)[4], f32 *outValueA, f32 *outValueB);
extern void mnuSampleCampDisplayTrack(EvtRuntime *viewer, EvtRuntimeGroup *track, s32 value,
                                      CampDisplayDefaults *display);
extern void mnuSampleCampListLayoutTrack(EvtRuntime *viewer, EvtRuntimeGroup *track, EvtBlendKey *out);
extern KwlnDrawVectorParams kwlnDrawVector;

/* Capture kind-specific defaults before adding a new key to its track. */
EvtRuntimeChild *evtCreateTrackKeyAtFrame(EvtRuntimeGroup *group, s32 frame, EvtRuntime *viewer) {
    CampDisplayDefaults display;
    EvtBlendKey layout;
    f32 first, second;
    f32 (*vectors)[4] = NULL;
    EvtRuntimeChild *key;

    switch (group->type) {
    case 10:
        vectors = sdfChipAllocDirect(0x30);
        mnuSampleCampPanelVisualTrack(viewer, group, frame, vectors, &first, &second);
        break;
    case 24:
        mnuSampleCampDisplayTrack(viewer, viewer->frameGroup, viewer->curFrame, &display);
        break;
    case 25:
        mnuSampleCampListLayoutTrack(viewer, viewer->frameGroup, &layout);
        break;
    }
    key = sdfChipAllocDirect(sizeof(*key));
    memset(key, 0, sizeof(*key));
    key->frame = frame;
    key->serializedValue = -1;
    LinkFrameInfo(group, key);

    switch (group->type) {
    case 2:
        key->p08.f = 0.5235987306f;
        key->serializedValue = evtEventViewerAddName(viewer->entryName[group->entryHeader], viewer);
        break;
    case 1:
        key->serializedValue = evtEventViewerAddName(viewer->entryName[group->entryHeader], viewer);
        key->p08.sb[0] = 0;
        key->p0C.sb[2] = 1;
        key->p0C.sb[3] = 20;
        break;
    case 18:
        key->serializedValue = evtEventViewerAddName(viewer->entryName[group->entryHeader], viewer);
        key->p08.h[0] = 0;
        key->p08.h[1] = -1;
        key->p08.sb[0] = 0;
        break;
    case 20: case 21:
        key->serializedValue = evtEventViewerAddName(viewer->entryName[group->entryHeader], viewer);
        key->p08.sb[0] = -1;
        key->p08.sb[1] = 0;
        key->p08.h[1] = 0;
        key->p0C.sb[0] = -1;
        key->p0C.sb[1] = -1;
        key->p0C.sb[2] = -1;
        key->p0C.sb[3] = -1;
        break;
    case 3: case 26:
        key->serializedValue = evtEventViewerAddName(viewer->entryName[group->entryHeader], viewer);
        key->p08.sb[0] = -1;
        key->p08.h[1] = 0;
        key->p0C.sb[0] = 0;
        key->p0C.sb[1] = -1;
        key->p0C.h[1] = -1;
        break;
    case 9:
        key->serializedValue = evtEventViewerAddName(viewer->entryName[group->entryHeader], viewer);
        key->p08.h[0] = -1;
        key->p08.h[1] = 1;
        break;
    case 12:
        key->p08.h[0] = 1;
        key->p08.h[1] = 128;
        key->p0C.i = 0;
        break;
    case 22:
        key->p08.h[0] = 1;
        break;
    case 10:
        key->p08.f = first;
        key->payload = vectors;
        key->p0C.f = second;
        key->p10.h[0] = (u16)group->setterId;
        key->serializedValue = 0;
        break;
    case 11: {
        EvtViewerDrawPayload *draw = sdfChipAllocDirect(sizeof(*draw));
        draw->parameters.vec[0] = kwlnDrawVector.fogFar;
        draw->parameters.color = kwlnDrawVector.fogColor;
        key->payload = draw;
        draw->parameters.vec[1] = kwlnDrawVector.fogNear;
        draw->parameters.vec[2] = kwlnDrawVector.depthNear;
        draw->parameters.vec[3] = kwlnDrawVector.depthFar;
        key->p08.h[0] = 1;
        break;
    }
    case 13: {
        void *payload = sdfChipAllocDirect(0x28);
        key->payload = payload;
        memcpy(payload, effGetLoadDescA(), 0x28);
        key->p08.h[0] = 1;
        break;
    }
    case 14: {
        void *payload = sdfChipAllocDirect(0x2C);
        key->payload = payload;
        memcpy(payload, &Eve_PartialMultiBlurGetParams()->body, sizeof(dds3Effect_PartialMultiBlurParam_t));
        key->p08.h[0] = 1;
        key->p10.h[0] = 0;
        break;
    }
    case 15: {
        void *payload = sdfChipAllocDirect(0x2C);
        key->payload = payload;
        memcpy(payload, &Eve_DistortionBlurGetParams()->params, sizeof(dds3Effect_DistortionBlurParam_t));
        key->p08.h[0] = 1;
        break;
    }
    case 23: {
        void *payload = sdfChipAllocDirect(0x2C);
        key->payload = payload;
        memcpy(payload, Eve_RippleBlurGetParams(), 0x2C);
        key->p08.h[0] = 1;
        break;
    }
    case 27: {
        void *payload = sdfChipAllocDirect(0x28);
        key->payload = payload;
        memcpy(payload, effGetCh73Params(), 0x28);
        key->p08.h[0] = 1;
        break;
    }
    case 16: {
        void *payload = sdfChipAllocDirect(0x18);
        key->payload = payload;
        memcpy(payload, Eve_FilterGetParams(), 0x18);
        key->p08.h[0] = 1;
        break;
    }
    case 17: {
        void *payload = sdfChipAllocDirect(0x24);
        key->payload = payload;
        memcpy(payload, Eve_PartialMultiFilterGetParams(), 0x24);
        key->p08.h[0] = 1;
        break;
    }
    case 24:
        key->serializedValue = evtEventViewerAddName(viewer->entryName[group->entryHeader], viewer);
        key->p08.sb[0] = display.enabled;
        key->p08.sb[1] = display.variant;
        key->p0C.h[0] = display.x;
        key->p0C.h[1] = display.y;
        key->p10.sb[0] = display.color[0];
        key->p10.sb[1] = display.color[1];
        key->p10.sb[2] = display.color[2];
        key->p10.sb[3] = display.color[3];
        key->p14.f = display.scaleX;
        key->p18.f = display.scaleY;
        break;
    case 25: {
        EvtCameraColorPayload *payload = sdfChipAllocDirect(0x40);
        s32 i;
        key->payload = payload;
        payload->parameters.x = layout.x;
        payload->parameters.w[0] = layout.w[0];
        payload->parameters.w[1] = layout.w[1];
        payload->parameters.w[2] = layout.w[2];
        payload->parameters.w[3] = layout.w[3];
        payload->parameters.flagWord = layout.flagWord;
        for (i = 0; i < 3; i++) {
            payload->parameters.y[i] = layout.y[i];
            payload->parameters.z[i] = layout.z[i];
        }
        break;
    }
    case 0: case 4: case 5: case 6: case 7: case 8: case 19:
    case 28: case 29: case 30: case 31: case 32:
    default:
        break;
    }
    return key;
}
