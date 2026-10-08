#ifndef EVT_VIEWER_H
#define EVT_VIEWER_H
#include "common.h"
#include "evt_world.h"
#include "evt_polygon_movie.h"
#include "fld.h"

/* A timeline key's scalar formats are selected by its track kind. */
typedef union EvtViewParam {
    f32 f; s32 i; u32 u; u16 h[2]; s16 sh[2]; u8 b[4]; s8 sb[4];
} EvtViewParam;

/* Complete 0x38-byte key allocated by the viewer and linked on a group. */
typedef struct EvtRuntimeChild {
    u16 frame;
    u16 duration; /* Also the consecutive index in type-0xB groups. */
    union {
        s32 interpolationMode;
        struct { u16 value04; u16 reserved06; };
    };
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

/* Complete 0x84-byte entry: metadata, selected object and owned key list. */
typedef struct EvtRuntimeGroup {
    s32 type;
    union {
        struct { u8 metadataFlag; u8 pad05[3]; };
        s32 setterId;
    };
    union { s32 word; u16 shortValue; } entryHeader;
    s32 argument0C;
    union {
        EffWorldNode *info;
        s32 transitionValue;
        u32 handle;
        struct PolyMovieObject *movie;
    };
    s32 argument14;
    u8 pad18[4];
    s16 metadataValue;
    s8 metadataByte1;
    s8 metadataByte2;
    u8 pad20[4];
    union { s32 entryValue; SdfTex *texture; };
    s32 unk28;
    f32 savedFirstVector[4];
    f32 savedSecondVector[4];
    s32 objectAttached;
    s32 childCount;
    EvtRuntimeChild *children;
    EvtRuntimeChild *lastChild;
    EvtRuntimeChild *cachedKey[4];
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
    s32 activeEntryIndex;
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
    u8 pad2288[4];
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
    u8 pad239C[4];
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
    u8 pad23C4;
    u8 windowActive;
    s16 unk23C6;
    s32 tableColumn;
    s32 cameraColorActive;
    s32 ch71;
    s32 ch72;
    s32 ch76;
    s32 ch75;
    s32 selectedEntry;
    s32 commandFirst;
    EvtCommandArgument commandSecond;
    EvtCommandArgument commandThird;
    s32 glyphTickCount;
    s32 editField;
    s32 framebufferQuadEnabled;
    u8 pad23FC[8];
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
    u8 pad248C[4];
#else
    s32 registeredIds[20];
    s32 voicePending;
    s32 voiceMessage;
    s32 unk24A0;
    u8 pad24A4[0xC];
    s32 commandStart;
    u8 pad24B4[4];
    s32 curveComponent;
#endif
} EvtRuntime;

typedef char EvtRuntimeChild_size[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];
typedef char EvtRuntimeGroup_size[(sizeof(EvtRuntimeGroup) == 0x84) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char EvtRuntime_size[(sizeof(EvtRuntime) == 0x2490) ? 1 : -1];
#else
typedef char EvtRuntime_size[(sizeof(EvtRuntime) == 0x24BC) ? 1 : -1];
#endif
typedef char EvtRuntime_camera_at23CC[((u32)&((EvtRuntime *)0)->cameraColorActive == 0x23CC) ? 1 : -1];
typedef char EvtRuntime_color_at2408[((u32)&((EvtRuntime *)0)->colorSelection == 0x2408) ? 1 : -1];
typedef char EvtRuntime_color_at240C[((u32)&((EvtRuntime *)0)->colorEditorActive == 0x240C) ? 1 : -1];

EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *viewer);
void fldApplyCameraColorKeyWords(EvtRuntime *viewer, const EvtBlendKey *source);
#endif
