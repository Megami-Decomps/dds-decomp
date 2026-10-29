#include "sdk_ps2.h"
#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_parameter_index.h"
#include "itf_mes_entry_start_api.h"
#include "itf_mes_voice_cue_result.h"
#include "itf.h"
#include "itf_message_manager_task.h"
#include "itf_option_bit_index_api.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "sdf_pac_reloc.h"
#include "fr_font_temporary_entry_api.h"
#include "fr_font_measure.h"
#include "itf_mes_state_api.h"
#include "itf_text_slots_api.h"
#include "fr_font_glyph_chain_count.h"
#include "itf_mes_window.h"
#include "itf_mes_panel_mode.h"
#include "itf_panel_api.h"
#include "kwln.h"
#include "kwln_task_create.h"


typedef mesflow_tag ItfMesWindowRec;






extern ItfMesSlot pinst[];

ItfMesGlobals itfMesMngManager __attribute__((section(".bss")));

extern ItfMesDebugState D_00357D80;

extern u32 itfMessageFlags;

ItfMesWindowIndex scrGetMessageHandle(void);

void fldNoopWithMode(s32 mode);

void itfMesMngSetMesPosition(ItfMesWindowIndex window, s32 x, s32 y);


void itfMesMngRequestMessageRelease(ItfMesWindowIndex window, ItfMesMessageReleaseMode releaseMode);

void itfMesMngRequestSelectRelease(ItfMesWindowIndex window);

ItfMesEntry *GetTypeHeader(instanceMes_tag *mes, ItfMesEntryIndex entryIndex);

ItfMesEntry *GetTypeHeader2(ItfMesSub *sub);

u8 *GetMessagePagePtr(ItfMesTable *table, s32 itemIndex);
const u8 *GetSpeakerName(instanceMes_tag *mes);

void ResolveResourceAddress(ItfMesSub *resource);

void DestructFlow(ItfMesWindowIndex window);

void itfMesSetTextSlotFromValue(ItfMesWindowIndex window, s32 slotIndex, s32 value, s32 selector);

s32 itfMesMngSetSelectDefKey(ItfMesWindowIndex window, s32 id, s32 value);




void frFontCenterGlyphChainAt(s32 x, s32 y, _FRQ *glyphChain);



extern u32 itfGetTextLipsUnitId(void);
extern u32 itfGetTextLipsMotionId(void);

void ChangeZBuffer(_FRQ *glyph, s32 renderValue);

_FRQ *GetLastLinkFontHandle(_FRQ *glyph);


void CommitVarTable(u8 **table);

s32 GetMessageWidth(_FRQ *glyph);

void SetVar(instanceMes_tag *mes, s32 slotIndex, const char *source, s32 byteCount);

void itfMesInitializePanelPlacementSprite(instanceMes_tag *panel);

void OpenCheckSpeakerWindow(instanceMes_tag *panel);

#define ITF_MES_MAGIC_MSG0 0x3047534d
#define ITF_MES_MAGIC_MSG1 0x3147534d
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
#define ITF_MES_OPTION_ROW_SPACING 21
#define ITF_MES_COLOR_BYTE_CLEAR_MASK 0xffffff00
#define ITF_MES_DEFAULT_COLOR_VALUE 0x80
#define ITF_MES_TEXT_SLOT_COUNT 0x20
#define ITF_MES_MASK_BIT_COUNT ITF_MES_OPTION_BIT_INDEX_NOT_FOUND
#define ITF_MES_STRING_COPY_ROUND_BIAS 4
#define ITF_MES_BINARY_COPY_ROUND_BIAS 5
#define ITF_MES_COPY_ALIGN_MASK 3

/* Store the script's panel mask; a negative script window is a no-op. Return 1. */
s32 itfMesMngMaskSelect(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetMaskSelect(window, scrGetIntPara(0));
    return 1;
}

/* Cache the clear-bit rank for the script's selected bit index; return 1. */
s32 itfMesMng_SEL_SELNO(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetSelect(window, scrGetIntPara(0));
    return 1;
}

/* Request script panel activation, then notify the script-side state routine. */
s32 itfMesMngOpenWindow(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngMarkBitStat(window, ITF_MES_SCRIPT_PANEL_BIT);
    fldNoopWithMode(2);
    return 1;
}

/* Finish single-bit states of the lower status pair; return whether its original
 * value was zero. Finishing a nonzero state therefore still returns 0. */
s32 itfMesMngCloseWindow(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    instanceMes_tag *mes;
    u32 scriptStatus;

    if (window < 0) {
        return 1;
    }
    mes = pinst[window].mes;
    scriptStatus = mes->flags & ITF_MES_LOWER_STATUS_PAIR_MASK;
    if (scriptStatus == ITF_MES_LOWER_STATUS_FIRST_BIT || scriptStatus == ITF_MES_LOWER_STATUS_SECOND_BIT) {
        itfMesMngMsgWndCls(window);
        fldNoopWithMode(3);
    }
    return scriptStatus < 1;
}

/* Mark each originally nonzero status pair with both bits, then clean/reset
 * content and clear the script panel bit. The status pairs are set, not cleared. */
void itfMesMngMsgWndCls(ItfMesWindowIndex window) {
    instanceMes_tag *mes = pinst[window].mes;
    u32 previousFlags = mes->flags;

    if (previousFlags & ITF_MES_LOWER_STATUS_PAIR_MASK) {
        mes->flags = previousFlags | ITF_MES_LOWER_STATUS_PAIR_MASK;
    }
    if (previousFlags & ITF_MES_UPPER_STATUS_PAIR_MASK) {
        mes->flags = mes->flags | ITF_MES_UPPER_STATUS_PAIR_MASK;
    }
    itfMesMngRequestMessageRelease(window, ITF_MES_RELEASE_ENTRY_AND_PRIMARY_TEXT);
    itfMesMngRequestSelectRelease(window);
    mes->flags &= ITF_MES_CLEAR_SCRIPT_PANEL_BIT;
}

u32 itfMesMngOpenSelectWindow(void) {
    return 1;
}

u32 itfMesMngCloseSelectWindow(void) {
    return 1;
}

/* Read the text-slot index, value and selector from script parameters. */
s32 itfMesMngSetVarScr(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    s32 slotIndex;
    s32 value;
    s32 selector;

    if (window < 0) {
        return 1;
    }
    slotIndex = scrGetIntPara(0);
    value = scrGetIntPara(1);
    selector = scrGetIntPara(2);
    itfMesSetTextSlotFromValue(window, slotIndex, value, selector);
    return 1;
}

/* Toggle suppression of automatic glyph-context enabling from script argument 0. */
s32 itfMesMngIgnoreConfig(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    if (scrGetIntPara(0)) {
        itfMesMngMarkBitStat(window, ITF_MES_SKIP_CONTEXT_ENABLE_BIT);
    } else {
        itfMesMngClearBitStat(window, ITF_MES_SKIP_CONTEXT_ENABLE_BIT);
    }
    return 1;
}

