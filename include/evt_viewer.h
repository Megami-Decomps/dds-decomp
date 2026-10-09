#ifndef EVT_VIEWER_H
#define EVT_VIEWER_H
#include "common.h"
#include "evt_world.h"
#include "evt_polygon_movie.h"
#include "fld.h"

struct KwlnTask;

/* A timeline key's scalar formats are selected by its track kind. */
typedef union EvtViewParam {
    f32 f; s32 i; u32 u; u16 h[2]; s16 sh[2]; u8 b[4]; s8 sb[4];
} EvtViewParam;

/* Allocation-backed 0x20 payload used by type-0xB draw keys. */
typedef struct EvtViewerDrawPayload {
    f32 x, y, z, w;
    s32 mode;
    u8 unknown14[0xC];
} EvtViewerDrawPayload;
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
    void *payload; /* Track-kind-specific allocation or borrowed data. */
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
    s8 extra1;
    s8 extra2;
} EvtGroupMetadata;

/* Complete 0x84-byte entry: metadata, selected object and owned key list. */
typedef struct EvtRuntimeGroup {
    s32 type;
    union {
        struct { u8 metadataFlag; u8 pad05[3]; };
        s32 setterId;
    };
    s32 entryHeader; /* Signed runtime name-table index. */
    s32 argument0C;
    EffWorldNode *info;
    union {
        void *resourceData;
        struct { u16 resourceGroup, resourceId; } resourceIds;
    };
    EvtGroupMetadata metadata;
    u8 pad20[4];
    SdfTex *texture; /* Retained kind-0x18 texture; released with the group. */
    s32 unk28;
    f32 savedPosition[4];
    f32 savedRotation[4];
    s32 objectAttached;
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

/* One allocation retained by the EventViewer task. Camp-named timeline
 * helpers receive this same pointer; their old types omitted the voice tail. */
typedef struct EvtRuntime {
    SdfMemBlock *resourceHandle;
    u32 flags;
    PolyMovieWork *windowContext;
    s32 headerThird;
    s32 headerFirst;
    EvtFrameRange frameRange;
    s32 curFrame;
    s32 previousGlyphPosition;
    s32 entryTotal;
    char entryName[256][32];
    EffWorldNode *activeEntryIndex;
    u8 pad2028[4];
    s32 fallbackEntry;
    s32 entryCount;
    EvtRuntimeGroup *groups;
    EvtRuntimeGroup *lastGroup;
    EffWorldNode *objects[127];
    s32 unk2238;
    struct { u16 id, a, b, pad6; } history[8];
    s32 historyCount;
    s32 actionMode;
    s32 commandResetId;
    s32 unk2288; /* Cleared when committing a type-12 key option. */
    struct KwlnTask *controlState;
    s32 blurRectangleEnabled;
    s32 texturedBlurEnabled;
    s32 filterBlurEnabled;
    s32 colorRectangleEnabled;
    s32 texturedSquareEnabled;
    s32 staggeredBlurEnabled;
    s32 inputA;
    s32 groupFirst;
    u8 pad22B0[4];
    s32 groupCursor;
    s32 inputB;
    s32 cursor;
    s32 itemCount;
    char *title;
    char **itemNames;
    s32 charCol;
    s32 charRow;
    union {
        struct { char nameStorage[0x20]; s32 entryCursor; s32 entryFirst; };
        struct { u8 pad22D4[0x14]; char eventName[0x14]; };
    };
    s32 frameColumn;
    s32 frameFirst;
    s32 frameCursor;
    EvtRuntimeGroup *frameGroup;
    s32 commandCategory;
    s32 value;
    s32 valueMin;
    s32 valueMax;
    f32 floatValue;
    f32 floatMin;
    f32 floatMax;
    s32 unk2328;
    s32 unk232C;
    f32 commandMatrix[12];
    f32 savedCommandMatrix[12];
    s32 messageField;
    s32 compareField;
    s32 fieldIndex;
    s32 lightPanelOffset; /* 0x239C: light/color editor horizontal offset. */
    s32 floatSelection;
    s32 floatEditMode;
    f32 floatEditX;
    f32 floatEditY;
    f32 savedFloatEditX;
    f32 savedFloatEditY;
    u8 savedOverlayFlag;
    u8 pad23B9[3];
    s32 horizontalOffset;
    s32 updateCount;
    s8 windowShadeFade; /* +0x23C4: signed viewer shade ramp, capped at 94. */
    s8 windowActive;
    s16 unk23C6;
    s32 tableColumn;
    s32 cameraColorActive;
    SdfTex *ch71;
    SdfTex *ch72;
    SdfTex *ch76;
    SdfTex *ch75;
    s32 selectedEntry;
    s32 commandFirst;
    EvtCommandArgument commandSecond;
    EvtCommandArgument commandThird;
    s32 glyphTickCount;
    s32 editField;
    s32 framebufferQuadEnabled;
    const char *frameTextFormat; /* 0x23FC: format of the frame-list cell being measured */
    s32 frameTextWidth;          /* 0x2400: columns advanced by the formatter */
    s32 unk2404;
    s32 colorSelection;
    s32 colorEditorActive;
    u32 glyph;
    s32 timedActive;
    s32 timedStart;
    s32 timedEnd;
    u8 shadowMode;
    u8 shadowAlpha;
    u8 pad2422[2];
    f32 shadowY;
    s32 pendingWork;
    s32 pendingResource;
    s32 menuState;
    s32 shopFlag;
    u32 auxResource;
    union { s32 headerMetadata; u32 optionFlags; };
    s32 titleStreamWaitFrames;
    s32 registeredCount;
#ifdef VERSION_DDS1
    s32 registeredIds[10];
    s32 voicePending;
    s32 voiceMessage;
    s32 unk2478;
    u8 pad247C[4];
    s32 voiceFrame;
    u8 pad2484[4];
    s32 commandStart;
    s32 optionFrameBase;
#else
    s32 registeredIds[20];
    s32 voicePending;
    s32 voiceMessage;
    s32 unk24A0;
    u8 pad24A4[4];
    s32 voiceFrame;
    u8 pad24AC[4];
    s32 commandStart;
    s32 optionFrameBase;
    s32 curveComponent;
#endif
} EvtRuntime;

/* Returns the next update function as the scheduler's signed callback word. */
s32 evtViewerStartUpdate(struct KwlnTask *task);
struct KwlnTask *evtViewerCreateTask(s32 taskId, s32 event, s32 id);

typedef char EvtRuntimeChild_size[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];
typedef char EvtGroupMetadata_size[(sizeof(EvtGroupMetadata) == 8) ? 1 : -1];
typedef char EvtRuntimeGroup_metadata_offset[((u32)&((EvtRuntimeGroup *)0)->metadata == 0x18) ? 1 : -1];
typedef char EvtRuntimeGroup_size[(sizeof(EvtRuntimeGroup) == 0x84) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char EvtRuntime_size[(sizeof(EvtRuntime) == 0x2490) ? 1 : -1];
#else
typedef char EvtRuntime_size[(sizeof(EvtRuntime) == 0x24BC) ? 1 : -1];
typedef char EvtRuntime_voiceFrame_at24A8[((u32)&((EvtRuntime *)0)->voiceFrame == 0x24A8) ? 1 : -1];
#endif
typedef char EvtRuntime_camera_at23CC[((u32)&((EvtRuntime *)0)->cameraColorActive == 0x23CC) ? 1 : -1];
typedef char EvtRuntime_color_at2408[((u32)&((EvtRuntime *)0)->colorSelection == 0x2408) ? 1 : -1];
typedef char EvtRuntime_color_at240C[((u32)&((EvtRuntime *)0)->colorEditorActive == 0x240C) ? 1 : -1];

EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *viewer);
void evtViewerDispatchFlagMode(EvtRuntime *viewer);
void evtEventViewerReset(EvtRuntime *viewer);
void evtEventViewerShutdown(EvtRuntime *viewer);
void evtEventViewerReleaseGroups(EvtRuntime *viewer);
void evtViewerSetMinimumFromCurrent(EvtRuntime *viewer);
void evtViewerSetMaximumFromCurrent(EvtRuntime *viewer);
void fldApplyCameraColorKeyWords(EvtRuntime *viewer, const EvtBlendKey *source);
#endif
