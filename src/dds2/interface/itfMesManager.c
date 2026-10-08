#include "common.h"
#include "itf.h"

void sdfRelocatePackedResourceWords(int *param_1, int param_2, u8 *param_3, int param_4);

extern u32 itfMessageFlags;

typedef struct ItfMesWindowRec ItfMesWindowRec;


extern ItfMesSlot itfWindowSlots[];


extern ItfMesGlobals itfMesWork;

#define ITF_MES_MAGIC_MSG0 0x3047534d
#define ITF_MES_MAGIC_MSG1 0x3147534d

typedef struct ItfMesRelocResource {
    u8 pad00[8];
    u32 magic;
    u8 pad0C[4];
    s32 fixupOffset;
    s32 fixupCount;
    u8 pad18[4];
    u8 relocated;
    u8 pad1D[3];
    u8 payload[1];
} ItfMesRelocResource;

/* State of the interactive message-layout inspector. */
typedef struct ItfMesDebugState {
    s32 selectedItem;
    s32 mode;
    ItfMesWindowRec *window;
} ItfMesDebugState;

extern ItfMesDebugState D_003B4770;


ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 index);

ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub);

u32 itfMesGetTableItem(ItfMesTable *table, s32 index);




s32 scrGetWindow(void);

s32 scrReadIntParameter(s32 parameterIndex);

void itfMesSetWindowPanelValue(s32 window, u32 panelMask);

void itfMesCountClearBits(s32 window, s32 selectedBitIndex);

void func_00154F18(s32);

void itfMesSetWindowHighFlags(s32 window, u32 mask);

void itfMesSetTextSlotFromValue(s32 window, s32 slotIndex, s32 value, s32 selector);

void itfMesClearWindowHighFlags(s32 window, u32 mask);

void itfMesBlk24MoveTo(s32 window, s32 x, s32 y);

void itfMesSetWindowPageAndRefresh(s32 window, s32 firstValue, s32 secondValue);

void func_001A4A10(s32 window, s32 first, s32 second);

void itfMesFinishWindowAndClearStatus();

void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock);

void itfMesResetWindow(s32 window);

void itfMesDestroyWindow(s32 window);

s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *glyph);

void itfMesResetCursorState(void *, s32);

void itfResetCursorPositionAndState(void *, s32);

void itfMesInitCharTable(s32 *table);

s32 itfMesMaxGroupedExtent(FrFontGlyph *node);

extern void frFontLoadTemporaryEntry(u32);


extern SdfTex *itfLoadTextureFromAsset(const char *path);

extern void itfInitPool(ItfMesPool *pool, ItfMesPoolNode *nodes, s32 count, s32 stride);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern s32 sndVisitQueuedResources(void);

extern void sndFlushMessageQueue(void);

extern void func_001A76C8(void);

extern struct ItfMesPoolNode *itfAcquirePoolNode();

extern SdfMemBlock *sdfAllocGeneralBlock(s32);
extern u32 sdfResourceRetainAddress(SdfMemBlock *);
extern void sdfReleaseResourceAllocation(SdfMemBlock *);
extern u32 strlen(const char *);
extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);
void func_001A5480();

extern void itfInitializeCursorResetState();

extern void itfResetWindowResourceBlock();

extern void itfClearDrawStateWords();

extern void itfResetBattleFadeState();

extern void func_0019DD48();

extern s32 func_0019DBA8();

extern UiSprite *func_001A1858(s32 kind, u32 payload);

extern void itfSetPanelLayoutAndNotify();

extern void itfPanelUpdateValuesAndNotify();


#define ITF_MES_SCRIPT_PANEL_BIT 0x200000
#define ITF_MES_LOWER_STATUS_PAIR_MASK 0x300
#define ITF_MES_LOWER_STATUS_FIRST_BIT 0x100
#define ITF_MES_LOWER_STATUS_SECOND_BIT 0x200
#define ITF_MES_UPPER_STATUS_PAIR_MASK 0x3000
#define ITF_MES_CONTENT_STATUS_MASK 0x3300
#define ITF_MES_CLEAR_SCRIPT_PANEL_BIT 0xFFDFFFFF
#define ITF_MES_SKIP_CONTEXT_ENABLE_BIT 0x400000
#define ITF_MES_TEXT_X_SHIFT 4
#define ITF_MES_TEXT_Y_SHIFT 3
#define ITF_MES_DEFAULT_TEXT_X 0x4B0
#define ITF_MES_DEFAULT_TEXT_Y 0xAF8
#define ITF_MES_OPTION_STATE_MASK 0x38
#define ITF_MES_OPTION_LIST_READY 0x10
#define ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK 0xFFFBFFFF
#define ITF_MES_ENTRY_STATE_MASK 7
#define ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK 0xFFFDFFFF
#define ITF_MES_HIGH_FLAGS_MASK 0xffff0000
#define ITF_MES_LOW_FLAGS_MASK 0xffff
#define ITF_MES_WINDOW_CAPACITY 0x40
#define ITF_MES_POOL_NODE_BYTES 0x14
#define ITF_MES_OPTION_ROW_SPACING 25
#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00
#define ITF_MES_DEFAULT_COLOR_VALUE 0x80
#define ITF_MES_TEXT_SLOT_COUNT 0x20
#define ITF_MES_MASK_BIT_COUNT 0x20
#define ITF_MES_STRING_COPY_ROUND_BIAS 4
#define ITF_MES_BINARY_COPY_ROUND_BIAS 5
#define ITF_MES_COPY_ALIGN_MASK 3

/* Store the script's panel mask; a negative script window is a no-op. Return 1. */
s32 itfMesScriptSetPanelValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowPanelValue(window, scrReadIntParameter(0));
    return 1;
}

/* Cache the clear-bit rank for the script's selected bit index; return 1. */
s32 itfMesScriptSetWindowValue(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesCountClearBits(window, scrReadIntParameter(0));
    return 1;
}

/* Request script panel activation, then notify the script-side state routine. */
s32 itfMesScriptActivatePanel(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowHighFlags(window, ITF_MES_SCRIPT_PANEL_BIT);
    func_00154F18(2);
    return 1;
}

/* Finish single-bit states of the lower status pair; return whether its original
 * value was zero. Finishing a nonzero state therefore still returns 0. */
s32 itfMesFinishScriptWindowIfActive(void) {
    s32 window = scrGetWindow();
    ItfMesState *mes;
    u32 scriptStatus;

    if (window < 0) {
        return 1;
    }
    mes = itfWindowSlots[window].mes;
    scriptStatus = mes->flags & ITF_MES_LOWER_STATUS_PAIR_MASK;
    if (scriptStatus == ITF_MES_LOWER_STATUS_FIRST_BIT || scriptStatus == ITF_MES_LOWER_STATUS_SECOND_BIT) {
        itfMesFinishWindowAndClearStatus(window);
        func_00154F18(3);
    }
    return scriptStatus < 1;
}

