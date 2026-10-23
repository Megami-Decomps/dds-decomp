#ifndef ITF_H
#define ITF_H

#include "common.h"
#include "itf_mes_resource.h"
#include "eff.h"
#include "sdf.h"
#include "fr_font.h"

/* Native 0xC-byte fade record shared by message windows and sound UI state. */
typedef struct BtlFade {
    u8 kind;
    u8 pad1;
    s16 phase;
    s16 alpha;
    s16 timer;
    u32 textAdvanceFrameCount; /* 0x08: entry advances count frames before triangle skip. */
} BtlFade;

typedef char BtlFade_size_must_be_0x0C[(sizeof(BtlFade) == 0x0C) ? 1 : -1];
typedef char BtlFade_textAdvanceFrameCount_offset_must_be_0x08[
    ((u32)&((BtlFade *)0)->textAdvanceFrameCount == 0x08) ? 1 : -1];

/* Common draw record allocated by the interface object constructors (0x40). */
/* Allocation fields own general-heap descriptors, not retained payload addresses. */
typedef struct itfpanel_t {
    sdfMemHandle_t *allocation;
    sdfMemHandle_t *payloadAllocation;
    u32 *payload;
    s32 renderValue;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 unk20;
    s32 screenY;
    s32 scrollSpan; /* Panel edge-fade divisor; both games read it as a signed word. */
    /* RGBA words consumed by the panel packet renderers. */
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    u8 kind; /* Unsigned index into the panel handler tables. */
    u8 pad3D[3];
} itfpanel_t;

/* Font items use their own 0x2C-byte pool, separate from parent chains. */
typedef struct _TEXT {
    union {
        u16 glyphCode;
        struct {
            u8 encodedContextByte;
            s8 spacing;
        } byteRoles;
    } glyphCodeOrContext;
    /* Child draw count: setup clears it; nonzero-fade draws increment it, and
     * the fade helper also uses it to select the jitter phase. */
    u16 drawCount;
    s32 x;
    s32 y;
    s32 advance;
    /* The same render-state word is also accessed by its low fade byte. */
    union {
        struct {
            u16 cellAdvance;
            u16 cellHeight;
        } parentDimensions;
        u32 renderWord;
        struct {
            u8 value;
            u8 opaque[3];
        } fadeByte;
    } parentDimensionsOrRenderWord;
    /* Rendering, glyph setup, and message shade paths use distinct byte views. */
    union {
        u32 renderValue;
        struct {
            u8 firstOption;
            u8 fontIndex;
            u8 secondOption;
            u8 sharedFlags;
        } setupBytes;
        struct {
            u8 green;
            u8 red;
            u8 blue;
            u8 opaque;
        } shadeColor;
    } renderValueOrSetupOrShade;
    struct {
        u8 cellWidth;
        u8 cellHeight;
        u8 opaque[2];
    } cellDimensions;
    GsFont_t *cachedItem;
    GsFont_t *sourceItem;
    struct _TEXT *previous;
    struct _TEXT *next;
} _TEXT;

typedef char FrFontChildGlyph_size_must_be_0x2C[
    sizeof(_TEXT) == 0x2C ? 1 : -1];
typedef char FrFontChildGlyph_owner_offsets[
    ((u32)&((_TEXT *)0)->cellDimensions == 0x18 &&
     (u32)&((_TEXT *)0)->cachedItem == 0x1C &&
     (u32)&((_TEXT *)0)->sourceItem == 0x20 &&
     (u32)&((_TEXT *)0)->next == 0x28) ? 1 : -1];

