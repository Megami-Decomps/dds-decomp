#ifndef EVT_VIEWER_H
#define EVT_VIEWER_H
#include "common.h"
#include "evt_viewer_name_index.h"
#include "evt_blend_vec_color.h"
#include "evt_world.h"
#include "evt_polygon_movie.h"
#include "fld.h"
#include "kwln.h"

struct dds3ProcessID_t;
struct EffNodeDescriptor;
struct SoundMixer;

/* A timeline key's scalar formats are selected by its track kind. */
typedef union EvtViewParam {
    f32 f; s32 i; u32 u; u16 h[2]; s16 sh[2]; u8 b[4]; s8 sb[4];
} EvtViewParam;

/* Allocation-backed 0x20 payload used by type-0xB draw keys. */
typedef struct EvtViewerDrawPayload {
    EvtBlendVecColor parameters;
    u8 unknown14[0xC];
} EvtViewerDrawPayload;
typedef char EvtViewerDrawPayload_parameters_offset[((u32)&((EvtViewerDrawPayload *)0)->parameters == 0x00) ? 1 : -1];
typedef char EvtViewerDrawPayload_tail_offset[((u32)&((EvtViewerDrawPayload *)0)->unknown14 == 0x14) ? 1 : -1];
typedef char EvtViewerDrawPayload_size[(sizeof(EvtViewerDrawPayload) == 0x20) ? 1 : -1];

/* Complete 0x38-byte key allocated by the viewer and linked on a group. */
typedef struct EvtRuntimeChild {
    u16 frame;
    u16 duration; /* Also the consecutive index in type-0xB groups. */
    s32 serializedValue;
    union {
        s8 parameterBytes[0x20];
        union {
            s32 words[8];
            struct {
                u8 pad00[2]; u16 groupTypeIndex;
                u8 pad04[6]; u16 groupTypeAIndex;
            } f;
        } body;
        struct {
            EvtViewParam p08, p0C, p10, p14, p18, p1C, p20, p24;
        };
    };
    u8 pad28[4];
    void *payload; /* Type 0x12 owns a dds3Basic_t; other kinds hold buffers or borrowed data. */
    struct EvtRuntimeChild *next;
    struct EvtRuntimeChild *prev;
} EvtRuntimeChild;

/* Allocated and serialized type-0x19 payload; the final bytes remain opaque. */
typedef struct EvtCameraColorPayload {
    EvtBlendKey parameters;
    u8 unknown38[8];
} EvtCameraColorPayload;
typedef char EvtCameraColorPayload_size[(sizeof(EvtCameraColorPayload) == 0x40) ? 1 : -1];

/* Serialized and embedded group-link metadata share these eight bytes. */
typedef struct EvtGroupMetadata {
    u8 type;
    u8 flag;
    s16 entry;
    s16 value;
    s8 option; /* +0x06: type-specific group mode chosen by the viewer. */
    s8 extra2;
} EvtGroupMetadata;

typedef s32 EvtViewerParameterKeySource;
enum {
    EVT_VIEWER_PARAMETER_KEY_SOURCE_TIMELINE = 0,
    EVT_VIEWER_PARAMETER_KEY_SOURCE_PENDING_KEY = 1
};

typedef enum EvtObjectAttachmentState {
    EVT_OBJECT_ATTACHMENT_DETACHED = 0,
    EVT_OBJECT_ATTACHMENT_ATTACHED = 1
} EvtObjectAttachmentState;

/* The timed action's end word is either a frame limit or an open-ended policy. */
typedef enum EvtTimedActionEndMode {
    EVT_TIMED_ACTION_END_UNBOUNDED = -1,
    EVT_TIMED_ACTION_END_PRESERVE_MOVIE_ON_RELEASE = -2
} EvtTimedActionEndMode;

typedef union EvtTimedActionEnd {
    s32 frame;
    EvtTimedActionEndMode mode;
} EvtTimedActionEnd;
typedef char EvtTimedActionEnd_size[(sizeof(EvtTimedActionEnd) == sizeof(s32)) ? 1 : -1];