/* Mark each originally nonzero status pair with both bits, then clean/reset
 * content and clear the script panel bit. The status pairs are set, not cleared. */
void itfMesFinishWindowAndClearStatus(s32 window, s32 unused1, s32 unused2) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    u32 previousFlags = mes->flags;

    if (previousFlags & ITF_MES_LOWER_STATUS_PAIR_MASK) {
        mes->flags = previousFlags | ITF_MES_LOWER_STATUS_PAIR_MASK;
    }
    if (previousFlags & ITF_MES_UPPER_STATUS_PAIR_MASK) {
        mes->flags = mes->flags | ITF_MES_UPPER_STATUS_PAIR_MASK;
    }
    itfMesCleanupWindow(window, 1);
    itfMesResetWindow(window);
    mes->flags &= ITF_MES_CLEAR_SCRIPT_PANEL_BIT;
}

u32 func_001A3560(void) {
    return 1;
}

u32 func_001A3568(void) {
    return 1;
}

/* Read the text-slot index, value and selector from script parameters. */
s32 itfMesScriptSetTextSlotFromValue(void) {
    s32 window = scrGetWindow();
    s32 slotIndex;
    s32 value;
    s32 selector;

    if (window < 0) {
        return 1;
    }
    slotIndex = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    selector = scrReadIntParameter(2);
    itfMesSetTextSlotFromValue(window, slotIndex, value, selector);
    return 1;
}

/* Toggle suppression of automatic glyph-context enabling from script argument 0. */
s32 itfMesScriptToggleMessageFlag(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (scrReadIntParameter(0)) {
        itfMesSetWindowHighFlags(window, ITF_MES_SKIP_CONTEXT_ENABLE_BIT);
    } else {
        itfMesClearWindowHighFlags(window, ITF_MES_SKIP_CONTEXT_ENABLE_BIT);
    }
    return 1;
}

/* Convert script coordinates to the text units (x * 16, y * 8); return 1. */
s32 itfMesScriptSetScaledPosition(void) {
    s32 window = scrGetWindow();
    s32 x;
    s32 y;

    if (window < 0) {
        return 1;
    }
    x = scrReadIntParameter(0);
    y = scrReadIntParameter(1);
    itfMesBlk24MoveTo(window, x << ITF_MES_TEXT_X_SHIFT, y << ITF_MES_TEXT_Y_SHIFT);
    return 1;
}

/* Move selected-entry text to the fixed default position; return 1. */
s32 itfMesScriptSetDefaultBounds(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesBlk24MoveTo(window, ITF_MES_DEFAULT_TEXT_X, ITF_MES_DEFAULT_TEXT_Y);
    return 1;
}

/* The two opaque high bits use inverted script polarity: true clears both.
 * Keep the two separate calls and their original order. */
s32 itfMesScriptToggleHighFlags(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    if (scrReadIntParameter(0)) {
        itfMesClearWindowHighFlags(window, 0x800000);
        itfMesClearWindowHighFlags(window, 0x100000);
    } else {
        itfMesSetWindowHighFlags(window, 0x800000);
        itfMesSetWindowHighFlags(window, 0x100000);
    }
    return 1;
}

/* Refresh the script window with argument 0 and a zero second value; return 1. */
s32 itfMesScriptSetMessageOption(void) {
    s32 window = scrGetWindow();

    if (window < 0) {
        return 1;
    }
    itfMesSetWindowPageAndRefresh(window, scrReadIntParameter(0), 0);
    return 1;
}

/* Read both refresh values in order; a negative script window is a no-op. */
s32 itfMesScriptSetMessagePair(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    itfMesSetWindowPageAndRefresh(window, first, second);
    return 1;
}

/* Pass both script range values in order; a negative script window is a no-op. */
s32 itfMesScriptSetMessageRange(void) {
    s32 window = scrGetWindow();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_001A4A10(window, first, second);
    return 1;
}

/* Return the loaded message-window texture resource, not a window index. */
SdfTex *itfMesGetGlobalWindowValue(void) {
    return itfMesWork.windowTexture;
}

/* Set requested bits in the separate message-global word. */
void itfMesSetFlags(u32 mask) {
    itfMessageFlags = itfMessageFlags | mask;
}

/* Clear requested bits in the separate message-global word. */
void itfMesClearFlags(u32 mask) {
    itfMessageFlags = itfMessageFlags & ~mask;
}

