#include "mnu.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_parameter_index.h"
#include "scr_integer_return_value_api.h"
#include "scr_command_timer_api.h"
#include "sdf_packet_list.h"
#include "itf.h"
#include "itf_mes_window.h"
#include "itf_mes_panel_mode.h"
#include "itf_mes_entry_start_api.h"
#include "sdf.h"
#include "itf_panel_draw.h"
#include "itf_text_vector.h"
#include "itf_panel_api.h"
#include "itf_panel_layouts.h"
#include "itf_held_panel_message_step.h"
#include "sdk_ps2.h"
#include "common.h"
#include "itf_mes_voice_cue_result.h"
#include "itf_mes_select_def_key_result.h"
#include "itf_message_manager_task.h"
#include "itf_option_bit_index_api.h"
#include "sdf_heap_api.h"
#include "sdf_pac_reloc.h"
#include "fr_font_temporary_entry_api.h"
#include "fr_font_measure.h"
#include "itf_mes_state_api.h"
#include "itf_text_slots_api.h"
#include "fr_font_glyph_chain_count.h"
#include "kernel.h"
#include "dds3_task_create.h"
#include "itf_debug_panel_work.h"
#include "itf_mes_selection_commit_result.h"
#include "snd_sequence_volume_pan.h"
#include "snd_pad.h"
#include "ps2asm.h"
#include "scr.h"
#include "scr.h"
#include "sdf_sif_command.h"
#include "sdf_projection.h"

/* Declared before first use (were implicit int). */
extern void itfMesBuildEntryGlyph(instanceMes_tag *mes);
extern void CommitVarTable(u8 **table);
#include "sdf_debug_api.h"
extern ItfMesGlobals itfMesMngManager;

extern void itfMesShiftPanelVertically(instanceMes_tag *panel, s32 dy);
extern void ChangeFontColorAll(_FRQ *node, u8 flagValue);
extern ItfMesOptionBitIndex itfMesFindOptionBitIndex(ItfMesVisibleOptionOrdinal optionOrdinal, u32 optionMask);
extern ItfMesSelectionCommitResult SelectItem(instanceMes_tag *);
extern void itfUpdateSoundSelectionTransition(instanceMes_tag *);

extern SndPad dds3PadButtonState;

extern sdfGrOt_t sdfGrOtTable[];

extern ItfMesDebugState D_00357D80;

extern void itfPnlDraw(itfpanel_t *sprite, sdfGrOt_t *surface);

extern void ChangeFontPosition(_FRQ *node, s32 dx, s32 dy);

extern void DebugDrawWindowLine(itfpanel_t *sprite);

extern s32 SelectItemDefKey(ItfMesBlk40 *seq);

extern void ChangeFontColor(_FRQ *glyphChain, s32 rowIndex, s32 rowCount, s32 requestedFlagValue);

#define ITF_PANEL_MESSAGE_FADE_TICKS 30
#define ITF_PANEL_MESSAGE_EXIT_TICKS 10
extern ItfMesWindowIndex scrGetMessageHandle(void);
extern void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);
extern void itfMesMngRequestSelectRelease(s32 window);
extern void itfMesMngRequestMessageRelease(s32 window, s32 releasePrimaryBlock);
#include "sdf_packet_alloc.h"
extern PanelHold itfHeldPanelCursor;
#include "sdf_packet_alloc.h"

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

/* Poll/start an entry: one ends the command, zero keeps waiting.
   A negative signed status is consumed by clearing it; positive status keeps waiting. */
s32 itfMesMngMessage(void) {
    ItfMesWindowIndex window = scrGetMessageHandle();
    instanceMes_tag *record;
    ItfMesEntryBlock *entryState;
    s32 entryIndex;
    s8 status;

    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entryIndex = scrGetIntPara(0);
    entryState = &record->entryBlock;
    status = entryState->textState;
    if (status == 0) {
        if (itfMesStartEntry(window, entryIndex, 0) == ITF_MES_ENTRY_START_EMPTY_TABLE) {
            return 1;
        }
    } else if (status < 0) {
        entryState->textState = 0;
        return 1;
    }
    return 0;
}

/* Forward this record's layout sprite address and the supplied value; index is unchecked. */
void itfMesMngSetOffsetY1(s32 windowIndex, s32 topDelta) {
    itfPnlSetPositionOffset(pinst[windowIndex].mes->blkA4.sprite, 0, topDelta, 0, 0, 0);
}

/* Return the signed byte entry status; this is separate from the option-block s16 status. */
s8 itfMesMngGetPhase(s32 windowIndex) {
    return pinst[windowIndex].mes->entryBlock.textState;
}

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 windowIndex, s8 status) {
    pinst[windowIndex].mes->entryBlock.textState = status;
}

/* Capture the script window, reset its held queue, and switch to page three.
   Invalid windows still return the native command-complete value of one. */
s32 itfPanelAcquireHold(void) {
    PanelHold *hold;
    ItfMesWindowIndex window;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    hold = &itfHeldPanelCursor;
    hold->window = window;
    hold->unk4 = -1;
    hold->entryEndFrame = -1;
    hold->queuedCount = 0;
    itfMesMngChangeWindowType(window, ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION, 0);
    return 1;
}

/* Clean up the active held window and reset its queue; an inactive hold is a no-op. */
s32 itfPanelReleaseHold(void) {
    PanelHold *hold = &itfHeldPanelCursor;

    if (hold->window < 0) {
        return 1;
    }
    itfMesMngRequestMessageRelease(hold->window, 1);
    hold->window = -1;
    hold->unk4 = -1;
    hold->entryEndFrame = -1;
    hold->queuedCount = 0;
    return 1;
}

/* Queue script words in native second/first/third field order; first and third narrow to s16.
   Inactive/full queues are ignored with status one; negative queuedCount is not checked. */