/* Convert script coordinates to the text units (x * 16, y * 8); return 1. */
s32 itfMesMngSetMesPositionScript(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    s32 x;
    s32 y;

    if (window < 0) {
        return 1;
    }
    x = scrGetIntPara(0);
    y = scrGetIntPara(1);
    itfMesMngSetMesPosition(window, x << ITF_MES_TEXT_X_SHIFT, y << ITF_MES_TEXT_Y_SHIFT);
    return 1;
}

/* Move selected-entry text to the fixed default position; return 1. */
s32 itfMesMngSetMesPosDefault(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    itfMesMngSetMesPosition(window, ITF_MES_DEFAULT_TEXT_X, ITF_MES_DEFAULT_TEXT_Y);
    return 1;
}

/* The two opaque high bits use inverted script polarity: true clears both.
 * Keep the two separate calls and their original order. */
s32 itfMesMngValidateNext(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();

    if (window < 0) {
        return 1;
    }
    if (scrGetIntPara(0)) {
        itfMesMngClearBitStat(window, 0x800000);
        itfMesMngClearBitStat(window, 0x100000);
    } else {
        itfMesMngMarkBitStat(window, 0x800000);
        itfMesMngMarkBitStat(window, 0x100000);
    }
    return 1;
}

/* Refresh the script window with argument 0 and a zero second value; return 1. */
s32 itfMesMngWindowType(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    ItfMesPanelMode panelMode;

    if (window < 0) {
        return 1;
    }
    panelMode = scrGetIntPara(0);
    itfMesMngChangeWindowType(window, panelMode, 0);
    return 1;
}

/* Read both refresh values in order; a negative script window is a no-op. */
s32 itfMesMngWindowTypeAlpha(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    ItfMesPanelMode panelMode;
    ItfMesPanelFadeLimit fadeLimit;

    if (window < 0) {
        return 1;
    }
    panelMode = scrGetIntPara(0);
    fadeLimit = scrGetIntPara(1);
    itfMesMngChangeWindowType(window, panelMode, fadeLimit);
    return 1;
}

/* Pass both script range values in order; a negative script window is a no-op. */
s32 itfMesMngSelectDefKey(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    s32 first;
    s32 second;

    if (window < 0) {
        return 1;
    }
    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    itfMesMngSetSelectDefKey(window, first, second);
    return 1;
}

/* Return the loaded message-window texture resource, not a window index. */
sdfTexHandle_t *itfMesGetWindowTexture(void) {
    return itfMesMngManager.windowTexture;
}

/* Set requested bits in the separate message-global word. */
void itfMesSetFlags(u32 mask) {
    itfMessageFlags |= mask;
}

/* Clear requested bits in the separate message-global word. */
void itfMesClearFlags(u32 mask) {
    itfMessageFlags &= ~mask;
}

extern ItfMesSub *itfMesMngSetResourcePtr(ItfMesWindowIndex window, ItfMesSub *sub);

ItfMesWindowIndex itfMesMngInitialize(ItfMesSub *sub) {
    mesflow_tag *node = itfMiscLinkTake(&itfMesMngManager.pool);
    ItfMesWindowIndex window = node->index;
    instanceMes_tag *mes;
    struct sdfMemHandle_t *handle;

    handle = sdfMemAllocDirect(0x1E0);
    node->resourceHandle = handle;
    mes = (instanceMes_tag *)sdfMemLock(handle);
    node->state = mes;
    mes->sub = NULL;
    itfMesMngSetResourcePtr(window, sub);
    mes->flags = 0;
    mes->drawSurfaceIndex = 0x53;
    mes->renderValue = 0xFFFFF0;
    mes->panelMode = ITF_MES_PANEL_MODE_STANDARD;
    mes->entryGlyphCallback = 0;
    InitializeSpeaker(&mes->blk14, 1);
    InitializeMessage(&mes->entryBlock, 1);
    itfInitializeCursorResetState(&mes->blk40);
    itfResetWindowResourceBlock(&mes->blkA4);
    InitializeVariable(&mes->textSlots);
    InitializeCurCtr(&mes->fade, 0);
    itfMesMngManager.activeWindowCount++;
    return window;
}

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(ItfMesWindowIndex window) {
    if (window >= 0 && pinst[window].mes != NULL) {
        DestructFlow(window);
    }
}

void itfMesBuildEntryGlyph(instanceMes_tag *mes);

/* Install an entry's table into the window, rebuild its text glyphs and the
 * selected-item caption, then mark the entry block active. */
ItfMesEntryStartResult itfMesStartEntry(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex) {
    instanceMes_tag *mes = pinst[window].mes;
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;
    ItfMesTable *table = GetTypeHeader(mes, entryIndex)->table;
    ItfMesBlk14 *captionBlock;
    u16 selected;
    u32 flags;

    entryBlock->itemIndex = itemIndex;
    entryBlock->table = table;
    entryBlock->tableCount = table->count;
    if (table->count == 0) {
        return ITF_MES_ENTRY_START_EMPTY_TABLE;
    }
    CommitVarTable(mes->textSlots.addresses);
    if (mes->temporaryFontEntry != 0) {
        frSetExtDic(mes->temporaryFontEntry);
    }
    itfMesBuildEntryGlyph(mes);
    captionBlock = &mes->blk14;
    if (captionBlock->selectedIndex != (u16)table->bitCount) {
        selected = table->bitCount;
        if (captionBlock->glyphChain != NULL) {
            frDestroyFontRequestAfter(captionBlock->glyphChain);
            captionBlock->glyphChain = NULL;
        }
        captionBlock->selectedIndex = selected;
        if (selected != 0xFFFF) {
            if (mes->blkA4.frame != NULL) {
                itfPnlRelease(mes->blkA4.frame);
                mes->blkA4.frame = NULL;
            }
            if (selected & 0x8000) {
                captionBlock->glyphChain = MessageRequestEx(captionBlock->x, captionBlock->y, 0, 3, 0, ITF_MES_DEFAULT_COLOR_VALUE,
                    itfMesMngGetVar(window, selected & 0x7FFF), NULL);
            } else {
                captionBlock->glyphChain = MessageRequestEx(captionBlock->x, captionBlock->y, 0, 3, 0, ITF_MES_DEFAULT_COLOR_VALUE,
                    GetSpeakerName(mes), NULL);
            }
            if (frFontMeasureGlyphStripWidth(0, captionBlock->glyphChain) >= 0x69) {
                frSetDrawPosition(captionBlock->glyphChain, captionBlock->x, captionBlock->y);
            } else {
                frFontCenterGlyphChainAt(0x5C0, captionBlock->y, captionBlock->glyphChain);
            }
            ChangeZBuffer(captionBlock->glyphChain, mes->renderValue);
        }
    }
    OpenCheckSpeakerWindow(mes);
    flags = mes->flags;
    if ((flags & ITF_MES_SCRIPT_PANEL_BIT) &&
        ((flags & ITF_MES_LOWER_STATUS_PAIR_MASK) == 0 || (flags & ITF_MES_LOWER_STATUS_PAIR_MASK) == ITF_MES_LOWER_STATUS_PAIR_MASK)) {
        itfMesInitializePanelPlacementSprite(mes);
        mes->flags = (mes->flags & ~ITF_MES_ENTRY_STATE_MASK) | 2;
    } else {
        mes->flags = (flags & ~ITF_MES_ENTRY_STATE_MASK) | 3;
    }
    entryBlock->textState = 1;
    mes->flags |= 0x1000000;
    return ITF_MES_ENTRY_START_STARTED;
}