/* Parent chain heads use the 0x44-byte glyph pool and own child-item links. */
typedef struct _FRQ {
    union {
        u16 glyphCode;
        struct {
            u8 encodedContextByte;
            s8 spacing;
        } byteRoles;
    } glyphCodeOrContext;
    /* Preserved by parent-chain initialization. */
    u16 drawCount;
    s32 x;
    s32 y;
    s32 advance;
    /* Parent glyphs store dimensions; render words also carry a fade byte. */
    union {
        struct {
            u16 cellAdvance;
            u16 cellHeight;
        } parentDimensions;
        u32 renderWord;
        struct {
            u8 value;
            u8 opaque[3];
        } fadeByte;
    } parentDimensionsOrRenderWord;
    /* Rendering, glyph setup, and message shade paths use distinct byte views. */
    union {
        u32 renderValue;
        struct {
            u8 firstOption;
            u8 fontIndex;
            u8 secondOption;
            u8 sharedFlags;
        } setupBytes;
        struct {
            u8 green;
            u8 red;
            u8 blue;
            u8 opaque;
        } shadeColor;
    } renderValueOrSetupOrShade;
    u32 childCount;
    _TEXT *firstChild;
    _TEXT *lastChild;
    struct _FRQ *previous;
    struct _FRQ *next;
    struct _FRQ *chainHead;
    u32 timedControlCode; /* F214/F215 terminal timing control. */
    u32 pendingLipsStopCode; /* F117 until evtLipsStopFunction runs. */
    u32 skipLipsStopWait; /* Set when a timing control follows F117. */
    s32 remainingWaitFrames; /* F215 uses 0xFFFF for title-sound completion. */
    s32 contextModeEnabled;
} _FRQ;

typedef char FrFontGlyph_size_must_be_0x44[
    sizeof(_FRQ) == 0x44 ? 1 : -1];
typedef char FrFontGlyph_owner_offsets[
    ((u32)&((_FRQ *)0)->childCount == 0x18 &&
     (u32)&((_FRQ *)0)->firstChild == 0x1C &&
     (u32)&((_FRQ *)0)->lastChild == 0x20 &&
     (u32)&((_FRQ *)0)->chainHead == 0x2C) ? 1 : -1];

/* Encoded message controls consumed by the glyph timing/lipsync state. */
typedef enum ItfGlyphControlCode {
    ITF_GLYPH_CONTROL_WAIT_FRAMES = 0xF214,
    ITF_GLYPH_CONTROL_WAIT_FRAME_OR_SOUND = 0xF215,
    ITF_GLYPH_CONTROL_STOP_LIPS = 0xF117,
} ItfGlyphControlCode;

#define ITF_GLYPH_WAIT_FOR_SOUND_SENTINEL 0xFFFF

/* Encoded text cursor and pending glyph updates; distinct from a glyph (0x20). */
typedef struct FrFontCtx {
    s32 x;
    s32 y;
    s32 z;
    s8 fontIndex;
    s8 firstOption;
    s8 secondOption;
    u8 contextEncodedByte;
    u8 *encodedText;
    _FRQ *glyphChain;
    s32 encodedTextOffset;
    s8 pendingCreate;
    s8 pendingPosition;
    u8 pad1E[2];
} FrFontCtx;

/* Serialized font-bank entries use byte offsets relative to their bank. */
typedef struct FrFontTextIndex {
    u32 unk00;
    u32 offset;
} FrFontTextIndex;

typedef struct FrFontTextBank {
    u8 pad00[0x18];
    s32 count;
    u8 pad1C[4];
    FrFontTextIndex entries[1];
} FrFontTextBank;

void frSetDrawSpeed(_FRQ *glyph, s32 inputValue);
void frFontEnableContextMode(_FRQ *glyph);
void frSetFontSpace(_FRQ *glyph, s32 spacing);
void frSetDrawPosition(_FRQ *glyph, u32 x, u32 y);
void frSetPriority(_FRQ *glyph, u32 unshiftedValue);
void frSetFontColor(_FRQ *parentGlyph, u32 colorWord);
void frSetFontColorNo(_FRQ *glyph, u8 firstOption);
void frSetFontSize(_FRQ *glyph, s32 cellAdvance, s32 cellHeight);

typedef struct ItfMesBlk14 {
    u32 x;
    u32 y;
    _FRQ *glyphChain;
    u16 selectedIndex;
    u8 pad0E[2];
} ItfMesBlk14;