s32 itfCommandQueueHeldPanelEntry(void) {
    PanelHold *cursor = &itfHeldPanelCursor;
    s32 first;
    s32 second;
    s32 third;

    if (cursor->window < 0) {
        return 1;
    }
    if (cursor->queuedCount >= ITF_PANEL_HOLD_CAPACITY) {
        return 1;
    }
    first = scrGetIntPara(0);
    second = scrGetIntPara(1);
    third = scrGetIntPara(2);
    cursor->items[cursor->queuedCount].second = first;
    cursor->items[cursor->queuedCount].first = second;
    cursor->items[cursor->queuedCount].third = third;
    cursor->queuedCount++;
    return 1;
}

ItfHeldPanelMessageStepResult itfStepHeldPanelMessages(void) {
    s32 frame;
    s32 latest = -1;
    s32 i;

    if (itfHeldPanelCursor.window < 0) {
        return ITF_HELD_PANEL_MESSAGES_COMPLETE;
    }
    frame = scrGetCommandTimer();
    if (itfHeldPanelCursor.queuedCount == 0) {
        return ITF_HELD_PANEL_MESSAGES_COMPLETE;
    }
    for (i = 0; i < itfHeldPanelCursor.queuedCount; i++) {
        s32 start = itfHeldPanelCursor.items[i].first;

        if (start >= latest) {
            latest = start;
        }
        if (start == frame) {
            s16 entry = itfHeldPanelCursor.items[i].second;
            s32 deadline = frame + itfHeldPanelCursor.items[i].third;

            itfMesStartEntry(itfHeldPanelCursor.window, entry, 0);
            itfHeldPanelCursor.unk4 = entry;
            itfHeldPanelCursor.entryEndFrame = deadline;
        }
    }
    if (frame == itfHeldPanelCursor.entryEndFrame) {
        itfMesMngRequestMessageRelease(itfHeldPanelCursor.window, 1);
        itfHeldPanelCursor.unk4 = itfHeldPanelCursor.entryEndFrame = -1;
    }
    if (frame >= latest && frame >= itfHeldPanelCursor.entryEndFrame) {
        itfPanelReleaseHold();
        return ITF_HELD_PANEL_MESSAGES_COMPLETE;
    }
    return ITF_HELD_PANEL_MESSAGES_PENDING;
}

s32 itfMesMngSelect(void) {
    ItfMesWindowIndex window;
    s32 entry;
    instanceMes_tag *record;
    ItfMesBlk40 *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    record = pinst[window].mes;
    entry = scrGetIntPara(0);
    options = &record->blk40;
    if (options->selectionState == 0) {
        itfMesBuildOptionList(window, entry);
    } else if (options->selectionState < 0) {
        options->selectionState = 0;
        scrSetIntReturnValue(options->selectedIndex);
        itfMesMngRequestSelectRelease(window);
        return 1;
    }
    return 0;
}

extern void dds3SetFilter2Para(u32);
extern void dds3SetFilter2RGBA(u32);
extern void dds3Filter2On(s32);
extern void dds3Filter2Off(s32);
extern s32 D_003BD820;

/* Configure the fade on timer zero, wait thirty ticks, then consume a negative option status.
   Keep both timer reads: setup tests a second read, while later comparisons use the cached one. */
s32 itfCommandSelectOptionWithFade(void) {
    ItfMesWindowIndex window;
    s32 elapsedTicks;
    s32 entryIndex;
    instanceMes_tag *record;
    ItfMesBlk40 *options;

    window = scrGetMessageHandle();
    if (window < 0) {
        return 1;
    }
    elapsedTicks = scrGetCommandTimer();
    if (scrGetCommandTimer() == 0) {
        dds3SetFilter2Para(0x44);
        dds3SetFilter2RGBA(0x5E1C1C1C);
        dds3Filter2On(ITF_PANEL_MESSAGE_FADE_TICKS);
        D_003BD820 = -1;
        return 0;
    }
    if (elapsedTicks < ITF_PANEL_MESSAGE_FADE_TICKS) {
        return 0;
    }
    if (elapsedTicks == ITF_PANEL_MESSAGE_FADE_TICKS) {
        itfMesMngChangeWindowType(window, ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION, 0);
    } else if (D_003BD820 == -1) {
        record = pinst[window].mes;
        entryIndex = scrGetIntPara(0);
        options = &record->blk40;
        if (options->selectionState == 0) {
            itfMesBuildOptionList(window, entryIndex);
        } else if (options->selectionState < 0) {
            options->selectionState = 0;
            scrSetIntReturnValue(options->selectedIndex);
            itfMesMngRequestSelectRelease(window);
            dds3Filter2Off(ITF_PANEL_MESSAGE_EXIT_TICKS);
            D_003BD820 = 0;
            return 1;
        }
    }
    return 0;
}

/* Return the option-block signed status word, not the separate entry status byte;
 * its callers read the sign-extended halfword as a word. */
s32 itfMesMngGetSelPhase(s32 windowIndex) {
    return pinst[windowIndex].mes->blk40.selectionState;
}

/* Store the option-block signed status word without validating the slot. */
void itfMesMngSetSelPhase(s32 windowIndex, s16 status) {
    pinst[windowIndex].mes->blk40.selectionState = status;
}

/* Return the same signed word forwarded as the script result when an option completes;
 * its callers read the sign-extended halfword as a word. */