extern s32 itfInitTextDrawArgs(const u8 *encodedText, _FRQ *sub);

/* Cache the selected table/item; report whether its text contains the F413 cue. */
ItfMesVoiceCueResult itfMesSelectTableItemText(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex) {
    instanceMes_tag *mes = pinst[window].mes;
    ItfMesTable *table = GetTypeHeader(mes, entryIndex)->table;
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;

    entryBlock->itemIndex = itemIndex;
    entryBlock->table = table;
    entryBlock->tableCount = table->count;
    if (table->count != 0) {
        return itfInitTextDrawArgs(GetMessagePagePtr(table, (s16)itemIndex), 0);
    }
    return ITF_MES_VOICE_CUE_NOT_FOUND;
}

/* Release/reset selected-entry glyphs and their status; optionally do the same
 * for primary text. Negative window is a no-op; no other slot validation. */
void itfMesMngRequestMessageRelease(ItfMesWindowIndex window, ItfMesMessageReleaseMode releaseMode) {
    instanceMes_tag *mes;
    ItfMesEntryBlock *entryBlock;
    ItfMesBlk14 *primaryTextBlock;

    if (window < 0) {
        return;
    }
    mes = pinst[window].mes;
    entryBlock = &mes->entryBlock;
    primaryTextBlock = &mes->blk14;
    if (entryBlock->glyphChain != NULL) {
        frDestroyFontRequestAfter(entryBlock->glyphChain);
        entryBlock->glyphChain = NULL;
    }
    InitializeMessage(entryBlock, 0);
    mes->flags &= ~ITF_MES_ENTRY_STATE_MASK;
    mes->flags &= ITF_MES_ENTRY_STATE_HIGH_CLEAR_MASK;
    if (releaseMode == ITF_MES_RELEASE_ENTRY_ONLY) {
        return;
    }
    if (primaryTextBlock->glyphChain != NULL) {
        frDestroyFontRequestAfter(primaryTextBlock->glyphChain);
        primaryTextBlock->glyphChain = NULL;
    }
    InitializeSpeaker(primaryTextBlock, 0);
}

/* Build the option panel around the widest measured row, using this game's
 * fixed row spacing. Preserve the existing panel creation/layout order. */
void itfMesBuildOptionFrame(instanceMes_tag *mes) {
    ItfMesBlk40 *optionBlock = &mes->blk40;
    ItfMesBlkA4 *panelBlock = &mes->blkA4;
    s32 bounds[4];
    s32 maxRowWidth = 0;
    s32 rowsHeight;
    s32 halfWidth;
    s32 rowIndex;
    s32 rowWidth;

    frFontCenterGlyphChainAt(0x1000, 0x4B0, optionBlock->glyphChain);
    rowsHeight = optionBlock->rowCount * ITF_MES_OPTION_ROW_SPACING * 8;
    for (rowIndex = 0; rowIndex < optionBlock->rowCount; rowIndex++) {
        rowWidth = frFontMeasureGlyphStripWidth(rowIndex, optionBlock->glyphChain);
        if (maxRowWidth < rowWidth) {
            maxRowWidth = rowWidth;
        }
    }
    halfWidth = maxRowWidth / 2;
    bounds[0] = 0xE00 - halfWidth;
    bounds[1] = 0x430;
    bounds[2] = 0x1200 + halfWidth;
    bounds[3] = 0x530 + rowsHeight;
    panelBlock->overlay = itfCreateUiSpriteByKind(9, itfMesMngManager.windowTexture);
    itfPnlSetPosition(panelBlock->overlay, bounds[0], bounds[1], bounds[2], bounds[3], mes->renderValue);
    itfPnlSetColor(panelBlock->overlay, 0, 0, 0, 0);
    mes->flags = (mes->flags & ~0xC00) | 0x400;
}

extern s32 GetNumSelectItem(s32 bitCount, u32 remainingMask);

/* Replace the option glyph chain with rows selected by clear panel-mask bits.
 * Item zero is skipped; mode 3 additionally builds the surrounding frame. */
void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex) {
    instanceMes_tag *mes = pinst[window].mes;
    ItfMesBlk40 *optionBlock = &mes->blk40;
    ItfMesEntry *entry = GetTypeHeader(mes, entryIndex);
    ItfMesTable *table;
    s32 visibleRowCount;
    s32 firstRowY;

    if (optionBlock->glyphChain != NULL) {
        frDestroyFontRequestAfter(optionBlock->glyphChain);
        optionBlock->glyphChain = NULL;
    }
    table = entry->table;
    visibleRowCount = GetNumSelectItem(table->bitCount, optionBlock->panelValue);
    firstRowY = optionBlock->y - ((visibleRowCount - 1) * ITF_MES_OPTION_ROW_SPACING << ITF_MES_TEXT_Y_SHIFT);
    CommitVarTable(mes->textSlots.addresses);
    if (mes->temporaryFontEntry != 0) {
        frSetExtDic(mes->temporaryFontEntry);
    }
    optionBlock->glyphChain = itfMesBuildNodeRows(&table->items[1], table->bitCount, optionBlock->panelValue, optionBlock->x, firstRowY, mes->renderValue);
    optionBlock->rowCount = visibleRowCount;
    mes->flags = (mes->flags & ~ITF_MES_OPTION_STATE_MASK) | ITF_MES_OPTION_LIST_READY;
    if (mes->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
        itfMesBuildOptionFrame(mes);
    }
    optionBlock->selectionState = ITF_SOUND_SELECTION_STATE_OPENING;
}

/* Release option glyphs, clear option selection/state and invalidate its rank.
 * Other content and the window allocation remain installed. */
void itfMesMngRequestSelectRelease(ItfMesWindowIndex window) {
    instanceMes_tag *mes = pinst[window].mes;
    ItfMesBlk40 *optionBlock = &mes->blk40;

    if (optionBlock->glyphChain != NULL) {
        frDestroyFontRequestAfter(optionBlock->glyphChain);
        optionBlock->glyphChain = NULL;
    }
    optionBlock->panelValue = 0;
    optionBlock->selectionState = ITF_SOUND_SELECTION_STATE_INACTIVE;
    optionBlock->rowCount = 0;
    optionBlock->selectedIndex = -1;
    mes->flags &= ~ITF_MES_OPTION_STATE_MASK;
    mes->flags &= ITF_MES_OPTION_STATE_HIGH_CLEAR_MASK;
}

/* Store the mask whose clear bits select displayed option rows. */
void itfMesMngSetMaskSelect(ItfMesWindowIndex window, u32 panelMask) {
    pinst[window].mes->blk40.panelValue = panelMask;
}

/* Cache the number of clear bits below selectedBitIndex only when that bit is
 * itself clear; a set bit produces zero. Negative indices also produce zero;
 * no upper bound is checked. */