typedef struct ItfMesEntryBlock {
    u32 x;
    s32 y;
    ItfMesTable *table;
    _FRQ *glyphChain;
    s8 textState;
    u8 textInterfaceMask; /* 0x11: result of frGetMessageResult(3). */
    u8 color[4];
    s16 lineCount;
    s16 itemIndex;
    s16 tableCount;
} ItfMesEntryBlock;

typedef char ItfMesEntryBlock_size_must_be_0x1C[
    (sizeof(ItfMesEntryBlock) == 0x1C) ? 1 : -1];
typedef char ItfMesEntryBlock_textInterfaceMask_offset_must_be_0x11[
    ((u32)&((ItfMesEntryBlock *)0)->textInterfaceMask == 0x11) ? 1 : -1];

typedef struct ItfMesOption {
    s16 id;
    s16 value;
} ItfMesOption;

typedef struct ItfMesBlk40 {
    u32 x;
    u32 y;
    _FRQ *glyphChain;
    u32 panelValue;
    s16 selectionState;
    s16 selectedIndex;
    s16 savedIndex;
    s16 rowCount;
    s32 widestRowX;     /* the widest row's x, which the option panel's highlight spans from */
    s32 widestRowWidth; /* the widest row's width */
    s16 glyphColorIntensity;
    s16 optionCount;
    ItfMesOption options[15];
} ItfMesBlk40;

/* The three sprite records occupy +0xA4/+0xA8/+0xAC in the window state. */
typedef struct ItfMesBlkA4 {
    itfpanel_t *frame;
    itfpanel_t *sprite;
    itfpanel_t *overlay;
    s32 bounds[4];
    s32 red;
    s32 green;
    s32 blue;
    s32 fadeLimit;
} ItfMesBlkA4;

typedef struct ItfMesTextSlots {
    u8 *addresses[0x20];
    sdfMemHandle_t *handles[0x20];
} ItfMesTextSlots;

typedef struct instanceMes_tag {
    u32 flags;
    ItfMesSub *sub;
    u8 *temporaryFontEntry; /* 0x08: relocated font resource bytes from the MSG1 trailing entry. */
    s32 renderValue;
    s16 drawSurfaceIndex;
    s16 panelMode;
    ItfMesBlk14 blk14;
    ItfMesEntryBlock entryBlock;
    ItfMesBlk40 blk40;
    u8 unkA0[4];
    ItfMesBlkA4 blkA4;
    ItfMesTextSlots textSlots;
    BtlFade fade;
    void (*entryGlyphCallback)(void);
} instanceMes_tag;

typedef char ItfMesState_temporaryFontEntry_offset_must_be_0x08[
    ((u32)&((instanceMes_tag *)0)->temporaryFontEntry == 0x08) ? 1 : -1];

typedef struct ItfMesSlot {
    instanceMes_tag *mes;
    u8 unk4[0x10];
} ItfMesSlot;

void InitializeSpeaker(ItfMesBlk14 *cursor, s32 resetPosition);
void InitializeMessage(ItfMesEntryBlock *cursor, s32 resetPosition);

_FRQ *itfMesBuildNodeRows(u32 *items, s32 itemCount, u32 mask,
    s32 x, s32 y, s32 renderValue);
_FRQ *UnchainFontHandleUnit(_FRQ *glyph, s32 from, s32 to);
_FRQ *GetLastLinkFontHandle(_FRQ *glyph);
void ChangeZBuffer(_FRQ *glyph, s32 renderValue);
void ChangeFontAlpha(_FRQ *glyph, u32 color);
s32 CountLineMessage(_FRQ *last, _FRQ *first);
void GetLastMessagePara(_FRQ *glyph, ItfMesEntryBlock *entryBlock);
void itfMesEnableUnflaggedNodeContexts(_FRQ *glyph);

void itfPnlRelease(itfpanel_t *sprite);
_FRQ *MessageRequest(s32 x, s32 y, u8 *encodedText, _FRQ *sub);
_FRQ *MessageRequestEx(s32 x, s32 y, s32 channel0, s32 channel1,
    s32 channel2, s32 channel3, u8 *encodedText, _FRQ *sub);
