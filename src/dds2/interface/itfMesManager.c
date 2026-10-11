#include "common.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "itf.h"
#include "itf_mes_window.h"
#include "itf_panel_api.h"
#include "fld_area_work.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "pcp_vu0.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "eff.h"
#include "itf_panel_draw.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "btl_action.h"
#include "scr.h"
#include "dat_state.h"
#include "mnu_result.h"
#include "dat_command.h"
#include "sdf_sif_command.h"
#include "mdl_resource_table.h"
#include "kwln_task_lifecycle.h"

void sdfRelocatePackedResourceWords(int *param_1, int param_2, u8 *param_3, int param_4);

extern u32 itfMessageFlags;

/* Option IDs index the same signed-byte bank used by the named controls. */
typedef struct SndPad {
    u8 pad00[0x20];
    union {
        s8 buttons[0x20];
        struct {
            u8 pad20;
            s8 confirm;
            u8 pad22[4];
            s8 prev;
            s8 next;
            u8 pad28[9];
            s8 unk31;
            s8 unk32;
            s8 cancel;
            s8 coarseDown;
            s8 coarseUp;
            s8 unk36;
            s8 unk37;
            s8 fineDown;
            u8 pad39;
            s8 fineUp;
            u8 pad3B[5];
        };
    };
} SndPad;

extern SndPad D_0037F510;

typedef struct SndPadStepTarget {
    u8 pad00[0x38];
    s32 value; /* 0x38 */
} SndPadStepTarget;

typedef struct SndPadStepper {
    u8 pad00[4];
    SndPadStepTarget *target; /* 0x04 */
    u8 pad08[0x20];
    s32 index; /* 0x28 */
} SndPadStepper;

typedef ItfMesPoolNode ItfMesWindowRec;

extern ItfMesSlot itfWindowSlots[];

extern ItfMesGlobals itfMesWork;

#define ITF_MES_MAGIC_MSG0 0x3047534d
#define ITF_MES_MAGIC_MSG1 0x3147534d

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

void itfMesRelocate(ItfMesRelocHeader *resource);

u32 scrGetWindow(void);

s32 scrReadIntParameter(s32 parameterIndex);

void func_00154F18(s32);

void itfMesSetTextSlotFromValue(s32 window, s32 slotIndex, s32 value, s32 selector);
void itfMesBlk24MoveTo(s32 window, s32 x, s32 y);

s32 func_001A4A10(s32 window, s32 first, s32 second);

void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock);

void itfMesResetWindow(s32 window);

void itfMesDestroyWindow(s32 window);

void itfMesInitCharTable(s32 *table);

s32 itfMesMaxGroupedExtent(FrFontGlyph *node);

extern void frFontLoadTemporaryEntry(u32);

extern SdfTex *itfLoadTextureFromAsset(const char *path);

extern void itfInitPool(ItfMesPool *pool, ItfMesPoolNode *nodes, s32 count, s32 stride);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern s32 sndVisitQueuedResources(void);

extern void sndFlushMessageQueue(void);

extern s32 func_001A76C8(void);

extern struct ItfMesPoolNode *itfAcquirePoolNode();

extern u32 strlen(const char *);
extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);
void func_001A5480();

extern void itfInitializeCursorResetState();

extern void itfResetWindowResourceBlock();

extern void itfClearDrawStateWords();

extern void itfResetBattleFadeState();

extern void func_0019DD48();

extern s32 frFontMeasureLineWidth();

extern UiSprite *func_001A1858(s32 kind, u32 payload);

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
void itfMesFinishWindowAndClearStatus(s32 window) {
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
SdfTex *itfMesGetWindowTexture(void) {
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
    mes = (ItfMesState *)sdfResourceRetainAddress((struct SdfMemBlock *)handle);
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
        frFontQueueGlyphForCurrentDrawBuffer(entryBlock->glyphChain);
        entryBlock->glyphChain = NULL;
    }
    itfMesResetCursorState(entryBlock, 0);
    mes->flags &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->flags &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (releasePrimaryBlock == 0) {
        return;
    }
    if (primaryTextBlock->glyphChain != NULL) {
        frFontQueueGlyphForCurrentDrawBuffer(primaryTextBlock->glyphChain);
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
        rowWidth = frFontMeasureLineWidth(rowIndex, optionBlock->glyphChain);
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
        frFontQueueGlyphForCurrentDrawBuffer(optionBlock->glyphChain);
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
        frFontQueueGlyphForCurrentDrawBuffer(optionBlock->glyphChain);
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
void itfMesCopyStringToWindowTableSlot(s32 window, u32 slotIndex, const void *sourceText);

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
        itfMesCopyStringToWindowTableSlot(window, slotIndex, formatted);
        break;
    case 1:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (const void *)(D_00435E4C + value * 0x11));
        break;
    case 14:
        memset(converted14, 0, sizeof(converted14));
        itfConvertText(converted14, (char *)D_00435E4C + value * 0x11);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, converted14);
        break;
    case 8:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (const void *)(D_00435E60 + value * 7));
        break;
    case 2:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (const void *)(D_00435E48 + value * 0x11));
        break;
    case 15:
        memset(converted15, 0, sizeof(converted15));
        itfConvertText(converted15, (char *)D_00435E48 + value * 0x11);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, converted15);
        break;
    case 3:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (const void *)(D_00435E5C + value * 0x19));
        break;
    case 13:
        memset(converted13, 0, sizeof(converted13));
        itfConvertText(converted13, (char *)D_00435E5C + value * 0x19);
        itfMesCopyStringToWindowTableSlot(window, slotIndex, converted13);
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
        itfMesCopyStringToWindowTableSlot(window, slotIndex, number);
        break;
    case 5:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, (const void *)(D_00435E64 + value * 0x11));
        break;
    case 6:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003A41A8[value]);
        break;
    case 7:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_003A47E8[value]);
        break;
    case 9:
        itfMesCopyStringToWindowTableSlot(window, slotIndex, D_00386048[value]);
        break;
    case 10:
    case 11:
    case 12:
        break;
    }
}