void itfMesMngSetSelect(ItfMesWindowIndex window, ItfMesOptionBitIndex selectedBitIndex) {
    ItfMesBlk40 *optionBlock = &pinst[window].mes->blk40;
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

extern void ChangeFontPosition(_FRQ *, s32, s32);

/* Move primary text to an absolute position; an unchanged position is a no-op. */
void itfMesMngSetSpeakerPosition(ItfMesWindowIndex window, s32 x, s32 y) {
    ItfMesBlk14 *primaryTextBlock = &pinst[window].mes->blk14;
    s32 delta[2];

    delta[0] = x - primaryTextBlock->x;
    delta[1] = y - primaryTextBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(primaryTextBlock->glyphChain, delta[0], delta[1]);
    primaryTextBlock->x = x;
    primaryTextBlock->y = y;
}

/* Translate primary text and update its cached position, including zero deltas. */
void itfMesMngSetSpeakerPositionOffset(ItfMesWindowIndex window, s32 dx, s32 dy) {
    ItfMesBlk14 *primaryTextBlock = &pinst[window].mes->blk14;

    ChangeFontPosition(primaryTextBlock->glyphChain, dx, dy);
    primaryTextBlock->x += dx;
    primaryTextBlock->y += dy;
}

/* Move selected-entry text absolutely; an unchanged position is a no-op. */
void itfMesMngSetMesPosition(ItfMesWindowIndex window, s32 x, s32 y) {
    ItfMesEntryBlock *entryBlock = &pinst[window].mes->entryBlock;
    s32 delta[2];

    delta[0] = x - entryBlock->x;
    delta[1] = y - entryBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(entryBlock->glyphChain, delta[0], delta[1]);
    entryBlock->x = x;
    entryBlock->y = y;
}

/* Translate selected-entry text and update its cached position. */
void itfMesMngSetMesPositionOffset(ItfMesWindowIndex window, s32 dx, s32 dy) {
    ItfMesEntryBlock *entryBlock = &pinst[window].mes->entryBlock;

    ChangeFontPosition(entryBlock->glyphChain, dx, dy);
    entryBlock->x += dx;
    entryBlock->y += dy;
}

/* Move option text absolutely; an unchanged position is a no-op. */
void itfMesMngSetSelPosition(ItfMesWindowIndex window, s32 x, s32 y) {
    ItfMesBlk40 *optionBlock = &pinst[window].mes->blk40;
    s32 delta[2];

    delta[0] = x - optionBlock->x;
    delta[1] = y - optionBlock->y;
    if (delta[0] == 0 && delta[1] == 0) {
        return;
    }
    ChangeFontPosition(optionBlock->glyphChain, delta[0], delta[1]);
    optionBlock->x = x;
    optionBlock->y = y;
}

/* Translate option text and update its cached position. */
void itfMesMngSetSelPositionOffset(ItfMesWindowIndex window, s32 dx, s32 dy) {
    ItfMesBlk40 *optionBlock = &pinst[window].mes->blk40;

    ChangeFontPosition(optionBlock->glyphChain, dx, dy);
    optionBlock->x += dx;
    optionBlock->y += dy;
}

/* Propagate a changed render value to all three glyph chains before caching it. */
void itfMesMngSetPriority(ItfMesWindowIndex window, s32 renderValue) {
    instanceMes_tag *mes = pinst[window].mes;

    if (mes->renderValue == renderValue) {
        return;
    }
    ChangeZBuffer(mes->blk14.glyphChain, renderValue);
    ChangeZBuffer(mes->entryBlock.glyphChain, renderValue);
    ChangeZBuffer(mes->blk40.glyphChain, renderValue);
    mes->renderValue = renderValue;
}

/* Return the complete status/flag word, not just its high-half flags. */
ItfMesStatusWord itfMesMngGetStat(ItfMesWindowIndex window) {
    return pinst[window].mes->flags;
}

/* Replace only the high half, preserving the existing low-half cast. */
void itfMesMngSetStat(ItfMesWindowIndex window, ItfMesHighStatusBits highFlags) {
    instanceMes_tag *mes = pinst[window].mes;

    mes->flags = (u32)(u16)mes->flags | (highFlags & ITF_MES_HIGH_FLAGS_MASK);
}

/* Set only high-half mask bits; low-half status is unchanged. */
void itfMesMngMarkBitStat(ItfMesWindowIndex window, ItfMesHighStatusBits mask) {
    instanceMes_tag *mes = pinst[window].mes;

    mes->flags |= mask & ITF_MES_HIGH_FLAGS_MASK;
}

/* Clear only high-half mask bits; low-half status is unchanged. */
void itfMesMngClearBitStat(ItfMesWindowIndex window, ItfMesHighStatusBits mask) {
    instanceMes_tag *mes = pinst[window].mes;

    mes->flags &= ~mask | ITF_MES_LOW_FLAGS_MASK;
}

/* After chooser completion, selectedIndex holds the original mask bit; reset is -1. */
ItfMesSelectedOptionBitResult itfMesMngGetSelect(ItfMesWindowIndex window) {
    return pinst[window].mes->blk40.selectedIndex;
}

/* Return the selected table's cached item count, without another table lookup. */
s16 itfMesMngGetMesPage(ItfMesWindowIndex window) {
    return pinst[window].mes->entryBlock.tableCount;
}

/* Return the stored signed-halfword item index, without another table lookup. */
s16 itfMesMngGetMesCurrentPage(ItfMesWindowIndex window) {
    return pinst[window].mes->entryBlock.itemIndex;
}

/* Return the replacement text stored in a window slot. */
const u8 *itfMesMngGetVar(ItfMesWindowIndex window, s32 slotIndex) {
    return pinst[window].mes->textSlots.addresses[slotIndex];
}

extern const char *D_003BAA70;
extern const char *D_003BAA74;
extern const char *D_003BAA84;
extern const char *D_003BAA88;
extern const char *D_003BAA8C;
extern const char *D_003BAA90;
extern const char D_00347C68[][0x20];
extern const char D_003482A8[][0x20];
extern s32 D_0032ACA8[128];
extern char D_003BB1F0[];
extern void itfConvertText(char *dst, const char *src);
extern u16 *txtFormatNumberU16(s32 value, u16 *dst);
void itfMesMngSetVarImmediate(ItfMesWindowIndex window, u32 slotIndex, const char *sourceText);

/* Copy one of the built-in interface strings into a window replacement slot. */
void itfMesSetTextSlotFromValue(ItfMesWindowIndex window, s32 slotIndex, s32 value, s32 selector) {
    char formatted[0x10];
    u16 number[0x20];
    char converted12[0x11];
    char converted13[0x19];
    char converted15[0x11];
    char converted14[0x11];
    char *converted;

    switch (selector) {
    case 0:
        sprintf(formatted, D_003BB1F0, value);
        itfMesMngSetVarImmediate(window, slotIndex, formatted);
        break;
    case 1:
        itfMesMngSetVarImmediate(window, slotIndex, D_003BAA74 + value * 0x11);
        break;
    case 14:
        memset(converted14, 0, sizeof(converted14));
        itfConvertText(converted14, D_003BAA74 + value * 0x11);
        itfMesMngSetVarImmediate(window, slotIndex, converted14);
        break;
    case 8:
        itfMesMngSetVarImmediate(window, slotIndex, D_003BAA88 + value * 7);
        break;
    case 2:
        itfMesMngSetVarImmediate(window, slotIndex, D_003BAA70 + value * 0x11);
        break;
    case 15:
        memset(converted15, 0, sizeof(converted15));
        itfConvertText(converted15, D_003BAA70 + value * 0x11);
        itfMesMngSetVarImmediate(window, slotIndex, converted15);
        break;
    case 11:
        itfMesMngSetVarImmediate(window, slotIndex, D_003BAA90 + value * 0x11);
        break;
    case 12:
        memset(converted12, 0, sizeof(converted12));
        itfConvertText(converted12, D_003BAA90 + value * 0x11);
        itfMesMngSetVarImmediate(window, slotIndex, converted12);
        break;
    case 3:
        itfMesMngSetVarImmediate(window, slotIndex, D_003BAA84 + value * 0x19);
        break;
    case 13:
        memset(converted13, 0, sizeof(converted13));
        itfConvertText(converted13, D_003BAA84 + value * 0x19);
        itfMesMngSetVarImmediate(window, slotIndex, converted13);
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
            }
            converted = (char *)txtFormatNumberU16(value, (u16 *)converted);
            ((u16 *)converted)[0] = 0xA680;
            ((u16 *)converted)[1] = 0;
        }
        itfMesMngSetVarImmediate(window, slotIndex, number);
        break;
    case 5:
        itfMesMngSetVarImmediate(window, slotIndex, D_003BAA8C + value * 0x11);
        break;
    case 6:
        itfMesMngSetVarImmediate(window, slotIndex, D_00347C68[value]);
        break;
    case 7:
        itfMesMngSetVarImmediate(window, slotIndex, D_003482A8[value]);
        break;
    case 9:
        itfMesMngSetVarImmediate(window, slotIndex, (const void *)(D_0032ACA8[value]));
        break;
    case 10:
        break;
    }
}