s32 itfMesCreateWindow(ItfMesSub *sub) {
    ItfMesPoolNode *node = itfAcquirePoolNode(&itfMesWork.pool);
    s32 window = node->index;
    ItfMesState *mes;
    u32 handle;

    handle = (u32)sdfAllocGeneralBlock(0x1E0);
    node->resourceHandle = handle;
    mes = (ItfMesState *)sdfResourceRetainAddress((SdfMemBlock *)handle);
    node->stateAddress = (s32)mes;
    mes->sub = NULL;
    itfMesSetSubResource(window, sub);
    mes->flags = 0;
    mes->unk10 = 0x53;
    mes->renderValue = 0xFFFFF0;
    mes->unk12 = 0;
    mes->callbackAddress = 0;
    itfResetCursorPositionAndState(&mes->blk14, 1);
    itfMesResetCursorState(&mes->entryBlock, 1);
    itfInitializeCursorResetState(&mes->blk40);
    itfResetWindowResourceBlock(&mes->blkA4);
    itfClearDrawStateWords(&mes->textSlots);
    itfResetBattleFadeState(&mes->fade, 0);
    itfMesWork.activeWindowCount++;
    return window;
}

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesDestroyWindowIfPresent(s32 window) {
    if (window >= 0 && itfWindowSlots[window].mes != NULL) {
        itfMesDestroyWindow(window);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", itfMesStartEntry);

extern s32 itfInitTextDrawArgs(u8 *encodedText, FrFontGlyph *sub);

/* Cache the selected table and narrowed item index, then initialize its text.
 * Empty tables return 0; nonempty tables return the text initializer's result. */
s32 itfMesSelectTableItemText(s32 window, s32 entryIndex, s32 itemIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesTable *table = itfMesGetEntry(mes, entryIndex)->table;
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;

    entryBlock->itemIndex = itemIndex;
    entryBlock->table = table;
    entryBlock->tableCount = table->count;
    if (table->count != 0) {
        return itfInitTextDrawArgs((u8 *)itfMesGetTableItem(table, (s16)itemIndex), 0);
    }
    return 0;
}

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock) {
    ItfMesState *mes;
    ItfMesEntryBlock *entryBlock;
    ItfMesBlk14 *primaryTextBlock;

    if (window < 0) {
        return;
    }
    mes = itfWindowSlots[window].mes;
    entryBlock = &mes->entryBlock;
    primaryTextBlock = &mes->blk14;
    if (entryBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(entryBlock->glyphChain);
        entryBlock->glyphChain = NULL;
    }
    itfMesResetCursorState(entryBlock, 0);
    mes->flags &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->flags &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (releasePrimaryBlock == 0) {
        return;
    }
    if (primaryTextBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(primaryTextBlock->glyphChain);
        primaryTextBlock->glyphChain = NULL;
    }
    itfResetCursorPositionAndState(primaryTextBlock, 0);
}

/* Build the option panel around the widest measured row, using this game's
 * fixed row spacing. Preserve the existing panel creation/layout order. */
void itfMesBuildOptionFrame(ItfMesState *mes) {
    ItfMesBlk40 *optionBlock = &mes->blk40;
    ItfMesBlkA4 *panelBlock = &mes->blkA4;
    s32 bounds[4];
    s32 maxRowWidth = 0;
    s32 rowsHeight;
    s32 halfWidth;
    s32 rowIndex;
    s32 rowWidth;

    func_0019DD48(0x1000, 0x4B0, optionBlock->glyphChain);
    rowsHeight = optionBlock->rowCount * ITF_MES_OPTION_ROW_SPACING * 8;
    for (rowIndex = 0; rowIndex < optionBlock->rowCount; rowIndex++) {
        rowWidth = func_0019DBA8(rowIndex, optionBlock->glyphChain);
        if (maxRowWidth < rowWidth) {
            maxRowWidth = rowWidth;
        }
    }
    halfWidth = maxRowWidth / 2;
    bounds[0] = 0xE00 - halfWidth;
    bounds[1] = 0x430;
    bounds[2] = 0x1200 + halfWidth;
    bounds[3] = 0x530 + rowsHeight;
    panelBlock->overlay = func_001A1858(9, (u32)itfMesWork.windowTexture);
    itfSetPanelLayoutAndNotify(panelBlock->overlay, bounds[0], bounds[1], bounds[2], bounds[3], mes->renderValue);
    itfPanelUpdateValuesAndNotify(panelBlock->overlay, 0, 0, 0, 0);
    mes->flags = (mes->flags & ~0xC00) | 0x400;
}

/* Replace the option glyph chain with rows selected by clear panel-mask bits.
 * Item zero is skipped; mode 3 additionally builds the surrounding frame. */
void itfMesBuildOptionList(s32 window, s32 entryIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlk40 *optionBlock = &mes->blk40;
    ItfMesEntry *entry = itfMesGetEntry(mes, entryIndex);
    ItfMesTable *table;
    s32 visibleRowCount;
    s32 firstRowY;

    if (optionBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(optionBlock->glyphChain);
        optionBlock->glyphChain = NULL;
    }
    table = entry->table;
    visibleRowCount = itfMesCountZeroBits(table->bitCount, optionBlock->panelValue);
    firstRowY = optionBlock->y - ((visibleRowCount - 1) * ITF_MES_OPTION_ROW_SPACING << ITF_MES_TEXT_Y_SHIFT);
    itfMesInitCharTable((s32 *)mes->textSlots.addresses);
    if (mes->temporaryFontEntry != 0) {
        frFontLoadTemporaryEntry(mes->temporaryFontEntry);
    }
    optionBlock->glyphChain = (FrFontGlyph *)itfMesBuildNodeRows(&table->items[1], table->bitCount, optionBlock->panelValue, optionBlock->x, firstRowY, mes->renderValue);
    optionBlock->rowCount = visibleRowCount;
    mes->flags = (mes->flags & ~ITF_MES_OPTION_STATE_MASK) | ITF_MES_OPTION_LIST_READY;
    if (mes->unk12 == 3) {
        itfMesBuildOptionFrame(mes);
    }
    optionBlock->unk10 = 1;
}

/* Release option glyphs, clear option selection/state and invalidate its rank.
 * Other content and the window allocation remain installed. */
void itfMesResetWindow(s32 window) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlk40 *optionBlock = &mes->blk40;
    if (optionBlock->glyphChain != NULL) {
        frFontQueueGlyphInSelectedSlot(optionBlock->glyphChain);
        optionBlock->glyphChain = NULL;
    }
    optionBlock->panelValue = 0;
    optionBlock->unk10 = 0;
    optionBlock->rowCount = 0;
    optionBlock->selectedIndex = -1;
    mes->flags &= ~ITF_MES_OPTION_STATE_MASK;
    mes->flags &= ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK;
}

/* Store the mask whose clear bits select displayed option rows. */
void itfMesSetWindowPanelValue(s32 window, u32 panelMask) {
    itfWindowSlots[window].mes->blk40.panelValue = panelMask;
}

/* Cache the number of clear bits below selectedBitIndex only when that bit is
 * itself clear; a set bit produces zero. Negative indices also produce zero;
 * no upper bound is checked. */
void itfMesCountClearBits(s32 window, s32 selectedBitIndex) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;
    u32 remainingMask = optionBlock->panelValue;
    s32 bitValue = remainingMask & 1;
    s32 clearBitCount = bitValue == 0;
    s32 bitIndex = 0;
    if (selectedBitIndex > 0) {
        do {
            bitIndex++;
            remainingMask >>= 1;
            bitValue = remainingMask & 1;
            if (bitValue == 0) {
                clearBitCount++;
            }
        } while (bitIndex < selectedBitIndex);
    }
    clearBitCount = bitValue == 0 ? clearBitCount - 1 : 0;
    optionBlock->selectedIndex = clearBitCount;
    optionBlock->savedIndex = clearBitCount;
}

void itfMesOffsetNodeChain(FrFontGlyph *node, s32 dx, s32 dy);

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesBlk14MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk14 *primaryTextBlock = &itfWindowSlots[window].mes->blk14;
    s32 delta[2];
    delta[0] = x - primaryTextBlock->x;
    delta[1] = y - primaryTextBlock->y;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain(primaryTextBlock->glyphChain, delta[0], delta[1]);
        primaryTextBlock->x = x;
        primaryTextBlock->y = y;
    }
}

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesBlk14MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk14 *primaryTextBlock = &itfWindowSlots[window].mes->blk14;
    itfMesOffsetNodeChain(primaryTextBlock->glyphChain, dx, dy);
    primaryTextBlock->x += dx;
    primaryTextBlock->y += dy;
}

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesBlk24MoveTo(s32 window, s32 x, s32 y) {
    ItfMesEntryBlock *entryBlock = &itfWindowSlots[window].mes->entryBlock;
    s32 delta[2];
    delta[0] = x - entryBlock->x;
    delta[1] = y - entryBlock->y;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain(entryBlock->glyphChain, delta[0], delta[1]);
        entryBlock->x = x;
        entryBlock->y = y;
    }
}