/* Replace a text slot with a copied NUL-terminated encoded string. */
void itfMesCopyStringToWindowTableSlot(s32 window, u32 slotIndex, const void *sourceText) {
    func_001A5480((u32)itfWindowSlots[window].mes, slotIndex, sourceText, 0);
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
    itfMesRelocate((ItfMesRelocHeader *)sub);
    entry = itfMesGetNextEntry(sub);
    temporaryFontEntry = 0;
    if (sub->magic == ITF_MES_MAGIC_MSG1) {
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

/* Set or append a window option; reserved and out-of-range IDs are rejected. */
s32 func_001A4A10(s32 window, s32 id, s32 value) {
    ItfMesBlk40 *optionBlock = &itfWindowSlots[window].mes->blk40;
    ItfMesOption *option = NULL;
    s32 index;

    if ((u32)id >= 0x10) {
        return 0;
    }
    if (id == 1 || id == 6 || id == 7) {
        return 0;
    }
    for (index = 0; index < optionBlock->optionCount; index++) {
        if (optionBlock->options[index].id == id) {
            option = &optionBlock->options[index];
            break;
        }
    }
    if (option == NULL) {
        if (optionBlock->optionCount >= 15) {
            return 0;
        }
        option = &optionBlock->options[optionBlock->optionCount];
        optionBlock->optionCount++;
    }
    option->id = id;
    option->value = value;
    return 1;
}

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
    frFontQueueGlyphForCurrentDrawBuffer(glyphChain);
    return textExtent;
}

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesSetEntryLastColorChannel(s32 window, u8 channelValue) {
    itfWindowSlots[window].mes->entryBlock.color[3] = channelValue;
}

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesSetWindowCallbackAddress(s32 window, void (*callback)(void)) {
    itfWindowSlots[window].mes->callbackAddress = (u32)callback;
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

extern char *D_003B4990[];
extern char *D_00436600[2];
extern char D_00436608[];
extern char D_00436610[];
extern char D_00436618[];
extern char D_00436620[];
extern char D_00436628[];
extern SdfPoolNode kwlnPositionedTextSurface;
extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);
extern void kwlnDrawSpriteCell();
extern ItfMesWindowRec *func_001A7A98(ItfMesWindowRec *window);
extern void itfAdjustPanelBoundsWithPad(ItfMesBlkA4 *panel, s32 selectedItem);
extern void sndStepIndexByPad(SndPadStepper *panel);

/* Run and draw the interactive message-layout inspector. */
INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CE0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CF0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D00);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D10);

s32 itfMesRunPanelLayoutInspector(void) {
    ItfMesBlkA4 *panel;
    UiSprite *panelSprite;
    s32 *bounds;
    SdfListHead *packetList;
    s32 menuItemIndex;
    s32 menuItemY;
    const char *menuItemFormat;
    const char *selectionMarker;
    const char *coordinateFormat;

    if (D_003B4770.window == NULL) {
        ItfMesWindowRec *window = func_001A7A98(NULL);

        D_003B4770.window = window;
        if (window == NULL) {
            return 0;
        }
    }
    {
        ItfMesDebugState *debug = &D_003B4770;

        panel = &((ItfMesState *)debug->window->stateAddress)->blkA4;
        switch (debug->mode) {
        case 0:
            if (D_0037F510.unk36 & 2) {
                if (--debug->selectedItem < 0) {
                    debug->selectedItem = 2;
                }
            } else if (D_0037F510.unk37 & 2) {
                if (++debug->selectedItem >= 5) {
                    debug->selectedItem = 0;
                }
            }
            if (D_0037F510.unk31 < 0) {
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
            } else if (D_0037F510.cancel < 0) {
                return -1;
            }
            break;
        case 1:
            itfAdjustPanelBoundsWithPad(panel, debug->selectedItem);
            if (D_0037F510.cancel < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            sndStepIndexByPad((SndPadStepper *)panel);
            if (D_0037F510.cancel < 0) {
                D_003B4770.mode = 0;
            }
            break;
        }
    }

    packetList = sdfCreateResetPacketList();
    kwlnDrawSpriteCell(packetList, 0x10, 0x10, 0x1E, 9);
    menuItemFormat = D_00436618;
    menuItemY = 0x7A00;
    for (menuItemIndex = 0; menuItemIndex < 5; menuItemIndex++, menuItemY += 0x60) {
        if (D_003B4770.selectedItem == menuItemIndex) {
            selectionMarker = D_00436600[D_003B4770.mode != 0];
        } else {
            selectionMarker = D_00436620;
        }
        sdfAppendPacket(packetList,
                        (u32)sdfCreateFormattedSifCommand(0x7180, menuItemY, 0xFFFFF0, 0,
                                                     menuItemFormat, selectionMarker, D_003B4990[menuItemIndex]));
    }

    coordinateFormat = "( %3d,%3d )";
    panelSprite = panel->sprite;
    bounds = &panelSprite->left;
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7A00, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[0] >> 4,
                                                 bounds[1] >> 3));
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7A60, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[2] >> 4,
                                                 bounds[3] >> 3));
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7E00, 0x7B20, 0xFFFFF0, 0,
                                                 D_00436628, panel->fadeLimit));
    bounds = ((ItfMesState *)D_003B4770.window->stateAddress)->blkA4.bounds;
    sdfAppendPacket(packetList,
                    (u32)sdfCreateFormattedSifCommand(0x7180, 0x7C40, 0xFFFFF0, 0,
                                                 "OFFSET : %3d,%3d - %3d,%3d",
                                                 bounds[0] >> 4,
                                                 bounds[1] >> 3,
                                                 bounds[2] >> 4,
                                                 bounds[3] >> 3));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, packetList);
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
    ItfMesState *mes = (ItfMesState *)windowRecord->stateAddress;
    if (mes != NULL) {
        itfMesCleanupWindow(window, 1);
        itfMesResetWindow(window);
        btlReleaseEffectResourceHandles(mes);
        itfReleaseUiResourceSlotHandles(&mes->textSlots);
        mes->flags = 0;
        sdfReleaseResourceAllocation((struct SdfMemBlock *)windowRecord->resourceHandle);
        windowRecord->stateAddress = 0;
        itfReleasePoolNode(windowRecord, (u8 *)D_00452960 - 0x10);
        ((ItfMesGlobals *)((u8 *)D_00452960 - 0x20))->activeWindowCount -= 1;
    }
}