/* Replace a text slot with a copied NUL-terminated encoded string. */
void itfMesMngSetVarImmediate(ItfMesWindowIndex window, u32 slotIndex, const char *sourceText) {
    SetVar(pinst[window].mes, slotIndex, sourceText, 0);
}

/* Copy text into one of the window's message text slots. */
void itfMesMngSetVarImmediate2(ItfMesWindowIndex window, s32 slotIndex, const char *source, s32 byteCount) {
    SetVar(pinst[window].mes, slotIndex, source, byteCount);
}

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
ItfMesSub *itfMesMngSetResourcePtr(ItfMesWindowIndex window, ItfMesSub *sub) {
    instanceMes_tag *mes = pinst[window].mes;
    ItfMesSub *previous = mes->sub;
    ItfMesEntry *entry;
    u8 *temporaryFontEntry;

    mes->sub = sub;
    ResolveResourceAddress(sub);
    entry = GetTypeHeader2(sub);
    temporaryFontEntry = 0;
    if (sub->magic == ITF_MES_MAGIC_MSG1) {
        temporaryFontEntry = ((ItfMesTrailingFontEntry *)entry)->temporaryFontEntry;
    }
    mes->temporaryFontEntry = temporaryFontEntry;
    return previous;
}

/* Look up an entry-table item; only the item index has a bounds check. */
u8 *itfMesMngGetDataPtrImmediate(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex) {
    return GetMessagePagePtr(GetTypeHeader(pinst[window].mes, entryIndex)->table, itemIndex);
}

extern void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *blk, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

/* Refresh changed panel values only when both content-status pairs are clear.
 * Release the old primitive after applying the panel mode and fade limit. */
void itfMesMngChangeWindowType(ItfMesWindowIndex window, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit) {
    instanceMes_tag *mes = pinst[window].mes;
    ItfMesBlkA4 *panelBlock = &mes->blkA4;

    if ((mes->flags & ITF_MES_CONTENT_STATUS_MASK) == 0) {
        if (mes->panelMode != panelMode || panelBlock->fadeLimit != fadeLimit) {
            mes->panelMode = panelMode;
            itfMesConfigurePanelBoundsAndFadeLimit(panelBlock, panelMode, fadeLimit);
            if (panelBlock->sprite != NULL) {
                itfPnlRelease(panelBlock->sprite);
                panelBlock->sprite = NULL;
            }
        }
    }
}

/* Set or append the option with `id` in the window's option list; ids 1, 6, 7 and >= 16 are rejected. */
s32 itfMesMngSetSelectDefKey(ItfMesWindowIndex window, s32 id, s32 value) {
    ItfMesBlk40 *optionBlock = &pinst[window].mes->blk40;
    ItfMesOption *option = NULL;
    s32 optionIndex;

    if ((u32)id >= 0x10) {
        return 0;
    }
    if (id == 1 || id == 6 || id == 7) {
        return 0;
    }
    for (optionIndex = 0; optionIndex < optionBlock->optionCount; optionIndex++) {
        if (optionBlock->options[optionIndex].id == id) {
            option = &optionBlock->options[optionIndex];
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
s32 itfMesMngGetWidth(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex, s32 itemIndex) {
    instanceMes_tag *mes = pinst[window].mes;
    ItfMesTable *table;
    u8 *encodedText;
    _FRQ *glyphChain;
    s32 textExtent;

    table = GetTypeHeader(mes, entryIndex)->table;
    if (table->count == 0) {
        return 0;
    }
    CommitVarTable(mes->textSlots.addresses);
    if (mes->temporaryFontEntry != 0) {
        frSetExtDic(mes->temporaryFontEntry);
    }
    encodedText = GetMessagePagePtr(table, itemIndex);
    if (encodedText == 0) {
        return 0;
    }
    glyphChain = MessageRequest(0, 0, encodedText, 0);
    textExtent = GetMessageWidth(glyphChain);
    frDestroyFontRequestAfter(glyphChain);
    return textExtent;
}

/* Set the fourth color channel used when this window's entry glyphs are built. */
void itfMesMngSetDefaultSpeed(ItfMesWindowIndex window, u8 channelValue) {
    pinst[window].mes->entryBlock.color[3] = channelValue;
}

/* Store the optional callback address invoked without arguments during glyph building. */
void itfMesMngSetKutiCallback(ItfMesWindowIndex window, void (*callback)(void)) {
    pinst[window].mes->entryGlyphCallback = callback;
}

/* Initialize the fixed window pool, load its texture and register the named
 * calculation/drawing tasks. Keep the existing task flags and callback casts. */
void itfMesMngInitializeManager(void) {
    mesflow_tag *poolNode;

    itfMesMngManager.activeWindowCount = 0;
    itfMesMngManager.drawTickCount = 0;
    itfMesMngManager.flags = 0;
    itfMesMngManager.windowTexture = itfMiscLoadTexture("/itf/MESWIN.TMX");
    itfMiscLinkInitialize(&itfMesMngManager.pool, itfMesMngManager.nodes, ITF_MES_WINDOW_CAPACITY, ITF_MES_POOL_NODE_BYTES);
    for (poolNode = itfMesMngManager.pool.firstFree; poolNode != NULL; poolNode = poolNode->next) {
        poolNode->state = NULL;
    }
    dds3StartProcess("CalcMsgMng", 0x409, 0, 0, CalcMsgMng_Process, DrawMsgMng_Shutdown, 0);
    dds3StartProcess("DrawMsgMng", 0x2B1A, 0, 0, DrawMsgMng_Process, DrawMsgMng_Shutdown, 0);
}

/* Set manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngSetBitStatManager(u32 mask) {
    itfMesMngManager.flags |= mask;
}

/* Clear manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngClearBitStatManager(u32 mask) {
    itfMesMngManager.flags &= ~mask;
}

/* Read the manager's 16-bit flags, distinct from itfMessageFlags. */
u16 itfMesMngGetStatManager(void) {
    return itfMesMngManager.flags;
}



extern s8 dds3PadButtonState[];
extern char *D_00357FA0[];
extern char *D_003BB200[2];
extern char D_003BB208[];
extern char D_003BB210[];
extern char D_003BB218[];
extern char D_003BB220[];
extern char D_003BB228[];
extern SdfPoolNode kwlnPositionedTextSurface;
extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);
extern ItfMesWindowRec *DebugChangeInstance(ItfMesWindowRec *window);
extern void DebugMoveWinVert(ItfMesBlkA4 *panel, s32 selectedItem);

/* Run and draw the interactive message-layout inspector. */
INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1480);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1490);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14A0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14B0);