s32 itfMesMngGetSelectNo(s32 windowIndex) {
    return pinst[windowIndex].mes->blk40.selectedIndex;
}

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
ItfMesSelectDefKeyResult itfMesMngSetSelectDefKey(ItfMesWindowIndex window,
    s32 id, s32 value) {
    ItfMesBlk40 *optionBlock = &pinst[window].mes->blk40;
    ItfMesOption *option = NULL;
    s32 optionIndex;

    if ((u32)id >= 0x10) {
        return ITF_MES_SELECT_DEF_KEY_REJECTED;
    }
    if (id == 1 || id == 6 || id == 7) {
        return ITF_MES_SELECT_DEF_KEY_REJECTED;
    }
    for (optionIndex = 0; optionIndex < optionBlock->optionCount; optionIndex++) {
        if (optionBlock->options[optionIndex].id == id) {
            option = &optionBlock->options[optionIndex];
            break;
        }
    }
    if (option == NULL) {
        if (optionBlock->optionCount >= 15) {
            return ITF_MES_SELECT_DEF_KEY_REJECTED;
        }
        option = &optionBlock->options[optionBlock->optionCount];
        optionBlock->optionCount++;
    }
    option->id = id;
    option->value = value;
    return ITF_MES_SELECT_DEF_KEY_SET;
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

extern char *D_00357FA0[];
extern char *D_003BB200[2];
extern char D_003BB208[];
extern char D_003BB210[];
extern char D_003BB218[];
extern char D_003BB220[];
extern char D_003BB228[];
extern sdfGrOt_t sdfDebugGrOt;
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
            if (dds3PadButtonState.secondaryUp & 2) {
                if (--debug->selectedItem < 0) {
                    debug->selectedItem = 2;
                }
            } else if (dds3PadButtonState.secondaryDown & 2) {
                if (++debug->selectedItem >= 5) {
                    debug->selectedItem = 0;
                }
            }
            if (dds3PadButtonState.secondaryCross < 0) {
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
            } else if (dds3PadButtonState.secondaryCancel < 0) {
                return -1;
            }
            break;
        case 1:
            DebugMoveWinVert(panel, debug->selectedItem);
            if (dds3PadButtonState.secondaryCancel < 0) {
                debug->mode = 0;
            }
            break;
        case 2:
            DebugMoveWinAlpha(panel);
            if (dds3PadButtonState.secondaryCancel < 0) {
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
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packetList);
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

void frFontEnableNodeContextModes(_FRQ *node) {
    for (; node != NULL; node = node->previous) {
        frFontEnableContextMode(node);
    }
}

/* Initialize panel placement and retain its screen-space Y coordinate. */
void itfMesInitializePanelPlacementSprite(instanceMes_tag *panel) {
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    s32 spriteTop;
    if (place->sprite == 0) {
        place->sprite = itfCreateUiSpriteByKind(6, itfMesMngManager.windowTexture);
        if (place->frame != 0) {
            place->sprite->textRightX = panel->blk14.glyphChain->x + frFontMeasureGlyphStripWidth(0, panel->blk14.glyphChain);
        }
    }
    spriteTop = pos->y + place->bounds[1];
    itfPnlSetPosition(place->sprite, pos->x + place->bounds[0], spriteTop, pos->x + place->bounds[2], pos->y + place->bounds[3], panel->renderValue);
    place->sprite->screenY = spriteTop;
    itfPnlSetColor(place->sprite, place->red, place->green, place->blue, 0);
    panel->flags = (panel->flags & ~0x300) | 0x100;
}

void OpenCheckSpeakerWindow(instanceMes_tag *panel) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesBlkA4 *place = &panel->blkA4;
    if (origin->glyphChain != NULL && !(panel->flags & 0x10000)) {
        if (place->frame == NULL) {
            s32 width = origin->glyphChain->advance * 16;
            place->frame = itfCreateUiSpriteByKind(7, itfMesMngManager.windowTexture);
            itfPnlSetPosition(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0xF0, panel->renderValue);
            itfPnlSetColor(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        panel->flags = (panel->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        panel->flags |= 0x3000;
    }
}

void InitializeSpeaker(ItfMesBlk14 *cursor, s32 resetPosition) {
    if (resetPosition != 0) {
        cursor->x = 0x280;
        cursor->y = 0xa10;
    }
    cursor->glyphChain = 0;
    cursor->selectedIndex = 0xffff;
}

void InitializeMessage(ItfMesEntryBlock *cur, s32 resetPos) {
    if (resetPos != 0) {
        cur->x = 0x4B0;
        cur->y = 0xAF8;
    }
    cur->glyphChain = NULL;
    cur->textState = 0;
    cur->textInterfaceMask = 0;
    cur->lineCount = 0;
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
    cursor->y = 0xC48;
    cursor->glyphChain = NULL;
    cursor->panelValue = 0;
    cursor->selectionState = ITF_SOUND_SELECTION_STATE_INACTIVE;
    cursor->selectedIndex = -1;
    cursor->savedIndex = -1;
    cursor->rowCount = 0;
    cursor->widestRowX = 0;
    cursor->widestRowWidth = 0;
    cursor->glyphColorIntensity = 0;
    cursor->optionCount = 0;
}

void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *block, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

void itfResetWindowResourceBlock(ItfMesBlkA4 *block) {
    block->frame = NULL;
    block->sprite = NULL;
    block->overlay = NULL;
    itfMesConfigurePanelBoundsAndFadeLimit(block, ITF_MES_PANEL_MODE_STANDARD, 0);
}

void InitializeVariable(ItfMesTextSlots *slots) {
    s32 remaining;
    u8 **cursor;

    cursor = &slots->addresses[31];
    remaining = 0x1f;
    do {
        remaining = remaining - 1;
        *cursor = 0;
        cursor = cursor + -1;
    } while (-1 < remaining);
}

void InitializeCurCtr(BtlFade *fade, s32 preserveKind) {
    if (preserveKind == 0) {
        fade->kind = 0;
    }
    fade->phase = BTL_FADE_PHASE_ALPHA_RISING;
    fade->timer = 0;
    fade->alpha = 0x40;
    fade->textAdvanceFrameCount = 0;
}

void ForceSetCurCtr(BtlFade *fade, BtlFadePhase phase, s16 alpha, s16 timer) {
    fade->phase = phase;
    fade->alpha = alpha;
    fade->timer = timer;
}

void btlReleaseEffectResourceHandles(instanceMes_tag *effect) {
    ItfMesBlkA4 *place = &effect->blkA4;
    if (place->frame != NULL) {
        itfPnlRelease(place->frame);
        place->frame = NULL;
    }
    if (place->sprite != NULL) {
        itfPnlRelease(place->sprite);
        place->sprite = NULL;
    }
    if (place->overlay != NULL) {
        itfPnlRelease(place->overlay);
        place->overlay = NULL;
    }
    effect->flags &= ~0xF00;
}

void ReleaseAllVariable(ItfMesTextSlots *slots) {
    s32 remaining;
    s32 index;

    remaining = 0x1f;
    index = 0;
    do {
        if (slots->addresses[index] != 0) {
            sdfMemFreeDirect(slots->handles[index]);
            slots->addresses[index] = 0;
        }
        remaining = remaining - 1;
        index++;
    } while (-1 < remaining);
}

u16 *txtFormatNumberU16(s32 number, u16 *out) {
    s32 digits[10];
    s32 count = 0;
    s32 i;
    do {
        digits[count] = number % 10;
        number = number / 10;
        count++;
    } while (number > 0 && count < 10);
    for (i = count - 1; i >= 0; i--) {
        *out++ = (digits[i] << 8) - 0x6F80;
    }
    *out = 0;
    return out;
}

/* Select the message panel's bounds by layout mode (2 is the wide panel) and
 * reset its fade channels; nonpositive limits default to 0x80. */
void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *block, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit) {
    switch (panelMode) {
    default:
        block->bounds[0] = -0x390;
        block->bounds[1] = -0x80;
        block->bounds[2] = 0x1A30;
        block->bounds[3] = 0x288;
        block->red = 0x80;
        block->green = 0x80;
        block->blue = 0x80;
        block->fadeLimit = fadeLimit > 0 ? fadeLimit : 0x80;
        break;
    case ITF_MES_PANEL_MODE_STANDARD_VARIANT:
        block->bounds[0] = -0x390;
        block->bounds[1] = -0x80;
        block->bounds[2] = 0x1A30;
        block->bounds[3] = 0x288;
        block->red = 0x80;
        block->green = 0x80;
        block->blue = 0x80;
        block->fadeLimit = fadeLimit > 0 ? fadeLimit : 0x80;
        break;
    case ITF_MES_PANEL_MODE_WIDE:
        block->bounds[0] = -0x610;
        block->bounds[1] = -0x80;
        block->bounds[2] = 0x1CB0;
        block->bounds[3] = 0x288;
        block->red = 0x80;
        block->green = 0x80;
        block->blue = 0x80;
        block->fadeLimit = fadeLimit > 0 ? fadeLimit : 0x80;
        break;
    }
}

void itfMesUpdatePanelFades(instanceMes_tag *panel);
void CalcCurCtr(instanceMes_tag *panel);

void itfMesAdvanceEntryText(instanceMes_tag *mes);

void itfUpdateBattleDisplayAndFadeIndicator(instanceMes_tag *object) {
    itfMesUpdatePanelFades(object);
    itfMesAdvanceEntryText(object);
    itfUpdateSoundSelectionTransition(object);
    CalcCurCtr(object);
}

void itfMesUpdatePanelFades(instanceMes_tag *panel) {
    ItfMesBlkA4 *place = &panel->blkA4;
    itfpanel_t *sprite;
    s32 transition;

    sprite = place->sprite;
    transition = panel->flags & 0x300;
    switch (transition) {
    case 0x100:
        sprite->alpha += 24;
        if (sprite->alpha >= place->fadeLimit ||
            panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            sprite->alpha = place->fadeLimit;
            panel->flags = (panel->flags & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite->alpha -= 8;
        if (sprite->alpha <= 0 || panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            sprite->alpha = 0;
            panel->flags &= ~0x300;
        }
        break;
    }

    sprite = place->frame;
    transition = panel->flags & 0x3000;
    switch (transition) {
    case 0x1000:
        sprite->alpha += 32;
        if (sprite->alpha >= 200) {
            sprite->alpha = 200;
            panel->flags = (panel->flags & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        sprite->alpha -= 32;
        if (sprite->alpha <= 0) {
            sprite->alpha = 0;
            panel->flags &= ~0x3000;
            itfPnlRelease(sprite);
            place->frame = NULL;
        }
        break;
    }

    sprite = place->overlay;
    transition = panel->flags & 0xC00;
    switch (transition) {
    case 0x400:
        sprite->alpha += 24;
        if (sprite->alpha >= place->fadeLimit) {
            sprite->alpha = place->fadeLimit;
            panel->flags = (panel->flags & ~0xC07) | 0x803;
        }
        break;
    case 0xC00:
        sprite->alpha -= 8;
        if (sprite->alpha <= 0) {
            sprite->alpha = 0;
            panel->flags &= ~0xC00;
            itfPnlRelease(sprite);
            place->overlay = NULL;
        }
        break;
    }
}

extern u32 itfMessageFlags;
void itfMesAdvanceEntryText(instanceMes_tag *mes) {
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;
    u32 flags;

    if (entryBlock->textState != 1) {
        return;
    }
    flags = mes->flags;
    if ((flags & 7) == 4) {
        mes->fade.textAdvanceFrameCount++;
        if (mes->flags & 0x800000) {
            entryBlock->textState = -1;
            return;
        }
        if (!(mes->flags & 0x100000)) {
            mes->fade.kind = 1;
        }
        if (!(dds3PadButtonState.confirm < 0 || dds3PadButtonState.primaryCancel < 0 ||
              (dds3PadButtonState.primaryTriangle != 0 && mes->fade.textAdvanceFrameCount >= 5 && !(itfMessageFlags & 1))) &&
              entryBlock->textInterfaceMask != 0) {
            return;
        }
        if (entryBlock->textInterfaceMask != 0) {
            p2sdPlaySE(9, 0x7F, 0x3F);
        }
        InitializeCurCtr(&mes->fade, 0);
        entryBlock->itemIndex++;
        if (entryBlock->itemIndex >= entryBlock->tableCount) {
            entryBlock->textState = -1;
            return;
        }
        CommitVarTable(mes->textSlots.addresses);
        if (mes->temporaryFontEntry != 0) {
            frSetExtDic(mes->temporaryFontEntry);
        }
        itfMesBuildEntryGlyph(mes);
        OpenCheckSpeakerWindow(mes);
        mes->flags = (mes->flags & ~7) | 3;
        return;
    }
    if ((dds3PadButtonState.confirm < 0 || dds3PadButtonState.primaryCancel < 0 || (dds3PadButtonState.primaryTriangle != 0 && !(itfMessageFlags & 1))) &&
        !(mes->flags & 0x1000000) && !(mes->flags & 0x400000)) {
        frFontEnableNodeContextModes(entryBlock->glyphChain);
        if (mes->blk14.glyphChain != NULL) {
            frFontEnableContextMode(mes->blk14.glyphChain);
        }
        flags = mes->flags;
    }
    mes->flags = flags & ~0x1000000;
}

void itfUpdateSoundSelectionTransition(instanceMes_tag *panel) {
    ItfMesBlk40 *selection = &panel->blk40;
    ItfMesEntryBlock *entry = &panel->entryBlock;
    u32 flags = panel->flags;
    s32 top;
    s32 bottom;
    s32 entryY;

    switch (selection->selectionState) {
    case ITF_SOUND_SELECTION_STATE_OPENING:
        if (panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            selection->selectionState = ITF_SOUND_SELECTION_STATE_INTERACTIVE;
            panel->flags = (flags & ~0x38) | 0x18;
            break;
        }
        entryY = entry->y;
        bottom = entryY + entry->lineCount * (21 << 3);
        top = selection->y - ((selection->rowCount * 21 - 21) << 3);
        if (entryY == 0xAF8 && panel->blkA4.sprite != NULL) {
            panel->blkA4.sprite->scrollSpan = ((bottom - top) / 64) * 64 + 64;
        }
        if (entry->glyphChain != NULL && top < bottom) {
            itfMesShiftPanelVertically(panel, -64);
            return;
        }
        if ((flags & 0xC00) != 0x400) {
            if (entry->glyphChain != NULL) {
                ChangeFontColorAll(entry->glyphChain, 3);
            }
            selection->selectionState = ITF_SOUND_SELECTION_STATE_INTERACTIVE;
            panel->flags = (panel->flags & ~0x38) | 0x18;
        }
        break;
    case ITF_SOUND_SELECTION_STATE_INTERACTIVE:
        if ((flags & 0x38) == 0x20 && SelectItem(panel) == ITF_MES_SELECTION_COMMITTED) {
            selection->glyphChain = UnchainFontHandleUnit(selection->glyphChain,
                selection->selectedIndex, selection->rowCount);
            if ((flags & 0xC00) == 0x800) {
                panel->flags |= 0xC00;
            }
            selection->selectedIndex = itfMesFindOptionBitIndex(selection->selectedIndex, selection->panelValue);
            selection->optionCount = 0;
            ForceSetCurCtr(&panel->fade, BTL_FADE_PHASE_ALPHA_FALLING, 0x7F, 0);
            panel->flags = (panel->flags & ~0x38) | 0x28;
            selection->glyphColorIntensity = 0x80;
            selection->selectionState = ITF_SOUND_SELECTION_STATE_SELECTION_FADE;
        }
        break;
    case ITF_SOUND_SELECTION_STATE_SELECTION_FADE:
        selection->glyphColorIntensity -= 16;
        if (selection->glyphColorIntensity <= 0) {
            selection->glyphColorIntensity = 0;
            selection->selectionState = ITF_SOUND_SELECTION_STATE_RETURNING;
            InitializeCurCtr(&panel->fade, 0);
        }
        ChangeFontAlpha(selection->glyphChain, selection->glyphColorIntensity);
        return;
    case ITF_SOUND_SELECTION_STATE_RETURNING:
        if (entry->glyphChain != NULL && entry->y < 0xAF8) {
            itfMesShiftPanelVertically(panel, 64);
            return;
        }
        selection->selectionState = ITF_SOUND_SELECTION_STATE_COMPLETE;
        break;
    }
}

void itfMesShiftPanelVertically(instanceMes_tag *panel, s32 dy) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    pos->y += dy;
    ChangeFontPosition(pos->glyphChain, 0, dy);
    if (place->sprite != 0) {
        itfPnlSetPositionOffset(place->sprite, 0, dy, 0, 0, 0);
    }
    if (place->frame != 0) {
        itfPnlSetPositionOffset(place->frame, 0, dy, 0, dy, 0);
    }
    if (origin->glyphChain != NULL) {
        origin->y += dy;
        ChangeFontPosition(origin->glyphChain, 0, dy);
    }
}

void SelectItemMove(ItfMesBlk40 *obj, s32 dir);

ItfMesSelectionCommitResult SelectItem(instanceMes_tag *panel) {
    ItfMesBlk40 *sel = &panel->blk40;
    s32 dir = 0;
    s32 index;
    if (dds3PadButtonState.prev & 2) {
        if (sel->selectedIndex != 0) {
            dir = -1;
        } else if (dds3PadButtonState.prev < 0) {
            dir = -1;
        }
    } else if (dds3PadButtonState.next & 2) {
        if (sel->selectedIndex != sel->rowCount - 1 || dds3PadButtonState.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        SelectItemMove(sel, dir);
        InitializeCurCtr(&panel->fade, 1);
    }
    if (dds3PadButtonState.confirm < 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        return ITF_MES_SELECTION_COMMITTED;
    }
    if (sel->optionCount > 0 && (index = SelectItemDefKey(sel)) >= 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        if (index != sel->selectedIndex) {
            ChangeFontColor(sel->glyphChain, sel->selectedIndex, sel->rowCount, 0);
            ChangeFontColor(sel->glyphChain, index, sel->rowCount, 1);
            sel->selectedIndex = index;
            sel->savedIndex = index;
        }
        return ITF_MES_SELECTION_COMMITTED;
    }
    return ITF_MES_SELECTION_NOT_COMMITTED;
}

void SelectItemMove(ItfMesBlk40 *obj, s32 dir) {
    s32 cur = obj->selectedIndex;
    u16 maxv;
    ChangeFontColor(obj->glyphChain, cur, obj->rowCount, 0);
    if (dir < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = obj->rowCount - 1;
        }
        maxv = obj->rowCount;
    } else {
        cur = cur + 1;
        if (cur >= obj->rowCount) {
            cur = 0;
        }
        maxv = obj->rowCount;
    }
    ChangeFontColor(obj->glyphChain, cur, (s16)maxv, 1);
    obj->selectedIndex = cur;
    obj->savedIndex = cur;
    p2sdPlaySE(1, 0x7F, 0x3F);
}

s32 SelectItemDefKey(ItfMesBlk40 *selection) {
    s32 i;

    for (i = 0; i < selection->optionCount; i++) {
        ItfMesOption *option = &selection->options[i];

        if (dds3PadButtonState.buttons[option->id] < 0) {
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

void CalcCurCtr(instanceMes_tag *panel) {
    BtlFade *fade = &panel->fade;
    s32 minimumAlpha;
    if (fade->kind != 0) {
        if (fade->timer > 0) {
            fade->timer -= 8;
        }
        switch (fade->phase) {
        case BTL_FADE_PHASE_ALPHA_RISING:
            fade->alpha += 8;
            if (fade->alpha >= 0xFF) {
                fade->phase = BTL_FADE_PHASE_ALPHA_FALLING;
                fade->alpha = 0xFF;
                fade->timer = 0x80;
            }
            break;
        case BTL_FADE_PHASE_ALPHA_FALLING:
            fade->alpha -= 8;
            minimumAlpha = (fade->kind & 1) ? 0x20 : 0x40;
            if (fade->alpha <= minimumAlpha) {
                fade->alpha = minimumAlpha;
                fade->phase = BTL_FADE_PHASE_ALPHA_RISING;
            }
            break;
        }
    }
}

extern void DrawWindow(instanceMes_tag *);
extern void itfDrawSoundSelectorFadeLayers(instanceMes_tag *);
extern void itfDrawSelectedSoundRow(instanceMes_tag *);
extern void itfDrawSoundSelectorOptionPanel(instanceMes_tag *);

/* Draw the panel's glyph layers, advance its selection state and fade. */
void DrawMesFlow(instanceMes_tag *panel) {
    u32 flags = panel->flags;
    ItfMesBlk40 *selection;
    _FRQ *glyph;

    itfMesMngManager.flags &= ~2;
    DrawWindow(panel);
    glyph = panel->blk14.glyphChain;
    if (!(flags & 0x10000) && glyph != 0) {
        frDrawFont(glyph);
    }
    glyph = panel->entryBlock.glyphChain;
    if (!(flags & 0x20000) && (flags & 7) >= 3) {
        if (frDrawFont(glyph) > 0) {
            if ((panel->flags & 7) != 4) {
                panel->fade.textAdvanceFrameCount = 0;
                panel->flags = (panel->flags & ~7) | 4;
            }
        }
    }
    glyph = panel->blk40.glyphChain;
    if (!(flags & 0x40000)) {
        flags &= 0x38;
        if (flags >= 0x18 && frDrawFontEx(glyph, 1) > 0) {
            if (flags == 0x18) {
                selection = &panel->blk40;
                if (selection->selectedIndex == -1) {
                    selection->selectedIndex = 0;
                    selection->savedIndex = 0;
                }
                ChangeFontColor(selection->glyphChain, selection->selectedIndex, selection->rowCount, 1);
                panel->flags = (panel->flags & ~0x38) | 0x20;
                panel->fade.kind = 2;
            }
        }
    }
    if (panel->fade.kind & 1) {
        itfDrawSoundSelectorFadeLayers(panel);
    }
    if (panel->fade.kind & 2) {
        if (panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            itfDrawSoundSelectorOptionPanel(panel);
        } else {
            itfDrawSelectedSoundRow(panel);
        }
    }
}

void DrawWindow(instanceMes_tag *panel) {
    ItfMesBlkA4 *place;
    if (panel->flags & 0x80000) {
        return;
    }
    place = &panel->blkA4;
    if ((panel->flags & 0x300) >= 0x100) {
        if (panel->panelMode != ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            itfPnlDraw(place->sprite, &sdfGrOtTable[panel->drawSurfaceIndex]);
        }
        itfMesMngManager.flags |= 2;
        if (place->overlay != 0) {
            itfPnlDraw(place->overlay, &sdfGrOtTable[panel->drawSurfaceIndex]);
        }
    }
    if (D_00357D80.window != NULL && D_00357D80.window->state == panel && place != 0) {
        DebugDrawWindowLine(place->sprite);
    }
}

extern DrawColorRec D_00357FC8[];
#include "sdf_packet_alloc.h"
extern void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);
extern void itfMiscDrawSpritePrim2(DrawVertex *, DrawColorRec *, DrawColorRec *, u32, s32, sdfDrawTag_t *);
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *, s32, s32);

void itfDrawSelectedSoundRow(instanceMes_tag *panel) {
    DrawColorRec uv;
    DrawColorRec color;
    DrawVertex bounds[2];
    ItfMesBlk40 *selection = &panel->blk40;
    BtlFade *fade = &panel->fade;
    s32 selected = selection->savedIndex;
    sdfDrawTag_t *list;
    s32 x;
    s32 y;
    s32 expansion;
    s32 rightExpansion;
    s32 verticalExpansion;
    sdfGrOt_t *surface;

    if (selected == -1) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    x = selection->x;
    y = (s32)selection->y - ((selection->rowCount * 21 - 21) << 3) + selected * 0xA0;
    bounds[0].x = x - 0x1D0;
    bounds[0].y = y + 0x10;
    bounds[1].x = x + frFontMeasureGlyphStripWidth(selected, selection->glyphChain) + 0x1D0;
    bounds[1].y = bounds[0].y + 0x80;
    itfDrawThreePanelBorderBands(bounds, D_00357FC8, panel->renderValue, 0x1D0, list);

    bounds[0].x = x;
    bounds[0].y = y + 8;
    bounds[1].x = x + 0x60;
    bounds[1].y = y + 0x88;
    uv.components[0] = 0x150;
    uv.components[1] = 0x2F0;
    uv.components[2] = 0x1B0;
    uv.components[3] = 0x3F0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = 0x26;
    itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)&uv, (DrawColorRec *)&color, panel->renderValue,
                                  itfMesMngManager.windowTexture, 0, list);

    bounds[0].x = x - 0xF0;
    bounds[0].y = y + 0x28;
    bounds[1].x = x - 0x30;
    bounds[1].y = y + 0x78;
    uv.components[0] = 0x290;
    uv.components[1] = 0x10;
    uv.components[2] = 0x350;
    uv.components[3] = 0xC0;
    color.components[3] = fade->alpha;
    itfMiscDrawSpritePrim2(bounds, &uv, &color, panel->renderValue, 0, list);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        rightExpansion = expansion << 1;
        verticalExpansion = expansion >> 1;
        bounds[0].x -= expansion;
        bounds[0].y -= verticalExpansion;
        bounds[1].x += rightExpansion;
        bounds[1].y += verticalExpansion;
        color.components[3] = fade->timer;
        itfMiscDrawAlphaPrim(list, 1, 0);
        itfMiscDrawSpritePrim2(bounds, &uv, &color, panel->renderValue, 0, list);
        itfMiscDrawAlphaPrim(list, 0, 0);
    }
    surface = &sdfGrOtTable[panel->drawSurfaceIndex];
    surface->regFunc(surface, list);
}

#ifdef NON_MATCHING
#include "common.h"
#include "itf_debug_panel_work.h"
#include "snd_sequence_volume_pan.h"
#include "snd_pad.h"
#include "itf_mes_window.h"
#include "sdf_packet_list.h"
#include "kernel.h"
#include "dds3_task_create.h"
#include "sdf_heap_api.h"
#include "itf.h"
#include "fr_font_temporary_entry_api.h"
#include "itf_mes_state_api.h"
#include "scr.h"
#include "sdf_sif_command.h"
#include "sdf_projection.h"
#include "itf_panel_draw.h"
#include "itf_panel_api.h"
#include "sdf.h"
#include "sdf_debug_api.h"
#include "sdf_packet_alloc.h"
#include "fr_font_position.h"
#include "fr_font_glyph_chain_count.h"
#include "fr_font_measure.h"

extern ItfMesGlobals itfMesMngManager;
extern sdfGrOt_t sdfGrOtTable[];
extern void itfDrawSoundSelectorOptionPanel(instanceMes_tag *);
extern void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);

extern DrawColorRec itfSoundSelectorBorderColor;
/* Draws the sound selector's option panel border and row highlights behind the
 * selected row: it widens the stored selection extent to the widest glyph
 * strip, draws the three-band border and the two end caps tinted by the panel
 * fade, then one pair of side bars for every row, and queues the packets on the
 * panel's draw surface. Upstream: INCLUDE_ASM in interface/itfMesManager; written
 * from the retail routine, the widest row and each row's top and bottom read
 * once, as retail does. It differs from retail only in register allocation
 * and scheduling (retail spills the fade pointer and several row coordinates
 * to the frame). */
void itfDrawSoundSelectorOptionPanel(instanceMes_tag *panel) {
    DrawColorRec uv;
    DrawColorRec color;
    DrawVertex bounds[2];
    FrFontPosition position;
    ItfMesBlk40 *selection = &panel->blk40;
    BtlFade *fade = &panel->fade;
    s32 selected = selection->savedIndex;
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 i;
    s32 width;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 repeatLeft;
    s32 repeatRight;

    if (selected == -1) {
        return;
    }
    list = (sdfDrawTag_t *)(u32)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    for (i = 0; i < frFontCountGlyphChainNodes(selection->glyphChain); i++) {
        width = frFontMeasureGlyphStripWidth(i, selection->glyphChain);
        if (selection->widestRowWidth < width) {
            frFontGetGlyphPositionAtIndex(&position, i, selection->glyphChain);
            selection->widestRowWidth = width;
            selection->widestRowX = position.x;
        }
    }
    frFontGetGlyphPositionAtIndex(&position, selected, selection->glyphChain);
    left = selection->widestRowX;
    right = left + selection->widestRowWidth;
    repeatLeft = left - 0x250;
    repeatRight = right + 0x250;
    top = position.y + 0x28;
    bottom = position.y + 0xA0;
    bounds[0].x = left - 0x370;
    bounds[0].y = position.y + 0x10;
    bounds[1].x = right + 0x370;
    bounds[1].y = position.y + 0xB8;
    itfDrawThreePanelBorderBands(bounds, &itfSoundSelectorBorderColor, panel->renderValue, 0x370, list);

    bounds[0].x = left - 0x140;
    bounds[0].y = top;
    bounds[1].x = left - 0x60;
    bounds[1].y = bottom;
    uv.components[0] = 0x410;
    uv.components[1] = 0xF0;
    uv.components[2] = 0x4F0;
    uv.components[3] = 0x1E0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = fade->alpha;
    itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);

    bounds[0].x = right + 0x60;
    bounds[0].y = top;
    bounds[1].x = right + 0x140;
    bounds[1].y = bottom;
    uv.components[0] = 0x4F0;
    uv.components[1] = 0xF0;
    uv.components[2] = 0x410;
    uv.components[3] = 0x1E0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = fade->alpha;
    itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);

    for (i = 0; i < frFontCountGlyphChainNodes(selection->glyphChain); i++) {
        frFontGetGlyphPositionAtIndex(&position, i, selection->glyphChain);
        top = position.y + 0x28;
        bottom = position.y + 0xA0;
        bounds[0].x = repeatLeft;
        bounds[0].y = top;
        bounds[1].x = left - 0x120;
        bounds[1].y = bottom;
        uv.components[0] = 0x2C0;
        uv.components[1] = 0xF0;
        uv.components[2] = 0x3F0;
        uv.components[3] = 0x1E0;
        color.components[0] = 0x80;
        color.components[1] = 0x80;
        color.components[2] = 0x80;
        color.components[3] = 0x80;
        itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);

        bounds[0].x = right + 0x120;
        bounds[0].y = top;
        bounds[1].x = repeatRight;
        bounds[1].y = bottom;
        uv.components[0] = 0x3F0;
        uv.components[1] = 0xF0;
        uv.components[2] = 0x2C0;
        uv.components[3] = 0x1E0;
        color.components[0] = 0x80;
        color.components[1] = 0x80;
        color.components[2] = 0x80;
        color.components[3] = 0x80;
        itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);
    }
    surface = &sdfGrOtTable[panel->drawSurfaceIndex];
    surface->regFunc(surface, list);
}
#else
INCLUDE_ASM(const s32, "interface/itfMesManager", itfDrawSoundSelectorOptionPanel);
#endif