/* Relocate packed payload words once, then set the resource's relocated marker. */
/* Semantic reference: Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c). */
void itfMesRelocate(ItfMesRelocHeader *resource)
{
    u8 *payload;
    u8 *fixupTable;
    s32 fixupSize;
    if (resource->relocated == 0) {
        payload = resource->payload;
        fixupTable = (u8 *)resource + resource->fixupTableOffset;
        fixupSize = resource->fixupTableBytes;
        sdfRelocatePackedResourceWords((int *)payload, (int)payload, fixupTable, fixupSize);
        resource->relocated = 1;
    }
}

/* Recognize MSG0/MSG1 magic only; this is not complete format validation. */
u32 itfMesIsMsgData(ItfMesRelocHeader *resource) {
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
        frFontQueueGlyphForCurrentDrawBuffer(previousGlyph);
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
            rowHeightUnits = glyphChain->parentDimensionsOrRenderWord.parentDimensions.cellHeight;
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
            frFontQueueGlyphForCurrentDrawBuffer(node);
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
        frFontQueueGlyphForCurrentDrawBuffer(node);
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
    FrFontChildGlyph *shade = glyph->lastChild;
    u8 encodedIntensity = glyph->glyphCodeOrContext.byteRoles.encodedContextByte;

    entryBlock->color[3] = encodedIntensity >> 1;
    entryBlock->color[0] = shade->renderValueOrSetupOrShade.shadeColor.red;
    entryBlock->color[1] = shade->renderValueOrSetupOrShade.shadeColor.green;
    entryBlock->color[2] = shade->renderValueOrSetupOrShade.shadeColor.blue;
}

/* Inclusive step count from the y delta in eighths and the first row's height.
 * Both pointers and a nonzero first-row height are required. */
s32 itfMesCountSpanSteps(FrFontGlyph *last, FrFontGlyph *first) {
    s16 rowHeightUnits = first->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

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
        node->renderValueOrSetupOrShade.renderValue = renderValue;
        node = node->previous;
    }
}

/* Skip (last - first - 1) preceding y-groups when positive, then set each
 * child's flag byte in the next row. Requires non-NULL input; past-end is a no-op. */
void itfMesSetRowItemFlag(FrFontGlyph *node, s32 first, s32 last, s32 requestedFlags) {
    s32 rowsToSkip = last - first - 1;
    s32 rowY = node->y;
    s32 currentY = rowY;
    FrFontChildGlyph *child;
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
        for (child = node->firstChild; child != NULL; child = child->next) {
            child->renderValueOrSetupOrShade.setupBytes.firstOption = flagValue;
        }
        node = node->previous;
    } while (node != NULL && rowY == node->y);
}

/* Set every child's flag byte across the parent chain; NULL is a no-op. */
void itfMesSetChildChainFlags(FrFontGlyph *glyph, u8 flagValue) {
    FrFontChildGlyph *child;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->renderValueOrSetupOrShade.setupBytes.firstOption = flagValue;
        }
    }
}

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void itfMesRecolorNodeChildren(FrFontGlyph *node, u32 color) {
    FrFontChildGlyph *child;

    for (; node != NULL; node = node->previous) {
        for (child = node->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = child->parentDimensionsOrRenderWord.renderWord & ITF_MES_COLOR_BYTE_CLEAR_MASK | color;
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
        if (node->firstChild->renderValueOrSetupOrShade.setupBytes.secondOption == 0) {
            frFontEnableContextMode(node);
        }
    }
}

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMessageFlags);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F8);

char *D_00436600[2] __attribute__((section(".sdata"))) = {D_00436610, D_00436608};

/* Named and indexed views of the same four signed 32-bit color channels. */
typedef union UiQuadColor {
    struct {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    };
    s32 channels[4];
} UiQuadColor;
typedef char UiQuadColorSizeCheck[sizeof(UiQuadColor) == 0x10 ? 1 : -1];

extern const UiQuadColor D_00414D50;

extern SdfTex *itfLoadTextureFromAsset(const char *path);

typedef struct EncBgEntry {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
} EncBgEntry;

extern ItfMesGlobals itfMesWork;

extern void itfMesDestroyWindow(s32 arg0);

extern void sdfTexReleaseReferenceViaHandler(SdfTex *texture);

extern s32 sdfAllocPacketAligned(s32 size);

extern void itfSendTablePacket(SdfListHead *list, s32 context, s32 mode);

extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);

extern s32 frFontMeasureLineWidth(s32 row, FrFontGlyph *glyph);

extern UiSprite *func_001A1858(s32, u32);

extern void itfMesOffsetNodeChain(FrFontGlyph *node, s32 dx, s32 dy);

extern void itfMesSetRowItemFlag();

extern void sndSetSequenceVolumePan();

extern void sndStepSequenceIndex(ItfMesBlk40 *sel, s32 dir);

extern s32 func_001A6AB8(ItfMesBlk40 *);

extern void itfResetBattleFadeState(BtlFade *, s32);

typedef struct UiOwnerRef { u8 pad0[0xC]; ItfMesState *owner; } UiOwnerRef;

extern SdfPoolNode kwlnDrawSurfaces[];