s32 itfMesMngDebugProc(void) {
    ItfMesBlkA4 *panel;
    itfpanel_t *panelSprite;
    s32 *bounds;
    sdfDrawTag_t *packetList;
    s32 menuItemIndex;
    s32 menuItemY;
    const char *menuItemFormat;
    const char *selectionMarker;
    const char *coordinateFormat;

    if (D_00357D80.window == NULL) {
        ItfMesWindowRec *window = DebugChangeInstance(NULL);

        D_00357D80.window = window;
        if (window == NULL) {
            return 0;
        }
    }
    {
        ItfMesDebugState *debug = &D_00357D80;

        panel = &debug->window->state->blkA4;
        switch (debug->mode) {
        case 0:
            if (dds3PadButtonState[0x36] & 2) {
                if (--debug->selectedItem < 0) {
                    debug->selectedItem = 2;
                }
            } else if (dds3PadButtonState[0x37] & 2) {
                if (++debug->selectedItem >= 5) {
                    debug->selectedItem = 0;
                }
            }
            if (dds3PadButtonState[0x31] < 0) {
                ItfMesDebugState *confirmDebug = &D_00357D80;

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
                    confirmDebug->window = DebugChangeInstance(confirmDebug->window);
                    break;
                }
            } else if (dds3PadButtonState[0x33] < 0) {
                return -1;
            }
            break;
        case 1:
            DebugMoveWinVert(panel, debug->selectedItem);
            if (dds3PadButtonState[0x33] < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            DebugMoveWinAlpha(panel);
            if (dds3PadButtonState[0x33] < 0) {
                D_00357D80.mode = 0;
            }
            break;
        }
    }

    packetList = dds3FontReqAllocDrawTag();
    dds3ConsPanelTagLink(packetList, 0x10, 0x10, 0x1E, 9);
    menuItemFormat = D_003BB218;
    menuItemY = 0x7A00;
    for (menuItemIndex = 0; menuItemIndex < 5; menuItemIndex++, menuItemY += 0x60) {
        if (D_00357D80.selectedItem == menuItemIndex) {
            selectionMarker = D_003BB200[D_00357D80.mode != 0];
        } else {
            selectionMarker = D_003BB220;
        }
        sdfDrawTagLink(packetList,
                        (u32)sdfDevPrintf(0x7180, menuItemY, 0xFFFFF0, 0,
                                                     menuItemFormat, selectionMarker, D_00357FA0[menuItemIndex]));
    }

    coordinateFormat = "( %3d,%3d )";
    panelSprite = panel->sprite;
    bounds = &panelSprite->left;
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7E00, 0x7A00, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[0] >> 4,
                                                 bounds[1] >> 3));
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7E00, 0x7A60, 0xFFFFF0, 0,
                                                 coordinateFormat, bounds[2] >> 4,
                                                 bounds[3] >> 3));
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7E00, 0x7B20, 0xFFFFF0, 0,
                                                 D_003BB228, panel->fadeLimit));
    bounds = D_00357D80.window->state->blkA4.bounds;
    sdfDrawTagLink(packetList,
                    (u32)sdfDevPrintf(0x7180, 0x7C40, 0xFFFFF0, 0,
                                                 "OFFSET : %3d,%3d - %3d,%3d",
                                                 bounds[0] >> 4,
                                                 bounds[1] >> 3,
                                                 bounds[2] >> 4,
                                                 bounds[3] >> 3));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, packetList);
    return 0;
}

/* Reset the three otherwise unnamed message-state words. */
void itfMesMngDebugShutdown(void) {
    D_00357D80.selectedItem = 0;
    D_00357D80.mode = 0;
    D_00357D80.window = NULL;
}

/* Slot record reached through the pool base, one 0x14 bytes per window. */
extern ItfMesWindowRec D_003D6EC0[];

/* Tear down installed content/resources, return the pool record and decrement
 * the pool-array-relative counter. An empty record is a no-op. */
void DestructFlow(ItfMesWindowIndex window) {
    ItfMesWindowRec *windowRecord = &D_003D6EC0[window];
    instanceMes_tag *mes = windowRecord->state;

    if (mes != NULL) {
        itfMesMngRequestMessageRelease(window, ITF_MES_RELEASE_ENTRY_AND_PRIMARY_TEXT);
        itfMesMngRequestSelectRelease(window);
        btlReleaseEffectResourceHandles(mes);
        ReleaseAllVariable(&mes->textSlots);
        mes->flags = 0;
        sdfMemFreeDirect(windowRecord->resourceHandle);
        windowRecord->state = NULL;
        itfMiscLinkGive(windowRecord, (ItfMesPool *)((u8 *)D_003D6EC0 - 0x10));
        ((ItfMesGlobals *)((u8 *)D_003D6EC0 - 0x20))->activeWindowCount -= 1;
    }
}

/* Relocate packed payload words once, then set the resource's relocated marker. */
/* Semantic reference: Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c). */
void ResolveResourceAddress(ItfMesSub *resource) {
    if (resource->relocated == 0) {
        sdfAddressResolver((s32 *)resource->entries, (u32)resource->entries,
                      (u8 *)resource + resource->fixupTableOffset, resource->fixupTableBytes);
        resource->relocated = 1;
    }
}

/* Recognize MSG0/MSG1 magic only; this is not complete format validation. */
u32 CheckMessageBinary(ItfMesSub *resource) {
    u32 isMessageResource;

    isMessageResource = 0;
    if (resource->magic == ITF_MES_MAGIC_MSG0 || resource->magic == ITF_MES_MAGIC_MSG1) { /* "MSG0"/"MSG1" */
        isMessageResource = 1;
    }
    return isMessageResource;
}

/* Address an installed resource entry; caller supplies an in-range index. */
ItfMesEntry *GetTypeHeader(instanceMes_tag *mes, ItfMesEntryIndex entryIndex) {
    ItfMesEntry *entries = mes->sub->entries;

    return &entries[entryIndex];
}