/* Translate selected-entry text and update its cached position. */
void itfMesBlk24MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesEntryBlock *entryBlock = &itfWindowSlots[window].mes->entryBlock;
    itfMesOffsetNodeChain(entryBlock->glyphChain, dx, dy);
    entryBlock->x += dx;
    entryBlock->y += dy;
}

/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesBlk40MoveTo(s32 window, s32 x, s32 y) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;
    s32 delta[2];
    delta[0] = x - optionBlock->x;
    delta[1] = y - optionBlock->y;
    if (delta[0] != 0 || delta[1] != 0) {
        itfMesOffsetNodeChain(optionBlock->glyphChain, delta[0], delta[1]);
        optionBlock->x = x;
        optionBlock->y = y;
    }
}

/* Translate option text and update its cached position. */
void itfMesBlk40MoveBy(s32 window, s32 dx, s32 dy) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;
    itfMesOffsetNodeChain(optionBlock->glyphChain, dx, dy);
    optionBlock->x += dx;
    optionBlock->y += dy;
}

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfUpdateMessageWindowRenderValue(s32 window, s32 renderValue) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    if (mes->renderValue != renderValue) {
        itfMesSetNodeChainRenderValue(mes->blk14.glyphChain, renderValue);
        itfMesSetNodeChainRenderValue(mes->entryBlock.glyphChain, renderValue);
        itfMesSetNodeChainRenderValue(mes->blk40.glyphChain, renderValue);
        mes->renderValue = renderValue;
    }
}

/* Return the complete status/flag word, not just its high-half flags. */
u32 itfMesGetWindowFlags(s32 window) {
    return itfWindowSlots[window].mes->flags;
}

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesReplaceWindowHighFlags(s32 window, u32 highFlags) {
    ItfMesState *mes;

    mes = itfWindowSlots[window].mes;
    mes->flags = (u32)(u16)mes->flags | (highFlags & ITF_MES_HIGH_FLAGS_MASK);
}

/* Set only high-half mask bits; low-half status is unchanged. */
void itfMesSetWindowHighFlags(s32 window, u32 mask) {
    ItfMesState *mes;

    mes = itfWindowSlots[window].mes;
    mes->flags = mes->flags | (mask & ITF_MES_HIGH_FLAGS_MASK);
}

/* Clear only high-half mask bits; low-half status is unchanged. */
void itfMesClearWindowHighFlags(s32 window, u32 mask) {
    ItfMesState *mes;

    mes = itfWindowSlots[window].mes;
    mes->flags = mes->flags & (~mask | ITF_MES_LOW_FLAGS_MASK);
}

/* Return the cached clear-bit rank; reset state uses -1. */
s16 itfMesGetWindowClearBitCount(s32 window) {
    return itfWindowSlots[window].mes->blk40.selectedIndex;
}

/* Return the selected table's cached item count, without another table lookup. */
s16 itfMesGetWindowTableCount(s32 window) {
    return itfWindowSlots[window].mes->entryBlock.tableCount;
}

/* Return the stored signed-halfword item index, without another table lookup. */
s16 itfMesGetWindowItemIndex(s32 window) {
    return itfWindowSlots[window].mes->entryBlock.itemIndex;
}

/* Return a replacement-text address; caller supplies an in-range slot index. */
u32 itfMesGetWindowTableValue(s32 window, s32 slotIndex) {
    return itfWindowSlots[window].mes->textSlots.addresses[slotIndex];
}

extern s32 D_00435E48;
extern s32 D_00435E4C;
extern s32 D_00435E5C;
extern s32 D_00435E60;
extern s32 D_00435E64;
extern char D_003A41A8[][0x20];
extern char D_003A47E8[][0x20];
extern char *D_00386048[];
extern char D_004365F0[];
extern void func_0035C860(char *dst, const char *format, ...);
extern void itfConvertText(char *dst, const char *src);
extern u16 *txtFormatNumberU16(s32 value, u16 *dst);
void itfMesCopyStringToWindowTableSlot(s32 window, u32 slotIndex, u32 sourceAddress);

/* Copy one of the built-in interface strings into a window replacement slot. */
void itfMesSetTextSlotFromValue(s32 window, s32 slotIndex, s32 value, s32 selector) {
    char formatted[0x10];
    u16 number[0x20];
    char converted13[0x19];
    char converted15[0x11];
    char converted14[0x11];
    char *converted;
    s32 negative = 0;

    switch (selector) {
    case 0:
        func_0035C860(formatted, D_004365F0, value);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)formatted);
        break;
    case 1:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_00435E4C + value * 0x11);
        break;
    case 14:
        memset(converted14, 0, sizeof(converted14));
        itfConvertText(converted14, (char *)D_00435E4C + value * 0x11);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)converted14);
        break;
    case 8:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_00435E60 + value * 7);
        break;
    case 2:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_00435E48 + value * 0x11);
        break;
    case 15:
        memset(converted15, 0, sizeof(converted15));
        itfConvertText(converted15, (char *)D_00435E48 + value * 0x11);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)converted15);
        break;
    case 3:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_00435E5C + value * 0x19);
        break;
    case 13:
        memset(converted13, 0, sizeof(converted13));
        itfConvertText(converted13, (char *)D_00435E5C + value * 0x19);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)converted13);
        break;
    case 4:
        converted = (char *)number;
        if (value >= 1001) {
            *(u16 *)converted = 0xB280;
            number[1] = 0;
        } else {
            if (value < 0) {
                value = -value;
                *(u16 *)converted = 0xA280;
                converted += 2;
                negative = 1;
            }
            if (!negative) {
                converted = (char *)txtFormatNumberU16(value, (u16 *)converted);
                ((u16 *)converted)[0] = 0xA680;
                ((u16 *)converted)[1] = 0;
            } else {
                converted = (char *)txtFormatNumberU16(value, (u16 *)converted);
                ((u16 *)converted)[1] = 0;
            }
        }
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)number);
        break;
    case 5:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_00435E64 + value * 0x11);
        break;
    case 6:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)D_003A41A8[value]);
        break;
    case 7:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)D_003A47E8[value]);
        break;
    case 9:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (u32)D_00386048[value]);
        break;
    case 10:
    case 11:
    case 12:
        break;
    }
}