extern UiOwnerRef *D_003B4778[];

extern void itfBuildAndSubmitPanelPacket(UiSprite *sprite, SdfPoolNode *surface);

extern void func_001A7798(UiSprite *sprite);


/* Enable context rendering for every font object in the linked chain. */
void frFontEnableNodeContextModes(FrFontGlyph *fontObject) {
    for (; fontObject != NULL; fontObject = fontObject->previous) {
        frFontEnableContextMode(fontObject);
    }
}

void itfMesInitializePanelPlacementSprite(ItfMesState *panel) {
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    s32 top;
    if (place->sprite == 0) {
        place->sprite = func_001A1858(6, (u32)itfMesWork.windowTexture);
        if (place->frame != 0) {
            place->sprite->unk20 = panel->blk14.glyphChain->x + frFontMeasureLineWidth(0, panel->blk14.glyphChain);
        }
    }
    top = pos->y + place->bounds[1];
    itfSetPanelLayoutAndNotify(place->sprite, pos->x + place->bounds[0], top, pos->x + place->bounds[2], pos->y + place->bounds[3], panel->renderValue);
    place->sprite->screenY = top;
    itfPanelUpdateValuesAndNotify(place->sprite, place->unk1C, place->unk20, place->unk24, 0);
    panel->flags = (panel->flags & ~0x300) | 0x100;
}

