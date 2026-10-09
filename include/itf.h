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
    u32 unk08;
} BtlFade;

/* Common draw record allocated by the interface object constructors (0x40). */
/* Allocation fields own general-heap descriptors, not retained payload addresses. */
typedef struct UiSprite {
    SdfMemBlock *allocation;
    SdfMemBlock *payloadAllocation;
    u32 *payload;
    s32 unk0C;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 unk20;
    s32 screenY;
    s32 scrollSpan; /* Panel edge-fade divisor; both games read it as a signed word. */
    /* The secondary panel notification copies these four opaque values. */
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u8 kind; /* Unsigned index into the panel handler tables. */
    u8 pad3D[3];
} UiSprite;

/* Font items use their own 0x2C-byte pool, separate from parent chains. */
typedef struct FrFontChildGlyph {
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
    FrFontRecord *cachedItem;
    FrFontRecord *sourceItem;
    struct FrFontChildGlyph *previous;
    struct FrFontChildGlyph *next;
} FrFontChildGlyph;

typedef char FrFontChildGlyph_size_must_be_0x2C[
    sizeof(FrFontChildGlyph) == 0x2C ? 1 : -1];
typedef char FrFontChildGlyph_owner_offsets[
    ((u32)&((FrFontChildGlyph *)0)->cellDimensions == 0x18 &&
     (u32)&((FrFontChildGlyph *)0)->cachedItem == 0x1C &&
     (u32)&((FrFontChildGlyph *)0)->sourceItem == 0x20 &&
     (u32)&((FrFontChildGlyph *)0)->next == 0x28) ? 1 : -1];

/* Parent chain heads use the 0x44-byte glyph pool and own child-item links. */
typedef struct FrFontGlyph {
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
    FrFontChildGlyph *firstChild;
    FrFontChildGlyph *lastChild;
    struct FrFontGlyph *previous;
    struct FrFontGlyph *next;
    struct FrFontGlyph *chainHead;
    u32 timedControlCode; /* F214/F215 terminal timing control. */
    u32 pendingLipsStopCode; /* F117 until evtLipsStopFunction runs. */
    u32 skipLipsStopWait; /* Set when a timing control follows F117. */
    s32 remainingWaitFrames; /* F215 uses 0xFFFF for title-sound completion. */
    s32 contextModeEnabled;
} FrFontGlyph;

typedef char FrFontGlyph_size_must_be_0x44[
    sizeof(FrFontGlyph) == 0x44 ? 1 : -1];
typedef char FrFontGlyph_owner_offsets[
    ((u32)&((FrFontGlyph *)0)->childCount == 0x18 &&
     (u32)&((FrFontGlyph *)0)->firstChild == 0x1C &&
     (u32)&((FrFontGlyph *)0)->lastChild == 0x20 &&
     (u32)&((FrFontGlyph *)0)->chainHead == 0x2C) ? 1 : -1];

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
    FrFontGlyph *glyphChain;
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

void frFontSetContextEncodedByte(FrFontGlyph *glyph, s32 inputValue);
void frFontEnableContextMode(FrFontGlyph *glyph);
void frFontSetSpacingAndMeasureGlyphs(FrFontGlyph *glyph, s32 spacing);
void frFontSetGlyphPosition(FrFontGlyph *glyph, u32 x, u32 y);
void frFontStoreShiftedRenderValue(FrFontGlyph *glyph, u32 unshiftedValue);
void frFontSetChildColors(FrFontGlyph *parentGlyph, u32 colorWord);
void frFontSetChildChainFirstOption(FrFontGlyph *glyph, u8 firstOption);
void frFontSetGlyphChainDimensions(FrFontGlyph *glyph, s32 cellAdvance, s32 cellHeight);

typedef struct ItfMesBlk14 {
    u32 x;
    u32 y;
    FrFontGlyph *glyphChain;
    u16 selectedIndex;
    u8 pad0E[2];
} ItfMesBlk14;

typedef struct ItfMesEntryBlock {
    u32 x;
    s32 y;
    ItfMesTable *table;
    FrFontGlyph *glyphChain;
    s8 textState;
    u8 unk11;
    u8 color[4];
    s16 unk16;
    s16 itemIndex;
    s16 tableCount;
} ItfMesEntryBlock;

typedef struct ItfMesOption {
    s16 id;
    s16 value;
} ItfMesOption;

typedef struct ItfMesBlk40 {
    u32 x;
    u32 y;
    FrFontGlyph *glyphChain;
    u32 panelValue;
    s16 unk10;
    s16 selectedIndex;
    s16 savedIndex;
    s16 rowCount;
    s32 unk18;
    s32 unk1C;
    s16 unk20;
    s16 optionCount;
    ItfMesOption options[15];
} ItfMesBlk40;