/* Complete 0x84-byte entry: metadata, selected object and owned key list. */
typedef struct EvtRuntimeGroup {
    s32 type;
    union {
        struct { u8 metadataFlag; u8 pad05[3]; };
        s32 setterId;
    };
    EvtViewerNameIndex entryHeader; /* 0x08: signed runtime name-table index. */
    s32 argument0C;
    dds3Basic_t *info;
    union {
        void *resourceData; /* Generic borrowed payload for other track kinds. */
        struct EffNodeDescriptor *effectDescriptor; /* Type 3, 0x14, and 0x15. */
        struct SoundMixer *soundMixerResource; /* Type 0x12 borrowed BED template. */
        struct { u16 resourceGroup, resourceId; } resourceIds;
    };
    EvtGroupMetadata metadata;
    u8 pad20[4];
    sdfTexHandle_t *texture; /* Retained kind-0x18 texture; released with the group. */
    EvtViewerParameterKeySource parameterKeySource; /* 0x28: timeline or pending key. */
    f32 savedPosition[4];
    f32 savedRotation[4];
    EvtObjectAttachmentState attachmentState;
    s32 childCount;
    EvtRuntimeChild *children;
    EvtRuntimeChild *lastChild;
    EvtRuntimeChild *motionKeyCache[4]; /* Last applied key for each model motion channel. */
    u8 pad6C[0x10];
    struct EvtRuntimeGroup *next;
    struct EvtRuntimeGroup *prev;
} EvtRuntimeGroup;

typedef union EvtCommandArgument { s32 word; char *text; } EvtCommandArgument;
typedef union EvtFrameRange {
    s32 word;
    struct { u16 end; u16 unk02; } f;
} EvtFrameRange;

/* EvtRuntime.flags: the viewer plays its scene, set as evtViewerCreateTask's
 * task ("(ZikkiPlayMode)EventViewer") starts (evtViewerStartUpdate); and the
 * scene's end countdown has begun (evtEventViewerStartEndCountdown, which
 * Start's skip calls; evtViewerHasUpdateFlag tests it). */
#define EVT_VIEWER_FLAG_PLAYING 0x08
#define EVT_VIEWER_FLAG_ENDING 0x10

#include "event.h"

/* Find a runtime group by its name-table entry. */
EvtRuntimeGroup *mnuCampFindEntryByName(evtViewerInfo_t *scene, const char *name);

/* Start the viewer task's scheduler callback chain. */
KwlnTaskUpdateResult evtViewerStartUpdate(struct dds3ProcessID_t *task);
struct dds3ProcessID_t *evtViewerCreateTask(u32 priority, s32 eventId, s32 sceneId);

typedef char EvtRuntimeChild_size[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];
typedef char EvtGroupMetadata_size[(sizeof(EvtGroupMetadata) == 8) ? 1 : -1];
typedef char EvtGroupMetadata_option_offset[
    ((u32)&((EvtGroupMetadata *)0)->option == 0x06) ? 1 : -1];
typedef char EvtRuntimeGroup_metadata_offset[((u32)&((EvtRuntimeGroup *)0)->metadata == 0x18) ? 1 : -1];
typedef char EvtRuntimeGroup_size[(sizeof(EvtRuntimeGroup) == 0x84) ? 1 : -1];
typedef char EvtRuntimeGroup_entryHeader_at_0x08[
    ((u32)&((EvtRuntimeGroup *)0)->entryHeader == 0x08) ? 1 : -1];


EvtRuntimeChild *evtEventViewerGetPendingNode(evtViewerInfo_t *viewer);
void evtEventViewerReleaseQueuedNode(evtViewerInfo_t *viewer, EvtRuntimeGroup *entry, EvtRuntimeChild *node);
void evtViewerDispatchFlagMode(evtViewerInfo_t *viewer);
void evtEventViewerReset(evtViewerInfo_t *viewer);
void evtEventViewerShutdown(evtViewerInfo_t *viewer);
void evtEventViewerReleaseGroups(evtViewerInfo_t *viewer);
void evtViewerSetMinimumFromCurrent(evtViewerInfo_t *viewer);
void evtViewerSetMaximumFromCurrent(evtViewerInfo_t *viewer);
void fldApplyCameraColorKeyWords(evtViewerInfo_t *viewer, const EvtBlendKey *source);
#endif