void itfMesCreatePanelOriginFrameWhenVisible(ItfMesState *panel) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesBlkA4 *place = &panel->blkA4;
    if (origin->glyphChain != NULL && !(panel->flags & 0x10000)) {
        if (place->frame == NULL) {
            s32 width = origin->glyphChain->advance * 16;
            place->frame = func_001A1858(7, (u32)itfMesWork.windowTexture);
            itfSetPanelLayoutAndNotify(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0x110, panel->renderValue);
            itfPanelUpdateValuesAndNotify(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        panel->flags = (panel->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        panel->flags |= 0x3000;
    }
}

void itfResetCursorPositionAndState(ItfMesBlk14 *cursor, s32 resetPosition) {
    if (resetPosition != 0) {
        cursor->x = 0x280;
        cursor->y = 0xa10;
    }
    cursor->glyphChain = NULL;
    cursor->selectedIndex = 0xffff;
}

void itfMesResetCursorState(ItfMesEntryBlock *cur, s32 resetPos) {
    if (resetPos != 0) {
        cur->x = 0x4B0;
        cur->y = 0xAF8;
    }
    cur->glyphChain = NULL;
    cur->textState = 0;
    cur->unk11 = 0;
    cur->unk16 = 0;
    cur->itemIndex = 0;
    cur->tableCount = 0;
    cur->color[0] = 0;
    cur->color[1] = 0;
    cur->color[2] = 0;
    cur->color[3] = 0x80;
    cur->table = NULL;
}

void itfInitializeCursorResetState(ItfMesBlk40 *cursor) {
    cursor->x = 0x560;
    cursor->y = 0xC88;
    cursor->glyphChain = NULL;
    cursor->panelValue = 0;
    cursor->unk10 = 0;
    cursor->selectedIndex = -1;
    cursor->savedIndex = -1;
    cursor->rowCount = 0;
    cursor->unk18 = 0;
    cursor->unk1C = 0;
    cursor->unk20 = 0;
    cursor->optionCount = 0;
}

extern void func_001A6078(ItfMesBlkA4 *, s32, s32);

void itfResetWindowResourceBlock(ItfMesBlkA4 *block) {
    block->frame = NULL;
    block->sprite = NULL;
    block->overlay = NULL;
    func_001A6078(block, 0, 0);
}

/* Clear 32 words, from the end back toward the beginning of the buffer. */
void itfClearDrawStateWords(ItfMesTextSlots *slots) {
    s32 remaining;
    u32 *word;

    word = &slots->addresses[31];
    remaining = 0x1f;
    do {
        remaining = remaining - 1;
        *word = 0;
        word = word + -1;
    } while (-1 < remaining);
}

void itfResetBattleFadeState(BtlFade *fade, s32 preserveKind) {
    if (preserveKind == 0) {
        fade->kind = 0;
    }
    fade->phase = 0;
    fade->timer = 0;
    fade->alpha = 0x40;
    fade->unk08 = 0;
}

void btlSetFadePhaseAlphaTimer(BtlFade *fade, s16 phase, s16 alpha, s16 timer) {
    fade->phase = phase;
    fade->alpha = alpha;
    fade->timer = timer;
}

void btlReleaseEffectResourceHandles(ItfMesState *effect) {
    ItfMesBlkA4 *place = &effect->blkA4;
    if (place->frame != NULL) {
        itfPanelReleasePrimitiveResources(place->frame);
        place->frame = NULL;
    }
    if (place->sprite != NULL) {
        itfPanelReleasePrimitiveResources(place->sprite);
        place->sprite = NULL;
    }
    if (place->overlay != NULL) {
        itfPanelReleasePrimitiveResources(place->overlay);
        place->overlay = NULL;
    }
    effect->flags &= ~0xF00;
}

/* Release the handles in the second half for occupied entries in the first. */
void itfReleaseUiResourceSlotHandles(ItfMesTextSlots *slots) {
    s32 remaining;
    u32 *entries = slots->addresses;

    remaining = 0x1f;
    do {
        if (*entries != 0) {
            sdfReleaseResourceAllocation(slots->handles[entries - slots->addresses]);
            *entries = 0;
        }
        remaining = remaining - 1;
        entries = entries + 1;
    } while (-1 < remaining);
}

u16 *txtFormatNumberU16(s32 value, u16 *out) {
    s32 digits[10];
    s32 count = 0;
    s32 i;
    do {
        digits[count] = value % 10;
        value = value / 10;
        count++;
    } while (value > 0 && count < 10);
    for (i = count - 1; i >= 0; i--) {
        *out++ = (digits[i] << 8) - 0x6F80;
    }
    *out = 0;
    return out;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A6078);

void itfMesUpdatePanelFades(ItfMesState *panel);

void btlUpdateFadeIndicator(ItfMesState *panel);

void func_001A6528(ItfMesState *panel);

void itfUpdateBattleDisplayAndFadeIndicator(ItfMesState *panel) {
    itfMesUpdatePanelFades(panel);
    func_001A6350(panel);
    func_001A6528(panel);
    btlUpdateFadeIndicator(panel);
}

void itfMesUpdatePanelFades(ItfMesState *panel) {
    ItfMesBlkA4 *place = &panel->blkA4;
    UiSprite *sprite;
    s32 transition;

    sprite = place->sprite;
    transition = panel->flags & 0x300;
    switch (transition) {
    case 0x100:
        sprite->unk38 += 24;
        if (sprite->unk38 >= place->fadeLimit || panel->unk12 == 3) {
            sprite->unk38 = place->fadeLimit;
            panel->flags = (panel->flags & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite->unk38 -= 8;
        if (sprite->unk38 <= 0 || panel->unk12 == 3) {
            sprite->unk38 = 0;
            panel->flags &= ~0x300;
        }
        break;
    }

    sprite = place->frame;
    transition = panel->flags & 0x3000;
    switch (transition) {
    case 0x1000:
        sprite->unk38 += 32;
        if (sprite->unk38 >= 200) {
            sprite->unk38 = 200;
            panel->flags = (panel->flags & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        sprite->unk38 -= 32;
        if (sprite->unk38 <= 0) {
            sprite->unk38 = 0;
            panel->flags &= ~0x3000;
            itfPanelReleasePrimitiveResources(sprite);
            place->frame = NULL;
        }
        break;
    }

    sprite = place->overlay;
    transition = panel->flags & 0xC00;
    switch (transition) {
    case 0x400:
        sprite->unk38 += 24;
        if (sprite->unk38 >= place->fadeLimit) {
            sprite->unk38 = place->fadeLimit;
            panel->flags = (panel->flags & ~0xC07) | 0x803;
        }
        break;
    case 0xC00:
        sprite->unk38 -= 8;
        if (sprite->unk38 <= 0) {
            sprite->unk38 = 0;
            panel->flags &= ~0xC00;
            itfPanelReleasePrimitiveResources(sprite);
            place->overlay = NULL;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A6350);

extern void itfMesSetChildChainFlags(FrFontGlyph *glyph, u8 flagValue);

extern s32 itfMesNthClearBit(s32 clearBitsToSkip, u32 mask);

extern s32 sndSeqSelectPoll(ItfMesState *panel);

extern void itfMesShiftPanelVertically(ItfMesState *panel, s32 dy);

void func_001A6528(ItfMesState *panel) {
    ItfMesBlk40 *selection = &panel->blk40;
    ItfMesEntryBlock *entry = &panel->entryBlock;
    u32 flags = panel->flags;
    s32 top;
    s32 bottom;
    s32 entryY;

    switch (selection->unk10) {
    case 1:
        if (panel->unk12 == 3) {
            selection->unk10 = 2;
            panel->flags = (flags & ~0x38) | 0x18;
            break;
        }
        entryY = entry->y;
        bottom = entryY + entry->unk16 * (25 << 3);
        top = selection->y - ((selection->rowCount * 25 - 25) << 3);
        if (entryY == 0xAF8 && panel->blkA4.sprite != NULL) {
            panel->blkA4.sprite->scrollSpan = ((bottom - top) / 64) * 64 + 64;
        }
        if (entry->glyphChain != NULL && top < bottom) {
            itfMesShiftPanelVertically(panel, -64);
            return;
        }
        if ((flags & 0xC00) != 0x400) {
            if (entry->glyphChain != NULL) {
                itfMesSetChildChainFlags(entry->glyphChain, 3);
            }
            selection->unk10 = 2;
            panel->flags = (panel->flags & ~0x38) | 0x18;
        }
        break;
    case 2:
        if ((flags & 0x38) == 0x20 && sndSeqSelectPoll(panel) == 1) {
            selection->glyphChain = itfMesTrimGlyphChainToRow(selection->glyphChain,
                selection->selectedIndex, selection->rowCount);
            if ((flags & 0xC00) == 0x800) {
                panel->flags |= 0xC00;
            }
            selection->selectedIndex = itfMesNthClearBit(selection->selectedIndex, selection->panelValue);
            selection->optionCount = 0;
            btlSetFadePhaseAlphaTimer(&panel->fade, 1, 0x7F, 0);
            panel->flags = (panel->flags & ~0x38) | 0x28;
            selection->unk20 = 0x80;
            selection->unk10 = 3;
        }
        break;
    case 3:
        selection->unk20 -= 16;
        if (selection->unk20 <= 0) {
            selection->unk20 = 0;
            selection->unk10 = 4;
            itfResetBattleFadeState(&panel->fade, 0);
        }
        itfMesRecolorNodeChildren(selection->glyphChain, selection->unk20);
        return;
    case 4:
        if (entry->glyphChain != NULL && entry->y < 0xAF8) {
            itfMesShiftPanelVertically(panel, 64);
            return;
        }
        selection->unk10 = -1;
        break;
    }
}

void itfMesShiftPanelVertically(ItfMesState *panel, s32 dy) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    pos->y += dy;
    itfMesOffsetNodeChain(pos->glyphChain, 0, dy);
    if (place->sprite != 0) {
        itfAdvancePanelLayoutAndNotify(place->sprite, 0, dy, 0, 0, 0);
    }
    if (place->frame != 0) {
        itfAdvancePanelLayoutAndNotify(place->frame, 0, dy, 0, dy, 0);
    }
    if (origin->glyphChain != NULL) {
        origin->y += dy;
        itfMesOffsetNodeChain(origin->glyphChain, 0, dy);
    }
}

s32 sndSeqSelectPoll(ItfMesState *panel) {
    ItfMesBlk40 *sel = &panel->blk40;
    s32 dir = 0;
    s32 index;
    if (D_0037F510.prev & 2) {
        if (sel->selectedIndex != 0) {
            dir = -1;
        } else if (D_0037F510.prev < 0) {
            dir = -1;
        }
    } else if (D_0037F510.next & 2) {
        if (sel->selectedIndex != sel->rowCount - 1 || D_0037F510.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        sndStepSequenceIndex(sel, dir);
        itfResetBattleFadeState(&panel->fade, 1);
    }
    if (D_0037F510.confirm < 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        return 1;
    }
    if (sel->optionCount > 0 && (index = func_001A6AB8(sel)) >= 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        if (index != sel->selectedIndex) {
            itfMesSetRowItemFlag(sel->glyphChain, sel->selectedIndex, sel->rowCount, 0);
            itfMesSetRowItemFlag(sel->glyphChain, index, sel->rowCount, 1);
            sel->selectedIndex = index;
            sel->savedIndex = index;
        }
        return 1;
    }
    return 0;
}

void sndStepSequenceIndex(ItfMesBlk40 *sel, s32 dir) {
    s32 index = sel->selectedIndex;
    itfMesSetRowItemFlag(sel->glyphChain, index, sel->rowCount, 0);
    if (dir < 0) {
        index--;
        if (index < 0) {
            index = sel->rowCount - 1;
        }
    } else {
        index++;
        if (index >= sel->rowCount) {
            index = 0;
        }
    }
    itfMesSetRowItemFlag(sel->glyphChain, index, sel->rowCount, 1);
    sel->selectedIndex = index;
    sel->savedIndex = index;
    sndSetSequenceVolumePan(1, 0x7F, 0x3F);
}

s32 func_001A6AB8(ItfMesBlk40 *selection) {
    s32 i;

    for (i = 0; i < selection->optionCount; i++) {
        ItfMesOption *option = &selection->options[i];

        if (D_0037F510.buttons[option->id] < 0) {
            s32 prefixLength = option->value;
            s32 rank = 0;
            u32 mask = selection->panelValue;

            if (prefixLength > 0) {
                s32 remaining = prefixLength;
                do {
                    if ((mask & 1) == 0) {
                        rank++;
                    }
                    mask >>= 1;
                } while (--remaining != 0);
            }
            if ((mask & 1) == 0) {
                return rank;
            }
        }
    }
    return -1;
}

void btlUpdateFadeIndicator(ItfMesState *panel) {
    BtlFade *fade = &panel->fade;
    s32 minimumAlpha;
    if (fade->kind != 0) {
        if (fade->timer > 0) {
            fade->timer -= 8;
        }
        switch (fade->phase) {
        case 0:
            fade->alpha += 8;
            if (fade->alpha >= 0xFF) {
                fade->phase = 1;
                fade->alpha = 0xFF;
                fade->timer = 0x80;
            }
            break;
        case 1:
            fade->alpha -= 8;
            minimumAlpha = (fade->kind & 1) ? 0x20 : 0x40;
            if (fade->alpha <= minimumAlpha) {
                fade->alpha = minimumAlpha;
                fade->phase = 0;
            }
            break;
        }
    }
}

extern void itfMesRenderActivePanelSprites(ItfMesState *);

extern void itfDrawSoundSelectorFadeLayers(ItfMesState *);

extern void func_001A7120(ItfMesState *);

extern void func_001A6E88(ItfMesState *);

void itfUpdateSoundSelectorPanel(ItfMesState *panel) {
    u32 flags = panel->flags;
    ItfMesBlk40 *selection;
    FrFontGlyph *glyph;

    itfMesWork.flags &= ~2;
    itfMesRenderActivePanelSprites(panel);
    glyph = panel->blk14.glyphChain;
    if (!(flags & 0x10000) && glyph != 0) {
        frFontDrawGlyphInDefaultMode(glyph);
    }
    glyph = panel->entryBlock.glyphChain;
    if (!(flags & 0x20000) && (flags & 7) >= 3) {
        if (frFontDrawGlyphInDefaultMode(glyph) > 0) {
            if ((panel->flags & 7) != 4) {
                panel->fade.unk08 = 0;
                panel->flags = (panel->flags & ~7) | 4;
            }
        }
    }
    glyph = panel->blk40.glyphChain;
    if (!(flags & 0x40000)) {
        flags &= 0x38;
        if (flags >= 0x18 && frFontDrawGlyphWithSharedFlags(glyph, 1) > 0) {
            if (flags == 0x18) {
                selection = &panel->blk40;
                if (selection->selectedIndex == -1) {
                    selection->selectedIndex = 0;
                    selection->savedIndex = 0;
                }
                itfMesSetRowItemFlag(selection->glyphChain, selection->selectedIndex, selection->rowCount, 1);
                panel->flags = (panel->flags & ~0x38) | 0x20;
                panel->fade.kind = 2;
            }
        }
    }
    if (panel->fade.kind & 1) {
        itfDrawSoundSelectorFadeLayers(panel);
    }
    if (panel->fade.kind & 2) {
        if (panel->unk12 == 3) {
            func_001A7120(panel);
        } else {
            func_001A6E88(panel);
        }
    }
}

void itfMesRenderActivePanelSprites(ItfMesState *panel) {
    ItfMesBlkA4 *place;
    if (panel->flags & 0x80000) {
        return;
    }
    place = &panel->blkA4;
    if ((panel->flags & 0x300) >= 0x100) {
        if (panel->unk12 != 3) {
            itfBuildAndSubmitPanelPacket(place->sprite, &kwlnDrawSurfaces[panel->unk10]);
        }
        itfMesWork.flags |= 2;
        if (place->overlay != 0) {
            itfBuildAndSubmitPanelPacket(place->overlay, &kwlnDrawSurfaces[panel->unk10]);
        }
    }
    if (D_003B4778[0] != 0 && D_003B4778[0]->owner == panel && place != 0) {
        func_001A7798(place->sprite);
    }
}

extern DrawColorRec D_003B49B8[];

extern void func_001A09C0(DrawVertex *, DrawColorRec *, u32, s32, SdfListHead *);

extern void itfQueueColoredTexturedQuadPacket(DrawVertex *, DrawColorRec *, DrawColorRec *, u32, s32, SdfListHead *);

/* Draw the selected sound row and its expanding fade outline. */
void func_001A6E88(ItfMesState *panel) {
    DrawColorRec uv;
    DrawColorRec color;
    DrawVertex bounds[2];
    ItfMesBlk40 *selection = &panel->blk40;
    BtlFade *fade = &panel->fade;
    s32 selected = selection->savedIndex;
    SdfListHead *list;
    s32 x;
    s32 y;
    s32 bottom;
    s32 expansion;
    s32 rightExpansion;
    s32 verticalExpansion;
    SdfPoolNode *surface;

    if (selected == -1) {
        return;
    }
    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    x = selection->x;
    y = (s32)selection->y - ((selection->rowCount * 25 - 23) << 3) + selected * 0xA0;
    bottom = y + 0x88;
    bounds[0].x = x - 0x1D0;
    bounds[0].y = y + 0x20;
    bounds[1].x = x + frFontMeasureLineWidth(selected, selection->glyphChain) + 0x1D0;
    bounds[1].y = bounds[0].y + 0x90;
    func_001A09C0(bounds, D_003B49B8, panel->renderValue, 0x1D0, list);

    bounds[0].x = x;
    bounds[0].y = y + 8;
    bounds[1].x = x + 0x60;
    bounds[1].y = bottom;
    uv.components[0] = 0x150;
    uv.components[1] = 0x2F0;
    uv.components[2] = 0x1B0;
    uv.components[3] = 0x3F0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = 0x26;
    itfQueueTextureBoundQuadPacket(bounds, &uv, &color, panel->renderValue,
                                  itfMesWork.windowTexture, 0, list);

    bounds[0].x = x - 0xF0;
    bounds[0].y = y + 0x30;
    bounds[1].x = x - 0x30;
    bounds[1].y = bottom;
    uv.components[0] = 0x290;
    uv.components[1] = 0x10;
    uv.components[2] = 0x350;
    uv.components[3] = 0xC0;
    color.components[3] = fade->alpha;
    itfQueueColoredTexturedQuadPacket(bounds, &uv, &color, panel->renderValue, 0, list);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        rightExpansion = expansion << 1;
        verticalExpansion = expansion >> 1;
        bounds[0].x -= expansion;
        bounds[0].y -= verticalExpansion;
        bounds[1].x += rightExpansion;
        bounds[1].y += verticalExpansion;
        color.components[3] = fade->timer;
        itfSendTablePacket(list, 1, 0);
        itfQueueColoredTexturedQuadPacket(bounds, &uv, &color, panel->renderValue, 0, list);
        itfSendTablePacket(list, 0, 0);
    }
    surface = &kwlnDrawSurfaces[panel->unk10];
    surface->append(surface, list);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A7120);

extern u8 D_003B49F8[];

extern u8 D_003B49E8[];

extern s32 D_003B4A08[];

/* Draw the sound selector frame, its fade layer and the expanding timer outline. */
void itfDrawSoundSelectorFadeLayers(ItfMesState *object) {
    s32 bounds[4];
    BtlFade *fade = &object->fade;
    SdfListHead *packet;
    s32 expansion;
    SdfPoolNode *surface;

    bounds[0] = 0x1AA0;
    bounds[1] = 0xC60;
    bounds[2] = 0x1BD0;
    bounds[3] = 0xD58;
    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    D_003B4A08[3] = 0xFF;
    itfQueueTextureBoundQuadPacket(bounds, D_003B49F8, D_003B4A08, object->renderValue,
                                  itfMesWork.windowTexture, 0, packet);
    D_003B4A08[3] = fade->alpha;
    itfQueueTextureBoundQuadPacket(bounds, D_003B49E8, D_003B4A08, object->renderValue,
                                  itfMesWork.windowTexture, 0, packet);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        bounds[0] -= expansion * 2;
        bounds[1] -= expansion;
        bounds[2] += expansion * 2;
        bounds[3] += expansion;
        D_003B4A08[3] = fade->timer;
        itfSendTablePacket(packet, 1, 0);
        itfQueueTextureBoundQuadPacket(bounds, D_003B49E8, D_003B4A08, object->renderValue,
                                      itfMesWork.windowTexture, 0, packet);
        itfSendTablePacket(packet, 0, 0);
    }
    surface = &kwlnDrawSurfaces[object->unk10];
    surface->append(surface, packet);
}

s32 sndVisitQueuedResources(void) {
    ItfMesPoolNode *node;
    for (node = itfMesWork.pool.activeHead; node != 0; node = node->next) {
        itfUpdateBattleDisplayAndFadeIndicator((ItfMesState *)node->stateAddress);
    }
    return 0;
}

extern s32 func_001200E0(void);

s32 func_001A76C8(void) {
    ItfMesPoolNode *node;

    if (func_001200E0() != 0) {
        return 0;
    }
    for (node = itfMesWork.pool.activeHead; node != NULL; node = node->next) {
        itfUpdateSoundSelectorPanel((ItfMesState *)node->stateAddress);
    }
    itfMesWork.unk8++;
    return 0;
}

void sndFlushMessageQueue(void) {
    ItfMesPoolNode *node = itfMesWork.pool.activeHead;
    s32 message;
    while (node != 0) {
        message = node->index;
        node = node->next;
        itfMesDestroyWindow(message);
    }
    sdfTexReleaseReferenceViaHandler(itfMesWork.windowTexture);
    itfMesWork.windowTexture = NULL;
}

extern SdfPoolNode D_00380708;

extern s32 D_003B4A18[];

extern u8 D_00436630[5];

extern u8 D_00436638[5];

extern void itfEmitQuadListA(void *, void *, u8 *, u8 *, s32, u32, SdfListHead *);

void func_001A7798(UiSprite *sprite) {
    s32 vertices[4][2] = {
        {sprite->left, sprite->top},
        {sprite->right, sprite->top},
        {sprite->right, sprite->bottom},
        {sprite->left, sprite->bottom}
    };
    SdfListHead *list;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    itfEmitQuadListA(vertices, D_003B4A18, D_00436630, D_00436638, 5, 0xFFFFFF, list);
    D_00380708.append(&D_00380708, list);
}

void itfAdjustPanelBoundsWithPad(ItfMesBlkA4 *object, s32 mode) {
    UiSprite *sprite = object->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = object->bounds;
        if (D_0037F510.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)D_0037F510.coarseUp << 3) & 0x10;
        }
        if (D_0037F510.unk36 & 2) {
            dy = -8;
        } else {
            dy = ((u8)D_0037F510.unk37 << 2) & 8;
        }
        if (D_0037F510.unk31 != 0) {
            dx *= 8;
            dy *= 8;
        }
        if (dx != 0 || dy != 0) {
            switch (mode) {
            case 0:
                itfAdvancePanelLayoutAndNotify(sprite, dx, dy, 0, 0, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                break;
            case 1:
                itfAdvancePanelLayoutAndNotify(sprite, 0, 0, dx, dy, 0);
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            case 2:
                itfAdvancePanelLayoutAndNotify(sprite, dx, dy, dx, dy, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            }
        }
    }
}

/* Step the stepper's index by pad input: one per press, ten with the fast modifier held; mirror it into the target. */
void sndStepIndexByPad(SndPadStepper *stepper) {
    s32 step;

    if (D_0037F510.coarseDown & 2) {
        step = -1;
    } else {
        step = (D_0037F510.coarseUp & 2) > 0;
    }
    if (D_0037F510.unk36 & 2) {
        step = -1;
    } else if (D_0037F510.unk37 & 2) {
        step = 1;
    }
    if (D_0037F510.unk31 != 0) {
        step *= 10;
    }
    if (step != 0) {
        s32 index = (stepper->index + step) & 0xFF;

        stepper->index = index;
        if (stepper->target != NULL) {
            stepper->target->value = index;
        }
    }
}

ItfMesPoolNode *func_001A7A98(ItfMesPoolNode *node) {
    s32 index = 0;
    s32 count = itfMesWork.activeWindowCount;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (node != NULL) {
            node = node->previous;
        }
        if (node == NULL) {
            node = itfMesWork.pool.activeTail;
        }
        if (((ItfMesState *)node->stateAddress)->blkA4.sprite != NULL) {
            break;
        }
        index++;
        if (count < index) {
            node = NULL;
            break;
        }
    }
    return node;
}

void itfQueueOffsetTexturedRect(s32 *bounds, s32 *region, s32 x, s32 y,
                   s32 alpha, SdfTex *texture, SdfListHead *command) {
    s32 positions[4];
    s32 uv[4];
    UiQuadColor color = D_00414D50;

    positions[0] = (bounds[0] + x) << 4;
    positions[1] = (bounds[1] + y) << 3;
    positions[2] = (bounds[2] + x) << 4;
    positions[3] = (bounds[3] + y) << 3;
    uv[0] = region[0] << 4;
    uv[1] = region[1] << 4;
    uv[2] = (region[0] + region[2]) << 4;
    uv[3] = (region[1] + region[3]) << 4;
    color.alpha = alpha;
    itfQueueTextureBoundQuadPacket(positions, uv, &color, 0, texture, 0, command);
}

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D50);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D60);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A7C08);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A81F0);

