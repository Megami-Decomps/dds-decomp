#ifndef EVENT_H
#define EVENT_H

#include "common.h"

typedef struct evtViewerInfo_t evtViewerInfo_t;

#include "evt_viewer.h"

/* One allocation retained by the EventViewer task. Camp-named timeline
 * helpers receive this same pointer; their old types omitted the voice tail. */
struct evtViewerInfo_t {
    sdfMemHandle_t *resourceHandle;
    u32 flags;
    PolyMovieWork *windowContext;
    s32 headerThird;
    s32 headerFirst;
    EvtFrameRange frameRange;
    s32 nowframe;
    s32 previousGlyphPosition;
    s32 entryTotal;
    char entryName[256][32];
    dds3Basic_t *activeEntryIndex;
    u8 pad2028[4];
    dds3Basic_t *fallbackEntry;
    s32 entryCount;
    EvtRuntimeGroup *groups;
    EvtRuntimeGroup *lastGroup;
    dds3Basic_t *objects[127];
    s32 unk2238;
    struct { u16 id, a, b, pad6; } history[8];
    s32 historyCount;
    s32 actionMode;
    s32 commandResetId;
    s32 cachedType12FadeAreaId; /* Last type-12 fade area applied by the viewer. */
    struct dds3ProcessID_t *controlState;
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
    s32 lightEditorSelectionIndex;
    s32 lightEditorEditMode;
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
    s16 windowResultVariableIndex; /* VM integer destination for the completed message-window result. */
    s32 tableColumn;
    s32 cameraColorActive;
    sdfTexHandle_t *ch71;
    sdfTexHandle_t *ch72;
    sdfTexHandle_t *ch76;
    sdfTexHandle_t *ch75;
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
    EvtTimedActionEnd timedEnd;
    u8 shadowMode;
    u8 shadowAlpha;
    u8 pad2422[2];
    f32 shadowY;
    u8 *pendingFrameBuffer; /* Locked byte view used as the descriptor frame image. */
    sdfMemHandle_t *pendingFrameBufferAllocation; /* Owner released after queued draws finish. */
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
    s32 backgroundFadeStartAlpha; /* Initial background alpha, interpolated to 128 over 25 updates. */
    u8 pad247C[4];
    s32 voiceFrame;
    u8 pad2484[4];
    s32 commandStart;
    s32 optionFrameBase;
#else
    s32 registeredIds[20];
    s32 voicePending;
    s32 voiceMessage;
    s32 backgroundFadeStartAlpha; /* Initial background alpha, interpolated to 128 over 25 updates. */
    u8 pad24A4[4];
    s32 voiceFrame;
    u8 pad24AC[4];
    s32 commandStart;
    s32 optionFrameBase;
    s32 curveComponent;
#endif
};

#ifdef VERSION_DDS1
typedef char evtViewerInfo_t_size[(sizeof(evtViewerInfo_t) == 0x2490) ? 1 : -1];
typedef char evtViewerInfo_t_timedEnd_at241C[((u32)&((evtViewerInfo_t *)0)->timedEnd == 0x241C) ? 1 : -1];
#else
typedef char evtViewerInfo_t_size[(sizeof(evtViewerInfo_t) == 0x24BC) ? 1 : -1];
typedef char evtViewerInfo_t_voiceFrame_at24A8[((u32)&((evtViewerInfo_t *)0)->voiceFrame == 0x24A8) ? 1 : -1];
typedef char evtViewerInfo_t_timedEnd_at241C[((u32)&((evtViewerInfo_t *)0)->timedEnd == 0x241C) ? 1 : -1];
#endif
typedef char evtViewerInfo_t_camera_at23CC[((u32)&((evtViewerInfo_t *)0)->cameraColorActive == 0x23CC) ? 1 : -1];
typedef char evtViewerInfo_t_color_at2408[((u32)&((evtViewerInfo_t *)0)->colorSelection == 0x2408) ? 1 : -1];
typedef char evtViewerInfo_t_color_at240C[((u32)&((evtViewerInfo_t *)0)->colorEditorActive == 0x240C) ? 1 : -1];
typedef char evtViewerInfo_t_pendingFrameBuffer_at2428[
    ((u32)&((evtViewerInfo_t *)0)->pendingFrameBuffer == 0x2428) ? 1 : -1];
typedef char evtViewerInfo_t_pendingFrameBufferAllocation_at242C[
    ((u32)&((evtViewerInfo_t *)0)->pendingFrameBufferAllocation == 0x242C) ? 1 : -1];

#endif /* EVENT_H */