/* Replace a text slot with a copied NUL-terminated string; sourceAddress
 * remains a raw address in the existing interface. */
void itfMesCopyStringToWindowTableSlot(s32 window, u32 slotIndex, u32 sourceAddress) {
    func_001A5480((u32)itfWindowSlots[window].mes, slotIndex, sourceAddress, 0);
}

/* Legacy short-arity entry point: retain its one-argument copier call.
 * This function does not specify the copier's other parameters. */
void func_001A4888(s32 window) {
    func_001A5480((u32)itfWindowSlots[window].mes);
}

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
ItfMesSub *itfMesSetSubResource(s32 window, ItfMesSub *sub) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesSub *previous = mes->sub;
    ItfMesEntry *entry;
    s32 temporaryFontEntry;
    mes->sub = sub;
    itfMesRelocate(sub);
    entry = itfMesGetNextEntry(sub);
    temporaryFontEntry = 0;
    if (((ItfMesRelocResource *)sub)->magic == ITF_MES_MAGIC_MSG1) {
        temporaryFontEntry = *(s32 *)((u8 *)entry + 8);
    }
    mes->temporaryFontEntry = temporaryFontEntry;
    return previous;
}

/* Look up an entry-table item; only the item index has a bounds check. */
u32 itfMesGetEntryTableItem(s32 window, s32 entryIndex, s32 itemIndex) {
    return itfMesGetTableItem(itfMesGetEntry(itfWindowSlots[window].mes, entryIndex)->table, itemIndex);
}

extern void func_001A6078(ItfMesBlkA4 *blk, s32 arg1, s32 arg2);

/* Refresh changed panel values only when both content-status pairs are clear.
 * The first value is stored as a halfword; release the old primitive afterward. */
void itfMesSetWindowPageAndRefresh(s32 window, s32 firstValue, s32 secondValue) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesBlkA4 *panelBlock = &mes->blkA4;

    if ((mes->flags & ITF_MES_CONTENT_STATUS_MASK) == 0) {
        if (mes->unk12 != firstValue || panelBlock->fadeLimit != secondValue) {
            mes->unk12 = firstValue;
            func_001A6078(panelBlock, firstValue, secondValue);
            if (panelBlock->sprite != NULL) {
                itfPanelReleasePrimitiveResources(panelBlock->sprite);
                panelBlock->sprite = NULL;
            }
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4A10);

/* Build and release a temporary glyph chain to measure the widest grouped row.
 * Empty tables and zero/missing encoded items return zero. */
s32 itfMesMeasureEntryItem(s32 window, s32 entryIndex, s32 itemIndex) {
    ItfMesState *mes = itfWindowSlots[window].mes;
    ItfMesTable *table;
    u32 encodedText;
    FrFontGlyph *glyphChain;
    s32 textExtent;

    table = itfMesGetEntry(mes, entryIndex)->table;
    if (table->count == 0) {
        return 0;
    }
    itfMesInitCharTable((s32 *)mes->textSlots.addresses);
    if (mes->temporaryFontEntry != 0) {
        frFontLoadTemporaryEntry(mes->temporaryFontEntry);
    }
    encodedText = itfMesGetTableItem(table, itemIndex);
    if (encodedText == 0) {
        return encodedText;
    }
    glyphChain = itfDrawDefaultColorText(0, 0, (u8 *)encodedText, 0);
    textExtent = itfMesMaxGroupedExtent(glyphChain);
    frFontQueueGlyphInSelectedSlot(glyphChain);
    return textExtent;
}

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesSetEntryLastColorChannel(s32 window, u8 channelValue) {
    itfWindowSlots[window].mes->entryBlock.color[3] = channelValue;
}

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesSetWindowCallbackAddress(s32 window, u32 callbackAddress) {
    itfWindowSlots[window].mes->callbackAddress = callbackAddress;
}

/* Initialize the fixed window pool, load its texture and register the named
 * calculation/drawing tasks. Keep the existing task flags and callback casts. */
void itfMesInit(void) {
    ItfMesPoolNode *poolNode;

    itfMesWork.activeWindowCount = 0;
    itfMesWork.unk8 = 0;
    itfMesWork.flags = 0;
    itfMesWork.windowTexture = itfLoadTextureFromAsset("/itf/MESWIN.TMX");
    itfInitPool(&itfMesWork.pool, itfMesWork.nodes, ITF_MES_WINDOW_CAPACITY, ITF_MES_POOL_NODE_BYTES);
    for (poolNode = itfMesWork.pool.firstFree; poolNode != NULL; poolNode = poolNode->next) {
        poolNode->stateAddress = 0;
    }
    kwlnTaskCreate("CalcMsgMng", 0x409, 0, 0, (void (*)(void))sndVisitQueuedResources, sndFlushMessageQueue, NULL);
    kwlnTaskCreate("DrawMsgMng", 0x2B1A, 0, 0, func_001A76C8, sndFlushMessageQueue, NULL);
}

/* Set manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesSetGlobalFlags(u32 mask) {
    itfMesWork.flags |= mask;
}

/* Clear manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesClearGlobalFlags(u32 mask) {
    itfMesWork.flags &= ~mask;
}

/* Read the manager's 16-bit flags, distinct from itfMessageFlags. */
u16 itfMesGetGlobalFlags(void) {
    return itfMesWork.flags;
}


struct ItfMesWindowRec {
    u8 pad00[0xC];
    ItfMesState *mes;
    s32 handle;
};

extern s8 D_0037F510[];
extern char *D_003B4990[];
extern char *D_00436600[2];
extern char D_00436608[];
extern char D_00436610[];
extern char D_00436618[];
extern char D_00436620[];
extern char D_00436628[];
extern SdfPoolNode kwlnPositionedTextSurface;
extern s32 sdfCreateResetPacketList(void);
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);
extern void sdfAppendPacket(SdfListHead *list, u32 packetAddress);
extern void kwlnDrawSpriteCell();
extern ItfMesWindowRec *func_001A7A98(ItfMesWindowRec *window);
extern void itfAdjustPanelBoundsWithPad(ItfMesBlkA4 *panel, s32 selectedItem);
extern void sndStepIndexByPad(ItfMesBlkA4 *panel);

/* Run and draw the interactive message-layout inspector. */
INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CE0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CF0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D00);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D10);