extern void scrSetIntegerReturnValue(s32 value);
extern s32 itfMesStartEntry(s32 window, s32 entry, s32 option);
extern void itfPanelSetStatus(s32 window, s8 status);
extern void func_001A81F0(void);
extern s32 D_00438F2C;
extern s32 D_00438F30;
extern s16 D_00438F34;
extern u8 D_00438F36;
extern s16 D_00438F38;
extern u8 D_00452E60[0x10];
extern s8 D_0037F531[];

/* Start the window entry, then poll completion and restore its normal flags. */
s32 func_001A85E0(void) {
    s32 window = scrGetWindow();
    ItfMesState *state;
    s32 entry;
    if (window < 0) {
        return 1;
    }
    state = itfWindowSlots[window].mes;
    entry = scrReadIntParameter(0);
    if (state->entryBlock.textState == 0) {
        D_00438F2C = 0;
        D_00438F30 = 0;
        D_00438F34 = 0;
        D_00438F36 = 0;
        D_00438F38 = 0;
        memset(D_00452E60, 0, sizeof(D_00452E60));
        itfMesSetWindowHighFlags(window, 0x800000);
        itfMesSetWindowHighFlags(window, 0x100000);
        if (itfMesStartEntry(window, entry, 0) == 0) {
            return 1;
        }
    } else {
        func_001A81F0();
        if (D_00438F36 == 0) {
            if (D_0037F531[0] < 0) {
                D_00438F36 = 1;
                sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            }
        }
        if (D_00438F36 != 0) {
            if (D_00438F34 == 0) {
                itfMesClearWindowHighFlags(window, 0x800000);
                itfMesClearWindowHighFlags(window, 0x100000);
                itfPanelSetStatus(window, 0);
                itfMesCleanupWindow(window, 0);
                scrSetIntegerReturnValue(D_00438F2C);
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436608);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436610);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436618);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436620);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436628);