extern u8 D_00358008[];
extern u8 D_00357FF8[];
extern s32 D_00358018[];
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *, s32, s32);

/* Draw the sound selector frame, its fade layer and the expanding timer outline. */
void itfDrawSoundSelectorFadeLayers(instanceMes_tag *panel) {
    s32 bounds[4];
    BtlFade *fade = &panel->fade;
    sdfDrawTag_t *packet;
    s32 expansion;
    sdfGrOt_t *surface;

    bounds[0] = 0x1AA0;
    bounds[1] = 0xC60;
    bounds[2] = 0x1BD0;
    bounds[3] = 0xD58;
    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    D_00358018[3] = 0xFF;
    itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)D_00358008, (DrawColorRec *)D_00358018, panel->renderValue,
                                  itfMesMngManager.windowTexture, 0, packet);
    D_00358018[3] = fade->alpha;
    itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)D_00357FF8, (DrawColorRec *)D_00358018, panel->renderValue,
                                  itfMesMngManager.windowTexture, 0, packet);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        bounds[0] -= expansion * 2;
        bounds[1] -= expansion;
        bounds[2] += expansion * 2;
        bounds[3] += expansion;
        D_00358018[3] = fade->timer;
        itfMiscDrawAlphaPrim(packet, 1, 0);
        itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)D_00357FF8, (DrawColorRec *)D_00358018, panel->renderValue,
                                      itfMesMngManager.windowTexture, 0, packet);
        itfMiscDrawAlphaPrim(packet, 0, 0);
    }
    surface = &sdfGrOtTable[panel->drawSurfaceIndex];
    surface->regFunc(surface, packet);
}

