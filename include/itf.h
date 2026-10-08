#ifndef ITF_H
#define ITF_H

#include "common.h"
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

/* Retained parent/child glyph record shared by font and interface code (0x44). */
typedef struct FrFontGlyph {
    union {
        s16 h;
        struct { s8 b0; s8 b1; } b;
    } u0;
    u16 unk2;
    s32 x;
    s32 y;
    s32 advance;
    union {
        u32 word;
        u16 half[2];
        u8 byte[4];
    } u10;
    union {
        u32 w;
        u8 b[4];
    } u14;
    union {
        u32 w;
        u8 b[4];
    } unk18;
    /* Parent: first child. Font item: retained glyph-cache record. */
    union {
        struct FrFontGlyph *firstChild;
        FrFontRecord *cachedItem;
    } link1C;
    /* Parent/message: last child or shade. Font item: borrowed source. */
    union {
        struct FrFontGlyph *linkedGlyph;
        FrFontRecord *sourceItem;
    } link20;
    struct FrFontGlyph *previous;
    struct FrFontGlyph *next;
    struct FrFontGlyph *chainHead;
    u32 unk30;
    u32 unk34;
    u32 unk38;
    s32 unk3C;
    s32 unk40;
} FrFontGlyph;

/* Encoded text cursor and pending glyph updates; distinct from a glyph (0x20). */
typedef struct FrFontCtx {
    s32 x;
    s32 y;
    s32 z;
    s8 channel0;
    s8 channel1;
    s8 channel2;
    u8 channel3;
    u8 *bytes;
    FrFontGlyph *glyphChain;
    s32 offset;
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

FrFontGlyph *frFontAppendGlyphFromData(void *text, s8 fontIndex, s8 firstOption,
    s8 secondOption, FrFontGlyph *previousGlyph);
void frFontSetContextEncodedByte(FrFontGlyph *glyph, s32 inputValue);
void frFontEnableContextMode(FrFontGlyph *glyph);
void frFontSetFlagAndMeasureGlyphs(FrFontGlyph *glyph, s32 requestedFlag);
void frFontSetContextPair(FrFontGlyph *glyph, u32 first, u32 second);
void frFontStoreShiftedContextValue(FrFontGlyph *glyph, u32 unshiftedValue);
void frFontSetChildColors(FrFontGlyph *parentGlyph, u32 colorWord);

/* Message tables contain relocated encoded-text addresses. */
typedef struct ItfMesTable {
    u8 unk0[0x18];
    s16 count;
    s16 bitCount;
    u32 items[1];
} ItfMesTable;

typedef struct ItfMesEntry {
    u32 itemList;
    ItfMesTable *table;
} ItfMesEntry;

typedef struct ItfMesSub {
    u8 unk0[8];
    u32 magic;
    u8 unkC[0xC];
    u32 entryCount;
    u8 unk1C[4];
    ItfMesEntry entries[1];
} ItfMesSub;

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
    struct { s16 id; s16 value; } options[15];
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