/* Return the trailing entry at entryCount; do not advance or validate the count. */
ItfMesEntry *GetTypeHeader2(ItfMesSub *sub) {
    ItfMesEntry *entries = sub->entries;

    return &entries[sub->entryCount];
}

/* Return the entry's item-address table or branch sentinel 1; index unchecked. */
ItfMesEntryItemsResult itfMesGetWindowEntryItems(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex) {
    return GetTypeHeader(pinst[window].mes, entryIndex)->itemList;
}

/* Read the installed sub-resource's entry count without slot/resource validation. */
u32 itfMesGetEntryCount(ItfMesWindowIndex window) {
    return pinst[window].mes->sub->entryCount;
}

/* Return relocated encoded text, or NULL for a negative/past-end index. */
u8 *GetMessagePagePtr(ItfMesTable *table, s32 itemIndex) {
    s32 itemCount = table->count;

    if (itemIndex < 0 || itemIndex >= itemCount) {
        return 0;
    }
    return (u8 *)table->items[itemIndex];
}

/* Rebuild colored entry glyphs, synchronize interface flags/hooks and cache
 * the span count. Existing span helpers require a valid created chain. */
void itfMesBuildEntryGlyph(instanceMes_tag *mes) {
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;
    _FRQ *glyph;
    _FRQ *previousGlyph;
    u32 lipsValue;
    u32 interfaceMask;
    void (*callback)(void);

    previousGlyph = entryBlock->glyphChain;
    if (previousGlyph != NULL) {
        frDestroyFontRequestAfter(previousGlyph);
        entryBlock->glyphChain = NULL;
    }
    glyph = MessageRequestEx((s32)mes->entryBlock.x, (s32)entryBlock->y, entryBlock->color[0], entryBlock->color[1], entryBlock->color[2], entryBlock->color[3],
                          GetMessagePagePtr(entryBlock->table, entryBlock->itemIndex), 0);
    if (mes->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
        if (frFontCountGlyphChainNodes(glyph) == 1) {
            frFontCenterGlyphChainAt(0x1000, 0xC60, glyph);
        } else {
            frFontCenterGlyphChainAt(0x1000, 0xBF8, glyph);
        }
    }
    if (!(mes->flags & ITF_MES_SKIP_CONTEXT_ENABLE_BIT) && (itfMesMngManager.flags & 1)) {
        itfMesEnableUnflaggedNodeContexts(glyph);
    }
    GetLastMessagePara(glyph, entryBlock);
    interfaceMask = frGetMessageResult(3);
    entryBlock->textInterfaceMask = interfaceMask;
    /* Copy interface-mask bit 0x2 into window flag 0x10000. */
    if (interfaceMask & 2) {
        mes->flags |= 0x10000;
    } else {
        mes->flags &= 0xFFFEFFFF;
    }
    if (frGetMessageResult(4) != 0) {
        callback = mes->entryGlyphCallback;
        if (callback != NULL) {
            callback();
        }
    }
    if (frGetMessageResult(8) != 0) {
        lipsValue = itfGetTextLipsUnitId();
        evtLipsExecFunction((s32)lipsValue, (s32)itfGetTextLipsMotionId());
    }
    ChangeZBuffer(glyph, mes->renderValue);
    entryBlock->lineCount = CountLineMessage(GetLastLinkFontHandle(glyph), glyph);
    entryBlock->glyphChain = glyph;
}

/* Replace a retained text-slot allocation. Nonpositive byteCount copies the
 * padded string span; positive counts copy that many bytes into a zeroed block.
 * DDS1 string mode intentionally has no memset. Keep the K&R declaration form. */
void SetVar(mes, slotIndex, source, byteCount)
    instanceMes_tag *mes;
    s32 slotIndex;
    const char *source;
    s32 byteCount;
{
    ItfMesTextSlots *slots = &mes->textSlots;
    u8 **textAddress = &slots->addresses[slotIndex];
    s32 allocationBytes;

    if (*textAddress != 0) {
        sdfMemFreeDirect(slots->handles[slotIndex]);
        *textAddress = 0;
    }
    if (byteCount <= 0) {
        allocationBytes = (strlen(source) + ITF_MES_STRING_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
        slots->handles[slotIndex] = sdfMemAllocDirect(allocationBytes);
        *textAddress = (u8 *)sdfMemLock(slots->handles[slotIndex]);
        /* String mode copies the padded span, rather than only strlen + 1. */
        memcpy(*textAddress, source, allocationBytes);
        return;
    }
    allocationBytes = (byteCount + ITF_MES_BINARY_COPY_ROUND_BIAS) & ~ITF_MES_COPY_ALIGN_MASK;
    slots->handles[slotIndex] = sdfMemAllocDirect(allocationBytes);
    *textAddress = (u8 *)sdfMemLock(slots->handles[slotIndex]);
    memset(*textAddress, 0, allocationBytes);
    memcpy(*textAddress, source, byteCount);
}

/* Install all replacement-text addresses into the font's fixed slot table. */
void CommitVarTable(u8 **table) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < ITF_MES_TEXT_SLOT_COUNT; slotIndex++) {
        frSetVariableRequest(slotIndex, table[slotIndex]);
    }
}

/* Read the selected item from the window's trailing entry, without validation. */
const u8 *GetSpeakerName(instanceMes_tag *mes) {
    u32 *items;

    items = (u32 *)GetTypeHeader2(mes->sub)->itemList;
    return (const u8 *)items[mes->blk14.selectedIndex];
}

/* Count clear low bits by consuming one bit per iteration. Nonpositive counts
 * return zero; counts beyond the mask width also count shifted-in zero bits. */
s32 GetNumSelectItem(s32 bitCount, u32 remainingMask) {
    s32 zeroCount;
    u32 bitValue;

    zeroCount = 0;
    while (bitCount > 0) {
        bitValue = remainingMask & 1;
        remainingMask >>= 1;
        bitCount--;
        if (bitValue == 0) {
            zeroCount++;
        }
    }
    return zeroCount;
}

/* Append text for each clear mask bit. Masked items are still consumed, while
 * y advances only for built text. Return NULL if nothing was built. */
_FRQ *itfMesBuildNodeRows(const u32 *items, s32 itemCount, u32 mask, s32 x, s32 y, s32 renderValue) {
    _FRQ *glyphChain = NULL;
    s32 itemIndex;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++, items++) {
        if (mask & 1) {
            mask >>= 1;
        } else {
            s16 rowHeightUnits;

            glyphChain = MessageRequestEx(x, y, 0, 0, 0, ITF_MES_DEFAULT_COLOR_VALUE, (u8 *)*items, glyphChain);
            mask >>= 1;
            rowHeightUnits = glyphChain->parentDimensionsOrRenderWord.parentDimensions.cellHeight;
            y += rowHeightUnits << ITF_MES_TEXT_Y_SHIFT;
        }
    }
    if (glyphChain != NULL) {
        ChangeZBuffer(glyphChain, renderValue);
        ChangeFontAlpha(glyphChain, ITF_MES_DEFAULT_COLOR_VALUE);
    }
    return glyphChain;
}