s32 itfMesRunPanelLayoutInspector(void) {
    ItfMesBlkA4 *panel;
    UiSprite *panelSprite;
    s32 *position;
    SdfListHead *packetList;
    s32 item;
    s32 y;
    const char *format;
    const char *marker;
    const char *positionFormat;

    if (D_003B4770.window == NULL) {
        ItfMesWindowRec *window = func_001A7A98(NULL);

        D_003B4770.window = window;
        if (window == NULL) {
            return 0;
        }
    }
    {
        ItfMesDebugState *debug = &D_003B4770;

        panel = &debug->window->mes->blkA4;
        switch (debug->mode) {
        case 0:
            if (D_0037F510[0x36] & 2) {
                if (--debug->selectedItem < 0) {
                    debug->selectedItem = 2;
                }
            } else if (D_0037F510[0x37] & 2) {
                if (++debug->selectedItem >= 5) {
                    debug->selectedItem = 0;
                }
            }
            if (D_0037F510[0x31] < 0) {
                ItfMesDebugState *confirmDebug = &D_003B4770;

                switch (confirmDebug->selectedItem) {
                case 0:
                case 1:
                case 2:
                    confirmDebug->mode = 1;
                    break;
                case 3:
                    confirmDebug->mode = 2;
                    break;
                case 4:
                    confirmDebug->window = func_001A7A98(confirmDebug->window);
                    break;
                }
            } else if (D_0037F510[0x33] < 0) {
                return -1;
            }
            break;
        case 1:
            itfAdjustPanelBoundsWithPad(panel, debug->selectedItem);
            if (D_0037F510[0x33] < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            sndStepIndexByPad(panel);
            if (D_0037F510[0x33] < 0) {
                D_003B4770.mode = 0;
            }
            break;
        }
    }

    packetList = (SdfListHead *)sdfCreateResetPacketList();
    kwlnDrawSpriteCell(packetList, 0x10, 0x10, 0x1E, 9);
    format = D_00436618;
    y = 0x7A00;
    for (item = 0; item < 5; item++, y += 0x60) {
        if (D_003B4770.selectedItem == item) {
            marker = D_00436600[D_003B4770.mode != 0];
        } else {
            marker = D_00436620;
        }
        sdfAppendPacket(packetList,
                        (u32)sdfCreateFormattedSifCommand(0x7180, y, 0xFFFFF0, 0,
                                                     format, marker, D_003B4990[item]));
    }

    positionFormat = "( %3d,%3d )";
    panelSprite = panel->sprite;
    position = &panelSprite->left;
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7A00, 0xFFFFF0, 0,
                                                 positionFormat, position[0] >> 4,
                                                 position[1] >> 3));
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7A60, 0xFFFFF0, 0,
                                                 positionFormat, position[2] >> 4,
                                                 position[3] >> 3));
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7B20, 0xFFFFF0, 0,
                                                 D_00436628, panel->fadeLimit));
    position = D_003B4770.window->mes->blkA4.bounds;
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7180, 0x7C40, 0xFFFFF0, 0,
                                                 "OFFSET : %3d,%3d - %3d,%3d",
                                                 position[0] >> 4,
                                                 position[1] >> 3,
                                                 position[2] >> 4,
                                                 position[3] >> 3));
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, packetList);
    return 0;
}

/* Reset the three otherwise unnamed message-state words. */
void itfMesClearGlobalWords(void) {
    D_003B4770.selectedItem = 0;
    D_003B4770.mode = 0;
    D_003B4770.window = NULL;
}

extern ItfMesWindowRec D_00452960[];
extern void btlReleaseEffectResourceHandles();
extern void itfReleaseUiResourceSlotHandles(ItfMesTextSlots *slots);
extern void itfReleasePoolNode();

/* Tear down installed content/resources, return the pool record and decrement
 * the pool-array-relative counter. An empty record is a no-op. */
void itfMesDestroyWindow(s32 window) {
    ItfMesWindowRec *windowRecord = &D_00452960[window];
    ItfMesState *mes = windowRecord->mes;
    if (mes != NULL) {
        itfMesCleanupWindow(window, 1);
        itfMesResetWindow(window);
        btlReleaseEffectResourceHandles(mes);
        itfReleaseUiResourceSlotHandles(&mes->textSlots);
        mes->flags = 0;
        sdfReleaseResourceAllocation((SdfMemBlock *)windowRecord->handle);
        windowRecord->mes = NULL;
        itfReleasePoolNode(windowRecord, (u8 *)D_00452960 - 0x10);
        ((ItfMesGlobals *)((u8 *)D_00452960 - 0x20))->activeWindowCount -= 1;
    }
}

/* Relocate packed payload words once, then set the resource's relocated marker. */
/* Semantic reference: Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c). */
void itfMesRelocate(ItfMesRelocResource *resource)
{
    u8 *payload;
    u8 *fixupTable;
    s32 fixupSize;
    if (resource->relocated == 0) {
        payload = resource->payload;
        fixupTable = (u8 *)resource + resource->fixupOffset;
        fixupSize = resource->fixupCount;
        sdfRelocatePackedResourceWords((int *)payload, (int)payload, fixupTable, fixupSize);
        resource->relocated = 1;
    }
}

/* Recognize MSG0/MSG1 magic only; this is not complete format validation. */
u32 itfMesIsMsgData(ItfMesRelocResource *resource) {
    u32 isMessageResource;

    isMessageResource = 0;
    if ((resource->magic == ITF_MES_MAGIC_MSG0) || (resource->magic == ITF_MES_MAGIC_MSG1)) {
        isMessageResource = 1;
    }
    return isMessageResource;
}

/* Address an installed resource entry; caller supplies an in-range index. */
ItfMesEntry *itfMesGetEntry(ItfMesState *mes, s32 entryIndex) {
    ItfMesEntry *entries = mes->sub->entries;

    return &entries[entryIndex];
}

/* Return the trailing entry at entryCount; do not advance or validate the count. */
ItfMesEntry *itfMesGetNextEntry(ItfMesSub *sub) {
    ItfMesEntry *entries = sub->entries;

    return &entries[sub->entryCount];
}

/* Return the entry's raw item-list word; the entry index is not validated. */
u32 itfMesGetWindowEntryItems(s32 window, s32 entryIndex) {
    return itfMesGetEntry(itfWindowSlots[window].mes, entryIndex)->itemList;
}

/* Read the installed sub-resource's entry count without slot/resource validation. */
u32 itfMesGetEntryCount(s32 window) {
    return itfWindowSlots[window].mes->sub->entryCount;
}