/* The three sprite records occupy +0xA4/+0xA8/+0xAC in the window state. */
typedef struct ItfMesBlkA4 {
    UiSprite *frame;
    UiSprite *sprite;
    UiSprite *overlay;
    s32 bounds[4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 fadeLimit;
} ItfMesBlkA4;

typedef struct ItfMesTextSlots {
    u32 addresses[0x20];
    SdfMemBlock *handles[0x20];
} ItfMesTextSlots;

typedef struct ItfMesState {
    u32 flags;
    ItfMesSub *sub;
    s32 temporaryFontEntry;
    s32 renderValue;
    s16 unk10;
    s16 unk12;
    ItfMesBlk14 blk14;
    ItfMesEntryBlock entryBlock;
    ItfMesBlk40 blk40;
    u8 unkA0[4];
    ItfMesBlkA4 blkA4;
    ItfMesTextSlots textSlots;
    BtlFade fade;
    u32 callbackAddress;
} ItfMesState;

typedef struct ItfMesSlot {
    ItfMesState *mes;
    u8 unk4[0x10];
} ItfMesSlot;

void itfResetCursorPositionAndState(ItfMesBlk14 *cursor, s32 resetPosition);
void itfMesResetCursorState(ItfMesEntryBlock *cursor, s32 resetPosition);

FrFontGlyph *itfMesBuildNodeRows(u32 *items, s32 itemCount, u32 mask,
    s32 x, s32 y, s32 renderValue);
FrFontGlyph *itfMesTrimGlyphChainToRow(FrFontGlyph *node, s32 from, s32 to);
FrFontGlyph *itfMesGetLastNode(FrFontGlyph *node);
void itfMesSetNodeChainRenderValue(FrFontGlyph *node, s32 renderValue);
void itfMesRecolorNodeChildren(FrFontGlyph *node, u32 color);
s32 itfMesCountSpanSteps(FrFontGlyph *last, FrFontGlyph *first);
void itfMesCopyGlyphShade(FrFontGlyph *glyph, ItfMesEntryBlock *entryBlock);
void itfMesEnableUnflaggedNodeContexts(FrFontGlyph *node);

void itfPanelReleasePrimitiveResources(UiSprite *sprite);
FrFontGlyph *itfDrawDefaultColorText(s32 x, s32 y, u8 *encodedText, FrFontGlyph *sub);
FrFontGlyph *itfDrawCustomColorText(s32 x, s32 y, s32 channel0, s32 channel1,
    s32 channel2, s32 channel3, u8 *encodedText, FrFontGlyph *sub);
FrFontGlyph *itfDrawEncodedTextStream(s32 x, s32 y, s32 depth, s32 channel0,
    s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, FrFontGlyph *sub);
FrFontGlyph *itfDrawColor(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
    s32 channel2, s32 channel3, u8 *encodedText, FrFontGlyph *unusedSub);
FrFontGlyph *itfDrawPlainEncodedTextWithByteColors(s32 x, s32 y, s32 depth,
    s32 channel0, s32 channel1, s32 channel2, s32 channel3,
    u8 *encodedText, FrFontGlyph *unusedSub);
s32 frFontDrawGlyphInDefaultMode(FrFontGlyph *glyph);
s32 frFontDrawGlyphWithSharedFlags(FrFontGlyph *glyph, s8 mode);

/* The message manager owns a fixed pool of 64 windows in this 0x520-byte bank. */
typedef struct ItfMesPoolNode {
    struct ItfMesPoolNode *previous;
    struct ItfMesPoolNode *next;
    s32 index;
    s32 stateAddress;
    s32 resourceHandle;
} ItfMesPoolNode;

typedef struct ItfMesPool {
    ItfMesPoolNode *activeHead;
    ItfMesPoolNode *activeTail;
    ItfMesPoolNode *firstFree;
    ItfMesPoolNode *lastFree;
} ItfMesPool;

typedef struct ItfMesGlobals {
    u32 activeWindowCount;
    SdfTex *windowTexture;
    u32 unk8;
    u16 flags;
    u16 unkE;
    ItfMesPool pool;
    ItfMesPoolNode nodes[0x40];
} ItfMesGlobals;

typedef char ItfMesPoolNode_size_must_be_0x14[(sizeof(ItfMesPoolNode) == 0x14) ? 1 : -1];
typedef char ItfMesPool_size_must_be_0x10[(sizeof(ItfMesPool) == 0x10) ? 1 : -1];
typedef char ItfMesGlobals_size_must_be_0x520[(sizeof(ItfMesGlobals) == 0x520) ? 1 : -1];



#endif /* ITF_H */