/* Discard (to - from - 1) preceding rows when positive, retain the next
 * complete y-group and queue everything after it. Requires non-NULL input;
 * return NULL if discarding preceding rows exhausts the chain. */
_FRQ *UnchainFontHandleUnit(_FRQ *glyph, s32 from, s32 to) {
    s32 rowsToDiscard = to - from - 1;
    s32 rowY = glyph->y;
    _FRQ *nextNode;
    _FRQ *rowHead;
    _FRQ *rowTail;

    while (rowsToDiscard > 0) {
        while (rowY == glyph->y) {
            nextNode = glyph->previous;
            glyph->next = NULL;
            glyph->chainHead = glyph;
            glyph->previous = NULL;
            frDestroyFontRequestAfter(glyph);
            glyph = nextNode;
            if (glyph == NULL) {
                return NULL;
            }
        }
        rowsToDiscard--;
        rowY = glyph->y;
    }
    rowHead = glyph;
    do {
        rowTail = glyph;
        glyph = glyph->previous;
    } while (glyph != NULL && rowY == glyph->y);
    while (glyph != NULL) {
        nextNode = glyph->previous;
        glyph->next = NULL;
        glyph->chainHead = glyph;
        glyph->previous = NULL;
        frDestroyFontRequestAfter(glyph);
        glyph = nextNode;
    }
    rowHead->next = NULL;
    rowTail->previous = NULL;
    for (glyph = rowHead; glyph != NULL; glyph = glyph->previous) {
        /* Despite its current name, this link points to the retained row tail. */
        glyph->chainHead = rowTail;
    }
    return rowHead;
}

/* Return the final previous-linked glyph; input must be non-NULL. */
_FRQ *GetLastLinkFontHandle(_FRQ *glyph) {
    while (glyph->previous != NULL) {
        glyph = glyph->previous;
    }
    return glyph;
}

/* Copy the auxiliary glyph's shade bytes; its encoded intensity is unsigned. */
void GetLastMessagePara(_FRQ *glyph, ItfMesEntryBlock *entryBlock) {
    _TEXT *shade = glyph->lastChild;
    u8 encodedIntensity = glyph->glyphCodeOrContext.byteRoles.encodedContextByte;

    entryBlock->color[3] = encodedIntensity >> 1;
    entryBlock->color[0] = shade->renderValueOrSetupOrShade.shadeColor.red;
    entryBlock->color[1] = shade->renderValueOrSetupOrShade.shadeColor.green;
    entryBlock->color[2] = shade->renderValueOrSetupOrShade.shadeColor.blue;
}

/* Inclusive step count from the y delta in eighths and the first row's height.
 * Both pointers and a nonzero first-row height are required. */
s32 CountLineMessage(_FRQ *last, _FRQ *first) {
    s16 rowHeightUnits = first->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

    return ((first->y - last->y) >> ITF_MES_TEXT_Y_SHIFT) / rowHeightUnits + 1;
}

/* Translate every linked node in text units; a NULL chain is a no-op. */
void ChangeFontPosition(_FRQ *glyph, s32 dx, s32 dy) {
    if (glyph == NULL) {
        return;
    }
    do {
        glyph->x += dx;
        glyph->y += dy;
        glyph = glyph->previous;
    } while (glyph != NULL);
}

/* Assign the render value across the complete chain; a NULL chain is a no-op. */
void ChangeZBuffer(_FRQ *glyph, s32 renderValue) {
    while (glyph != NULL) {
        glyph->renderValueOrSetupOrShade.renderValue = renderValue;
        glyph = glyph->previous;
    }
}

/* Skip (rowCount - rowIndex - 1) preceding y-groups when positive, then set each
 * child's flag byte in the next row. Requires non-NULL input; past-end is a no-op. */
void ChangeFontColor(_FRQ *glyph, s32 rowIndex, s32 rowCount, s32 requestedFlagValue) {
    s32 rowsToSkip = rowCount - rowIndex - 1;
    s32 rowY = glyph->y;
    s32 currentY = rowY;
    _TEXT *child;
    u8 flagValue;

    while (rowsToSkip > 0) {
        while (rowY == currentY) {
            glyph = glyph->previous;
            if (glyph == NULL) {
                return;
            }
            currentY = glyph->y;
        }
        rowsToSkip--;
        rowY = currentY;
    }
    flagValue = requestedFlagValue;
    do {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->renderValueOrSetupOrShade.setupBytes.firstOption = flagValue;
        }
        glyph = glyph->previous;
    } while (glyph != NULL && rowY == glyph->y);
}

/* Set every child's flag byte across the parent chain; NULL is a no-op. */
void ChangeFontColorAll(_FRQ *glyph, u8 flagValue) {
    _TEXT *child;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->renderValueOrSetupOrShade.setupBytes.firstOption = flagValue;
        }
    }
}

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void ChangeFontAlpha(_FRQ *glyph, u32 color) {
    _TEXT *child;

    for (; glyph != NULL; glyph = glyph->previous) {
        for (child = glyph->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = child->parentDimensionsOrRenderWord.renderWord & ITF_MES_COLOR_BYTE_CLEAR_MASK | color;
        }
    }
}

/* Find the maximum summed advance of contiguous equal-y groups, scaled by 16.
 * NULL returns zero; the initial zero also excludes negative group totals. */
s32 GetMessageWidth(_FRQ *glyph) {
    s32 maxAdvance = 0;

    while (glyph != NULL) {
        s32 rowY = glyph->y;
        s32 rowAdvance = 0;

        do {
            rowAdvance += glyph->advance;
            glyph = glyph->previous;
        } while (glyph != NULL && rowY == glyph->y);
        if (rowAdvance > maxAdvance) {
            maxAdvance = rowAdvance;
        }
    }
    return maxAdvance << ITF_MES_TEXT_X_SHIFT;
}

/* Resolve the zero-based visible-option ordinal to its clear bit, or 32 if
 * absent. Keep the predecrement; the ordinal itself is not validated. */
ItfMesOptionBitIndex itfMesFindOptionBitIndex(ItfMesVisibleOptionOrdinal optionOrdinal, u32 optionMask) {
    s32 bitIndex = 0;
    while (bitIndex < ITF_MES_MASK_BIT_COUNT) {
        if ((optionMask & 1) == 0) {
            if (--optionOrdinal < 0) {
                break;
            }
        }
        bitIndex++;
        optionMask >>= 1;
    }
    return bitIndex;
}

/* Enable parent contexts whose first child's tested flag is zero.
 * NULL chain is allowed, but every visited node must have a first child. */
void itfMesEnableUnflaggedNodeContexts(_FRQ *glyph) {
    for (; glyph != NULL; glyph = glyph->previous) {
        if (glyph->firstChild->renderValueOrSetupOrShade.setupBytes.secondOption == 0) {
            frFontEnableContextMode(glyph);
        }
    }
}

INCLUDE_SDATA(const s32, "interface/itfMesManager", itfMessageFlags);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB1F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB1F8);

char *D_003BB200[2] __attribute__((section(".sdata"))) = {D_003BB210, D_003BB208};

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB208);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB210);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB218);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB220);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB228);