/* Return an encoded item word, or zero for a negative/past-end item index. */
u32 itfMesGetTableItem(ItfMesTable *table, s32 itemIndex) {
    s32 itemCount = table->count;

    if (itemIndex < 0 || itemIndex >= itemCount) {
        return 0;
    }
    return table->items[itemIndex];
}

extern s32 func_0019DB30();
extern void func_0019DD48();
extern s8 itfTestTextInterfaceMask();
extern s32 func_0019E908();
extern s32 func_0019E910();
extern void evtLipsExecFunction();
extern void itfMesCopyGlyphShade();
extern void itfMesEnableUnflaggedNodeContexts();

/* Rebuild colored entry glyphs, synchronize interface flags/hooks and cache
 * the span count. Existing span helpers require a valid created chain. */
void itfMesBuildEntryGlyph(ItfMesState *mes) {
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;
    FrFontGlyph *glyph;
    FrFontGlyph *previousGlyph;
    s32 lipsValue;
    s8 interfaceMask;
    void (*callback)();

    previousGlyph = entryBlock->glyphChain;
    if (previousGlyph != NULL) {
        frFontQueueGlyphInSelectedSlot(previousGlyph);
        entryBlock->glyphChain = NULL;
    }
    glyph = itfDrawCustomColorText((s32)mes->entryBlock.x, (s32)entryBlock->y, entryBlock->color[0], entryBlock->color[1], entryBlock->color[2], entryBlock->color[3],
                          (u8 *)itfMesGetTableItem(entryBlock->table, entryBlock->itemIndex), 0);
    if (mes->unk12 == 3) {
        if (func_0019DB30(glyph) == 1) {
            func_0019DD48(0x1000, 0xC60, glyph);
        } else {
            func_0019DD48(0x1000, 0xBF8, glyph);
        }
    }
    if (!(mes->flags & ITF_MES_SKIP_CONTEXT_ENABLE_BIT) && (itfMesWork.flags & 1)) {
        itfMesEnableUnflaggedNodeContexts(glyph);
    }
    itfMesCopyGlyphShade(glyph, entryBlock);
    interfaceMask = itfTestTextInterfaceMask(3);
    entryBlock->unk11 = interfaceMask;
    /* Copy interface-mask bit 0x2 into window flag 0x10000. */
    if (interfaceMask & 2) {
        mes->flags |= 0x10000;
    } else {
        mes->flags &= 0xFFFEFFFF;
    }
    if (itfTestTextInterfaceMask(4) != 0) {
        callback = (void (*)())mes->callbackAddress;
        if (callback != NULL) {
            callback();
        }
    }
    if (itfTestTextInterfaceMask(8) != 0) {
        lipsValue = func_0019E908();
        evtLipsExecFunction(lipsValue, func_0019E910());
    }
    itfMesSetNodeChainRenderValue(glyph, mes->renderValue);
    entryBlock->unk16 = itfMesCountSpanSteps(itfMesGetLastNode(glyph), glyph);
    entryBlock->glyphChain = glyph;
}

/* Replace a retained text-slot allocation. Nonpositive byteCount copies the
 * padded string span; positive counts copy that many bytes into a zeroed block.
 * DDS2 also zeroes string-mode blocks first. Keep the K&R declaration form. */
void func_001A5480(mes, slotIndex, source, byteCount)
    ItfMesState *mes;
    s32 slotIndex;
    const char *source;
    s32 byteCount;
{
    ItfMesTextSlots *slots = &mes->textSlots;
    u32 *textAddress = &slots->addresses[slotIndex];
    s32 allocationBytes;

    if (*textAddress != 0) {
        sdfReleaseResourceAllocation(slots->handles[slotIndex]);
        *textAddress = 0;
    }
    if (byteCount <= 0) {
        allocationBytes = (strlen(source) + ITF_MES_STRING_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
        slots->handles[slotIndex] = sdfAllocGeneralBlock(allocationBytes);
        *textAddress = sdfResourceRetainAddress(slots->handles[slotIndex]);
        memset((void *)*textAddress, 0, allocationBytes);
        /* String mode copies the padded span, rather than only strlen + 1. */
        memcpy((void *)*textAddress, source, allocationBytes);
        return;
    }
    allocationBytes = (byteCount + ITF_MES_BINARY_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
    slots->handles[slotIndex] = sdfAllocGeneralBlock(allocationBytes);
    *textAddress = sdfResourceRetainAddress(slots->handles[slotIndex]);
    memset((void *)*textAddress, 0, allocationBytes);
    memcpy((void *)*textAddress, source, byteCount);
}

extern void func_0019E8F0();

/* Install all replacement-text addresses into the font's fixed slot table. */
void itfMesInitCharTable(s32 *table) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < ITF_MES_TEXT_SLOT_COUNT; slotIndex++) {
        func_0019E8F0(slotIndex, table[slotIndex]);
    }
}

/* Read the selected item from the window's trailing entry, without validation. */
u32 itfMesGetNextEntrySelectedItem(ItfMesState *mes) {
    u32 *items;

    items = (u32 *)itfMesGetNextEntry(mes->sub)->itemList;
    return items[mes->blk14.selectedIndex];
}

/* Count clear low bits by consuming one bit per iteration. Nonpositive counts
 * return zero; counts beyond the mask width also count shifted-in zero bits. */
s32 itfMesCountZeroBits(s32 bitCount, u32 remainingMask) {
    s32 zeroCount;
    u32 bitValue;

    zeroCount = 0;
    while (0 < bitCount) {
        bitValue = remainingMask & 1;
        remainingMask = remainingMask >> 1;
        bitCount = bitCount - 1;
        if (bitValue == 0) {
            zeroCount = zeroCount + 1;
        }
    }
    return zeroCount;
}

/* Append text for each clear mask bit. Masked items are still consumed, while
 * y advances only for built text. Return NULL if nothing was built. */