dds3TaskUpdateResult_t CalcMsgMng_Process(dds3ProcessID_t *task) {
    mesflow_tag *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        itfUpdateBattleDisplayAndFadeIndicator(node->state);
        node = node->next;
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

extern void DestructFlow(s32 window);
extern u32 dds3NullDrawProc(void);

dds3TaskUpdateResult_t DrawMsgMng_Process(dds3ProcessID_t *task) {
    mesflow_tag *node;

    if (dds3NullDrawProc() != 0) {
        return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
    }
    for (node = itfMesMngManager.pool.activeHead; node != NULL; node = node->next) {
        DrawMesFlow(node->state);
    }
    itfMesMngManager.drawTickCount++;
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

void DrawMsgMng_Shutdown(dds3ProcessID_t *task) {
    mesflow_tag *node = itfMesMngManager.pool.activeHead;
    s32 window;
    while (node != 0) {
        window = node->index;
        node = node->next;
        DestructFlow(window);
    }
    sdfTexFreeDelayed(itfMesMngManager.windowTexture);
    itfMesMngManager.windowTexture = NULL;
}

extern sdfGrOt_t D_00325708;
extern s32 D_00358028[];
extern u8 D_003BB230[5];
extern u8 D_003BB238[5];
extern void itfMiscDrawLineStripPrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

void DebugDrawWindowLine(itfpanel_t *sprite) {
    s32 vertices[4][2] = {
        {sprite->left, sprite->top},
        {sprite->right, sprite->top},
        {sprite->right, sprite->bottom},
        {sprite->left, sprite->bottom}
    };
    sdfDrawTag_t *list;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    itfMiscDrawLineStripPrim((DrawVertex *)vertices, (DrawColorRec *)D_00358028, D_003BB230, D_003BB238, 5, 0xFFFFFF, list);
    D_00325708.regFunc(&D_00325708, list);
}

void DebugMoveWinVert(ItfMesBlkA4 *panel, s32 boundsMode) {
    itfpanel_t *sprite = panel->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = panel->bounds;
        if (dds3PadButtonState.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)dds3PadButtonState.coarseUp << 3) & 0x10;
        }
        if (dds3PadButtonState.secondaryUp & 2) {
            dy = -8;
        } else {
            dy = ((u8)dds3PadButtonState.secondaryDown << 2) & 8;
        }
        if (dds3PadButtonState.secondaryCross != 0) {
            dx *= 8;
            dy *= 8;
        }
        if (dx != 0 || dy != 0) {
            switch (boundsMode) {
            case 0:
                itfPnlSetPositionOffset(sprite, dx, dy, 0, 0, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                break;
            case 1:
                itfPnlSetPositionOffset(sprite, 0, 0, dx, dy, 0);
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            case 2:
                itfPnlSetPositionOffset(sprite, dx, dy, dx, dy, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            }
        }
    }
}

/* Step the panel fade limit by pad input: one per press, ten with the fast modifier held. */
void DebugMoveWinAlpha(ItfMesBlkA4 *panel) {
    s32 step;

    if (dds3PadButtonState.coarseDown & 2) {
        step = -1;
    } else {
        step = (dds3PadButtonState.coarseUp & 2) > 0;
    }
    if (dds3PadButtonState.secondaryUp & 2) {
        step = -1;
    } else if (dds3PadButtonState.secondaryDown & 2) {
        step = 1;
    }
    if (dds3PadButtonState.secondaryCross != 0) {
        step *= 10;
    }
    if (step != 0) {
        s32 alpha = (panel->fadeLimit + step) & 0xFF;

        panel->fadeLimit = alpha;
        if (panel->sprite != NULL) {
            panel->sprite->alpha = alpha;
        }
    }
}

mesflow_tag *DebugChangeInstance(mesflow_tag *node) {
    s32 index = 0;
    s32 count = itfMesMngManager.activeWindowCount;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (node != NULL) {
            node = node->previous;
        }
        if (node == NULL) {
            node = itfMesMngManager.pool.activeTail;
        }
        if ((node->state)->blkA4.sprite != NULL) {
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

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB208);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB210);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB218);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB220);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB228);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB230);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_003BB238);