_FRQ *MessageRequestEx2(s32 x, s32 y, s32 depth, s32 channel0,
    s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, _FRQ *sub);
_FRQ *itfDrawColor(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
    s32 channel2, s32 channel3, u8 *encodedText, _FRQ *unusedSub);
_FRQ *itfDrawPlainEncodedTextWithByteColors(s32 x, s32 y, s32 depth,
    s32 channel0, s32 channel1, s32 channel2, s32 channel3,
    u8 *encodedText, _FRQ *unusedSub);
s32 frFontDrawGlyphInDefaultMode(_FRQ *glyph);
s32 frFontDrawGlyphWithSharedFlags(_FRQ *glyph, s8 mode);

/* The message manager owns a fixed pool of 64 windows in this 0x520-byte bank. */
typedef struct mesflow_tag {
    struct mesflow_tag *previous;
    struct mesflow_tag *next;
    s32 index;
    instanceMes_tag *state;
    sdfMemHandle_t *resourceHandle;
} mesflow_tag;

/* State of the interactive message-layout inspector. */
typedef struct ItfMesDebugState {
    s32 selectedItem;
    s32 mode;
    mesflow_tag *window;
} ItfMesDebugState;

typedef struct ItfMesPool {
    mesflow_tag *activeHead;
    mesflow_tag *activeTail;
    mesflow_tag *firstFree;
    mesflow_tag *lastFree;
} ItfMesPool;

typedef struct ItfMesGlobals {
    u32 activeWindowCount;
    sdfTexHandle_t *windowTexture;
    u32 unk8;
    u16 flags;
    u16 unkE;
    ItfMesPool pool;
    mesflow_tag nodes[0x40];
} ItfMesGlobals;

typedef char ItfMesPoolNode_size_must_be_0x14[(sizeof(mesflow_tag) == 0x14) ? 1 : -1];
typedef char ItfMesPoolNode_resourceHandle_offset_must_be_0x10[
    ((u32)&((mesflow_tag *)0)->resourceHandle == 0x10) ? 1 : -1];
typedef char ItfMesPool_size_must_be_0x10[(sizeof(ItfMesPool) == 0x10) ? 1 : -1];
typedef char ItfMesGlobals_size_must_be_0x520[(sizeof(ItfMesGlobals) == 0x520) ? 1 : -1];

/* Native interior symbols alias these fields without allocating another object. */
typedef char ItfMesGlobals_windowTexture_offset_must_be_0x4[
    ((u32)&((ItfMesGlobals *)0)->windowTexture == 0x4) ? 1 : -1];
typedef char ItfMesGlobals_flags_offset_must_be_0xC[
    ((u32)&((ItfMesGlobals *)0)->flags == 0xC) ? 1 : -1];
typedef char ItfMesGlobals_pool_offset_must_be_0x10[
    ((u32)&((ItfMesGlobals *)0)->pool == 0x10) ? 1 : -1];
typedef char ItfMesGlobals_nodes_offset_must_be_0x20[
    ((u32)&((ItfMesGlobals *)0)->nodes == 0x20) ? 1 : -1];
typedef char ItfMesGlobals_firstStateAddress_offset_must_be_0x2C[
    ((u32)&((ItfMesGlobals *)0)->nodes[0].state == 0x2C) ? 1 : -1];


/* Message-window pool operations move nodes between the free and active lists.
 * Acquire returns NULL when the free list is empty; release expects an active
 * node belonging to the supplied pool and appends it to the free-list tail. */
void itfMiscLinkInitialize(ItfMesPool *pool, mesflow_tag *nodes, s32 count, s32 stride);
mesflow_tag *itfMiscLinkTake(ItfMesPool *pool);
void itfMiscLinkGive(mesflow_tag *node, ItfMesPool *pool);

/* Return a retained texture handle after releasing the temporary asset allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *path);



#endif /* ITF_H */