FrFontGlyph *itfMesBuildNodeRows(u32 *items, s32 itemCount, u32 mask, s32 x, s32 y, s32 renderValue) {
    FrFontGlyph *glyphChain = NULL;
    s32 itemIndex;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++, items++) {
        if (mask & 1) {
            mask >>= 1;
        } else {
            s16 rowHeightUnits;

            glyphChain = itfDrawCustomColorText(x, y, 0, 0, 0, ITF_MES_DEFAULT_COLOR_VALUE, (u8 *)*items, glyphChain);
            mask >>= 1;
            rowHeightUnits = glyphChain->u10.half[1];
            y += rowHeightUnits << ITF_MES_TEXT_Y_SHIFT;
        }
    }
    if (glyphChain != NULL) {
        itfMesSetNodeChainRenderValue(glyphChain, renderValue);
        itfMesRecolorNodeChildren(glyphChain, ITF_MES_DEFAULT_COLOR_VALUE);
    }
    return glyphChain;
}

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
FrFontGlyph *itfMesTrimGlyphChainToRow(FrFontGlyph *node, s32 from, s32 to) {
    s32 rowsToDiscard = to - from - 1;
    s32 rowY = node->y;
    FrFontGlyph *nextNode;
    FrFontGlyph *rowHead;
    FrFontGlyph *rowTail;

    while (rowsToDiscard > 0) {
        while (rowY == node->y) {
            nextNode = node->previous;
            node->next = NULL;
            node->chainHead = node;
            node->previous = NULL;
            frFontQueueGlyphInSelectedSlot(node);
            node = nextNode;
            if (node == NULL) {
                return NULL;
            }
        }
        rowsToDiscard--;
        rowY = node->y;
    }
    rowHead = node;
    do {
        rowTail = node;
        node = node->previous;
    } while (node != NULL && rowY == node->y);
    while (node != NULL) {
        nextNode = node->previous;
        node->next = NULL;
        node->chainHead = node;
        node->previous = NULL;
        frFontQueueGlyphInSelectedSlot(node);
        node = nextNode;
    }
    rowHead->next = NULL;
    rowTail->previous = NULL;
    for (node = rowHead; node != NULL; node = node->previous) {
        /* Despite its current name, this link points to the retained row tail. */
        node->chainHead = rowTail;
    }
    return rowHead;
}

/* Return the final previous-linked glyph; input must be non-NULL. */
FrFontGlyph *itfMesGetLastNode(FrFontGlyph *node) {
    while (node->previous != NULL) {
        node = node->previous;
    }
    return node;
}

/* Copy the auxiliary glyph's shade bytes; its encoded intensity is unsigned. */
void itfMesCopyGlyphShade(FrFontGlyph *glyph, ItfMesEntryBlock *entryBlock) {
    FrFontGlyph *shade = glyph->link20.linkedGlyph;
    u8 encodedIntensity = glyph->u0.b.b0;

    entryBlock->color[3] = encodedIntensity >> 1;
    entryBlock->color[0] = shade->u14.b[1];
    entryBlock->color[1] = shade->u14.b[0];
    entryBlock->color[2] = shade->u14.b[2];
}

/* Inclusive step count from the y delta in eighths and the first row's height.
 * Both pointers and a nonzero first-row height are required. */
s32 itfMesCountSpanSteps(FrFontGlyph *last, FrFontGlyph *first) {
    s16 rowHeightUnits = first->u10.half[1];

    return ((first->y - last->y) >> ITF_MES_TEXT_Y_SHIFT) / rowHeightUnits + 1;
}

/* Translate every linked node in text units; a NULL chain is a no-op. */
void itfMesOffsetNodeChain(FrFontGlyph *node, s32 dx, s32 dy) {
    if (node == NULL) {
        return;
    }
    do {
        node->x += dx;
        node->y += dy;
        node = node->previous;
    } while (node != NULL);
}

/* Assign the render value across the complete chain; a NULL chain is a no-op. */
/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void itfMesSetNodeChainRenderValue(FrFontGlyph *node, s32 renderValue) {
    while (node != NULL) {
        node->u14.w = renderValue;
        node = node->previous;
    }
}

/* Skip (last - first - 1) preceding y-groups when positive, then set each
 * child's flag byte in the next row. Requires non-NULL input; past-end is a no-op. */
void itfMesSetRowItemFlag(FrFontGlyph *node, s32 first, s32 last, s32 requestedFlags) {
    s32 rowsToSkip = last - first - 1;
    s32 rowY = node->y;
    s32 currentY = rowY;
    FrFontGlyph *child;
    u8 flagValue;

    while (rowsToSkip > 0) {
        while (rowY == currentY) {
            node = node->previous;
            if (node == NULL) {
                return;
            }
            currentY = node->y;
        }
        rowsToSkip--;
        rowY = currentY;
    }
    flagValue = requestedFlags;
    do {
        for (child = node->link1C.firstChild; child != NULL; child = child->next) {
            child->u14.b[0] = flagValue;
        }
        node = node->previous;
    } while (node != NULL && rowY == node->y);
}

/* Set every child's flag byte across the parent chain; NULL is a no-op. */
void itfMesSetChildChainFlags(FrFontGlyph *glyph, u8 flagValue) {
    FrFontGlyph *child;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->link1C.firstChild; child != NULL; child = child->next) {
            child->u14.b[0] = flagValue;
        }
    }
}

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void itfMesRecolorNodeChildren(FrFontGlyph *node, u32 color) {
    FrFontGlyph *child;

    for (; node != NULL; node = node->previous) {
        for (child = node->link1C.firstChild; child != NULL; child = child->next) {
            child->u10.word = child->u10.word & ITF_MES_COLOR_BYTE_CLEAR_MASK | color;
        }
    }
}

/* Find the maximum summed advance of contiguous equal-y groups, scaled by 16.
 * NULL returns zero; the initial zero also excludes negative group totals. */
s32 itfMesMaxGroupedExtent(FrFontGlyph *node) {
    s32 maxAdvance = 0;

    while (node != NULL) {
        s32 rowY = node->y;
        s32 rowAdvance = 0;

        do {
            rowAdvance += node->advance;
            node = node->previous;
        } while (node != NULL && rowY == node->y);
        if (rowAdvance > maxAdvance) {
            maxAdvance = rowAdvance;
        }
    }
    return maxAdvance << ITF_MES_TEXT_X_SHIFT;
}

/* Return the bit index of the zero-based clear-bit ordinal, or 32 if absent.
 * Keep the predecrement; the ordinal itself is not validated. */
s32 itfMesNthClearBit(s32 clearBitsToSkip, u32 mask) {
    s32 bitIndex = 0;
    while (bitIndex < ITF_MES_MASK_BIT_COUNT) {
        if ((mask & 1) == 0) {
            if (--clearBitsToSkip < 0) {
                break;
            }
        }
        bitIndex++;
        mask >>= 1;
    }
    return bitIndex;
}

/* Enable parent contexts whose first child's tested flag is zero.
 * NULL chain is allowed, but every visited node must have a first child. */
void itfMesEnableUnflaggedNodeContexts(FrFontGlyph *node) {
    for (; node != NULL; node = node->previous) {
        if (node->link1C.firstChild->u14.b[2] == 0) {
            frFontEnableContextMode(node);
        }
    }
}

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMessageFlags);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F8);

char *D_00436600[2] __attribute__((section(".sdata"))) = {D_00436610, D_00436608};

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436608);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436610);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436618);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436620);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436628);

